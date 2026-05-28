/**
 * \file advanced_two_way_example.c
 *
 * \brief Sends and receives packets in both directions using two way channels.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Two packets are constructed and then sent in either direction using two
 * way channels and the advanced transfer functions.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

/**
 * Receives a single packet on the specified channel and prints its contents.
 * @param channel the channel to receive packet on.
 */
void receiveAdvancedPacket(STAR_CHANNEL_ID channel)
{
    /* Create transfer operation to receive 1 packet */
    STAR_TRANSFER_OPERATION * receiveTransferOperation =
        STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);

    /* If transfer operation was created */
    if(receiveTransferOperation)
    {
        /* Define transfer status for receive status information */
        STAR_TRANSFER_STATUS status;

        /* Start receiving packet */
        STAR_submitTransferOperation(channel,
            receiveTransferOperation);

        /* Wait for packet to be received */
        status = STAR_waitOnTransferOperationCompletion
            (receiveTransferOperation, -1);

        /* If valid stream item was received */
        if(status == STAR_TRANSFER_STATUS_COMPLETE)
        {
            /* Get received stream item */
            STAR_STREAM_ITEM * streamItem =
                STAR_getTransferItem(
                receiveTransferOperation, 0);

            /* Initialise data length to 0 */
            unsigned int dataLength = 0;

            /* Get packet data and its length */
            unsigned char * data = STAR_getPacketData(
                (STAR_SPACEWIRE_PACKET *)streamItem->
                item, &dataLength);

            if (data != NULL)
            {
                /* Prints the packet contents from the receive buffer */
                printPacketContents(data, dataLength);

                /* Destroy packet data now that it has been output */
                STAR_destroyPacketData(data);
            }
        }

        /* Dispose transfer operation */
        STAR_disposeTransferOperation(receiveTransferOperation);
    }
}

void advancedTwoWayExample()
{
    /* Define transfer status for send and receive status information */
    STAR_TRANSFER_STATUS status;

    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* Define packet data to be sent out of first channel */
    unsigned char packetData1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    /* Define packet data to be sent out of second channel */
    unsigned char packetData2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};

    /* If there is a device to use */
    if(firstDevice)
    {
        /* Open channel to send and receive on */
        STAR_CHANNEL_ID channel1 = STAR_openChannelToLocalDevice(
            firstDevice, STAR_CHANNEL_DIRECTION_INOUT, CHANNEL1, 1);

        /* Open channel to send and receive on */
        STAR_CHANNEL_ID channel2 = STAR_openChannelToLocalDevice(
            firstDevice, STAR_CHANNEL_DIRECTION_INOUT, CHANNEL2, 1);

        /* If both channels were opened */
        if(channel1 && channel2)
        {
            /* Get packet length */
            unsigned int packetLength = sizeof(packetData1) /
                sizeof(unsigned char);

            /* Create packet to send out of first channel */
            STAR_STREAM_ITEM * packet1 = STAR_createPacket(NULL,
                (unsigned char *)packetData1, packetLength, STAR_EOP_TYPE_EOP);

            /* If packet was created */
            if(packet1)
            {
                /* Create transfer operation to send packet out of first
                channel */
                STAR_TRANSFER_OPERATION * sendTransferOperation1 =
                    STAR_createTxOperation(&packet1, 1);

                /* If transfer operation was created */
                if(sendTransferOperation1)
                {
                    /* Start transmitting the packet */
                    STAR_submitTransferOperation(channel1,
                        sendTransferOperation1);

                    /* Wait for packet to be trasmitted */
                    status = STAR_waitOnTransferOperationCompletion(
                        sendTransferOperation1, -1);

                    /* Dispose transfer operation */
                    STAR_disposeTransferOperation(sendTransferOperation1);

                    /* Destroy packet after it was sent */
                    STAR_destroyStreamItem(packet1);

                    /* If packet was sent */
                    if(status == STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        /* Define packet to be sent out of second channel */
                        STAR_STREAM_ITEM * packet2;

                        /* Receive packet on channel 2 */
                        receiveAdvancedPacket(channel2);

                        /* Get packet length */
                        packetLength = sizeof(packetData2) /
                            sizeof(unsigned char);

                        /* Create packet to send out of second channel */
                        packet2 = STAR_createPacket(NULL,
                            (unsigned char *)packetData2, packetLength,
                            STAR_EOP_TYPE_EOP);

                        /* If packet was created */
                        if(packet2)
                        {
                            /* Create transfer operation to send packet out of
                            second channel */
                            STAR_TRANSFER_OPERATION * sendTransferOperation2 =
                                STAR_createTxOperation(&packet2, 1);

                            /* If transfer operation was created */
                            if(sendTransferOperation2)
                            {
                                /* Start transmitting the packet */
                                STAR_submitTransferOperation(channel2,
                                    sendTransferOperation2);

                                /* Wait for packet to be transmitted */
                                status = STAR_waitOnTransferOperationCompletion(
                                    sendTransferOperation2, -1);

                                /* Dispose transfer operation */
                                STAR_disposeTransferOperation(
                                    sendTransferOperation2);

                                /* Destroy packet after it was sent */
                                STAR_destroyStreamItem(packet2);

                                /* If packet was sent */
                                if(status == STAR_TRANSFER_STATUS_COMPLETE)
                                {
                                    /* Receive packet on first channel */
                                    receiveAdvancedPacket(channel1);
                                }
                            }
                        }
                    }
                }
            }
        }

        /* Close first channel */
        STAR_closeChannel(channel1);

        /* Close second channel */
        STAR_closeChannel(channel2);
    }
}
