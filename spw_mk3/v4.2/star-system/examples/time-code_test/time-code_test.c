/**
 * \file time-code_test.c
 *
 * \brief Example test program, showing how to transmit and receive time-codes,
 * enable and disable a time-code master, and to set the period of the
 * time-code master.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd.
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include "star-api.h"
#include "cfg_api_generic.h"


#if defined(_WIN32)

    #include <process.h>
    #include <windows.h>

    #define STAR_THREAD             HANDLE
    #define STAR_THREAD_RETURN_TYPE unsigned int WINAPI
    #define STAR_THREAD_RETURN_VAL  0

    #define STAR_CreateThread(thread, func, arg)    \
        ((thread = (HANDLE)_beginthreadex(NULL, 0, func, arg, 0, NULL)) != 0)

    #define STAR_CloseThread(thread)    CloseHandle(thread)

    #define STAR_WaitForThreadCompletion(thread)    \
        (WaitForSingleObject(thread, INFINITE) == WAIT_OBJECT_0)

#else

    #include <pthread.h>
    #include <unistd.h>

    #define STAR_THREAD             pthread_t
    #define STAR_THREAD_RETURN_TYPE void *
    #define STAR_THREAD_RETURN_VAL  NULL

    #define STAR_CreateThread(thread, func, arg)    \
        (!pthread_create(&(thread), NULL, func, arg))

    #define STAR_CloseThread(thread)    \
        ((thread) ? (!pthread_detach(thread)) : 1)

    #define STAR_WaitForThreadCompletion(thread)    \
        ((!pthread_join(thread, NULL)) && (!((thread) = 0)))

#endif



#if !defined(UNREFERENCED_PARAMETER)
    #define UNREFERENCED_PARAMETER(a)   ((void)(a))
#endif



/**
 * The next time-code to be transmitted, when transmitting individual
 * time-codes.
 */
U8 g_currentTimecode = 0;

/** The thread which receives time-codes. */
STAR_THREAD g_receiveThread = 0;

/** Whether the thread is still running. */
int g_receiveThreadRunning = 0;

/** The channel to receive time-codes on. */
STAR_CHANNEL_ID g_receiveChannel = 0;


/**
 * Transmit the next time-code.
 *
 * @param deviceId the device to be used to transmit the time-code
 */
static void transmitTimecode(STAR_DEVICE_ID deviceId)
{
    /* Open channel 0 to transmit */
    /* Note that STAR-System devices currently only support transmitting and */
    /* receiving time-codes on channel 0 */
    STAR_CHANNEL_ID txChannel = STAR_openChannelToLocalDevice(deviceId,
        STAR_CHANNEL_DIRECTION_OUT, 0, 0);

    /* If transmit channel was opened */
    if(txChannel)
    {
        STAR_TRANSFER_STATUS status;
        STAR_TRANSFER_OPERATION * transferOperation;

        /* Create a time-code */
        STAR_STREAM_ITEM *timecode = STAR_createTimeCode(g_currentTimecode);

        /* If time-code was created */
        if (timecode)
        {
            /* Create transmit operation to transmit time-code */
            transferOperation = STAR_createTxOperation(&timecode, 1);

            /* Start transmitting the time-code */
            STAR_submitTransferOperation(txChannel, transferOperation);

            /* Wait indefinitely for transfer to complete */
            status = STAR_waitOnTransferOperationCompletion(transferOperation,
                -1);

            /* If time-code was transmitted successfully */
            if (status == STAR_TRANSFER_STATUS_COMPLETE)
            {
                /* Print success message */
                printf("Time-code %u transmitted successfully.\n",
                    g_currentTimecode);
            }
            else
            {
                /* Print error message */
                printf("Error transmitting time-code %u, status = %d.\n",
                    g_currentTimecode, status);
            }

            /* Dispose of transfer operation */
            STAR_disposeTransferOperation(transferOperation);

            /* Destroy time-code */
            STAR_destroyStreamItem(timecode);

            /* Move to the next time-code value */
            g_currentTimecode++;
            if (g_currentTimecode == 64)
            {
                g_currentTimecode = 0;
            }
        }

        /* Close transmit channel */
        STAR_closeChannel(txChannel);
    }
}


