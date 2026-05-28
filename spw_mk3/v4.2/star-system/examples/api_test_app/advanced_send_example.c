/**
 * \file advanced_send_example.c
 *
 * \brief Sends a packet out of a channel using the advanced transfer functions.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * The user is prompted for the device to use, the channel to send out of and
 * a series of bytes to be sent in the packet. The packet is sent using the
 * advanced transfer functions.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

#include <stdlib.h>

void advancedSendExample()
{
    /* Pointer to a list of available SpaceWire devices */
    STAR_DEVICE_ID * devices = NULL;

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

        /* Define packet that will hold the packet buffer data */
        STAR_STREAM_ITEM * packet;

        /* Ask user to enter a series of bytes to send */
        printf("\nPlease enter a series of bytes to be transmitted over "
                "SpaceWire:\n");

        /* If valid bytes read input bytes from console */
        if(readBytes(bytes, 256, &readBytesLength))
        {
            /* Create packet */
            packet = STAR_createPacket(NULL, bytes,
                (unsigned int)readBytesLength, STAR_EOP_TYPE_EOP);

            /* If packet was created */
            if(packet)
            {
                /* Create transfer operation */
                STAR_TRANSFER_OPERATION * transferOperation =
                    STAR_createTxOperation(&packet, 1);

                /* If transfer operation was created */
                if(transferOperation)
                {
                    /* Define transfer status for result of transfer
                    operation */
                    STAR_TRANSFER_STATUS status;

                    /* Start transmitting the packet */
                    STAR_submitTransferOperation(selectedChannel,
                        transferOperation);

                    /* Wait for packet to be transmitted */
                    status = STAR_waitOnTransferOperationCompletion(
                        transferOperation, -1);

                    /* Check transfer status */
                    if (status != STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        printf("Unexpected transfer status: %d\n", status);
                    }

                    /* Dispose transfer operation */
                    STAR_disposeTransferOperation(transferOperation);

                    /* Destroy packet after it was sent */
                    STAR_destroyStreamItem(packet);
                }
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
