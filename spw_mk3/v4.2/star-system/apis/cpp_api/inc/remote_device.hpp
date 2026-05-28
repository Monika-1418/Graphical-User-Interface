/**
 * \file remote_device.hpp
 *
 * \brief Represents a remote device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a remote device, i.e. a device which isn't connected directly to
 * a 'host' but which can be accessed indirectly via a SpaceWire link and from
 * a specified channel on a local device.
 *
 * Copyright &copy; 2014 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __REMOTE_DEVICE_CPP__
#define __REMOTE_DEVICE_CPP__

#include "star-dundee_annotations.h"
#include "star_device.hpp"
#include "device_factory.hpp"
#include "address.hpp"
#include "cfg_api_remote.h"
#include "brick_mk3_device_configuration.hpp"
#include "router_mk2s_device_configuration.hpp"
#include "brick_mk2_device_configuration.hpp"
#include "pcie_device_configuration.hpp"
#include "pci_mk2_device_configuration.hpp"
#include "remote_device_exception.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Represents a remote device, i.e. a device which isn't connected directly to
* a 'host' but which can be accessed indirectly via a SpaceWire link and from
* a specified channel on a local device
*/
class RemoteDevice : public STARDevice
{
public:
    /** String constants, e.g. maximum expected length of strings provided as
      * parameters into functions. */
    enum StringConstants
    {
        /** maxmimum expected length of remote device name string */
        STR_REMOTE_DEVICE_NAME_LEN = STAR_STR_MAX_LEN
    };

private:
    /** record whether the device ID 'owned' by this device was provided (e.g.
     * via a constructor) or whether it was created by the object using a call
     * to CreateRemoteDevice().
     */
    bool deviceIDProvidedRatherThanCreated;

