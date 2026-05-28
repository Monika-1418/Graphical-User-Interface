/**
 * \file rmap_initiator_example.cpp
 *
 * \brief Implementation of class(es) which provide an example of implementing
 *        a simple RMAP initiator using the STAR-System C++ API.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Implementation of class(es) which provide an example of implementing a 
 * simple RMAP initiator using the STAR-System C++ API, as described in the 
 * application note "Implementing A Simple RMAP Initiator Using The STAR-System 
 * C++ API".
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */
#include "rmap_initiator_example.h"
#include "device_list.hpp"
#include "device.hpp"
#include "channel.hpp"
#include "packet.hpp"
#include "transmit_operation.hpp"
#include "receive_operation.hpp"
#include "rmap_read_command_packet.hpp"
#include "rmap_packet_library.hpp"
#include "device_selecter.h"
#include <iostream>
using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::transferoperations;
using namespace stardundee::com::starsystem::rmap;
using namespace std;

/**
 * Frees the given Device object.
 *
 * @param ppDevice pointer to the device object to be freed.
 */
void CleanUp(Device **ppDevice)
{
    if (*ppDevice != NULL)
    {
        delete (*ppDevice);
        *ppDevice = NULL;
    }
}

/**
 * Provides an example of implementing a simple RMAP initiator using the
 * STAR-System C++ API, as described in the application note "Implementing A
 * Simple RMAP Initiator Using The STAR-System C++ API".
 */
