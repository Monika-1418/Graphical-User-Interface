/**
 * \file ui.c
 *
 * \brief User Interface functions used by example applications.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the definitions of the UI functions that
 * are common across the example applications.
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "star-api.h"

STAR_DEVICE_ID STAR_UI_chooseDevice(STAR_DEVICE_TYPE aDeviceTypes[], U32 NumRequestedTypes)
{
    STAR_DEVICE_ID* devices;
    U32 deviceCount = 0, i;
    unsigned int chosen;
    int status;

    STAR_DEVICE_ID deviceId;
    char *deviceName, s[256];

    devices = STAR_getDeviceListForTypes(aDeviceTypes, NumRequestedTypes, &deviceCount);

    /* If there are no devices present */
    if (!devices)
    {
        /* Return an error */
        puts("No SpaceWire devices detected!");
        return 0;
    }

    /* Display the devices detected */
    if (deviceCount == 1)
    {
        printf("One device detected:");
    }
    else
    {
        printf("%u devices detected:\n", deviceCount);
    }

    /* For each device */
    for (i = 0; i < deviceCount; i++)
    {
        /* Display its name */
        if (devices[i])
        {
            deviceName = STAR_getDeviceName(devices[i]);
            if (deviceName)
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
    if (deviceCount == 1)
    {
        /* Return that device */
        deviceId = devices[0];
        STAR_destroyDeviceList(devices);
        return deviceId;
    }

    /* Ask the user which device to use */
    printf("Please select which device to use: ");
    if (!fgets(s, 256, stdin))
    {
        puts("No device number selected.");
        STAR_destroyDeviceList(devices);
        return 0;
    }
    status = sscanf(s, "%u", &chosen);
    if ((!status) || (chosen > deviceCount - 1))
    {
        puts("Incorrect device number selected.");
        STAR_destroyDeviceList(devices);
        return 0;
    }

    /* Return the chosen device */
    deviceId = devices[chosen];
    STAR_destroyDeviceList(devices);
    return deviceId;
}

unsigned char STAR_UI_chooseChannel(STAR_DEVICE_ID deviceId)
{
    STAR_CHANNEL_MASK channelMask;
    unsigned int channelNumber;
    unsigned char channelCount;
    int status;
    unsigned char i;
    char s[256];

    /* Get the channels present on the device */
    channelMask = STAR_getDeviceChannels(deviceId);
    if (channelMask == 0 || channelMask == 1)
    {
        printf("ERROR: The device doesn't appear to have any valid channels.");
        return 0;
    }

    /* Determine the number of channels on the device, */
    /* ignoring the configuration channel */
    channelCount = 0;
    channelNumber = 0;
    for (i = 1; i < 32; i++)
    {
        if ((channelMask >> i) & 1)
        {
            channelCount++;
            channelNumber = i;
        }
    }

    /* If there's only one channel on the device, use this channel */
    if (channelCount == 1)
    {
        printf("Using channel %u as the channel, as this is the only channel on the device.",
            channelNumber);
        return (unsigned char)channelNumber;
    }

    /* Display the available channels */
    printf("Enter channel (");
    for (i = 1; i < 32; i++)
    {
        /* If the channel exists */
        if ((channelMask >> i) & 1)
        {
            printf("%d", i);
            channelCount--;
            if (channelCount == 1)
            {
                printf(" or ");
            }
            else if (channelCount > 1)
            {
                printf(", ");
            }
            else
            {
                break;
            }
        }
    }
    printf("): ");

    /* Read in the channel to use */
    if (!fgets(s, 256, stdin))
    {
        puts("\nERROR: No channel specified");
        return 0;
    }
    status = sscanf(s, "%u", &channelNumber);
    if ((!status) || (!((1 << channelNumber) & channelMask)))
    {
        puts("\nERROR: The channel specified is not present");
        return 0;
    }

    return (unsigned char)channelNumber;
}

int STAR_UI_chooseDeviceAndChannel(STAR_DEVICE_TYPE aDeviceTypes[], U32 NumRequestedTypes,
    STAR_DEVICE_ID *pDeviceId,
    STAR_CHANNEL_ID *pChannelId,
    STAR_CHANNEL_DIRECTION direction)
{
    unsigned char channelNumber;

    *pDeviceId = STAR_UI_chooseDevice(aDeviceTypes,NumRequestedTypes);
    if (!*pDeviceId)
    {
        return 0;
    }

    channelNumber = STAR_UI_chooseChannel(*pDeviceId);
    if (!channelNumber)
    {
        return 0;
    }

    /* Open the channel */
    *pChannelId = STAR_openChannelToLocalDevice(*pDeviceId, direction,
        channelNumber, TRUE);
    if (!(*pChannelId))
    {
        printf("\nERROR: Unable to open channel\n");
        return 0;
    }

    puts("");

    return 1;
}

STAR_SPACEWIRE_ADDRESS *STAR_UI_getAddress()
{
    char s[256];
    STAR_SPACEWIRE_ADDRESS *pAddress;
    unsigned char newPath[256];
    U16 pathLen = 0;
    char *pos = (char *)s;

    /* Ask the user to enter the path to add to the front of packets */
    puts("Enter SpaceWire Address:");
    puts("(Values should be in hex, separated by a space, i.e.: 01 0f 02)");

    /* Read in the path */
    if (!fgets(s, 256, stdin))
    {
        puts("No address entered");
        return NULL;
    }

    /* Read until end of buffer or line feed */
    while ((*pos) && (pathLen < 256) && (*pos != '\n'))
    {
        unsigned long value;

        errno = 0;
        value = strtoul(pos, &pos, 16);
        if (errno)
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
