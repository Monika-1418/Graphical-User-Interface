/**
 * \file timestamp_test.cpp
 *
 * \brief Example test program, exercising the timestamping capabilities of the 
 *        Brick Mk3.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd.
 *
 */

#include "timestamp_test.h"

#include "device_list.hpp"
#include "trigger_factory.hpp"
#include "channel.hpp"
#include "packet.hpp"
#include "brick_mk3_device_configuration.hpp"
#include "brick_mk3_trigger_configuration.hpp"
#include "hardware_info.hpp"
#include "router_global_state.hpp"
#include "transmit_operation.hpp"
#include "receive_operation.hpp"
#include "timestamp_event.hpp"

#include <stdio.h>
#include <vector>

#ifdef _WIN32
    #include "windows.h"

    #define SLEEP(milliseconds) Sleep(milliseconds);
#else
    #include <unistd.h>

    #define SLEEP(milliseconds) usleep(milliseconds * 1000);
#endif

#if !defined(UNREFERENCED_PARAMETER)
    #define UNREFERENCED_PARAMETER(a)   ((void)(a))
#endif

/* Comment this out to use external trigger in example. */
#define GLOBAL_PULSE_GENERATOR

/* Uncomment this to trigger on falling edge of external trigger */
//#define EXTERNAL_TRIGGER_FALLING_EDGE

#ifdef GLOBAL_PULSE_GENERATOR

/* Set sync pulse frequency to 1Hz (global pulse generator supports 1Hz, 
 * 10Hz, 100Hz or 1000Hz).
 */
#define SYNC_PULSE_FREQUENCY 1 /* 1Hz */

#else

/* Set sync pulse frequency to match external trigger pulse generation 
 * frequency.
 */
#define SYNC_PULSE_FREQUENCY 1 /* 1Hz */

#endif

/* Define transmit and receive links */
#define TRANSMIT_LINK 1
#define RECEIVE_LINK 2

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::transferoperations;
using namespace stardundee::com::starsystem::deviceconfig;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices::brickmk3;
using namespace stardundee::com::starsystem::triggering;
using namespace stardundee::com::starsystem::triggering::brickmk3;

/**
 * Returns the first device compatible with timestamping.
 *
 * @return first device compatible with timestamping.
 */
Device* getDevice()
{
    /* the selected device to use for the example */
    Device *pSelectedDevice = NULL;
    size_t stringLen = 0;
    char deviceName[Device::STR_DEVICE_NAME_LEN];

    /* get all Brick Mk3 devices */
    DeviceList deviceList;
    U32 deviceCount = deviceList.PopulateForType(STAR_DEVICE_BRICK_MK3);

    /* if at least one device */
    if(deviceCount > 0)
    {
        U32 compatibleDeviceCount = 0;
        std::vector<Device*> compatibleDeviceVector;
        char s[256];
        int status;
        unsigned int chosen;
        U32 x;

        /* for all devices */
        for(x = 0; x < deviceCount; x++)
        {
            /* get the current device */
            Device *pCurrentDevice = deviceList[x];
            BrickMk3DeviceConfiguration *pDeviceConfiguration = 
                (BrickMk3DeviceConfiguration *)pCurrentDevice->
                GetDeviceConfiguration();

            /* get hardware info for current device */
            HardwareInfo hardwareInfo;
            pDeviceConfiguration->GetHardwareInfo(hardwareInfo);
            delete (pDeviceConfiguration);
            
            /* if Brick Mk3 is v1.02 or later */
            if(hardwareInfo.GetMajor() > 1 || 
                (hardwareInfo.GetMajor() == 1 && hardwareInfo.GetMinor() >= 2))
            {
                compatibleDeviceCount++;
                compatibleDeviceVector.push_back(pCurrentDevice);
            }
        }

        if (compatibleDeviceCount > 0)
        {
            if (compatibleDeviceCount == 1)
            {
                printf("One compatible SpaceWire device detected:");
                pSelectedDevice = (Device*)compatibleDeviceVector[0]->CopyDevice();
            }
            else
            {
                printf("%d compatible SpaceWire devices detected:\n", compatibleDeviceCount);
                for(x = 0; x < compatibleDeviceCount; x++)
                {
                    stringLen = compatibleDeviceVector[x]->GetDeviceName(deviceName);
                    if (stringLen)
                    {
                        /* display it */
                        printf("\t%d - %s\n", x, deviceName);
                    }
                    else
                    {
                        /* display error msg */
                        printf("\t%d - Unknown SpaceWire Device\n", x);
                    }
                }

                printf("Please select which device to open: ");
                if (!fgets(s, 256, stdin))
                {
                    /* display error message */
                    puts("No device number selected.");
                }
                else
                {
                    /* attempt to parse selection entered */
                    status = sscanf(s, "%u", &chosen);

                    /* if failed to read selection entered or invalid selection */
                    if ((!status) || (chosen > deviceCount - 1))
                    {
                        /* display error message */
                        puts("Incorrect device number selected.");
                    }
                    else
                    {
                        /* set selected device to match the user's chosen selection */

                        /* make a clone/copy of the relevant device on the list (the
                         * device list itself has to be freed later)  */
                        pSelectedDevice = (Device*)compatibleDeviceVector[chosen]->CopyDevice();
                    }
                }
            }
        }
    }

    /* return the selected device */
    return pSelectedDevice;
}

