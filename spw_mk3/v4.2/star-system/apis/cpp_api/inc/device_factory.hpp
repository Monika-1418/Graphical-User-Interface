/**
 * \file device_factory.hpp
 *
 * \brief Factory, for creating instances of Device objects.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Factory, to create instances of Device objects.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_DEVICE_FACTORY_CPP__
#define __STAR_DEVICE_FACTORY_CPP__

#include "star-dundee_annotations.h"
#include "device.hpp"
#include "virtual_device.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Responsible for creating instances of Device objects.
*/
class DeviceFactory
{
public:
    /**
    * Create and return an instance of a Device object corresponding to
    * the given device ID.
    *
    * @param[in] deviceID The ID of a device to create an instance of a
    *                     Device object for.
    *
    * @return A pointer to a new Device object, or NULL.
    *
    */
    _Check_return_ static Device* CreateDevice(STAR_DEVICE_ID deviceID)
    {
        /* if invalid device ID, e.g. zero */
        if (deviceID == 0)
        {
            return NULL;
        }

        /* create null Device */
        Device *newDevice = NULL;

        /* try to create instance of new Device based on the ID provided */
        try
        {
            /* call STAR API to determine if device corresponding to ID is
             * virtual or not */
            int result = STAR_isDeviceVirtual(deviceID);

            /* if virtual device */
            if (result == 1)
            {
                /* create instance of VirtualDevice */
                newDevice = new VirtualDevice();
                newDevice->SetDeviceID(deviceID);
            }
            /* else, create instance of standard star device */
            else
            {
                /* create instance of standard / local Device */
                newDevice = new Device();
                newDevice->SetDeviceID(deviceID);
            }
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* error allocating memory */
            newDevice = NULL;

        }

        /* return the new Device, may be NULL */
        return newDevice;
    }
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

