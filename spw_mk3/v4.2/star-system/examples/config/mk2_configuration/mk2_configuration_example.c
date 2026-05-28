/**
 * \file mk2_configuration_example.c
 *
 * \brief Example  usage of the Mk2 configuration API
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example code demonstrating the use of the
 * Mk2 compatible device specific configuration functions.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd.
 */

#include <stdio.h>
#include "star-api.h"
#include "cfg_api_generic.h"

#if defined(__rtems__)
    #include <bsp.h>
    #include <pthread.h>
    #include "cfg_service_api.h"

    int __cdecl main();

    void *POSIX_Init(void *pArgs)
    {
        UNREFERENCED_PARAMETER(pArgs);

        /* Start the Config Service */
        STAR_runConfigService();

        main();

        exit(0);
        return NULL;
    }

    #define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
    #define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

    #define CONFIGURE_OBJECTS_UNLIMITED
    #define CONFIGURE_UNIFIED_WORK_AREAS

    #define CONFIGURE_MAXIMUM_POSIX_THREADS rtems_resource_unlimited(4)
    #define CONFIGURE_MAXIMUM_POSIX_MUTEXES rtems_resource_unlimited(4)
    #define CONFIGURE_MAXIMUM_POSIX_SEMAPHORES rtems_resource_unlimited(4)

    #define CONFIGURE_POSIX_INIT_THREAD_TABLE

    #define CONFIGURE_INIT
    #include <rtems/confdefs.h>

    #include <drvmgr/drvmgr_confdefs.h>
#endif


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
        deviceID = devices[0];
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
    STAR_CFG_FPGA_INFO fpgaInfo;
    char versionStr[STAR_CFG_MK2_VERSION_STR_MAX_LEN];
    char buildDateStr[STAR_CFG_MK2_BUILD_DATE_STR_MAX_LEN];

    /* Select device to configure */
    deviceID = chooseStarDevice();
    if (!deviceID)
    {
        return 0;
    }

    /* Get hardware info*/
    CFG_getFPGAInfo(deviceID, &fpgaInfo);

    CFG_FPGAInfoToString(deviceID, &fpgaInfo, versionStr, buildDateStr);

    /* Display the hardware info*/
    printf("\nVersion: %s", versionStr);
    printf("\nBuildDate: %s", buildDateStr);

    /* Set general purpose register to 0xABCD */
    CFG_setGeneralPurpose(deviceID, 0xABCD);

    /* Flash the devices LEDs*/
    CFG_identify(deviceID);

    /* Set the device to be a time-code master*/
    CFG_enableTimeCodeMaster(deviceID);

    /* Set 2 second delay between time-codes */
    CFG_setTimeCodePeriod(deviceID, 2000000);

    /* Enable interface mode */
    CFG_enableInterfaceMode(deviceID);

    /* Disable interface mode on port 2 only */
    CFG_disableInterfaceModeOnPort(deviceID, 2);

    /* Enable adding the source port number as a leading byte
       to received packets on ports 1 and 3 */

    CFG_enableIdentifySource(deviceID);
    CFG_enableIdentifySourceOnPort(deviceID, 1);
    CFG_enableIdentifySourceOnPort(deviceID, 3);

    /* Set port 2 to have a base transmit rate of 150 Mbit/s */
    CFG_setTransmitSignallingRate(deviceID, 2, 150);

    /* Inject a disconnect error on link 1 */
    CFG_injectError(deviceID, 1, SPW_ERROR_DISCONNECT);

    return 0;
}
