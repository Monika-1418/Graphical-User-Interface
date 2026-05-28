/**
 * \file transfer_operation_list_send_receive_example.c
 *
 * \brief Transmits send and receive operations in a list and prints the result.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This example assumes that channel 1 (defined in general.h) is connected
 * to link 2 on a SpaceWire USB Brick or other routing device.
 * Any packets sent out of channel 1 will end up coming back to where they
 * started.
 *
 * PCI Mk2       Brick
 * -----------------------
 * Link 1 []-----[] Link 2
 * Link 2 []
 * Link 3 []
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

void transferOperationListSendReceiveExample()
{
    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* If there is a first device */
    if(firstDevice)
    {
        /* Open channel to send out of */
        STAR_CHANNEL_ID channel = STAR_openChannelToLocalDevice(firstDevice,
            STAR_CHANNEL_DIRECTION_INOUT, CHANNEL1, 1);

        /* If channel was opened */
        if(channel)
        {
            /* Create address path */
            unsigned char addressPath[] = {2};

            /* Get address path length */
            U16 addressPathLength = sizeof(addressPath) / sizeof(unsigned char);

            /* Create SpaceWire address */
            STAR_SPACEWIRE_ADDRESS * address = STAR_createAddress(addressPath,
                addressPathLength);

            /* Define packet data to be sent out */
            unsigned char packetData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

            /* Get packet length */
            unsigned int packetDataLength = sizeof(packetData) /
                sizeof(unsigned char);

            /* Create packet */
            STAR_STREAM_ITEM * packet = STAR_createPacket(address, packetData,
                packetDataLength, STAR_EOP_TYPE_EOP);

            /* If packet was created */
            if(packet)
            {
                /* Create send transfer operation */
                STAR_TRANSFER_OPERATION * sendTransferOperation =
                    STAR_createTxOperation(&packet, 1);

                /* Create receive transfer operation */
                STAR_TRANSFER_OPERATION * receiveTransferOperation =
                    STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);

                /* If send and receive transfer operations were created */
                if ((sendTransferOperation != NULL) &&
                    (receiveTransferOperation != NULL))
                {
                    /* Define transfer status */
                    STAR_TRANSFER_STATUS status;

                    /* Put send and receive transfer operations in an array */
                    STAR_TRANSFER_OPERATION * transferOperations[2];
                    transferOperations[0] = sendTransferOperation;
                    transferOperations[1] = receiveTransferOperation;

                    /* Submit transfer operation list */
                    STAR_submitTransferOperationList(channel,
                        transferOperations, 2);

                    /* Wait for send transfer operation to complete */
                    status = STAR_waitOnTransferOperationCompletion(
                        sendTransferOperation, -1);

                    /* If packet was sent successfully */
                    if(status == STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        /* Wait for receive transfer operation to complete */
                        status = STAR_waitOnTransferOperationCompletion(
                            receiveTransferOperation, -1);

                        /* If packet was received successfully */
                        if (status == STAR_TRANSFER_STATUS_COMPLETE)
                        {
                            /* Get received packet */
                            STAR_STREAM_ITEM * streamItem =
                                STAR_getTransferItem(
                                receiveTransferOperation, 0);

                            /* Initialise data length to 0 */
                            unsigned int dataLength = 0;

                            /* Get packet data and its length */
                            unsigned char * data = STAR_getPacketData(
                                (STAR_SPACEWIRE_PACKET *)streamItem->
                                item, &dataLength);

                            /* Check that packet data is valid */
                            if (data)
                            {
                                /* Print received packet */
                                printPacketContents(data, dataLength);

                                /* Destroy packet data now that it has been
                                output */
                                STAR_destroyPacketData(data);
                            }
                        }

                        /* Dispose send transfer operation */
                        STAR_disposeTransferOperation(sendTransferOperation);
                    }

                    /* Dispose receive transfer operation */
                    STAR_disposeTransferOperation(receiveTransferOperation);
                }

                /* Destroy packet */
                STAR_destroyStreamItem(packet);
            }

            /* Close channel */
            STAR_closeChannel(channel);
        }
    }
}
