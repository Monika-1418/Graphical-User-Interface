/**
 * \file device_identifier_info.hpp
 *
 * \brief Holds device identifier information for a device, e.g. manufacturer ID,
 *        chip type and version.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds device identifier information for a device, e.g. manufacturer ID, chip
 * type and version.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_IDENTIFIER_INFO_CPP__
#define __DEVICE_IDENTIFIER_INFO_CPP__

#include "star-dundee_annotations.h"
#include "cfg_api_router_types.h"
#include "cfg_api_router.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Holds device identifier information for a device, e.g. manufacturer ID, chip
* type and version
*/
class DeviceIdentifierInfo
{
public:
    /** String constants, e.g. maximum expected length of strings provided as
      * parameters into functions. */
    enum StringConstants
    {
        /** maxmimum expected length of device type string */
        STR_DEVICE_TYPE_LEN = STAR_CFG_DEVICE_STR_MAX_LEN,
        /** maxmimum expected length of device manufacturer string */
        STR_DEVICE_MANUFACTURER_LEN = STAR_CFG_MANUFACTURER_STR_MAX_LEN
    };

private:
    /** struct required by STAR-API to hold device identifier info */
    STAR_CFG_DEVICE_IDENTIFIER_INFO deviceInfo;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    DeviceIdentifierInfo()
    {
        deviceInfo.chipType = 0;
        deviceInfo.manufacturerID = 0;
        deviceInfo.versionNum = 0;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    *
    * @param dInfo A STAR_CFG_DEVICE_IDENTIFIER_INFO struct which contains
    *              the device identifier values to assign to this object.
    */
    explicit DeviceIdentifierInfo(STAR_CFG_DEVICE_IDENTIFIER_INFO dInfo)
    {
        deviceInfo.chipType = dInfo.chipType;
        deviceInfo.manufacturerID = dInfo.manufacturerID;
        deviceInfo.versionNum = dInfo.versionNum;
    }

    /**
    * Copy constructor - to allow copies of DeviceIdentifierInfo objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    DeviceIdentifierInfo(const DeviceIdentifierInfo& deviceInfoToCopyFrom)
    {
        /* copy contents of object being copied into this object */
        deviceInfo.chipType = deviceInfoToCopyFrom.deviceInfo.chipType;
        deviceInfo.manufacturerID =
                            deviceInfoToCopyFrom.deviceInfo.manufacturerID;
        deviceInfo.versionNum = deviceInfoToCopyFrom.deviceInfo.versionNum;
    }

    /**
    * Overloaded assignment operator - to allow instances of DeviceIdentifierInfo
    * objects to have their contents assigned to each other.
    *
    */
    DeviceIdentifierInfo& operator=(const DeviceIdentifierInfo&
                                          deviceInfoBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &deviceInfoBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        deviceInfo.chipType = deviceInfoBeingAssigned.deviceInfo.chipType;
        deviceInfo.manufacturerID =
                            deviceInfoBeingAssigned.deviceInfo.manufacturerID;
        deviceInfo.versionNum = deviceInfoBeingAssigned.deviceInfo.versionNum;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~DeviceIdentifierInfo()
    {
        /* no resources to free at present */
    }

    /**
    * Get the chip type for the device, i.e. the identity code for the
    * SpaceWire chip from the particular manufacturer.
    *
    * @return The chip type.
    *
    */
    U8 GetChipType()
    {
        return deviceInfo.chipType;
    }

    /**
    * Get the manufacturer ID for the device.
    *
    * @return The manufacturer ID.
    *
    */
    U16 GetManufacturerID()
    {
        return deviceInfo.manufacturerID;
    }

    /**
    * Get the version number for the device.
    *
    * @return The version number.
    *
    */
    U8 GetVersionNum()
    {
        return deviceInfo.versionNum;
    }

    /**
    * If the manufacturer is known, this function obtains a string representation
    * of the manufacturer's name.
    *
    * @param[out] buffer  A user supplied buffer of length
    *                     DeviceIdentifierInfo::STR_DEVICE_MANUFACTURER_LEN
    *                     which will be updated to contain the device
    *                     manufacturer's name as a null-terminated string.
    *
    * @return The length of the string read into the buffer or zero if failed
    *         to read the device type as a string.
    */
    size_t GetDeviceManufacturerAsString(
         _Out_z_cap_c_(STR_DEVICE_MANUFACTURER_LEN) char *buffer)
    {
        /* call 'c' API to get the manufacturer's name */
        CFG_ROUTER_getDeviceManufacturerAsString(&deviceInfo, buffer);

        /* return length of string read */
        return strlen(buffer);
    }

    /**
    * If the manufacturer and device type is known, this function obtains a
    * string representation of the device's type.
    *
    * @param[out] buffer  A user supplied buffer of length
    *                     DeviceIdentifierInfo::STR_DEVICE_TYPE_LEN which will
    *                     be updated to contain the device type as a
    *                     null-terminated string.
    *
    * @return The length of the string read into the buffer or zero if failed
    *         to read the device type as a string.
    */
    size_t GetDeviceTypeAsString(_Out_z_cap_c_(STR_DEVICE_TYPE_LEN) char *buffer)
    {
        /* call 'c' API to get the device's type */
        CFG_ROUTER_getDeviceTypeAsString(&deviceInfo, buffer);

        /* return length of string read */
        return strlen(buffer);
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

