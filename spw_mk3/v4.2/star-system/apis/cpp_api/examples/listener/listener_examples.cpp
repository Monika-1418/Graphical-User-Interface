/**
 * \file listener_examples.cpp
 *
 * \brief Provides examples of registering various types of listeners for
 *        events such as devices being added or removed, channels being opened
 *        or closed, etc.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides examples of registering various types of listeners for events such
 * as devices being added or removed, channels being opened or closed, etc.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "listener_examples.h"
#include "my_device_listener.h"
#include "my_channel_listener.h"
#include "my_driver_listener.h"
#include "my_transfer_completion_listener.h"
#include "star_system.hpp"
#include "packet.hpp"
#include "transmit_operation.hpp"
#include "receive_operation.hpp"
#include "cpp_api_macros.hpp"
#include "utility.h"
#include "device_selecter.h"
#include <iostream>

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::transferoperations;
using namespace std;

/**
 * Enum containing the different possible menu choices for the application.
 */
typedef enum
{
    MENU_CHOICE_REGISTER_CHANNEL_LISTENER = 1,
    MENU_CHOICE_REGISTER_CHANNEL_LISTENER_FOR_DEVICE,
    MENU_CHOICE_REGISTER_CHANNEL_LISTENER_FOR_DRIVER,
    MENU_CHOICE_REGISTER_DEVICE_LISTENER,
    MENU_CHOICE_REGISTER_DEVICE_LISTENER_FOR_DEVICE,
    MENU_CHOICE_REGISTER_DEVICE_LISTENER_FOR_DRIVER,
    MENU_CHOICE_MULTIPLE_REGISTER_DRIVER_LISTENER,
    MENU_CHOICE_REGISTER_TRANSFER_COMPLETION_LISTENER,
    MENU_CHOICE_EXIT,
    MENU_CHOICE_INVALID
} MENU_CHOICE;

/**
 * Provides an example of registering a device listener to detect when
 * devices are added or removed for a specific driver.
 */
void ListenerExamples::Example_RegisterDeviceListenerForDriver()
{
    /* define variables required for test */
    U32 driverCount;
    Driver **pDrivers = NULL;
    MyDeviceListenerCallback deviceCallbackObj("listener for devices being"
                                    " added or removed for a specific driver");
    DeviceListener *pDeviceListener = NULL;
    char s[256];
    char bExit = 0;

    /* write test header */
    cout << "### - TEST: Register a Listener for devices being added or removed"
            " for a specified driver - ###" << endl;

    /* get a list of all drivers present */
    pDrivers = STARSystem::GetDriverList(1, 1, &driverCount);

    /* if drivers obtained */
    if ((driverCount > 0) && (pDrivers != NULL))
    {
        /* register a device listener call-back object to receive notifications
         * of whenever any devices are added or removed for the first driver
         * in the list
         */
        pDeviceListener = STARSystem::RegisterDeviceListenerForDriver(
                                                    pDrivers[0]->GetDriverID(),
                                                    &deviceCallbackObj, 0);

        /* if device listener successfully registered */
        if (pDeviceListener)
        {
            /* until user wishes to exit */
            while (!bExit)
            {
                cout << "\nPlease try adding or removing devices. Enter 'q' when"
                        " you want to quit" << endl;

                if (!fgets(s, 256, stdin))
                {
                    cout << "\nPlease press 'q' when you want to quit" << endl;
                }
                else if ((s[0] == 'q') || (s[0] == 'Q'))
                {
                    bExit = 1;
                }
            }

            /* free listener object obtained */
            /* \note This will automatically unregister the listener too if it
             * hasn't been already, assuming there are no other copies of
             * this DeviceListener object which still exist and which share a
             * reference to the same underlying device listener - in which case
             * the listener will be unregistered when the copy objects go out of
             * scope instead. */
            delete pDeviceListener;
        }
        else
        {
            /* report error */
            cout << "Failed to register device listener for driver" << endl;
        }

        /* free list of drivers obtained */
        for (U32 i=0; i<driverCount; i++)
        {
            if (pDrivers[i] != NULL)
                delete pDrivers[i];
        }
        delete [] pDrivers;

        /* \note The following helper function is also provided to free the
         *       driver list obtained:
         *
         *       STARSystem::DestroyDriverList(pDrivers, driverCount);
         */
    }
    else
    {
        /* report no drivers found */
        cout << "No drivers found" << endl;
    }
}

