/**
 * \file device.hpp
 *
 * \brief Represents a locally connected device (e.g. via USB or PCI).
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a locally connected device (e.g. via USB or PCI).
 *
 * Copyright &copy; 2014 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __LOCAL_DEVICE_CPP__
#define __LOCAL_DEVICE_CPP__

#include "star_device.hpp"
#include "version_information.hpp"
#include "channel.hpp"
#include "driver_factory.hpp"
#include "brick_mk3_device_configuration.hpp"
#include "router_mk2s_device_configuration.hpp"
#include "brick_mk2_device_configuration.hpp"
#include "pcie_device_configuration.hpp"
#include "pci_mk2_device_configuration.hpp"
#include "pxi_device_configuration.hpp"
#include <new>
#include "cpp_api_macros.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/* advance declaration of Driver class */
class Driver;

/**
* Represents a locally connected device (e.g. via USB or PCI).
*/
class Device : public STARDevice
{
public:
    /** The different types of bus that can be used to connect a device to a
      * PC. */
    enum BusType
    {
        /** the bus type is unknown */
        BUS_TYPE_UNKNOWN = STAR_BUS_UNKNOWN,
        /** PCI bus type */
        BUS_TYPE_PCI = STAR_BUS_PCI,
        /** USB bus type */
        BUS_TYPE_USB = STAR_BUS_USB,
        /** TCP/IP bus type, used by Ethernet devices */
        BUS_TYPE_TCP = STAR_BUS_TCP,
        /** cPCI bus type */
        BUS_TYPE_CPCI = STAR_BUS_CPCI,
        /** The bus type for virtual devices */
        BUS_TYPE_VIRTUAL = STAR_BUS_VIRTUAL
    };

