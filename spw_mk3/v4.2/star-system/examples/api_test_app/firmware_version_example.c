/**
 * \file firmware_version_example.c
 *
 * \brief Retrieves list of devices that are available and firmware versions.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Gets a list of available devices using STAR_getDeviceList() and then iterates
 * over them. The firmware version for each device is retrieved and printed.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

void firmwareVersionExample()
{
    /* Initialise device count to 0 */
    U32 deviceCount = 0;

    /* Counter for loop */
    unsigned int index;

    /* Get device list */
    STAR_DEVICE_ID *devices = STAR_getDeviceList(&deviceCount);

    /* For all devices */
    for(index = 0; (devices != NULL) && (index < deviceCount); index++)
    {
        /* Get firmware version */
        STAR_VERSION_INFO * firmwareVersion = STAR_getDeviceFirmwareVersion(
            devices[index]);

        /* Print firmware version label */
        printf("Firmware version:");

        /* Print firmware version */
        printVersionInfo(firmwareVersion);
    }

    /* Destroy device list */
    STAR_destroyDeviceList(devices);
}