/**
 * Provides an example of registering a channel listener to detect whenever
 * any channels are opened or closed.
 */
void ListenerExamples::Example_RegisterChannelListener()
{
    /* define variables required for test */
    MyChannelListenerCallback channelListenerCallbackObj("listener for whenever"
                                            " any channels are opened or closed");
    ChannelListener *pChannelListener = NULL;
    unsigned char channelNumber = 1;
    Channel channel;
    Device *pDevice = NULL;

    /* write test header */
    cout << "### - TEST: Register a Listener for any channels being opened or"
            " closed - ###" << endl;

    /* register a channel listener call-back object to receive notifications
     * of whenever any channel is opened or closed
     */
    pChannelListener = STARSystem::RegisterChannelListener(
                                                &channelListenerCallbackObj, 0);

    /* if channel listener failed to register */
    if (!pChannelListener)
    {
        /* report error */
        cout << "Failed to register listener for whenever any channel is opened"
                " or closed" << endl;
    }
    else
    {
        /* get a list of all devices present */
        pDevice = DeviceSelector::ChooseDevice();
        if (pDevice == NULL)
        {
            delete pChannelListener;
            puts("No device selected");
            return;
        }

        /* open channel 1 on the first device in the list */
        channel = pDevice->OpenChannel(Channel::DIRECTION_INOUT,
                                                channelNumber, 1);

        /* if channel opened successfully */
        if (channel.IsOpen())
        {
            /* \todo - do something with the channel */

            /* mimic a consuming task, sleep for 1 second */
            SLEEP(1000);

            /* close the channel after finished using it */
            if (!channel.CloseChannel())
            {
                cout << "Error closing channel" << endl;
            }
        }
        else
        {
            cout << "Failed to open channel 1 on the device" << endl;
        }

        /* free the channel listener object obtained */
        /* \note This will automatically unregister the listener too if it
         * hasn't been already, assuming there are no other copies of
         * this ChannelListener object which still exist and which share a
         * reference to the same underlying channel listener - in which case
         * the listener will be unregistered when the copy objects go out of
         * scope instead. */
        delete pChannelListener;

        /* \note The DeviceList will be automatically freed when it goes out
         *       of scope. You can free it manually if you wish as below:
         *
         *       deviceList.Empty();
         */

        /* free the device object */
        delete pDevice;
    }
}

/**
 * Provides an example of registering a channel listener to detect when
 * channels are opened or closed on a specific device.
 */
void ListenerExamples::Example_RegisterChannelListenerForDevice()
{
    /* define variables required for test */
    ChannelListener *pChannelListenerForDevice = NULL;
    unsigned char channelNumber = 1;
    Channel channel;
    Device* pDevice = NULL;
    MyChannelListenerCallback channelListenerForDeviceCallbackObj("listener for"
                        " channels being opened or closed on a specific device");

    /* write test header */
    cout << "### - TEST: Register a Listener for channels being opened or closed"
            " on a specified device - ###" << endl;

    /* get a list of all devices present */
    pDevice = DeviceSelector::ChooseDevice();
    if (pDevice == NULL)
    {
        puts("No device selected");
        return;
    }

    /* register a channel listener call-back object to receive notifications
     * of whenever any channel is opened or closed on the first device in
     * the list. */
    pChannelListenerForDevice =
        STARSystem::RegisterChannelListenerForDevice(
                                        pDevice->GetDeviceID(),
                                        &channelListenerForDeviceCallbackObj,
                                        0);

    /* if channel listener for device failed to register */
    if (!pChannelListenerForDevice)
    {
        /* report error */
        cout << "Failed to register channel listener for device" << endl;
    }
    else
    {
        /* open channel 1 on the device */
        channel = pDevice->OpenChannel(Channel::DIRECTION_INOUT,
                                                channelNumber, 1);

        /* if channel opened successfully */
        if (channel.IsOpen())
        {
            /* \todo - do something with the channel */

            /* mimic a consuming task, sleep for 1 second */
            SLEEP(1000);

            /* close the channel after finished using it */
            if (!channel.CloseChannel())
            {
                cout << "Error closing channel" << endl;
            }
        }
        else
        {
            cout << "Failed to open channel 1 on the device" << endl;
        }

        /* free the channel listener object obtained */
        /* \note This will automatically unregister the listener too if it
         * hasn't been already, assuming there are no other copies of
         * this ChannelListener object which still exist and which share a
         * reference to the same underlying channel listener - in which case
         * the listener will be unregistered when the copy objects go out of
         * scope instead. */
        delete pChannelListenerForDevice;

        /* \note The DeviceList will be automatically freed when it goes
         *       out of scope. You can free it manually if you wish as
         *       below:
         *
         *       deviceList.Empty();
         */
    }

    /* free the device object */
    delete pDevice;
}

