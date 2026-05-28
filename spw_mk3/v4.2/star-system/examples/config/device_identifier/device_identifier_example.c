/**
 * \file device_identifier_example.c
 *
 * \brief Example usage of the device identifier functions
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example code demonstrating the use of the Router
 * Configuration API to get the device identifier information for SpaceWire
 * routing devices.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd.
 */

#include <stdio.h>
#include "star-api.h"
#include "cfg_api_generic.h"

STAR_DEVICE_ID chooseStarDevice()
{
    STAR_DEVICE_ID *devices;
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
    STAR_CFG_DEVICE_IDENTIFIER_INFO deviceIdentifierInfo;
    STAR_CFG_NETWORK_DISCOVERY_INFO networkInfo;
    char manufacturerStr[STAR_CFG_MANUFACTURER_STR_MAX_LEN];
    char deviceStr[STAR_CFG_DEVICE_STR_MAX_LEN];
    int i;

    /* Select device to configure */
    deviceID = chooseStarDevice();

    /* Get the router Identification Information */
    CFG_getDeviceIdentificationInfo(deviceID, &deviceIdentifierInfo);

    /* Display the Identification Information*/
    printf("\nManufacturer ID:\t%u", deviceIdentifierInfo.manufacturerID);
    printf("\nChip ID:\t%u", deviceIdentifierInfo.chipType);
    printf("\nVersion:\t%u", deviceIdentifierInfo.versionNum);

    /* Display parsed information*/
    CFG_getDeviceManufacturerAsString(&deviceIdentifierInfo, manufacturerStr);
    printf("\nManufacturer Name:\t%s", manufacturerStr);

    CFG_getDeviceTypeAsString(&deviceIdentifierInfo, deviceStr);
    printf("\nDevice Type:\t%s", deviceStr);

    /* Get Device network information */
    CFG_getNetworkDiscoveryInfo(deviceID, &networkInfo);

    /* Display device network information */
    switch(networkInfo.deviceType)
    {
    case STAR_CFG_DEVICE_TYPE_ROUTER:
          printf("\nDevice Type:\tRouter");
          break;
    case STAR_CFG_DEVICE_TYPE_UNKNOWN:
          printf("\nDevice Type:\tUnknown");
          break;
    case STAR_CFG_DEVICE_TYPE_INVALID:
          printf("\nDevice Type:\tInvalid");
          break;
    }

    printf("\nNumber of Ports (Total/Running):\t%u/%u", networkInfo.portCount, networkInfo.runningPortsCount);
    printf("\nRunning port numbers: ");
    /* Test each bit in mask to see if it is set */
    for(i = 1; i < 32; i++)
    {
        if( (1 << i) & networkInfo.runningPortsMask)
            printf(" %d", i);
    }

    printf("\nReturn Port:\t%u", networkInfo.returnPort);

    return 0;
}


