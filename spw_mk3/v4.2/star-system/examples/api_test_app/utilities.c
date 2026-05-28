/**
 * \file utilities.c
 *
 * \brief Common utilities used throughout the example programs.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains common utility functions that are used throughout the
 * example programs.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "utilities.h"

#include "star-api.h"
#include "star-dundee_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int readBytes(unsigned char * bytes, unsigned int maximumBytesLength,
    U16 * actualBytesLength)
{
    U16 bytesRead = 0U;
    unsigned long value;
    char currentChar;
    char * pos;

    /* Allocate string for input */
    char * input = (char *)calloc(maximumBytesLength, sizeof(char));

    /* Check that input was allocated successfully */
    if (input == NULL)
    {
        return 0;
    }

    /* Flush the input stream */
    while ((currentChar = getchar()) != '\n' && currentChar != EOF);

    /* Read in the path */
    if (fgets(input, maximumBytesLength, stdin) == NULL)
    {
        puts("No bytes entered.");
        free(input);
        return 0;
    }

    /* Get start of input */
    pos = (char *)input;

    /* Detect no bytes */
    if (*pos == '\n')
    {
        puts("No bytes entered.");
        free(input);
        return 0;
    }

    /* Read until end of buffer or line feed */
    while ((*pos !='\0') && (bytesRead < maximumBytesLength) && (*pos != '\n'))
    {
        errno = 0;
        value = strtoul(pos, &pos, 16);
        if (errno != 0)
        {
            puts("Invalid value entered in bytes.");
            free(input);
            return 0;
        }

        bytes[bytesRead] = (unsigned char)value;
        bytesRead++;
    }

    /* Free the input string */
    free(input);

    /* Update actual number of bytes that were read */
    *actualBytesLength = bytesRead;

    return 1;
}

STAR_DEVICE_ID promptForDevice(TRANSMIT_TYPE transmitType)
{
    /* The number of devices that were found */
    U32 deviceCount = 0;

    /* Get the list of available SpaceWire devices */
    STAR_DEVICE_ID * devices = STAR_getDeviceList(&deviceCount);

    /* If at least one device is present */
    if(devices)
    {
        /* Initialise selected device index to -1 */
        int selectedDeviceIndex = -1;

        /* Define selected device */
        STAR_DEVICE_ID selectedDevice;

        /* Counter to itterate over device array */
        int index;

        /* Initialise transmit type string to null by default */
        char * transmitTypeString = NULL;

        /* Print header statement */
        printf("The following SpaceWire devices were detected:\n");

        /* For all devices found on system */
        for(index = 0; index < (int)deviceCount; index++)
        {
            /* Get current device */
            STAR_DEVICE_ID device = devices[index];

            /* Get current device name */
            char * deviceName = STAR_getDeviceName(device);

            /* Get current device serial number */
            char * deviceSerial = STAR_getDeviceSerialNumber(device);

            if ((deviceSerial != NULL) && (deviceName != NULL))
            {
                /* Print current device */
                printf("%d: %s (Serial Number: %s)\n", index + 1, deviceName,
                    deviceSerial);
            }

            if (deviceName != NULL)
            {
                /* Destroy device name string */
                STAR_destroyString(deviceName);
            }

            if (deviceSerial)
            {
                /* Destroy device serial string */
                STAR_destroyString(deviceSerial);
            }
        }

        /* Switch on transmit type */
        switch(transmitType)
        {
        /* Case send */
        case TRANSMIT_TYPE_SEND:
            /* Set transmit type string to "send out of" */
            transmitTypeString = "send out of";

            break;
        /* Case receive */
        case TRANSMIT_TYPE_RECEIVE:
            /* Set transmit type string to "receive into" */
            transmitTypeString = "receive into";

            break;
        }

        /* If there is a transmit type string */
        if(transmitTypeString)
        {
            /* Ask which device to use with transmit type string */
            printf("\nPlease enter the number of the device that you would like"
                " to %s:\n", transmitTypeString);
        }
        else
        {
            /* Ask which device to use */
            printf("\nPlease enter the number of the device that you would like"
                " use:\n");
        }

        /* Get user input */
        (void)scanf("%d", &selectedDeviceIndex);

        /* While invalid user input */
        while(selectedDeviceIndex < 1 || selectedDeviceIndex > (int)deviceCount)
        {
            /* Flush input buffer */
            fflush(stdin);

            /* Ask again */
            printf("Invalid device number. Please enter a number between 1 and"
                " %u.\n", deviceCount);

            /* Get user input */
            (void)scanf("%d", &selectedDeviceIndex);
        }

        /* Decrement selected device index */
        selectedDeviceIndex--;

        /* Get selected device */
        selectedDevice = devices[selectedDeviceIndex];

        /* Destroy device list */
        STAR_destroyDeviceList(devices);

        /* Return selected device */
        return selectedDevice;
    }

    /* Return 0 if no selected device */
    return 0;
}

