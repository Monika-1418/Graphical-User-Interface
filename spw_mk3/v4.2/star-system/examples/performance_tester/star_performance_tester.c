/**
 * \file  star_performance_tester.c
 *
 * \brief Program to test the performance of SpaceWire devices.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the functions for the SpaceWire Performance Tester, a
 * program to test the performance of SpaceWire devices using the STAR-System
 * software stack.
 *
 * Copyright &copy; 2015 STAR-Dundee Ltd.
 *
 */


/**
 * \mainpage
 *
 * The STAR-System Performance Tester is a command line program that can be used
 * to test the performance of devices, networks and the system itself.  The
 * source code also provides useful examples of potential methods to get the
 * best performance from the API.
 */

/**
 * \defgroup PerformanceTester STAR-System Performance Tester
 * This section contains the functions provided by the STAR-System Performance
 * Tester application.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "star-api.h"

#if !defined(__vxworks)
    #define TRUE  1
    #define FALSE 0
#else
    /* Required for strcasecmp() */
    #include <strings.h>
#endif


#define STAR_INFINITE -1

#ifdef _WIN32

    #include <windows.h>

    /**
     * \ingroup PerformanceTester
     * Compare two strings, ignoring case.
     *
     * @param s1 the first string to be compared
     * @param s2 the second string to be compared
     *
     * @return 0 if the strings are the same
     */
    #define strcasecmp(s1, s2)    _stricmp((s1), (s2))

    /**
     * \ingroup PerformanceTester
     * Get the current time as an integer.
     *
     * @return the current time
     */
    #define GET_TIME()  clock()

    /**
     * \ingroup PerformanceTester
     * The number of ticks in a second.
     */
    #define TIME_DIVIDER    CLOCKS_PER_SEC

    /**
     * \ingroup PerformanceTester
     * Sleep for a period of time in milliseconds.
     */
    #define SLEEP(time)     Sleep(time)

    /**
     * \ingroup PerformanceTester
     * Prevent the system sleeping (note that the display may still switch off).
     */
    #define DISABLE_SYSTEM_SLEEP()  \
        SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED)

    /**
     * \ingroup PerformanceTester
     * Allow the system to sleep normally after previously disabling sleeping.
     */
    #define ENABLE_SYSTEM_SLEEP()   \
        SetThreadExecutionState(ES_CONTINUOUS)

    /**
     * \ingroup PerformanceTester
     * The structure used to store the CPU usage.
     */
    typedef struct
    {
        /** The idle time */
        FILETIME idleTime;

        /** The kernel time */
        FILETIME kernelTime;

        /** The user time */
        FILETIME userTime;

        /** The process's kernel time */
        FILETIME processKernelTime;

        /** The process's user time */
        FILETIME processUserTime;

    } CPU_USAGE_PROPERTIES;


    /**
     * \ingroup PerformanceTester
     * Start recording the CPU usage, by storing the current usage.
     *
     * @param pCpuProperties a pointer to a structure which will be updated to
     *                       contain the current CPU properties
     */
    static void STORE_CPU_USAGE(CPU_USAGE_PROPERTIES * const pCpuProperties)
    {
        FILETIME creationTime, exitTime;

        GetSystemTimes(&pCpuProperties->idleTime, &pCpuProperties->kernelTime,
            &pCpuProperties->userTime);
        GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime,
            &pCpuProperties->processKernelTime,
            &pCpuProperties->processUserTime);
    }

    /**
     * \ingroup PerformanceTester
     * Print the headers for the CPU usage for a test written to file.
     *
     * @param stream the stream to write the CPU usage figures to
     */
    static void PRINT_CPU_USAGE_HEADERS(FILE * const stream)
    {
        fprintf(stream, "\tProcess CPU usage (%%)\tTotal CPU usage (%%)\n");
    }


    /**
     * \ingroup PerformanceTester
     * Print the CPU usage for a test.
     *
     * @param stream the stream to write the CPU usage figures to.  This should
     *               be NULL for stdout
     * @param pStartCpuProperties a pointer to the original CPU usage properties
     *                            when the test started
     * @param pEndCpuProperties a pointer to the final CPU usage properties when
     *                          the test completed
     */
    static void PRINT_CPU_USAGE(FILE * const stream,
        const CPU_USAGE_PROPERTIES * const pStartCpuProperties,
        const CPU_USAGE_PROPERTIES * const pEndCpuProperties)
    {
        ULARGE_INTEGER originalIdleTime, originalKernelTime, originalUserTime;
        ULARGE_INTEGER idleTime, kernelTime, userTime, totalTime;
        double totalUsage, processUsage;

        /* Subtract the original total CPU figures from the final figures */
        originalIdleTime.HighPart =
            pStartCpuProperties->idleTime.dwHighDateTime;
        originalIdleTime.LowPart =
            pStartCpuProperties->idleTime.dwLowDateTime;
        idleTime.HighPart = pEndCpuProperties->idleTime.dwHighDateTime;
        idleTime.LowPart = pEndCpuProperties->idleTime.dwLowDateTime;
        idleTime.QuadPart -= originalIdleTime.QuadPart;
        originalKernelTime.HighPart =
            pStartCpuProperties->kernelTime.dwHighDateTime;
        originalKernelTime.LowPart =
            pStartCpuProperties->kernelTime.dwLowDateTime;
        kernelTime.HighPart = pEndCpuProperties->kernelTime.dwHighDateTime;
        kernelTime.LowPart = pEndCpuProperties->kernelTime.dwLowDateTime;
        kernelTime.QuadPart -= originalKernelTime.QuadPart;
        originalUserTime.HighPart =
            pStartCpuProperties->userTime.dwHighDateTime;
        originalUserTime.LowPart =
            pStartCpuProperties->userTime.dwLowDateTime;
        userTime.HighPart = pEndCpuProperties->userTime.dwHighDateTime;
        userTime.LowPart = pEndCpuProperties->userTime.dwLowDateTime;
        userTime.QuadPart -= originalUserTime.QuadPart;

        /* Calculate the total time */
        totalTime.QuadPart = userTime.QuadPart + kernelTime.QuadPart;

        /* Calculate the total usage */
        if (totalTime.QuadPart != 0U)
        {
            totalUsage =
                (double)((totalTime.QuadPart - idleTime.QuadPart) * 100U) /
                    (double)totalTime.QuadPart;
        }
        else
        {
            totalUsage = 0.0;
        }

        /* Subtract the original process CPU figures from the final figures */
        originalKernelTime.HighPart =
            pStartCpuProperties->processKernelTime.dwHighDateTime;
        originalKernelTime.LowPart =
            pStartCpuProperties->processKernelTime.dwLowDateTime;
        kernelTime.HighPart =
            pEndCpuProperties->processKernelTime.dwHighDateTime;
        kernelTime.LowPart = pEndCpuProperties->processKernelTime.dwLowDateTime;
        kernelTime.QuadPart -= originalKernelTime.QuadPart;
        originalUserTime.HighPart =
            pStartCpuProperties->processUserTime.dwHighDateTime;
        originalUserTime.LowPart =
            pStartCpuProperties->processUserTime.dwLowDateTime;
        userTime.HighPart =
            pEndCpuProperties->processUserTime.dwHighDateTime;
        userTime.LowPart =
            pEndCpuProperties->processUserTime.dwLowDateTime;
        userTime.QuadPart -= originalUserTime.QuadPart;

        /* Calculate the process's usage */
        if (totalTime.QuadPart != 0U)
        {
            processUsage =
                (double)((userTime.QuadPart + kernelTime.QuadPart) * 100U) /
                (double)totalTime.QuadPart;
        }
        else
        {
            processUsage = 0.0;
        }

        if (stream != NULL)
        {
            fprintf(stream, "\t%-3.2f\t%-3.2f\n", processUsage, totalUsage);
        }
        else
        {
            printf("\t\t%-3.2f%% Process CPU usage\t%-3.2f%% Total CPU usage\n",
                processUsage, totalUsage);
        }
    }

#else

    /** VxWorks includes for GET_TIME functionality */
       #if defined(__vxworks)
           #include <time.h>
           #include <utime.h>
           #include <sys/times.h>
           #include <taskLib.h>
           #include <sysLib.h>
       #else
           #include <sys/time.h>
           #include <sys/times.h>

       #endif

       #include <unistd.h>

    /**
     * \ingroup PerformanceTester
     * Get the current time as an integer.
     *
     * @return the current time
     */
    clock_t GET_TIME()
    {
        struct timeval tv;
        struct timezone tz;

        gettimeofday(&tv, &tz);
        return tv.tv_sec * 1000000 + tv.tv_usec;
    }

    /**
     * \ingroup PerformanceTester
     * The number of ticks in a second.
     */
    #define TIME_DIVIDER    1000000

    /**
     * \ingroup PerformanceTester
     * Sleep for a period of time in milliseconds.
     */
#if defined(__vxworks)
        void SLEEP(unsigned int delay)
        {
            double rate = (double)sysClkRateGet();
            unsigned int ticks = (int)((rate / 1000.0) * delay + 0.5);
            taskDelay(ticks);
        }
    #else
        #define SLEEP(time)        usleep(time * 1000)
    #endif
    /**
     * \ingroup PerformanceTester
     * The maximum length to use for file paths.
     */
    #define MAX_PATH        1024

    /**
     * \ingroup PerformanceTester
     * Prevent the system sleeping (note that the display may still switch off).
     * Not implemented.
     */
    #define DISABLE_SYSTEM_SLEEP()

    /**
     * \ingroup PerformanceTester
     * Allow the system to sleep normally after previously disabling sleeping.
     * Not implemented.
     */
    #define ENABLE_SYSTEM_SLEEP()

    /**
     * \ingroup PerformanceTester
     * The structure used to store the CPU usage.
     */
    typedef struct
    {
#if !defined(__vxworks)
        /** The process's times */
        struct tms processTimes;
#endif

        /** The current time */
        clock_t  currentTime;

        /** The current user time for the system */
        unsigned long userTime;

        /** The current system time for the system */
        unsigned long systemTime;

        /** The current nice time for the system */
        unsigned long niceTime;

        /** The current idle time for the system */
        unsigned long idleTime;

        /** The current I/O wait time for the system */
        unsigned long iowaitTime;

    } CPU_USAGE_PROPERTIES;


    /**
     * \ingroup PerformanceTester
     * Start recording the CPU usage, by storing the current usage.
     *
     * @param pCpuProperties a pointer to a structure which will be updated to
     *                       contain the current CPU properties
     */
    void STORE_CPU_USAGE(CPU_USAGE_PROPERTIES *pCpuProperties)
    {
        FILE *procFile;
#if !defined(__vxworks)
        pCpuProperties->currentTime = times(&pCpuProperties->processTimes);
#endif
        pCpuProperties->userTime = 0;
        pCpuProperties->systemTime = 0;
        pCpuProperties->niceTime = 0;
        pCpuProperties->idleTime = 0;
        pCpuProperties->iowaitTime = 0;
        procFile = fopen("/proc/stat", "r");
        if (procFile)
        {
            char line[256];
            if (fgets(line, 256, procFile))
            {
                sscanf(line, "%*s %lu %lu %lu %lu %lu",
                    &pCpuProperties->userTime, &pCpuProperties->systemTime,
                    &pCpuProperties->niceTime, &pCpuProperties->idleTime,
                    &pCpuProperties->iowaitTime);
            }

            fclose(procFile);
        }
    }


    /**
     * \ingroup PerformanceTester
     * Print the headers for the CPU usage for a test written to file.
     *
     * @param stream the stream to write the CPU usage figures to
     */
    void PRINT_CPU_USAGE_HEADERS(FILE *stream)
    {
        fprintf(stream, "\tProcess CPU usage (%%)\tTotal CPU usage (%%)\n");
    }


    /**
     * \ingroup PerformanceTester
     * Get the number of CPU cores on this PC.
     *
     * @return the number of CPU cores
     */
