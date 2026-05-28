/**
 * \file advanced_address_example.c
 *
 * \brief Sends a packet out of a link, which is routed back over the same link.
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

void advancedAddressExample()
{
    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* If there is a first device */
    if(firstDevice)
    {
        /* Open channel to send out of and receive into */
        STAR_CHANNEL_ID channel = STAR_openChannelToLocalDevice(firstDevice,
            STAR_CHANNEL_DIRECTION_OUT, CHANNEL1, 0);

        /* If transmit channel was opened */
        if(channel)
        {
            /* Create address path */
            unsigned char addressPath[] = {2};

            /* Get address path length */
            U16 addressPathLength = sizeof(addressPath) / sizeof(unsigned char);

            /* Create SpaceWire address */
            STAR_SPACEWIRE_ADDRESS * address = STAR_createAddress(addressPath,
                addressPathLength);

            /* If address was created */
            if(address)
            {
                /* Define packet data to be sent out */
                unsigned char packetData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

                /* Get packet length */
                unsigned int packetDataLength = sizeof(packetData) /
                    sizeof(unsigned char);

                /* Create packet */
                STAR_STREAM_ITEM * packet = STAR_createPacket(address,
                    (unsigned char *)packetData, packetDataLength,
                    STAR_EOP_TYPE_EOP);

                /* If packet was created */
                if(packet)
                {
                    /* Create send transfer operation */
                    STAR_TRANSFER_OPERATION * sendTransferOperation =
                        STAR_createTxOperation(&packet, 1);

                    /* If send transfer operation was created */
                    if (sendTransferOperation)
                    {
                        /* Define transfer status */
                        STAR_TRANSFER_STATUS status;

                        /* Start transmitting the packet */
                        STAR_submitTransferOperation(channel,
                            sendTransferOperation);

                        /* Wait indefinitely for transfer to complete */
                        status = STAR_waitOnTransferOperationCompletion(
                            sendTransferOperation, -1);

                        /* Close transmit channel */
                        STAR_closeChannel(channel);

                        /* Open receive channel */
                        channel = STAR_openChannelToLocalDevice(firstDevice,
                            STAR_CHANNEL_DIRECTION_IN, CHANNEL2, 0);

                        /* If receive channel was opened */
                        if(channel)
                        {
                            /* If packet was sent */
                            if(status == STAR_TRANSFER_STATUS_COMPLETE)
                            {
                                /* Create receive transfer operation */
                                STAR_TRANSFER_OPERATION
                                    *receiveTransferOperation =
                                    STAR_createRxOperation(1,
                                    STAR_RECEIVE_PACKETS);

                                /* If receive transfer operation was created */
                                if(receiveTransferOperation)
                                {
                                    /* Start receiving packet */
                                    STAR_submitTransferOperation(channel,
                                        receiveTransferOperation);

                                    /* Wait for packet to be received */
                                    status = STAR_waitOnTransferOperationCompletion
                                        (receiveTransferOperation, -1);

                                    /* If valid packet was received */
                                    if(status == STAR_TRANSFER_STATUS_COMPLETE)
                                    {
                                        /* Get received packet */
                                        STAR_STREAM_ITEM * streamItem =
                                            STAR_getTransferItem(
                                            receiveTransferOperation, 0);

                                        /* Initialise data length to 0 */
                                        unsigned int dataLength = 0;

                                        /* Get packet data and its length */
                                        unsigned char * data =
                                            STAR_getPacketData(
                                            (STAR_SPACEWIRE_PACKET *)streamItem->
                                            item, &dataLength);
                                        if(data != NULL)
                                        {
                                            /* Print received packet */
                                            printPacketContents(data,
                                                dataLength);

                                            /* Destroy packet data now that it
                                            has been output */
                                            STAR_destroyPacketData(data);
                                        }
                                    }

                                    /* Dispose receive transfer operation */
                                    STAR_disposeTransferOperation(
                                        receiveTransferOperation);
                                }
                            }
                        }

                        /* Dispose send transfer operation */
                        STAR_disposeTransferOperation(sendTransferOperation);
                    }

                    /* Destroy packet */
                    STAR_destroyStreamItem(packet);
                }

                /* Dispose address */
                STAR_destroyAddress(address);
            }

            /* Close channel */
            STAR_closeChannel(channel);
        }
    }
}
