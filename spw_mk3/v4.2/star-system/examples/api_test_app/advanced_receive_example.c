/**
 * \file advanced_receive_example.c
 *
 * \brief Prompts for a device and channel to receive into and prints output.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * The user is prompted for the device to use and which channel to receive into
 * and then once packets have been received, they are displayed.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

void advancedReceiveExample()
{
    /* Define selected channel number */
    int selectedChannelNumber;

    /* Get selected device */
    STAR_DEVICE_ID selectedDevice = promptForDevice(TRANSMIT_TYPE_RECEIVE);

    /* Get selected channel */
    STAR_CHANNEL_ID selectedChannel = promptForChannel(selectedDevice,
        &selectedChannelNumber, TRANSMIT_TYPE_RECEIVE);

    /* If channel was opened */
    if(selectedChannel)
    {
        /* Create transfer operation to receive 1 packet */
        STAR_TRANSFER_OPERATION * transferOperation = STAR_createRxOperation
            (1, STAR_RECEIVE_PACKETS);

        /* If transfer operation was created */
        if(transferOperation)
        {
            /* Define transfer status */
            STAR_TRANSFER_STATUS status;

            /* Start receiving the packet */
            STAR_submitTransferOperation(selectedChannel, transferOperation);

            /* Print waiting on incoming packet message */
            printf("Waiting on incoming packet...\n");

            /* Wait for packet to be received */
            status = STAR_waitOnTransferOperationCompletion(
                transferOperation, -1);

            /* If packet was received */
            if(status == STAR_TRANSFER_STATUS_COMPLETE)
            {
                /* Get received stream item */
                STAR_STREAM_ITEM * streamItem = STAR_getTransferItem(
                    transferOperation, 0);

                /* If valid stream item was received */
                if(streamItem)
                {
                    /* Initialise data length to 0 */
                    unsigned int dataLength = 0;

                    /* Get packet data and its length */
                    unsigned char * data = STAR_getPacketData(
                        (STAR_SPACEWIRE_PACKET *)streamItem->item,
                        &dataLength);

                    if (data != NULL)
                    {
                        /* Prints the packet contents from the receive buffer */
                        printPacketContents(data, dataLength);

                        /* Destroy packet data now that it has been output */
                        STAR_destroyPacketData(data);
                    }
                }
            }

            /* Dispose transfer operation */
            STAR_disposeTransferOperation(transferOperation);
        }

        /* Close channel after transmitting packet */
        STAR_closeChannel(selectedChannel);
    }
}
