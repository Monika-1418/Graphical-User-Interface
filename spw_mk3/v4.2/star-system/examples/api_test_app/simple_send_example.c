/**
 * \file simple_send_example.c
 *
 * \brief Sends a packet using the simple transmit functions.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Creates a simple packet and sends it out of a channel. Uses the simple
 * send and receive functions.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

#include <stdlib.h>

void simpleSendExample()
{
    /* Pointer to a list of available SpaceWire devices */
    STAR_DEVICE_ID * devices = NULL;

    /* Define transfer status */
    STAR_TRANSFER_STATUS status;

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

        /* Ask user to enter a series of bytes to send */
        printf("\nPlease enter a series of bytes to be transmitted over "
                "SpaceWire:\n");

        /* If valid bytes read input bytes from console */
        if(readBytes(bytes, 256, &readBytesLength))
        {
            /* Send packet out of channel */
            status = STAR_transmitPacket(selectedChannel, bytes,
                (unsigned int)readBytesLength, STAR_EOP_TYPE_EOP, 2);

            /* If packet was transmitted */
            if(status == STAR_TRANSFER_STATUS_COMPLETE)
            {
                /* Print success */
                printf("\nPacket was sent successfully over channel %d.\n",
                    selectedChannelNumber);
            }
            else
            {
                /* Print error */
                printf("\nPacket could not be sent.\n");
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
