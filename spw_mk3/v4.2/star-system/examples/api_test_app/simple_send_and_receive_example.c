/**
 * \file simple_send_and_receive_example.c
 *
 * \brief Sends and receives a packet using the simple transmit functions.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Creates a simple packet and sends it out of a channel and receives it into
 * another channel. Uses the simple send and receive functions.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

void simpleSendAndReceiveExample()
{
    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* Define packet data to be sent */
    unsigned char packetData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    /* If there is a first device */
    if(firstDevice)
    {
        /* Open send channel */
        STAR_CHANNEL_ID sendChannel = STAR_openChannelToLocalDevice(
            firstDevice, STAR_CHANNEL_DIRECTION_OUT, CHANNEL1, 1);

        /* If send channel was opened */
        if(sendChannel)
        {
            /* Get packet length */
            unsigned int packetLength = sizeof(packetData) /
                sizeof(unsigned char);

            /* Transmit packet and wait indefinitely */
            STAR_TRANSFER_STATUS status = STAR_transmitPacket(sendChannel,
                packetData, packetLength, STAR_EOP_TYPE_EOP, -1);

            /* Close send channel */
            STAR_closeChannel(sendChannel);

            /* If packet was sent */
            if(status == STAR_TRANSFER_STATUS_COMPLETE)
            {
                /* Open receive channel */
                STAR_CHANNEL_ID receiveChannel = STAR_openChannelToLocalDevice
                    (firstDevice, STAR_CHANNEL_DIRECTION_IN, CHANNEL2, 1);

                /* If receive channel was opened */
                if(receiveChannel)
                {
                    /* Define receive buffer of maximum packet length */
                    unsigned char receiveBuffer[MAX_PACKET_LENGTH];

                    /* Initialise receive buffer length to size of receive
                    buffer array */
                    unsigned int receiveBufferLength = sizeof(receiveBuffer);

                    /* Define EOP type */
                    STAR_EOP_TYPE eopType = STAR_EOP_TYPE_EOP;

                    /* Receive packet and wait indefinitely */
                    status = STAR_receivePacket(receiveChannel, receiveBuffer,
                        &receiveBufferLength, &eopType, -1);

                    /* If packet was received */
                    if(status == STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        /* Print packet contents */
                        printPacketContents(receiveBuffer, receiveBufferLength);
                    }

                    /* Close receive channel */
                    STAR_closeChannel(receiveChannel);
                }
            }
        }
    }
}