    /**
    * Creates this remote device using the properties of another given device.
    *
    * @param idOfExistingDevice The ID of the device to copy from.
    *
    */
    int CopyExistingDevice(STAR_DEVICE_ID idOfExistingDevice)
    {
        /* if invalid device ID */
        if (!idOfExistingDevice)
        {
            /* end */
            return 0;
        }

        /* get properties of existing device */
        STAR_DEVICE_ID idOfLocalDevice;
        char *deviceName = NULL;
        STAR_SPACEWIRE_ADDRESS *pathToDevice = NULL, *returnPath = NULL;
        unsigned char channelNumber;
        char error = 0;
        int result = 0;

        /* get the device name */
        deviceName = STAR_CFG_getRemoteDeviceDescriptionNameString(
                                                           idOfExistingDevice);

        /* if no device name */
        if ((deviceName == NULL) || (strlen(deviceName)==0))
        {
            error = 1;
        }

        /* the id of the local device */
        if (!STAR_CFG_getRemoteDeviceLocalDevice(idOfExistingDevice,
                                                &idOfLocalDevice))
        {
            error = 1;
        }
        else
        {
            if (!idOfLocalDevice)
                error = 1;
        }

        /* get the channel number used */
        if (!STAR_CFG_getRemoteDeviceLocalChannel(idOfExistingDevice,
                                                  &channelNumber))
        {
            error = 1;
        }

        /* get the path to the device */
        if (!STAR_CFG_getRemoteDevicePath(idOfExistingDevice, &pathToDevice))
        {
            error = 1;
        }

        /* get the return path */
        if (!STAR_CFG_getRemoteDeviceRetPath(idOfExistingDevice, &returnPath))
        {
            error = 1;
        }

        /* if no errors getting properties of existing device */
        if (!error)
        {
            /* attempt to create a new remote device with the same properties */
            this->deviceID =
            STAR_CFG_createRemoteDeviceIdentifier(idOfLocalDevice,
                                    channelNumber, deviceName, pathToDevice,
                                    returnPath);

            /* if valid device created */
            if (this->deviceID)
            {
                /* record success */
                result = 1;
            }
        }

        /* clean up */
        if (deviceName)
            STAR_destroyString(deviceName);
        if (pathToDevice)
            STAR_destroyAddress(pathToDevice);
        if (returnPath)
            STAR_destroyAddress(returnPath);

        /* return the result */
        return result;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RemoteDevice() : deviceIDProvidedRatherThanCreated(false)
    {
    }

    /**
    * Overloaded constructor. Initialise object with given remote device ID.
    *
    * \note This constructor is typically used internally by the C++ API itself,
    *       rather than being required by client code.
    *
    * @param id The ID of the RemoteDevice that this object will represent.
    *
    */
    explicit RemoteDevice(STAR_DEVICE_ID id) :
        deviceIDProvidedRatherThanCreated(false)
    {
        /* set the device's ID */
        this->deviceID = id;

        /* indicate that the device ID held by this object was provided ready-
         * made, rather than being created internally by this object. This
         * affects later processing: when this object is destroyed it won't
         * attempt to destroy the remote device for the ID provided, because
         * it is assumed that it has been created and managed elsewhere.
         */
        deviceIDProvidedRatherThanCreated = true;
    }

    /**
    * Overloaded constructor. Automatically attempts to create the device
    * using the values specified
    *
    * @param pLocalDevice The local device through which communication with the
    *                    remote device is initiated on.
    * @param localChannel The number of the channel on the local device on which
    *                     communication with the remote device is made. The
    *                     channel cannot be used for any other purpose while a
    *                     \link stardundee::com::starsystem::deviceconfig::DeviceConfiguration DeviceConfiguration \endlink
    *                     operation is being performed.
    * @param name The name to be used to identify the remote device.
    * @param pathTo Path to the remote device. This should be the path to the
    *               remote device's configuration port (port 0), including a
    *               default logical address. This means that the path will
    *               normally end with "0 254".
    * @param returnPath Return path from the remote device to reach the local
    *                   channel. Configuration response packets are automatically
    *                   sent out of the port on which the command is received,
    *                   so this port number is not required. The path should also
    *                   be terminated with a default logical address. This means
    *                   that returnPath is normally one byte shorter than pathTo
    *                   and ends with "254".
    *
    * \throws RemoteDeviceException If the RemoteDevice fails to create.
    *
    */
    RemoteDevice(Device* pLocalDevice, unsigned char localChannel, char *name,
                 stardundee::com::starsystem::streamitems::Address *pathTo,
                 stardundee::com::starsystem::streamitems::Address *returnPath)
                 : deviceIDProvidedRatherThanCreated(false)
    {
        /* attempt to create the remote device */
        int result = CreateRemoteDevice(pLocalDevice, localChannel, name, pathTo,
                                        returnPath);

        /* if error creating remote device */
        if (result == 0)
        {
            /* throw exception */
            throw RemoteDeviceException();
        }
    }

    /**
    * Destructor. Free any resources created by object.
    */
    virtual ~RemoteDevice()
    {
        /* destroy the remote device if required */
        DestroyDevice();
    }

    /**
    * Copy constructor - to allow copies of remote device objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    *
    * \note This will result in a duplicate RemoteDevice being created which
    *       has the same properties as that being copied from.
    *
    */
    RemoteDevice(const RemoteDevice& deviceToCopyFrom)
        : STARDevice()
    {
        /* indicate that device ID held by object hasn't been provided */
        deviceIDProvidedRatherThanCreated = false;

        /* if device being copied from has a valid device ID (i.e. non-zero) */
        if (deviceToCopyFrom.deviceID)
        {
            /* create this remote device as a copy of the device provided */
            if (!CopyExistingDevice(deviceToCopyFrom.deviceID))
            {
                /* failed to create remote device */

                /* throw exception */
                throw RemoteDeviceException();
            }
        }
        else
        {
            /* set this as an un-initialised device */
            this->deviceID = 0;
        }
    }

    /**
    * Overloaded assignment operator - to allow instances of remote device
    * objects to have their contents assigned to each other.
    *
    * \note Any existing RemoteDevice held by this object will be freed /
    *       destroyed and a duplicate RemoteDevice will be be created which
    *       has the same properties as that being assigned.
    *
    */
    RemoteDevice& operator=(const RemoteDevice& deviceBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &deviceBeingAssigned)
            return *this;

        /* free contents of this device, if any. They are about to replaced
         * with new values.
         */
        DestroyDevice();

        /* indicate that device ID held by object hasn't been provided */
        deviceIDProvidedRatherThanCreated = false;

        /* if device being assigned has a valid device ID (i.e. non-zero) */
        if (deviceBeingAssigned.deviceID)
        {
            /* create this remote device as a copy of the assigned device */
            if (!CopyExistingDevice(deviceBeingAssigned.deviceID))
            {
                /* failed to create copy of remote device */

                /* throw exception */
                throw RemoteDeviceException();
            }
        }
        else
        {
            /* set this as an un-initialised device */
            this->deviceID = 0;
        }

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Create a remote device.
    *
    * \note Any existing RemoteDevice held by this object will be freed /
    *       destroyed and a new RemoteDevice will be created using the
    *       properties provided.
    *
    * @param pLocalDevice The local device through which communication with the
    *                    remote device is initiated on.
    * @param localChannel The number of the channel on the local device on which
    *                     communication with the remote device is made. The
    *                     channel cannot be used for any other purpose while a
    *                     \link stardundee::com::starsystem::deviceconfig::DeviceConfiguration DeviceConfiguration \endlink
    *                     operation is being performed.
    * @param name The name to be used to identify the remote device.
    * @param pathTo Path to the remote device. This should be the path to the
    *               remote device's configuration port (port 0), including a
    *               default logical address. This means that the path will
    *               normally end with "0 254".
    * @param returnPath Return path from the remote device to reach the local
    *                   channel. Configuration response packets are automatically
    *                   sent out of the port on which the command is received,
    *                   so this port number is not required. The path should also
    *                   be terminated with a default logical address. This means
    *                   that returnPath is normally one byte shorter than pathTo
    *                   and ends with "254".
    *
    * @return 1 if remote device created successfully, otherwise 0.
    *
    */
    int CreateRemoteDevice(Device* pLocalDevice, unsigned char localChannel,
         char *name, stardundee::com::starsystem::streamitems::Address *pathTo,
         stardundee::com::starsystem::streamitems::Address *returnPath)
    {
        /* check validity of input params */
        if ((pathTo == NULL) || (returnPath == NULL) || (pLocalDevice == NULL))
        {
            /* at least one invalid param, return error */
            return 0;
        }

        /* if remote device already created */
        if (deviceID != 0)
        {
            /* close / destroy the device before attempting to create a new one */
            DestroyDevice();
        }

        /* call c 'API' to create remote device, and receive the device ID */
        this->deviceID =
        STAR_CFG_createRemoteDeviceIdentifier(pLocalDevice->GetDeviceID(),
                                    localChannel, name,
                                    pathTo->GetAs_STAR_SPACEWIRE_ADDRESS(),
                                    returnPath->GetAs_STAR_SPACEWIRE_ADDRESS());

        /* if valid ID obtained */
        if (deviceID != 0)
        {
            /* reset flag to indicate that the device ID was created by this
             * object rather than being provided ready-made */
            deviceIDProvidedRatherThanCreated = false;

            /* return success */
            return 1;
        }
        else
        {
            /* return error */
            return 0;
        }
    }

    /**
    * Destroy this remote device.
    *
    * @return Whether the remote device was successfully destroyed (1) or not (0).
    */
    int DestroyDevice()
    {
        /* record result of destroy attempt, set to zero / error initially */
        int result = 0;

        /* if there is a remote device to destroy */
        if (deviceID != 0)
        {
            /* if this device ID was provided rather than being created
             * internally by this object */
            if (deviceIDProvidedRatherThanCreated)
            {
                /* don't need to destroy, reset flag */
                deviceIDProvidedRatherThanCreated = false;
            }
            else
            {
                /* call 'c' API to destroy the remote device, record result */
                result = STAR_CFG_destroyRemoteDeviceIdentifier(deviceID);
            }

            /* reset device ID */
            deviceID = 0;
        }

        /* return result */
        return result;
    }

    /**
    * Gets the name used to describe / identify this device.
    *
    * @param[out] strName A user supplied buffer of length
    *                     RemoteDevice::STR_REMOTE_DEVICE_NAME_LEN which will
    *                     be updated to contain the device name as a null-
    *                     terminated string.
    *
    * @return The length of the string read into the buffer or zero if failed
    *         to read the remote device name.
    *
    */
    size_t GetName(_Out_z_cap_c_(STR_REMOTE_DEVICE_NAME_LEN) char *strName)
    {
        /* if no valid device ID */
        if (deviceID == 0)
        {
            /* return 0 / error */
            return 0;
        }

        /* if no buffer supplied */
        if (!strName)
        {
            /* return 0 / error */
            return 0;
        }

        /* call 'c' API to obtain device's name / description */
        char *deviceName;
        deviceName = STAR_CFG_getRemoteDeviceDescriptionNameString(deviceID);

        /* if no device name obtained */
        if (!deviceName)
        {
            /* return 0 / error */
            return 0;
        }

        /* get the size of the string obtained */
        size_t len = strlen(deviceName);

        /* if size exceeds the expected buffer size */
        if (len > (STR_REMOTE_DEVICE_NAME_LEN-1))
        {
            /* adjust the length to be the maximum size (this will truncate
             * the string) */
            len = STR_REMOTE_DEVICE_NAME_LEN-1;
        }

        /* copy the string obtained to the user-supplied buffer */
        for (unsigned int i=0; i<len; i++)
            strName[i] = deviceName[i];

        /* append with a NULL */
        strName[len] = '\0';

        /* destroy the string obtained from the STAR API */
        STAR_destroyString(deviceName);

        /* return the length of string obtained */
        return strlen(strName);
    }

    /**
    * Gets the number of the channel on the local device used to communicate
    * with this remote device.
    *
    * @param[out] channelNumber Will receive the number of the local channel
    *                           being used, set by this function.
    *
    * @return 1 on success, otherwise 0.
    *
    */
    int GetLocalDeviceChannelNumber(unsigned char *channelNumber)
    {
        /* if no valid device ID */
        if (deviceID == 0)
        {
            /* return error */
            return 0;
        }

        /* call 'c' API to get local channel number */
        return STAR_CFG_getRemoteDeviceLocalChannel(deviceID, channelNumber);
    }

    /**
    * Gets the local device used to communicate with the remote device on.
    *
    * @return A pointer to a Device object representing the local device,
    *         or NULL upon error or no device found.
    *
    * \note The object returned by this function must be freed when no longer
    *       required by calling the C++ delete operator.
    */
    Device* GetLocalDevice()
    {
        /* if no valid remote device ID to enquire upon */
        if (deviceID == 0)
        {
            /* return NULL */
            return NULL;
        }

        /* call 'c' API to get the ID of the local device which is associated
         * with the remote device */
        STAR_DEVICE_ID idForLocalDevice = 0;
        int result = STAR_CFG_getRemoteDeviceLocalDevice(deviceID,
                                                         &idForLocalDevice);

        /* if failed to get local device */
        if (result == 0)
        {
            /* return NULL */
            return NULL;
        }

        /* create new Device object for local device */
        Device *localDevice = DeviceFactory::CreateDevice(idForLocalDevice);

        /* return local device (Note: may be NULL if create failed) */
        return localDevice;
    }

    /**
    * Gets the path to the device.
    *
    * @return A pointer to a Address object representing the path, or
    *         NULL if error.
    *
    * \note The object returned by this function must be freed when no longer
    *       required by calling the C++ delete operator.
    */
    stardundee::com::starsystem::streamitems::Address *GetPathToDevice()
    {
        /* if no valid remote device ID to enquire upon */
        if (deviceID == 0)
        {
            /* return NULL */
            return NULL;
        }

        /* call 'c' API to get the path to the remote device */
        STAR_SPACEWIRE_ADDRESS *pathToDevice;
        int result = STAR_CFG_getRemoteDevicePath(deviceID, &pathToDevice);

        /* if failed to get path */
        if (result == 0)
        {
            /* return NULL */
            return NULL;
        }

        /* else, create and return a new Address object with the path */
        stardundee::com::starsystem::streamitems::Address *pAddress = new
             stardundee::com::starsystem::streamitems::Address(pathToDevice);
        return pAddress;
    }

    /**
    * Gets the return path from the device.
    *
    * @return A pointer to a Address object representing the return
    *         path, or NULL if error.
    *
    * \note The object returned by this function must be freed when no longer
    *       required by calling the C++ delete operator.
    */
    stardundee::com::starsystem::streamitems::Address *GetReturnPath()
    {
        /* if no valid remote device ID to enquire upon */
        if (deviceID == 0)
        {
            /* return NULL */
            return NULL;
        }

        /* call 'c' API to get the return path from the remote device */
        STAR_SPACEWIRE_ADDRESS *returnPath;
        int result = STAR_CFG_getRemoteDeviceRetPath(deviceID, &returnPath);

        /* if failed to get path */
        if (result == 0)
        {
            /* return NULL */
            return NULL;
        }

        /* else, create and return a new Address object with the return path*/
        stardundee::com::starsystem::streamitems::Address *pAddress = new
            stardundee::com::starsystem::streamitems::Address(returnPath);
        return pAddress;
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
    * @return A pointer to a new DeviceConfiguration object which contains
    *         the configuration state for the device, or NULL if failed to
    *         allocate memory for the object or failed to determine its type,
    *         hence the corresponding \link stardundee::com::starsystem::deviceconfig::DeviceConfiguration DeviceConfiguration \endlink
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
    stardundee::com::starsystem::deviceconfig::DeviceConfiguration *GetDeviceConfiguration()
    {
        /* a pointer to a new DeviceConfiguration object, initially null */
        stardundee::com::starsystem::deviceconfig::DeviceConfiguration
                                                   *pDeviceConfiguration = NULL;

        /* the type of device needs to be determined so the appropriate
         * configuration object for the device can be instantiated and returned
         */

        /* first, get the device identification info for this device, which
         * contains the device's chip type */

        /* declare struct required by 'c' API to hold device identifier info */
        STAR_CFG_DEVICE_IDENTIFIER_INFO deviceInfo;

        /* call 'c' API to get identifier info for this device */
        int result = CFG_ROUTER_getDeviceIdentificationInfo(this->deviceID,
                                                            &deviceInfo);

        /* if failed to obtain device identifier info */
        if (result == 0)
        {
            /* return error / null */
            return NULL;
        }

        /* create an instance of a DeviceConfiguration object based on the
         * device's chip type */
        try
        {
            switch (deviceInfo.chipType)
            {
            case 7:
                /* a PCI Mk2 device */
                pDeviceConfiguration = new
                stardundee::com::starsystem::deviceconfig::mk2devices::pcimk2::PCIMk2DeviceConfiguration(this->deviceID);
                break;
            case 8:
                /* a PCIe device */
                pDeviceConfiguration = new
                stardundee::com::starsystem::deviceconfig::mk2devices::pcie::PCIeDeviceConfiguration(this->deviceID);
                break;
            case 15:
                /* an SPLT device BUT currently the same as a PCIe device */
                pDeviceConfiguration = new
                stardundee::com::starsystem::deviceconfig::mk2devices::pcie::PCIeDeviceConfiguration(this->deviceID);
                break;
            case 16:
                /* a Router Mk2S device */
                pDeviceConfiguration = new
                stardundee::com::starsystem::deviceconfig::mk2devices::routermk2s::RouterMk2SDeviceConfiguration(this->deviceID);
                break;
            case 17:
                /* a Brick Mk2 device */
                pDeviceConfiguration = new
                stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2DeviceConfiguration(this->deviceID);
                break;
            case 19:
                /* a Brick Mk3 device */
                pDeviceConfiguration = new
                stardundee::com::starsystem::deviceconfig::mk2devices::brickmk3::BrickMk3DeviceConfiguration(this->deviceID);
                break;
            default:
                /* assume it to be an older device for which a standard
                 * configuration state will be provided */
                pDeviceConfiguration = new
                stardundee::com::starsystem::deviceconfig::DeviceConfiguration(this->deviceID);
                break;
            }
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for new object */
            pDeviceConfiguration = NULL;
        }

        /* return the device configuration, or NULL if an error occurred */
        return pDeviceConfiguration;
    }

    /**
     * Create and return an instance of a RemoteDevice object which
     * is a copy of 'this' object.
     *
     * @return A pointer to a new RemoteDevice object, or NULL if failed to
     *         allocate memory for the object.
     *
     * \note The object returned must be freed when no longer required by
     *       calling the C++ delete operator.
     *
     */
    virtual STARDevice *CopyDevice()
    {
        /* a pointer to the clone device, initially null */
        RemoteDevice *pCloneDevice = NULL;

        /* create an instance of new a RemoteDevice */
        try
        {
            pCloneDevice = new RemoteDevice();
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