STAR_DEVICE_ID getFirstDevice()
{
    /* The number of devices that were found */
    unsigned int deviceCount = 0;

    /* Get the list of available SpaceWire devices */
    STAR_DEVICE_ID * devices = STAR_getDeviceList(&deviceCount);

    /* If at least one device is present */
    if(devices)
    {
        /* Get first device */
        STAR_DEVICE_ID firstDevice = devices[0];

        /* Destroy device list */
        STAR_destroyDriverList(devices);

        /* Return first device */
        return firstDevice;
    }

    /* Return 0 if no devices exist */
    return 0;
}

int isChannelValid(STAR_DEVICE_ID device, unsigned int channel)
{
    /* Get available channels */
    U32 channelMask = STAR_getDeviceChannels(device);

    /* Define counter for loop */
    unsigned int index;

    /* For all possible channels (apart from port 0) */
    for (index = 1; index < 32; index++)
    {
        /* If the channel exists */
        if ((channelMask >> index) & 1)
        {
            /* If this is the channel that we are looking for */
            if(index == channel)
            {
                /* Return true */
                return 1;
            }
        }
    }

    /* Return false if channel is invalid */
    return 0;
}

STAR_CHANNEL_ID promptForChannel(STAR_DEVICE_ID device, int *
    selectedChannelNumber, TRANSMIT_TYPE transmitType)
{
    /* Define selected channel id */
    STAR_CHANNEL_ID selectedChannel;

    /* Initialise transmit type string to null by default */
    char * transmitTypeString = NULL;

    /* Initialise channel direction to inout by default */
    STAR_CHANNEL_DIRECTION channelDirection = STAR_CHANNEL_DIRECTION_INOUT;

    /* Initialise selected channel number to -1 */
    * selectedChannelNumber = -1;

    /* Switch on transmit type */
    switch(transmitType)
    {
    /* Case send */
    case TRANSMIT_TYPE_SEND:
        /* Set transmit type string to "send out of" */
        transmitTypeString = "send out of";

        /* Set channel direction to out */
        channelDirection = STAR_CHANNEL_DIRECTION_OUT;

        break;
    /* Case receive */
    case TRANSMIT_TYPE_RECEIVE:
        /* Set transmit type string to "receive into" */
        transmitTypeString = "receive into";

        /* Set channel direction to in */
        channelDirection = STAR_CHANNEL_DIRECTION_IN;

        break;
    }

    /* If there is a transmit type string */
    if(transmitTypeString)
    {
        /* Ask user for channel number to use with transmit type string */
        printf("\nPlease enter a channel number between 1 and 31 to %s:\n",
            transmitTypeString);
    }
    else
    {
        /* Ask user for channel number to use */
        printf("\nPlease enter a channel number between 1 and 31 to use:\n");
    }

    /* Get user input */
    (void)scanf("%d", selectedChannelNumber);

    /* While invalid user input */
    while(!isChannelValid(device, * selectedChannelNumber))
    {
        /* Flush input buffer */
        fflush(stdin);

        /* Ask again */
        printf("Invalid channel number. Please enter a valid channel number "
            "between 1 and 31:\n");

        /* Get user input */
        (void)scanf("%d", selectedChannelNumber);
    }

    /* Open channel to device */
    selectedChannel = STAR_openChannelToLocalDevice(device, channelDirection,
        (unsigned char)* selectedChannelNumber, 1);

    /* Return selected channel */
    return selectedChannel;
}

/**
 * Prints the packet contents as hex byte values seperated by spaces.
 * @param packetContents the packet data to be printed
 * @param receiveBufferLength the length of data that was received
 */
void printPacketContents(unsigned char * packetContents,
    unsigned int receiveBufferLength)
{
    /* Counter for loop */
    unsigned int index;

    /* Print packet contents label */
    printf("Received packet of length %u bytes, contents:", receiveBufferLength);

    /* For all characters in receive buffer */
    for(index = 0; index < receiveBufferLength; index++)
    {
        /* Print current character with leading zero */
        printf(" %02x", packetContents[index]);
    }

    /* Print full stop */
    printf(".\n");
}

void printVersionInfo(STAR_VERSION_INFO * versionInfo)
{
    /* Get module name */
    char * moduleName = versionInfo->name;

    /* Get module author */
    char * moduleAuthor = versionInfo->author;

    /* Get major version number */
    U16 major = versionInfo->major;

    /* Get minor version number */
    U16 minor = versionInfo->minor;

    /* Get edit version number */
    U16 edit = versionInfo->edit;

    /* Get patch version number */
    U16 patch = versionInfo->patch;

    /* Print version information string */
    printf("%s %u.%u.%u.%u\n", moduleName, major, minor, edit, patch);

    /* If module has a name */
    if(strlen(moduleName) > 0)
    {
        /* Print module name */
        printf("   Module name: %s\n", moduleName);
    }

    /* If module has an author */
    if(strlen(moduleAuthor) > 0)
    {
        /* Print module author */
        printf(" Module author: %s\n", moduleAuthor);
    }

    /* Print module author */
    printf("Module version: v%u.%u", major, minor);

    /* If there is an edit version number */
    if (edit)
    {
        /* Print edit version number */
        printf("(%u)", edit);
    }

    /* If there is a patch version number */
    if (patch)
    {
        /* Print patch version number */
        printf("p%u", patch);
    }

    /* Take new line */
    printf("\n");
}