#if defined(__QNX__)
    #include <sys/syspage.h>
    int GetCpuCount()
    {
        return _syspage_ptr->num_cpu;
    }
#elif defined(__vxworks)
    /* First version of vxworks driver is for single core targets only */
    int GetCpuCount()
    {
        return 1;
    }
#else
    int GetCpuCount()
    {
        return (int)sysconf(_SC_NPROCESSORS_ONLN);
    }
#endif


    /**
     * \ingroup PerformanceTester
     * Print the CPU usage for a test.
     *
     * @param stream the stream to write the CPU usage figures to.  This should
     *               be NULL for stdout
     * @param pStartCpuProperties a pointer to the original CPU usage properties
     *                            when the test started
     * @param pEndCpuProperties a pointer to the final CPU usage properties when
     *                          the test completed
     */
    void PRINT_CPU_USAGE(FILE *stream,
        CPU_USAGE_PROPERTIES const *pStartCpuProperties,
        CPU_USAGE_PROPERTIES const *pEndCpuProperties)
    {
        unsigned long userTime, systemTime, niceTime, idleTime, iowaitTime;
        unsigned long totalTime;
        clock_t processKernelTime, processUserTime, processTotalTime;
        double totalUsage, processUsage;
        int cpuCount;

        /* Determine the number of CPUs */
        cpuCount = GetCpuCount();

        /* Subtract the original total CPU figures from the final figures */
        userTime = pEndCpuProperties->userTime -
            pStartCpuProperties->userTime;
        systemTime = pEndCpuProperties->systemTime -
            pStartCpuProperties->systemTime;
        niceTime = pEndCpuProperties->niceTime -
            pStartCpuProperties->niceTime;
        idleTime = pEndCpuProperties->idleTime -
            pStartCpuProperties->idleTime;
        iowaitTime = pEndCpuProperties->iowaitTime -
            pStartCpuProperties->iowaitTime;

        /* Calculate the total time */
        totalTime = userTime + systemTime + niceTime + idleTime + iowaitTime;

        /* Calculate the total usage */
        if (totalTime)
        {
            totalUsage =
                (double)((totalTime - idleTime) * 100) / (double)totalTime;
        }
        else
        {
            totalUsage = 0;
        }

#if !defined(__vxworks)
        /* Subtract the original process CPU figures from the final figures */
        processKernelTime = pEndCpuProperties->processTimes.tms_stime -
            pStartCpuProperties->processTimes.tms_stime;
        processUserTime = pEndCpuProperties->processTimes.tms_utime -
            pStartCpuProperties->processTimes.tms_utime;
#else
        processKernelTime = 0;
        processUserTime = 0;
#endif


        /* Calculate the total process time */
        processTotalTime = pEndCpuProperties->currentTime -
            pStartCpuProperties->currentTime;

        /* Calculate the process's usage */
        if (processTotalTime && cpuCount)
        {
            processUsage =
                (double)((processUserTime + processKernelTime) * 100) /
                (double)(processTotalTime * cpuCount);
        }
        else
        {
            processUsage = 0;
        }

        /* Note: the kernel time may not be needed, if this is included in */
        /*       the user time (at least according to some manuals!) */
        /* processUsage = (double)(processUserTime * 100) / */
        /*    (double)(processTotalTime * cpuCount); */

        if (stream)
        {
            fprintf(stream, "\t%-3.2f\t%-3.2f\n", processUsage, totalUsage);
        }
        else
        {
            printf("\t\t%-3.2f%% Process CPU usage\t%-3.2f%% Total CPU usage\n",
                processUsage, totalUsage);
        }
    }

#endif


/**
 * \ingroup PerformanceTester
 * The mode in which packets are received.  This can be ORed with
 * PERFORM_TRANSMIT.
 */
#define PERFORM_RECEIVE     1U

/**
 * \ingroup PerformanceTester
 * The mode in which packets are sent.  This can be ORed with PERFORM_RECEIVE.
 */
#define PERFORM_TRANSMIT    2U

/**
 * \ingroup PerformanceTester
 * Whether a mode indicates that receives are being performed.
 */
#define PERFORMING_RECEIVES(_mode_)   (((_mode_) & PERFORM_RECEIVE) != 0U)

/**
 * \ingroup PerformanceTester
 * Whether a mode indicates that transmits are being performed.
 */
#define PERFORMING_TRANSMITS(_mode_)  (((_mode_) & PERFORM_TRANSMIT) != 0U)

/**
 * \ingroup PerformanceTester
 * Macro to determine the minimum of two numbers.
 */
#define MIN(a, b)   (((a) > (b)) ? (b) : (a))

/**
 * \ingroup PerformanceTester
 * Macro to determine the maximum of two numbers.
 */
#define MAX(a, b)   (((a) < (b)) ? (b) : (a))

/**
 * \ingroup PerformanceTester
 * The number of transmit and/or receive operations (and buffers) to use at a
 * time.
 */
#define BUFFER_NUM  4

/* The following values ensure that each test runs for at least 10 seconds */
/* (at 200 Mbit/s) */

/**
 * \ingroup PerformanceTester
 * The size of buffer to be used for each transmit and/or receive operation.
 */
#define BUFFER_SIZE 200000

/**
 * \ingroup PerformanceTester
 * The number of transmit and/or receive operations to perform in a test.
 */
#define LOOP_NUM    1000

/**
 * \ingroup PerformanceTester
 * The different types of tests that can be performed.
 */
typedef enum
{
    /** The data and packet rate test type. */
    RATE_TEST,

    /** The random packet size test type. */
    RANDOM_TEST,

    /** The latency test type. */
    LATENCY_TEST

} TEST_TYPE;

/**
 * \ingroup PerformanceTester
 * Structure to represent the properties of the tests to be performed.
 */
typedef struct
{
    /** The mode of the tests: read, write or read and write. */
    U8 mode;

    /** The type of the tests. */
    TEST_TYPE testType;

    /** The number of tests to be performed concurrently. */
    int testCount;

    /** The starting packet size. */
    int packetStart;

    /** The ending packet size. */
    int packetEnd;

    /** The amount to increment the packet size each time. */
    int packetStep;

    /**
     * The identifiers of the transmit channels to be used to transmit packets
     * for each test.
     */
    STAR_CHANNEL_ID *pTxChannelIds;

    /**
     * The address paths to be added to the front of transmitted packets for
     * each test.
     */
    STAR_SPACEWIRE_ADDRESS **pAddressPaths;

    /**
     * The identifiers of the receive channels to be used to receive packets for
     * each test.
     */
    STAR_CHANNEL_ID *pRxChannelIds;

    /** The path to a file to be used to store the results of the tests. */
    char pFilePath[MAX_PATH + 1];

    /**
     * The description to be added to the start of the file used to store the
     * results of the tests.
     */
    char pFileDescription[1000];

    /** Whether the use of channel 0 is enabled. */
    int useChannel0;

} TEST_PROPERTIES;


/**
 * \ingroup PerformanceTester
 * Prompts the user to choose which device to use for the test if more than one
 * device is available.
 *
 * @param deviceTypeStr a description of the type of device to be opened, e.g.
 *                       "transmit" or "receive"
 *
 * @return the identifier of the device to be used for the test, or 0 if there
 *         was an error or no devices are present
*/
static STAR_DEVICE_ID chooseDevice(const char * const deviceTypeStr,
    const int argCount, const char * const arguments[],
    int * const pCurrentArgument)
{
    STAR_DEVICE_ID *devices;
    U32 devCount = 0U, i, chosen;
    int status;
    STAR_DEVICE_ID deviceID;
    char *deviceName, s[256];

    /* Get the list of devices which can transmit and receive packets */
    devices = STAR_getDeviceListForType(STAR_DEVICE_TXRX_SUPPORTED, &devCount);

    /* If there was an error getting the list of devices */
    if (devices == NULL)
    {
        /* Return an error */
        puts("Error occurred detecting devices!");
        return 0U;
    }

    /* Display the devices detected */
    if (devCount == 1U)
    {
        puts("One SpaceWire device detected:");
    }
    else
    {
        printf("%u SpaceWire devices detected:\n", devCount);
    }

    /* For each device */
    for (i = 0U; i < devCount; i++)
    {

        if (devices[i] != 0U)
        {
            deviceName = STAR_getDeviceName(devices[i]);
            if (deviceName != NULL)
            {
                printf("\t%u - %s\n", i, deviceName);
                STAR_destroyString(deviceName);
            }
            else
            {
                printf("\t%u - Unknown SpaceWire Device\n", i);
            }
        }
        else
        {
            printf("\t%u - Unable to access device\n", i);
        }
    }

    /* If there's only 1 device on the list */
    if (devCount == 1U)
    {
        /* Return that device */
        deviceID = devices[0U];
        STAR_destroyDeviceList(devices);
        return deviceID;
    }

    /* Ask the user which device to use */
    printf("\nPlease select which %s device to use: ", deviceTypeStr);
    fflush(stdout);
    if ((*pCurrentArgument) < argCount)
    {
        puts(arguments[*pCurrentArgument]);
        strncpy(s, arguments[*pCurrentArgument], 255U);
        s[255U] = '\0';
        (*pCurrentArgument)++;
    }
    else
    {
        if (fgets(s, 256, stdin) == NULL)
        {
            puts("No device number selected.");
            STAR_destroyDeviceList(devices);
            return 0U;
        }
    }
    status = sscanf(s, "%u", &chosen);
    if ((status == 0) || (chosen >= devCount))
    {
        puts("Incorrect device number selected.");
        STAR_destroyDeviceList(devices);
        return 0U;
    }

    /* Return the chosen device */
    deviceID = devices[chosen];
    STAR_destroyDeviceList(devices);
    return deviceID;
}


/**
 * \ingroup PerformanceTester
 * Opens a channel to be used for a test to transmit or receive packets
 *
 * @param channelTypeStr a description of the type of channel to be opened, e.g.
 *                       "transmit" or "receive"
 * @param testNumber the number of the test to be performed.  The first test is
 *                   numbered 1
 * @param direction the direction in which the channel should be opened
 * @param lowestChannel the lowest channel number that can be used, determining
 *                      whether channel 0 is supported
 *
 * @return the channel to be used, or NULL if there was an error
 *
 */