/**
 * Set the time-code period used when the device is enabled as a time-code
 * master.
 *
 * @param deviceId the device to set the time-code period for
 */
static void setTimecodePeriod(STAR_DEVICE_ID deviceId)
{
    char s[256];

    /* Display the current time-code period */
    U32 period;
    if (CFG_SUCCESS(CFG_getTimeCodePeriod(deviceId, &period)))
    {
        printf("Current time-code period: %u microseconds\n", period);
    }
    else
    {
        puts("Unable to read the current time-code period");
    }

    /* Read in the time-code period to be set */
    printf("Please enter the time-code period in microseconds: ");
    fflush(stdout);
    if (!fgets(s, 256, stdin))
    {
        puts("No period specified.");
    }
    else
    {
        int status = sscanf(s, "%u", &period);
        if (!status)
        {
            puts("Invalid time-code period specified.");
        }
        else
        {
            /* Set the time-code period */
            if (CFG_SUCCESS(CFG_setTimeCodePeriod(deviceId, period)))
            {
                printf("Time-code period set to %u\n", period);
            }
            else
            {
                puts("Error setting time-code period");
            }
        }
    }
}


/**
 * Enable a device as a time-code master.
 *
 * @param deviceId the device to be enabled as a time-code master
 */
static void enableTimecodeMaster(STAR_DEVICE_ID deviceId)
{
    /* Enable the device as a time-code master */
    if (CFG_SUCCESS(CFG_enableTimeCodeMaster(deviceId)))
    {
        puts("Device enabled as a time-code master");
    }
    else
    {
        puts("Error enabling device as a time-code master");
    }
}


/**
 * Disable a device as a time-code master.
 *
 * @param deviceId the device to no longer be enabled as a time-code master
 */
static void disableTimecodeMaster(STAR_DEVICE_ID deviceId)
{
    /* Disable the device as a time-code master */
    if (CFG_SUCCESS(CFG_disableTimeCodeMaster(deviceId)))
    {
        puts("Device disabled as a time-code master");
    }
    else
    {
        puts("Error disabling device as a time-code master");
    }
}


/**
 * Function that runs as a thread, receiving time-codes.
 *
 * @param pArg the argument passed to the thread when it was created, containing
 *             the identifier of the device to receive time-codes on
 *
 * @return the return value expected by thread functions
 */
STAR_THREAD_RETURN_TYPE timecodeReceiveThread(void *pArg)
{
    STAR_TRANSFER_OPERATION *pRxTransferOp;

    /* Get the device ID from the argument passed to the thread */
    STAR_DEVICE_ID deviceId = *(STAR_DEVICE_ID *)pArg;
    free(pArg);

    /* Open channel 0 to receive */
    /* Note that STAR-System devices currently only support transmitting and */
    /* receiving time-codes on channel 0 */
    g_receiveChannel = STAR_openChannelToLocalDevice(deviceId,
        STAR_CHANNEL_DIRECTION_IN, 0, 1);
    if (!g_receiveChannel)
    {
        puts("Unable to open channel to receive time-codes");
        goto timecodeReceiveThread_openChannelFail;
    }

    /* Create a receive operation to receive 1 time-code */
    pRxTransferOp = STAR_createRxOperation(1, STAR_RECEIVE_TIMECODES);
    if (!pRxTransferOp)
    {
        puts("Unable to create receive operation to receive a time-code");
        goto timecodeReceiveThread_createRxOperationFail;
    }

    /* While the thread has not been stopped */
    while (g_receiveThreadRunning)
    {
        STAR_TRANSFER_STATUS rxStatus;
        STAR_STREAM_ITEM *pRxStreamItem;

        /* Submit the receive operation */
        if (!STAR_submitTransferOperation(g_receiveChannel, pRxTransferOp))
        {
            if (g_receiveThreadRunning)
            {
                puts("Unable to submit receive operation to receive a time-code");
            }
            break;
        }

        /* Wait indefinitely on the receive operation completing */
        rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp, -1);
        if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
        {
            if (g_receiveThreadRunning)
            {
                printf("Error waiting for receive operation to receive a time-code, status = %d\n", rxStatus);
            }
            break;
        }

        /* Get the received time-code */
        pRxStreamItem = STAR_getTransferItem(pRxTransferOp, 0);
        if ((!pRxStreamItem) || (pRxStreamItem->itemType !=
                STAR_STREAM_ITEM_TYPE_TIMECODE) || (!pRxStreamItem->item))
        {
            if (g_receiveThreadRunning)
            {
                puts("Error getting receive stream item");
            }
            break;
        }

        /* Display the received time-code */
        printf("\t\tReceived time-code with value: %2u\n",
            STAR_getTimeCodeValue((STAR_TIMECODE *)pRxStreamItem->item));
    }

    /* Dispose the receive operation */
    STAR_disposeTransferOperation(pRxTransferOp);