/**
 * Provides an example of registering a channel listener to detect when
 * channels are opened or closed on any device for a specific driver.
 */
void ListenerExamples::Example_RegisterChannelListenerForDriver()
{
    /* define variables required for test */
    U32 driverCount;
    Driver** pDrivers = NULL;
    MyChannelListenerCallback channelCallbackObj("listener for channels being"
                " opened or closed on any device which has a specified driver");
    ChannelListener *pChannelListener = NULL;
    unsigned char channelNumber = 1;
    Channel channel;
    Device* pDevice = NULL;

    /* write test header */
    cout << "### - TEST: Register a Listener for channels being opened or closed"
            " on any device which has a specified driver - ###" << endl;

    /* get a list of all drivers present */
    pDrivers = STARSystem::GetDriverList(1, 1, &driverCount);

    /* if drivers obtained */
    if ((driverCount > 0) && (pDrivers != NULL))
    {
        /* register a channel listener call-back object to receive notifications
         * of whenever any channels are opened or closed on any device which has
         * the first driver the list */
        pChannelListener = STARSystem::RegisterChannelListenerForDriver(
                                                    pDrivers[0]->GetDriverID(),
                                                    &channelCallbackObj, 0);

        /* if channel listener successfully registered */
        if (pChannelListener)
        {
            /* get a list of all devices present */
            pDevice = DeviceSelector::ChooseDevice();
            if (pDevice == NULL)
            {
                delete pChannelListener;
                puts("No device selected");
                return;
            }

            /* open channel 1 on the first device in the list */
            channel = pDevice->OpenChannel(Channel::DIRECTION_INOUT,
                                           channelNumber, 1);

            /* if channel opened successfully */
            if (channel.IsOpen())
            {
                /* \todo - do something with the channel */

                /* mimic a consuming task, sleep for 1 second */
                SLEEP(1000);

                /* close the channel after finished using it */
                if (!channel.CloseChannel())
                {
                    cout << "Error closing channel" << endl;
                }
            }
            else
            {
                cout << "Failed to open channel 1 on the device" << endl;
            }

            /* free listener object obtained */
            /* \note This will automatically unregister the listener too if it
             * hasn't been already, assuming there are no other copies of
             * this ChannelListener object which still exist and which share a
             * reference to the same underlying channel listener - in which case
             * the listener will be unregistered when the copy objects go out of
             * scope instead. */
            delete pChannelListener;

            /* free the device object */
            delete pDevice;
        }
        else
        {
            /* report error */
            cout << "Failed to register channel listener for driver";
        }

        /* free list of drivers obtained */
        for (U32 i=0; i<driverCount; i++)
        {
            if (pDrivers[i] != NULL)
                delete pDrivers[i];
        }
        delete [] pDrivers;

        /* \note The following helper function is also provided to free the
         *       driver list obtained:
         *
         *       STARSystem::DestroyDriverList(pDrivers, driverCount);
         */
    }
    else
    {
        /* report no drivers found */
        cout << "No drivers found" << endl;
    }
}

/**
 * Provides an example of registering a device listener to detect whenever
 * any device is added or removed.
 */
