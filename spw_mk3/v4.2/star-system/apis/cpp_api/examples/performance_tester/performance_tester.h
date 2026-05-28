/**
 * \file performance_tester.h
 *
 * \brief Declarations for C++ version of the STAR-System Performance Tester
 *         program.
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

#include "star_system.hpp"
#include "device.hpp"
#include "channel.hpp"
#include "address.hpp"
#include "packet.hpp"
#include "transmit_operation.hpp"
#include "receive_operation.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cerrno>
#include <new>

#if defined(__QNX__)
    #include <time.h>
    #include <stdlib.h>
#endif

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::transferoperations;

#ifndef STAR_SYSTEM_PERFORMANCE_TESTER_H
#define STAR_SYSTEM_PERFORMANCE_TESTER_H

#define TRUE    1
#define FALSE   0


#ifndef UNREFERENCED_PARAMETER
    #define UNREFERENCED_PARAMETER(a) ((void)(a))
#endif

#ifdef _WIN32
    #include <windows.h>
    #include <crtdbg.h>

    /**
     * The number of ticks in a second.
     */
    #define TIME_DIVIDER    CLOCKS_PER_SEC

    /**
     * Sleep for a period of time in milliseconds.
     */
#if !defined(SLEEP)
    #define SLEEP(time)     Sleep(time)
#endif

    /**
     * Prevent the system sleeping (note that the display may still switch off).
     */
    #define DISABLE_SYSTEM_SLEEP()  \
        SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED)

    /**
     * Allow the system to sleep normally after previously disabling sleeping.
     */
    #define ENABLE_SYSTEM_SLEEP()   \
        SetThreadExecutionState(ES_CONTINUOUS)

    /**
     * Store data for CPU usage.
     */
    class CPUProperties
    {
    public:
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

    };

    /**
     * Define helper functions related to CPU usage.
     */
    class CPUUsageData
    {
    public:
        /**
         * Start recording the CPU usage, by storing the current usage.
         *
         * @param pCpuProperties A pointer to a CpuProperties object which will be
         *                       updated to contain the current CPU properties.
         */
        static void STORE_CPU_USAGE(CPUProperties *pCpuProperties);

        /**
         * Print the headers for the CPU usage for a test written to file.
         *
         * @param stream the stream to write the CPU usage figures to
         */
        static void PRINT_CPU_USAGE_HEADERS(FILE *stream);


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
        static void PRINT_CPU_USAGE(FILE *stream,
                                    CPUProperties *pStartCpuProperties,
                                    CPUProperties *pEndCpuProperties);
    };

#else

    #include <sys/time.h>
    #include <sys/times.h>
    #include <unistd.h>

    /**
     * The number of ticks in a second.
     */
    #define TIME_DIVIDER    1000000

    /**
     * Sleep for a period of time in milliseconds.
     */
    #define SLEEP(time)     usleep(time * 1000)

    /**
     * The maximum length to use for file paths.
     */
    #define MAX_PATH        1024

    /**
     * Prevent the system sleeping (note that the display may still switch off).
     * Not implemented.
     */
    #define DISABLE_SYSTEM_SLEEP()

    /**
     * Allow the system to sleep normally after previously disabling sleeping.
     * Not implemented.
     */
    #define ENABLE_SYSTEM_SLEEP()

    /**
     * The structure used to store the CPU usage.
     */
    class CPUProperties
    {
    public:
        /** The process's times */
        struct tms processTimes;

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

    };

    /**
     * Define helper functions related to CPU usage.
     */
    class CPUUsageData
    {
    public:
        /**
         * Start recording the CPU usage, by storing the current usage.
         *
         * @param pCpuProperties A pointer to a CPUProperties object which will be
         *                       updated to contain the current CPU properties.
         */
        static void STORE_CPU_USAGE(CPUProperties *pCpuProperties);


        /**
         * Print the headers for the CPU usage for a test written to file.
         *
         * @param stream the stream to write the CPU usage figures to
         */
        static void PRINT_CPU_USAGE_HEADERS(FILE *stream);


        /**
         * Get the number of CPU cores on this PC.
         *
         * @return the number of CPU cores
         */
    #if defined(__QNX__)
        static int GetCpuCount();
    #else
        static int GetCpuCount();
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
        static void PRINT_CPU_USAGE(FILE *stream,
                                    CPUProperties *pStartCpuProperties,
                                    CPUProperties *pEndCpuProperties);
    };

