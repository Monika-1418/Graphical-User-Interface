/**
 * \file star_device.hpp
 *
 * \brief Parent class for devices.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Parent class for devices.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __DEVICE_BASE_CPP__
#define __DEVICE_BASE_CPP__

#include "star-api.h"
#include "device_configuration.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Parent class for Devices.
*/
class STARDevice
{
protected:
    /** the ID of the device */
    STAR_DEVICE_ID deviceID;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    STARDevice() : deviceID(0)
    {
    }

    /**
    * Destructor. Free any resources created by object.
    */
    virtual ~STARDevice()
    {
        /* no resources to free at present */
    }

    /**
    * Get the device's ID.
    *
    * @return The ID of the device.
    *
    */
    STAR_DEVICE_ID GetDeviceID()
    {
        /* return the ID */
        return deviceID;
    }

    /**
    * Set the device's ID.
    *
    * @param id The ID for the device.
    */
    void SetDeviceID(STAR_DEVICE_ID id)
    {
        deviceID = id;
    }

    /**
    * Gets the configuration state for the device.
    *
    * The device's configuration state provides access to information regarding
    * the ports on the device and their status, the routing table entries for
    * the device, the user configurable registers on the device, configuring the
    * internal state of the device, e.g. timeout periods, and obtaining time
    * code values, and so on.
    *
    * @return A pointer to a new \link stardundee::com::starsystem::deviceconfig::DeviceConfiguration DeviceConfiguration \endlink
    *         object which contains the configuration state for the device, or
    *         NULL if failed to allocate memory for the object or failed to
    *         determine its type, hence the corresponding \link stardundee::com::starsystem::deviceconfig::DeviceConfiguration DeviceConfiguration \endlink
    *         required.
    *
    * \note A hierarchy of \link stardundee::com::starsystem::deviceconfig::DeviceConfiguration DeviceConfiguration \endlink
    *       objects exists to represent the different forms of devices which
    *       can be configured, e.g. PCI Mk2, Brick Mk2, etc. Whereas the parent
    *       class (\link stardundee::com::starsystem::deviceconfig::DeviceConfiguration DeviceConfiguration \endlink)
    *       will provide access to the general properties of any device, the
    *       child classes extend these to include additional properties which
    *       are specific to the form of device that they represent. The
    *       \link stardundee::com::starsystem::deviceconfig::DeviceConfiguration DeviceConfiguration \endlink
    *       object returned can be cast to a more specific form to access these
    *       more specific properties where required.
    *
    * \note The \link stardundee::com::starsystem::deviceconfig::DeviceConfiguration DeviceConfiguration \endlink
    *       object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    virtual stardundee::com::starsystem::deviceconfig::DeviceConfiguration
                                                *GetDeviceConfiguration() = 0;

    /**
    * Create and return an instance of a new Device object which is a copy of
    * this device.
    *
    * @return A instance of a new Device object, or NULL if a memory allocation
    *         failure occurs.
    *
    * \note This is a pure virtual function. Derived classes are expected to
    *       provide the implementation.
    */
    virtual STARDevice *CopyDevice() = 0;
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

