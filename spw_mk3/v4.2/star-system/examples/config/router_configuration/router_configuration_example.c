/**
 * \file router_configuration_example.c
 *
 * \brief Example usage of the router configuration functions
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example code demonstrating the use of the
 * Router Configuration API to configure the router settings.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd.
 */

#include <stdio.h>
#include "star-api.h"
#include "cfg_api_generic.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

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
    deviceID = devices[chosen];
    STAR_destroyDeviceList(devices);
    return deviceID;
}

int __cdecl main()
{
    STAR_DEVICE_ID deviceID;
    U32 portMask;
    U8  tcFlagMode;
    U8  tcValue;
    STAR_CFG_ROUTER_GLOBAL_STATE globalState;
    unsigned int i;

    /* Select device to configure */
    deviceID = chooseStarDevice();

    /* Get Time-code output ports */
    CFG_getTimeCodeDistributionPorts(deviceID, &portMask);

    printf("\nTime-codes are bieng forwarded on ports: ");

    /* Test each bit in mask to see if it is set */
    for(i = 1; i < 32; i++)
    {
        if( (1 << i) & portMask)
            printf(" %u", i);
    }

    /* Forward time codes on ports 1, 2 and 3 only */
    CFG_setTimeCodeDistributionPorts(deviceID, 0xE);

    /* Get time-code flags mode */
    CFG_getTimeCodeFlagMode(deviceID, &tcFlagMode);

    printf("\nTime-code flag interpretation mode: %s", tcFlagMode ?
                                                        "Flags ignored" :
                                                        "Time-code discarded on flag not \"00\"");

    /* Get current time-code value */
    CFG_getTimeCodeValue(deviceID, &tcValue);
    printf("\n Current time-code value: %u", tcValue);


    /* Get Router global settings */
    CFG_getRouterGlobalSettings(deviceID, &globalState);

    printf("\n\nDisable on silence: %s", globalState.disableOnSilence ? "Enabled" : "Disabled");
    printf("\nSelf addressing: %s", globalState.enableSelfAddressing ? "Enabled" : "Disabled");
    printf("\nStart on request: %s", globalState.startOnRequest ? "Enabled" : "Disabled");

    switch(globalState.timeoutMode)
    {
    case STAR_CFG_TIMEOUT_MODE_WATCHDOG:
        printf("\nWatchdog mode enabled.");
        break;
    case STAR_CFG_TIMEOUT_MODE_BLOCKING:
        printf("\nWatchdog mode disabled.");
        break;
    }

    printf("\nTimeout period: ");
    switch(globalState.timeoutPeriod)
    {
    case STAR_CFG_PORT_TIMEOUT_100US:
        printf("60-100us");
        break;
    case STAR_CFG_PORT_TIMEOUT_1MS:
        printf("1.3ms");
        break;
    case STAR_CFG_PORT_TIMEOUT_10MS:
        printf("10ms");
        break;
    case STAR_CFG_PORT_TIMEOUT_100MS:
        printf("82ms");
        break;
    case STAR_CFG_PORT_TIMEOUT_1S:
        printf("1.3s");
        break;
    }

    /* Set Global settings */
    globalState.disableOnSilence = FALSE;
    globalState.enableSelfAddressing = TRUE;
    globalState.startOnRequest = TRUE;
    globalState.timeoutMode = STAR_CFG_TIMEOUT_MODE_WATCHDOG;
    globalState.timeoutPeriod = STAR_CFG_PORT_TIMEOUT_10MS;

    CFG_setRouterGlobalSettings(deviceID, &globalState);

    return 0;
}