/**
 * Prints the given packet data as a series of bytes.
 *
 * @param pPacketData the packet data to be printed.
 * @param packetLength the length of the packet.
 */
void printPacket(unsigned char *pPacketData, unsigned int packetLength)
{
    /* print packet contents */
    unsigned int x;
    for(x = 0; x < packetLength; x++)
    {
        printf("%02X ", pPacketData[x]);
    }
}

/**
 * Transmits the given number of packets over the specified channel.
 *
 * @param pChannel the channel to transmit packets over.
 * @param transmitPacketCount the number of packets to be transmitted.
 */
void transmitPackets(Channel *pChannel, unsigned int transmitPacketCount)
{
    U8 buffer[32];
    unsigned int x;

    /* for all packets to be transmitted */
    for(x = 1; x <= transmitPacketCount; x++)
    {
        /* populate packet buffer */
        memset(buffer, 0xAB, 32);
        memset(buffer, x, 1);

        /* create packet from buffer */
        Packet packet;
        packet.CreatePacket((Address *)NULL, buffer, 32, Packet::EOP_TYPE_EOP);

        /* get pointer to packet required for transmit operation */
        Packet *pPacket = &packet;

        /* create transmit operation from packet */
        TransmitOperation transmitOperation;
        transmitOperation.CreateTxOperation((StreamItem **)&pPacket, 1);

        /* transmit the packet */
        pChannel->SubmitTransferOperation(&transmitOperation);

        /* wait for transmit operation to complete */
        transmitOperation.WaitOnTransferOperationCompletion(-1);

        /* cleanup resources required for transmit operation */
        transmitOperation.DisposeTransferOperation();
        packet.DestroyPacket();

        /* print packet transmit status */
        printf("Packet %d transmitted : ", x);
        printPacket(buffer, 32);
        printf("\n");

        /* sleep for 100 milliseconds */
        SLEEP(100);
    }
}

/**
 * Converts the specified seconds and nanoseconds values into a double.
 *
 * @param seconds the input seconds value.
 * @param nanoseconds the input nanoseconds value.
 *
 * @return value as double.
 */
double convertToDouble(U32 seconds, U32 nanoseconds)
{
    /* convert value to double */
    double doubleValue = (double)seconds + ((double)nanoseconds / 1E9);

    /* return value as double */
    return doubleValue;
}

/**
 * Receives the given number of packets and timestamps over the specified 
 * channel.
 *
 * @param pChannel the channel to receive packets and timestamps on.
 * @param receiveCount the number of packets and timestamps to be received.
 * @param syncPulseFrequency the frequency of the synchronisation pulse.
 */