#endif

/**
 * The mode in which packets are received.  This can be ORed with
 * PERFORM_TRANSMIT.
 */
#define PERFORM_RECEIVE     1

/**
 * The mode in which packets are sent.  This can be ORed with PERFORM_RECEIVE.
 */
#define PERFORM_TRANSMIT    2

/**
 * Macro to determine the minimum of two numbers.
 */
#if !defined(MIN)
    #define MIN(a, b)   (((a) > (b)) ? (b) : (a))
#endif

/**
 * Macro to determine the maximum of two numbers.
 */
#if !defined(MAX)
    #define MAX(a, b)   (((a) < (b)) ? (b) : (a))
#endif

/**
 * The number of transmit and/or receive operations (and buffers) to use at a
 * time.
 */
#define BUFFER_NUM  4

/* The following values ensure that each test runs for at least 10 seconds */
/* (at 200 Mbit/s) */

/**
 * The size of buffer to be used for each transmit and/or receive operation.
 */
#define BUFFER_SIZE 200000

/**
 * The number of transmit and/or receive operations to perform in a test.
 */
#define LOOP_NUM    1000

/**
 * Enum: defines the different types of tests that can be performed.
 */
typedef enum
{
    /* The data and packet rate test type. */
    RATE_TEST,

    /* The random packet size test type. */
    RANDOM_TEST,

    /* The latency test type. */
    LATENCY_TEST,

    /* Undefined test type. */
    UNDEFINED_TEST_TYPE

} TEST_TYPE;

/**
 * Holds the data necessary to perform a test.
 */
class TestProperties
{
public:
    /**
     * Default constructor. Initialise object to default state;
     */
    TestProperties();

    /**
     * Destructor. Free resources created or held by object.
     */
    ~TestProperties();

    /* The mode of the tests: read, write or read and write. */
    char mode;

    /* The type of the tests. */
    TEST_TYPE testType;

    /* The number of tests to be performed concurrently. */
    int testCount;

    /* The starting packet size. */
    int packetStart;

    /* The ending packet size. */
    int packetEnd;

    /* The amount to increment the packet size each time. */
    int packetStep;

    /**
     * The identifiers of the transmit channels to be used to transmit packets
     * for each test. Transmit channels to be used to transmit packets for each 
     * test.
     */
    Channel *pTxChannels;

    /**
     * The address paths to be added to the front of transmitted packets for
     * each test.
     */
    Address **ppAddresses;

    /**
     * The identifiers of the receive channels to be used to receive packets for
     * each test. Receive channels to be used to receive packets for each test.
     */
    Channel *pRxChannels;

    /* The path to a file to be used to store the results of the tests. */
    char pFilePath[MAX_PATH + 1];

    /**
     * The description to be added to the start of the file used to store the
     * results of the tests.
     */
    char pFileDescription[1000];

    /**
     * Clean up / free resources created and/or held by this object.
     */
    void CleanUp();

    /**
     * Free the given list of channels, closing them first if required.
     *
     * @param pChannels A list / array of channels to free.
     * @param channelNum The number of channels in the list.
     * @param closeFirst Whether to close the channel first, prior to freeing.
     */
    void FreeChannels(Channel *pChannels, int channelNum, bool closeFirst);

    /**
     * Free the given list of address paths.
     *
     * @param pAddresses An array of pointers to SpaceWireAddresses to be freed.
     * @param count The number of items in the list.
     *
     */
    void FreeAddressPaths(Address **pAddresses, int count);

    /**
     * Create a required number of transmit channels.
     *
     * @param numberOfChannelsRequired The number of channels required.
     *
     * @return 1 if channels created successfully, otherwise 0.
     */
    int CreateTransmitChannels(int numberOfChannelsRequired);

    /**
     * Create a required number of receive channels.
     *
     * @param numberOfChannelsRequired The number of channels required.
     *
     * @return 1 if channels created successfully, otherwise 0.
     */
    int CreateReceiveChannels(int numberOfChannelsRequired);

    /**
     * Create a required number of pointers to SpaceWireAddresses
     *
     * @param numberOfAddressPathsRequired The number of adddress paths 
     *                                     required.
     *
     * @return 1 if channels created successfully, otherwise 0.
     */
    int CreateAddressPaths(int numberOfAddressPathsRequired);