void ListenerExamples::Example_RegisterDeviceListener()
{
    /* define variables required for test */
    MyDeviceListenerCallback deviceCallbackObj("listener for any devices being"
                                    " added or removed");
    DeviceListener *pDeviceListener = NULL;
    char s[256];
    char bExit = 0;

    /* write test header */
    cout << "### - TEST: Register a Listener for any devices being added or"
            " removed - ###" << endl;

    /* register a device listener call-back object to receive notifications
     * of whenever any devices are added or removed
     */
    pDeviceListener = STARSystem::RegisterDeviceListener(&deviceCallbackObj, 0);

    /* if device listener successfully registered */
    if (pDeviceListener)
    {
        /* until user wishes to exit */
        while (!bExit)
        {
            cout << "\nPlease try adding or removing devices. Enter 'q' when"
                    " you want to quit" << endl;

            if (!fgets(s, 256, stdin))
            {
                cout << "\nPlease press 'q' when you want to quit" << endl;
            }
            else if ((s[0] == 'q') || (s[0] == 'Q'))
            {
                bExit = 1;
            }
        }

        /* free listener object obtained */
        /* \note This will automatically unregister the listener too if it
         * hasn't been already, assuming there are no other copies of
         * this DeviceListener object which still exist and which share a
         * reference to the same underlying device listener - in which case
         * the listener will be unregistered when the copy objects go out of
         * scope instead. */
        delete pDeviceListener;
    }
    else
    {
        /* report error */
        cout << "Failed to register device listener!" << endl;
    }
}

/**
 * Provides an example of registering a device listener to detect when
 * a specific device is removed.
 */
void ListenerExamples::Example_RegisterDeviceListenerForDevice()
{
    /* define variables required for test */
    MyDeviceListenerCallback deviceCallbackObj("listener for a specified"
                                                " device being removed");
    DeviceListener *pDeviceListener = NULL;
    char s[256];
    char bExit = 0;
    Device* pDevice = NULL;

    /* write test header */
    cout << "### - TEST: Register a Listener for when a specified device is"
            " removed - ###" << endl;

    /* get a list of all devices present */
    pDevice = DeviceSelector::ChooseDevice();
    if (pDevice == NULL)
    {
        puts("No device selected");
        return;
    }

    /* register a device listener call-back object to receive notifications
     * of when a specified device is removed - in this case, the first
     * device on the list
     */
    pDeviceListener = STARSystem::RegisterDeviceListenerForDevice(
                                                pDevice->GetDeviceID(),
                                                &deviceCallbackObj);

    /* if device listener successfully registered */
    if (pDeviceListener)
    {
        /* until user wishes to exit */
        while (!bExit)
        {
            cout << "\nPlease try removing a device. Enter 'q' when"
                    " you want to quit" << endl;

            if (!fgets(s, 256, stdin))
            {
                cout << "\nPlease press 'q' when you want to quit" << endl;
            }
            else if ((s[0] == 'q') || (s[0] == 'Q'))
            {
                bExit = 1;
            }
        }

        /* free listener object obtained */
        /* \note This will automatically unregister the listener too if it
            * hasn't been already, assuming there are no other copies of
            * this DeviceListener object which still exist and which share a
            * reference to the same underlying device listener - in which case
            * the listener will be unregistered when the copy objects go out of
            * scope instead. */
        delete pDeviceListener;
    }
    else
    {
        /* report error */
        cout << "Failed to register device listener!" << endl;
    }

    /* free the device object */
    delete pDevice;
}

/**
 * Provides an example of registering a driver listener to detect whenever
 * any drivers are added or removed.
 */