static STAR_CHANNEL_ID openChannel(const char * const channelTypeStr,
    const int testNumber, const STAR_CHANNEL_DIRECTION direction,
    const int argCount, const char * const arguments[], int * const pCurrentArgument,
    const int lowestChannel)
{
    STAR_CHANNEL_MASK channelMask;
    int maximumChannelNumber = 0, i, channelNumber;
    STAR_DEVICE_ID deviceId;
    STAR_CHANNEL_ID channelId;

    /* Choose the device to be used */
    deviceId = chooseDevice(channelTypeStr, argCount, arguments,
        pCurrentArgument);

    /* Check for an invalid device id */
    if (deviceId == 0U)
    {
        printf("\nERROR: Failed to open %s device for test %d.\n",
            channelTypeStr, testNumber);
        return 0U;
    }

    /* Check for a device with zero channels or an invalid list of channels */
    channelMask = STAR_getDeviceChannels(deviceId);
    if (channelMask == 0U)
    {
        printf("\nERROR: The chosen %s device doesn't appear to have any valid channels.\n",
            channelTypeStr);
        return 0U;
    }

    /* For each possible channel */
    for (i = 31; i >= lowestChannel; i--)
    {
        /* If the channel exists */
        if (((channelMask >> (U32)i) & 1U) != 0U)
        {
            /* This is the maximum channel number */
            maximumChannelNumber = i;
            break;
        }
    }

    if (maximumChannelNumber < lowestChannel)
    {
        printf("\nERROR: The chosen %s device doesn't appear to have any valid channels.\n",
            channelTypeStr);
        return 0U;
    }
    else if (maximumChannelNumber == lowestChannel)
    {
        printf("\nUsing %s channel %d for test %d\n", channelTypeStr,
            lowestChannel, testNumber);
        channelNumber = lowestChannel;
    }
    else
    {
        char s[256];
        int status;

        /* Get channel and check for validity */
        printf("\nEnter %s channel (%d..%d) for test %d: ", channelTypeStr,
            lowestChannel, maximumChannelNumber, testNumber);
        fflush(stdout);
        if ((*pCurrentArgument) < argCount)
        {
            puts(arguments[*pCurrentArgument]);
            strncpy(s, arguments[*pCurrentArgument], 255U);
            s[255U] = '\0';
            (*pCurrentArgument)++;
        }
        else
        {
            if (fgets(s, 256, stdin) == NULL)
            {
                puts("\nERROR: No channel number specified");
                return 0U;
            }
        }
        status = sscanf(s, "%d", &channelNumber);
        if ((status == 0) || (channelNumber < lowestChannel) ||
            (channelNumber > maximumChannelNumber))
        {
            puts("\nERROR: Incorrect channel number specified");
            return 0U;
        }
        if (((1U << (U32)channelNumber) & channelMask) == 0U)
        {
            puts("\nERROR: The channel specified is not present");
            return 0U;
        }
    }

    /* Open the channel */
    channelId = STAR_openChannelToLocalDevice(deviceId, direction,
        (unsigned char)channelNumber, TRUE);
    if (channelId == 0U)
    {
        printf("\nFailed to open %s channel %d for test %d\n", channelTypeStr,
            channelNumber, testNumber);
        return 0U;
    }

    return channelId;
}


/**
 * \ingroup PerformanceTester
 * Closes all channels on the device passed in and frees all memory created for
 * a test.
 *
 * @param pTestProperties the test properties to be cleaned up
 */
static void cleanupTest(const TEST_PROPERTIES * const pTestProperties)
{
    int i;

    /* Close any channels which were opened */
    if (pTestProperties->pRxChannelIds != NULL)
    {
        for (i = 0; i < pTestProperties->testCount; i++)
        {
            STAR_closeChannel(pTestProperties->pRxChannelIds[i]);
        }
        free(pTestProperties->pRxChannelIds);
    }
    if (pTestProperties->pTxChannelIds != NULL)
    {
        for (i = 0; i < pTestProperties->testCount; i++)
        {
            STAR_closeChannel(pTestProperties->pTxChannelIds[i]);
        }
        free(pTestProperties->pTxChannelIds);
    }

    /* Clean up the address paths */
    if (pTestProperties->pAddressPaths != NULL)
    {
        for (i = 0; i < pTestProperties->testCount; i++)
        {
            if (pTestProperties->pAddressPaths[i] != NULL)
            {
                STAR_destroyAddress(pTestProperties->pAddressPaths[i]);
            }
        }
        free(pTestProperties->pAddressPaths);
    }
}


/**
 * \ingroup PerformanceTester
 * This function reads a SpaceWire address as a hex string from stdin. After
 * tokenizing and validating the address the function returns the
 * STAR_SPACEWIRE_ADDRESS, or NULL upon failure.
 *
 * @return the STAR_SPACEWIRE_ADDRESS, or NULL upon failure.
 */
static STAR_SPACEWIRE_ADDRESS *readSpaceWireAddress(const int argCount,
    const char * const arguments[], int * const pCurrentArgument)
{
    char s[256];
    STAR_SPACEWIRE_ADDRESS *pAddress;
    unsigned char newPath[256];
    U16 pathLen = 0U;
    char *pos = (char *)s;

    /* Read in the path */
    if ((*pCurrentArgument) < argCount)
    {
        puts(arguments[*pCurrentArgument]);
        strncpy(s, arguments[*pCurrentArgument], 255U);
        s[255U] = '\0';
        (*pCurrentArgument)++;
    }
    else
    {
        if (fgets(s, 256, stdin) == NULL)
        {
            puts("No address entered");
            return NULL;
        }
    }

    /* Read until end of buffer or line feed */
    while ((*pos != '\0') && (pathLen < 256U) && (*pos != '\n'))
    {
        unsigned long value;

        errno = 0;
        value = strtoul(pos, &pos, 16);
        if (errno != 0)
        {
            puts("Invalid value entered in address");
            return NULL;
        }

        newPath[pathLen] = (unsigned char)value;
        pathLen++;
    }

    /* Create a SpaceWire address from the path */
    pAddress = STAR_createAddress(newPath, pathLen);

    /* Return the completed address */
    return pAddress;
}


/**
 * \ingroup PerformanceTester
 * Read in the parameters of the tests to be performed from the user.
 *
 * @param pTestProperties a pointer to a structure which will be updated to
 *                        contain the properties of the test to be performed
 * @param argCount the number of parameters passed to the program (which have
 *                 not already been processed)
 * @param arguments the array of arguments passed to the program (which have not
 *                  already been processed)
 *
 * @return whether the parameters were successfully read
 */
static int readTestParameters(TEST_PROPERTIES * const pTestProperties,
    const int argCount, const char * const arguments[])
{
    char s[256], txRxStr[100];
    size_t len;
    int status, testNumber, currentArgument = 0;

    puts("STAR-System Performance Tester");
    puts("Copyright STAR-Dundee Ltd. (c) 2011-2018");
    puts("www.star-dundee.com\n");

    /* Ask the user which test they wish to perform */
    puts("Which test type do you wish to perform?");
    puts("\t(m) Maximum data rate test");
    puts("\t(r) Random packet size data rate test");
    puts("\t(l) Latency test");

    /* Read in the test type */
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(s, arguments[currentArgument], 255U);
        s[255U] = '\0';
        currentArgument++;
    }
    else
    {
        if (fgets(s, 256, stdin) == NULL)
        {
            puts("ERROR: No test type selected");
            return 0;
        }
    }
    if ((s[0U] == 'm') || (s[0U] == 'M'))
    {
        pTestProperties->testType = RATE_TEST;
    }
    else if ((s[0U] == 'r') || (s[0U] == 'R'))
    {
        pTestProperties->testType = RANDOM_TEST;
    }
    else if ((s[0U] == 'l') || (s[0U] == 'L'))
    {
        pTestProperties->testType = LATENCY_TEST;
    }
    else
    {
        puts("ERROR: Invalid test type selected");
        return 0;
    }

    /* Ask the user to how many tests they wish to perform */
    printf("\nHow many tests do you wish to perform: ");
    fflush(stdout);
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(s, arguments[currentArgument], 255U);
        s[255U] = '\0';
        currentArgument++;
    }
    else
    {
        if (fgets(s, 256, stdin) == NULL)
        {
            puts("\nERROR: No test count specified");
            return 0;
        }
    }
    status = sscanf(s, "%d", &pTestProperties->testCount);
    if ((status == 0) || (pTestProperties->testCount < 1) ||
        (pTestProperties->testCount > 100))
    {
        puts("\nERROR: No valid test count specified, enter a value between 1 and 100");
        return 0;
    }

    /* Ask the user if the test should transmit, receive or */
    /* both transmit and receive packets */
    printf("Should these tests transmit packets only (\"t\"), receive packets only (\"r\"), or both (\"b\")? ");
    fflush(stdout);
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(s, arguments[currentArgument], 255U);
        s[255U] = '\0';
        currentArgument++;
    }
    else
    {
        if (fgets(s, 256, stdin) == NULL)
        {
            puts("\nERROR: No transmit/receive option specified");
            return 0;
        }
    }
    pTestProperties->mode = 0U;
    if ((s[0U] == 't') || (s[0U] == 'T') || (s[0U] == 's') || (s[0U] == 'S') ||
        (s[0U] == 'b') || (s[0U] == 'B'))
    {
        strcpy(txRxStr, "transmit");
        pTestProperties->mode |= PERFORM_TRANSMIT;
    }
    if ((s[0U] == 'r') || (s[0U] == 'R') || (s[0U] == 'b') || (s[0U] == 'B'))
    {
        if (pTestProperties->mode != 0U)
        {
            strcat(txRxStr, "/receive");
        }
        else
        {
            strcpy(txRxStr, "receive");
        }
        pTestProperties->mode |= PERFORM_RECEIVE;
    }
    if (pTestProperties->mode == 0U)
    {
        puts("\nERROR: No valid transmit/receive option specified");
        return 0;
    }

    /* Ask the user to enter a starting packet size */
    printf("\nEnter the minimum size of packet to %s: ", txRxStr);
    fflush(stdout);
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(s, arguments[currentArgument], 255U);
        s[255U] = '\0';
        currentArgument++;
    }
    else
    {
        if (fgets(s, 256, stdin) == NULL)
        {
            puts("\nERROR: No minimum packet size specified");
            return 0;
        }
    }
    status = sscanf(s, "%d", &pTestProperties->packetStart);
    if ((status == 0) || (pTestProperties->packetStart < 1) ||
        (pTestProperties->packetStart > (1024 * 1024)))
    {
        printf("\nERROR: No valid minimum packet size specified, enter a packet size between 1 and %d\n",
            1024 * 1024);
        return 0;
    }

    /* Ask the user to enter an ending packet size */
    printf("\nEnter the maximum size of packet to %s: ", txRxStr);
    fflush(stdout);
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(s, arguments[currentArgument], 255U);
        s[255U] = '\0';
        currentArgument++;
    }
    else
    {
        if (fgets(s, 256, stdin) == NULL)
        {
            puts("\nERROR: No maximum packet size specified");
            return 0;
        }
    }
    status = sscanf(s, "%d", &pTestProperties->packetEnd);
    if ((status == 0) || (pTestProperties->packetEnd < 1) ||
        (pTestProperties->packetEnd > (1024 * 1024)))
    {
        printf("\nERROR: No valid maximum packet size specified, enter a packet size between 1 and %d\n",
            1024 * 1024);
        return 0;
    }
    if (pTestProperties->packetStart > pTestProperties->packetEnd)
    {
        puts("\nERROR: Enter a maximum packet size greater than or equal to the minimum packet size");
        return 0;
    }

    /* Ask the user to enter the number of bytes to increment the packet size */
    /* by in each loop */
    if (pTestProperties->packetStart == pTestProperties->packetEnd)
    {
        pTestProperties->packetStep = 1;
    }
    else
    {
        printf(
            "\nEnter the number of bytes to increment the packet size by in each loop (0 for log-like pattern): ");
        fflush(stdout);
        if (currentArgument < argCount)
        {
            puts(arguments[currentArgument]);
            strncpy(s, arguments[currentArgument], 255U);
            s[255U] = '\0';
            currentArgument++;
        }
        else
        {
            if (fgets(s, 256, stdin) == NULL)
            {
                puts("\nERROR: No packet size increment specified");
                return 0;
            }
        }
        status = sscanf(s, "%d", &pTestProperties->packetStep);
        if ((status == 0) || (pTestProperties->packetStep < 0))
        {
            puts("\nERROR: No valid packet size increment specified");
            return 0;
        }
    }

    /* Allocate memory for the channels */
    if (PERFORMING_TRANSMITS(pTestProperties->mode))
    {
        pTestProperties->pTxChannelIds = (STAR_CHANNEL_ID *)calloc(
            pTestProperties->testCount, sizeof(STAR_CHANNEL_ID));
        if (pTestProperties->pTxChannelIds == NULL)
        {
            puts(
                "\nERROR: Unable to allocate memory for the transmit channels");
            return 0;
        }
        pTestProperties->pAddressPaths = (STAR_SPACEWIRE_ADDRESS **)calloc(
            pTestProperties->testCount, sizeof(STAR_SPACEWIRE_ADDRESS *));
        if (pTestProperties->pAddressPaths == NULL)
        {
            puts(
                "\nERROR: Unable to allocate memory for the address paths");
            return 0;
        }
    }
    if (PERFORMING_RECEIVES(pTestProperties->mode))
    {
        pTestProperties->pRxChannelIds = (STAR_CHANNEL_ID *)calloc(
            pTestProperties->testCount, sizeof(STAR_CHANNEL_ID));
        if (pTestProperties->pRxChannelIds == NULL)
        {
            puts("\nERROR: Unable to allocate memory for the receive channels");
            return 0;
        }
    }

    /* For each test to perform */
    for (testNumber = 0; testNumber < pTestProperties->testCount; testNumber++)
    {
        /* If transmitting packets */
        if (PERFORMING_TRANSMITS(pTestProperties->mode))
        {
            /* Choose the transmit channel */
            pTestProperties->pTxChannelIds[testNumber] = openChannel("transmit",
                testNumber + 1, STAR_CHANNEL_DIRECTION_OUT, argCount, arguments,
                &currentArgument, (pTestProperties->useChannel0 != 0) ? 0 : 1);
            if (pTestProperties->pTxChannelIds[testNumber] == 0U)
            {
                return 0;
            }

            /* Ask the user to enter the path to add to the front of packets */
            printf("\nEnter the path to add to the front of packets sent for test %d:\n",
                testNumber);
            puts("(Values should be in hex, separated by a space, i.e.: 01 0f 02)");

            /* Read in the path */
            pTestProperties->pAddressPaths[testNumber] = readSpaceWireAddress(
                argCount, arguments, &currentArgument);
        }

        /* If receiving packets */
        if (PERFORMING_RECEIVES(pTestProperties->mode))
        {
            /* Choose the receive channel */
            pTestProperties->pRxChannelIds[testNumber] = openChannel("receive",
                testNumber + 1, STAR_CHANNEL_DIRECTION_IN, argCount, arguments,
                &currentArgument, (pTestProperties->useChannel0 != 0) ? 0 : 1);
            if (pTestProperties->pRxChannelIds[testNumber] == 0U)
            {
                return 0;
            }
        }
    }

    /* Ask the user to enter the path to the file to use, */
    /* or to press enter to not store to a file */
    printf("\nEnter the path and the file to be used to store the results (hit enter if the results should not be stored): ");
    fflush(stdout);

    /* Read in the file path */
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(pTestProperties->pFilePath, arguments[currentArgument],
            MAX_PATH);
        pTestProperties->pFilePath[MAX_PATH] = '\0';
        currentArgument++;
    }
    else
    {
        if (fgets(pTestProperties->pFilePath, MAX_PATH, stdin) == NULL)
        {
            strcpy(pTestProperties->pFilePath, "");
        }
    }

    len = strlen(pTestProperties->pFilePath);
    if (len != 0U)
    {
        while ((len > 0U) &&
            ((pTestProperties->pFilePath[len - 1] == '\r') ||
            (pTestProperties->pFilePath[len - 1] == '\n')))
        {
            pTestProperties->pFilePath[len - 1] = '\0';
            len--;
        }
    }

    /* if writing the results to file */
    if (len > 0U)
    {
        printf("\nEnter a description of the test: ");
        fflush(stdout);
        if (currentArgument < argCount)
        {
            puts(arguments[currentArgument]);
            strncpy(pTestProperties->pFileDescription,
                arguments[currentArgument], 999U);
            pTestProperties->pFileDescription[999U] = '\0';
        }
        else
        {
            if (fgets(pTestProperties->pFileDescription, 999U, stdin) == NULL)
            {
                strcpy(pTestProperties->pFileDescription, "");
            }
        }
    }

    puts("\n");

    return 1;
}


