/**
 * \file port_control_status_example.c
 *
 * \brief Example usage of the port control and status functions
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example code demonstrating the use of the
 * Router Configuration API to configure the ports of a routing device.
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
    PORT_STATUS_CONTROL registerValue;
    U8 portNum = 0; /* Configuration port */

    STAR_CFG_CONFIG_PORT_ERRORS configPortErrors;
    STAR_CFG_SPW_LINK_STATUS spwPortStatus;

    /* Select device to configure */
    deviceID = chooseStarDevice();

    /* Get port information */
    CFG_getPortStatusControl(deviceID, portNum, &registerValue);

    /* Display port type */
    switch(CFG_getPortType(registerValue))
    {
    case STAR_CFG_PORT_TYPE_CONFIGURATION:
        printf("\nPort type of port %u: Configuration Port", portNum);
        break;
    case STAR_CFG_PORT_TYPE_LINK:
        printf("\nPort type of port %u: SpaceWire Link Port", portNum);
        break;
    case STAR_CFG_PORT_TYPE_EXTERNAL:
        printf("\nPort type of port %u: External Port", portNum);
        break;
    case STAR_CFG_PORT_TYPE_INVALID:
        printf("\nPort type of port %u: Invalid port value", portNum);
        break;
    }

    /* Display current internal connection */
    printf("\nPort %d is currently connected to port %d ",
            portNum, CFG_getPortConnection(registerValue));

    /* Get configuration port errors (if portNum == 0) */
    CFG_getConfigPortErrors(registerValue, &configPortErrors);
    if(configPortErrors.errorCount)
    {
         printf("\n%d error(s) present on configuration port ", configPortErrors.errorCount);
    }

     /* Clear port errors */
    CFG_clearPortErrors(deviceID, portNum);

    /* Get SpaceWire port Status */
    portNum = 1; /* Assume for the purpose of this example that port One
                    for this device is a SpaceWire port */

    CFG_getPortStatusControl(deviceID, portNum, &registerValue);

    /* Obtain the links status values */
    CFG_getSpaceWireLinkStatus(registerValue, &spwPortStatus);

    /* Display some details about the links status
       Note that the links state machine state can be obtained from the linkState member*/
    printf("\n\nLink %u %s running.", portNum, spwPortStatus.running ? "is":"is not");
    printf("\nLink %u %s set to autostart.", portNum, spwPortStatus.autoStart ? "is":"is not");
    printf("\nLink %u %s set to start.", portNum, spwPortStatus.start ? "is":"is not");
    printf("\nLink %u %s disabled.", portNum, spwPortStatus.disable? "is":"is not");
    printf("\nLink %u %s in tri-state mode.", portNum, spwPortStatus.triState ? "is":"is not");


    /* Disable the link (Set the disable bit, clear the start bit)*/
    spwPortStatus.disable = 1;
    spwPortStatus.start   = 0;
    CFG_setSpaceWireLinkStatus(deviceID, portNum, &spwPortStatus);

    /* Start and stop the link using the shortcut functions.*/
    CFG_startLink(deviceID, portNum);
    CFG_stopLink(deviceID, portNum);

    return 0;
}