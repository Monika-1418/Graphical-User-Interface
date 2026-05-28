/**
 * \file performance_tester.cpp
 *
 * \brief Implementation for C++ version of the STAR-System Performance Tester
 *        program.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides a C++ version of the STAR-System Performance Tester: a program to
 * test the performance of SpaceWire devices using the STAR-System software
 * stack. It can be used to test the performance of devices, networks and the
 * system itself.  The source code also provides useful examples of potential
 * methods to get the best performance from the API.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "performance_tester.h"
#include "cpp_api_macros.hpp"
using namespace stardundee::com::starsystem::general;


/* ############################################ */
/* ### CPUUsage class(es) implementations   ### */
/* ############################################ */


#ifdef _WIN32

/**
 * Start recording the CPU usage, by storing the current usage.
 *
 * @param pCpuProperties A pointer to a CpuProperties object which will be
 *                        updated to contain the current CPU properties.
 */
void CPUUsageData::STORE_CPU_USAGE(CPUProperties *pCpuProperties)
{
    FILETIME creationTime, exitTime;

    GetSystemTimes(&pCpuProperties->idleTime, &pCpuProperties->kernelTime,
        &pCpuProperties->userTime);
    GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime,
        &pCpuProperties->processKernelTime,
        &pCpuProperties->processUserTime);
}

/**
 * Print the headers for the CPU usage for a test written to file.
 *
 * @param stream the stream to write the CPU usage figures to
 */
void CPUUsageData::PRINT_CPU_USAGE_HEADERS(FILE *stream)
{
    fprintf(stream, "\tProcess CPU usage (%%)\tTotal CPU usage (%%)\n");
}


/**
 * Print the CPU usage for a test.
 *
 * @param stream the stream to write the CPU usage figures to.  This should
 *               be NULL for stdout
 * @param pStartCpuProperties a pointer to the original CPU usage properties
 *                            when the test started
 * @param pEndCpuProperties a pointer to the final CPU usage properties when
 *                          the test completed
 */