void RMAPInitiatorExample::DoRMAPInitiatorExample()
{
    Device* pDevice = NULL;
    STAR_CHANNEL_MASK channelMask;
    Channel configChannel;
    Packet transmitPacket, *pReceivePacket;
    TransmitOperation transmitOp;
    ReceiveOperation receiveOp;
    TransferOperation::TransferStatus receiveStatus;
    RMAPReadCommandPacket readCommandPacket;
    RMAPPacket *pRMAPPacket;
    U8 targetAddress[] = {0, 254};
    U8 replyAddress[] = {254};
    const unsigned long targetAddressLength = sizeof(targetAddress);
    const unsigned long replyAddressLength = sizeof(replyAddress);
    const char alignment = 1, incrementAddress = 1;
    const U8 key = 0x20, extendedReadAddress = 0;
    const U16 transactionIdentifier = 0x1234;
    U16 transactionIdentifierReceived;
    const U32 readAddress = 0x106, readLength = 4;
    RMAPPacket::RMAPStatus status;
    U8 *pData;
    const char configChannelNumber = 0;
    U32 dataLength, i;

    /* get pointer to packet required for transmit operation */
    Packet *pTransmitPacket = &transmitPacket;

    /* write header for example to output */
    cout << endl;
    cout << "### STAR-System C++ API - Simple RMAP Initiator Example ###";
    cout << endl;

    /* get a list of all devices present */
    pDevice = DeviceSelector::ChooseDevice();
    if (pDevice == NULL)
    {
        cout << "No device selected" << endl;
        return;
    }

    /* get the channels on the device and check that the channel to be used
     * exists
     */
    channelMask = pDevice->GetDeviceChannels();

    /* check that the channel to use exists on the device */
    if (!(channelMask & (1 << configChannelNumber)))
    {
        cout << "The device in use doesn't have a channel " << configChannelNumber << endl;
        CleanUp(&pDevice);
        return;
    }

    /* open a bidirectional channel to channel 1 of the device */
    configChannel = pDevice->OpenChannel(Channel::DIRECTION_INOUT, configChannelNumber, 1);
    if (!configChannel.IsOpen())
    {
        cout << "Unable to open a channel to the device." << endl;
        return;
    }

    /* build an RMAP read command packet to read 4 bytes */
    if  (!readCommandPacket.CreateReadCommandPacket(targetAddress,
                targetAddressLength, replyAddress, replyAddressLength,
                incrementAddress, key, transactionIdentifier,
                readAddress, extendedReadAddress, readLength, alignment))
    {
        cout << "Couldn't build the read command packet" << endl;

        /* clean up resources created thus far, prior to exiting function */
        configChannel.CloseChannel();
        CleanUp(&pDevice);

        /* \note Some resources (such as transfer operations, and
         *       packets) will automatically dispose of and/or destroy
         *       themselves if required when they go out of scope or are
         *       deleted, if they haven't already been 'manually' disposed of
         *       by the user. Therefore, explicit calls to Dispose and
         *       Destroy methods need not be utilised during clean up stages
         *       and early exit stages if preferred, instead letting the objects
         *       clean after themselves when the function exits and they go out
         *       of scope.
         *
         *       It is worth paying attention to the order in which objects
         *       may be disposed however. For example, if a packet is destroyed
         *       while a transmit operation is still in the process of
         *       attempting to transmit the packet's data it may cause
         *       unexpected side effects.
         *
         *       This example makes explicit use of Dispose and Destroy
         *       methods for the purposes of illustration.
         */

        /* exit the function */
        return;
    }

    /* create a packet stream item for the RMAP read command to be transmitted,
     * terminated with an EOP
     */
    if (!transmitPacket.CreatePacket((Address*)NULL,
                    (unsigned char *)readCommandPacket.GetPacketData(),
                    readCommandPacket.GetPacketLength(), Packet::EOP_TYPE_EOP))
    {
        cout << "Couldn't create the stream item to be transmitted." << endl;

        /* clean up resources created thus far, prior to exiting function */
        readCommandPacket.Dispose();
        configChannel.CloseChannel();
        CleanUp(&pDevice);

        /* exit the function */
        return;
    }

    /* create a transmit operation to transmit the packet */
    if (!transmitOp.CreateTxOperation((StreamItem **)&pTransmitPacket, 1))
    {
        cout << "Couldn't create the transmit operation." << endl;

        /* clean up resources created thus far, prior to exiting function */
        transmitPacket.DestroyPacket();
        readCommandPacket.Dispose();
        configChannel.CloseChannel();
        CleanUp(&pDevice);

        /* exit the function */
        return;
    }

    /* create a receive operation for receiving one packet, the RMAP reply */
    if (!receiveOp.CreateRxOperation(1, ReceiveOperation::RECEIVE_PACKETS))
    {
        cout << "Couldn't create receive operation, exiting." << endl;

        /* clean up resources created thus far, prior to exiting function */
        transmitOp.DisposeTransferOperation();
        transmitPacket.DestroyPacket();
        readCommandPacket.Dispose();
        configChannel.CloseChannel();
        CleanUp(&pDevice);

        /* exit the function */
        return;
    }

    /* start receiving a packet */
    configChannel.SubmitTransferOperation(&receiveOp);

    /* start transmitting the packet */
    configChannel.SubmitTransferOperation(&transmitOp);

    /* wait indefinitely on the reply packet being received */
    receiveStatus = receiveOp.WaitOnTransferOperationCompletion(
                                TransferOperation::WAIT_INDEFINITELY);
    if (receiveStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        cout << "Could not receive packet, error of " << receiveStatus << endl;

        /* clean up resources created thus far, prior to exiting function */
        receiveOp.DisposeTransferOperation();
        transmitOp.DisposeTransferOperation();
        transmitPacket.DestroyPacket();
        readCommandPacket.Dispose();
        configChannel.CloseChannel();
        CleanUp(&pDevice);

        /* exit the function */
        return;
    }

    /* get the packet stream item received */
    pReceivePacket = (Packet*)receiveOp.GetTransferItem(0);

    /* no packet received */
    if (!pReceivePacket)
    {
        cout << "Unable to get the stream item received." << endl;

        /* clean up resources created thus far, prior to exiting function */
        receiveOp.DisposeTransferOperation();
        transmitOp.DisposeTransferOperation();
        transmitPacket.DestroyPacket();
        readCommandPacket.Dispose();
        configChannel.CloseChannel();
        CleanUp(&pDevice);

        /* exit the function */
        return;
    }

    /* extract RMAP packet from the received stream item */
    pRMAPPacket = RMAPPacketLibrary::GetRMAPPacketFromStreamItem(pReceivePacket,
                                                                 1, &status);

    /* free the received packet stream item, now that its contents have been
     * processed
     */
    delete pReceivePacket;

    /* if failed to extract valid RMAP packet from received stream item */
    if (!pRMAPPacket)
    {
        cout << "A valid RMAP packet was not received." << endl;

        /* clean up resources created thus far, prior to exiting function */
        receiveOp.DisposeTransferOperation();
        transmitOp.DisposeTransferOperation();
        transmitPacket.DestroyPacket();
        readCommandPacket.Dispose();
        configChannel.CloseChannel();
        CleanUp(&pDevice);

        /* exit the function */
        return;
    }

    /* check the packet is a read reply */
    if (pRMAPPacket->GetPacketType() != RMAPPacket::RMAP_PACKET_TYPE_READ_REPLY)
    {
        cout << "The packet received is not an RMAP read reply packet." << endl;
    }
    else
    {
        /* check the status in the reply packet is success */
        status = pRMAPPacket->GetStatus();
        if (status != RMAPPacket::RMAP_STATUS_SUCCESS)
        {
            cout << "The reply packet contains an error status." << endl;
        }
        else
        {
            /* Check the transaction identifier is correct */
            transactionIdentifierReceived = pRMAPPacket->GetTransactionID();
            if (transactionIdentifierReceived != transactionIdentifier)
            {
                cout << "The packet received has an unexpected transaction"
                           " identifier." << endl;
                cout << "Expected a value of " << hex << transactionIdentifier
                     << ", received a value of "
                     << hex << transactionIdentifierReceived << endl;
            }
            else
            {
                /* get the data in the reply */
                pData = pRMAPPacket->GetData(&dataLength);

                /* if no data */
                if (!pData)
                {
                    cout << "Unable to get the data in the RMAP read reply"
                                " packet." << endl;
                }
                else
                {
                    if (dataLength != readLength)
                    {
                        cout << "Unexpected data length in the RMAP read"
                                   " reply packet." << endl;
                        cout << "Expected a length of " << dataLength
                             << ", received a length of " << readLength << endl;
                    }

                    /* display the data in the reply */
                    cout << "The data read:";
                    for (i = 0; i < dataLength; i++)
                    {
                        cout << " " << hex << (int)pData[i];
                    }
                    cout << "." << endl;
                }
            }
        }
    }

    /* free the RMAP packet extracted from the received packet data */
    delete pRMAPPacket;

    /* dispose of the receive operation and transmit operation */
    /* \note As mentioned, transfer operation objects (such as ReceiveOperation
     *       and TransmitOperation) will automatically dispose of any transfer
     *       operation they hold if required when they go out of scope or are
     *       deleted. Therefore, the following lines of code may be omitted if
     *       preferred.
     */
    receiveOp.DisposeTransferOperation();
    transmitOp.DisposeTransferOperation();

    /* destroy the transmit packet */
    /* \note The Packet object will automatically destroy the packet it contains
     *       if required when it goes out of scope or is deleted. Therefore,
     *       the following line may be omitted if preferred.
     */
    transmitPacket.DestroyPacket();

    /* free / dispose of the RMAP read command packet */
    /* \note The RMAPReadCommandPacket object will automatically dispose of the
     *       RMAP packet it contains if required when it goes out of scope or
     *       is deleted. Therefore, the following line may be omitted if
     *       preferred.
     */
    readCommandPacket.Dispose();

    /* close the channel to the device */
    /* \note The Channel object does NOT automatically close the channel it refers
     *       to when it goes out of scope or is deleted. Therefore, it must be
     *       closed by the user when no longer required as below:
     */
    configChannel.CloseChannel();

    /* dispose of device object */
    CleanUp(&pDevice);
}