    /** String constants, e.g. maximum expected length of strings provided as
      * parameters into functions. */
    enum StringConstants
    {
        /** maxmimum expected length of device name string */
        STR_DEVICE_NAME_LEN = STAR_SPW_DEVICE_NAME_MAX_LENGTH,
        /** maxmimum expected length of device serial number string */
        STR_DEVICE_SERIAL_LEN = STAR_SPW_DEVICE_SERIAL_NUMBER_MAX_LENGTH,
        /** maxmimum expected length of device type string */
        STR_DEVICE_TYPE_LEN = STAR_STR_MAX_LEN,
        /** maxmimum expected length of device bus type string */
        STR_DEVICE_BUS_TYPE_LEN = STAR_STR_MAX_LEN
    };

protected:
    /**
    * Free and reset all fields in the Device object.
    */
    void FreeDevice()
    {
        /* reset device ID */
        deviceID = 0;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    Device()
    {
        /* set no / empty device */
        deviceID = 0;
    }

    /**
    * Overloaded constructor. Initialise device to the given ID.
    *
    * \note This constructor is typically used internally by the C++ API itself,
    *       rather than being required by client code.
    *
    * @param id The ID for the device.
    */
    explicit Device(STAR_DEVICE_ID id)
    {
        /* set device ID */
        deviceID = id;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    virtual ~Device()
    {
        /* free memory held for any char* fields, if required */
        FreeDevice();
    }

    /**
    * Copy constructor - to allow copies of Device objects to be made when
    * being passed by value in functions or returned by value from functions.
    *
    */
    Device(const Device& deviceToCopyFrom) : STARDevice()
    {
        /* Set contents of this device to default / empty values. */
        deviceID = 0;

        /* set ID of this device to be the same as that being copied from */
        this->deviceID = deviceToCopyFrom.deviceID;

        /* no other fields have to be copied, they will all be loaded and
         * set on the fly when the member functions of the object are called
         * during normal usage, using the device's ID.
         */
    }

    /**
    * Overloaded assignment operator - to allow instances of device objects to
    * have their contents assigned to each other.
    *
    */
    Device& operator=(const Device& deviceBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &deviceBeingAssigned)
            return *this;

        /* free contents of this device, if any. They are about to replaced with
         * new values.
         */
        FreeDevice();

        /* set ID of this device to be the same as that being assigned */
        this->deviceID = deviceBeingAssigned.deviceID;

        /* no other fields have to be copied, they will all be loaded and
         * set on the fly when the member functions of the object are called
         * during normal usage, using the device's ID.
         */

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Resets the device.
    *
    * @return 1 if the device was successfully reset, else 0.
    *
    * \note This function will return an error if called on a virtual device.
    */
    int ResetDevice()
    {
        /* call standard 'c' API to reset the device */
        return STAR_resetDevice(this->deviceID);
    }

    /**
    * Gets the bus type of the device, e.g. USB, PCI, Virtual, etc.
    *
    * @return The bus type for the device.
    */
    BusType GetDeviceBusType()
    {
        /* call standard 'c' API to get the device's bus type */
        return (BusType)STAR_getDeviceBusType(this->deviceID);
    }

    /**
    * Gets a string representation of the device bus type.
    *
    * @param[out] buffer  A user supplied buffer of length
    *                     Device::STR_DEVICE_BUS_TYPE_LEN which will be
    *                     updated to contain the device bus type name, as a
    *                     null-terminated string.
    *
    * @return The length of the string read into the buffer or zero if failed
    *         to read the bus type as a string.
    */
    size_t GetDeviceBusTypeAsString(_Out_z_cap_c_(STR_DEVICE_BUS_TYPE_LEN)
                                    char *buffer)
    {
        /* if invalid buffer provided */
        if (!buffer)
        {
            /* return error */
            return 0;
        }

        /* set user-supplied buffer to an empty string */
        buffer[0] = '\0';

        /* call 'c' API to get the device's bus type as a string */
        char *name = STAR_getDeviceBusTypeAsString(this->deviceID);

        /* if no string obtained */
        if (!name)
        {
            /* return error / zero length string */
            return 0;
        }

        /* get the size of the string obtained */
        size_t len = strlen(name);

        /* if string has zero length */
        if (len == 0)
        {
            /* destroy the string obtained from the STAR API */
            STAR_destroyString(name);

            /* return error / zero length string */
            return 0;
        }

        /* if size exceeds the expected buffer size */
        if (len > (STR_DEVICE_BUS_TYPE_LEN-1))
        {
            /* adjust the length to be the maximum size (this will truncate
             * the string) */
            len = STR_DEVICE_BUS_TYPE_LEN-1;
        }

        /* copy the string obtained to the user-supplied buffer */
        for (unsigned int i=0; i<len; i++)
            buffer[i] = name[i];

        /* append with a NULL */
        buffer[len] = '\0';

        /* destroy the string obtained from the STAR API */
        STAR_destroyString(name);

        /* return the length of string obtained */
        return strlen(buffer);
    }

    /**
    * Gets the device index.
    *
    * @return The index number of the device, or -1 if the function call was
    *         unsuccessful.
    *
    * \note This function is provided for backwards compatibility reasons, not
    *       for typical usage.
    */
    int GetDeviceIndex()
    {
        /* call 'c' API to get the device's index */
        return STAR_getDeviceIndex(deviceID);
    }

    /**
    * Gets the device's type identifier.
    *
    * @return The type identifier for the device.
    */
    STAR_DEVICE_TYPE GetDeviceType()
    {
        /* call 'c' API to get the device's type */
        return STAR_getDeviceType(this->deviceID);
    }

    /**
    * Gets a string representation of the device's type.
    *
    * @param[out] buffer  A user supplied buffer of length
    *                     Device::STR_DEVICE_TYPE_LEN which will be
    *                     updated to contain the device type as a
    *                     null-terminated string.
    *
    * @return The length of the string read into the buffer or zero if failed
    *         to read the device type as a string.
    */
    size_t GetDeviceTypeAsString(_Out_z_cap_c_(STR_DEVICE_TYPE_LEN) char *buffer)
    {
        /* if invalid buffer provided */
        if (!buffer)
        {
            /* return error */
            return 0;
        }

        /* set user-supplied buffer to an empty string */
        buffer[0] = '\0';

        /* call 'c' API to get the device's type as a string */
        char *name = STAR_getDeviceTypeAsString(this->deviceID);

        /* if no string obtained */
        if (!name)
        {
            /* return error / zero length string */
            return 0;
        }

        /* get the size of the string obtained */
        size_t len = strlen(name);

        /* if string has zero length */
        if (len == 0)
        {
            /* destroy the string obtained from the STAR API */
            STAR_destroyString(name);

            /* return error / zero length string */
            return 0;
        }

        /* if size exceeds the expected buffer size */
        if (len > (STR_DEVICE_TYPE_LEN-1))
        {
            /* adjust the length to be the maximum size (this will truncate
             * the string) */
            len = STR_DEVICE_TYPE_LEN-1;
        }

        /* copy the string obtained to the user-supplied buffer */
        for (unsigned int i=0; i<len; i++)
            buffer[i] = name[i];

        /* append with a NULL */
        buffer[len] = '\0';

        /* destroy the string obtained from the STAR API */
        STAR_destroyString(name);

        /* return the length of string obtained */
        return strlen(buffer);
    }

    /**
    * Determine whether the device is capable of transmitting and receiving
    * packets on channels. This function can be used to determine whether a
    * device can be used to route packets over channels and in and out of the
    * device's SpaceWire links (#STAR_DEVICE_TXRX_SUPPORTED), or whether it is
    * a special device such as a Link Analyser or Conformance Tester
    * (#STAR_DEVICE_TXRX_NOT_SUPPORTED) which cannot route packets. Note that
    * some devices, such as the EGSE may be capable of transmitting and
    * receiving packets, but a value of #STAR_DEVICE_TXRX_NOT_SUPPORTED will be
    * returned, as these devices do not simply route any packets transmitted or
    * received on channels, and must be specially configured to transmit or
    * receive packets.
    *
    * @return A value indicating whether the device is capable of transmitting
    *         and receiving packets (#STAR_DEVICE_TXRX_SUPPORTED), not capable
    *         (#STAR_DEVICE_TXRX_NOT_SUPPORTED), or 0 if an error occurred.
    */
    STAR_DEVICE_TYPE GetDeviceTxRxCapabilities()
    {
        /* return device transmit and receive capabilities */
        return STAR_getDeviceTxRxCapabilities(this->deviceID);
    }

    /**
    * Determine whether the device is capable of being configured. This function
    * can be used to determine whether a device can be configured using the
    * Configuration APIs (#STAR_DEVICE_CONFIG_SUPPORTED), or whether it is a
    * special device such as a Link Analyser or Conformance Tester
    * (#STAR_DEVICE_CONFIG_NOT_SUPPORTED) which does not have a router with a
    * configuration port.
    *
    * @return A value indicating whether the device is capable of being
    *         configured (#STAR_DEVICE_CONFIG_SUPPORTED), not capable
    *         (#STAR_DEVICE_CONFIG_NOT_SUPPORTED), or 0 if an error occurred.
    */
    STAR_DEVICE_TYPE GetDeviceConfigCapabilities()
    {
        /* return device configuration capabilities */
        return STAR_getDeviceConfigCapabilities(this->deviceID);
    }

    /**
    * Gets the name of the device.
    *
    * @param[out] nameBuffer  A user supplied buffer of length
    *                         Device::STR_DEVICE_NAME_LEN which will be
    *                         updated to contain the device name as a
    *                         null-terminated string.
    *
    * @return The length of the string read into the buffer or zero if failed
    *         to read the device name.
    */
    size_t GetDeviceName(_Out_z_cap_c_(STR_DEVICE_NAME_LEN) char *nameBuffer)
    {
        /* if invalid buffer provided */
        if (!nameBuffer)
        {
            /* return error */
            return 0;
        }

        /* set user-supplied buffer to an empty string */
        nameBuffer[0] = '\0';

        /* call 'c' API to get the device's name */
        char *name = STAR_getDeviceName(this->deviceID);

        /* if no string obtained */
        if (!name)
        {
            /* return error / zero length string */
            return 0;
        }

        /* get the size of the string obtained */
        size_t len = strlen(name);

        /* if string has zero length */
        if (len == 0)
        {
            /* destroy the string obtained from the STAR API */
            STAR_destroyString(name);

            /* return error / zero length string */
            return 0;
        }

        /* if size exceeds the expected buffer size */
        if (len > (STR_DEVICE_NAME_LEN-1))
        {
            /* adjust the length to be the maximum size (this will truncate
             * the string) */
            len = STR_DEVICE_NAME_LEN-1;
        }

        /* copy the string obtained to the user-supplied buffer */
        for (unsigned int i=0; i<len; i++)
            nameBuffer[i] = name[i];

        /* append with a NULL */
        nameBuffer[len] = '\0';

        /* destroy the string obtained from the STAR API */
        STAR_destroyString(name);

        /* return the length of string obtained */
        return strlen(nameBuffer);
    }

    /**
    * Sets the name of the device.
    *
    * @param[in] newName A null-terminated string containing the device name.
    *
    * \note If the length of the string provided exceeds
    *       Device::STR_DEVICE_NAME_LEN it will be truncated.
    *
    * @return 1 if the device name has been set successfully,
    *           otherwise 0
    *
    */
    int SetDeviceName(_In_z_count_c_(STR_DEVICE_NAME_LEN) char* newName)
    {
        /* call standard 'c' API to set the device's name */
        return STAR_setDeviceName(this->deviceID, newName);
    }

    /**
    * Gets the channels on the device.
    *
    * @return Bitmask representing the channels on the device.
    */
    STAR_CHANNEL_MASK GetDeviceChannels()
    {
        /* call 'c' API to get the channels on the device */
        return STAR_getDeviceChannels(this->deviceID);
    }

    /**
    * Gets whether a given channel on the device is open.
    *
    * @param channelNumber The channel number to check.
    *
    * @return What sort of thing the channel is attached to (Not Attached /
    *         %Device / Application).
    */
    stardundee::com::starsystem::channels::Channel::ChannelType
    IsChannelOpen(unsigned int channelNumber)
    {
        /* call standard 'c' API to get whether the given channel on the
         * device is open.
         */
        return (stardundee::com::starsystem::channels::Channel::ChannelType)
               STAR_isChannelOpen(this->deviceID, channelNumber);
    }

    /**
    * Get the identifier of a device which is attached to a given channel on
    * the device.
    *
    * @param channelNumber The channel number to check.
    *
    * @return The identifier of the attached device, or 0 if no device is
    *         attached.
    */
    STAR_DEVICE_ID GetLocalDeviceAttachedToChannel(unsigned int channelNumber)
    {
        /* call standard 'c' API to get local device attached to channel */
        return STAR_getLocalDeviceAttachedToChannel(this->deviceID,
                                                    channelNumber);
    }

    /**
    * Gets the serial number (unique alphanumeric identifier) of the device.
    *
    * @param[out] buffer      A user supplied buffer of length
    *                         Device::STR_DEVICE_SERIAL_LEN which will be
    *                         updated to contain the device serial number as a
    *                         null-terminated string.
    *
    * @return The length of the string read into the buffer or zero if failed
    *         to read the device serial number.
    *
    */
    size_t GetDeviceSerialNumber(_Out_z_cap_c_(STR_DEVICE_SERIAL_LEN)char *buffer)
    {
        /* if invalid buffer provided */
        if (!buffer)
        {
            /* return error */
            return 0;
        }

        /* set user-supplied buffer to an empty string */
        buffer[0] = '\0';

        /* call 'c' API to get the device's serial number as a string */
        char *name = STAR_getDeviceSerialNumber(this->deviceID);

        /* if no string obtained */
        if (!name)
        {
            /* return error / zero length string */
            return 0;
        }

        /* get the size of the string obtained */
        size_t len = strlen(name);

        /* if string has zero length */
        if (len == 0)
        {
            /* destroy the string obtained from the STAR API */
            STAR_destroyString(name);

            /* return error / zero length string */
            return 0;
        }

        /* if size exceeds the expected buffer size */
        if (len > (STR_DEVICE_SERIAL_LEN-1))
        {
            /* adjust the length to be the maximum size (this will truncate
             * the string) */
            len = STR_DEVICE_SERIAL_LEN-1;
        }

        /* copy the string obtained to the user-supplied buffer */
        for (unsigned int i=0; i<len; i++)
            buffer[i] = name[i];

        /* append with a NULL */
        buffer[len] = '\0';

        /* destroy the string obtained from the STAR API */
        STAR_destroyString(name);

        /* return the length of string obtained */
        return strlen(buffer);
    }

    /**
    * Get the device's firmware version.
    *
    * @return A pointer to a VersionInformation object, or NULL if no version
    *         information was retreived OR if an error occurred.
    *
    * \note The object returned by this function must be freed when no longer
    *       required by calling the C++ delete operator.
    *
    */
    _Check_return_ VersionInformation* GetDeviceFirmwareVersion()
    {
        /* create empty pointer to VersionInformation */
        VersionInformation *versionInfo = NULL;

        /* request firmware version using the STAR API */
        STAR_VERSION_INFO *vInfo = STAR_getDeviceFirmwareVersion(this->deviceID);

        /* if version info received successfully */
        if (vInfo != NULL)
        {
            /* attempt to create new VersionInformation object */
            try
            {
                versionInfo = new VersionInformation(vInfo);
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);

                /* error allocating memory */

                /* ensure empty version information object */
                versionInfo = NULL;
            }

            /* free the memory struct created by the 'c' API */
            STAR_destroyVersionInfo(vInfo);
        }

        /* return pointer to the new version information object */
        return versionInfo;
    }

    /**
    * Open a channel on the device, on the specified channel number.
    *
    * @param direction Specifies whether the channel is for transmitting data
    *                  from this application, or receiving data into it, or
    *                  both. See note below also.
    * @param channelNumber The channel number on the device to attach to.
    * @param isQueued Specifies whether traffic received on this channel should
    *                 be buffered if there is no receive op waiting to receive
    *                 it.
    *
    * @return A \link stardundee::com::starsystem::channels::Channel Channel \endlink
    *         object representing the new channel opened.
    *
    * \note The \link stardundee::com::starsystem::channels::Channel::IsOpen() Channel::IsOpen() \endlink
    *       function must be called object which is returned to determine if the \link stardundee::com::starsystem::channels::Channel Channel \endlink
    *       was opened successfully or not and can therefore be used to
    *       transmit or receive traffic.
    *
    * \note There is no performance penalty in opening a channel in both
    *       directions when only receiving, for example. It simply stops the
    *       channel from being opened for transmitting in another process or
    *       thread.
    */
    stardundee::com::starsystem::channels::Channel
    OpenChannel(stardundee::com::starsystem::channels::Channel::ChannelDirection direction,
                unsigned char channelNumber, int isQueued)
    {
        /* call standard 'c' API to open channel to the device */
        STAR_CHANNEL_ID id =  STAR_openChannelToLocalDevice(this->deviceID,
                                         (STAR_CHANNEL_DIRECTION)direction,
                                         channelNumber, isQueued);

        /* return Channel object containing channel's ID */
        return stardundee::com::starsystem::channels::Channel(id, channelNumber);
    }

    /**
    * Open a channel between this device and another device on the specified
    * channel numbers.
    *
    * @param channelOnThisDevice The channel number on this device to attach to.
    * @param deviceB A pointer to the other device to attach to.
    * @param channelOnDeviceB The channel number on the other device to attach
    *                         to.
    *
    * @return A \link stardundee::com::starsystem::channels::Channel Channel \endlink
    *         object representing the new channel opened.
    *
    * \note The \link stardundee::com::starsystem::channels::Channel::IsOpen() Channel::IsOpen() \endlink
    *       function must be called on the Channel object which is returned to
    *       determine if the \link stardundee::com::starsystem::channels::Channel Channel \endlink
    *       was opened successfully or not and can therefore be used to
    *       transmit or receive traffic.
    *
    */
    _Check_return_ stardundee::com::starsystem::channels::Channel
    OpenChannelBetweenLocalDevices(unsigned char channelOnThisDevice,
                                   Device* deviceB,
                                   unsigned char channelOnDeviceB)
    {
        /* declare channel id, initially set to invalid / zero */
        STAR_CHANNEL_ID id = 0;

        /* if other device to attach to is valid */
        if (deviceB != NULL)
        {
            /* call standard 'c' API to open channel between local devices,
             * and to obtain the ID of that channel. */
            id = STAR_openChannelBetweenLocalDevices(this->deviceID,
                                                    channelOnThisDevice,
                                                    deviceB->GetDeviceID(),
                                                    channelOnDeviceB);
        }

        /* return Channel object containing channel's ID */
        return stardundee::com::starsystem::channels::Channel(id, channelOnThisDevice);
    }

    /**
    * Returns the ID of an application attached to a given channel on the
    * device.
    *
    * @param[in] channelNumber The channel number to check
    *
    * @return Requested applicationID, or 0 if no application is attached.
    *
    */
    STAR_APP_ID GetApplicationAttachedToChannel(unsigned int channelNumber)
    {
        return STAR_getApplicationAttachedToChannel(this->deviceID,
                                                    channelNumber);
    }

    /**
    * Gets whether this device is virtual or not.
    *
    * @return 1 if the device is virtual, otherwise 0.
    */
    int IsDeviceVirtual()
    {
        /* call 'c' API to determine if device is virtual */
        return STAR_isDeviceVirtual(this->deviceID);
    }

    /**
    * Get the device's driver.
    *
    * @param pDriverFactory A pointer to a DriverFactory object, used to
    *                       create instances of Driver objects.
    *
    * \note A DriverFactory object can be obtained by calling the following
    *       function STARSystem::GetDriverFactory. See code
    *       example below.
    *
    * @return A pointer to a new Driver object which identifies the driver for
    *         the device, or NULL if failed to locate or create driver object.
    *
    * \note The Driver object returned must be freed when no longer required
    *       by using the C++ delete operator.
    *
    * \verbatim
    *       Sample use:
    *
    *       // get driver factory
    *       DriverFactory *pDriverFactory = STARSystem::GetDriverFactory();
    *
    *       // if driver factory obtained
    *       if (pDriverFactory != NULL)
    *       {
    *           // get the driver for the device
    *           Driver *pDriver = myDevice->GetDeviceDriver(pDriverFactory);
    *
    *           // if driver obtained
    *           if (pDriver != NULL)
    *           {
    *               // call driver functions..
    *
    *               // free driver object when no longer required
    *               delete pDriver;
    *           }
    *
    *           // free driver factory object when no longer required
    *           delete pDriverFactory;
    *       }
    * \endverbatim
    *
    */
    Driver* GetDeviceDriver(DriverFactory *pDriverFactory)
    {
        /* if no valid driver factory provided */
        if (!pDriverFactory)
        {
            /* cannot create instance of driver, return error / NULL */
            return NULL;
        }

        /* call 'c' API to get the id of the driver associated with this
         * device */
        STAR_DRIVER_ID driverID = STAR_getDeviceDriver(this->deviceID);

        /* if valid driver ID obtained */
        if (driverID != 0)
        {
            /* create a new Driver */
            Driver *pDriver = pDriverFactory->CreateDriver(driverID);

            /* return instance of a new Driver object for the given driver
             * id (Note: may be NULL if driver factory failed to create) */
            return pDriver;
        }
        else
        {
            /* no valid driver ID obtained, return NULL */
            return NULL;
        }
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
    *       class (\link stardundee::com::starsystem::deviceconfig::DeviceConfiguration DeviceConfiguration\endlink)
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
    stardundee::com::starsystem::deviceconfig::DeviceConfiguration
    *GetDeviceConfiguration()
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
            case 21:
            case 22:
            case 24:
            case 29:
            case 30:
            case 31:
                /* a PXI or PXI Mk2 device */
                pDeviceConfiguration = new stardundee::com::starsystem::deviceconfig::mk2devices::pxi::PXIDeviceConfiguration(this->deviceID);
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
     * Create and return an instance of a Device object which
     * is a copy of 'this' object.
     *
     * @return A pointer to a new Device object, or NULL if failed to
     *         allocate memory for the object.
     *
     * \note The object returned must be freed when no longer required by
     *       calling the C++ delete operator.
     *
     */
    virtual STARDevice *CopyDevice()
    {
        /* a pointer to the clone device, initially null */
        Device *pCloneDevice = NULL;

        /* create an instance of new a Device */
        try
        {
            pCloneDevice = new Device();
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