    /**
     * Add the given channel to the given position in the list of transmit 
     * channels.
     *
     * @param ch The channel to add.
     * @param index The position to add it to in the list.
     *
     * @return 1 if the channel added successfully, otherwise 0.
     *
     */
    int AddTransmitChannel(Channel& ch, int index);

    /**
     * Add the given channel to the given position in the list of receive 
     * channels.
     *
     * @param ch The channel to add.
     * @param index The position to add it to in the list.
     *
     * @return 1 if the channel added successfully, otherwise 0.
     */
    int AddReceiveChannel(Channel& ch, int index);

    /**
     * Add the given pointer to a SpaceWire Address to the given position in 
     * the list of pointers to SpaceWire Addresses.
     *
     * @param pAddress A pointer to the SpaceWire Address to add.
     * @param index The position to add it to in the list.
     *
     * @return 1 if the address added successfully, otherwise 0.
     */
    int AddAddressPath(Address *pAddress, int index);
};

/**
 * Contains a range of functions to do performance testing on various aspects 
 * of the STAR-System API.
 */
class StarSystemPerformanceTester
{
public:
    /**
     * Default constructor. Initialise object to default state.
     */
    StarSystemPerformanceTester();

    /**
     * Destructor. Free any resources created by the object.
     */
    ~StarSystemPerformanceTester();

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
    Device* ChooseDevice(int argCount, char *arguments[], int *pCurrentArgument);

    /**
     * Opens a channel to be used to transmit or receive packets
     *
     * @param channelTypeStr a description of the type of channel to be opened, 
     *                       e.g. "transmit" or "receive"
     * @param testNumber the number of the test to be performed. The first test 
     *                   is numbered 1
     * @param direction the direction in which the channel should be opened
     *
     * @return A Channel object representing the opened channel ready for use.
     *
     * \note In the event of an error or the channel failing to open the Channel
     *       object returned will not represent a valid, opened channel. To
     *       determine if the channel is valid or not, one may call the 'IsValid()'
     *       method on the Channel object which is returned.
     */
    Channel OpenChannel(const char *channelTypeStr, int testNumber,
        Channel::ChannelDirection direction, int argCount,
        char *arguments[], int *pCurrentArgument);

    /**
     * Free all memory associated with the given test properties, including
     * closing any channels it contains.
     *
     * @param pTestProperties the test properties to be cleaned up
     */
    void CleanupTest(TestProperties *pTestProperties);

    /**
     * This function reads a SpaceWire address as a hex string from stdin. After
     * tokenizing and validating the address the function returns a
     * Address, or NULL upon failure.
     *
     * @return A pointer to a new Address, or NULL upon failure.
     */
    Address *ReadSpaceWireAddress(int argCount, char *arguments[],
        int *pCurrentArgument);

    /**
     * Read in the parameters of the tests to be performed from the user.
     *
     * @param pTestProperties A pointer to a TestProperties object which will be
     *                        updated to contain the properties of the test to
     *                        be performed
     *
     * @return whether the parameters were successfully read
     */
    int ReadParameters(TestProperties *pTestProperties, int argCount,
        char *arguments[]);

    /**
     * \ingroup PerformanceTester
     * Perform the rate test for a specific packet size.
     *
     * @param packetSize the size of packet to transmit/receive
     * @param mode the mode of operation (receive and/or transmit)
     * @param testCount the number of tests to perform concurrently
     * @param pTxChannels the channels to transmit on
     * @param pAddressPaths the addresses to add to the start of all packets 
     *                      sent, if packets are to be sent
     * @param pRxChannels the channels to receive on
     * @param pDataRate a pointer to a variable which will be updated to 
     *                  contain the data rate recorded for the test
     * @param pPacketRate a pointer to a variable which will be updated to 
     *                    contain the packet rate recorded for the test
     * @param pPacketTime a pointer to a variable which will be updated to 
     *                    contain the average time to transmit a single packet
     * @param pStartCpuProperties a pointer to a structure which will be 
     *                            updated to contain the CPU properties when 
     *                            the test is started
     * @param pEndCpuProperties a pointer to a structure which will be updated 
     *                          to contain the CPU properties when the test 
     *                          completes
     */
    void PerformRateTestForPacketSize(int packetSize, int mode, int testCount,
        Channel *pTxChannels, Address **pAddressPaths,
        Channel *pRxChannels, double *pDataRate, double *pPacketRate,
        double *pPacketTime, CPUProperties *pStartCpuProperties,
        CPUProperties *pEndCpuProperties);

