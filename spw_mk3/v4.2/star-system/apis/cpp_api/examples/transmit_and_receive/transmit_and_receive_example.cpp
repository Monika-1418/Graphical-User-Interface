/**
 * \file transmit_and_receive_example.cpp
 *
 * \brief Implementation of class(es) which provide examples of performing a
 *        transmit and receive of a packet using the STAR-System C++ API.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Implementation of class(es) which provide examples of performing a transmit
 * and receive of a packet using the STAR-System C++ API, as described in the
 * application note "Transmitting And Receiving Packets Using the
 * STAR-System C++ API"
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "transmit_and_receive_example.h"
#include "star_system.hpp"
#include "device_list.hpp"
#include "device.hpp"
#include "channel.hpp"
#include "packet.hpp"
#include "transmit_operation.hpp"
#include "receive_operation.hpp"
#include "device_selecter.h"
#include <iostream>

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::transferoperations;
using namespace std;

/**
 * Closes transmit and receive channels used by the example.
 * \note The transmit and receive channel may be the same (e.g. on Brick Mk2).
 *
 * @param txChNum the transmit channel number.
 * @param rxChNum the receive channel number.
 * @param pTxCh the transmit channel object to be closed.
 * @param pRxCh the receive channel object to be closed.
 */
void CloseChannels(char txChNum, char rxChNum, Channel *pTxCh, Channel *pRxCh)
{
    if (pTxCh != NULL)
    {
        pTxCh->CloseChannel();
    }

    if (rxChNum != txChNum)
    {
        if (pRxCh != NULL)
        {
            pRxCh->CloseChannel();
        }
    }
}

/**
 * Frees the given Device and Address objects.
 *
 * @param ppDevice pointer to the device to be freed.
 * @param ppAddress pointer to the address to be freed.
 */
void CleanUp(Device **ppDevice, Address **ppAddress)
{
    if (*ppDevice != NULL)
    {
        delete (*ppDevice);
        *ppDevice = NULL;
    }
    if (*ppAddress != NULL)
    {
        delete (*ppAddress);
        *ppAddress = NULL;
    }
}

/**
 * Provides an example of performing a transmit and receive of a packet using
 * the STAR-System C++ API, as described in the application note "Transmitting
 * And Receiving Packets Using the STAR-System C++ API".
 */
