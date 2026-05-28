/**
 * \file timestamp_test.c
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

#include "star-api.h"
#include "cfg_api_generic.h"
#include "triggering_brick_mk3.h"

#include <stdio.h>

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
   10Hz, 100Hz or 1000Hz). */
#define SYNC_PULSE_FREQUENCY 1 /* 1Hz */

#else

/* Set sync pulse frequency to match external trigger pulse generation
   frequency. */
#define SYNC_PULSE_FREQUENCY 1 /* 1Hz */

#endif

/* Define transmit and receive links */
#define TRANSMIT_LINK 1
#define RECEIVE_LINK 2

/**
 * Returns the id of the first device compatible with timestamping.
 *
 * @return device id of first device compatible with timestamping.
 */
STAR_DEVICE_ID getDeviceId()
{
    /* The selected device to use for the example */
    STAR_DEVICE_ID selectedDevice = 0;

    /* Get all Brick Mk3 devices */
    U32 deviceCount = 0;
    STAR_DEVICE_ID *pDeviceList = STAR_getDeviceListForType(
        STAR_DEVICE_BRICK_MK3, &deviceCount);

    /* If at least one device */
    if(deviceCount > 0)
    {
        U32 x;

        /* For all devices */
        for(x = 0; x < deviceCount; x++)
        {
            /* Get the current device */
            STAR_DEVICE_ID currentDevice = pDeviceList[x];

            /* Get hardware info for current device */
            STAR_CFG_FPGA_INFO hardwareInfo;
            CFG_getFPGAInfo(currentDevice, &hardwareInfo);

            /* If Brick Mk3 is v1.02 or later */
            if(hardwareInfo.major > 1 ||
                (hardwareInfo.major == 1 && hardwareInfo.minor >= 2))
            {
                /* Use current device for test */
                selectedDevice = currentDevice;
                break;
            }
        }
    }

    /* Destroy the device list */
    STAR_destroyDeviceList(pDeviceList);

    /* Return the selected device */
    return selectedDevice;
}

/**
 * Prints the given packet data as a series of bytes.
 *
 * @param pPacketData the packet data to be printed.
 * @param packetLength the length of the packet.
 */
void printPacket(unsigned char *pPacketData, unsigned int packetLength)
{
    /* Print packet contents */
    unsigned int x;
    for(x = 0; x < packetLength; x++)
    {
        printf("%02X ", pPacketData[x]);
    }
}

/**
 * Transmits the given number of packets over the specified channel.
 *
 * @param channel the channel to transmit packets over.
 * @param transmitPacketCount the number of packets to be transmitted.
 */