void receivePacketsAndTimestamps(Channel *pChannel, 
    unsigned int receiveCount, U32 syncPulseFrequency)
{
    unsigned int x;
    unsigned int actualReceiveCount;

    ReceiveOperation receiveOperation;
    
    /* create receive operation for packets and timestamp events */
    receiveOperation.CreateRxOperation(receiveCount,
        (STAR_RECEIVE_MASK)(STAR_RECEIVE_PACKETS | 
        STAR_RECEIVE_TIMESTAMP_EVENTS));
    
    /* submit the receive operation */
    pChannel->SubmitTransferOperation(&receiveOperation);

    /* wait for receive operation to complete */
    receiveOperation.WaitOnTransferOperationCompletion(-1);

    /* get the number of stream items that were received */
    actualReceiveCount = receiveOperation.GetTransferItemCount();

    /* for all stream items that were received */
    for(x = 0; x < actualReceiveCount; x++)
    {
        /* get current stream item */
        StreamItem *pReceivedItem = receiveOperation.GetTransferItem(x);
        if (pReceivedItem != NULL)
        {
            /* if current stream item is a packet */
            if(pReceivedItem->GetStreamItemType() == 
                StreamItem::STREAM_ITEM_TYPE_PACKET)
            {
                unsigned char *pPacketData;
                unsigned int packetLength;

                /* get the packet data */
                Packet *pPacket = (Packet *)pReceivedItem;
                pPacketData = pPacket->GetPacketData(&packetLength);

                /* print the packet data */
                printf("Packet %d received    : ", pPacketData[0]);
                printPacket(pPacketData, packetLength);
                printf("\n");

                /* destroy the packet data now that it has been used */
                Packet::DestroyPacketData(pPacketData);
            }
            /* else if current stream item is a timestamp event */
            else if(pReceivedItem->GetStreamItemType() == 
                StreamItem::STREAM_ITEM_TYPE_TIMESTAMP_EVENT)
            {
                /* get the timestamp data */
                TimestampEvent *pTimestampEvent = (TimestampEvent *)pReceivedItem;

                /* get start and end timestamp values in seconds and a remainder of 
                 * nanoseconds
                 */
                U32 startSeconds;
                U32 startRemainder;
                U32 endSeconds;
                U32 endRemainder;
                pTimestampEvent->GetStartValue(syncPulseFrequency, &startSeconds, 
                    &startRemainder);
                pTimestampEvent->GetEndValue(syncPulseFrequency, &endSeconds, 
                    &endRemainder);

                /* print the start and end of packet timestamps */
                printf("Start of packet timestamp : %f seconds.\n", 
                    convertToDouble(startSeconds, startRemainder));
                printf("End of packet timestamp   : %f seconds.\n\n", 
                    convertToDouble(endSeconds, endRemainder));
            }
            delete pReceivedItem;
        }
    }
}

/**
 * Enables a 1Hz sync pulse on the external trigger.
 *
 * @param pDevice the device to enable external sync pulse for.
 */
void enableExternalSyncPulse(Device *pDevice)
{
    /* The following code uses the STAR-System triggering API to generate a 
     * pulse every second. An SMB lead should be connected between triggers 
     * A and B on the Brick Mk3.
     */
    
    BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration = 
        (BrickMk3TriggerConfiguration *)TriggerFactory::
        CreateTriggerConfiguration(pDevice);
    BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager = 
        (BrickMk3TriggerEventManager *)TriggerFactory::
        CreateTriggerEventManager(pDevice);
    BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager = 
        (BrickMk3TriggerActionManager *)TriggerFactory::
        CreateTriggerActionManager(pDevice);

    /* set timer 0 reload value to 1 second */
    pBrickMk3TriggerConfiguration->SetCounterReloadValue(0, 60000000);

    /* enable timer auto reload */
    pBrickMk3TriggerConfiguration->EnableCounterAutoReload(0);

    /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to 
     * be set
     */
    pBrickMk3TriggerEventManager->SetCounterInputEvents(0, 0, 
        COUNTER_EVENT_RELOAD);

    /* internal trigger 0 causes TIME_CODE_ACTION_TX action on time-code 
     * engine 0
     */
    pBrickMk3TriggerConfiguration->EnableExtTriggerOutput(1);
    pBrickMk3TriggerConfiguration->SetExtTriggerExtend(1, 50);
    pBrickMk3TriggerActionManager->SetExtTriggerOutputActions(1, 0, 
        EXT_TRIGGER_ACTION_OUT);

    /* enable internal trigger 0 */
    pBrickMk3TriggerConfiguration->EnableTrigger(0);
}

