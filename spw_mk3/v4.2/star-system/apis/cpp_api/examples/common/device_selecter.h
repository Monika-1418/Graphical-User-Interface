/**
 * \file device_selecter.h
 *
 * \brief Defines class used to detect available devices and allow the user to
 *        select one to work with.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Defines class used to detect available devices and allow the user to select
 * one to work with.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "device.hpp"
#include "star_system.hpp"

using namespace stardundee::com::starsystem::general;

#ifndef DEVICE_SELECTOR_H
#define DEVICE_SELECTOR_H

/**
 * Used to detect available devices and to allow the user to select one to
 * work with.
 */
class DeviceSelector
{
public:
    /**
     * Present user with a list of currently available devices to enable them to
     * select a device to query, and then return the device selected.
     *
     * @return A pointer to a Device object representing the device
     *         selected, or NULL if no devices found or an error retrieving the
     *         device information.
     *
     * \note The Device object returned must be freed when no longer required by
     *       calling the C++ delete operator.
     *
     */
    static Device* ChooseDevice()
    {
        char deviceName[Device::STR_DEVICE_NAME_LEN];
        size_t stringLen = 0;
        U32 deviceCount = 0;
        DeviceList deviceList;
        Device *deviceSelected = NULL;
        int status;
        unsigned int chosen;
        char s[256];

        /* get the list of devices present for all drivers */
        deviceCount = deviceList.Populate();

        /* if there are no devices present */
        if (!deviceCount)
        {
            /* report no devices available */
            puts("No SpaceWire devices detected!");

            /* return null / empty device */
            return NULL;
        }

        /* display the number of devices detected */
        deviceCount = deviceList.GetDeviceCount();
        if (deviceCount == 1)
        {
            printf("One SpaceWire device detected:");
        }
        else
        {
            printf("%d SpaceWire devices detected:\n", deviceCount);
        }

        /* for each device */
        for (unsigned i = 0; i < deviceCount; i++)
        {
            /* if device ID is valid */
            if (deviceList[i]->GetDeviceID())
            {
                /* get the device name */
                stringLen = deviceList[i]->GetDeviceName(deviceName);

                /* if device name available */
                if (stringLen)
                {
                    /* display it */
                    printf("\t%d - %s\n", i, deviceName);
                }
                else
                {
                    /* display error msg */
                    printf("\t%d - Unknown SpaceWire Device\n", i);
                }
            }
            else
            {
                /* display - invalid or inaccessible device */
                printf("\t%d - Unable to access device\n", i);
            }
        }

        /* if there's only 1 device on the list */
        if (deviceCount == 1)
        {
            /* make this the selected device to return */

            /* make a clone/copy of the device on the list which is being
             * selected (the device list itself has to be freed later) */
            deviceSelected = deviceList.CopyDeviceAt(0);
        }
        else
        {
            /* ask the user which device to use */
            printf("Please select which device to open: ");

            /* if failed to get device number */
            if (!fgets(s, 256, stdin))
            {
                /* display error message */
                puts("No device number selected.");
            }
            else
            {
                /* attempt to parse selection entered */
                status = sscanf(s, "%u", &chosen);

                /* if failed to read selection entered or invalid selection */
                if ((!status) || (chosen > deviceCount - 1))
                {
                    /* display error message */
                    puts("Incorrect device number selected.");
                }
                else
                {
                    /* set selected device to match the user's chosen selection */

                    /* make a clone/copy of the relevant device on the list (the
                     * device list itself has to be freed later)  */
                    deviceSelected = deviceList.CopyDeviceAt(chosen);
                }
            }
        }

        /* dispose of the device list */
        /* \note this is not required, the DeviceList object destructor will
         * free the contents of the list if they haven't been disposed of
         * already */
        deviceList.Empty();

        /* return the selected device */
        return deviceSelected;
    }
};

#endif /* DEVICE_SELECTOR_H */
