/**
 * \file simple_receive_example.c
 *
 * \brief Receives a packet using the simple receive function.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Prompts the user for the device to use and the channel to receive into. The
 * simple receive function is then used to wait for an incoming packet. The
 * received packet contents are then printed.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

void simpleReceiveExample()
{
    /* Get selected device */
    STAR_DEVICE_ID selectedDevice = promptForDevice(TRANSMIT_TYPE_RECEIVE);

    /* Define selected channel */
    int selectedChannelNumber;

    /* Get selected channel */
    STAR_CHANNEL_ID selectedChannel = promptForChannel(selectedDevice,
        &selectedChannelNumber, TRANSMIT_TYPE_RECEIVE);

    /* If channel was opened */
    if(selectedChannel)
    {
        /* Define receive buffer of maximum packet length */
        unsigned char receiveBuffer[MAX_PACKET_LENGTH];

        /* Initialise receive buffer length to size of receive buffer array */
        unsigned int receiveBufferLength = sizeof(receiveBuffer);

        /* Define eop type */
        STAR_EOP_TYPE eopType;

        /* Define transfer status */
        STAR_TRANSFER_STATUS status;

        /* Print waiting on incoming packet message */
        printf("Waiting on incoming packet...\n");

        /* Receive packet on selected channel */
        status = STAR_receivePacket(selectedChannel, receiveBuffer,
            &receiveBufferLength, &eopType, -1);

        /* If packet was received successfully */
        if(status == STAR_TRANSFER_STATUS_COMPLETE)
        {
            /* Prints the packet contents from the receive buffer */
            printPacketContents(receiveBuffer, receiveBufferLength);
        }
        else
        {
            /* Switch on transfer status */
            switch(status)
            {
            /* Case cancelled */
            case STAR_TRANSFER_STATUS_CANCELLED:
                /* Print error when packet transfer was cancelled */
                printf("The packet was not received because the transfer "
                       "was cancelled.\n");
                break;
            default:
                /* Print unexpected error when receiving packet */
                printf("An unexpected error occurred when receiving the "
                       "packet.\n");
                break;
            }
        }

        /* Close channel after transmitting packet */
        STAR_closeChannel(selectedChannel);
    }
}