/**
 * \ingroup PerformanceTester
 * Perform the rate test for a specific packet size.
 *
 * @param packetSize the size of packet to transmit/receive
 * @param mode the mode of operation (receive and/or transmit)
 * @param testCount the number of tests to perform concurrently
 * @param pTxChannels the channels to transmit on
 * @param pAddressPaths the addresses to add to the start of all packets sent,
 *                      if packets are to be sent
 * @param pRxChannels the channels to receive on
 * @param pDataRate a pointer to a variable which will be updated to contain the
 *                  data rate recorded for the test
 * @param pPacketRate a pointer to a variable which will be updated to contain
 *                    the packet rate recorded for the test
 * @param pPacketTime a pointer to a variable which will be updated to contain
 *                    the average time to transmit a single packet
 * @param pStartCpuProperties a pointer to a structure which will be updated to
 *                            contain the CPU properties when the test is
 *                            started
 * @param pEndCpuProperties a pointer to a structure which will be updated to
 *                          contain the CPU properties when the test completes
 */
static void performRateTestForPacketSize(const int packetSize, const U8 mode,
    const int testCount, const STAR_CHANNEL_ID * const pTxChannels,
    STAR_SPACEWIRE_ADDRESS ** const pAddressPaths,
    const STAR_CHANNEL_ID * const pRxChannels, double * const pDataRate,
    double * const pPacketRate, double * const pPacketTime,
    CPU_USAGE_PROPERTIES * const pStartCpuProperties,
    CPU_USAGE_PROPERTIES * const pEndCpuProperties)
{
    int i, j, bufferNum, buffersUsed, packetNum, loopNum;
    STAR_TRANSFER_OPERATION **ppRxTransOp = NULL, **ppTxTransOp = NULL;
    clock_t start = 0, finish;
    double duration, bitsSent;
    STAR_TRANSFER_STATUS status = STAR_TRANSFER_STATUS_ERROR;
    unsigned char *pTransmitBuffer = NULL;
    STAR_STREAM_ITEM **ppTxPackets = NULL;
    unsigned int packetItem;
    unsigned int rxPackSize;
    unsigned int receivedItemCount;
    STAR_STREAM_ITEM** receivedItems;
    STAR_SPACEWIRE_PACKET* pReceivedPacket;
    STAR_EOP_TYPE rxEopType;

    /* Calculate the number of packets to transmit in each loop and */
    /* the number of loops to perform */
    packetNum = MAX(BUFFER_NUM, BUFFER_SIZE / packetSize);
    loopNum = LOOP_NUM;

#ifdef DEBUG
    printf("\nPackets per loop:\t%d", packetNum);
    printf("\nNumber of loops:\t%d", loopNum);
    printf("\nBytes per packet:\t%d", packetSize);
    printf("\nExpected packets:\t%u", loopNum * packetNum);
    printf("\nExpected bytes:\t%f\n",
        (double)packetSize * (double)packetNum * (double)loopNum);
    printf("\n");
#endif

    /* if performing receives */
    if (PERFORMING_RECEIVES(mode))
    {
        /* Allocate memory for the receive transfer operations */
        ppRxTransOp = (STAR_TRANSFER_OPERATION **)calloc(BUFFER_NUM * testCount,
            sizeof(STAR_TRANSFER_OPERATION *));
        if (ppRxTransOp == NULL)
        {
            puts("Couldn't allocate memory for receive operations, exiting.");
            goto Rate_End_Cleanup;
        }

        /* Create transfer operations to receive all packets in each buffer */
        for (i = 0; i < (BUFFER_NUM * testCount); i++)
        {
            ppRxTransOp[i] =
                STAR_createRxOperation(packetNum, STAR_RECEIVE_PACKETS);
            if (ppRxTransOp[i] == NULL)
            {
                puts("Couldn't create receive operation, exiting.");
                goto Rate_End_Cleanup;
            }
        }
    }

    /* if performing transmits */
    if (PERFORMING_TRANSMITS(mode))
    {
        /* Allocate transmit buffer */
        /* This buffer will be reused for all packets sent */
        pTransmitBuffer = (unsigned char *)calloc(1U, packetSize);
        if (pTransmitBuffer == NULL)
        {
            puts("Couldn't allocate memory for transmit buffer, exiting.");
            goto Rate_End_Cleanup;
        }

        /* Set the first byte of the transmit buffer to 0xfe.  This is the */
        /* byte that will be at the front of the packet at the destination */
        /* (if path addressing is used) */
        if (packetSize > 0)
        {
            pTransmitBuffer[0U] = 0xfeU;
        }

        /* Allocate memory for packet pointers */
        ppTxPackets = (STAR_STREAM_ITEM **)calloc(
            packetNum * BUFFER_NUM * testCount, sizeof(STAR_STREAM_ITEM *));
        if (ppTxPackets == NULL)
        {
            puts("Couldn't allocate memory for transmit packets, exiting.");
            goto Rate_End_Cleanup;
        }

        /* Allocate memory for the transmit identifiers */
        ppTxTransOp = (STAR_TRANSFER_OPERATION **)calloc(BUFFER_NUM * testCount,
            sizeof(STAR_TRANSFER_OPERATION *));
        if (ppTxTransOp == NULL)
        {
            puts("Couldn't allocate memory for transmit operations, exiting.");
            goto Rate_End_Cleanup;
        }

        /* Construct packets */
        for (i = 0; i < (packetNum * BUFFER_NUM); i++)
        {
            for (j = 0; j < testCount; j++)
            {
                /* Create stream item using the transmit buffer */
                ppTxPackets[(i * testCount) + j] = STAR_createPacket(
                    pAddressPaths[j], pTransmitBuffer, packetSize,
                    STAR_EOP_TYPE_EOP);
                if (ppTxPackets[(i * testCount) + j] == NULL)
                {
                    puts("Couldn't create packet for transmit operation, exiting.");
                    goto Rate_End_Cleanup;
                }
            }
        }

        /* Create transfer operations to transmit all packets in each buffer */
        for (i = 0; i < (BUFFER_NUM * testCount); i++)
        {
            ppTxTransOp[i] = STAR_createTxOperation(
                ppTxPackets + (packetNum * i), packetNum);
            if (ppTxTransOp[i] == NULL)
            {
                puts("Couldn't create transmit operation, exiting.");
                goto Rate_End_Cleanup;
            }
        }

        /* Start the clock */
        start = GET_TIME();
        STORE_CPU_USAGE(pStartCpuProperties);
    }

    bufferNum = 0;

    /* for each loop to perform */
    for (i = 0; i < loopNum; i++)
    {
        /* If performing receives */
        if (PERFORMING_RECEIVES(mode))
        {
            /* If a receive has already been submitted for this buffer */
            if (i >= BUFFER_NUM)
            {
                for (j = 0; j < testCount; j++)
                {
                    /* Wait on packets being received */
                    status = STAR_waitOnTransferOperationCompletion(
                        ppRxTransOp[(bufferNum * testCount) + j],
                        STAR_INFINITE);

                    receivedItems = STAR_getTransferItemList(
                        ppRxTransOp[(bufferNum * testCount) + j],
                        &receivedItemCount);
                    if (status != STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        printf("ERROR: Could not complete receive in loop %d, error of %d\n",
                            i, status);
                        goto Rate_End_Cleanup;
                    }

                    if (receivedItems == NULL)
                    {
                         printf("ERROR: Receive completed but no data was received. Loop: %d\n",
                             i);
                         goto Rate_End_Cleanup;
                    }

                    for (packetItem = 0U; packetItem < receivedItemCount;
                        packetItem++)
                    {
                        pReceivedPacket = (STAR_SPACEWIRE_PACKET*)receivedItems[
                            packetItem]->item;

                        rxPackSize = STAR_getPacketLength(pReceivedPacket);
                        if (rxPackSize != (unsigned int)packetSize)
                        {
                            printf("Rx operation completed, received incorrect packet size: %u, Loop %d \n",
                                rxPackSize, i);
                        }

                        rxEopType = STAR_getPacketEOP(pReceivedPacket);
                        if (rxEopType != STAR_EOP_TYPE_EOP)
                        {
                            printf("Rx operation completed, received unexpected EOP type: %d, Loop %d \n",
                                rxEopType, i);
                        }
                    }
                    STAR_destroyTransferItemList(receivedItems,
                        receivedItemCount, 0);
                }

                /* If this is the first receive and not performing transmits */
                if ((i == BUFFER_NUM) && (!PERFORMING_TRANSMITS(mode)))
                {
                    /* Start the clock */
                    start = GET_TIME();
                    STORE_CPU_USAGE(pStartCpuProperties);
                }
            }

            /* Start receiving the next group of packets */
            for (j = 0; j < testCount; j++)
            {
                STAR_submitTransferOperation(pRxChannels[j],
                    ppRxTransOp[(bufferNum * testCount) + j]);

                status = STAR_getTransferOperationStatus(ppRxTransOp[
                    (bufferNum * testCount) + j]);
                if ((status != STAR_TRANSFER_STATUS_STARTED) &&
                    (status != STAR_TRANSFER_STATUS_COMPLETE))
                {
                    printf("Could not perform receive in loop %d, error of %d\n",
                            i, status);
                    goto Rate_End_Cleanup;
                }
            }
        }

        /* if performing transmits */
        if (PERFORMING_TRANSMITS(mode))
        {
            /* If a transmit has already been submitted for this buffer */
            if (i >= BUFFER_NUM)
            {
                /* Wait on the last transmit completing */
                for (j = 0; j < testCount; j++)
                {
                    status = STAR_waitOnTransferOperationCompletion(
                        ppTxTransOp[(bufferNum * testCount) + j],
                        STAR_INFINITE);

                    if (status != STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        printf("ERROR: Could not complete transmit in loop %d, error of %d\n",
                            i, status);
                        goto Rate_End_Cleanup;
                    }
                }
            }

            /* Start transmitting the next packets */
            for (j = 0; j < testCount; j++)
            {
                STAR_submitTransferOperation(pTxChannels[j],
                    ppTxTransOp[(bufferNum * testCount) + j]);

                status = STAR_getTransferOperationStatus(
                    ppTxTransOp[(bufferNum * testCount) + j]);
                if ((status != STAR_TRANSFER_STATUS_STARTED) &&
                    (status != STAR_TRANSFER_STATUS_COMPLETE))
                {
                    printf("Could not perform transmit in loop %d, error of %d\n",
                            i, status);
                    goto Rate_End_Cleanup;
                }
            }
        }

        /* Move to the next buffer */
        bufferNum++;
        if (bufferNum == BUFFER_NUM)
        {
            bufferNum = 0;
        }
    }

    /* Account for LOOP_NUM less than BUFFER_NUM */
    buffersUsed = BUFFER_NUM;
    #if(LOOP_NUM < BUFFER_NUM)
        buffersUsed = LOOP_NUM;
    #endif
    /* For each buffer used */
    for (bufferNum = 0; bufferNum < buffersUsed; bufferNum++)
    {
        /* If performing transmits */
        if (PERFORMING_TRANSMITS(mode))
        {
            /* Wait on the next transmit completing */
            for (j = 0; j < testCount; j++)
            {
                status = STAR_waitOnTransferOperationCompletion(
                    ppTxTransOp[(bufferNum * testCount) + j], STAR_INFINITE);
                if (status != STAR_TRANSFER_STATUS_COMPLETE)
                {
                    printf("ERROR: Could not complete transmit in loop %d, error of %d\n",
                        i, status);
                    goto Rate_End_Cleanup;
                }
            }
        }

        /* If performing receives */
        if (PERFORMING_RECEIVES(mode))
        {
            /* Wait on packets being received */
            for (j = 0; j < testCount; j++)
            {
                status = STAR_waitOnTransferOperationCompletion(
                    ppRxTransOp[(bufferNum * testCount) + j], STAR_INFINITE);

                receivedItems = STAR_getTransferItemList(
                    ppRxTransOp[(bufferNum * testCount) + j],
                    &receivedItemCount);

                if (receivedItems == NULL)
                {
                     printf("ERROR: Receive completed but no data was received. Loop: %d\n",
                         loopNum);
                     goto Rate_End_Cleanup;
                }

                for (packetItem = 0U; packetItem < receivedItemCount;
                    packetItem++)
                {
                    pReceivedPacket =
                        (STAR_SPACEWIRE_PACKET*)receivedItems[packetItem]->item;

                    rxPackSize = STAR_getPacketLength(pReceivedPacket);
                    if (rxPackSize != (unsigned int)packetSize)
                    {
                        printf("Rx operation completed, received incorrect packet size: %u, Loop %d \n",
                            rxPackSize, loopNum);
                    }

                    rxEopType = STAR_getPacketEOP(pReceivedPacket);
                    if (rxEopType != STAR_EOP_TYPE_EOP)
                    {
                        printf("Rx operation completed, received unexpected EOP type: %d, Loop %d \n",
                            rxEopType, loopNum);
                    }
                }
                STAR_destroyTransferItemList(receivedItems, receivedItemCount,
                    0);

                if (status != STAR_TRANSFER_STATUS_COMPLETE)
                {
                    printf("ERROR: Could not complete receive in loop %d, error of %d\n",
                            i, status);
                    goto Rate_End_Cleanup;
                }
            }
        }
    }

    /* Stop the clock */
    finish = GET_TIME();
    STORE_CPU_USAGE(pEndCpuProperties);

    duration = (double)(finish - start) / TIME_DIVIDER;

    bitsSent = (double)testCount * (double)packetSize;
    if (pAddressPaths != NULL)
    {
        for (i = 0; i < testCount; i++)
        {
            if (pAddressPaths[i] != NULL)
            {
                bitsSent += STAR_getSpaceWireAddressPathLength(pAddressPaths[i]);
            }
        }
    }
    bitsSent = bitsSent * (double)packetNum * (double)loopNum * (double)8;

    /* Calculate the data and packet rate */
    if (duration > 0.0)
    {
        *pDataRate = (bitsSent / duration) / 1000000.0;
        *pPacketRate = (double)(packetNum * testCount * loopNum) / duration;
    }
    else
    {
        *pDataRate = 0.0;
        *pPacketRate = 0.0;
    }
    *pPacketTime = (duration * 1000000.0) /
        (double)(packetNum * testCount * loopNum);

/* Clean-up */
Rate_End_Cleanup:

    /* If performing receives */
    if (PERFORMING_RECEIVES(mode))
    {
        /* Dispose of all receive transfer operations */
        if (ppRxTransOp != NULL)
        {
            for (i = 0; i < (BUFFER_NUM * testCount); i++)
            {
                if (ppRxTransOp[i] != NULL)
                {
                    STAR_disposeTransferOperation(ppRxTransOp[i]);
                }
            }

            free(ppRxTransOp);
        }
    }

    /* If performing transmits */
    if (PERFORMING_TRANSMITS(mode))
    {
        if (ppTxTransOp != NULL)
        {
            /* Dispose of all transmit transfer operations */
            for (i = 0; i < (BUFFER_NUM * testCount); i++)
            {
                if (ppTxTransOp[i] != NULL)
                {
                    STAR_disposeTransferOperation(ppTxTransOp[i]);
                }
            }
        }

        if (ppTxPackets != NULL)
        {
            /* Dispose of stream item buffer */
            for (i = 0; i < (packetNum * BUFFER_NUM * testCount); i++)
            {
                if (ppTxPackets[i] != NULL)
                {
                    STAR_destroyStreamItem(ppTxPackets[i]);
                }
            }

            /* Free the packet pointer array*/
            free(ppTxPackets);
        }

        /* Free the transfer operation pointers */
        if (ppTxTransOp != NULL)
        {
            free(ppTxTransOp);
        }

        /* Free the transmit buffer */
        if (pTransmitBuffer != NULL)
        {
            free(pTransmitBuffer);
        }
    }

    if (status != STAR_TRANSFER_STATUS_COMPLETE)
    {
        *pDataRate = 0.0;
        *pPacketRate = 0.0;
        *pPacketTime = 0.0;
    }
}


