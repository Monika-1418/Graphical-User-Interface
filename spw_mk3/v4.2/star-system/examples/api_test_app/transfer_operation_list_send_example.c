/**
 * \file transfer_operation_list_send_example.c
 *
 * \brief Sends multiple packets using the advanced transmit operations.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This example dynamically creates multiple packets and sends them using the
 * STAR_submitTransferOperationList() function.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

#include <stdlib.h>

void transferOperationListSendExample()
{
    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* If there is a first device */
    if(firstDevice)
    {
        /* Open channel to send out of */
        STAR_CHANNEL_ID channel = STAR_openChannelToLocalDevice(firstDevice,
            STAR_CHANNEL_DIRECTION_OUT, CHANNEL1, 1);

        /* If channel was opened */
        if(channel)
        {
            /* Initialise number of packets to 10 */
            unsigned int numberOfPackets = 10;

            /* Create array to hold transfer operations */
            STAR_TRANSFER_OPERATION **transferOperations =
                (STAR_TRANSFER_OPERATION **)malloc(numberOfPackets *
                sizeof(STAR_TRANSFER_OPERATION *));

            /* Create array to hold packets */
            STAR_STREAM_ITEM **packets = (STAR_STREAM_ITEM **)malloc(
                numberOfPackets * sizeof(STAR_STREAM_ITEM *));

            if ((transferOperations != NULL) && (packets != NULL))
            {
                /* Define counter for loop */
                unsigned char index;

                /* For number of packets */
                for (index = 0; index < numberOfPackets; index++)
                {
                    /* Create packet data with current index */
                    unsigned char packetData[1];
                    packetData[0] = index;

                    {
                        /* Create packet from packet data */
                        STAR_STREAM_ITEM * packet = STAR_createPacket(NULL,
                            packetData, 1, STAR_EOP_TYPE_EOP);

                        /* Create send transfer operation */
                        STAR_TRANSFER_OPERATION * sendTransferOperation =
                            STAR_createTxOperation(&packet, 1);

                        /* Store send transfer operation in array */
                        transferOperations[index] = sendTransferOperation;

                        /* Store packet so that it can be disposed later */
                        packets[index] = packet;
                    }
                }

                /* Start transmitting packets */
                STAR_submitTransferOperationList(channel, transferOperations,
                    numberOfPackets);

                /* For number of packets */
                for (index = 0; index < numberOfPackets; index++)
                {
                    /* Get current transfer operation */
                    STAR_TRANSFER_OPERATION * transferOperation =
                        transferOperations[index];

                    /* Get current packet */
                    STAR_STREAM_ITEM * packet = packets[index];

                    /* Wait for transfer operation to complete */
                    STAR_waitOnTransferOperationCompletion(transferOperation, -1);

                    /* Dispose transfer operation */
                    STAR_disposeTransferOperation(transferOperation);

                    /* Destroy packet */
                    STAR_destroyStreamItem(packet);
                }
            }

            if (packets != NULL)
            {
                /* Free array of packets */
                free(packets);
            }

            if (transferOperations != NULL)
            {
                /* Free array of transfer operations */
                free(transferOperations);
            }

            /* Close channel */
            STAR_closeChannel(channel);
        }
    }
}
