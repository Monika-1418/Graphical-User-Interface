/**
 * \file virtual_device.hpp
 *
 * \brief Represents a virtual device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a virtual device, i.e. a device that performs the same function
 * as a locally connected physical device but does so using virtual resources.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __VIRTUAL_DEVICE_CPP__
#define __VIRTUAL_DEVICE_CPP__

#include "star-dundee_annotations.h"
#include "device.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee
{
namespace com
{
namespace starsystem
{
/**
 * Classes which provide information about the STAR-System API
 * itself, including any Devices and Drivers present.
 **/
namespace general {

/**
* Represents a virtual device, i.e. a device that performs the same function
* as a locally connected physical device but does so using virtual resources.
*/
class VirtualDevice : public Device
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    VirtualDevice()
    {
        /* no members to initialise at present */
    }

    /**
    * Destructor. Free any resources created by object.
    */
    virtual ~VirtualDevice()
    {
        /* no resources to free at present */
    }

    /**
     * Create and return an instance of a VirtualDevice object which
     * is a copy of 'this' object.
     *
     * @return A pointer to a new VirtualDevice object, or NULL if failed to
     *         allocate memory for the object.
     *
     * \note The object returned must be freed when no longer required by
     *       calling the C++ delete operator.
     *
     */
    STARDevice *CopyDevice()
    {
        /* a pointer to the clone device, initially null */
        VirtualDevice *pCloneDevice = NULL;

        /* create an instance of new a VirtualDevice */
        try
        {
            pCloneDevice = new VirtualDevice();
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* error allocating memory */
            pCloneDevice = NULL;
        }

        /* if failed to create clone device */
        if (!pCloneDevice)
        {
            /* return NULL */
            return NULL;
        }

        /* copy all of the values of 'this' device into the clone device */
        pCloneDevice->SetDeviceID(this->deviceID);

        /* return the clone device */
        return pCloneDevice;
    }
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