void ListenerExamples::Example_RegisterDriverListener()
{
    /* define variables required for test */
    MyDriverListenerCallback driverCallbackObj("listener for any drivers being"
                                    " added or removed");
    DriverListener *pDriverListener = NULL;
    char s[256];
    char bExit = 0;

    /* write test header */
    cout << "### - TEST: Register a Listener for any drivers being added or"
            " removed - ###" << endl;

    /* register a driver listener call-back object to receive notifications
     * of whenever any drivers are added or removed
     */
    pDriverListener = STARSystem::RegisterDriverListener(&driverCallbackObj, 0);

    /* if device listener successfully registered */
    if (pDriverListener)
    {
        /* until user wishes to exit */
        while (!bExit)
        {
            cout << "\nPlease try adding or removing drivers. Enter 'q' when"
                    " you want to quit" << endl;

            if (!fgets(s, 256, stdin))
            {
                cout << "\nPlease press 'q' when you want to quit" << endl;
            }
            else if ((s[0] == 'q') || (s[0] == 'Q'))
            {
                bExit = 1;
            }
        }

        /* free listener object obtained */
        /* \note This will automatically unregister the listener too if it
         * hasn't been already, assuming there are no other copies of
         * this DriverListener object which still exist and which share a
         * reference to the same underlying driver listener - in which case
         * the listener will be unregistered when the copy objects go out of
         * scope instead. */
        delete pDriverListener;
    }
    else
    {
        /* report error */
        cout << "Failed to register driver listener!" << endl;
    }
}

/**
 * Provides an example of registering transfer operation completion listeners
 * to detect when transfer operations complete, e.g. receive and transmit
 * operations.
 */