void transmitPackets(STAR_CHANNEL_ID channel, unsigned int transmitPacketCount)
{
    U8 buffer[32];
    unsigned int x;
    STAR_STREAM_ITEM *pPacket;
    STAR_TRANSFER_OPERATION *pTransmitOperation;

    /* For all packets to be transmitted */
    for(x = 1; x <= transmitPacketCount; x++)
    {
        /* Populate packet buffer */
        memset(buffer, 0xAB, 32);
        memset(buffer, x, 1);

        /* Create packet from buffer */
        pPacket = STAR_createPacket(NULL, buffer, 32, STAR_EOP_TYPE_EOP);

        /* Create transmit operation from packet */
        pTransmitOperation = STAR_createTxOperation(&pPacket, 1);

        /* Transmit the packet */
        STAR_submitTransferOperation(channel, pTransmitOperation);

        /* Wait for transmit operation to complete */
        STAR_waitOnTransferOperationCompletion(pTransmitOperation, -1);

        /* Cleanup resources required for transmit operation */
        STAR_disposeTransferOperation(pTransmitOperation);
        STAR_destroyStreamItem(pPacket);

        /* Print packet transmit status */
        printf("Packet %d transmitted : ", x);
        printPacket(buffer, 32);
        printf("\n");

        /* Sleep for 100 milliseconds */
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
    /* Convert value to double */
    double doubleValue = (double)seconds + ((double)nanoseconds / 1E9);

    /* Return value as double */
    return doubleValue;
}

/**
 * Receives the given number of packets and timestamps over the specified
 * channel.
 *
 * @param channel the channel to receive packets and timestamps on.
 * @param receiveCount the number of packets and timestamps to be received.
 * @param syncPulseFrequency the frequency of the synchronisation pulse.
 */
void receivePacketsAndTimestamps(STAR_CHANNEL_ID channel,
    unsigned int receiveCount, U32 syncPulseFrequency)
{
    STAR_TRANSFER_OPERATION *pReceiveOperation;
    unsigned int x;
    unsigned int actualReceiveCount;

    /* Create receive operation for packets and timestamp events */
    pReceiveOperation = STAR_createRxOperation(receiveCount,
        STAR_RECEIVE_PACKETS | STAR_RECEIVE_TIMESTAMP_EVENTS);

    /* Submit the receive operation */
    STAR_submitTransferOperation(channel, pReceiveOperation);

    /* Wait for receive operation to complete */
    STAR_waitOnTransferOperationCompletion(pReceiveOperation, -1);

    /* Get the number of stream items that were received */
    actualReceiveCount = STAR_getTransferItemCount(pReceiveOperation);

    /* For all stream items that were received */
    for(x = 0; x < actualReceiveCount; x++)
    {
        /* Get current stream item */
        STAR_STREAM_ITEM *pReceivedItem =
            STAR_getTransferItem(pReceiveOperation, x);

        /* If current stream item is a packet */
        if(pReceivedItem->itemType == STAR_STREAM_ITEM_TYPE_SPACEWIRE_PACKET)
        {
            unsigned char *pPacketData;
            unsigned int packetLength;

            /* Get the packet data */
            STAR_SPACEWIRE_PACKET *pPacket =
                (STAR_SPACEWIRE_PACKET *)pReceivedItem->item;
            pPacketData = STAR_getPacketData(pPacket, &packetLength);

            /* Print the packet data */
            printf("Packet %d received    : ", pPacketData[0]);
            printPacket(pPacketData, packetLength);
            printf("\n");

            /* Destroy the packet data now that it has been used */
            STAR_destroyPacketData(pPacketData);
        }
        /* Else if current stream item is a timestamp event */
        else if(pReceivedItem->itemType ==
            STAR_STREAM_ITEM_TYPE_TIMESTAMP_EVENT)
        {
            U32 startSeconds;
            U32 startRemainder;
            U32 endSeconds;
            U32 endRemainder;

            /* Get the timestamp data */
            STAR_TIMESTAMP_EVENT *pTimestampEvent =
                (STAR_TIMESTAMP_EVENT *)pReceivedItem->item;

            /* Get start and end timestamp values in seconds and a remainder of
               nanoseconds */
            STAR_getTimestampEventStartValue(pTimestampEvent,
                syncPulseFrequency, &startSeconds, &startRemainder);
            STAR_getTimestampEventEndValue(pTimestampEvent,
                syncPulseFrequency, &endSeconds, &endRemainder);

            /* Print the start and end of packet timestamps */
            printf("Start of packet timestamp : %f seconds.\n",
                convertToDouble(startSeconds, startRemainder));
            printf("End of packet timestamp   : %f seconds.\n\n",
                convertToDouble(endSeconds, endRemainder));
        }
    }

    /* Cleanup receive operation */
    STAR_disposeTransferOperation(pReceiveOperation);
}

/**
 * Enables a 1Hz sync pulse on the external trigger.
 *
 * @param deviceId the device id to enable external sync pulse for.
 */
void enableExternalSyncPulse(STAR_DEVICE_ID deviceId)
{
    /* The following code uses the STAR-System triggering API to generate a
       pulse every second. An SMB lead should be connected between triggers
       A and B on the Brick Mk3. */

    /* Set timer 0 reload value to 1 second */
    TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, 0, 60000000);

    /* Enable timer auto reload */
    TRIGGER_BRICK_MK3_enableCounterAutoReload(deviceId, 0);

    /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to
       be set */
    TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 0, 0,
        COUNTER_EVENT_RELOAD);

    /* Internal trigger 0 causes TIME_CODE_ACTION_TX action on time-code
       engine 0 */
    TRIGGER_BRICK_MK3_enableExtTriggerOutput(deviceId, 1);
    TRIGGER_BRICK_MK3_setExtTriggerExtend(deviceId, 1, 50);
    TRIGGER_BRICK_MK3_setExtTriggerOutputActions(deviceId, 1, 0,
        EXT_TRIGGER_ACTION_OUT);

    /* Enable internal trigger 0 */
    TRIGGER_BRICK_MK3_enableTrigger(deviceId, 0);
}

