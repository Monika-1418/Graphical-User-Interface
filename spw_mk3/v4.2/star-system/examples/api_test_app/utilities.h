/**
 * \file utilities.h
 *
 * \brief Declarations of common utilities used throughout the example programs.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains declarations of common utility functions that are used
 * throughout the example programs.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "star-api.h"
#include "star-dundee_types.h"

#if defined(_WIN32)
    #include <windows.h>

    /* Define Windows sleep function */
    #define sleep(delay)     Sleep(delay)
#elif (defined(__linux__) || defined(__CYGWIN__) || defined(__QNX__))
    #include <unistd.h>

    /* Define Linux / Cygwin / QNX sleep function */
    #define sleep(delay)     usleep(delay * 1000)
#elif defined(__vxworks)
    #include <ticklib.h>
    #include <syslib.h>

    /* Define vxWorks sleep function */
    void sleep(U32 delay)
    {
        /* Calculate number of ticks to delay for */
        double rate = (double)sysClkRateGet();
        U32 ticks = (int)((rate / 1000.0) * delay + 0.5);

        /* Delay for number of ticks */
        taskDelay(ticks);
    }
#endif

/**
 * Reads a series of bytes from the console.
 * @param bytes the previously allocated array of bytes to be populated
 * @param maximumBytesLength the maximum number of bytes
 * @param actualBytesLength the number of bytes that were read from the console
 *
 * @return non-zero if bytes were read successfully
 */
int readBytes(unsigned char * bytes, unsigned int maximumBytesLength,
    U16 * actualBytesLength);

/**
 * Defines different transmit types (e.g. send and receive).
 */
typedef enum
{
    TRANSMIT_TYPE_SEND,
    TRANSMIT_TYPE_RECEIVE
} TRANSMIT_TYPE;

/**
 * Asks user which device they would like to use.
 * @param transmitType whether the device is being used for sending or receiving packets. Used to display correct prompt messages.
 * @return the chosen device id or 0 if no device was chosen
 */
STAR_DEVICE_ID promptForDevice(TRANSMIT_TYPE transmitType);

/**
 * Get first device if there is one.
 * @return the first available device or 0 if no available device
 */
STAR_DEVICE_ID getFirstDevice();

/**
 * Returns true if the specified channel is valid for the device.
 * @param device the device to check channel on
 * @param channel the channel to check for validity
 * @return true or false
 */
int isChannelValid(STAR_DEVICE_ID device, unsigned int channel);

/**
 * Asks user which channel they would like to use.
 * @param device the device to select channel on
 * @param selectedChanel the channel number that was selected
 * @return the chosen channel id
 */
STAR_CHANNEL_ID promptForChannel(STAR_DEVICE_ID device, int *
    selectedChannel, TRANSMIT_TYPE transmitType);

/**
 * Prints the packet contents as hex byte values seperated by spaces.
 * @param packetContents the packet data to be printed
 * @param receiveBufferLength the length of data that was received
 */
void printPacketContents(unsigned char * packetContents,
    unsigned int receiveBufferLength);

/**
 * Prints the specified version info data.
 * @param versionInfo the version info to be printed
 */
void printVersionInfo(STAR_VERSION_INFO * versionInfo);

#ifndef UNREFERENCED_PARAMETER
    /* Avoid unreferenced parameter compiler warnings */
    #define UNREFERENCED_PARAMETER(a)   ((void)(a))
#endif