void ListenerExamples::Example_RegisterTransferCompletionListener()
{
    /* define variables required for test */
    MyTransferCompletionListenerCallback transmitCompletionCallbackObj(
                                            "listener for transmit operation");
    MyTransferCompletionListenerCallback receiveCompletionCallbackObj(
                                            "listener for receive operation");
    TransferCompletionListener *pTransmitCompletionListener = NULL;
    TransferCompletionListener *pReceiveCompletionListener = NULL;
    char s[256];
    Device* pDevice = NULL;
    STAR_DEVICE_TYPE deviceType;
    const unsigned char transmitChannelNumber = 1;
    unsigned char receiveChannelNumber = 2;
    Channel transmitChannel;
    Channel receiveChannel;
    unsigned char transmitBuffer[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa };
    unsigned int transmitBufferLen = sizeof(transmitBuffer);
    unsigned char path[] = { 0x01 };
    unsigned int pathLen = sizeof(path);
    Address packetAddress(path, (U16)pathLen);
    Packet transmitPacket, *pReceivePacket = NULL;
    StreamItem *pStreamItem = NULL;
    int i, result;
    TransmitOperation transmitOp;
    ReceiveOperation receiveOp;
    TransferOperation::TransferStatus status;
    unsigned char *pReceiveBuffer;
    unsigned int receiveBufferLen;

    /* get pointer to packet required for transmit operation */
    Packet *pTransmitPacket = &transmitPacket;

    /* write test header */
    cout << "### - TEST: Register a Listener for when a transfer operation"
            " completes - ###" << endl;

    /* get a list of all devices present */
    cout << "Obtaining devices.." << endl;
    pDevice = DeviceSelector::ChooseDevice();
    if (pDevice == NULL)
    {
        puts("No device selected");
        return;
    }

    /* get device type and receive channel number */
    deviceType = pDevice->GetDeviceType();
    if ((deviceType == STAR_DEVICE_BRICK_MK2) || 
        (deviceType == STAR_DEVICE_ROUTER_MK2S))
    {
        receiveChannelNumber = 1; /* device has only 2 channels, 0 and 1 */
    }

    if (receiveChannelNumber == transmitChannelNumber)
    {
        /* open a bi-directional channel to the first device on the list */
        cout << "Opening channel.." << endl;
        transmitChannel = pDevice->OpenChannel(Channel::DIRECTION_INOUT,
                                             transmitChannelNumber, 1);

        /* if channel failed to open */
        if (!transmitChannel.IsOpen())
        {
            cout << "Failed to open channel!" << endl;
            goto channelOpenFailed;
        }
        receiveChannel = transmitChannel;
    }
    else
    {
        /* open a buffered channel to the device to transmit out of */
        transmitChannel = pDevice->OpenChannel(Channel::DIRECTION_OUT,
                                            transmitChannelNumber, 1);
        if (!transmitChannel.IsOpen())
        {
            cout << "Unable to open a channel to the device for transmit." << endl;
            goto channelOpenFailed;
        }

        /* open a buffered channel to the device to receive into */
        receiveChannel = pDevice->OpenChannel(Channel::DIRECTION_IN,
                                            receiveChannelNumber, 1);
        if (!receiveChannel.IsOpen())
        {
            cout << "Unable to open a channel to the device for receive." << endl;
            goto channelOpenFailed;
        }
    }

    /* create a packet to be transmitted, terminated with an EOP */
    cout << "Creating transmit packet.." << endl;
    if (receiveChannelNumber == transmitChannelNumber)
    {
        result = transmitPacket.CreatePacket(&packetAddress, transmitBuffer,
                                        transmitBufferLen, Packet::EOP_TYPE_EOP);
    }
    else
    {
        result = transmitPacket.CreatePacket((Address*)NULL, transmitBuffer,
                                        transmitBufferLen, Packet::EOP_TYPE_EOP);
    }

    /* if failed to create packet */
    if (!result)
    {
        cout << "Failed to create packet to be transmitted!" << endl;
        goto createPacketFailed;
    }

    /* create a transfer operation to transmit the packet */
    cout << "Creating transmit op for packet.." << endl;
    result = transmitOp.CreateTxOperation((StreamItem **)&pTransmitPacket, 1);

    /* if failed to create transmit op */
    if (!result)
    {
        cout << "Couldn't create transmit operation, exiting." << endl;
        goto createTransmitOpFailed;
    }

    /* register a transfer operation completion listener call-back object to
     * receive notification of when the transmit operation completes - see
     * stages below
     */

    /* store a pointer to the transmit operation in the call-back object which
     * will be registered to receive notification of when it completes
     */
    transmitCompletionCallbackObj.SetTransferOperation(&transmitOp);

    /* register the call-back object as a transfer completion listener */
    pTransmitCompletionListener = 
        STARSystem::RegisterTransferCompletionListener(&transmitOp, 
        &transmitCompletionCallbackObj);

    /* if failed to register listener for transmit op */
    if (!pTransmitCompletionListener)
    {
        cout << "Failed to register transfer operation completion listener for"
                " transmit operation" << endl;
        goto registerTransmitListenerFailed;
    }

    /* create a receive operation, to receive 1 packet */
    cout << "Creating receive op for packet.." << endl;
    result = receiveOp.CreateRxOperation(1, ReceiveOperation::RECEIVE_PACKETS);

    /* if failed to create receive op */
    if (!result)
    {
        cout << "Couldn't create receive operation, exiting." << endl;
        goto createReceiveOpFailed;
    }

    /* register a transfer operation completion listener call-back object to
     * receive notification of when the receive operation completes - see
     * stages below
     */

    /* store a pointer to the receive operation in the call-back object which
     * will be registered to receive notification of when it completes and
     * also the channel on which it was submitted
     */
    receiveCompletionCallbackObj.SetTransferOperation(&receiveOp);
    receiveCompletionCallbackObj.SetChannel(&receiveChannel);

    /* register the call-back object as a transfer completion listener */
    pReceiveCompletionListener = 
        STARSystem::RegisterTransferCompletionListener(&receiveOp, 
        &receiveCompletionCallbackObj);

    /* if failed to register listener for receive op */
    if (!pReceiveCompletionListener)
    {
        cout << "Failed to register transfer operation completion listener for"
                " receive operation" << endl;
        goto registerReceiveListenerFailed;
    }

    /* start transmitting the packet on the channel opened */
    cout << "Packet transmit started." << endl;
    transmitChannel.SubmitTransferOperation(&transmitOp);
    SLEEP(1000);
    printf("Press enter to continue...\n");
    fgets(s, 256, stdin);

    /* start receiving a packet on the channel opened */
    cout << "Packet receive started." << endl;
    receiveChannel.SubmitTransferOperation(&receiveOp);
    SLEEP(1000);
    printf("Press enter to continue...\n");
    fgets(s, 256, stdin);

    /* wait indefinitely for the packet being received */
    cout << "Waiting on receive completion.." << endl;
    status = receiveOp.WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

    /* if status isn't complete */
    if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        cout << "Could not receive packet. Status: " << status << "." << endl;
        goto rxFailed;
    }

    /* check that one packet was received (note: this is a sanity check and
     * shouldn't be required)
     */
    if (receiveOp.GetTransferItemCount() != 1)
    {
        cout << "Unexpected number of packets received: ";
        cout << receiveOp.GetTransferItemCount() << "." << endl;
        goto rxFailed;
    }

    /* get the packet stream item received */
    cout << "Getting the packet received.." << endl;
    pStreamItem = receiveOp.GetTransferItem(0);

    /* if no item received */
    if (pStreamItem == NULL)
    {
        cout << "Unable to get the stream item received" << endl;
        goto rxFailed;
    }

    /* check the item received is a SpaceWire Packet */
    /* \note This is a sanity check and shouldn't be required. */
    if (pStreamItem->GetStreamItemType() !=
        StreamItem::STREAM_ITEM_TYPE_PACKET)
    {
        cout << "Item received is not a Packet!" << endl;
        goto packetTypeCheckFailed;
    }

    /* cast item received to a packet */
    pReceivePacket = (Packet*)pStreamItem;

    /* \note Without sanity checking it may be possible to combine the above
     * steps into one as below:
     *
     * pReceivePacket = (Packet*)pReceiveOp->GetTransferItem(0);
     *
     */

    /* get the data received in the packet */
    cout << "Getting packet data.." << endl;
    pReceiveBuffer = pReceivePacket->GetPacketData(&receiveBufferLen);

    /* if no data received */
    if (!pReceiveBuffer)
    {
        cout << "Unable to get the contents of the packet!" << endl;
    }
    else
    {
        /* display length of received data */
        cout << "Length of received data: " << receiveBufferLen << endl;

        /* display the packet data */
        cout << "Received data:" << endl;
        for (i=0; i<(int)receiveBufferLen; i++)
        {
            cout << " " << hex << (int)pReceiveBuffer[i];
        }
        cout << "" << endl << endl;

        /* free the packet data obtained */
        pReceivePacket->DestroyPacketData(pReceiveBuffer);
    }

    /* clean-up */

