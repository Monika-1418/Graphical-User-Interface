/**
 * \file transfer_callback_example.c
 *
 * \brief Sends a packet and receives it using a callback function.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Creates a simple packet and sends it using the advanced transmit operations.
 * The packet is received using the transfer completion listener function.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

/* Define receive channel */
STAR_CHANNEL_ID receiveChannel;

/* Initialise transfer complete to false */
unsigned int transferComplete = 0;

void STAR_API_CC TransferCallback(STAR_TRANSFER_OPERATION *pOperation,
    STAR_TRANSFER_STATUS status, void *pContextInfo)
{
    /* Unreferenced parameters */
    UNREFERENCED_PARAMETER(pContextInfo);

    /* Unregister transfer completion listener */
    STAR_unregisterTransferCompletionListener(pOperation, TransferCallback);

    /* If valid stream item was received */
    if(status == STAR_TRANSFER_STATUS_COMPLETE)
    {
        /* Get received stream item */
        STAR_STREAM_ITEM * streamItem = STAR_getTransferItem(pOperation, 0);

        /* Initialise data length to 0 */
        unsigned int dataLength = 0;

        /* Get packet data and its length */
        unsigned char * data = STAR_getPacketData((STAR_SPACEWIRE_PACKET *)
            streamItem->item, &dataLength);

        if (data != NULL)
        {
            /* Prints the packet contents from the receive buffer */
            printPacketContents(data, dataLength);

            /* Destroy packet data now that it has been output */
            STAR_destroyPacketData(data);
        }
    }

    /* Dispose transfer operation */
    STAR_disposeTransferOperation(pOperation);

    /* Close receive channel */
    STAR_closeChannel(receiveChannel);

    /* Set transfer complete to true */
    transferComplete = 1;
}

void transferCallbackExample()
{
    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* If there is a first device */
    if(firstDevice)
    {
        /* Open receive channel */
        receiveChannel = STAR_openChannelToLocalDevice(firstDevice,
            STAR_CHANNEL_DIRECTION_IN, CHANNEL1, 1);

        /* If receive channel was opened */
        if(receiveChannel)
        {
            /* Create transfer operation to receive 1 packet */
            STAR_TRANSFER_OPERATION * receiveTransferOperation =
                STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);

            /* If transfer operation was created */
            if(receiveTransferOperation)
            {
                /* Register transfer completion listener */
                STAR_registerTransferCompletionListener(
                    receiveTransferOperation, TransferCallback, NULL);

                /* Start receiving packet */
                STAR_submitTransferOperation(receiveChannel,
                    receiveTransferOperation);

                /* Transfer completion listener is unregistered in callback
                function */
            }

            /* Receive transfer operation is disposed in callback function */

            /* Receive channel is closed in callback function */
        }
    }

    /* While transfer isn't complete, keep application alive */
    while(!transferComplete)
    {
        /* Sleep to relieve the processor */
        sleep(0);
    }
}
