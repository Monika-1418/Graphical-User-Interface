/**
 * \file time-code_example.c
 *
 * \brief Sends a time-code using the advanced transmit operations.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Demonstrates sending a time-code using the advanced transmit operations.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

void timecodeExample()
{
    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* If there is a first device */
    if(firstDevice)
    {
        /* Open send channel */
        STAR_CHANNEL_ID sendChannel = STAR_openChannelToLocalDevice(
            firstDevice, STAR_CHANNEL_DIRECTION_OUT, CHANNEL0, 1);

        /* If send channel was opened */
        if(sendChannel)
        {
            /* Define transfer status for transfer operation status updates */
            STAR_TRANSFER_STATUS status;

            /* Define transfer operation for sending packets */
            STAR_TRANSFER_OPERATION * sendTransferOperation;

            /* Initialise time-code value to 1 */
            U8 timecodeValue = 1;

            /* Create time-code */
            STAR_STREAM_ITEM * timecode = STAR_createTimeCode(timecodeValue);

            /* If time-code was created */
            if(timecode)
            {
                /* Create transmit operation to send time-code */
                sendTransferOperation = STAR_createTxOperation(&timecode, 1);

                /* Start transmitting the time-code */
                STAR_submitTransferOperation(sendChannel,
                    sendTransferOperation);

                /* Wait indefinitely for transfer to complete */
                status = STAR_waitOnTransferOperationCompletion(
                    sendTransferOperation, -1);

                /* If time-code was sent successfully */
                if(status == STAR_TRANSFER_STATUS_COMPLETE)
                {
                    /* Print success message */
                    printf("Time-code sent successfully.\n");
                }

                /* Dispose of transfer operation */
                STAR_disposeTransferOperation(sendTransferOperation);

                /* Destroy time-code */
                STAR_destroyStreamItem(timecode);
            }

            /* Close send channel */
            STAR_closeChannel(sendChannel);
        }
    }
}