void TransmitAndReceiveExample::DoTransmitAndReceive()
{
    Device* pDevice = NULL;
    STAR_DEVICE_TYPE deviceType;
    U32 i;
    char acDeviceType[Device::STR_DEVICE_TYPE_LEN];
    size_t stringLen;
    STAR_CHANNEL_MASK channelMask;
    Channel txChannel;
    Channel rxChannel;
    Packet transmitPacket, *pReceivedPacket;
    StreamItem *pStreamItem;
    TransmitOperation transmitOp;
    ReceiveOperation receiveOp;
    unsigned char transmitBuffer[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    unsigned char *pReceiveBuffer;
    const int transmitBufferLength = sizeof(transmitBuffer);
    TransferOperation::TransferStatus status;
    unsigned int packetCount, receiveBufferLength;
    const unsigned char txChannelNumber = 1;
    unsigned char rxChannelNumber = 2;
    Address *pAddress = NULL;

    /* get pointer to packet required for transmit operation */
    Packet *pTransmitPacket = &transmitPacket;

    /* write header for example to output */
    cout << endl;
    cout << "### STAR-System C++ API - Transmit and Receive Example ###";
    cout << endl;

    /* get a list of all devices present */
    pDevice = DeviceSelector::ChooseDevice();
    if (pDevice == NULL)
    {
        cout << "No device selected" << endl;
        return;
    }

    deviceType = pDevice->GetDeviceType();
    if ((deviceType == STAR_DEVICE_BRICK_MK2) || 
        (deviceType == STAR_DEVICE_ROUTER_MK2S))
    {
        rxChannelNumber = 1; /* device has only 2 channels, 0 and 1 */
    }

    /* get the type of the device as a string */
    stringLen = pDevice->GetDeviceTypeAsString(acDeviceType);
    if (stringLen)
    {
        cout << "Device type = " << acDeviceType << endl;
    }
    else
    {
        cout << "Unable to obtain the device type." << endl;
    }

    /* get the channels on the device */
    channelMask = pDevice->GetDeviceChannels();

    if (rxChannelNumber == txChannelNumber)
    {
        /* check that the channel to use exists on the device */
        if (!(channelMask & (1 << txChannelNumber)))
        {
            cout << "The device in use doesn't have a channel " << txChannelNumber << endl;
            CleanUp(&pDevice, &pAddress);
            return;
        }
    }
    else
    {
        /* check that the channels to use exist on the device */
        if (!(channelMask & (1 << txChannelNumber)))
        {
            cout << "The device in use doesn't have a channel " << txChannelNumber << endl;
            CleanUp(&pDevice, &pAddress);
            return;
        }
        if (!(channelMask & (1 << rxChannelNumber)))
        {
            cout << "The device in use doesn't have a channel " << rxChannelNumber << endl;
            CleanUp(&pDevice, &pAddress);
            return;
        }
    }

    if (rxChannelNumber == txChannelNumber)
    {
        /* open a bidirectional buffered channel to the device */
        txChannel = pDevice->OpenChannel(Channel::DIRECTION_INOUT,
                                            txChannelNumber, 1);
        if (!txChannel.IsOpen())
        {
            cout << "Unable to open a channel to the device." << endl;
            CleanUp(&pDevice,&pAddress);
            return;
        }
        rxChannel = txChannel;

        /* for devices with a single channel, this specifies the tx port */
        unsigned char pPath[] = { 1 };
        pAddress = new Address(pPath, 1);
    }
    else
    {
        /* open a buffered channel to the device to transmit out of */
        txChannel = pDevice->OpenChannel(Channel::DIRECTION_OUT,
                                            txChannelNumber, 1);
        if (!txChannel.IsOpen())
        {
            cout << "Unable to open a channel to the device for transmit." << endl;
            CleanUp(&pDevice, &pAddress);
            return;
        }

        /* open a buffered channel to the device to receive into */
        rxChannel = pDevice->OpenChannel(Channel::DIRECTION_IN,
                                            rxChannelNumber, 1);
        if (!rxChannel.IsOpen())
        {
            txChannel.CloseChannel();

            cout << "Unable to open a channel to the device for receive." << endl;
            CleanUp(&pDevice, &pAddress);
            return;
        }
    }

    /* create a packet to be transmitted, terminated with an EOP */
    if (!transmitPacket.CreatePacket(pAddress, transmitBuffer,
                                     transmitBufferLength,
                                     Packet::EOP_TYPE_EOP))
    {
        cout << "Couldn't create the stream item to be transmitted." << endl;

        /* clean up resources created thus far, prior to exiting function */
        CloseChannels(txChannelNumber, rxChannelNumber, &txChannel, &rxChannel);
        CleanUp(&pDevice, &pAddress);

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
         *       This example makes explicit use of Close, Dispose and Destroy
         *       methods for the purposes of illustration.
         */

        /* exit the function */
        return;
    }

    /* create a transmit operation to transmit the packet */
    if (!transmitOp.CreateTxOperation((StreamItem **)&pTransmitPacket, 1))
    {
        cout << "Couldn't create the transmit operation." << endl;

        /* clean up resources created thus far, prior to exiting function */
        transmitPacket.DestroyPacket();
        CloseChannels(txChannelNumber, rxChannelNumber, &txChannel, &rxChannel);
        CleanUp(&pDevice, &pAddress);

        /* exit the function */
        return;
    }

    /* create a receive operation for receiving one packet */
    if (!receiveOp.CreateRxOperation(1, ReceiveOperation::RECEIVE_PACKETS))
    {
        cout << "Couldn't create receive operation, exiting." << endl;

        /* clean up resources created thus far, prior to exiting function */
        transmitOp.DisposeTransferOperation();
        transmitPacket.DestroyPacket();
        CloseChannels(txChannelNumber, rxChannelNumber, &txChannel, &rxChannel);
        CleanUp(&pDevice, &pAddress);

        /* exit the function */
        return;
    }

    /* start receiving a packet */
    rxChannel.SubmitTransferOperation(&receiveOp);

    /* start transmitting the packet */
    txChannel.SubmitTransferOperation(&transmitOp);

    /* wait indefinitely on a packet being received */
    status = receiveOp.WaitOnTransferOperationCompletion(
                                TransferOperation::WAIT_INDEFINITELY);
    if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        cout << "Could not receive packet, error code: " << status << endl;

        /* clean up resources created thus far, prior to exiting function */
        receiveOp.DisposeTransferOperation();
        transmitOp.DisposeTransferOperation();
        transmitPacket.DestroyPacket();
        CloseChannels(txChannelNumber, rxChannelNumber, &txChannel, &rxChannel);
        CleanUp(&pDevice, &pAddress);

        /* exit the function */
        return;
    }

    /* check there was one packet received
     * (this is a sanity check, and shouldn't be required)
     */
    packetCount = receiveOp.GetTransferItemCount();
    if (packetCount != 1)
    {
        cout << "Unexpected number of packets received: " << packetCount << endl;

        /* clean up resources created thus far, prior to exiting function */
        receiveOp.DisposeTransferOperation();
        transmitOp.DisposeTransferOperation();
        transmitPacket.DestroyPacket();
        CloseChannels(txChannelNumber, rxChannelNumber, &txChannel, &rxChannel);
        CleanUp(&pDevice, &pAddress);

        /* exit the function */
        return;
    }

    /* get the stream item received */
    pStreamItem = receiveOp.GetTransferItem(0);
    if (!pStreamItem)
    {
        cout << "Unable to get the stream item received." << endl;

        /* clean up resources created thus far, prior to exiting function */
        receiveOp.DisposeTransferOperation();
        transmitOp.DisposeTransferOperation();
        transmitPacket.DestroyPacket();
        CloseChannels(txChannelNumber, rxChannelNumber, &txChannel, &rxChannel);
        CleanUp(&pDevice, &pAddress);

        /* exit the function */
        return;
    }

    /* check the stream item is a SpaceWire packet
     * (this is a sanity check, and shouldn�t be required)
     */
    if (pStreamItem->GetStreamItemType() != StreamItem::STREAM_ITEM_TYPE_PACKET)
    {
        cout << "The stream item received is not a packet." << endl;

        /* clean up resources created thus far, prior to exiting function */
        delete pStreamItem;
        receiveOp.DisposeTransferOperation();
        transmitOp.DisposeTransferOperation();
        transmitPacket.DestroyPacket();
        CloseChannels(txChannelNumber, rxChannelNumber, &txChannel, &rxChannel);
        CleanUp(&pDevice, &pAddress);

        /* exit the function */
        return;
    }

    /* cast stream item to packet */
    pReceivedPacket = (Packet*)pStreamItem;

    /* get the bytes of the packet received */
    pReceiveBuffer = pReceivedPacket->GetPacketData(&receiveBufferLength);
    if (!pReceiveBuffer)
    {
        cout << "Unable to get the contents of the packet." << endl;

        /* clean up resources created thus far, prior to exiting function */
        delete pStreamItem;
        receiveOp.DisposeTransferOperation();
        transmitOp.DisposeTransferOperation();
        transmitPacket.DestroyPacket();
        CloseChannels(txChannelNumber, rxChannelNumber, &txChannel, &rxChannel);
        CleanUp(&pDevice, &pAddress);

        /* exit the function */
        return;
    }

    /* display the bytes in the packet */
    cout << "Received packet contents:";
    for (i = 0; i < receiveBufferLength; i++)
    {
        cout << " " << hex << (int)pReceiveBuffer[i];
    }
    cout << "." << endl;

    /* Destroy the buffer containing the received packet */
    /* \note The buffer obtained from Packet.GetPacketData() must always be
     *       freed when no longer required by using Packet.DestroyPacketData(),
     *       if it is a valid buffer, i.e. non-NULL. The Packet object will not
     *       automatically free the buffer obtained in its destructor, e.g.
     *       when it goes out of scope or is deleted. Calls to
     *       Packet.DestroyPacket() will not free the buffer either.
     */
    pReceivedPacket->DestroyPacketData(pReceiveBuffer);

    /* free the stream item retreived from the receive operation */
    delete pStreamItem;

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

    /* close the channels to the device */
    /* \note The Channel object does NOT automatically close the channel it 
     *       refers to when it goes out of scope or is deleted. Therefore, it 
     *       must be closed by the user when no longer required as below:
     */
    CloseChannels(txChannelNumber, rxChannelNumber, &txChannel, &rxChannel);
    CleanUp(&pDevice, &pAddress);
}