timecodeReceiveThread_createRxOperationFail:
    /* Close the receive channel */
    STAR_closeChannel(g_receiveChannel);

timecodeReceiveThread_openChannelFail:
    g_receiveThreadRunning = 0;

    return STAR_THREAD_RETURN_VAL;
}


/**
 * Start receiving time-codes from a device.
 *
 * @param deviceId the device to receive time-codes from
 */
static void startReceivingTimecodes(STAR_DEVICE_ID deviceId)
{
    /* Check the thread isn't already running */
    if (g_receiveThread)
    {
        puts("The receive thread is already running");
    }
    else
    {
        /* Create the argument to be passed to the thread */
        STAR_DEVICE_ID *pDeviceId;
        pDeviceId = (STAR_DEVICE_ID *)calloc(1, sizeof(STAR_DEVICE_ID));
        if (!pDeviceId)
        {
            puts("Unable to create argument for thread to receive time-codes");
        }
        else
        {
            *pDeviceId = deviceId;

            /* Start a thread to receive time-codes */
            g_receiveThreadRunning = 1;
            if (!STAR_CreateThread(g_receiveThread, timecodeReceiveThread,
                pDeviceId))
            {
                puts("Unable to create thread to receive time-codes");
                g_receiveThreadRunning = 0;
                g_receiveThread = 0;
            }
        }
    }
}


/**
 * Stop receiving time-codes from a device.
 *
 * @param deviceId the device to stop receiving time-codes from
 */
static void stopReceivingTimecodes()
{
    if (!g_receiveThread)
    {
        puts("The receive thread is not currently running");
    }
    else
    {
        /* Close the receive channel to stop the thread receiving time-codes */
        g_receiveThreadRunning = 0;
        STAR_closeChannel(g_receiveChannel);

        /* Wait on the thread completing */
        if (!STAR_WaitForThreadCompletion(g_receiveThread))
        {
            puts("Error waiting for the receive thread to complete");
        }

        /* Close the thread */
        (void)STAR_CloseThread(g_receiveThread);
        g_receiveThread = 0;
    }
}


/**
 * Enable external time-code selection for a device, which means that the
 * software can specify the time-code value to be sent.  Note that this may not
 * be forwarded if it's not the next valid value.
 *
 * @param deviceId the device to enable external time-code selection for
 */
static void enableExternalTimecodeSelection(STAR_DEVICE_ID deviceId)
{
    /* Enable external time-codes selection for the device */
    if (CFG_SUCCESS(CFG_enableExternalTimeCodeSelection(deviceId)))
    {
        puts("External time-code selection enabled for the device");
    }
    else
    {
        puts("Error enabling external time-code selection for the device");
    }
}


/**
 * Disable external time-code selection for a device, which means that the
 * device will ignore the time-code value specified by the software, and will
 * instead always send the next valid value.
 *
 * @param deviceId the device to disable external time-code selection for
 */
static void disableExternalTimecodeSelection(STAR_DEVICE_ID deviceId)
{
    /* Disable external time-codes selection for the device */
    if (CFG_SUCCESS(CFG_disableExternalTimeCodeSelection(deviceId)))
    {
        puts("External time-code selection disabled for the device");
    }
    else
    {
        puts("Error disabling external time-code selection for the device");
    }
}


/**
 * Display the list of currently connected devices.
 */