/**
 * Disables sync pulse on the external trigger.
 *
 * @param deviceId the device to disable external sync pulse for.
 */
void disableExternalSyncPulse(STAR_DEVICE_ID deviceId)
{
    /* Disable the constant pulse on the external trigger */
    TRIGGER_BRICK_MK3_disableExtTriggerOutput(deviceId, 1);
    TRIGGER_BRICK_MK3_disableTrigger(deviceId, 0);
}

/**
 * Configures timestamp functionality for either global pulse generator or
 * external trigger synchronised timestamps. When GLOBAL_PULSE_GENERATOR is
 * defined, the global pulse generator is used and when GLOBAL_PULSE_GENERATOR
 * is not defined, the external trigger is used.
 *
 * @param deviceId the device to perform timestamp test on.
 */
void timestampTest(STAR_DEVICE_ID deviceId)
{
    STAR_CHANNEL_ID transmitChannel;
    STAR_CHANNEL_ID receiveChannel;

    /* Transmit 10 packets */
    unsigned int transmitPacketCount = 10;

    printf("Setting up timestamp system...\n");

    /* Ensure that timestamp counters are disabled before configuring */
    CFG_setTimestampMethod(deviceId,
        STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD_NONE);

    /* Set initial timestamp value of 10 seconds */
    CFG_setTimestampValue(deviceId, 10);

    /* Enable timestamp events on port 2 */
    CFG_enableRxTimestampEventsOnPort(deviceId, RECEIVE_LINK);

#ifdef GLOBAL_PULSE_GENERATOR

#if SYNC_PULSE_FREQUENCY == 1

    /* Set pulse generator frequency to 1Hz */
    CFG_setPulseGeneratorFrequency(deviceId,
        STAR_CFG_BRICK_MK3_PULSE_FREQ_1);

#elif SYNC_PULSE_FREQUENCY == 10

    /* Set pulse generator frequency to 10Hz */
    CFG_setPulseGeneratorFrequency(deviceId,
        STAR_CFG_BRICK_MK3_PULSE_FREQ_10);

#elif SYNC_PULSE_FREQUENCY == 100

    /* Set pulse generator frequency to 100Hz */
    CFG_setPulseGeneratorFrequency(deviceId,
        STAR_CFG_BRICK_MK3_PULSE_FREQ_100);

#elif SYNC_PULSE_FREQUENCY == 1000

    /* Set pulse generator frequency to 1KHz */
    CFG_setPulseGeneratorFrequency(deviceId,
        STAR_CFG_BRICK_MK3_PULSE_FREQ_1000);

#else

    /* Print error when unexpected global pulse frequency is detected  */
    printf("ERROR: Unexpected global pulse frequency of %dHz (valid values are: 1Hz, 10Hz, 100Hz, 1000Hz).\n");
    return;

#endif

    /* Enable global pulse generator */
    CFG_setTimestampMethod(deviceId,
        STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD_PULSE_GENERATOR);

#else

    /* Enable external trigger pulse detection */
    CFG_setTimestampMethod(deviceId,
        STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD_EXTERNAL_TRIGGER);

    /* Enable the external sync pulse before transmitting packets */
    enableExternalSyncPulse(deviceId);

#endif

    /* Create transmit and receive channels */
    transmitChannel = STAR_openChannelToLocalDevice(deviceId,
        STAR_CHANNEL_DIRECTION_OUT, TRANSMIT_LINK, 1);
    receiveChannel = STAR_openChannelToLocalDevice(deviceId,
        STAR_CHANNEL_DIRECTION_IN, RECEIVE_LINK, 1);

    /* Sleep to allow timer to run before transmitting packets */
    SLEEP(2000);

    /* Transmit a group of packets */
    transmitPackets(transmitChannel, transmitPacketCount);

    /* Receive the packets and timestamps */
    receivePacketsAndTimestamps(receiveChannel, transmitPacketCount * 2,
        SYNC_PULSE_FREQUENCY);

#ifndef GLOBAL_PULSE_GENERATOR

    /* Disable the external sync pulse after receiving packets and timestamps */
    disableExternalSyncPulse(deviceId);

#endif

    /* Disable timestamps after receiving packets and timestamps */
    CFG_setTimestampMethod(deviceId,
        STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD_NONE);

    /* Close the channels */
    STAR_closeChannel(transmitChannel);
    STAR_closeChannel(receiveChannel);
}

