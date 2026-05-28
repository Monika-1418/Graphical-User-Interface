/**
 * \file driver.hpp
 *
 * \brief Holds information for a currently running/installed driver.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information for a currently running/installed driver.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_DRIVER_CPP__
#define __STAR_DRIVER_CPP__

#include "version_information.hpp"
#include "star-dundee_annotations.h"
#include <new>
#include "device_factory.hpp"
#include "cpp_api_macros.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Holds information for a currently running/installed driver.
*/
class Driver
{
public:
    /** available driver types */
    enum DriverType
    {
        /** invalid */
        DRIVER_TYPE_INVALID = STAR_DRIVER_INVALID,
        /** USB driver */
        DRIVER_TYPE_USB = STAR_DRIVER_TYPE_USB,
        /** PCI driver */
        DRIVER_TYPE_PCI = STAR_DRIVER_TYPE_PCI,
        /** TCP/IP driver */
        DRIVER_TYPE_TCPIP = STAR_DRIVER_TYPE_TCPIP,
        /** Virtual driver */
        DRIVER_TYPE_VIRTUAL = STAR_DRIVER_TYPE_VIRTUAL
    };

private:
    /** the ID of the driver */
    STAR_DRIVER_ID driverID;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    Driver() : driverID(0)
    {
    }

    /**
    * Overloaded constructor. Initialise driver with given ID.
    *
    * \note This constructor is typically used internally by the C++ API itself,
    *       rather than being required by client code.
    *
    * @param id The id to assign to the driver.
    */
    explicit Driver(STAR_DRIVER_ID id) : driverID(id)
    {
    }

    /**
    * Destructor. Free any resources created.
    */
    ~Driver()
    {
    }

    /**
    * Copy constructor - to allow copies of driver objects to be made when
    * being passed by value in functions or returned by value from functions.
    *
    */
    Driver(const Driver& driverToCopyFrom)
    {
        /* Set contents of this driver to default / empty values. */
        /* ResetDriver(); - not required yet */

        /* set ID of this driver to be the same as that being copied from */
        this->driverID = driverToCopyFrom.driverID;

        /* no other fields have to be copied, they will all be loaded and
         * set on the fly when the member functions of the object are called
         * during normal usage, using the driver's ID.
         */
    }

    /**
    * Overloaded assignment operator - to allow instances of driver objects to
    * have their contents assigned to each other.
    *
    */
    Driver& operator=(const Driver& driverBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &driverBeingAssigned)
            return *this;

        /* free contents of this driver, if any. They are about to replaced with
         * new values.
         */
        /* ResetDriver(); - not required yet */

        /* set ID of this driver to be the same as that being assigned */
        this->driverID = driverBeingAssigned.driverID;

        /* no other fields have to be copied, they will all be loaded and
         * set on the fly when the member functions of the object are called
         * during normal usage, using the driver's ID.
         */

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Get version information for the driver.
    *
    * @return A pointer to a new VersionInformation object, containing the
    *         driver's version information.
    *
    * \note The object returned by this function must be freed when no longer
    *       required by calling the C++ delete operator.
    */
    VersionInformation* GetDriverVersion()
    {
        /* get driver version using standard 'c' API */
        STAR_VERSION_INFO *versionInfo = STAR_getDriverVersion(this->driverID);

        /* if no version info found */
        if (versionInfo == NULL)
        {
            /* return no versions */
            return NULL;
        }

        /* create a new version information object to hold the version data */
        VersionInformation* viObj = NULL;
        try
        {
            viObj = new VersionInformation(versionInfo);
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* error allocating memory, set version to NULL */
            viObj = NULL;
        }

        /* free the memory struct created by the 'c' API */
        STAR_destroyVersionInfo(versionInfo);

        /* return pointer to new version information object, or NULL */
        return viObj;
    }

    /**
    * Get the type (USB / PCI / specific virtual type identifier) of the driver.
    *
    * @return The driver's type.
    *
    */
    DriverType GetDriverType()
    {
        /* call standard 'c' API to get the driver type */
        return (DriverType)STAR_getDriverType(this->driverID);
    }

    /**
    * Get the driver's ID.
    *
    * @return The driver's ID.
    *
    */
    STAR_DRIVER_ID GetDriverID()
    {
        /* return ID */
        return driverID;
    }

    /**
    * Determines whether the driver is virtual or not.
    *
    * @return 1 if the driver is virtual, otherwise 0.
    *
    */
    int IsDriverVirtual()
    {
        /* call standard 'c' API to determine if the specified
        *  driver is virtual or not
        */
        return STAR_isDriverVirtual(this->driverID);
    }

    /**
    * Gets an array of devices present for this driver.
    *
    * @param[out] count Number of devices returned in the array.
    *
    * @return An array of pointers to Device objects representing all of the
    *         devices found for the driver, or NULL if no devices are found.
    *
    * \note This function returns a snapshot of the current state of the
    *       the system and is not automatically updated.
    *
    * \note This array and its contents must be freed when no longer required
    *       using the C++ delete operator or the STARSystem::DestroyDeviceList()
    *       function.
    */
    Device** GetDeviceListForDriver(_Out_ U32* count)
    {
        /* set empty / zero Devices */
        Device **deviceList = NULL;
        *count = 0;

        /* call STAR API to get a list of the IDs of all devices available
         * for this driver */
        U32 deviceIDNum;
        STAR_DEVICE_ID *deviceIDs = STAR_getDeviceListForDriver(this->driverID,
                                                                &deviceIDNum);

        /* if devices found */
        if (deviceIDs != NULL)
        {
            /* try to allocate array of pointers to Devices */
            try
            {
                deviceList = new Device*[deviceIDNum];
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* error allocating memory */

                /* ensure empty device list */
                deviceList = NULL;
            }

            /* if devices allocated successfully */
            if (deviceList != NULL)
            {
                /* set number of devices found */
                *count = deviceIDNum;

                /* for each device ID found */
                for (U32 i=0; i<deviceIDNum; i++)
                {
                    /* create a corresponding Device object
                     * Note: a factory object is used to prevent the Driver
                     * object from being coupled to specific child classes
                     * of Device */
                    deviceList[i] = DeviceFactory::CreateDevice(deviceIDs[i]);

                    /* ensure that the device's ID value has been set */
                    deviceList[i]->SetDeviceID(deviceIDs[i]);
                }
            }

            /* free list of device IDs obtained from STAR API */
            STAR_destroyDeviceList(deviceIDs);
        }

        /* return list of Devices, may be NULL */
        return deviceList;
    }
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