    /**
     * Perform the rate test.
     *
     * @param pTestProperties A pointer to a TestProperties object containing
     *                        the properties of the test to be performed.
     * @param pOutputFile The file to write the statistics to, or NULL if no
     *                    file is to be used.
     */
    void performRateTest(TestProperties *pTestProperties, FILE *pOutputFile);

    /**
     * Perform the random test for a specific packet size.
     *
     * @param minPacketSize the minimum size of packet to transmit/receive
     * @param maxPacketSize the maximum size of packet to transmit/receive
     * @param mode the mode of operation (receive and/or transmit)
     * @param testCount the number of tests to perform concurrently
     * @param pTxChannels the channels to transmit on
     * @param ppAddresses the addresses to add to the start of all packets 
     *                    sent, if packets are to be sent
     * @param pRxChannels the channels to receive on
     * @param pDataRate a pointer to a variable which will be updated to 
     *                  contain the data rate recorded for the test
     * @param pPacketRate a pointer to a variable which will be updated to 
     *                    contain the packet rate recorded for the test
     * @param pPacketTime a pointer to a variable which will be updated to 
     *                    contain the average time to transmit a single packet
     * @param pPacketSize a pointer to a variable which will be updated to 
     *                    contain the average packet size used in the test
     * @param pStartCpuProperties a pointer to an object which will be updated 
     *                            to contain the CPU properties when the test 
     *                            is started
     * @param pEndCpuProperties a pointer to an object which will be updated to
     *                          contain the CPU properties when the test 
     *                          completes
     */
    void performRandomTestForPacketSize(int minPacketSize, int maxPacketSize,
        int mode, int testCount, Channel *pTxChannels,
        Address **ppAddresses, Channel *pRxChannels,
        double *pDataRate, double *pPacketRate, double *pPacketTime,
        double *pPacketSize, CPUProperties *pStartCpuProperties,
        CPUProperties *pEndCpuProperties);

    /**
     * Perform the random test.
     *
     * @param pTestProperties A pointer to an object containing the properties 
     *                        of the test to be performed
     * @param pOutputFile the file to write the statistics to, or NULL if no 
     *                    file is to be used
     */
    void performRandomTest(TestProperties *pTestProperties, FILE *pOutputFile);

    /**
     * \ingroup PerformanceTester
     * Perform the latency test for a specific packet size.
     *
     * @param packetSize the size of packet to transmit/receive
     * @param mode the mode of operation (receive and/or transmit)
     * @param testCount the number of tests to perform concurrently
     * @param pTxChannels the channels to transmit on
     * @param pAddressPaths the addresses to add to the start of all packets 
     *                      sent, if packets are to be sent
     * @param pRxChannels the channels to receive on
     * @param pDataRate a pointer to a variable which will be updated to 
     *                  contain the data rate recorded for the test
     * @param pPacketRate a pointer to a variable which will be updated to 
     *                    contain the packet rate recorded for the test
     * @param pPacketTime a pointer to a variable which will be updated to 
     *                    contain the average time to transmit a single packet
     * @param pStartCpuProperties a pointer to a structure which will be 
     *                            updated to contain the CPU properties when 
     *                            the test is started
     * @param pEndCpuProperties a pointer to a structure which will be updated 
     *                          to contain the CPU properties when the test 
     *                          completes
     */
    void performLatencyTestForPacketSize(int packetSize, int mode, int testCount,
        Channel *pTxChannels, Address **ppAddressPaths,
        Channel *pRxChannels, double *pDataRate, double *pPacketRate,
        double *pPacketTime, CPUProperties *pStartCpuProperties,
        CPUProperties *pEndCpuProperties);

    /**
     * Perform the latency test.
     *
     * @param pTestProperties a pointer to an object containing the properties 
     *                        of the test to be performed
     * @param pOutputFile the file to write the statistics to, or NULL if no 
     *                    file is to be used
     */
    void performLatencyTest(TestProperties *pTestProperties, FILE *pOutputFile);

    /**
     * Runs the StarSystemPerformanceTester program, using the command line
     * arguments provided.
     *
     * @param argc The number of arguments passed to the program.
     * @param argv An array of command line arguments passed to the program.
     *
     * @return 0 if program completed normally, otherwise 1.
     */
    int Run(int argc, char *argv[]);

    /**
     * Get the current system time.
     */
    clock_t GET_TIME();
};

#endif /* STAR_SYSTEM_PERFORMANCE_TESTER_H */