/**
 * \ingroup PerformanceTester
 * Perform the rate test.
 *
 * @param pTestProperties a pointer to a structure containing the properties of
 *                        the test to be performed
 * @param pOutputFile the file to write the statistics to, or NULL if no file is
 *                    to be used
 */
static void performRateTest(const TEST_PROPERTIES * const pTestProperties,
    FILE * const pOutputFile)
{
    int packetSize;
    double dataRate = 0, packetRate = 0, packetTime = 0;
    CPU_USAGE_PROPERTIES startCpuProperties, endCpuProperties;

    /* If a file is to be written to */
    if (pOutputFile != NULL)
    {
        /* Write the statistics column headers to the file */
        fprintf(pOutputFile, "Packet Size\tData Rate (Mbit/s)\t");
        fprintf(pOutputFile,
            "Packet Rate (packets/s)\tAverage Packet Time (microseconds)");
        PRINT_CPU_USAGE_HEADERS(pOutputFile);
    }

    /* for each packet size to test */
    packetSize = pTestProperties->packetStart;
    while (packetSize <= pTestProperties->packetEnd)
    {
        /* Display the packet size on screen */
        printf(
            "Performing maximum data rate test using packets of %d bytes...\n",
            packetSize);

        /* Perform the test for the current packet size */
        performRateTestForPacketSize(packetSize, pTestProperties->mode,
            pTestProperties->testCount, pTestProperties->pTxChannelIds,
            pTestProperties->pAddressPaths, pTestProperties->pRxChannelIds,
            &dataRate, &packetRate, &packetTime, &startCpuProperties,
            &endCpuProperties);

        /* Display the results of the test */
        printf(
            "\t%-3.2f Mbit/s  %-3.2f packets/s  %-3.2f microseconds/packet\n",
            dataRate, packetRate, packetTime);
        PRINT_CPU_USAGE(NULL, &startCpuProperties, &endCpuProperties);

        /* If writing the results to file */
        if (pOutputFile != NULL)
        {
            /* Write the results to the file */
            fprintf(pOutputFile, "%8d\t%-3.2f\t%-3.2f\t%-3.2f", packetSize,
                dataRate, packetRate, packetTime);
            PRINT_CPU_USAGE(pOutputFile, &startCpuProperties,
                &endCpuProperties);
            fflush(pOutputFile);
        }

        /* if not performing receives and this isn't the last loop */
        if ((!PERFORMING_RECEIVES(pTestProperties->mode)) &&
            (packetSize < pTestProperties->packetEnd))
        {
            /* Sleep for 5 seconds to allow all packets to be sent */
            SLEEP(5000U);
        }

        /* Move to the next packet size */
        if (packetSize == pTestProperties->packetEnd)
        {
            packetSize++;
        }
        else if (pTestProperties->packetStep == 0)
        {
            packetSize = MIN(2 * packetSize, pTestProperties->packetEnd);
        }
        else
        {
            packetSize = MIN(packetSize + pTestProperties->packetStep,
                pTestProperties->packetEnd);
        }
    }
}