packetTypeCheckFailed:
    /* free the stream item received */
    delete pStreamItem;

rxFailed:
    /* free transfer completion listener object obtained for the receive op */
    /* \note This automatically unregisters the listener if it hasn't been
     * already (assuming no copies of the listener object were made which still
     * exist and which share a reference to the same underlying listener - in
     * which case the listener will be unergistered when the copy objects go
     * out of scope instead).
     */
    delete pReceiveCompletionListener;

registerReceiveListenerFailed:
    /* dispose of the receive op */
    /* \note This step may be omitted since the receive op will be
     *       automatically disposed of if required (if it hasn't already been
     *       manually disposed of by the user) when the receive op object goes
     *       out of scope or is deleted.
     */
    receiveOp.DisposeTransferOperation();

createReceiveOpFailed:
    /* free transfer completion listener object obtained for the transit op */
    /* \note This automatically unregisters the listener if it hasn't been
     * already (assuming no copies of the listener object were made which still
     * exist and which share a reference to the same underlying listener - in
     * which case the listener will be unergistered when the copy objects go
     * out of scope instead).
     */
    delete pTransmitCompletionListener;

registerTransmitListenerFailed:
    /* dispose of the transmit op */
    /* \note This step may be omitted since the transmit op will be
     *       automatically disposed of if required (if it hasn't already been
     *       manually disposed of by the user) when the transmit op object goes
     *       out of scope or is deleted.
     */
    transmitOp.DisposeTransferOperation();

createTransmitOpFailed:
    /* destroy transmit packet created */
    /* \note This step may be omitted since the packet will be automatically
     *       destroyed if required (if it hasn't already been manually destroyed
     *       by the user) when the packet object goes out of scope or is
     *       deleted.
     */
    transmitPacket.DestroyPacket();

createPacketFailed:
    /* close the channel opened to send and receive */
    /* \note The Channel object does NOT automatically close the channel it refers
     *       to when it goes out of scope or is deleted. Therefore, it must be
     *       closed by the user when no longer required as below:
     */
    transmitChannel.CloseChannel();

    if (receiveChannelNumber != transmitChannelNumber)
    {
        receiveChannel.CloseChannel();
    }

channelOpenFailed:

    /* free the device object */
    delete pDevice;
}

