/**
 * \file routing_table_example.c
 *
 * \brief Example  usage of the routing table functions
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example code demonstrating the use of the
 * Router Configuration API to configure Group Adaptive Routing.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd.
 */

#include <stdio.h>
#include "star-api.h"
#include "cfg_api_generic.h"

#define BIT1  0x02
#define BIT3  0x08

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
    STAR_CFG_GAR_ENTRY entry;
    U8 logicalAddress = 106;
    unsigned int i;

    /* Select device to configure */
    deviceID = chooseStarDevice();

    /* Get a routing table entry */
    CFG_getRoutingTableEntry(deviceID, logicalAddress, &entry);

    /* Display routing table entry details */

    printf("\nRouting table entry for address %u.\n", logicalAddress);

    printf("\n\tAddress is %s.", entry.invalidAddress ? "invalid":"valid");
    printf("\n\tHeader deletion %s.", entry.deleteHeader ? "enabled":"disabled");
    printf("\n\tPriority: %s.", entry.priority ? "high":"low");

    printf("\n\tOutput ports: ");

    /* Test each bit in mask to see if it is set */
    for(i = 1; i < 32; i++)
    {
        if( (1 << i) & entry.portMask)
            printf(" %u", i);
    }

    /* Set a routing table entry:
      Logical address 44, Header deletion enabled, High priority
      Output ports: 1,3    */
    entry.deleteHeader = 1;
    entry.invalidAddress = 0;
    entry.priority = 1;
    entry.portMask = BIT1 | BIT3;

    CFG_setRoutingTableEntry(deviceID, 44, &entry);

    return 0;
}