void CPUUsageData::PRINT_CPU_USAGE(FILE *stream,
    CPUProperties *pStartCpuProperties,
    CPUProperties *pEndCpuProperties)
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
    if (totalTime.QuadPart)
    {
        totalUsage =
            (double)((totalTime.QuadPart - idleTime.QuadPart) * 100) /
                (double)(totalTime.QuadPart);
    }
    else
    {
        totalUsage = 0;
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

    /* calculate the process's usage */
    if (totalTime.QuadPart)
    {
        processUsage =
            (double)((userTime.QuadPart + kernelTime.QuadPart) * 100) /
            (double)(totalTime.QuadPart);
    }
    else
    {
        processUsage = 0;
    }

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

#else

/**
 * Start recording the CPU usage, by storing the current usage.
 *
 * @param pCpuProperties A pointer to a CPUProperties object which will be
 *                       updated to contain the current CPU properties.
 */
void CPUUsageData::STORE_CPU_USAGE(CPUProperties *pCpuProperties)
{
    FILE *procFile;
    char line[256];

    pCpuProperties->currentTime = times(&pCpuProperties->processTimes);

    pCpuProperties->userTime = 0;
    pCpuProperties->systemTime = 0;
    pCpuProperties->niceTime = 0;
    pCpuProperties->idleTime = 0;
    pCpuProperties->iowaitTime = 0;
    procFile = fopen("/proc/stat", "r");
    if (procFile)
    {
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
 * Print the headers for the CPU usage for a test written to file.
 *
 * @param stream the stream to write the CPU usage figures to
 */
void CPUUsageData::PRINT_CPU_USAGE_HEADERS(FILE *stream)
{
    fprintf(stream, "\tProcess CPU usage (%%)\tTotal CPU usage (%%)\n");
}


/**
 * Get the number of CPU cores on this PC.
 *
 * @return the number of CPU cores
 */
#if defined(__QNX__)
    #include <sys/syspage.h>
    int CPUUsageData::GetCpuCount()
    {
        return _syspage_ptr->num_cpu;
    }
#else
    int CPUUsageData::GetCpuCount()
    {
        return (int)sysconf(_SC_NPROCESSORS_ONLN);
    }
#endif


/**
 * Print the CPU usage for a test.
 *
 * @param stream the stream to write the CPU usage figures to.  This should
 *               be NULL for stdout
 * @param pStartCpuProperties a pointer to the original CPU usage properties
 *                            when the test started
 * @param pEndCpuProperties a pointer to the final CPU usage properties when
 *                          the test completed
 */
void CPUUsageData::PRINT_CPU_USAGE(FILE *stream,
    CPUProperties *pStartCpuProperties,
    CPUProperties *pEndCpuProperties)
{
    unsigned long userTime, systemTime, niceTime, idleTime, iowaitTime;
    unsigned long totalTime;
    clock_t processKernelTime, processUserTime, processTotalTime;
    double totalUsage, processUsage;
    int cpuCount;

    /* determine the number of CPUs */
    cpuCount = CPUUsageData::GetCpuCount();

    /* subtract the original total CPU figures from the final figures */
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

    /* calculate the total time */
    totalTime = userTime + systemTime + niceTime + idleTime + iowaitTime;

    /* calculate the total usage */
    if (totalTime)
    {
        totalUsage =
            (double)((totalTime - idleTime) * 100) / (double)totalTime;
    }
    else
    {
        totalUsage = 0;
    }

    /* subtract the original process CPU figures from the final figures */
    processKernelTime = pEndCpuProperties->processTimes.tms_stime -
        pStartCpuProperties->processTimes.tms_stime;
    processUserTime = pEndCpuProperties->processTimes.tms_utime -
        pStartCpuProperties->processTimes.tms_utime;

    /* calculate the total process time */
    processTotalTime = pEndCpuProperties->currentTime -
        pStartCpuProperties->currentTime;

    /* calculate the process's usage */
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

    /* \note The kernel time may not be needed, if this is included in */
    /*       the user time (at least according to some manuals!). */
    //processUsage = (double)(processUserTime * 100) /
    //    (double)(processTotalTime * cpuCount);

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

/* ############################################ */
/* ### TestProperties Class Implementations ### */
/* ############################################ */

/**
 * Default constructor. Initialise object to default state;
 */
TestProperties::TestProperties()
{
    mode = 0;
    testType = UNDEFINED_TEST_TYPE;
    testCount = 0;
    packetStart = 0;
    packetEnd = 0;
    packetStep = 0;
    pTxChannels = NULL;
    pRxChannels = NULL;
    ppAddresses = NULL;
    pFilePath[0] = '\0';
    pFileDescription[0] = '\0';
}

/**
 * Destructor. Free resources created or held by object.
 */
TestProperties::~TestProperties()
{
    CleanUp();
}

/**
 * Clean up / free resources created and/or held by this object.
 */
void TestProperties::CleanUp()
{
    /* free the list of transmit channels */
    FreeChannels(pTxChannels, testCount, true);
    pTxChannels = NULL;

    /* free the list of receive channels */
    FreeChannels(pRxChannels, testCount, true);
    pRxChannels = NULL;

    /* free the list of address paths */
    FreeAddressPaths(ppAddresses, testCount);
    ppAddresses = NULL;
}

/**
 * Free the given list of channels, closing them first if required.
 *
 * @param pChannels A list / array of channels to free.
 * @param channelNum The number of channels in the list.
 * @param closeFirst Whether to close the channel first, prior to freeing.
 */
void TestProperties::FreeChannels(Channel *pChannels, int channelNum,
                                  bool closeFirst)
{
    /* if a valid list of channels is provided */
    if (pChannels != NULL)
    {
        /* if the channels should be closed first */
        if (closeFirst)
        {
            /* close each channel in the list */
            for (int i=0; i<channelNum; i++)
                pChannels[i].CloseChannel();
        }

        /* free the list of channels */
        delete [] pChannels;
    }
}

/**
 * Free the given list of address paths.
 *
 * @param pAddresses An array of pointers to SpaceWireAddresses to be freed.
 * @param count The number of items in the list.
 */
void TestProperties::FreeAddressPaths(Address **pAddresses, int count)
{
    /* if list of address paths is valid (non-NULL) */
    if (pAddresses != NULL)
    {
        /* for each pointer to a SpaceWire address held in the list */
        for (int i=0; i<count; i++)
        {
            /* if the pointer points to something */
            if (pAddresses[i] != NULL)
            {
                /* free it */
                delete pAddresses[i];
            }
        }

        /* free the list itself */
        delete [] pAddresses;
    }
}

/**
 * Create a required number of transmit channels.
 *
 * @param numberOfChannelsRequired The number of channels required.
 *
 * @return 1 if channels created successfully, otherwise 0.
 */
int TestProperties::CreateTransmitChannels(int numberOfChannelsRequired)
{
    /* if there is already a list of transmit channels */
    if (pTxChannels != NULL)
    {
        /* free the list */
        FreeChannels(pTxChannels, testCount, true);
    }

    /* attempt to allocate memory for a new list of channels */
    try
    {
        pTxChannels = new Channel[numberOfChannelsRequired];
    }
    catch (std::bad_alloc& ba)
    {
        /* memory allocation error */
        UNREFERENCED_PARAMETER(ba);

        /* set channel list to NULL */
        pTxChannels = NULL;

        /* return error / 0 */
        return 0;
    }

    /* return success */
    return 1;
}

/**
 * Create a required number of receive channels.
 *
 * @param numberOfChannelsRequired The number of channels required.
 *
 * @return 1 if channels created successfully, otherwise 0.
 */
int TestProperties::CreateReceiveChannels(int numberOfChannelsRequired)
{
    /* if there is already a list of receive channels */
    if (pRxChannels != NULL)
    {
        /* free the list */
        FreeChannels(pRxChannels, testCount, true);
    }

    /* attempt to allocate memory for a new list of channels */
    try
    {
        pRxChannels = new Channel[numberOfChannelsRequired];
    }
    catch (std::bad_alloc& ba)
    {
        /* memory allocation error */
        UNREFERENCED_PARAMETER(ba);

        /* set channel list to NULL */
        pRxChannels = NULL;

        /* return error / 0 */
        return 0;
    }

    /* return success */
    return 1;
}

/**
 * Create a required number of pointers to SpaceWireAddresses
 *
 * @param numberOfAddressPathsRequired The number of adddress paths required.
 *
 * @return 1 if channels created successfully, otherwise 0.
 */
int TestProperties::CreateAddressPaths(int numberOfAddressPathsRequired)
{
    /* if there is already a list of spacewire addresses */
    if (ppAddresses != NULL)
    {
        /* free the list */
        FreeAddressPaths(ppAddresses, testCount);
    }

    /* attempt to allocate memory for a new list of pointers to addresses */
    try
    {
        ppAddresses = new Address*[numberOfAddressPathsRequired];
    }
    catch (std::bad_alloc& ba)
    {
        /* memory allocation error */
        UNREFERENCED_PARAMETER(ba);

        /* set address list to NULL */
        ppAddresses = NULL;

        /* return error / 0 */
        return 0;
    }

    /* for each pointer to a Address in the list */
    for (int i=0; i<numberOfAddressPathsRequired; i++)
    {
        /* set it to NULL initially */
        ppAddresses[i] = NULL;
    }

    /* return success */
    return 1;
}

/**
 * Add the given channel to the given position in the list of transmit channels.
 *
 * @param ch The channel to add.
 * @param index The position to add it to in the list.
 *
 * @return 1 if the channel added successfully, otherwise 0.
 */
int TestProperties::AddTransmitChannel(Channel& ch, int index)
{
    /* if no list of transmit channels created yet */
    if (pTxChannels == NULL)
    {
        /* return error */
        return 0;
    }

    /* if invalid list position specified */
    if ((index<0) || (index>=testCount))
    {
        /* return error */
        return 0;
    }

    /* otherwise, add channel to given position in transmit list */
    pTxChannels[index] = ch;

    /* return success */
    return 1;
}

/**
 * Add the given channel to the given position in the list of receive channels.
 *
 * @param ch The channel to add.
 * @param index The position to add it to in the list.
 *
 * @return 1 if the channel added successfully, otherwise 0.
 */
int TestProperties::AddReceiveChannel(Channel& ch, int index)
{
    /* if no list of receive channels created yet */
    if (pRxChannels == NULL)
    {
        /* return error */
        return 0;
    }

    /* if invalid list position specified */
    if ((index<0) || (index>=testCount))
    {
        /* return error */
        return 0;
    }

    /* otherwise, add channel to given position in receive list */
    pRxChannels[index] = ch;

    /* return success */
    return 1;
}

/**
 * Add the given pointer to a SpaceWire Address to the given position in the
 * list of pointers to SpaceWire Addresses.
 *
 * @param pAddress A pointer to the SpaceWire Address to add.
 * @param index The position to add it to in the list.
 *
 * @return 1 if the address added successfully, otherwise 0.
 */
int TestProperties::AddAddressPath(Address *pAddress, int index)
{
    /* if valid address provided */
    if (pAddress != NULL)
    {
        /* if list of Address paths exists */
        if (ppAddresses != NULL)
        {
            /* if valid list position specified */
            if ((index>=0) && (index<testCount))
            {
                /* add pointer to address to given position in list */
                ppAddresses[index] = pAddress;

                /* return success */
                return 1;
            }
        }
    }

    /* return error */
    return 0;
}


/* ######################################################## */
/* ### StarSystemPerformanceTester class implementation ### */
/* ######################################################## */

/**
 * Default constructor. Initialise object to default state.
 */
StarSystemPerformanceTester::StarSystemPerformanceTester()
{
}

/**
 * Destructor. Free any resources created by the object.
 */
StarSystemPerformanceTester::~StarSystemPerformanceTester()
{
}

/**
 * Prompts the user to choose which device to use for the test if more than
 * one device is available.
 *
 * @param argCount The number of arguments passed to the function.
 * @param arguments A list of arguments provided to the function, as an
 *                  array of char strings.
 * @param pCurrentArgument An index into the list of arguments.
 *
 * @return A pointer to a Device object representing the selected
 *         device, or NULL if no devices are available or an error occurred
 *         during processing.
 *
 * \note The object returned must be freed when no longer required by using
 *       the C++ delete operator.
 */
Device* StarSystemPerformanceTester::ChooseDevice(int argCount,
                                                  char *arguments[],
                                                  int *pCurrentArgument)
{
    char deviceName[Device::STR_DEVICE_NAME_LEN];
    size_t stringLen;
    U32 deviceCount = 0;
    DeviceList deviceList;
    Device *selectedDevice = NULL;
    int status;
    unsigned int chosen, i;
    char s[256];
    bool deviceSelected = true;

    /* get the list of devices present for all drivers */
    deviceCount = deviceList.Populate();

    /* if there are no devices present */
    if (!deviceCount)
    {
        /* report no devices available */
        puts("No SpaceWire devices detected!");

        /* return null / empty device */
        return NULL;
    }

    /* display the number of devices detected */
    if (deviceCount == 1)
    {
        printf("One SpaceWire device detected: ");
    }
    else
    {
        printf("%d SpaceWire devices detected:\n", deviceCount);
    }

    /* for each device */
    for (i = 0; i < deviceCount; i++)
    {
        /* if device's ID is valid */
        if (deviceList[i]->GetDeviceID())
        {
            /* get the device's name */
            stringLen = deviceList[i]->GetDeviceName(deviceName);

            /* if device name available */
            if (stringLen)
            {
                /* display it */
                printf("\t%u - %s\n", i, deviceName);
            }
            else
            {
                /* display error msg */
                printf("\t%u - Unknown SpaceWire Device\n", i);
            }
        }
        else
        {
            /* display - invalid or inaccessible device */
            printf("\t%u - Unable to access device\n", i);
        }
    }

    /* if there's only 1 device on the list */
    if (deviceCount == 1)
    {
        /* make this the selected device to return */

        /* make a clone/copy of the device on the list which is being
         * selected (the device list itself has to be freed later)
         */
        selectedDevice = deviceList.CopyDeviceAt(0);
    }
    else
    {
        /* ask the user which device to use */
        printf("Please select which device to open: ");

        /* if device can be selected from arguments provided to function */
        if ((*pCurrentArgument) < argCount)
        {
            /* confirm device selected */
            puts(arguments[*pCurrentArgument]);

            /* copy device to string and null terminate it */
            strncpy(s, arguments[*pCurrentArgument], 255);
            s[255] = '\0';

            /* increment index into argument list */
            (*pCurrentArgument)++;
        }
        else
        {
            /* attempt to read device in from stdin */

            /* if failed to read in device number */
            if (!fgets(s, 256, stdin))
            {
                /* display error message */
                puts("No device number selected.");

                /* indicate that device has failed to be selected */
                deviceSelected = false;
            }
        }

        /* if device selected  */
        if (deviceSelected)
        {
            /* attempt to parse device ID selected */
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
                 * device list itself has to be freed later)
                 */
                selectedDevice = deviceList.CopyDeviceAt(chosen);
            }
        }
    }

    /* dispose of the device list */
    deviceList.Empty();

    /* return the selected device */
    return selectedDevice;
}

/**
 * Opens a channel to be used to transmit or receive packets
 *
 * @param channelTypeStr a description of the type of channel to be opened, e.g.
 *                       "transmit" or "receive"
 * @param testNumber the number of the test to be performed.  The first test is
 *                   numbered 1
 * @param direction the direction in which the channel should be opened
 *
 * @return A Channel object representing the opened channel ready for use.
 *
 * \note In the event of an error or the channel failing to open the Channel
 *       object returned will not represent a valid, opened channel. To
 *       determine if the channel is valid or not, one may call the 'IsOpen()'
 *       method on the Channel object which is returned.
 */
Channel StarSystemPerformanceTester::OpenChannel(const char *channelTypeStr,
                                 int testNumber,
                                 Channel::ChannelDirection direction,
                                 int argCount, char *arguments[],
                                 int *pCurrentArgument)
{
    STAR_CHANNEL_MASK channelMask;
    int maximumChannelNumber = 0, i, channelNumber, status;
    char s[256];
    Device *pDevice;

    /* Choose the device to be used */
    pDevice = ChooseDevice(argCount, arguments, pCurrentArgument);

    /* if failed to obtain a valid device */
    if (pDevice == NULL)
    {
        /* report error */
        printf("\nERROR: Failed to open %s device for test %d.\n",
            channelTypeStr, testNumber);

        /* return an unopened channel */
        return Channel();
    }

    /* get available channels for device */
    channelMask = pDevice->GetDeviceChannels();

    /* if no channels available */
    if (channelMask == 0)
    {
        /* report error */
        printf("\nERROR: The chosen %s device doesn't appear to have any"
               " valid channels.\n", channelTypeStr);

        /* free device obtained, no longer required */
        delete pDevice;

        /* return an unopened channel */
        return Channel();
    }

    /* for each channel */
    for (i = 31; i >= 0; i--)
    {
        /* if the channel exists */
        if ((channelMask >> i) & 1)
        {
            /* this is the maximum channel number */
            maximumChannelNumber = i;
            break;
        }
    }

    /* if there is only one channel available */
    if (maximumChannelNumber == 1)
    {
        /* use that for the test */
        printf("\nUsing %s channel 1 for test %d\n", channelTypeStr,
            testNumber);
        channelNumber = 1;
    }
    else
    {
        /* choose a channel from those available */
        printf("\nEnter %s channel (1..%d) for test %d: ", channelTypeStr,
            maximumChannelNumber, testNumber);

        /* if a channel is available within the list of arguments provided
         * to this function.
         */
        if ((*pCurrentArgument) < argCount)
        {
            puts(arguments[*pCurrentArgument]);
            strncpy(s, arguments[*pCurrentArgument], 255);
            s[255] = '\0';
            (*pCurrentArgument)++;
        }
        else
        {
            /* read a channel number from the stdin */
            if (!fgets(s, 256, stdin))
            {
                /* report an error if failed to obtain a channel number */
                puts("\nERROR: No channel number specified");

                /* free device obtained, no longer required */
                delete pDevice;

                /* return an unopened channel */
                return Channel();
            }
        }

        /* parse string containing channel, extract channel into a number */
        status = sscanf(s, "%d", &channelNumber);

        /* if failed to extract channel number from string */
        if ((!status) || (channelNumber < 1) ||
            (channelNumber > maximumChannelNumber))
        {
            /* report error */
            puts("\nERROR: Incorrect channel number specified");

            /* free device obtained, no longer required */
            delete pDevice;

            /* return an unopened channel */
            return Channel();
        }

        /* channel isn't valid / available on the device */
        if (!((1 << channelNumber) & (int)channelMask))
        {
            /* report error */
            puts("\nERROR: The channel specified is not present");

            /* free device obtained, no longer required */
            delete pDevice;

            /* return an unopened channel */
            return Channel();
        }
    }

    /* open the channel on the device */
    Channel ch = pDevice->OpenChannel(direction, (unsigned char)channelNumber,
                                      TRUE);

    /* if channel failed to open */
    if (!ch.IsOpen())
    {
        /* report error */
        printf("\nFailed to open %s channel %d for test %d\n", channelTypeStr,
            channelNumber, testNumber);
    }


    /* free device obtained, no longer required */
    delete pDevice;

    /* return the opened/unopened channel */
    return ch;
}

/**
 * Free all memory associated with the given test properties, including
 * closing any channels it contains.
 *
 * @param pTestProperties the test properties to be cleaned up
 */
void StarSystemPerformanceTester::CleanupTest(TestProperties *pTestProperties)
{
    /* ask the TestProperties object to clean up */
    pTestProperties->CleanUp();
}

/**
 * This function reads a SpaceWire address as a hex string from stdin. After
 * tokenizing and validating the address the function returns a
 * Address, or NULL upon failure.
 *
 * @return A pointer to a new Address, or NULL upon failure.
 */
Address* StarSystemPerformanceTester::ReadSpaceWireAddress(int argCount,
                                                        char *arguments[],
                                                        int *pCurrentArgument)
{
    char s[256];
    Address *pAddress = NULL;
    unsigned char newPath[256];
    U16 pathLen = 0;
    char *pos = (char *)s;
    unsigned long value;

    /* if arguments provided contain address path */
    if ((*pCurrentArgument) < argCount)
    {
        /* get the address path from the argument(s) */
        puts(arguments[*pCurrentArgument]);
        strncpy(s, arguments[*pCurrentArgument], 255);
        s[255] = '\0';
        (*pCurrentArgument)++;
    }
    else
    {
        /* attempt to read the address path from the stdin instead */
        if (!fgets(s, 256, stdin))
        {
            /* report error if no valid address obtained */
            puts("No address entered");

            /* end processing, return NULL address */
            return NULL;
        }
    }

    /* until the end of the address string is reached */
    while ((*pos) && (pathLen < 256) && (*pos != '\n'))
    {
        /* obtain next part of address from string, as a long */
        errno = 0;
        value = strtoul(pos, &pos, 16);

        /* if failed to read the address section as a long */
        if (errno)
        {
            /* report error */
            puts("Invalid value entered in address");

            /* return NULL address */
            return NULL;
        }

        /* add next part of address to new address string */
        newPath[pathLen] = (unsigned char)value;

        /* increment the path length */
        pathLen++;
    }

    /* attempt to create a new SpaceWire address from the path */
    try
    {
        if (pathLen > 0)
        {
            pAddress = new Address(newPath, pathLen);
        }
    }
    catch (std::bad_alloc& ba)
    {
        /* failed to allocate memory */
        UNREFERENCED_PARAMETER(ba);

        /* report error */
        puts("Failed to allocate memory for SpaceWire Address");

        /* set address to NULL */
        pAddress = NULL;
    }
    catch (AddressException& swae)
    {
        /* failed to create SpaceWire address */
        puts("Error creating SpaceWire Address");
        pAddress = NULL;
        UNREFERENCED_PARAMETER(swae);
    }

    /* return the address (may be NULL if error occurred) */
    return pAddress;
}

/**
 * Read in the parameters of the tests to be performed from the user.
 *
 * @param pTestProperties A pointer to a TestProperties object which will be
 *                        updated to contain the properties of the test to
 *                        be performed
 *
 * @return whether the parameters were successfully read
 */
int StarSystemPerformanceTester::ReadParameters(TestProperties *pTestProperties,
                                                int argCount, char *arguments[])
{
    char s[256], txRxStr[100];
    size_t len;
    int status, testNumber, currentArgument = 1, result;
    Channel ch;
    Address *pAddress;

    puts("STAR-System Performance Tester");
    puts("Copyright STAR-Dundee Ltd. (c) 2013-2018");
    puts("www.star-dundee.com\n");

    /* ask the user which test they wish to perform */
    puts("Which test type do you wish to perform?");
    puts("\t(m) Maximum date rate test");
    puts("\t(r) Random packet size date rate test");
    puts("\t(l) Latency test");

    /* read in the test type */
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(s, arguments[currentArgument], 255);
        s[255] = '\0';
        currentArgument++;
    }
    else
    {
        if (!fgets(s, 256, stdin))
        {
            puts("ERROR: No test type selected");
            return 0;
        }
    }
    if ((s[0] == 'm') || (s[0] == 'M'))
    {
        pTestProperties->testType = RATE_TEST;
    }
    else if ((s[0] == 'r') || (s[0] == 'R'))
    {
        pTestProperties->testType = RANDOM_TEST;
    }
    else if ((s[0] == 'l') || (s[0] == 'L'))
    {
        pTestProperties->testType = LATENCY_TEST;
    }
    else
    {
        puts("ERROR: Invalid test type selected");
        return 0;
    }

    /* ask the user to how many tests they wish to perform */
    printf("\nHow many tests do you wish to perform: ");
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(s, arguments[currentArgument], 255);
        s[255] = '\0';
        currentArgument++;
    }
    else
    {
        if (!fgets(s, 256, stdin))
        {
            puts("\nERROR: No test count specified");
            return 0;
        }
    }
    status = sscanf(s, "%d", &pTestProperties->testCount);
    if (!status)
    {
        puts("\nERROR: No valid test count specified");
        return 0;
    }

    /* ask the user if the test should transmit, receive or 
     * both transmit and receive packets
     */
    printf("Should these tests transmit packets only (\"t\"),"
           " receive packets only (\"r\"), or both (\"b\")? ");
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(s, arguments[currentArgument], 255);
        s[255] = '\0';
        currentArgument++;
    }
    else
    {
        if (!fgets(s, 256, stdin))
        {
            puts("\nERROR: No transmit/receive option specified");
            return 0;
        }
    }
    pTestProperties->mode = 0;
    if ((s[0] == 't') || (s[0] == 'T') || (s[0] == 's') || (s[0] == 'S') ||
        (s[0] == 'b') || (s[0] == 'B'))
    {
        strcpy(txRxStr, "transmit");
        pTestProperties->mode |= PERFORM_TRANSMIT;
    }
    if ((s[0] == 'r') || (s[0] == 'R') || (s[0] == 'b') || (s[0] == 'B'))
    {
        if (pTestProperties->mode)
        {
            strcat(txRxStr, "/receive");
        }
        else
        {
            strcpy(txRxStr, "receive");
        }
        pTestProperties->mode |= PERFORM_RECEIVE;
    }
    if (pTestProperties->mode == 0)
    {
        puts("\nERROR: No valid transmit/receive option specified");
        return 0;
    }

    /* ask the user to enter a starting packet size */
    printf("\nEnter the minimum size of packet to %s: ", txRxStr);
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(s, arguments[currentArgument], 255);
        s[255] = '\0';
        currentArgument++;
    }
    else
    {
        if (!fgets(s, 256, stdin))
        {
            puts("\nERROR: No minimum packet size specified");
            return 0;
        }
    }
    status = sscanf(s, "%d", &pTestProperties->packetStart);
    if (!status)
    {
        puts("\nERROR: No valid minimum packet size specified");
        return 0;
    }

    /* ask the user to enter an ending packet size */
    printf("\nEnter the maximum size of packet to %s: ", txRxStr);
    if (currentArgument < argCount)
    {
        puts(arguments[currentArgument]);
        strncpy(s, arguments[currentArgument], 255);
        s[255] = '\0';
        currentArgument++;
    }
    else
    {
        if (!fgets(s, 256, stdin))
        {
            puts("\nERROR: No maximum packet size specified");
            return 0;
        }
    }
    status = sscanf(s, "%d", &pTestProperties->packetEnd);
    if (!status)
    {
        puts("\nERROR: No valid maximum packet size specified");
        return 0;
    }

    /* ask the user to enter the number of bytes to increment the packet size 
     * by in each loop
     */
    if (pTestProperties->packetStart == pTestProperties->packetEnd)
    {
        pTestProperties->packetStep = 1;
    }
    else
    {
        printf(
            "\nEnter the number of bytes to increment the packet size by"
            " in each loop (0 for log-like pattern): ");
        if (currentArgument < argCount)
        {
            puts(arguments[currentArgument]);
            strncpy(s, arguments[currentArgument], 255);
            s[255] = '\0';
            currentArgument++;
        }
        else
        {
            if (!fgets(s, 256, stdin))
            {
                puts("\nERROR: No packet size increment specified");
                return 0;
            }
        }
        status = sscanf(s, "%d", &pTestProperties->packetStep);
        if (!status)
        {
            puts("\nERROR: No valid packet size increment specified");
            return 0;
        }
    }

    /* if a transmit test is being performed */
    if (pTestProperties->mode & PERFORM_TRANSMIT)
    {
        /* allocate memory for transmit channels */
        result = pTestProperties->CreateTransmitChannels(
                                                   pTestProperties->testCount);

        /* if failed to create transmit channels */
        if (result == 0)
        {
            /* report error */
            puts(
                "\nERROR: Unable to allocate memory for the transmit channels");

            /* return error code */
            return 0;
        }

        /* allocate memory for address paths */
        result = pTestProperties->CreateAddressPaths(pTestProperties->testCount);

        /* if failed to create address paths */
        if (result == 0)
        {
            /* report error */
            puts(
                "\nERROR: Unable to allocate memory for the transmit channels");

            /* return error code */
            return 0;
        }
    }

    /* if a receive test is being performed */
    if (pTestProperties->mode & PERFORM_RECEIVE)
    {
        /* allocate memory for receive channels */
        result = pTestProperties->CreateReceiveChannels(
                                                   pTestProperties->testCount);

        /* if failed to create receive channels */
        if (result == 0)
        {
            /* report error and return error code */
            puts("\nERROR: Unable to allocate memory for the receive channels");
            return 0;
        }
    }

    /* for each test to perform */
    for (testNumber = 0; testNumber < pTestProperties->testCount; testNumber++)
    {
        /* if transmitting packets */
        if (pTestProperties->mode & PERFORM_TRANSMIT)
        {
            /* get (and open) the next transmit channel */
            ch = OpenChannel("transmit", testNumber + 1,
                             Channel::DIRECTION_OUT, argCount,
                             arguments, &currentArgument);

            /* if no valid channel obtained */
            if (!ch.IsOpen())
            {
                /* return error code */
                return 0;
            }
            else
            {
                /* add channel to list of transmit channels for the test */
                result = pTestProperties->AddTransmitChannel(ch, testNumber);

                /* if failed to add channel to list */
                if (result == 0)
                {
                    /* return error code */
                    return 0;
                }
            }

            /* ask the user to enter the path to add to the front of packets */
            printf("\nEnter the path to add to the front of packets"
                   " sent for test %d:\n", testNumber);
            puts("(Values should be in hex, separated by a space,"
                 " i.e.: 01 0f 02)");

            /* read in the address */
            pAddress = ReadSpaceWireAddress(argCount, arguments, 
                &currentArgument);

            /* if address obtained */
            if (pAddress)
            {
                /* add address to list of address paths for the test */
                result = pTestProperties->AddAddressPath(pAddress, testNumber);

                /* if failed to add address to list */
                if (result == 0)
                {
                    /* return error code */
                    return 0;
                }
            }
            else
            {
                /* assume address not required */
            }
        }

        /* if receiving packets */
        if (pTestProperties->mode & PERFORM_RECEIVE)
        {
            /* get (and open) the next receive channel */
            ch = OpenChannel("receive", testNumber + 1,
                              Channel::DIRECTION_IN, argCount,
                              arguments, &currentArgument);

            /* if no valid channel obtained */
            if (!ch.IsOpen())
            {
                /* return error code */
                return 0;
            }
            else
            {
                /* add channel to list of receive channels for the test */
                result = pTestProperties->AddReceiveChannel(ch, testNumber);

                /* if failed to add channel to list */
                if (result == 0)
                {
                    /* return error code */
                    return 0;
                }
            }
        }
    }

    /* ask the user to enter the path to the file to use, or to press enter to 
     * not store to a file
     */
    printf("\nEnter the path to the file to be used to store the results"
           " (hit enter if the results should not be stored): ");

    /* read in the file path */
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
        if (!fgets(pTestProperties->pFilePath, MAX_PATH, stdin))
        {
            strcpy(pTestProperties->pFilePath, "");
        }
    }

    len = strlen(pTestProperties->pFilePath);
    if (len)
    {
        while ((len > 0) &&
            ((pTestProperties->pFilePath[len - 1] == '\r') ||
            (pTestProperties->pFilePath[len - 1] == '\n')))
        {
            pTestProperties->pFilePath[len - 1] = '\0';
            len--;
        }
    }

    /* if writing the results to file */
    if (len > 0)
    {
        printf("\nEnter a description of the test: ");
        if (currentArgument < argCount)
        {
            puts(arguments[currentArgument]);
            strncpy(pTestProperties->pFileDescription,
                arguments[currentArgument], 999);
            pTestProperties->pFileDescription[999] = '\0';
            currentArgument++;
        }
        else
        {
            if (!fgets(pTestProperties->pFileDescription, 99, stdin))
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
void StarSystemPerformanceTester::PerformRateTestForPacketSize(int packetSize,
    int mode, int testCount,
    Channel *pTxChannels, Address **pAddressPaths,
    Channel *pRxChannels, double *pDataRate, double *pPacketRate,
    double *pPacketTime, CPUProperties *pStartCpuProperties,
    CPUProperties *pEndCpuProperties)
{
    int i, j, bufferNum, packetNum, loopNum;
    TransmitOperation **ppTransmitOps = NULL;
    ReceiveOperation **ppReceiveOps = NULL;
    ReceiveOperation *pReceiveOp = NULL;
    TransmitOperation *pTransmitOp = NULL;
    clock_t start = 0, finish;
    double duration, bitsSent;
    TransferOperation::TransferStatus status = TransferOperation::TRANSFER_STATUS_ERROR;
    unsigned char *pTransmitBuffer = NULL;
    StreamItem **ppTransmitPackets = NULL;
    unsigned int packetItem;
    unsigned int rxPackSize;
    unsigned int receivedItemCount;
    StreamItem **ppReceivedItems;
    Packet *pReceivedPacket;
    Packet *pPacket;

    /* calculate the number of packets to transmit in each loop and the number 
     * of loops to perform
     */
    if (pAddressPaths)
    {
        packetNum = BUFFER_NUM;
        for (i = 0; i < testCount; i++)
        {
            if (pAddressPaths[i])
            {
                packetNum = MAX(packetNum, BUFFER_SIZE /
                    (packetSize + pAddressPaths[i]->GetPathLength()));
            }
            else
            {
                packetNum = MAX(packetNum, BUFFER_SIZE / packetSize);
            }
        }
    }
    else
    {
        packetNum = MAX(BUFFER_NUM, BUFFER_SIZE / packetSize);
    }

    loopNum = LOOP_NUM;
    receivedItemCount = 0;

#ifdef DEBUG
    printf("\nPackets per loop:\t%d", packetNum);
    printf("\nNumber of loops:\t%d", loopNum);
    printf("\nBytes per packet:\t%d", packetSize);
    printf("\nExpected packets:\t%d", loopNum * packetNum);
    printf("\nExpected bytes:\t%f\n",
        (double)packetSize * (double)packetNum * (double)loopNum);
    printf("\n");
#endif

    /* if performing receives */
    if (mode & PERFORM_RECEIVE)
    {
        /* create an array of pointers to receive operations */
        try
        {
            ppReceiveOps = new ReceiveOperation*[BUFFER_NUM * testCount];
        }
        catch (std::bad_alloc& ba)
        {
            /* memory allocation error */
            ppReceiveOps = NULL;
            UNREFERENCED_PARAMETER(ba);
        }

        /* if failed to create array of pointers to receive ops */
        if (ppReceiveOps == NULL)
        {
            /* report error, jump to clean-up phase of processing */
            puts("Couldn't allocate memory for receive operations, exiting.");
            goto Rate_End_Cleanup;
        }
        else
        {
            /* initialise all pointers in array to NULL */
            for (i=0;i<(BUFFER_NUM * testCount); i++)
                ppReceiveOps[i] = NULL;
        }

        /* create transfer operations to receive all packets in each buffer */

        /* for the number of receive ops required */
        for (i = 0; i < (BUFFER_NUM * testCount); i++)
        {
            /* create a new receive operation */
            try
            {
                pReceiveOp = new ReceiveOperation(packetNum,
                                             ReceiveOperation::RECEIVE_PACKETS);
            }
            catch (std::bad_alloc& ba)
            {
                /* memory allocation error */
                pReceiveOp = NULL;
                UNREFERENCED_PARAMETER(ba);
            }
            catch (TransferOperationException& tOpEx)
            {
                /* failed to create receive op */
                pReceiveOp = NULL;
                UNREFERENCED_PARAMETER(tOpEx);
            }

            /* if failed to create receive op */
            if (pReceiveOp == NULL)
            {
                /* report error, jump to clean-up phase of processing */
                puts("Couldn't create receive operation, exiting.");
                goto Rate_End_Cleanup;
            }
            else
            {
                /* add receive op to next position in array of receive ops */
                ppReceiveOps[i] = pReceiveOp;
            }
        }
    }

    /* if performing transmits */
    if (mode & PERFORM_TRANSMIT)
    {
        /* allocate transmit buffer.
         * This buffer will be reused for all packets sent.
         */
        pTransmitBuffer = (unsigned char *)calloc(1, (size_t)packetSize);

        /* if failed to allocate transmit buffer */
        if (!pTransmitBuffer)
        {
            /* report error, jump to clean-up phase of processing */
            puts("Couldn't allocate memory for transmit buffer, exiting.");
            goto Rate_End_Cleanup;
        }

        /* create an array of pointers to stream items - this will
         * eventually hold the packets being sent
         */
        try
        {
            ppTransmitPackets = new StreamItem*[packetNum * BUFFER_NUM * 
                testCount];
        }
        catch (std::bad_alloc& ba)
        {
            /* memory allocation error */
            ppTransmitPackets = NULL;
            UNREFERENCED_PARAMETER(ba);
        }

        /* if failed to create array of pointers to stream items */
        if (ppTransmitPackets == NULL)
        {
            /* report error, jump to clean-up phase of processing */
            puts("Couldn't allocate memory for transmit packets, exiting.");
            goto Rate_End_Cleanup;
        }
        else
        {
            /* initialise all pointers in array to NULL */
            for (i=0;i<(packetNum * BUFFER_NUM * testCount); i++)
                ppTransmitPackets[i] = NULL;
        }

        /* create an array of pointers to transmit operations */
        try
        {
            ppTransmitOps = new TransmitOperation*[BUFFER_NUM * testCount];
        }
        catch (std::bad_alloc& ba)
        {
            /* memory allocation error */
            ppTransmitOps = NULL;
            UNREFERENCED_PARAMETER(ba);
        }

        /* if failed to create array of pointers to transmit ops */
        if (ppTransmitOps == NULL)
        {
            /* report error, jump to clean-up phase of processing */
            puts("Couldn't allocate memory for transmit operations, exiting.");
            goto Rate_End_Cleanup;
        }
        else
        {
            /* initialise all pointers in array to NULL */
            for (i=0;i<(BUFFER_NUM * testCount); i++)
                ppTransmitOps[i] = NULL;
        }

        /* construct packets */

        /* for the number of packets required */
        for (i = 0; i < packetNum * BUFFER_NUM; i++)
        {
            /* for the number of tests being performed */
            for (j = 0; j < testCount; j++)
            {
                /* create a new SpaceWire packet */
                try
                {
                    pPacket = new Packet(pAddressPaths[j],
                                                    pTransmitBuffer,
                                                    (unsigned int)packetSize,
                                                    Packet::EOP_TYPE_EOP);
                }
                catch (std::bad_alloc& ba)
                {
                    /* memory allocation failed */
                    pPacket = NULL;
                    UNREFERENCED_PARAMETER(ba);
                }
                catch (PacketException& pEx)
                {
                    /* failed to create Packet */
                    pPacket = NULL;
                    UNREFERENCED_PARAMETER(pEx);
                }

                /* if failed to create packet */
                if (pPacket == NULL)
                {
                    /* report error, jump to clean-up phase of processing */
                    puts("Couldn't create packet for transmit operation, "
                        "exiting.");
                    goto Rate_End_Cleanup;
                }
                else
                {
                    /* add packet to list of packets being sent */
                    ppTransmitPackets[i * testCount + j] = pPacket;
                }
            }
        }

        /* create transfer operations to transmit all packets in each buffer */

        /* for the number of transmit operations required */
        for (i = 0; i < BUFFER_NUM * testCount; i++)
        {
            /* create a new transmit operation */
            try
            {
                pTransmitOp = new TransmitOperation(
                                        ppTransmitPackets + (packetNum * i),
                                        (unsigned int)packetNum);
            }
            catch (std::bad_alloc& ba)
            {
                /* memory allocation error */
                pTransmitOp = NULL;
                UNREFERENCED_PARAMETER(ba);
            }
            catch (TransferOperationException& tOpEx)
            {
                /* failed to create  transmit op */
                pTransmitOp = NULL;
                UNREFERENCED_PARAMETER(tOpEx);
            }

            /* if failed to create transmit op */
            if (pTransmitOp == NULL)
            {
                /* report error, jump to clean-up phase of processing */
                puts("Couldn't create transmit operation, exiting.");
                goto Rate_End_Cleanup;
            }
            else
            {
                /* add transmit op to next position in array of transmit ops */
                ppTransmitOps[i] = pTransmitOp;
            }
        }

        /* start the clock */
        start = GET_TIME();
        CPUUsageData::STORE_CPU_USAGE(pStartCpuProperties);
    }

    bufferNum = 0;

    /* for each loop to perform */
    for (i = 0; i < loopNum; i++)
    {
        /* if performing receives */
        if (mode & PERFORM_RECEIVE)
        {
            /* if a receive has already been submitted for this buffer */
            if (i >= BUFFER_NUM)
            {
                /* for the number of tests required */
                for (j = 0; j < testCount; j++)
                {
                    /* wait on packets being received */
                    status = ppReceiveOps[bufferNum * testCount + j]->
                        WaitOnTransferOperationCompletion(
                        TransferOperation::WAIT_INDEFINITELY);

                    /* get items received */
                    ppReceivedItems = ppReceiveOps[bufferNum * testCount + j]->
                        GetTransferItemList(&receivedItemCount);

                    /* if receive failed to complete */
                    if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                    {
                        /* report error, jump to clean-up phase of processing */
                        printf("ERROR: Could not complete receive in loop %d,"
                            " error of %d\n", i, status);
                        goto Rate_End_Cleanup;
                    }

                    /* if receive completed but no items received */
                    if (ppReceivedItems == NULL)
                    {
                        /* report error, jump to clean-up phase of processing */
                        printf("ERROR: Receive completed but no data was "
                            "received. Loop: %d\n", loopNum);
                        goto Rate_End_Cleanup;
                    }

                    /* for number of items received */
                    for (packetItem=0; packetItem < receivedItemCount;
                        packetItem++)
                    {
                        /* get next received item as a SpaceWire Packet */
                        pReceivedPacket = (Packet*)ppReceivedItems[packetItem];

                        /* get packet size */
                        rxPackSize = pReceivedPacket->GetPacketLength();

                        /* if packet size not as expected */
                        if (rxPackSize != (unsigned int)packetSize)
                        {
                            /* report discrepancy in packet size */
                            printf("Rx operation completed, received incorrect"
                                " packet size: %u, Loop %d \n", rxPackSize, 
                                loopNum);
                        }
                    }

                    /* free received items */
                    ppReceiveOps[bufferNum * testCount + j]->
                        FreeTransferItemList(ppReceivedItems, 
                        receivedItemCount);

                    /* if receive failed */
                    if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                    {
                        /* report error, jump to clean-up phase */
                        printf("ERROR: Could not complete receive in loop %d,"
                            " error of %d\n", i, status);
                        goto Rate_End_Cleanup;
                    }
                }

                /* if this is the first receive and not performing transmits */
                if ((i == BUFFER_NUM) && (!(mode & PERFORM_TRANSMIT)))
                {
                    /* start the clock */
                    start = GET_TIME();
                    CPUUsageData::STORE_CPU_USAGE(pStartCpuProperties);
                }
            }

            /* start receiving the next group of packets */
            for (j = 0; j < testCount; j++)
            {
                /* submit next receive op */
                pRxChannels[j].SubmitTransferOperation(
                    ppReceiveOps[bufferNum * testCount + j]);

                /* get the status of the receive op */
                status = ppReceiveOps[bufferNum * testCount + j]->
                    GetTransferOperationStatus();

                /* if receive failed */
                if ((status != TransferOperation::TRANSFER_STATUS_STARTED) &&
                    (status != TransferOperation::TRANSFER_STATUS_COMPLETE))
                {
                    /* report error, jump to clean-up phase */
                    printf("Could not perform receive in loop %d, error of %d\n",
                            i, status);
                    goto Rate_End_Cleanup;
                }
            }
        }

        /* if performing transmits */
        if (mode & PERFORM_TRANSMIT)
        {
            /* if a transmit has already been submitted for this buffer */
            if (i >= BUFFER_NUM)
            {
                /* for each test being performed */
                for (j = 0; j < testCount; j++)
                {
                    /* wait on the last transmit completing */
                    status = ppTransmitOps[bufferNum * testCount + j]->
                        WaitOnTransferOperationCompletion(
                        TransferOperation::WAIT_INDEFINITELY);

                    /* if transmit failed to complete */
                    if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                    {
                        /* report error, jump to clean-up phase */
                        printf("ERROR: Could not complete transmit in loop %d,"
                            " error of %d\n", i, status);
                        goto Rate_End_Cleanup;
                    }
                }
            }

            /* for each test being performed */
            for (j = 0; j < testCount; j++)
            {
                /* start transmitting the next packets */
                pTxChannels[j].SubmitTransferOperation(
                    ppTransmitOps[bufferNum * testCount + j]);

                /* get the status of the transmit */
                status = ppTransmitOps[bufferNum * testCount + j]->
                    GetTransferOperationStatus();

                /* if transmit failed */
                if ((status != TransferOperation::TRANSFER_STATUS_STARTED) &&
                    (status != TransferOperation::TRANSFER_STATUS_COMPLETE))
                {
                    /* report error, jump to clean-up phase */
                    printf("Could not perform transmit in loop %d, error of"
                        " %d\n", i, status);
                    goto Rate_End_Cleanup;
                }
            }
        }

        /* move to the next buffer */
        bufferNum++;
        if (bufferNum == BUFFER_NUM)
        {
            bufferNum = 0;
        }
    }

    /* for each buffer used */
    for (bufferNum = 0; bufferNum < BUFFER_NUM; bufferNum++)
    {
        /* If performing transmits */
        if (mode & PERFORM_TRANSMIT)
        {
            /* for the number of tests required */
            for (j = 0; j < testCount; j++)
            {
                /* Wait on the next transmit completing */
                status = ppTransmitOps[bufferNum * testCount + j]->
                    WaitOnTransferOperationCompletion(
                    TransferOperation::WAIT_INDEFINITELY);

                /* if transmit failed to complete */
                if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                {
                    /* report error, jump to clean-up phase */
                    printf("ERROR: Could not complete transmit in loop %d,"
                        " error of %d\n", i, status);
                    goto Rate_End_Cleanup;
                }
            }
        }

        /* if performing receives */
        if (mode & PERFORM_RECEIVE)
        {
            /* for the number of tests required */
            for (j = 0; j < testCount; j++)
            {
                /* wait on packets being received */
                status = ppReceiveOps[bufferNum * testCount + j]->
                    WaitOnTransferOperationCompletion(
                    TransferOperation::WAIT_INDEFINITELY);

                /* get items received */
                ppReceivedItems = ppReceiveOps[bufferNum * testCount + j]->
                    GetTransferItemList(&receivedItemCount);

                /* if no items received */
                if(ppReceivedItems == NULL)
                {
                    /* report error, jump to clean-up phase */
                    printf("ERROR: Receive completed but no data was received."
                        " Loop: %d\n", loopNum);
                    goto Rate_End_Cleanup;
                }

                /* for each item received */
                for(packetItem=0; packetItem < receivedItemCount; packetItem++)
                {
                    /* get next item as a SpaceWire packet */
                    pReceivedPacket = (Packet*)ppReceivedItems[packetItem];

                    /* get the size of the packet */
                    rxPackSize = pReceivedPacket->GetPacketLength();

                    /* if packet size not as expected */
                    if(rxPackSize != (unsigned int)packetSize)
                    {
                        /* report discrepancy in packet size */
                        printf("Rx operation completed, received incorrect"
                            " packet size: %u, Loop %d \n", rxPackSize, 
                            loopNum);
                    }
                }

                /* free list of received items */
                ppReceiveOps[bufferNum * testCount + j]->
                    FreeTransferItemList(ppReceivedItems, receivedItemCount);

                /* if receive didn't complete */
                if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                {
                    /* report error, jump to clean-up phase */
                    printf("ERROR: Could not complete receive in loop %d,"
                        " error of %d\n", i, status);
                    goto Rate_End_Cleanup;
                }
            }
        }
    }

    /* stop the clock */
    finish = GET_TIME();
    CPUUsageData::STORE_CPU_USAGE(pEndCpuProperties);

    /* calculate duration */
    duration = (double)(finish - start) / TIME_DIVIDER;

    /* get number of bits sent in packets */
    bitsSent = (double)testCount * (double)packetSize;

    /* if address paths provided */
    if (pAddressPaths)
    {
        /* for the number of tests performed */
        for (i = 0; i < testCount; i++)
        {
            /* if an address path was provided */
            if (pAddressPaths[i])
            {
                /* add address path length to total bits sent */
                bitsSent += pAddressPaths[i]->GetPathLength();
            }
        }
    }

    /* calculate total bit sent */
    bitsSent = bitsSent * (double)packetNum * (double)loopNum * (double)8;

    /* calculate the data and packet rate */
    if (duration)
    {
        *pDataRate = bitsSent / duration / 1000000;
        *pPacketRate = (double)(packetNum * testCount * loopNum) / duration;
    }
    else
    {
        *pDataRate = 0;
        *pPacketRate = 0;
    }
    *pPacketTime = duration * 1000000 /
        (double)(packetNum * testCount * loopNum);

/* cleanup */
Rate_End_Cleanup:

    /* if performing receives */
    if (mode & PERFORM_RECEIVE)
    {
        /* dispose of all receive transfer operations */

        /* if receive ops to dispose */
        if (ppReceiveOps != NULL)
        {
            /* for each item in the list of receive ops */
            for (i = 0; i < BUFFER_NUM * testCount; i++)
            {
                /* if the item is valid (non-NULL) */
                if (ppReceiveOps[i] != NULL)
                {
                    /* free it */
                    delete ppReceiveOps[i];
                }
            }

            /* free the list of receive ops */
            delete [] ppReceiveOps;

            /* set it to NULL */
            ppReceiveOps = NULL;
        }
    }

    /* if performing transmits */
    if (mode & PERFORM_TRANSMIT)
    {
        /* if transmit ops to dispose */
        if (ppTransmitOps != NULL)
        {
            /* for each item in the list of transmit ops */
            for (i = 0; i < BUFFER_NUM * testCount; i++)
            {
                /* if the item is valid (non-NULL) */
                if (ppTransmitOps[i] != NULL)
                {
                    /* free it */
                    delete ppTransmitOps[i];
                }
            }

            /* free the list of transmit ops */
            delete [] ppTransmitOps;

            /* set it to NULL */
            ppTransmitOps = NULL;
        }

        /* if there are packets to dispose */
        if (ppTransmitPackets != NULL)
        {
            /* for each item in the list of packets */
            for (i = 0; i < packetNum * BUFFER_NUM * testCount; i++)
            {
                /* if the item is valid (non-NULL) */
                if (ppTransmitPackets[i] != NULL)
                {
                    /* free it */
                    delete ppTransmitPackets[i];
                }
            }

            /* free the list of packets */
            delete [] ppTransmitPackets;

            /* set it to NULL */
            ppTransmitPackets = NULL;
        }

        /* free the transmit buffer */
        if (pTransmitBuffer)
        {
            free(pTransmitBuffer);
        }
    }

    if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        *pDataRate = 0;
        *pPacketRate = 0;
        *pPacketTime = 0;
    }
}

/**
 * Perform the rate test.
 *
 * @param pTestProperties A pointer to a TestProperties object containing
 *                        the properties of the test to be performed.
 * @param pOutputFile The file to write the statistics to, or NULL if no
 *                    file is to be used.
 */
void StarSystemPerformanceTester::performRateTest(
                            TestProperties *pTestProperties, FILE *pOutputFile)
{
    int packetSize;
    double dataRate, packetRate, packetTime;
    CPUProperties startCpuProperties, endCpuProperties;

    /* if a file is to be written to */
    if (pOutputFile)
    {
        /* write the statistics column headers to the file */
        fprintf(pOutputFile, "Packet Size\tData Rate (Mbit/s)\t");
        fprintf(pOutputFile,
            "Packet Rate (packets/s)\tAverage Packet Time (microseconds)");
        CPUUsageData::PRINT_CPU_USAGE_HEADERS(pOutputFile);
    }

    /* for each packet size to test */
    for (packetSize = pTestProperties->packetStart;
        packetSize <= pTestProperties->packetEnd;)
    {
        /* display the packet size on screen */
        printf(
            "Performing maximum data rate test using packets of %d bytes...\n",
            packetSize);

        /* perform the test for the current packet size */
        PerformRateTestForPacketSize(packetSize, pTestProperties->mode,
            pTestProperties->testCount, pTestProperties->pTxChannels,
            pTestProperties->ppAddresses, pTestProperties->pRxChannels,
            &dataRate, &packetRate, &packetTime, &startCpuProperties,
            &endCpuProperties);

        /* display the results of the test */
        printf(
            "\t%-3.2f Mbit/s  %-3.2f packets/s  %-3.2f microseconds/packet\n",
            dataRate, packetRate, packetTime);
        CPUUsageData::PRINT_CPU_USAGE(NULL, &startCpuProperties, &endCpuProperties);

        /* if writing the results to file */
        if (pOutputFile)
        {
            /* write the results to the file */
            fprintf(pOutputFile, "%8d\t%-3.2f\t%-3.2f\t%-3.2f", packetSize,
                dataRate, packetRate, packetTime);
            CPUUsageData::PRINT_CPU_USAGE(pOutputFile, &startCpuProperties,
                &endCpuProperties);
            fflush(pOutputFile);
        }

        /* if not performing receives and this isn't the last loop */
        if ((!(pTestProperties->mode & PERFORM_RECEIVE)) &&
            (packetSize < pTestProperties->packetEnd))
        {
            /* Sleep for 5 seconds to allow all packets to be sent */
            SLEEP(5000);
        }

        /* move to the next packet size */
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
 * Perform the random test for a specific packet size.
 *
 * @param minPacketSize the minimum size of packet to transmit/receive
 * @param maxPacketSize the maximum size of packet to transmit/receive
 * @param mode the mode of operation (receive and/or transmit)
 * @param testCount the number of tests to perform concurrently
 * @param pTxChannels the channels to transmit on
 * @param ppAddresses the addresses to add to the start of all packets sent,
 *                    if packets are to be sent
 * @param pRxChannels the channels to receive on 
 * @param pDataRate a pointer to a variable which will be updated to contain the
 *                  data rate recorded for the test
 * @param pPacketRate a pointer to a variable which will be updated to contain
 *                    the packet rate recorded for the test
 * @param pPacketTime a pointer to a variable which will be updated to contain
 *                    the average time to transmit a single packet
 * @param pPacketSize a pointer to a variable which will be updated to contain
 *                    the average packet size used in the test
 * @param pStartCpuProperties a pointer to an object which will be updated to
 *                            contain the CPU properties when the test is
 *                            started
 * @param pEndCpuProperties a pointer to an object which will be updated to
 *                          contain the CPU properties when the test completes
 */
void StarSystemPerformanceTester::performRandomTestForPacketSize(
    int minPacketSize, int maxPacketSize,
    int mode, int testCount, Channel *pTxChannels,
    Address **ppAddresses, Channel *pRxChannels,
    double *pDataRate, double *pPacketRate, double *pPacketTime,
    double *pPacketSize, CPUProperties *pStartCpuProperties,
    CPUProperties *pEndCpuProperties)
{
    int i, j, bufferNum, packetNum, loopNum;
    TransmitOperation **ppTransmitOps = NULL, *pTransmitOp = NULL;
    ReceiveOperation **ppReceiveOps = NULL, *pReceiveOp = NULL;
    clock_t start = 0, finish;
    double duration, bytesSent = 0;
    TransferOperation::TransferStatus status = TransferOperation::TRANSFER_STATUS_ERROR;
    unsigned char *pTransmitBuffer = NULL;
    StreamItem **ppTransmitPackets = NULL;
    int *pPacketLengths = NULL;
    Packet *pPacket = NULL;

    /* seed random number function */
    srand((unsigned int)time(NULL));

    /* calculate the number of packets to transmit in each loop and 
     * the number of loops to perform
     */

    /* if addresses are available */
    if (ppAddresses)
    {
        /* set number of packets to default number */
        packetNum = BUFFER_NUM;

        /* for each test being performed */
        for (i = 0; i < testCount; i++)
        {
            /* if address available for test */
            if (ppAddresses[i])
            {
                /* set number of packets to be greater of the current
                 * number required OR the number which can fit into the
                 * buffer inclusive of the address path
                 */
                packetNum = MAX(packetNum, BUFFER_SIZE /
                    (maxPacketSize + ppAddresses[i]->GetPathLength()));
            }
            else
            {
                /* set number of packets to be greater of the current
                 * number required OR the number which can fit into the
                 * buffer excluding the address path
                 */
                packetNum = MAX(packetNum, BUFFER_SIZE / maxPacketSize);
            }
        }
    }
    else
    {
        packetNum = MAX(BUFFER_NUM, BUFFER_SIZE / maxPacketSize);
    }
    loopNum = LOOP_NUM;

    /* allocate memory to hold length info for packets */
    pPacketLengths = (int *)calloc((size_t)(packetNum * BUFFER_NUM), 
        (size_t)sizeof(int));

    /* if failed to allocate memory */
    if (!pPacketLengths)
    {
        /* report error, goto clean up stage */
        puts("Couldn't allocate memory for the packet lengths, exiting.");
        goto Random_End_Cleanup;
    }

    /* for the number of packets required */
    for (i = 0; i < packetNum * BUFFER_NUM; i++)
    {
        /* set the initial random lengths for the packets */
        pPacketLengths[i] = (int)((double)rand() / ((double)RAND_MAX + 1) *
            (maxPacketSize - minPacketSize)) + minPacketSize;
    }

    /* if performing receives */
    if (mode & PERFORM_RECEIVE)
    {
        /* create an array of pointers to receive operations */
        try
        {
            ppReceiveOps = new ReceiveOperation*[BUFFER_NUM * testCount];
        }
        catch (std::bad_alloc& ba)
        {
            /* memory allocation error */
            ppReceiveOps = NULL;
            UNREFERENCED_PARAMETER(ba);
        }

        /* if failed to create array of pointers to receive ops */
        if (ppReceiveOps == NULL)
        {
            /* report error, jump to clean-up phase of processing */
            puts("Couldn't allocate memory for receive operations, exiting.");
            goto Random_End_Cleanup;
        }
        else
        {
            /* initialise all pointers in array to NULL */
            for (i=0;i<BUFFER_NUM * testCount; i++)
                ppReceiveOps[i] = NULL;
        }

        /* create transfer operations to receive all packets in each buffer */

        /* for the number of receive ops required */
        for (i = 0; i < BUFFER_NUM * testCount; i++)
        {
            /* create a new receive operation */
            try
            {
                pReceiveOp = new ReceiveOperation(packetNum,
                                            ReceiveOperation::RECEIVE_PACKETS);
            }
            catch (std::bad_alloc& ba)
            {
                /* memory allocation error */
                pReceiveOp = NULL;
                UNREFERENCED_PARAMETER(ba);
            }
            catch (TransferOperationException& tOpEx)
            {
                /* failed to create receive op */
                pReceiveOp = NULL;
                UNREFERENCED_PARAMETER(tOpEx);
            }

            /* if failed to create receive op */
            if (pReceiveOp == NULL)
            {
                /* report error, jump to clean-up phase of processing */
                puts("Couldn't create receive operation, exiting.");
                goto Random_End_Cleanup;
            }
            else
            {
                /* add receive op to next position in array of receive ops */
                ppReceiveOps[i] = pReceiveOp;
            }
        }
    }

    /* if performing transmits */
    if (mode & PERFORM_TRANSMIT)
    {
        /* allocate transmit buffer */
        pTransmitBuffer = (unsigned char *)calloc(1, (size_t)maxPacketSize);

        /* if failed to allocate buffer */
        if (!pTransmitBuffer)
        {
            /* report error, go to clean-up stage */
            puts("Couldn't allocate memory for transmit buffer, exiting.");
            goto Random_End_Cleanup;
        }

        /* create an array of pointers to stream items - this will eventually 
         * hold the packets being sent
         */
        try
        {
            ppTransmitPackets = new StreamItem*[packetNum * BUFFER_NUM * testCount];
        }
        catch (std::bad_alloc& ba)
        {
            /* memory allocation error */
            ppTransmitPackets = NULL;
            UNREFERENCED_PARAMETER(ba);
        }

        /* if failed to create array of pointers to stream items */
        if (ppTransmitPackets == NULL)
        {
            /* report error, jump to clean-up phase of processing */
            puts("Couldn't allocate memory for transmit packets, exiting.");
            goto Random_End_Cleanup;
        }
        else
        {
            /* initialise all pointers in array to NULL */
            for (i=0;i<packetNum * BUFFER_NUM * testCount; i++)
                ppTransmitPackets[i] = NULL;
        }

        /* create an array of pointers to transmit operations */
        try
        {
            ppTransmitOps = new TransmitOperation*[BUFFER_NUM * testCount];
        }
        catch (std::bad_alloc& ba)
        {
            /* memory allocation error */
            ppTransmitOps = NULL;
            UNREFERENCED_PARAMETER(ba);
        }

        /* if failed to create array of pointers to transmit ops */
        if (ppTransmitOps == NULL)
        {
            /* report error, jump to clean-up phase of processing */
            puts("Couldn't allocate memory for transmit operations, exiting.");
            goto Random_End_Cleanup;
        }
        else
        {
            /* initialise all pointers in array to NULL */
            for (i=0;i<BUFFER_NUM * testCount; i++)
                ppTransmitOps[i] = NULL;
        }

        /* for the number of packets required */
        for (i = 0; i < packetNum * BUFFER_NUM; i++)
        {
            /* for the number of tests being performed */
            for (j = 0; j < testCount; j++)
            {
                /* create a new SpaceWire packet */
                try
                {
                    pPacket = new Packet(ppAddresses[j],
                                                    pTransmitBuffer,
                                                    (unsigned int)pPacketLengths[i],
                                                    Packet::EOP_TYPE_EOP);
                }
                catch (std::bad_alloc& ba)
                {
                    /* memory allocation failed */
                    pPacket = NULL;
                    UNREFERENCED_PARAMETER(ba);
                }
                catch (PacketException& pEx)
                {
                    /* failed to create packet */
                    pPacket = NULL;
                    UNREFERENCED_PARAMETER(pEx);
                }

                /* if failed to create packet */
                if (pPacket == NULL)
                {
                    /* report error, jump to clean-up phase of processing */
                    puts("Couldn't create packet for transmit operation,"
                        " exiting.");
                    goto Random_End_Cleanup;
                }
                else
                {
                    /* add packet to list of packets being sent */
                    ppTransmitPackets[i * testCount + j] = pPacket;
                }
            }
        }

        /* create transfer operations to transmit all packets in each buffer */

        /* for the number of transmit operations required */
        for (i = 0; i < BUFFER_NUM * testCount; i++)
        {
            /* create a new transmit operation */
            try
            {
                pTransmitOp = new TransmitOperation(
                                        ppTransmitPackets + (packetNum * i),
                                        (unsigned int)packetNum);
            }
            catch (std::bad_alloc& ba)
            {
                /* memory allocation error */
                pTransmitOp = NULL;
                UNREFERENCED_PARAMETER(ba);
            }
            catch (TransferOperationException& tOpEx)
            {
                /* failed to create transmit op */
                pTransmitOp = NULL;
                UNREFERENCED_PARAMETER(tOpEx);
            }

            /* if failed to create transmit op */
            if (pTransmitOp == NULL)
            {
                /* report error, jump to clean-up phase of processing */
                puts("Couldn't create transmit operation, exiting.");
                goto Random_End_Cleanup;
            }
            else
            {
                /* add transmit op to next position in array of transmit ops */
                ppTransmitOps[i] = pTransmitOp;
            }
        }

        /* Start the clock */
        start = GET_TIME();
        CPUUsageData::STORE_CPU_USAGE(pStartCpuProperties);
    }

    bufferNum = 0;

    /* for each loop to perform */
    for (i = 0; i < loopNum; i++)
    {
        /* if performing receives */
        if (mode & PERFORM_RECEIVE)
        {
            /* if a receive has already been submitted for this buffer */
            if (i >= BUFFER_NUM)
            {
                /* for the number of tests required */
                for (j = 0; j < testCount; j++)
                {
                    /* wait on packets being received */
                    status = ppReceiveOps[bufferNum * testCount + j]->
                        WaitOnTransferOperationCompletion(
                        TransferOperation::WAIT_INDEFINITELY);

                    /* if receive failed to complete */
                    if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                    {
                        /* report error, jump to clean-up phase of processing */
                        printf("ERROR: Could not complete receive in loop %d,"
                            " error of %d\n", i, status);
                        goto Random_End_Cleanup;
                    }
                }

                /* if this is the first receive and not performing transmits */
                if ((i == BUFFER_NUM) && (!(mode & PERFORM_TRANSMIT)))
                {
                    /* start the clock */
                    start = GET_TIME();
                    CPUUsageData::STORE_CPU_USAGE(pStartCpuProperties);
                }
            }

            /* start receiving the next group of packets */

            /* for number of tests required */
            for (j = 0; j < testCount; j++)
            {
                /* submit next receive op */
                pRxChannels[j].SubmitTransferOperation(ppReceiveOps[bufferNum * testCount + j]);

                /* get the status of the receive op */
                status = ppReceiveOps[bufferNum * testCount + j]->GetTransferOperationStatus();

                /* if receive failed */
                if ((status != TransferOperation::TRANSFER_STATUS_STARTED) &&
                    (status != TransferOperation::TRANSFER_STATUS_COMPLETE))
                {
                    /* report error, jump to clean-up phase */
                    printf("Could not perform receive in loop %d, error of %d\n",
                            i, status);
                    goto Random_End_Cleanup;
                }
            }
        }

        /* if performing transmits */
        if (mode & PERFORM_TRANSMIT)
        {
            /* if a transmit op has already been submitted for this buffer */
            if (i >= BUFFER_NUM)
            {
                /* wait on the last transmit completing */

                /* for each test being performed */
                for (j = 0; j < testCount; j++)
                {
                    /* wait on the last transmit completing */
                    status = ppTransmitOps[bufferNum * testCount + j]->
                        WaitOnTransferOperationCompletion(
                        TransferOperation::WAIT_INDEFINITELY);

                    /* if transmit failed to complete */
                    if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                    {
                        /* report error, jump to clean-up phase */
                        printf("ERROR: Could not complete transmit in loop %d,"
                            " error of %d\n", i, status);
                        goto Random_End_Cleanup;
                    }
                }
            }
        }

        /* update the number of bytes transmitted/received */
        for (j = 0; j < packetNum; j++)
        {
            bytesSent +=
                pPacketLengths[(packetNum * bufferNum) + j] * testCount;
        }

        /* if path addresses are being used */
        if (ppAddresses)
        {
            /* for the number of tests being performed */
            for (j = 0; j < testCount; j++)
            {
                /* if an address path is being used for the test */
                if (ppAddresses[j])
                {
                    /* add address path length to byte count too */
                    bytesSent += packetNum * ppAddresses[j]->GetPathLength();
                }
            }
        }

        /* if performing transmits */
        if (mode & PERFORM_TRANSMIT)
        {
            /* \note This current version of the Performance Tester does 
             * not change the size of the data in each loop. This is due to 
             * an as yet unfixed bug in STAR_setPacketData()
             */

            /* start transmitting the next packets */

            /* for ythe number of tests being performed */
            for (j = 0; j < testCount; j++)
            {
                /* submit the next transmit operation */
                pTxChannels[j].SubmitTransferOperation(
                    ppTransmitOps[bufferNum * testCount + j]);

                /* get the status of the transmit operation */
                status = ppTransmitOps[bufferNum * testCount + j]->
                    GetTransferOperationStatus();

                /* if transmit failed */
                if ((status != TransferOperation::TRANSFER_STATUS_STARTED) &&
                    (status != TransferOperation::TRANSFER_STATUS_COMPLETE))
                {
                    /* report error, jump to clean-up phase */
                    printf("Could not perform transmit in loop %d, error of"
                        " %d\n", i, status);
                    goto Random_End_Cleanup;
                }
            }
        }

        /* move to the next buffer */
        bufferNum++;
        if (bufferNum == BUFFER_NUM)
        {
            bufferNum = 0;
        }
    }

    /* for each buffer used */
    for (bufferNum = 0; bufferNum < BUFFER_NUM; bufferNum++)
    {
        /* if performing transmits */
        if (mode & PERFORM_TRANSMIT)
        {
            /* wait on the next transmit completing */

            /* for the number of tests required */
            for (j = 0; j < testCount; j++)
            {
                /* wait on the next transmit op completing */
                status = ppTransmitOps[bufferNum * testCount + j]->
                    WaitOnTransferOperationCompletion(
                    TransferOperation::WAIT_INDEFINITELY);

                /* if transmit op failed to complete */
                if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                {
                    /* report error, jump to clean-up phase */
                    printf("ERROR: Could not complete transmit in loop %d,"
                        " error of %d\n", i, status);
                    goto Random_End_Cleanup;
                }
            }
        }

        /* if performing receives */
        if (mode & PERFORM_RECEIVE)
        {
            /* wait on packets being received */

            /* for the number of tests being performed */
            for (j = 0; j < testCount; j++)
            {
                /* wait on the receive op completing */
                status = ppReceiveOps[bufferNum * testCount + j]->
                    WaitOnTransferOperationCompletion(
                    TransferOperation::WAIT_INDEFINITELY);

                /* if receive op failed to complete */
                if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                {
                    /* report error, jump to clean-up phase */
                    printf("ERROR: Could not complete receive in loop %d,"
                        " error of %d\n", i, status);
                    goto Random_End_Cleanup;
                }
            }
        }
    }

    /* stop the clock */
    finish = GET_TIME();
    CPUUsageData::STORE_CPU_USAGE(pEndCpuProperties);

    /* calculate test duration */
    duration = (double)(finish - start) / TIME_DIVIDER;

    /* calculate the average packet size */
    *pPacketSize = (double)bytesSent / (packetNum * testCount * loopNum);

    /* calculate the data and packet rate */
    if (duration)
    {
        *pDataRate = (double)(bytesSent * 8) / duration / 1000000;
        *pPacketRate = (double)(packetNum * testCount * loopNum) / duration;
    }
    else
    {
        *pDataRate = 0;
        *pPacketRate = 0;
    }

    *pPacketTime = duration * 1000000 /
        (double)(packetNum * testCount * loopNum);

/* cleanup */
Random_End_Cleanup:

    /* if performing receives */
    if (mode & PERFORM_RECEIVE)
    {
        /* if array of receive ops to free */
        if (ppReceiveOps != NULL)
        {
            /* for each receive op in the array */
            for (i = 0; i < BUFFER_NUM * testCount; i++)
            {
                /* free it from memory if required*/
                if (ppReceiveOps[i] != NULL)
                    delete ppReceiveOps[i];
            }

            /* free the array itself */
            delete [] ppReceiveOps;
        }
    }

    /* if performing transmits */
    if (mode & PERFORM_TRANSMIT)
    {
        /* if there is an array of transmit ops to free */
        if (ppTransmitOps != NULL)
        {
            /* for each transmit op in the array */
            for (i = 0; i < BUFFER_NUM * testCount; i++)
            {
                /* free it from memory if required */
                if (ppTransmitOps[i])
                    delete ppTransmitOps[i];
            }

            /* free the array itself */
            delete [] ppTransmitOps;
        }

        /* free transmit stream items (packets) created */
        if (ppTransmitPackets != NULL)
        {
            /* for the number of packets created */
            for (i = 0; i < packetNum * BUFFER_NUM * testCount; i++)
            {
                /* free the corresponding packet held in the array, if required */
                if (ppTransmitPackets[i] != NULL)
                    delete ppTransmitPackets[i];
            }

            /* free the array itself */
            delete [] ppTransmitPackets;
        }

        /* free the transmit buffer */
        if (pTransmitBuffer)
        {
            free(pTransmitBuffer);
        }
    }

    /* free the array of packet lengths */
    if (pPacketLengths)
    {
        free(pPacketLengths);
    }

    if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        *pDataRate = 0;
        *pPacketRate = 0;
        *pPacketTime = 0;
    }
}

/**
 * Perform the random test.
 *
 * @param pTestProperties A pointer to an object containing the properties of
 *                        the test to be performed
 * @param pOutputFile the file to write the statistics to, or NULL if no file 
 *                    is to be used
 */
void StarSystemPerformanceTester::performRandomTest(
                            TestProperties *pTestProperties, FILE *pOutputFile)
{
    int minPacketSize;
    double dataRate, packetRate, packetTime, packetSize;
    CPUProperties startCpuProperties, endCpuProperties;

    /* if a file is to be written to */
    if (pOutputFile)
    {
        /* write the statistics column headers to the file */
        fprintf(pOutputFile,
            "Minimum Packet Size\tMaximum Packet Size\tData Rate (Mbit/s)\t");
        fprintf(pOutputFile,
            "Packet Rate (packets/s)\tAverage Packet Time (microseconds)\t");
        fprintf(pOutputFile,
            "Average Packet Size (bytes)");
        CPUUsageData::PRINT_CPU_USAGE_HEADERS(pOutputFile);
    }

    /* for each packet size to test */
    for (minPacketSize = pTestProperties->packetStart;
        minPacketSize <= pTestProperties->packetEnd;)
    {
        /* display the packet size on screen */
        printf("Performing random packet size data rate test using packets of"
            " size %d to %d bytes...\n", minPacketSize, 
            pTestProperties->packetEnd);

        /* perform the test for the current packet size */
        performRandomTestForPacketSize(minPacketSize,
            pTestProperties->packetEnd, pTestProperties->mode,
            pTestProperties->testCount, pTestProperties->pTxChannels,
            pTestProperties->ppAddresses, pTestProperties->pRxChannels,
            &dataRate, &packetRate, &packetTime, &packetSize,
            &startCpuProperties, &endCpuProperties);

        /* display the results of the test */
        printf("\t%-3.2f Mbit/s  %-3.2f packets/s  %-3.2f microseconds/packet"
            "  %-3.2f average packet size\n", dataRate, packetRate, packetTime, 
            packetSize);
        CPUUsageData::PRINT_CPU_USAGE(NULL, &startCpuProperties, 
            &endCpuProperties);

        /* if writing the results to file */
        if (pOutputFile)
        {
            /* write the results to the file */
            fprintf(pOutputFile, "%8d\t%8d\t%-3.2f\t%-3.2f\t%-3.2f\t%-3.2f",
                minPacketSize, pTestProperties->packetEnd, dataRate, 
                packetRate, packetTime, packetSize);
            CPUUsageData::PRINT_CPU_USAGE(pOutputFile, &startCpuProperties,
                &endCpuProperties);
            fflush(pOutputFile);
        }

        /* if not performing receives and this isn't the last loop */
        if ((!(pTestProperties->mode & PERFORM_RECEIVE)) &&
            (minPacketSize < pTestProperties->packetEnd))
        {
            /* sleep for 5 seconds to allow all packets to be sent */
            SLEEP(5000);
        }

        /* move to the next packet size */
        if (minPacketSize == pTestProperties->packetEnd)
        {
            minPacketSize++;
        }
        else if ((minPacketSize == pTestProperties->packetStart) &&
            (pTestProperties->packetStep != 0) && (minPacketSize +
                pTestProperties->packetStep > pTestProperties->packetEnd))
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
void StarSystemPerformanceTester::performLatencyTestForPacketSize(int packetSize,
    int mode, int testCount,
    Channel *pTxChannels, Address **ppAddressPaths,
    Channel *pRxChannels, double *pDataRate, double *pPacketRate,
    double *pPacketTime, CPUProperties *pStartCpuProperties,
    CPUProperties *pEndCpuProperties)
{
    int i, j, loopNum;
    TransmitOperation **ppTransmitOps = NULL, *pTransmitOp = NULL;
    ReceiveOperation **ppReceiveOps = NULL, *pReceiveOp = NULL;
    clock_t start = 0, finish;
    double duration, bitsSent;
    TransferOperation::TransferStatus status = TransferOperation::TRANSFER_STATUS_ERROR;
    unsigned char *pTransmitBuffer = NULL;
    StreamItem **ppTransmitPackets = NULL;
    Packet *pPacket = NULL;

    /* calculate the number of loops to perform */
    loopNum = LOOP_NUM;

    /* if performing receives */
    if (mode & PERFORM_RECEIVE)
    {
        /* create an array of pointers to receive operations */
        try
        {
            ppReceiveOps = new ReceiveOperation*[testCount];
        }
        catch (std::bad_alloc& ba)
        {
            /* memory allocation error */
            ppReceiveOps = NULL;
            UNREFERENCED_PARAMETER(ba);
        }

        /* if failed to create array of pointers to receive ops */
        if (ppReceiveOps == NULL)
        {
            /* report error, jump to clean-up phase of processing */
            puts("Couldn't allocate memory for receive operations, exiting.");
            goto Latency_End_Cleanup;
        }
        else
        {
            /* initialise all pointers in array to NULL */
            for (i=0;i<testCount; i++)
                ppReceiveOps[i] = NULL;
        }

        /* for the number of tests required */
        for (i = 0; i < testCount; i++)
        {
            /* create a new receive operation */
            try
            {
                pReceiveOp = new ReceiveOperation(1,
                                            ReceiveOperation::RECEIVE_PACKETS);
            }
            catch (std::bad_alloc& ba)
            {
                /* memory allocation error */
                pReceiveOp = NULL;
                UNREFERENCED_PARAMETER(ba);
            }
            catch (TransferOperationException& tOpEx)
            {
                /* failed to create receive op */
                pReceiveOp = NULL;
                UNREFERENCED_PARAMETER(tOpEx);
            }

            /* if failed to create receive op */
            if (pReceiveOp == NULL)
            {
                /* report error, jump to clean-up phase of processing */
                puts("Couldn't create receive operation, exiting.");
                goto Latency_End_Cleanup;
            }
            else
            {
                /* add receive op to next position in array of receive ops */
                ppReceiveOps[i] = pReceiveOp;
            }
        }
    }

    /* if performing transmits */
    if (mode & PERFORM_TRANSMIT)
    {
        /* allocate transmit buffer */
        pTransmitBuffer = (unsigned char *)calloc(1, (size_t)packetSize);
        if (!pTransmitBuffer)
        {
            puts("Couldn't allocate memory for transmit buffer, exiting.");
            goto Latency_End_Cleanup;
        }

        /* create an array of pointers to stream items - this will
         * eventually hold the packets being sent
         */
        try
        {
            ppTransmitPackets = new StreamItem*[testCount];
        }
        catch (std::bad_alloc& ba)
        {
            /* memory allocation error */
            ppTransmitPackets = NULL;
            UNREFERENCED_PARAMETER(ba);
        }

        /* if failed to create array of pointers to stream items */
        if (ppTransmitPackets == NULL)
        {
            /* report error, jump to clean-up phase of processing */
            puts("Couldn't allocate memory for transmit packets, exiting.");
            goto Latency_End_Cleanup;
        }
        else
        {
            /* initialise all pointers in array to NULL */
            for (i=0;i<testCount; i++)
                ppTransmitPackets[i] = NULL;
        }

        /* create an array of pointers to transmit operations */
        try
        {
            ppTransmitOps = new TransmitOperation*[testCount];
        }
        catch (std::bad_alloc& ba)
        {
            /* memory allocation error */
            ppTransmitOps = NULL;
            UNREFERENCED_PARAMETER(ba);
        }

        /* if failed to create array of pointers to transmit ops */
        if (ppTransmitOps == NULL)
        {
            /* report error, jump to clean-up phase of processing */
            puts("Couldn't allocate memory for transmit operations, exiting.");
            goto Latency_End_Cleanup;
        }
        else
        {
            /* initialise all pointers in array to NULL */
            for (i=0;i<testCount; i++)
                ppTransmitOps[i] = NULL;
        }

        /* for the number of tests being performed */
        for (i = 0; i < testCount; i++)
        {
            /* Create a packet to be transferred */
            try
            {
                pPacket = new Packet(ppAddressPaths[i], pTransmitBuffer, 
                    (unsigned int)packetSize, Packet::EOP_TYPE_EOP);
            }
            catch (std::bad_alloc& ba)
            {
                /* memory allocation failed */
                pPacket = NULL;
                UNREFERENCED_PARAMETER(ba);
            }
            catch (PacketException& pEx)
            {
                /* failed to create packet */
                pPacket = NULL;
                UNREFERENCED_PARAMETER(pEx);
            }

            /* if failed to create packet */
            if (pPacket == NULL)
            {
                /* report error, jump to clean-up phase of processing */
                puts("Couldn't create the packet to be transferred, exiting.");
                goto Latency_End_Cleanup;
            }
            else
            {
                /* add packet to list of packets to transmit */
                ppTransmitPackets[i] = pPacket;
            }

            /* create a transfer operation to transmit the packet */
            try
            {
                pTransmitOp = new TransmitOperation(&ppTransmitPackets[i], 1);
            }
            catch (std::bad_alloc& ba)
            {
                /* memory allocation error */
                pTransmitOp = NULL;
                UNREFERENCED_PARAMETER(ba);
            }
            catch (TransferOperationException& tOpEx)
            {
                /* failed to create transmit op */
                pTransmitOp = NULL;
                UNREFERENCED_PARAMETER(tOpEx);
            }

            /* if failed to create transmit op */
            if (pTransmitOp == NULL)
            {
                /* report error, jump to clean-up phase of processing */
                puts("Couldn't create transmit operation, exiting.");
                goto Latency_End_Cleanup;
            }
            else
            {
                /* add transmit op to next position in array of transmit ops */
                ppTransmitOps[i] = pTransmitOp;
            }
        }

        /* start the clock */
        start = GET_TIME();
        CPUUsageData::STORE_CPU_USAGE(pStartCpuProperties);
    }

    /* for each loop to perform */
    for (i = 0; i < loopNum; i++)
    {
        /* if performing transmits */
        if (mode & PERFORM_TRANSMIT)
        {
            /* for the number of tests required */
            for (j = 0; j < testCount; j++)
            {
                /* start transmitting the packet */
                pTxChannels[j].SubmitTransferOperation(ppTransmitOps[j]);

                /* get status of transmit */
                status = ppTransmitOps[j]->GetTransferOperationStatus();

                /* if transmit didn't complete */
                if ((status != TransferOperation::TRANSFER_STATUS_STARTED) &&
                    (status != TransferOperation::TRANSFER_STATUS_COMPLETE))
                {
                    /* report error, goto clean-up phase */
                    printf("Could not perform transmit in loop %d, error of %d\n",
                            i, status);
                    goto Latency_End_Cleanup;
                }
            }
        }

        /* if performing receives */
        if (mode & PERFORM_RECEIVE)
        {
            /* start receiving the next packet */
            for (j = 0; j < testCount; j++)
            {
                pRxChannels[j].SubmitTransferOperation(ppReceiveOps[j]);
            }

            /* wait on packets being received as below: */

            /* for the number of tests performed */
            for (j = 0; j < testCount; j++)
            {
                /* get the status of the receive */
                status = ppReceiveOps[j]->WaitOnTransferOperationCompletion(TransferOperation::WAIT_INDEFINITELY);

                /* if receive failed to complete */
                if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                {
                    /* report error, goto clean-up phase */
                    printf("Could not receive packet in loop %d, error of %d\n",
                        i, status);
                    goto Latency_End_Cleanup;
                }
            }

            /* if this is the first receive and not performing transmits */
            if ((i == 0) && (!(mode & PERFORM_TRANSMIT)))
            {
                /* start the clock */
                start = GET_TIME();
                CPUUsageData::STORE_CPU_USAGE(pStartCpuProperties);
            }
        }

        if ((mode & PERFORM_TRANSMIT) && (!(mode & PERFORM_RECEIVE)))
        {
            /* wait on the transmit completing */
            for (j = 0; j < testCount; j++)
            {
                status = ppTransmitOps[j]->WaitOnTransferOperationCompletion(TransferOperation::WAIT_INDEFINITELY);

                /* if transmit failed to complete */
                if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
                {
                    /* report error, goto clean-up phase */
                    printf("Could not complete transmit in loop %d, error of %d\n",
                        i, status);
                    goto Latency_End_Cleanup;
                }
            }
        }
    }

    /* stop the clock */
    finish = GET_TIME();
    CPUUsageData::STORE_CPU_USAGE(pEndCpuProperties);

    /* calculate duration of test */
    duration = (double)(finish - start) / TIME_DIVIDER;

    /* calculate bits sent, based on packet size and number of tests */
    bitsSent = (double)packetSize * (double)testCount;

    /* if address paths were used */
    if (ppAddressPaths)
    {
        /* include address paths in bits sent */
        for (i = 0; i < testCount; i++)
        {
            if (ppAddressPaths[i])
            {
                bitsSent += ppAddressPaths[i]->GetPathLength();
            }
        }
    }
    bitsSent = bitsSent * (double)loopNum * (double)8;

    /* calculate the data and packet rate */
    if (duration)
    {
        *pDataRate = bitsSent / duration / 1000000;
        *pPacketRate = (double)(loopNum * testCount) / duration;
    }
    else
    {
        *pDataRate = 0;
        *pPacketRate = 0;
    }
    *pPacketTime = duration * 1000000 / (double)(loopNum * testCount);

/* cleanup */
Latency_End_Cleanup:

    /* if performing receives */
    if (mode & PERFORM_RECEIVE)
    {
        /* if array of receive ops to free */
        if (ppReceiveOps != NULL)
        {
            /* for each receive op in the array */
            for (i = 0; i < testCount; i++)
            {
                /* free it from memory if required*/
                if (ppReceiveOps[i] != NULL)
                    delete ppReceiveOps[i];
            }

            /* free the array itself */
            delete [] ppReceiveOps;
        }
    }

    /* if performing transmits */
    if (mode & PERFORM_TRANSMIT)
    {
        /* if there is an array of transmit ops to free */
        if (ppTransmitOps != NULL)
        {
            /* for each transmit op in the array */
            for (i = 0; i < testCount; i++)
            {
                /* free it from memory if required */
                if (ppTransmitOps[i])
                    delete ppTransmitOps[i];
            }

            /* free the array itself */
            delete [] ppTransmitOps;
        }

        /* free transmit stream items (packets) created */
        if (ppTransmitPackets != NULL)
        {
            /* for the number of packets created */
            for (i = 0; i < testCount; i++)
            {
                /* free the corresponding packet held in the array, if required */
                if (ppTransmitPackets[i] != NULL)
                    delete ppTransmitPackets[i];
            }

            /* free the array itself */
            delete [] ppTransmitPackets;
        }

        /* free the transmit buffer */
        if (pTransmitBuffer)
        {
            free(pTransmitBuffer);
        }
    }

    /* in the event of an error */
    if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        *pDataRate = 0;
        *pPacketRate = 0;
        *pPacketTime = 0;
    }
}

/**
 * Perform the latency test.
 *
 * @param pTestProperties a pointer to an object containing the properties of
 *                        the test to be performed
 * @param pOutputFile the file to write the statistics to, or NULL if no file 
 *                    is to be used
 */
void StarSystemPerformanceTester::performLatencyTest(
                            TestProperties *pTestProperties, FILE *pOutputFile)
{
    int packetSize;
    double dataRate, packetRate, packetTime;
    CPUProperties startCpuProperties, endCpuProperties;

    /* if a file is to be written to */
    if (pOutputFile)
    {
        /* write the statistics column headers to the file */
        fprintf(pOutputFile, "Packet Size\tData Rate (Mbit/s)\t");
        fprintf(pOutputFile,
            "Packet Rate (packets/s)\tAverage Packet Time (microseconds)");
        CPUUsageData::PRINT_CPU_USAGE_HEADERS(pOutputFile);
    }

    /* for each packet size to test */
    for (packetSize = pTestProperties->packetStart;
        packetSize <= pTestProperties->packetEnd;)
    {
        /* display the packet size on screen */
        printf("Performing latency test using packets of %d bytes...\n",
            packetSize);

        /* perform the test for the current packet size */
        performLatencyTestForPacketSize(packetSize, pTestProperties->mode,
            pTestProperties->testCount, pTestProperties->pTxChannels,
            pTestProperties->ppAddresses, pTestProperties->pRxChannels,
            &dataRate, &packetRate, &packetTime, &startCpuProperties,
            &endCpuProperties);

        /* display the results of the test */
        printf(
            "\t%-3.2f Mbit/s  %-3.2f packets/s  %-3.2f microseconds/packet\n",
            dataRate, packetRate, packetTime);
        CPUUsageData::PRINT_CPU_USAGE(NULL, &startCpuProperties, &endCpuProperties);

        /* if writing the results to file */
        if (pOutputFile)
        {
            /* write the results to the file */
            fprintf(pOutputFile, "%8d\t%-3.2f\t%-3.2f\t%-3.2f", packetSize,
                dataRate, packetRate, packetTime);
            CPUUsageData::PRINT_CPU_USAGE(pOutputFile, &startCpuProperties,
                &endCpuProperties);
            fflush(pOutputFile);
        }

        /* if not performing receives and this isn't the last loop */
        if ((!(pTestProperties->mode & PERFORM_RECEIVE)) &&
            (packetSize < pTestProperties->packetEnd))
        {
            /* Sleep for 5 seconds to allow all packets to be sent */
            SLEEP(5000);
        }

        /* move to the next packet size */
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
 * Runs the StarSystemPerformanceTester program, using the command line
 * arguments provided.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv An array of command line arguments passed to the program.
 *
 * @return 0 if program completed normally, otherwise 1.
 */
int StarSystemPerformanceTester::Run(int argc, char *argv[])
{
    TestProperties testProperties;
    FILE *pOutputFile = NULL;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    /* read the test parameters */
    if (!ReadParameters(&testProperties, argc, argv))
    {
        goto error_in_main;
    }

    /* if writing the results to file */
    if (strlen(testProperties.pFilePath) > 0)
    {
        /* open the file */
        pOutputFile = fopen(testProperties.pFilePath, "wt");
        if (!pOutputFile)
        {
            puts("Couldn't open the file to store the statistics, exiting");
            goto error_in_main;
        }

        /* write the description to the file */
        fprintf(pOutputFile, "%s\n\n", testProperties.pFileDescription);
    }

    /* test is beginning, so prevent system sleeping */
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

        case UNDEFINED_TEST_TYPE:
            /* error */
            break;

        default:
            break;
    }

    ENABLE_SYSTEM_SLEEP();

    /* if writing the results to file */
    if (pOutputFile)
    {
        /* Close the file */
        fclose(pOutputFile);
    }

error_in_main:

    /* cleanup the test by freeing any memory and closing any open channels */
    CleanupTest(&testProperties);

    puts("");

    /* end program */
    return 0;
}

/**
 * Get the current system time.
 */
clock_t StarSystemPerformanceTester::GET_TIME()
{
#ifdef _WIN32
    return clock();
#else
   struct timeval tv;
   struct timezone tz;
   gettimeofday(&tv, &tz);
   return tv.tv_sec * 1000000 + tv.tv_usec;
#endif
}