/**
 * Prompts the user and returns the menu choice that the user selected.
 *
 * @return selected menu choice value.
 */
MENU_CHOICE GetMenuChoice(void)
{
    char buffer[32];
    unsigned int choice;

    /* show menu to user */
    printf("Please select example to run:\n");
    printf("%d. Register channel listener.\n",
        MENU_CHOICE_REGISTER_CHANNEL_LISTENER);
    printf("%d. Register channel listener for device.\n",
        MENU_CHOICE_REGISTER_CHANNEL_LISTENER_FOR_DEVICE);
    printf("%d. Register channel listener for driver.\n",
        MENU_CHOICE_REGISTER_CHANNEL_LISTENER_FOR_DRIVER);
    printf("%d. Register device listener.\n",
        MENU_CHOICE_REGISTER_DEVICE_LISTENER);
    printf("%d. Register device listener for device.\n",
        MENU_CHOICE_REGISTER_DEVICE_LISTENER_FOR_DEVICE);
    printf("%d. Register device listener for driver.\n",
        MENU_CHOICE_REGISTER_DEVICE_LISTENER_FOR_DRIVER);
    printf("%d. Register driver listener.\n",
        MENU_CHOICE_MULTIPLE_REGISTER_DRIVER_LISTENER);
    printf("%d. Register transfer completion listener.\n",
        MENU_CHOICE_REGISTER_TRANSFER_COMPLETION_LISTENER);
    printf("%d. Exit\n", MENU_CHOICE_EXIT);
    printf("> ");

    /* validate chosen number */
    if (!fgets(buffer, 32, stdin)) return MENU_CHOICE_INVALID;

    /* get menu choice value  */
    if (!sscanf(buffer, "%u", &choice) ||
        choice >= MENU_CHOICE_INVALID) return MENU_CHOICE_INVALID;

    /* return the chosen value */
    return (MENU_CHOICE)choice;
}

int ListenerExamples::DoListenerExample()
{
    MENU_CHOICE choice;

    /* print the program header */
    printf("----- Listener Examples -----\n");

    /* loop menu */
    choice = MENU_CHOICE_INVALID;
    do
    {
        choice = GetMenuChoice();
        switch (choice)
        {
            /* time-code on external trigger (not supported on PCIe or PXI
             * Router)
             */
            case MENU_CHOICE_REGISTER_CHANNEL_LISTENER:
                Example_RegisterChannelListener();
                break;
            /* time-code on counter (not supported on PCIe) */
            case MENU_CHOICE_REGISTER_CHANNEL_LISTENER_FOR_DEVICE:
                Example_RegisterChannelListenerForDevice();
                break;
            /* transmit packet on external trigger (not supported on PCIe or
             * PXI Router)
             */
            case MENU_CHOICE_REGISTER_CHANNEL_LISTENER_FOR_DRIVER:
                Example_RegisterChannelListenerForDriver();
                break;
            /* transmit packet on counter */
            case MENU_CHOICE_REGISTER_DEVICE_LISTENER:
                Example_RegisterDeviceListener();
                break;
            /* transmit packet on time-code (not supported on PCIe) */
            case MENU_CHOICE_REGISTER_DEVICE_LISTENER_FOR_DEVICE:
                Example_RegisterDeviceListenerForDevice();
                break;
            /* transmit multiple packets on external trigger
             * (not supported on PCIe or PXI Router)
             */
            case MENU_CHOICE_REGISTER_DEVICE_LISTENER_FOR_DRIVER:
                Example_RegisterDeviceListenerForDriver();
                break;
            /* transmit multiple packets on time-code (not supported on PCIe) */
            case MENU_CHOICE_MULTIPLE_REGISTER_DRIVER_LISTENER:
                Example_RegisterDriverListener();
                break;
            /* timed packet transmit on time-code (not supported on PCIe) */
            case MENU_CHOICE_REGISTER_TRANSFER_COMPLETION_LISTENER:
                Example_RegisterTransferCompletionListener();
                break;

            /* handle unused cases */
            case MENU_CHOICE_INVALID:
                break;
            case MENU_CHOICE_EXIT:
                break;
        }
    }
    while (choice != MENU_CHOICE_EXIT);

    return 0;
}