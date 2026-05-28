/**
 * \file transmit_errors_example.c
 *
 * \brief Sends a packet containing a disconnect error then a packet containing
 *        a parity error, out of a channel.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * The user is prompted for the device to use, the channel to send out of and
 * a series of bytes to be sent in the packets. The packet is sent twice, the
 * first time containing a disconnect error in the middle and the second time
 * containing a parity error in the middle.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

#include <stdlib.h>

void transmitErrorsExample()
{
    /* Pointer to a list of available SpaceWire devices */
    STAR_DEVICE_ID *devices = NULL;

    /* Define selected channel */
    int selectedChannelNumber;

    /* Get selected device */
    STAR_DEVICE_ID selectedDevice = promptForDevice(TRANSMIT_TYPE_SEND);

    /* Get selected channel */
    STAR_CHANNEL_ID selectedChannel = promptForChannel(selectedDevice,
        &selectedChannelNumber, TRANSMIT_TYPE_SEND);

    /* If channel was opened */
    if(selectedChannel)
    {
        /* Allocate array of bytes to be populated */
        unsigned char bytes[256];
        U16 readBytesLength = 0;

        /* Length of start of packet to be transmitted */
        U16 packetStartLength;

        /* Define the stream items that will hold the packet and errors */
        STAR_STREAM_ITEM *packetStart, *packetEnd;
        STAR_STREAM_ITEM *disconnectErrorItem, *parityErrorItem;
        STAR_STREAM_ITEM *packet[3];

        /* Ask user to enter a series of bytes to send */
        printf("\nPlease enter a series of bytes to be transmitted over "
                "SpaceWire:\n");

        /* If valid bytes read input bytes from console */
        if(readBytes(bytes, 256, &readBytesLength))
        {
            /* Get length of packet string */
            packetStartLength = readBytesLength / 2;

            /* Create the stream items */
            packetStart = STAR_createDataChunk(bytes,
                packetStartLength, 1, STAR_EOP_TYPE_NONE);
            packetEnd = STAR_createDataChunk(
                bytes + packetStartLength,
                readBytesLength - packetStartLength, 0, STAR_EOP_TYPE_EOP);
            disconnectErrorItem = STAR_createErrorInData(
                STAR_ERROR_IN_DATA_DISCONNECT);
            parityErrorItem = STAR_createErrorInData(STAR_ERROR_IN_DATA_PARITY);

            /* Make packet with from stream items with disconnect error */
            packet[0] = packetStart;
            packet[1] = disconnectErrorItem;
            packet[2] = packetEnd;

            /* If the stream items were created */
            if((packetStart != NULL) && (packetEnd != NULL) &&
                (disconnectErrorItem != NULL) && (parityErrorItem != NULL))
            {
                /* Create transfer operation */
                STAR_TRANSFER_OPERATION *transferOperation =
                    STAR_createTxOperation(packet, 3);

                /* If transfer operation was created */
                if(transferOperation != NULL)
                {
                    /* Define transfer status for result of transfer */
                    /* operation */
                    STAR_TRANSFER_STATUS status;

                    /* Start transmitting the packet */
                    STAR_submitTransferOperation(selectedChannel,
                        transferOperation);

                    /* Wait for packet to be transmitted */
                    status = STAR_waitOnTransferOperationCompletion(
                        transferOperation, -1);

                    /* Dispose transfer operation */
                    STAR_disposeTransferOperation(transferOperation);

                    /* If the transfer operation completed successfully */
                    if (status == STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        /* Change the packet to contain a parity error */
                        packet[1] = parityErrorItem;

                        /* Create transfer operation */
                        transferOperation = STAR_createTxOperation(packet, 3);

                        /* If transfer operation was created */
                        if (transferOperation != NULL)
                        {
                            /* Start transmitting the packet */
                            STAR_submitTransferOperation(selectedChannel,
                                transferOperation);

                            /* Wait for packet to be transmitted */
                            status = STAR_waitOnTransferOperationCompletion(
                                transferOperation, -1);

                            /* Check transfer status */
                            if (status != STAR_TRANSFER_STATUS_COMPLETE)
                            {
                                printf("Unexpected transfer status: %d\n",
                                    status);
                            }

                            /* Dispose transfer operation */
                            STAR_disposeTransferOperation(transferOperation);
                        }
                    }
                    else
                    {
                        /* Unexpected transfer status */
                        printf("Unexpected transfer status: %d\n", status);
                    }
                }
            }

            /* Destroy the stream items */
            if(packetStart != NULL)
            {
                STAR_destroyStreamItem(packetStart);
            }
            if(packetEnd != NULL)
            {
                STAR_destroyStreamItem(packetEnd);
            }
            if(disconnectErrorItem != NULL)
            {
                STAR_destroyStreamItem(disconnectErrorItem);
            }
            if(parityErrorItem != NULL)
            {
                STAR_destroyStreamItem(parityErrorItem);
            }
        }

        /* Close channel after transmitting packet */
        STAR_closeChannel(selectedChannel);
    }
    else
    {
        /* Print error */
        printf("Channel %d could not be opened.\n", selectedChannelNumber);
    }

    /* Destroy device list */
    STAR_destroyDeviceList(devices);
}
