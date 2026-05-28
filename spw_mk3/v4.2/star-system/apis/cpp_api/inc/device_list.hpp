/**
 * \file device_list.hpp
 *
 * \brief Holds a list of STAR devices.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds a list of STAR devices.
 *
 * \note The list holds information about local devices or virtual devices only.
 *       A separate list is provided for Remote devices.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_DEVICE_LIST_CPP__
#define __STAR_DEVICE_LIST_CPP__

#include "device_factory.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
 * Holds information about available STAR devices.
 *
 * \note This object holds information about local devices or virtual devices
 *       only. Separate functions are provided in the API for Remote devices.
 *
 * \note When created or populated this object holds a snapshot of the current
 *       state of the system and is not automatically updated. Call the
 *       Populate() method on the object to re-populate the list with a newer
 *       snapshot of the current state of the system.
 *
 */
class DeviceList
{
public:
    /** define preferences available for automatically populating the device
     * list when it is constructed */
    enum AutoPopulatePreference
    {
        /** automatically populate the device list when it is constructed */
        AUTOPOPULATE,
        /** don't populate the device list when it is constructed (this is the
          * default if no preference is specified) */
        NO_AUTOPOPULATE

    };

private:
    /** list of devices held */
    Device **pDevices;

    /** number of devices on the list */
    U32 deviceCount;

    /**
    * (Re-)Populate the contents of the device list by obtaining the currently
    * available devices from the STAR API.
    *
    * \note If the device list has already been populated previously, calling
    *       this method again will be re-populate its contents with a newer
    *       snapshot of the current state of the system.
    *
    * @return The number of devices found, or 0 if no devices were found.
    *
    */
    U32 PopulateDeviceList()
    {
        /* create array of device types to return all devices */
        STAR_DEVICE_TYPE deviceTypes[] = { STAR_DEVICE_ALL };

        /* populate device list with all devices */
        return PopulateDeviceListForTypes(deviceTypes, 1);
    }

    /**
    * (Re-)Populate the contents of the device list by obtaining the currently
    * available devices from the STAR API.
    *
    * \note If the device list has already been populated previously, calling
    *       this method again will be re-populate its contents with a newer
    *       snapshot of the current state of the system.
    *
    * @param deviceTypes The types of device to include in the device list.
    * @param deviceTypeCount The number of device types in the given array.
    *
    * @return The number of devices found, or 0 if no devices were found.
    *
    */
    U32 PopulateDeviceListForTypes(STAR_DEVICE_TYPE deviceTypes[],
        U32 deviceTypeCount)
    {
        U32 starDeviceNum, i;
        STAR_DEVICE_ID *starDeviceIDs;

        /* free any existing list if required, it will be refreshed */
        FreeDeviceList();

        /* if empty array was specified */
        if(deviceTypeCount == 0)
        {
            /* return 0 items in list */
            return 0;
        }

        /* get list of IDs of all current devices from STAR API */
        starDeviceIDs = STAR_getDeviceListForTypes(deviceTypes,
            deviceTypeCount, &starDeviceNum);

        /* if no devices */
        if ((!starDeviceIDs) || (!starDeviceNum))
        {
            /* if a valid list of IDs was returned */
            if (starDeviceIDs)
            {
                /* free it */
                STAR_destroyDeviceList(starDeviceIDs);
            }

            /* return 0 items in list */
            return 0;
        }

        /* allocate array of pointers to Devices */
        try
        {
            pDevices = new Device*[starDeviceNum];
        }
        catch (std::bad_alloc& ba)
        {
            /* error allocating memory */
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            pDevices = NULL;
        }


        /* if devices allocated successfully */
        if (pDevices)
        {
            /* set number of devices found */
            deviceCount = starDeviceNum;

            /* for each STAR device ID */
            for (i=0; i<starDeviceNum; i++)
            {
                /* create a corresponding Device object and add it to
                 * the device list */
                pDevices[i] = DeviceFactory::CreateDevice(starDeviceIDs[i]);
            }
        }

        /* free list of device IDs obtained from STAR API */
        STAR_destroyDeviceList(starDeviceIDs);

        /* return number of devices on the list */
        return deviceCount;
    }