static STAR_DEVICE_ID displayDeviceList(void)
{
    U32 deviceCount, deviceNum;
    char *deviceName, s[256];
    unsigned int chosenDevice;
    int status;

    /* Get the list of configurable devices */
    STAR_DEVICE_ID *devices = STAR_getDeviceListForType(
        STAR_DEVICE_CONFIG_SUPPORTED, &deviceCount);
    STAR_DEVICE_ID deviceId = 0;

    /* If there are devices present */
    if (devices)
    {
        if (!deviceCount)
        {
            puts("No devices present.\n");
        }
        else if (deviceCount == 1)
        {
            deviceName = STAR_getDeviceName(devices[0]);
            if (deviceName)
            {
                printf("One device present: %s\n", deviceName);
                STAR_destroyString(deviceName);
            }
            else
            {
                puts("One device present: Unable to determine device name");
            }
            deviceId = devices[0];
        }
        else
        {
            puts("Select device:");

            /* For each device in the list */
            for (deviceNum = 0; deviceNum < deviceCount; deviceNum++)
            {
                /* Display its name */
                deviceName = STAR_getDeviceName(devices[deviceNum]);
                if (deviceName)
                {
                    printf("%u: %s\n", deviceNum, deviceName);
                    STAR_destroyString(deviceName);
                }
                else
                {
                    printf("%u: Unable to determine device name\n", deviceNum);
                }
            }

            /* Get the device to use */
            if (!fgets(s, 256, stdin))
            {
                puts("No device number selected.");
            }
            else
            {
                status = sscanf(s, "%u", &chosenDevice);
                if ((!status) || (chosenDevice > deviceCount - 1))
                {
                    puts("Incorrect device number selected.");
                }
                else
                {
                    deviceId = devices[chosenDevice];
                }
            }
        }

        /* Destroy the device list */
        STAR_destroyDeviceList(devices);
    }
    else
    {
        puts("No devices present.\n");
    }

    return deviceId;
}


/**
 * Main function, called when the program is started.
 *
 * @param argc the number of parameters passed to the program.
 * @param argv the array of arguments passed to the program.
 *
 * @return returns 0 on normal program ending, otherwise 1
 */
int __cdecl main(int argc, char *argv[])
{
    char exitSelected = 0;
    char s[256];
    STAR_DEVICE_ID deviceId;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    STAR_setApplicationName("STAR-System Time-code Test Application");

    /* Get the device to be used */
    deviceId = displayDeviceList();
    if (!deviceId)
    {
        return 0;
    }

    /* Enable time-code forwarding on all ports of the device */
    if (!CFG_SUCCESS(CFG_setTimeCodeDistributionPorts(deviceId, 0x00000fff)))
    {
        puts("Unable to enable time-code distribution on all ports");
    }

    do
    {
        /* Display  Menu */
        puts("\n");
        puts("Select option:");
        puts("  0: Exit");
        puts("  1: Transmit a time-code");
        puts("  2: Set period of time-code master (cannot be done while receiving)");
        puts("  3: Enable time-code master (cannot be done while receiving)");
        puts("  4: Disable time-code master (cannot be done while receiving)");
        puts("  5: Start receiving time-codes");
        puts("  6: Stop receiving time-codes");
        puts("  7: Enable external time-code selection (cannot be done while receiving)");
        puts("  8: Disable external time-code selection (cannot be done while receiving)");
        puts("");

        /* Read in the operation type */
        if (!fgets(s, 256, stdin))
        {
            puts("Error reading input, exiting");
            exitSelected = 1;
        }
        else
        {
            puts("");
            switch (s[0])
            {
                case '0':
                    exitSelected = 1;
                    break;

                case '1':
                    transmitTimecode(deviceId);
                    break;

                case '2':
                    setTimecodePeriod(deviceId);
                    break;

                case '3':
                    enableTimecodeMaster(deviceId);
                    break;

                case '4':
                    disableTimecodeMaster(deviceId);
                    break;

                case '5':
                    startReceivingTimecodes(deviceId);
                    break;

                case '6':
                    stopReceivingTimecodes();
                    break;

                case '7':
                    enableExternalTimecodeSelection(deviceId);
                    break;

                case '8':
                    disableExternalTimecodeSelection(deviceId);
                    break;

                default:
                    printf("Invalid menu option selected: %c\n", s[0]);
            }
        }
    } while (!exitSelected);

    puts("Exiting");

    return 0;
}