/**
 * Disables sync pulse on the external trigger.
 *
 * @param pDevice the device to disable external sync pulse for.
 */
void disableExternalSyncPulse(Device *pDevice)
{
    BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration = 
        (BrickMk3TriggerConfiguration *)TriggerFactory::
        CreateTriggerConfiguration(pDevice);

    /* disable the constant pulse on the external trigger */
    pBrickMk3TriggerConfiguration->DisableExtTriggerOutput(1);
    pBrickMk3TriggerConfiguration->DisableTrigger(0);
}

/**
 * Configures timestamp functionality for either global pulse generator or 
 * external trigger synchronised timestamps. When GLOBAL_PULSE_GENERATOR is 
 * defined, the global pulse generator is used and when GLOBAL_PULSE_GENERATOR 
 * is not defined, the external trigger is used.
 *
 * @param pDevice the device to perform timestamp test on.
 */
void timestampTest(Device *pDevice)
{
    Channel transmitChannel;
    Channel receiveChannel;

    BrickMk3DeviceConfiguration *pDeviceConfiguration = 
        (BrickMk3DeviceConfiguration *)pDevice->GetDeviceConfiguration();

    /* transmit 10 packets */
    unsigned int transmitPacketCount = 10;

    printf("Setting up timestamp system...\n");

    /* ensure that timestamp counters are disabled before configuring */
    pDeviceConfiguration->SetTimestampMethod(
        STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD_NONE);

    /* set initial timestamp value of 10 seconds */
    pDeviceConfiguration->SetTimestampValue(10);

    /* enable timestamp events on port 2 */
    pDeviceConfiguration->EnableRxTimestampEventsOnPort(RECEIVE_LINK);

#ifdef GLOBAL_PULSE_GENERATOR

#if SYNC_PULSE_FREQUENCY == 1
    
    /* set pulse generator frequency to 1Hz */
    pDeviceConfiguration->SetPulseGeneratorFrequency(
        STAR_CFG_BRICK_MK3_PULSE_FREQ_1);

#elif SYNC_PULSE_FREQUENCY == 10

    /* set pulse generator frequency to 10Hz */
    pDeviceConfiguration->SetPulseGeneratorFrequency(
        STAR_CFG_BRICK_MK3_PULSE_FREQ_10);

#elif SYNC_PULSE_FREQUENCY == 100

    /* set pulse generator frequency to 100Hz */
    pDeviceConfiguration->SetPulseGeneratorFrequency(
        STAR_CFG_BRICK_MK3_PULSE_FREQ_100);

#elif SYNC_PULSE_FREQUENCY == 1000

    /* set pulse generator frequency to 1KHz */
    pDeviceConfiguration->SetPulseGeneratorFrequency(
        STAR_CFG_BRICK_MK3_PULSE_FREQ_1000);

#else

    /* print error when unexpected global pulse frequency is detected  */
    printf("ERROR: Unexpected global pulse frequency of %dHz (valid values are: 1Hz, 10Hz, 100Hz, 1000Hz).\n");
    return;

#endif
    
    /* enable global pulse generator */
    pDeviceConfiguration->SetTimestampMethod(
        STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD_PULSE_GENERATOR);

#else

    /* enable external trigger pulse detection */
    pDeviceConfiguration->SetTimestampMethod(
        STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD_EXTERNAL_TRIGGER);

    /* enable the external sync pulse before transmitting packets */
    enableExternalSyncPulse(pDevice);

#endif

    /* create transmit and receive channels */
    transmitChannel = pDevice->OpenChannel(Channel::DIRECTION_OUT, 
        TRANSMIT_LINK, 1);
    receiveChannel = pDevice->OpenChannel(Channel::DIRECTION_IN, 
        RECEIVE_LINK, 1);

    /* sleep to allow timer to run before transmitting packets */
    SLEEP(2000);

    /* transmit a group of packets */
    transmitPackets(&transmitChannel, transmitPacketCount);

    /* receive the packets and timestamps */
    receivePacketsAndTimestamps(&receiveChannel, transmitPacketCount * 2, 
        SYNC_PULSE_FREQUENCY);

#ifndef GLOBAL_PULSE_GENERATOR

    /* disable the external sync pulse after receiving packets and timestamps */
    disableExternalSyncPulse(pDevice);

#endif

    /* disable timestamps after receiving packets and timestamps */
    pDeviceConfiguration->SetTimestampMethod(
        STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD_NONE);

    /* close the channels */
    transmitChannel.CloseChannel();
    receiveChannel.CloseChannel();

    /* free the device configuration object */
    delete pDeviceConfiguration;
}