/**
 * Main function, called when the program is started.
 *
 * @param argc the number of parameters passed to the program.
 * @param argv the array of arguments passed to the program.
 *
 * @return returns 0 on normal program ending, otherwise 1.
 */
int main(int argc, char **argv)
{
    /* Select device to use in examples */
    STAR_DEVICE_ID deviceId = getDeviceId();

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    STAR_setApplicationName("STAR-System Timestamp Test Application");

    /* Print the program header */
    printf("----- Timestamping API Example -----\n\n");

    /* If compatible device was found */
    if(deviceId)
    {
        STAR_CFG_ROUTER_GLOBAL_STATE globalState;
        PORT_STATUS_CONTROL portStatusControl;
        STAR_CFG_SPW_LINK_STATUS linkStatus;

        /* Get device name and serial */
        char * pDeviceName = STAR_getDeviceName(deviceId);
        char * pDeviceSerial = STAR_getDeviceSerialNumber(deviceId);

        /* Print selected serial */
        printf("%s with serial %s selected.\n\n", pDeviceName, pDeviceSerial);

        /* Disable router timeouts so that timestamps are not blocked */
        CFG_getRouterGlobalSettings(deviceId, &globalState);
        globalState.timeoutMode = STAR_CFG_TIMEOUT_MODE_BLOCKING;
        CFG_setRouterGlobalSettings(deviceId, &globalState);

        /* Timestamping requires interface mode to be enabled */
        CFG_enableInterfaceMode(deviceId);

        /* Start the link since watchdog timer is disabled */
        CFG_getPortStatusControl(deviceId, 1, &portStatusControl);
        CFG_getSpaceWireLinkStatus(portStatusControl, &linkStatus);
        linkStatus.start = 1;
        CFG_setSpaceWireLinkStatus(deviceId, 1, &linkStatus);

#ifdef EXTERNAL_TRIGGER_FALLING_EDGE
        /* Trigger on falling edge */
        TRIGGER_BRICK_MK3_enableExtTriggerEdgeDetectMode(deviceId, 1);
        TRIGGER_BRICK_MK3_enableExtTriggerInvert(deviceId, 1);
#else
        /* Trigger on rising edge */
        TRIGGER_BRICK_MK3_enableExtTriggerEdgeDetectMode(deviceId, 1);
        TRIGGER_BRICK_MK3_disableExtTriggerInvert(deviceId, 1);
#endif

        /* Perform timestamp test */
        timestampTest(deviceId);
    }
    else
    {
        printf("No compatible devices were found.\n");
    }

    return 0;
}