    /**
    * Free the list, if it exists.
    */
    void FreeDeviceList()
    {
        /* if a list exists */
        if (pDevices)
        {
            /* for each item in the list */
            for (U32 i = 0; i<deviceCount; i++)
            {
                /* if the item exists */
                if (pDevices[i])
                {
                    /* free it */
                    delete pDevices[i];
                }
            }

            /* free the list itself */
            delete [] pDevices;

            /* set list to NULL to support checks for list validity elsewhere */
            pDevices = NULL;

            /* reset count of devices */
            deviceCount = 0;
        }
    }

    /**
    * Copy the contents of the given device list into 'this' list.
    *
    * @return The number of devices copied, or 0 if no devices were copied.
    */
    U32 CopyList(const DeviceList& listToCopyFrom)
    {
        U32 i;

        /* free any existing list if required, it will be replaced with the
         * contents of the copied list */
        FreeDeviceList();

        /* if there are no devices to copy */
        if (!listToCopyFrom.pDevices)
        {
            /* return 0 items copied */
            return 0;
        }

        /* allocate array of pointers to Devices */
        try
        {
            pDevices = new Device*[listToCopyFrom.deviceCount];
        }
        catch (std::bad_alloc& ba)
        {
            /* error allocating memory */
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            pDevices = NULL;
        }

        /* if devices allocated successfully */
        if (pDevices)
        {
            /* set number of devices found */
            deviceCount = listToCopyFrom.deviceCount;

            /* for each device being copied */
            for (i=0; i<deviceCount; i++)
            {
                /* if there is a device to copy */
                if (listToCopyFrom.pDevices[i])
                {
                    /* create a corresponding Device object and add it to
                    * this device list */
                    pDevices[i] = DeviceFactory::CreateDevice(
                                  listToCopyFrom.pDevices[i]->GetDeviceID());
                }
                else
                {
                    /* no device to copy */
                    pDevices[i] = NULL;
                }
            }
        }

        /* return number of devices on the list */
        return deviceCount;
    }

public:
    /**
    * Default constructor - initialise device list to its default state.
    *
    * @param autoPopulate Optional parameter which specifies whether the device
    *                     list should automatically populate itself
    *                     (DeviceList::AUTOPOPULATE) or not
    *                     (DeviceList::NO_AUTOPOPULATE). The default, if no
    *                     input parameter is provided is 'NO_AUTOPOPULATE'.
    */
    explicit DeviceList(
        AutoPopulatePreference autoPopulate=DeviceList::NO_AUTOPOPULATE) :
        pDevices(NULL), deviceCount(0)
    {
        /* if the list should be automatically populated */
        if (autoPopulate == DeviceList::AUTOPOPULATE)
        {
            /* automatically populate the list of available devices */
            PopulateDeviceList();
        }
    }

    /**
    * Destructor. Free device list contents if required.
    */
    ~DeviceList()
    {
        /* free the device list if required */
        FreeDeviceList();
    }

    /**
    * Copy constructor to allow copies of DeviceList objects to be made.
    *
    * \note This will result in a duplicate copy of the contents of the device
    *       list being made.
    */
    DeviceList(const DeviceList& listToCopyFrom)
    {
        /* copy the contents of the provided list into 'this' */
        CopyList(listToCopyFrom);
    }

    /**
    * Overloaded assignment operator - to allow instances of device list
    * objects to have their contents assigned to each other.
    *
    * The existing content of the device list will be overwritten with the
    * content of the list being assigned.
    *
    */
    DeviceList& operator=(const DeviceList& listBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &listBeingAssigned)
            return *this;

        /* copy the contents of the assigned list into 'this' */
        CopyList(listBeingAssigned);

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Populate (or re-populate) the contents of the device list.
    *
    * \note If a list already exists this method will re-populate it with a
    *       newer snapshot of the current state of the system.
    *
    * @return The number of devices found.
    */
    U32 Populate()
    {
        return PopulateDeviceList();
    }