/**
 * \ingroup PerformanceTester
 * Perform the random test for a specific packet size.
 *
 * @param minPacketSize the minimum size of packet to transmit/receive
 * @param maxPacketSize the maximum size of packet to transmit/receive
 * @param mode the mode of operation (receive and/or transmit)
 * @param testCount the number of tests to perform concurrently
 * @param pTxChannels the channels to transmit on
 * @param pAddressPaths the addresses to add to the start of all packets sent,
 *                      if packets are to be sent
 * @param pRxChannels the channels to receive on
 * @param pDataRate a pointer to a variable which will be updated to contain the
 *                  data rate recorded for the test
 * @param pPacketRate a pointer to a variable which will be updated to contain
 *                    the packet rate recorded for the test
 * @param pPacketTime a pointer to a variable which will be updated to contain
 *                    the average time to transmit a single packet
 * @param pPacketSize a pointer to a variable which will be updated to contain
 *                    the average packet size used in the test
 * @param pStartCpuProperties a pointer to a structure which will be updated to
 *                            contain the CPU properties when the test is
 *                            started
 * @param pEndCpuProperties a pointer to a structure which will be updated to
 *                          contain the CPU properties when the test completes
 */
static void performRandomTestForPacketSize(const int minPacketSize,
    const int maxPacketSize, const U8 mode, const int testCount,
    const STAR_CHANNEL_ID * const pTxChannels,
    STAR_SPACEWIRE_ADDRESS ** const pAddressPaths,
    const STAR_CHANNEL_ID * const pRxChannels, double * const pDataRate,
    double * const pPacketRate, double * const pPacketTime,
    double * const pPacketSize,
    CPU_USAGE_PROPERTIES * const pStartCpuProperties,
    CPU_USAGE_PROPERTIES * const pEndCpuProperties)
{
    int i, j, bufferNum, buffersUsed, packetNum, loopNum;
    STAR_TRANSFER_OPERATION **ppRxTransOp = NULL, **ppTxTransOp = NULL;
    clock_t start = 0, finish;
    double duration, bytesSent = 0.0;
    STAR_TRANSFER_STATUS status = STAR_TRANSFER_STATUS_ERROR;
    unsigned char *pTransmitBuffer = NULL;
    STAR_STREAM_ITEM **ppTxPackets = NULL;
    int *pPacketLengths = NULL;

    /* Seed random number function */
    srand((unsigned int)time(NULL));

    /* Calculate the number of packets to transmit in each loop and */
    /* the number of loops to perform */
    packetNum = MAX(BUFFER_NUM, BUFFER_SIZE / maxPacketSize);
    loopNum = LOOP_NUM;

    /* Allocate memory to hold length info for packets */
    pPacketLengths = (int *)calloc(packetNum * BUFFER_NUM, sizeof(int));
    if (pPacketLengths == NULL)
    {
        puts("Couldn't allocate memory for the packet lengths, exiting.");
        goto Random_End_Cleanup;
    }

    for (i = 0; i < (packetNum * BUFFER_NUM); i++)
    {
        /* Set the initial random lengths for the packets */
        pPacketLengths[i] = ((int)(((double)rand() / ((double)RAND_MAX + 1.0)) *
            (maxPacketSize - minPacketSize))) + minPacketSize;
    }

    /* if performing receives */
    if (PERFORMING_RECEIVES(mode))
    {
        /* Allocate memory for the receive transfer operations */
        ppRxTransOp = (STAR_TRANSFER_OPERATION **)calloc(BUFFER_NUM * testCount,
            sizeof(STAR_TRANSFER_OPERATION *));
        if (ppRxTransOp == NULL)
        {
            puts("Couldn't allocate memory for receive operations, exiting.");
            goto Random_End_Cleanup;
        }

        /* Create transfer operations to receive all packets in each buffer */
        for (i = 0; i < (BUFFER_NUM * testCount); i++)
        {
            ppRxTransOp[i] =
                STAR_createRxOperation(packetNum, STAR_RECEIVE_PACKETS);
            if (ppRxTransOp[i] == NULL)
            {
                puts("Couldn't create receive operation, exiting.");
                goto Random_End_Cleanup;
            }
        }
    }

    /* if performing transmits */
    if (PERFORMING_TRANSMITS(mode))
    {
        /* Allocate transmit buffer */
        pTransmitBuffer = (unsigned char *)calloc(1U, maxPacketSize);
        if (pTransmitBuffer == NULL)
        {
            puts("Couldn't allocate memory for transmit buffer, exiting.");
            goto Random_End_Cleanup;
        }

        /* Allocate memory for packet pointers */
        ppTxPackets = (STAR_STREAM_ITEM **)calloc(
            packetNum * BUFFER_NUM * testCount, sizeof(STAR_STREAM_ITEM *));
        if (ppTxPackets == NULL)
        {
            puts("Couldn't allocate memory for transmit packets, exiting.");
            goto Random_End_Cleanup;
        }

        /* Set the first byte of the transmit buffer to 0xfe.  This is the */
        /* byte that will be at the front of the packet at the destination */
        /* (if path addressing is used) */
        if (maxPacketSize > 0)
        {
            pTransmitBuffer[0U] = 0xfeU;
        }

        /* Allocate memory for the transmit identifiers */
        ppTxTransOp = (STAR_TRANSFER_OPERATION **)calloc(BUFFER_NUM * testCount,
            sizeof(STAR_TRANSFER_OPERATION *));
        if (ppTxTransOp == NULL)
        {
            puts("Couldn't allocate memory for transmit operations, exiting.");
            goto Random_End_Cleanup;
        }

        /* Construct packets */
        for (i = 0; i < (packetNum * BUFFER_NUM); i++)
        {
            for (j = 0; j < testCount; j++)
            {
                /* Create stream item using the transmit buffer */
                ppTxPackets[(i * testCount) + j] = STAR_createPacket(
                    pAddressPaths[j], pTransmitBuffer, pPacketLengths[i],
                    STAR_EOP_TYPE_EOP);
                if (ppTxPackets[(i * testCount) + j] == NULL)
                {
                    puts("Couldn't create packets for transmit operations, exiting.");
                    goto Random_End_Cleanup;
                }
            }
        }

        /* Create transfer operations to transmit all packets in each buffer */
        for (i = 0; i < (BUFFER_NUM * testCount); i++)
        {
            ppTxTransOp[i] = STAR_createTxOperation(
                ppTxPackets + (packetNum * i), packetNum);
            if (ppTxTransOp[i] == NULL)
            {
                puts("Couldn't create transmit operations, exiting.");
                goto Random_End_Cleanup;
            }
        }

        /* Start the clock */
        start = GET_TIME();
        STORE_CPU_USAGE(pStartCpuProperties);
    }

    bufferNum = 0;

    /* for each loop to perform */
    for (i = 0; i < loopNum; i++)
    {
        /* If performing receives */
        if (PERFORMING_RECEIVES(mode))
        {
            /* If a receive has already been submitted for this buffer */
            if (i >= BUFFER_NUM)
            {
                for (j = 0; j < testCount; j++)
                {
                    /* Wait on packets being received */
                    status = STAR_waitOnTransferOperationCompletion(
                        ppRxTransOp[(bufferNum * testCount) + j],
                        STAR_INFINITE);
                    if (status != STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        printf("ERROR: Could not complete receive in loop %d, error of %d\n",
                            i, status);
                        goto Random_End_Cleanup;
                    }
                }

                /* If this is the first receive and not performing transmits */
                if ((i == BUFFER_NUM) && (!PERFORMING_TRANSMITS(mode)))
                {
                    /* Start the clock */
                    start = GET_TIME();
                    STORE_CPU_USAGE(pStartCpuProperties);
                }
            }

            /* Start receiving the next group of packets */
            for (j = 0; j < testCount; j++)
            {
                STAR_submitTransferOperation(pRxChannels[j],
                    ppRxTransOp[(bufferNum * testCount) + j]);

                status = STAR_getTransferOperationStatus(
                    ppRxTransOp[(bufferNum * testCount) + j]);
                if ((status != STAR_TRANSFER_STATUS_STARTED) &&
                    (status != STAR_TRANSFER_STATUS_COMPLETE))
                {
                    printf("Could not perform receive in loop %d, error of %d\n",
                            i, status);
                    goto Random_End_Cleanup;
                }
            }
        }

        /* if performing transmits */
        if (PERFORMING_TRANSMITS(mode))
        {
            /* If a transmit op has already been submitted for this buffer */
            if (i >= BUFFER_NUM)
            {
                /* Wait on the last transmit completing */
                for (j = 0; j < testCount; j++)
                {
                    status = STAR_waitOnTransferOperationCompletion(
                        ppTxTransOp[(bufferNum * testCount) + j], STAR_INFINITE);

                    if (status != STAR_TRANSFER_STATUS_COMPLETE)
                    {
                        printf("ERROR: Could not complete transmit in loop %d, error of %d\n",
                            i, status);
                        goto Random_End_Cleanup;
                    }
                }
            }
        }

        /* Update the number of bytes transmitted/received */
        for (j = 0; j < packetNum; j++)
        {
            bytesSent +=
                (pPacketLengths[(packetNum * bufferNum) + j] * testCount);
        }
        if (pAddressPaths != NULL)
        {
            for (j = 0; j < testCount; j++)
            {
                if (pAddressPaths[j] != NULL)
                {
                    bytesSent += (packetNum * STAR_getSpaceWireAddressPathLength(pAddressPaths[j]));
                }
            }
        }

        /* if performing transmits */
        if (PERFORMING_TRANSMITS(mode))
        {
            /* Note that this current version of the Performance Tester does */
            /* not change the size of the data in each loop.  This is due to */
            /* an as yet unfixed bug in STAR_setPacketData */

            /* Start transmitting the next packets */
            for (j = 0; j < testCount; j++)
            {
                STAR_submitTransferOperation(pTxChannels[j],
                    ppTxTransOp[(bufferNum * testCount) + j]);

                status = STAR_getTransferOperationStatus(
                    ppTxTransOp[(bufferNum * testCount) + j]);
                if ((status != STAR_TRANSFER_STATUS_STARTED) &&
                    (status != STAR_TRANSFER_STATUS_COMPLETE))
                {
                    printf("Could not perform transmit in loop %d, error of %d\n",
                            i, status);
                    goto Random_End_Cleanup;
                }
            }
        }

        /* Move to the next buffer */
        bufferNum++;
        if (bufferNum == BUFFER_NUM)
        {
            bufferNum = 0;
        }
    }

    /* Account for LOOP_NUM less than BUFFER_NUM */
    buffersUsed = BUFFER_NUM;
    #if(LOOP_NUM < BUFFER_NUM)
        buffersUsed = LOOP_NUM;
    #endif

    /* For each buffer used */
    for (bufferNum = 0; bufferNum < buffersUsed; bufferNum++)
    {
        /* If performing transmits */
        if (PERFORMING_TRANSMITS(mode))
        {
            /* Wait on the next transmit completing */
            for (j = 0; j < testCount; j++)
            {
                status = STAR_waitOnTransferOperationCompletion(
                    ppTxTransOp[(bufferNum * testCount) + j], STAR_INFINITE);
                if (status != STAR_TRANSFER_STATUS_COMPLETE)
                {
                    printf("ERROR: Could not complete transmit in loop %d, error of %d\n",
                        i, status);
                    goto Random_End_Cleanup;
                }
            }
        }

        /* If performing receives */
        if (PERFORMING_RECEIVES(mode))
        {
            /* Wait on packets being received */
            for (j = 0; j < testCount; j++)
            {
                status = STAR_waitOnTransferOperationCompletion(
                    ppRxTransOp[(bufferNum * testCount) + j], STAR_INFINITE);
                if (status != STAR_TRANSFER_STATUS_COMPLETE)
                {
                    printf("ERROR: Could not complete receive in loop %d, error of %d\n",
                            i, status);
                    goto Random_End_Cleanup;
                }
            }
        }
    }

    /* Stop the clock */
    finish = GET_TIME();
    STORE_CPU_USAGE(pEndCpuProperties);

    duration = (double)(finish - start) / TIME_DIVIDER;

    /* Calculate the average packet size */
    *pPacketSize = (double)bytesSent / (packetNum * testCount * loopNum);

    /* Calculate the data and packet rate */
    if (duration > 0.0)
    {
        *pDataRate = ((bytesSent * 8.0) / duration) / 1000000.0;
        *pPacketRate = (double)(packetNum * testCount * loopNum) / duration;
    }
    else
    {
        *pDataRate = 0.0;
        *pPacketRate = 0.0;
    }

    *pPacketTime = (duration * 1000000.0) /
        (double)(packetNum * testCount * loopNum);

/* Clean-up */
Random_End_Cleanup:

    /* If performing receives */
    if (PERFORMING_RECEIVES(mode))
    {
        if (ppRxTransOp != NULL)
        {
            for (i = 0; i < (BUFFER_NUM * testCount); i++)
            {
                /* Dispose of all receive transfer operations */
                if (ppRxTransOp[i] != NULL)
                {
                    STAR_disposeTransferOperation(ppRxTransOp[i]);
                }
            }

            free(ppRxTransOp);
        }
    }

    /* If performing transmits */
    if (PERFORMING_TRANSMITS(mode))
    {
        if (ppTxTransOp != NULL)
        {
            /* Dispose of all transmit transfer operations */
            for (i = 0; i < (BUFFER_NUM * testCount); i++)
            {
                if (ppTxTransOp[i] != NULL)
                {
                    STAR_disposeTransferOperation(ppTxTransOp[i]);
                }
            }
        }

        /* Free transmit stream items (packets) */
        if (ppTxPackets != NULL)
        {
            for (i = 0; i < (packetNum * BUFFER_NUM * testCount); i++)
            {
                if (ppTxPackets[i] != NULL)
                {
                    STAR_destroyStreamItem(ppTxPackets[i]);
                }
            }

            /* Free the packet pointer array*/
            free(ppTxPackets);
        }

        /* Free the transfer operation pointers */
        if (ppTxTransOp != NULL)
        {
            free(ppTxTransOp);
        }

        /* Free the transmit buffer */
        if (pTransmitBuffer != NULL)
        {
            free(pTransmitBuffer);
        }
    }

    /* Free the array of packet lengths */
    if (pPacketLengths != NULL)
    {
        free(pPacketLengths);
    }

    if (status != STAR_TRANSFER_STATUS_COMPLETE)
    {
        *pDataRate = 0.0;
        *pPacketRate = 0.0;
        *pPacketTime = 0.0;
    }
}


