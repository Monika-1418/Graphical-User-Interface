/**
 * \file driver_list_example.c
 *
 * \brief Retrieves a list of drivers that are available and prints each one.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Gets a list of available drivers using STAR_getDriverList() then iterates
 * over them and prints the type of driver.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "utilities.h"

#include "star-api.h"

void printDriverType(STAR_DRIVER_TYPE driverType)
{
    /* Switch on driver type */
    switch(driverType)
    {
        /* Case Invalid */
        case STAR_DRIVER_INVALID:
            /* Print invalid driver */
            printf("Invalid driver detected.\n");

            break;
        /* Case USB */
        case STAR_DRIVER_TYPE_USB:
            /* Print USB driver type */
            printf("USB driver detected.\n");

            break;
        /* Case PCI */
        case STAR_DRIVER_TYPE_PCI:
            /* Print PCI driver type */
            printf("PCI driver detected.\n");

            break;
        /* Case TCPIP */
        case STAR_DRIVER_TYPE_TCPIP:
            /* Print TCP/IP driver type */
            printf("TCP/IP driver detected.\n");

            break;
        /* Case Virtual */
        case STAR_DRIVER_TYPE_VIRTUAL:
            /* Print virtual driver type */
            printf("Virtual driver detected.\n");

            break;
    }
}

void driverListExample()
{
    /* The number of drivers that were found */
    U32 driverCount = 0;

    /* The current driver index */
    unsigned int index;

    /* Get list of drivers */
    STAR_DRIVER_ID * drivers = STAR_getDriverList(1, 1, &driverCount);

    /* For all drivers */
    for(index = 0; (drivers != NULL) && (index < driverCount); index++)
    {
        /* Get driver type */
        STAR_DRIVER_TYPE driverType = STAR_getDriverType(drivers[index]);

        /* Print driver type */
        printDriverType(driverType);
    }

    /* Destroy device list */
    STAR_destroyDeviceList(drivers);
}
