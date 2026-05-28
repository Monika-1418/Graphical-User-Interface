/**
 * \file advanced_continuous_receive_example.c
 *
 * \brief Continously waits for 10 packets to be received.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Loops over the receive functions until 10 packets have been received. After
 * a packet is received, the contents are displayed.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

void advancedContinuousReceiveExample()
{
    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* If there is a first device */
    if(firstDevice)
    {
        /* Open receive channel */
        STAR_CHANNEL_ID receiveChannel = STAR_openChannelToLocalDevice(
            firstDevice, STAR_CHANNEL_DIRECTION_IN, CHANNEL1, 1);

        /* Initialise received packet count to 0 */
        unsigned int receivedPacketCount = 0;

        /* Create receive transfer operation */
        STAR_TRANSFER_OPERATION * receiveTransferOperation =
            STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);

        /* While less than 10 packets have been received */
        while(receivedPacketCount < 10)
        {
            /* Define transfer status for receive transfer status updates */
            STAR_TRANSFER_STATUS status;

            /* Start receiving packet */
            STAR_submitTransferOperation(receiveChannel,
                receiveTransferOperation);

            /* Wait for packet to be received */
            status = STAR_waitOnTransferOperationCompletion(
                receiveTransferOperation, -1);

            /* If packet was received */
            if(status == STAR_TRANSFER_STATUS_COMPLETE)
            {
                /* Get received stream item */
                STAR_STREAM_ITEM * streamItem = STAR_getTransferItem(
                    receiveTransferOperation, 0);

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

                /* Increment received packet count */
                receivedPacketCount++;
            }
        }

        /* Dispose receive transfer operation */
        STAR_disposeTransferOperation(receiveTransferOperation);

        /* Close receive channel */
        STAR_closeChannel(receiveChannel);
    }
}