/**
 * \ingroup PerformanceTester
 * Perform the random test.
 *
 * @param pTestProperties a pointer to a structure containing the properties of
 *                        the test to be performed
 * @param pOutputFile the file to write the statistics to, or NULL if no file is
 *                    to be used
 */
static void performRandomTest(const TEST_PROPERTIES * const pTestProperties,
    FILE * const pOutputFile)
{
    int minPacketSize;
    double dataRate = 0, packetRate = 0, packetTime = 0, packetSize = 0;
    CPU_USAGE_PROPERTIES startCpuProperties, endCpuProperties;

    /* If a file is to be written to */
    if (pOutputFile != NULL)
    {
        /* Write the statistics column headers to the file */
        fprintf(pOutputFile,
            "Minimum Packet Size\tMaximum Packet Size\tData Rate (Mbit/s)\t");
        fprintf(pOutputFile,
            "Packet Rate (packets/s)\tAverage Packet Time (microseconds)\t");
        fprintf(pOutputFile,
            "Average Packet Size (bytes)");
        PRINT_CPU_USAGE_HEADERS(pOutputFile);
    }

    /* for each packet size to test */
    minPacketSize = pTestProperties->packetStart;
    while (minPacketSize <= pTestProperties->packetEnd)
    {
        /* Display the packet size on screen */
        printf("Performing random packet size data rate test using packets of size %d to %d bytes...\n",
            minPacketSize, pTestProperties->packetEnd);

        /* Perform the test for the current packet size */
        performRandomTestForPacketSize(minPacketSize,
            pTestProperties->packetEnd, pTestProperties->mode,
            pTestProperties->testCount, pTestProperties->pTxChannelIds,
            pTestProperties->pAddressPaths, pTestProperties->pRxChannelIds,
            &dataRate, &packetRate, &packetTime, &packetSize,
            &startCpuProperties, &endCpuProperties);

        /* Display the results of the test */
        printf("\t%-3.2f Mbit/s  %-3.2f packets/s  %-3.2f microseconds/packet  %-3.2f average packet size\n",
            dataRate, packetRate, packetTime, packetSize);
        PRINT_CPU_USAGE(NULL, &startCpuProperties, &endCpuProperties);

        /* If writing the results to file */
        if (pOutputFile != NULL)
        {
            /* Write the results to the file */
            fprintf(pOutputFile, "%8d\t%8d\t%-3.2f\t%-3.2f\t%-3.2f\t%-3.2f",
                minPacketSize, pTestProperties->packetEnd, dataRate, packetRate, packetTime,
                packetSize);
            PRINT_CPU_USAGE(pOutputFile, &startCpuProperties,
                &endCpuProperties);
            fflush(pOutputFile);
        }

        /* if not performing receives and this isn't the last loop */
        if ((!PERFORMING_RECEIVES(pTestProperties->mode)) &&
            (minPacketSize < pTestProperties->packetEnd))
        {
            /* Sleep for 5 seconds to allow all packets to be sent */
            SLEEP(5000U);
        }

        /* Move to the next packet size */
        if (minPacketSize == pTestProperties->packetEnd)
        {
            minPacketSize++;
        }
        else if ((minPacketSize == pTestProperties->packetStart) &&
            (pTestProperties->packetStep != 0) && ((minPacketSize +
                pTestProperties->packetStep) > pTestProperties->packetEnd))
        {
            minPacketSize = pTestProperties->packetEnd + 1;
        }
        else if (pTestProperties->packetStep == 0)
        {
            minPacketSize = MIN(2 * minPacketSize, pTestProperties->packetEnd);
        }
        else
        {
            minPacketSize = MIN(minPacketSize + pTestProperties->packetStep,
                pTestProperties->packetEnd);
        }
    }
}


/**
 * \ingroup PerformanceTester
 * Perform the latency test for a specific packet size.
 *
 * @param packetSize the size of packet to transmit/receive
 * @param mode the mode of operation (receive and/or transmit)
 * @param testCount the number of tests to perform concurrently
 * @param pTxChannels the channels to transmit on
 * @param pAddressPaths the addresses to add to the start of all packets sent,
 *                      if packets are to be sent
 * @param pRxChannels the channels to receive on
 * @param pDataRate a pointer to a variable which will be updated to contain the
 *                  data rate recorded for the test
 * @param pPacketRate a pointer to a variable which will be updated to contain
 *                    the packet rate recorded for the test
 * @param pPacketTime a pointer to a variable which will be updated to contain
 *                    the average time to transmit a single packet
 * @param pStartCpuProperties a pointer to a structure which will be updated to
 *                            contain the CPU properties when the test is
 *                            started
 * @param pEndCpuProperties a pointer to a structure which will be updated to
 *                          contain the CPU properties when the test completes
 */
