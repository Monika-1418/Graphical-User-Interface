/**
 * \file hardware_info.hpp
 *
 * \brief Holds information about the hardware version of a device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information about the hardware version of a device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_MK2_CONFIG
 */

#ifndef __DEVICE_HARDWARE_INFO_CPP__
#define __DEVICE_HARDWARE_INFO_CPP__

#include "star-api.h"
#include "cfg_api_mk2_types.h"

namespace stardundee { namespace com { namespace starsystem {
              namespace deviceconfig { namespace mk2devices {

/**
* Holds information about the hardware version of a device.
*/
class HardwareInfo
{
private:
    /** struct required by the STAR-API to hold device hardware info */
    STAR_CFG_MK2_HARDWARE_INFO hardwareInfo;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    HardwareInfo()
    {
        hardwareInfo.day = 0;
        hardwareInfo.edit = 0;
        hardwareInfo.hour = 0;
        hardwareInfo.major = 0;
        hardwareInfo.minor = 0;
        hardwareInfo.minute = 0;
        hardwareInfo.month = 0;
        hardwareInfo.patch = 0;
        hardwareInfo.year = 0;

    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param info A STAR_CFG_MK2_HARDWARE_INFO struct which contains the
    *             hardware info values to assign to this object.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    */
    explicit HardwareInfo(STAR_CFG_MK2_HARDWARE_INFO info)
    {
        hardwareInfo.day = info.day;
        hardwareInfo.edit = info.edit;
        hardwareInfo.hour = info.hour;
        hardwareInfo.major = info.major;
        hardwareInfo.minor = info.minor;
        hardwareInfo.minute = info.minute;
        hardwareInfo.month = info.month;
        hardwareInfo.patch = info.patch;
        hardwareInfo.year = info.year;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    */
    HardwareInfo(U8 major, U8 minor, U8 edit, U8 patch, U16 year, U8 month,
                 U8 day, U8 hour, U8 minute)
    {
        hardwareInfo.day = day;
        hardwareInfo.edit = edit;
        hardwareInfo.hour = hour;
        hardwareInfo.major = major;
        hardwareInfo.minor = minor;
        hardwareInfo.minute = minute;
        hardwareInfo.month = month;
        hardwareInfo.patch = patch;
        hardwareInfo.year = year;
    }

    /**
    * Copy constructor - to allow copies of HardwareInfo objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    HardwareInfo(const HardwareInfo& infoToCopyFrom)
    {
        /* copy contents of object being copied into this object */
        hardwareInfo.day = infoToCopyFrom.hardwareInfo.day;
        hardwareInfo.edit = infoToCopyFrom.hardwareInfo.edit;
        hardwareInfo.hour = infoToCopyFrom.hardwareInfo.hour;
        hardwareInfo.major = infoToCopyFrom.hardwareInfo.major;
        hardwareInfo.minor = infoToCopyFrom.hardwareInfo.minor;
        hardwareInfo.minute = infoToCopyFrom.hardwareInfo.minute;
        hardwareInfo.month = infoToCopyFrom.hardwareInfo.month;
        hardwareInfo.patch = infoToCopyFrom.hardwareInfo.patch;
        hardwareInfo.year = infoToCopyFrom.hardwareInfo.year;
    }

    /**
    * Overloaded assignment operator - to allow instances of HardwareInfo
    * objects to have their contents assigned to each other.
    *
    */
    HardwareInfo& operator=(const HardwareInfo& infoBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &infoBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        hardwareInfo.day = infoBeingAssigned.hardwareInfo.day;
        hardwareInfo.edit = infoBeingAssigned.hardwareInfo.edit;
        hardwareInfo.hour = infoBeingAssigned.hardwareInfo.hour;
        hardwareInfo.major = infoBeingAssigned.hardwareInfo.major;
        hardwareInfo.minor = infoBeingAssigned.hardwareInfo.minor;
        hardwareInfo.minute = infoBeingAssigned.hardwareInfo.minute;
        hardwareInfo.month = infoBeingAssigned.hardwareInfo.month;
        hardwareInfo.patch = infoBeingAssigned.hardwareInfo.patch;
        hardwareInfo.year = infoBeingAssigned.hardwareInfo.year;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~HardwareInfo()
    {
        /* no resources to free at present */
    }

    /**
    * Get the major version number.
    *
    * @return The major version number.
    *
    */
    U8 GetMajor()
    {
        return hardwareInfo.major;
    }

    /**
    * Get the minor version number.
    *
    * @return The minor version number.
    *
    */
    U8 GetMinor()
    {
        return hardwareInfo.minor;
    }

    /**
    * Get the version number edit.
    *
    * @return The version number edit.
    *
    */
    U8 GetEdit()
    {
        return hardwareInfo.edit;
    }

    /**
    * Get the version number patch.
    *
    * @return The version number patch.
    *
    */
    U8 GetPatch()
    {
        return hardwareInfo.patch;
    }

    /**
    * Get the year value of time and date.
    *
    * @return The year value of time and date.
    *
    */
    U16 GetYear()
    {
        return hardwareInfo.year;
    }

    /**
    * Get the month value of time and date.
    *
    * @return The month value of time and date.
    *
    */
    U8 GetMonth()
    {
        return hardwareInfo.month;
    }

    /**
    * Get the day value of time and date.
    *
    * @return The day value of time and date.
    *
    */
    U8 GetDay()
    {
        return hardwareInfo.day;
    }

    /**
    * Get the hour value of time and date.
    *
    * @return The hour value of time and date.
    *
    */
    U8 GetHour()
    {
        return hardwareInfo.hour;
    }

    /**
    * Get the minute value of time and date.
    *
    * @return The minute value of time and date.
    *
    */
    U8 GetMinute()
    {
        return hardwareInfo.minute;
    }

    /**
    * Return the STAR_CFG_MK2_HARDWARE_INFO struct which is held
    * internally by this object.
    *
    * @return A pointer to the STAR_CFG_MK2_HARDWARE_INFO structure which
    *         is held internally by this object.
    *
    * \note This is provided to support internal processing of other objects
    *       within the C++ API and would not normally require to be invoked by
    *       client applications using the API.
    */
    STAR_CFG_MK2_HARDWARE_INFO *GetAsSTAR_CFG_MK2_HARDWARE_INFO()
    {
        /* return pointer to the STAR_CFG_MK2_HARDWARE_INFO struct held
         * internally by this object. */
        return &hardwareInfo;
    }
};

/* end namespace tags */
} /* mk2devices */ } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