    /**
    * Populate (or re-populate) the contents of the device list with devices
    * only of the specified type.
    *
    * \note If a list already exists this method will re-populate it with a
    *       newer snapshot of the current state of the system based on the
    *       specified device type.
    *
    * @param deviceType The type of device to populate list for.
    *
    * @return The number of devices found.
    */
    U32 PopulateForType(STAR_DEVICE_TYPE deviceType)
    {
        /* create array of device types to populate */
        STAR_DEVICE_TYPE deviceTypes[] = { deviceType };

        /* populate the device list and return the device count */
        return PopulateDeviceListForTypes(deviceTypes, 1);
    }

    /**
    * Populate (or re-populate) the contents of the device list with devices
    * only of the specified types.
    *
    * \note If a list already exists this method will re-populate it with a
    *       newer snapshot of the current state of the system based on the
    *       specified device types.
    *
    * @param deviceTypes The types of device to populate list for.
    * @param deviceTypeCount The number of devices in the given array.
    *
    * @return The number of devices found.
    */
    U32 PopulateForTypes(STAR_DEVICE_TYPE deviceTypes[], U32 deviceTypeCount)
    {
        /* populate the device list and return the device count */
        return PopulateDeviceListForTypes(deviceTypes, deviceTypeCount);
    }

    /**
    * Empty the contents of the device list.
    */
    void Empty()
    {
        /* free the device list */
        FreeDeviceList();
    }

    /**
    * Get whether the device list is empty or not
    */
    bool IsEmpty()
    {
        /* if no devices held */
        if (!deviceCount)
        {
            /* empty */
            return true;
        }
        else
        {
            /* not empty */
            return false;
        }
    }

    /**
    * Get the number of devices on the list.
    *
    * @return The count of devices held on the list.
    */
    U32 GetDeviceCount()
    {
        return deviceCount;
    }

    /**
    * Overloaded subscript operator, used to iterate through the Device
    * instances on the list. Returns a pointer to the Device at the specified
    * index position in the list.
    *
    * @param index The index position of the device being sought.
    *
    * @return A pointer to the device at the index'th position in the list or
    *         NULL if there is no device at the position specified or if an
    *         invalid index is provided or if the list of devices is empty.
    *
    * \note If the list is re-populated or destroyed the pointer returned will
    *       no longer be valid. If you wish to make a copy of the Device at the
    *       specified position in the list, use the CopyDeviceAt() method
    *       instead.
    */
    Device* operator[] (const unsigned int index)
    {
        /* if no devices to select from */
        if (!pDevices)
        {
            /* return NULL */
            return NULL;
        }

        /* if invalid index */
        if (index >= deviceCount)
        {
            /* return NULL */
            return NULL;
        }

        /* return the index'th item in the list */
        return pDevices[index];
    }

    /**
    * Create a new instance of a Device object which is a copy of the Device
    * at the specified index position in the list.
    *
    * @param index The index position of the device to make a new copy of.
    *
    * @return A pointer to a new Device object which is a copy of the device at
    *         the index'th position in the list OR NULL if there is no device at
    *         the position specified, or if an invalid index is provided, or if
    *         the list of devices is empty, or if failed to make a copy, e.g.
    *         a memory allocation error.
    *
    * \note This method returns a newly allocated Device object which must be
    *       freed when no longer required by using the C++ delete operator. If
    *       the DeviceList is destroyed the pointer returned will still be
    *       valid. This method is intended to be useful when wishing to select
    *       a specific Device from the list to work with and then be able to
    *       immediately free the DeviceList itself if desired, since it is no
    *       longer required.
    *
    */
    Device *CopyDeviceAt(unsigned int index)
    {
        /* if no devices to select from */
        if (!pDevices)
        {
            /* return NULL */
            return NULL;
        }

        /* if invalid index */
        if (index >= deviceCount)
        {
            /* return NULL */
            return NULL;
        }

        /* if the index'th device doesn't exist */
        if (!pDevices[index])
        {
            /* return NULL */
            return NULL;
        }

        /* return instance of a new Device object which is a copy of the
         * index'th item on the list */
        return DeviceFactory::CreateDevice(pDevices[index]->GetDeviceID());
    }

};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

