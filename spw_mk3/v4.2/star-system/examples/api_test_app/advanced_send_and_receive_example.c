/**
 * \file advanced_send_and_receive_example.c
 *
 * \brief Sends a packet out of a channel and receives it into another.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * A simple packet is created, sent using a transfer operation out of one
 * channel which is then received on another. The received packet is printed.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

void advancedSendAndReceiveExample()
{
    /* Define transfer status for send and receive status information */
    STAR_TRANSFER_STATUS transmitStatus;

    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* Define packet data to be sent */
    unsigned char packetData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    /* If there is a first device */
    if(firstDevice)
    {
        /* Open send channel */
        STAR_CHANNEL_ID sendChannel = STAR_openChannelToLocalDevice(
            firstDevice, STAR_CHANNEL_DIRECTION_OUT, CHANNEL1, 1);

        /* If send channel was opened */
        if(sendChannel)
        {
            /* Get packet length */
            unsigned int packetLength = sizeof(packetData) /
                sizeof(unsigned char);

            /* Create packet */
            STAR_STREAM_ITEM * packet = STAR_createPacket(NULL,
                (unsigned char *)packetData, packetLength, STAR_EOP_TYPE_EOP);

            /* If packet was created */
            if(packet)
            {
                /* Create transfer operation to send packet */
                STAR_TRANSFER_OPERATION * sendTransferOperation =
                    STAR_createTxOperation(&packet, 1);

                /* If transfer operation was created */
                if(sendTransferOperation)
                {
                    /* Start transmitting the packet */
                    STAR_submitTransferOperation(sendChannel,
                        sendTransferOperation);

                    /* Wait for packet to be transmitted */
                    transmitStatus = STAR_waitOnTransferOperationCompletion(
                        sendTransferOperation, -1);

                    /* Dispose transfer operation */
                    STAR_disposeTransferOperation(sendTransferOperation);

                    /* Destroy packet after it was sent */
                    STAR_destroyStreamItem(packet);

                    /* If packet was sent */
                    if(transmitStatus == STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        /* Open receive channel */
                        STAR_CHANNEL_ID receiveChannel =
                            STAR_openChannelToLocalDevice(firstDevice,
                            STAR_CHANNEL_DIRECTION_IN, CHANNEL2, 1);

                        /* If receive channel was opened */
                        if(receiveChannel)
                        {
                            /* Create transfer operation to receive 1 packet */
                            STAR_TRANSFER_OPERATION * receiveTransferOperation =
                                STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);

                            /* If transfer operation was created */
                            if(receiveTransferOperation)
                            {
                                /* Define transfer status */
                                STAR_TRANSFER_STATUS receiveStatus;

                                /* Start receiving packet */
                                STAR_submitTransferOperation(receiveChannel,
                                    receiveTransferOperation);

                                /* Wait for packet to be received */
                                receiveStatus =
                                    STAR_waitOnTransferOperationCompletion(
                                        receiveTransferOperation, -1);

                                /* If valid stream item was received */
                                if(receiveStatus ==
                                    STAR_TRANSFER_STATUS_COMPLETE)
                                {
                                    /* Get received stream item */
                                    STAR_STREAM_ITEM * streamItem =
                                        STAR_getTransferItem(
                                        receiveTransferOperation, 0);

                                    /* Initialise data length to 0 */
                                    unsigned int dataLength = 0;

                                    /* Get packet data and its length */
                                    unsigned char *data = STAR_getPacketData(
                                        (STAR_SPACEWIRE_PACKET *)streamItem->
                                        item, &dataLength);

                                    if (data != NULL)
                                    {
                                        /* Prints the packet contents from the
                                        receive buffer */
                                        printPacketContents(data, dataLength);

                                        /* Destroy packet data now that it has
                                        been output */
                                        STAR_destroyPacketData(data);
                                    }
                                }

                                /* Dispose receive transfer operation */
                                STAR_disposeTransferOperation(
                                    receiveTransferOperation);
                            }

                            /* Close receive channel */
                            STAR_closeChannel(receiveChannel);
                        }
                    }
                }
            }

            /* Close send channel */
            STAR_closeChannel(sendChannel);
        }
    }
}
