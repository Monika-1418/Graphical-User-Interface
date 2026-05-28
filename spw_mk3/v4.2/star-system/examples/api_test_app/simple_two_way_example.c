/**
 * \file simple_two_way_example.c
 *
 * \brief Sends and receives packets in both directions using two way channels.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Two packets are constructed and then sent in either direction using two
 * way channels and the simple transmit functions.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

/**
 * Receives a single packet on the specified channel and prints its contents.
 * @param channel the channel to receive packet on.
 */
void receiveSimplePacket(STAR_CHANNEL_ID channel)
{
    /* Define transfer status for receive status information */
    STAR_TRANSFER_STATUS status;

    /* Define receive buffer of maximum packet length */
    unsigned char receiveBuffer[MAX_PACKET_LENGTH];

    /* Initialise receive buffer length to size of receive
    buffer array */
    unsigned int receiveBufferLength = sizeof(receiveBuffer);

    /* Define EOP type */
    STAR_EOP_TYPE eopType = STAR_EOP_TYPE_EOP;

    /* Receive packet and wait indefinitely */
    status = STAR_receivePacket(channel, receiveBuffer,
        &receiveBufferLength, &eopType, -1);

    /* If packet was received */
    if(status == STAR_TRANSFER_STATUS_COMPLETE)
    {
        /* Print packet contents */
        printPacketContents(receiveBuffer, receiveBufferLength);
    }
}

void simpleTwoWayExample()
{
    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* Define packet data to be sent out of first channel */
    unsigned char packetData1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    /* Define packet data to be sent out of second channel */
    unsigned char packetData2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};

    /* If there is a device to use */
    if(firstDevice)
    {
        /* Open first channel to send and receive on */
        STAR_CHANNEL_ID channel1 = STAR_openChannelToLocalDevice(
            firstDevice, STAR_CHANNEL_DIRECTION_INOUT, CHANNEL1, 1);

        /* Open second channel to send and receive on */
        STAR_CHANNEL_ID channel2 = STAR_openChannelToLocalDevice(
            firstDevice, STAR_CHANNEL_DIRECTION_INOUT, CHANNEL2, 1);

        /* If both channels were opened */
        if(channel1 && channel2)
        {
            /* Get packet length */
            unsigned int packetLength = sizeof(packetData1) /
                sizeof(unsigned char);

            /* Define EOP type */
            STAR_EOP_TYPE eopType = STAR_EOP_TYPE_EOP;

            /* Transmit packet out of first channel */
            STAR_TRANSFER_STATUS status = STAR_transmitPacket(channel1,
                packetData1, packetLength, eopType, -1);

            /* If packet was transmitted */
            if(status == STAR_TRANSFER_STATUS_COMPLETE)
            {
                /* Receive packet on second channel */
                receiveSimplePacket(channel2);

                /* Get packet length */
                packetLength = sizeof(packetData2) / sizeof(unsigned char);

                /* Transmit packet out of second channel */
                status = STAR_transmitPacket(channel2, packetData2,
                    packetLength, eopType, -1);

                /* If packet was transmitted */
                if(status == STAR_TRANSFER_STATUS_COMPLETE)
                {
                    /* Receive packet on first channel */
                    receiveSimplePacket(channel1);
                }
            }

            /* Close first channel */
            STAR_closeChannel(channel1);

            /* Close second channel */
            STAR_closeChannel(channel2);
        }
    }
}
