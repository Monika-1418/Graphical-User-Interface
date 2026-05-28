/**
 * \file advanced_multiple_packet_example.c
 *
 * \brief Sends an array of packets in a single trasfer operation.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Creates 3 packets, puts them in an array and then transmits the array in a
 * single transfer operation. The packets are then received and printed.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

void advancedMultiplePacketExample()
{
    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* If there is a first device */
    if(firstDevice)
    {
        /* Define EOP type */
        STAR_EOP_TYPE eopType = STAR_EOP_TYPE_EOP;

        /* Create first packet data */
        unsigned char packetData1[] = {1, 2, 3, 4, 5};

        /* Get length of first packet */
        unsigned int packetData1Length = sizeof(packetData1) /
            sizeof(unsigned char);

        /* Create first packet */
        STAR_STREAM_ITEM * packet1 = STAR_createPacket(NULL, packetData1,
            packetData1Length, eopType);

        /* Create second packet data */
        unsigned char packetData2[] = {6, 7, 8, 9, 10};

        /* Get length of second packet */
        unsigned int packetData2Length = sizeof(packetData2) /
            sizeof(unsigned char);

        /* Create second packet */
        STAR_STREAM_ITEM * packet2 = STAR_createPacket(NULL, packetData2,
            packetData2Length, eopType);

        /* Create third packet data */
        unsigned char packetData3[] = {11, 12, 13, 14, 15};

        /* Get length of third packet */
        unsigned int packetData3Length = sizeof(packetData3) /
            sizeof(unsigned char);

        /* Create third packet */
        STAR_STREAM_ITEM * packet3 = STAR_createPacket(NULL, packetData3,
            packetData3Length, eopType);

        /* If valid packets */
        if(packet1 && packet2 && packet3)
        {
            /* Open channel to send packets out of */
            STAR_CHANNEL_ID sendChannel = STAR_openChannelToLocalDevice(
                firstDevice, STAR_CHANNEL_DIRECTION_OUT, CHANNEL1, 0);

            /* If send channel was opened */
            if(sendChannel)
            {
                /* Define send transfer operation for sending packets */
                STAR_TRANSFER_OPERATION * sendTransferOperation;

                /* Wrap individual packets in an array */
                STAR_STREAM_ITEM * streamItems[3];
                streamItems[0] = packet1;
                streamItems[1] = packet2;
                streamItems[2] = packet3;

                /* Create send operation to send packets */
                sendTransferOperation =
                    STAR_createTxOperation(streamItems, 3);

                /* If transfer operation was created */
                if(sendTransferOperation)
                {
                    /* Define transfer status */
                    STAR_TRANSFER_STATUS transmitStatus;

                    /* Start transmitting the packets */
                    STAR_submitTransferOperation(sendChannel,
                        sendTransferOperation);

                    /* Wait for packets to be transmitted */
                    transmitStatus = STAR_waitOnTransferOperationCompletion(
                        sendTransferOperation, -1);

                    /* If packets were transmitted */
                    if(transmitStatus == STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        /* Open receive channel */
                        STAR_CHANNEL_ID receiveChannel =
                            STAR_openChannelToLocalDevice(firstDevice,
                            STAR_CHANNEL_DIRECTION_IN, CHANNEL2, 1);

                        /* If receive channel was opened */
                        if(receiveChannel)
                        {
                            /* Counter for loop */
                            unsigned int index;

                            /* Create transfer operation to receive 1 packet */
                            STAR_TRANSFER_OPERATION * receiveTransferOperation =
                                STAR_createRxOperation(3, STAR_RECEIVE_PACKETS);

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
                                if (receiveStatus ==
                                    STAR_TRANSFER_STATUS_COMPLETE)
                                {
                                    /* Get transfer item count */
                                    unsigned int transferItemCount =
                                        STAR_getTransferItemCount(
                                        receiveTransferOperation);

                                    /* For all transfer items */
                                    for(index = 0; index < transferItemCount;
                                        index++)
                                    {
                                        /* Get received stream item */
                                        STAR_STREAM_ITEM * streamItem =
                                            STAR_getTransferItem(
                                            receiveTransferOperation, index);

                                        /* Initialise data length to 0 */
                                        unsigned int dataLength = 0;

                                        /* Get packet data and its length */
                                        unsigned char * data =
                                            STAR_getPacketData(
                                            (STAR_SPACEWIRE_PACKET *)
                                            streamItem->item, &dataLength);

                                        if (data != NULL)
                                        {
                                            /* Prints the packet contents from
                                            the receive buffer */
                                            printPacketContents(data,
                                                dataLength);

                                            /* Destroy packet data now that it
                                            has been output */
                                            STAR_destroyPacketData(data);
                                        }
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

                    /* Dispose send transfer operation */
                    STAR_disposeTransferOperation(sendTransferOperation);
                }

                /* Close send channel */
                STAR_closeChannel(sendChannel);
            }
        }
    }
}
