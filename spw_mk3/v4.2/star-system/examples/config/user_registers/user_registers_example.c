/**
 * \file user_registers_example.c
 *
 * \brief Example usage of the user configurable register functions
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example code demonstrating the use of the
 * Router Configuration API to configure the user registers.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd.
 */


#include <stdio.h>
#include "star-api.h"
#include "cfg_api_generic.h"

STAR_DEVICE_ID chooseStarDevice()
{
    STAR_DEVICE_ID * devices;
    unsigned int devCount = 0, chosen, i, status;
    STAR_DEVICE_ID deviceID;
    char *deviceName, s[256];

    /* Get the list of devices present which can be configured */
    devices = STAR_getDeviceListForType(STAR_DEVICE_CONFIG_SUPPORTED,
        &devCount);

    /* If there was an error getting the list of devices */
    if (!devices)
    {
        /* Return an error */
        puts("No SpaceWire devices detected!");
        return 0;
    }

    /* Display the devices detected */
    if (devCount == 1)
    {
        puts("One SpaceWire device detected:");
    }
    else
    {
        printf("%u SpaceWire devices detected:", devCount);
    }

    /* For each device */
    for (i = 0; i < devCount; i++)
    {
        /* Display its name */
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

    /* If there's only 1 device on the list */
    if (devCount == 1)
    {
        /* Return that device */
        deviceID =  devices[0];
        STAR_destroyDeviceList(devices);
        return deviceID;
    }

    /* Ask the user which device to use */
    printf("\nPlease select which device to open: ");
    fgets(s, 256, stdin);
    status = sscanf(s, "%d", &chosen);
    if ((!status) || (chosen > devCount - 1))
    {
        puts("Incorrect device number selected.");
        STAR_destroyDeviceList(devices);
        return 0;
    }

    /* Return the chosen device */
    deviceID =  devices[chosen];
    STAR_destroyDeviceList(devices);
    return deviceID;
}

int __cdecl main()
{
    STAR_DEVICE_ID deviceID;
    REGISTER value;

    /* Select device to configure */
    deviceID = chooseStarDevice();

    /* Set general purpose register to 0xABCD */
    CFG_setGeneralPurpose(deviceID, 0xABCD);

    /* Set device identity register to 0xCAFE */
    CFG_setNetworkIdentity(deviceID, 0xCAFE);

    /* Display user register values*/
    CFG_getGeneralPurpose(deviceID, &value);
    printf("General Purpose register value: %x", value);
    CFG_getNetworkIdentity(deviceID, &value);
    printf("\nNetwork Identity register value: %x", value);

    return 0;
}