int TimestampTest::DoTimestampTest()
{
    /* print the program header */
    printf("----- Timestamping API Example -----\n\n");

    /* select device to use in examples */
    Device *pDevice = getDevice();

    /* if compatible device was found */
    if(pDevice != NULL)
    {
        BrickMk3DeviceConfiguration *pDeviceConfiguration = 
            (BrickMk3DeviceConfiguration *)pDevice->GetDeviceConfiguration();
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration = 
            (BrickMk3TriggerConfiguration *)TriggerFactory::
            CreateTriggerConfiguration(pDevice);

        char deviceName[Device::STR_DEVICE_NAME_LEN];
        char deviceSerial[Device::STR_DEVICE_SERIAL_LEN];

        /* get device name and serial */
        pDevice->GetDeviceName(deviceName);
        pDevice->GetDeviceSerialNumber(deviceSerial);

        /* print selected serial */
        printf("%s with serial %s selected.\n\n", deviceName, deviceSerial);

        /* disable router timeouts so that timestamps are not blocked */
        RouterGlobalState *pRouterGlobalState = 
            pDeviceConfiguration->GetGlobalSettings();
        pRouterGlobalState->SetTimeoutMode(
            RouterGlobalState::TIMEOUT_MODE_BLOCKING);
        pDeviceConfiguration->SetGlobalSettings(*pRouterGlobalState);

        /* timestamping requires interface mode to be enabled */
        pDeviceConfiguration->EnableInterfaceMode();

        /* start the link since watchdog timer is disabled */
        pDeviceConfiguration->StartLink(1);

#ifdef EXTERNAL_TRIGGER_FALLING_EDGE
        /* trigger on falling edge */
        pBrickMk3TriggerConfiguration->EnableExtTriggerEdgeDetectMode(1);
        pBrickMk3TriggerConfiguration->EnableExtTriggerInvert(1);
#else
        /* trigger on rising edge */
        pBrickMk3TriggerConfiguration->EnableExtTriggerEdgeDetectMode(1);
        pBrickMk3TriggerConfiguration->DisableExtTriggerInvert(1);
#endif

        /* perform timestamp test */
        timestampTest(pDevice);

        /* free resources */
        delete pRouterGlobalState;
        delete pDeviceConfiguration;
        delete pBrickMk3TriggerConfiguration;
        delete pDevice;
    }
    else
    {
        printf("No compatible devices were found.\n");
    }

    return 0;
}