static void performLatencyTestForPacketSize(const int packetSize, const U8 mode,
    const int testCount, const STAR_CHANNEL_ID * const pTxChannels,
    STAR_SPACEWIRE_ADDRESS ** const pAddressPaths,
    const STAR_CHANNEL_ID * const pRxChannels, double * const pDataRate,
    double * const pPacketRate, double * const pPacketTime,
    CPU_USAGE_PROPERTIES * const pStartCpuProperties,
    CPU_USAGE_PROPERTIES * const pEndCpuProperties)
{
    int i, j, loopNum;
    STAR_TRANSFER_OPERATION **ppRxTransOp = NULL, **ppTxTransOp = NULL;
    clock_t start = 0, finish;
    double duration, bitsSent;
    STAR_TRANSFER_STATUS status = STAR_TRANSFER_STATUS_ERROR;
    unsigned char *pTransmitBuffer = NULL;
    STAR_STREAM_ITEM **ppTxPackets = NULL;

    /* Calculate the number of loops to perform */
    loopNum = LOOP_NUM;

    /* if performing receives */
    if (PERFORMING_RECEIVES(mode))
    {
        /* Allocate memory for the receive transfer operations */
        ppRxTransOp = (STAR_TRANSFER_OPERATION **)calloc(testCount,
            sizeof(STAR_TRANSFER_OPERATION *));
        if (ppRxTransOp == NULL)
        {
            puts("Couldn't allocate memory for receive operations, exiting.");
            goto Latency_End_Cleanup;
        }

        for (i = 0; i < testCount; i++)
        {
            /* Create a transfer operation for receiving */
            ppRxTransOp[i] = STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);
            if (ppRxTransOp[i] == NULL)
            {
                puts("Couldn't create receive operation, exiting.");
                goto Latency_End_Cleanup;
            }
        }
    }

    /* if performing transmits */
    if (PERFORMING_TRANSMITS(mode))
    {
        /* Allocate transmit buffer */
        pTransmitBuffer = (unsigned char *)calloc(1U, packetSize);
        if (pTransmitBuffer == NULL)
        {
            puts("Couldn't allocate memory for transmit buffer, exiting.");
            goto Latency_End_Cleanup;
        }

        /* Set the first byte of the transmit buffer to 0xfe.  This is the */
        /* byte that will be at the front of the packet at the destination */
        /* (if path addressing is used) */
        if (packetSize > 0)
        {
            pTransmitBuffer[0U] = 0xfeU;
        }

        /* Allocate memory for packet pointers */
        ppTxPackets = (STAR_STREAM_ITEM **)calloc(testCount,
            sizeof(STAR_STREAM_ITEM *));
        if (ppTxPackets == NULL)
        {
            puts("Couldn't allocate memory for transmit packets, exiting.");
            goto Latency_End_Cleanup;
        }

        /* Allocate memory for the transmit identifiers */
        ppTxTransOp = (STAR_TRANSFER_OPERATION **)calloc(testCount,
            sizeof(STAR_TRANSFER_OPERATION *));
        if (ppTxTransOp == NULL)
        {
            puts("Couldn't allocate memory for transmit operations, exiting.");
            goto Latency_End_Cleanup;
        }

        for (i = 0; i < testCount; i++)
        {
            /* Create a packet to be transferred */
            ppTxPackets[i] = STAR_createPacket(pAddressPaths[i],
                pTransmitBuffer, packetSize, STAR_EOP_TYPE_EOP);
            if (ppTxPackets[i] == NULL)
            {
                puts("Couldn't create the packet to be transferred, exiting.");
                goto Latency_End_Cleanup;
            }

            /* Create a transfer operation to transmit the packet */
            ppTxTransOp[i] = STAR_createTxOperation(&ppTxPackets[i], 1);
            if (ppTxTransOp[i] == NULL)
            {
                puts("Couldn't create transmit operation, exiting.");
                goto Latency_End_Cleanup;
            }
        }

        /* Start the clock */
        start = GET_TIME();
        STORE_CPU_USAGE(pStartCpuProperties);
    }

    /* for each loop to perform */
    for (i = 0; i < loopNum; i++)
    {
        /* if performing transmits */
        if (PERFORMING_TRANSMITS(mode))
        {
            for (j = 0; j < testCount; j++)
            {
                /* Start transmitting the packet */
                STAR_submitTransferOperation(pTxChannels[j], ppTxTransOp[j]);

                status = STAR_getTransferOperationStatus(ppTxTransOp[j]);
                if ((status != STAR_TRANSFER_STATUS_STARTED) &&
                    (status != STAR_TRANSFER_STATUS_COMPLETE))
                {
                    printf("Could not perform transmit in loop %d, error of %d\n",
                            i, status);
                    goto Latency_End_Cleanup;
                }
            }
        }

        /* If performing receives */
        if (PERFORMING_RECEIVES(mode))
        {
            /* Start receiving the next packet */
            for (j = 0; j < testCount; j++)
            {
                STAR_submitTransferOperation(pRxChannels[j], ppRxTransOp[j]);
            }


            /* Wait on packets being received */
            for (j = 0; j < testCount; j++)
            {
                status = STAR_waitOnTransferOperationCompletion(ppRxTransOp[j],
                    STAR_INFINITE);
                if (status != STAR_TRANSFER_STATUS_COMPLETE)
                {
                    printf("Could not receive packet in loop %d, error of %d\n",
                        i, status);
                    goto Latency_End_Cleanup;
                }
            }

            /* If this is the first receive and not performing transmits */
            if ((i == 0) && (!PERFORMING_TRANSMITS(mode)))
            {
                /* Start the clock */
                start = GET_TIME();
                STORE_CPU_USAGE(pStartCpuProperties);
            }
        }

        /* If performing transmits but not performing receives */
        if (PERFORMING_TRANSMITS(mode) && (!PERFORMING_RECEIVES(mode)))
        {
            /* Wait on the transmit completing */
            for (j = 0; j < testCount; j++)
            {
                status = STAR_waitOnTransferOperationCompletion(ppTxTransOp[j],
                    STAR_INFINITE);
                if (status != STAR_TRANSFER_STATUS_COMPLETE)
                {
                    printf("Could not complete transmit in loop %d, error of %d\n",
                        i, status);
                    goto Latency_End_Cleanup;
                }
            }
        }
    }

    /* Stop the clock */
    finish = GET_TIME();
    STORE_CPU_USAGE(pEndCpuProperties);

    duration = (double)(finish - start) / TIME_DIVIDER;
    bitsSent = (double)packetSize * (double)testCount;
    if (pAddressPaths != NULL)
    {
        for (i = 0; i < testCount; i++)
        {
            if (pAddressPaths[i] != NULL)
            {
                bitsSent += STAR_getSpaceWireAddressPathLength(pAddressPaths[i]);
            }
        }
    }
    bitsSent = bitsSent * (double)loopNum * (double)8;

    /* Calculate the data and packet rate */
    if (duration > 0.0)
    {
        *pDataRate = (bitsSent / duration) / 1000000.0;
        *pPacketRate = (double)(loopNum * testCount) / duration;
    }
    else
    {
        *pDataRate = 0.0;
        *pPacketRate = 0.0;
    }
    *pPacketTime = (duration * 1000000.0) / (double)(loopNum * testCount);

/* Clean-up */
Latency_End_Cleanup:

    /* If performing receives */
    if (PERFORMING_RECEIVES(mode))
    {
        /* Dispose of the receive operations */
        if (ppRxTransOp != NULL)
        {
            for (j = 0; j < testCount; j++)
            {
                STAR_disposeTransferOperation(ppRxTransOp[j]);
            }

            free(ppRxTransOp);
        }
    }

    /* If performing transmits */
    if (PERFORMING_TRANSMITS(mode))
    {
        /* Dispose of the transmit operations */
        if (ppTxTransOp != NULL)
        {
            for (j = 0; j < testCount; j++)
            {
                STAR_disposeTransferOperation(ppTxTransOp[j]);
            }
        }

        /* Destroy the transmit packet */
        if (ppTxPackets != NULL)
        {
            for (j = 0; j < testCount; j++)
            {
                STAR_destroyStreamItem(ppTxPackets[j]);
            }

            /* Free the packet pointer array*/
            free(ppTxPackets);
        }

        /* Free the transfer operation pointers */
        if (ppTxTransOp != NULL)
        {
            free(ppTxTransOp);
        }

        /* Free the transmit buffer */
        if (pTransmitBuffer != NULL)
        {
            free(pTransmitBuffer);
        }
    }

    /* In the event of an error */
    if (status != STAR_TRANSFER_STATUS_COMPLETE)
    {
        *pDataRate = 0.0;
        *pPacketRate = 0.0;
        *pPacketTime = 0.0;
    }
}


/**
 * \ingroup PerformanceTester
 * Perform the latency test.
 *
 * @param pTestProperties a pointer to a structure containing the properties of
 *                        the test to be performed
 * @param pOutputFile the file to write the statistics to, or NULL if no file is
 *                    to be used
 */
static void performLatencyTest(const TEST_PROPERTIES * const pTestProperties,
    FILE * const pOutputFile)
{
    int packetSize;
    double dataRate = 0, packetRate = 0, packetTime = 0;
    CPU_USAGE_PROPERTIES startCpuProperties, endCpuProperties;

    /* If a file is to be written to */
    if (pOutputFile != NULL)
    {
        /* Write the statistics column headers to the file */
        fprintf(pOutputFile, "Packet Size\tData Rate (Mbit/s)\t");
        fprintf(pOutputFile,
            "Packet Rate (packets/s)\tAverage Packet Time (microseconds)");
        PRINT_CPU_USAGE_HEADERS(pOutputFile);
    }

    /* for each packet size to test */
    packetSize = pTestProperties->packetStart;
    while (packetSize <= pTestProperties->packetEnd)
    {
        /* Display the packet size on screen */
        printf("Performing latency test using packets of %d bytes...\n",
            packetSize);

        /* Perform the test for the current packet size */
        performLatencyTestForPacketSize(packetSize, pTestProperties->mode,
            pTestProperties->testCount, pTestProperties->pTxChannelIds,
            pTestProperties->pAddressPaths, pTestProperties->pRxChannelIds,
            &dataRate, &packetRate, &packetTime, &startCpuProperties,
            &endCpuProperties);

        /* Display the results of the test */
        printf(
            "\t%-3.2f Mbit/s  %-3.2f packets/s  %-3.2f microseconds/packet\n",
            dataRate, packetRate, packetTime);
        PRINT_CPU_USAGE(NULL, &startCpuProperties, &endCpuProperties);

        /* If writing the results to file */
        if (pOutputFile != NULL)
        {
            /* Write the results to the file */
            fprintf(pOutputFile, "%8d\t%-3.2f\t%-3.2f\t%-3.2f", packetSize,
                dataRate, packetRate, packetTime);
            PRINT_CPU_USAGE(pOutputFile, &startCpuProperties,
                &endCpuProperties);
            fflush(pOutputFile);
        }

        /* if not performing receives and this isn't the last loop */
        if ((!PERFORMING_RECEIVES(pTestProperties->mode)) &&
            (packetSize < pTestProperties->packetEnd))
        {
            /* Sleep for 5 seconds to allow all packets to be sent */
            SLEEP(5000U);
        }

        /* Move to the next packet size */
        if (packetSize == pTestProperties->packetEnd)
        {
            packetSize++;
        }
        else if (pTestProperties->packetStep == 0)
        {
            packetSize = MIN(2 * packetSize, pTestProperties->packetEnd);
        }
        else
        {
            packetSize = MIN(packetSize + pTestProperties->packetStep,
                pTestProperties->packetEnd);
        }
    }
}



/**
 * \ingroup PerformanceTester
 * Read in any arguments from the command line, which are not test parameters.
 *
 * @param pTestProperties a pointer to a structure which will be updated to
 *                        contain the properties of the test to be performed
 * @param argCount the number of parameters passed to the program
 * @param arguments the array of arguments passed to the program
 *
 * @return whether the parameters were successfully read
 */
static int readArguments(TEST_PROPERTIES * const pTestProperties,
    const int argCount, const char * const arguments[])
{
    int processedArguments = 1;

    /* While not reached the end of the arguments */
    while (processedArguments < argCount)
    {
        /* If the argument begins with a hyphen */
        if ((strlen(arguments[processedArguments]) > 1U) &&
            (arguments[processedArguments][0U] == '-'))
        {
            /* If the argument is to use channel 0, enable its use */
            if (strcasecmp(arguments[processedArguments], "-usechannel0") == 0)
            {
                pTestProperties->useChannel0 = 1;
            }
            /* Otherwise this argument isn't supported, so display a warning */
            else
            {
                printf("Unsupported argument \"%s\" ignored\n",
                    arguments[processedArguments]);
            }
            processedArguments++;
        }
        else
        {
            break;
        }
    }

    return processedArguments;
}

/**
 * \ingroup PerformanceTester
 * Main function, called when the program is started.
 *
 * @param argc the number of parameters passed to the program
 * @param argv the array of arguments passed to the program
 *
 * @return always returns 0 to indicate success
 */
int main(int argc, char* argv[])
{
    TEST_PROPERTIES testProperties = {{ 0 }};
    FILE *pOutputFile = NULL;
    int processedArguments;

    STAR_setApplicationName("STAR-System Performance Tester Application");

    /* Read in any arguments */
    processedArguments = readArguments(&testProperties, argc,
        (const char * const *)argv);

    /* Read the test parameters */
    if (readTestParameters(&testProperties, argc - processedArguments,
        (const char * const *)(argv + processedArguments)) == 0)
    {
        cleanupTest(&testProperties);
        return 0;
    }

    /* if writing the results to file */
    if (strlen(testProperties.pFilePath) > 0U)
    {
        /* Open the file */
        pOutputFile = fopen(testProperties.pFilePath, "wt");
        if (pOutputFile == NULL)
        {
            puts("Couldn't open the file to store the statistics, exiting");
            cleanupTest(&testProperties);
            return 0;
        }

        /* Write the description to the file */
        fprintf(pOutputFile, "%s\n\n", testProperties.pFileDescription);
    }

    /* Test is beginning, so prevent system sleeping */
    DISABLE_SYSTEM_SLEEP();

    switch (testProperties.testType)
    {
        case RATE_TEST:
            performRateTest(&testProperties, pOutputFile);
            break;

        case RANDOM_TEST:
            performRandomTest(&testProperties, pOutputFile);
            break;

        case LATENCY_TEST:
            performLatencyTest(&testProperties, pOutputFile);
            break;

        default:
            puts("Unexpected test type encountered, exiting");
            break;
    }

    ENABLE_SYSTEM_SLEEP();


    /* if writing the results to file */
    if (pOutputFile != NULL)
    {
        /* Close the file */
        fclose(pOutputFile);
    }

    /* Clean-up the test by freeing any memory and closing any open channels */
    cleanupTest(&testProperties);

    return 0;
}


