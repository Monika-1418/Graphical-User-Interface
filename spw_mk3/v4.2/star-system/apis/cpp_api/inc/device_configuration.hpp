/**
 * \file device_configuration.hpp
 *
 * \brief Base class to represent the configuration state of a device and which
 *        can be used to query or set a device's configuration state.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Base class to represent the configuration state of a device and which can be
 * used to query or set a device's configuration state.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __STAR_DEVICE_CONFIGURATION_STATE_CPP__
#define __STAR_DEVICE_CONFIGURATION_STATE_CPP__

#include <new>
#include "star-dundee_annotations.h"
#include "star-api.h"
#include "device_identifier_info.hpp"
#include "network_discovery_info.hpp"
#include "config_port.hpp"
#include "link_port.hpp"
#include "external_port.hpp"
#include "routing_table_entry.hpp"
#include "router_global_state.hpp"
#include "cpp_api_macros.hpp"

namespace stardundee
{
namespace com
{
namespace starsystem
{
/**
 * Classes required to perform general device configuration tasks with any
 * type of device.
 **/
namespace deviceconfig
{

/**
* Base class to represent the configuration state of a device and which can be
* used to query or set a device's configuration state.
*/
class DeviceConfiguration
{
protected:
    /** the ID of the device which the configuration state relates to */
    STAR_DEVICE_ID idOfOwningDevice;

    /**
    * Create and return an instance of a Port object which corresponds to the
    * given port number on the given device (identified by its STAR_DEVICE_ID).
    *
    * @param deviceID The ID of device containing the port.
    * @param portNumber The number of the port on the device to create an
    *                   instance of a Port object for.
    *
    * @return A pointer to a new Port object, or NULL if error occurred, e.g.
    *         memory allocation error or failure to locate valid port.
    *
    * \note The object returned by this function must be freed when no longer
    *       required by using the C++ delete operator.
    *
    */
    Port* CreatePort(STAR_DEVICE_ID deviceID, U8 portNumber)
    {
        PORT_STATUS_CONTROL portStatusControl;
        Port *thePort = NULL;

        /* request the port status control value for the given device and port
         * number */
        int result = CFG_ROUTER_getPortStatusControl(deviceID, portNumber,
                                                     &portStatusControl);

        /* if failed to obtain port's status / control value */
        if (result == 0)
        {
            /* return error / NULL */
            return NULL;
        }

        /* get the port's type */
        STAR_CFG_PORT_TYPE portType = CFG_ROUTER_getPortType(portStatusControl);

        switch (portType)
        {
        case STAR_CFG_PORT_TYPE_CONFIGURATION:
            /* create a config port */
            thePort = CreateConfigPort(deviceID, portType, portNumber,
                                       portStatusControl);
            break;
        case STAR_CFG_PORT_TYPE_LINK:
            /* create a link port */
            thePort = CreateLinkPort(deviceID, portType, portNumber,
                                     portStatusControl);
            break;
        case STAR_CFG_PORT_TYPE_EXTERNAL:
            /* create an external port */
            thePort = CreateExternalPort(deviceID, portType, portNumber,
                                          portStatusControl);
            break;
        default:
            /* invalid or unrecognised port */
            break;
        }

        /* return the Port object created */
        return thePort;
    }

    /**
    * Create an instance of a ConfigPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a ConfigPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateConfigPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        ConfigPort *configPort = NULL;

        try
        {
            configPort = new ConfigPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the ConfigPort object*/
            configPort = NULL;
        }

        /* return new ConfigPort object, or NULL if failed to allocate */
        return (Port*)configPort;
    }

    /**
    * Create an instance of a LinkPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a LinkPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateLinkPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        LinkPort *linkPort = NULL;

        try
        {
            linkPort = new LinkPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the LinkPort object*/
            linkPort = NULL;
        }

        /* return new LinkPort object, or NULL if failed to allocate */
        return (Port*)linkPort;
    }

    /**
    * Create an instance of an ExternalPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of an ExternalPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateExternalPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        ExternalPort *externalPort = NULL;

        try
        {
            externalPort = new ExternalPort(id, type, portNumber,
                                            portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the ExternalPort object*/
            externalPort = NULL;
        }

        /* return new ExternalPort object, or NULL if failed to allocate */
        return externalPort;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    DeviceConfiguration() : idOfOwningDevice(0)
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this configuration state is
    *                 associated with and describes.
    */
    explicit DeviceConfiguration(STAR_DEVICE_ID deviceID) :
        idOfOwningDevice(deviceID)
    {
    }

    /**
    * Copy constructor - to allow copies of DeviceConfiguration objects to
    * be made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    DeviceConfiguration(const DeviceConfiguration&
                                   deviceConfigStateToCopyFrom)
    {
        /* copy required contents of object being copied into this object */
        idOfOwningDevice = deviceConfigStateToCopyFrom.idOfOwningDevice;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * DeviceConfiguration objects to have their contents
    * assigned to each other.
    *
    */
    DeviceConfiguration&
    operator=(const DeviceConfiguration& deviceConfigStateBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &deviceConfigStateBeingAssigned)
            return *this;

        /* copy required contents of object being copied into this object */
        idOfOwningDevice = deviceConfigStateBeingAssigned.idOfOwningDevice;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~DeviceConfiguration()
    {
    }

    /**
    * Get the device identifier information for the device, e.g. manufacturer
    * ID, chip type and version.
    *
    * @return A pointer to a new DeviceIdentifierInfo object which contains the
    *         device identifier information, or NULL if error.
    *
    * \note The object returned by this function must be freed when no longer
    *       required using the C++ delete operator.
    */
    DeviceIdentifierInfo *GetDeviceIdentificationInfo()
    {
        /* declare struct required by 'c' API to hold device identifier info */
        STAR_CFG_DEVICE_IDENTIFIER_INFO deviceInfo;

        /* call 'c' API to get identifier info for this device */
        int result = CFG_ROUTER_getDeviceIdentificationInfo(idOfOwningDevice,
                                                            &deviceInfo);

        /* if failed to read device identifier info */
        if (result == 0)
        {
            /* return error / null */
            return NULL;
        }
        else
        {
            /* create and return a new DeviceIdentifierInfo object which contains
             * the device identifier information read */
            DeviceIdentifierInfo *deviceInfoObj = new DeviceIdentifierInfo(deviceInfo);
            return deviceInfoObj;
        }
    }

    /**
    * Get the network discovery information for the device.
    *
    * This information can be used by a network manager to determine the layout
    * of the network.
    *
    * @return A pointer to a new NetworkDiscoveryInfo object which contains the
    *         network discovery information, or NULL if error.
    *
    * \note This object holds a snapshot of the network discovery information
    *       at the time requested. If there have been changes in the network
    *       since, e.g. changes in the number of running ports due to links
    *       being started or stopped, a fresh copy of the network discovery
    *       information needs to be requested. Alternatively, the \link stardundee::com::starsystem::deviceconfig::mk2devices::NetworkDiscoveryInfo::Refresh() NetworkDiscoveryInfo::Refresh() \endlink
    *       method can be called on an existing instance of the
    *       NetworkDiscoveryInfo object to update its contents with the latest
    *       network discovery information.
    *
    * \note The object returned by this function must be freed when no longer
    *       required using the C++ delete operator.
    */
    NetworkDiscoveryInfo *GetNetworkDiscoveryInfo()
    {
        /* declare struct required by 'c' API to hold network discovery info */
        STAR_CFG_NETWORK_DISCOVERY_INFO networkInfo;

        /* call 'c' API to get network discovery info for this device */
        int result = CFG_ROUTER_getNetworkDiscoveryInfo(idOfOwningDevice,
                                                        &networkInfo);

        /* if failed to read device identifier info */
        if (result == 0)
        {
            /* return error / null */
            return NULL;
        }
        else
        {
            /* create and return a new NetworkDiscoveryInfo object which contains
             * the network discovery information read */
            NetworkDiscoveryInfo *networkInfoObj =
                                  new NetworkDiscoveryInfo(idOfOwningDevice,
                                                           networkInfo);
            return networkInfoObj;
        }
    }

    /**
    * Get the number of ports on the device
    *
    * @return The number of ports on the device, inclusive of external ports,
    *         link ports and the configuration port.
    *
    */
    int GetPortCount()
    {
        /* set port count to zero */
        int portCount = 0;

        /* get the network discovery information for the device */
        NetworkDiscoveryInfo *pNetworkInfo = GetNetworkDiscoveryInfo();

        /* if network discovery info found */
        if (pNetworkInfo != NULL)
        {
            /* get the port count from the network discovery info */
            portCount = pNetworkInfo->GetPortCount();

            /* delete the network discovery object */
            delete pNetworkInfo;
        }

        /* return port count */
        return portCount;
    }

    /**
    * Get the port on the device corresponding to the given port number.
    *
    * @param portNumber The port number of the port to obtain.
    *
    * @return A pointer to a new Port object, representing the port at the
    *         given port number, or NULL if failed to obtain or create the
    *         port object.
    *
    * \note The object returned by this function must be freed when no longer
    *       required using the C++ delete operator.
    *
    * \note A hierarchy of Port objects exists to represent the different types
    *       of port available on a device (such as config, link and external
    *       ports) and on different types of devices (such as Mk2 devices, PCI
    *       Mk2 devices, Brick Mk2 devices, and so on). Depending on the type
    *       of port and type of device different properties and functions
    *       may be available to query and/or set the status of the port.
    *       Whereas the parent class (Port) will provide access to the general
    *       properties of any port on any device, the child classes extend these
    *       to include the additional properties which specific to the types of
    *       port and the types of device they are on. The Port object which is
    *       returned from this function can be cast to a an instance of the
    *       relevant child class to access these more specific properties where
    *       required. For example, cast to a \link stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2LinkPort BrickMk2LinkPort\endlink
    *       if we know the Port represents a link port on a Brick Mk2 device,
    *       or cast it to an instance of an ExternalPort object if it is known
    *       to be an external port (on any type of device) or cast it to an
    *       instance of a \link stardundee::com::starsystem::deviceconfig::mk2devices::Mk2ExternalPort Mk2ExternalPort \endlink
    *       if it is known to be an external port on a Mk2 device, and so on.
    */
    Port* GetPort(U8 portNumber)
    {
        /* create a Port object for the requested port */
        Port *thePort = CreatePort(idOfOwningDevice, portNumber);

        /* return the port (may be NULL if error occured) */
        return thePort;
    }

    /**
    * Gets the output port to which the given (source) port is currently
    * connected to whilst routing is in operation.
    *
    * @param pSourcePort A pointer to a Port object which represents the
    *                    source port.
    *
    * @return A pointer to a Port object which represents the output port to
    *         which this port is connected or NULL if there is no connected port
    *         available or if a memory allocation failure occured.
    *
    * \note The object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    * \note A hierarchy of Port objects exists to represent the different types
    *       of port available on a device (such as config, link and external
    *       ports) and on different types of devices (such as Mk2 devices, PCI
    *       Mk2 devices, Brick Mk2 devices, and so on). Depending on the type
    *       of port and type of device different properties and functions
    *       may be available to query and/or set the status of the port.
    *       Whereas the parent class (Port) will provide access to the general
    *       properties of any port on any device, the child classes extend these
    *       to include the additional properties which specific to the types of
    *       port and the types of device they are on. The Port object which is
    *       returned from this function can be cast to a an instance of the
    *       relevant child class to access these more specific properties where
    *       required. For example, cast to a \link stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2LinkPort BrickMk2LinkPort\endlink
    *       if we know the Port represents a link port on a Brick Mk2 device,
    *       or cast it to an instance of an ExternalPort object if it is known
    *       to be an external port (on any type of device) or cast it to an
    *       instance of a \link stardundee::com::starsystem::deviceconfig::mk2devices::Mk2ExternalPort Mk2ExternalPort \endlink
    *       if it is known to be an external port on a Mk2 device, and so on.
    *
    */
    Port *GetPortConnection(Port *pSourcePort)
    {
        /* if invalid source port */
        if (!pSourcePort)
        {
            /* return error / NULL */
            return NULL;
        }

        /* if source port type is invalid and/or has an invalid owner device */
        if ((pSourcePort->GetType() == (Port::PortType)STAR_CFG_PORT_TYPE_INVALID) ||
            (pSourcePort->GetIDOfOwningDevice() == 0))
        {
            /* attempting to access properties on an invalid or
             * un-initialised port */
            return NULL;
        }

        /* call 'c' API to get port number of connected output port */
        U8 portNum = CFG_ROUTER_getPortConnection(
                                         pSourcePort->GetPortStatusControl());

        /* if port isn't valid (a value of '31' indicates no connected port
         * found) */
        if (portNum == 31)
        {
            /* return no port available / NULL */
            return NULL;
        }

        /* create and return a Port object for the connected port */
        return CreatePort(idOfOwningDevice, portNum);
    }

    /**
    * Starts a given SpaceWire link on the device by setting the start bit
    * and clearing the disable bit.
    *
    * @param linkNum The link to start.
    *
    * @return 1 if the link was successfully started, otherwise 0.
    *
    * \note This is equivalent to calling the LinkPort::StartLink() method,
    *       but without having to obtain an instance of a LinkPort object
    *       for the relevant link port first, instead addressing it by the
    *       link / port number only.
    *
    *
    */
    int StartLink(U8 linkNum)
    {
        /* call 'c' API to start the given link */
        return CFG_ROUTER_startLink(idOfOwningDevice, linkNum);
    }

    /**
    * Stops a given SpaceWire link on the device by clearing the start bit and
    * setting the disable bit.
    *
    * \note If auto-start is enabled the link will start again as soon as it
    *       receives NULLs.
    *
    * @param linkNum The link to stop.
    *
    * @return 1 if the link was successfully stopped, otherwise 0.
    *
    * \note This is equivalent to calling the LinkPort::StopLink() method,
    *       but without having to obtain an instance of a LinkPort object
    *       for the relevant link port first, instead addressing it by the
    *       link / port number only.
    *
    */
    int StopLink(U8 linkNum)
    {
        /* call 'c' API to stop the given link */
        return CFG_ROUTER_stopLink(idOfOwningDevice, linkNum);
    }

    /**
    * Get the routing table entry for the given logical address.
    *
    * @param logicalAddress The logical address to obtain the routing table
    *                       entry for.
    *
    * @return A pointer to a new RoutingTableEntry object, containing the
    *         details of routing table entry for the given logical address, or
    *         NULL if no entry found or if a memory allocation failure occurred.
    *
    * \note The object returned must be freed when no longer required by using
    *       the C++ delete operator.
    *
    */
    RoutingTableEntry* GetRoutingTableEntry(U8 logicalAddress)
    {
        /* declare struct required by 'c' API to hold routing table entry */
        STAR_CFG_GAR_ENTRY tableEntry;

        /* call 'c' API to get the routing table entry for the given logical
         * address on the device associated with this router configuration
         * state object. */
        int result = CFG_ROUTER_getRoutingTableEntry(idOfOwningDevice,
                                                     logicalAddress,
                                                     &tableEntry);

        /* if failed to get routing table entry */
        if (result == 0)
        {
            /* return error / null */
            return NULL;
        }
        else
        {
            /* create a new RoutingTableEntry object  */
            RoutingTableEntry *newRoutingTableEntry = NULL;
            try
            {
                newRoutingTableEntry = new RoutingTableEntry(tableEntry);
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory allocation failure */
                newRoutingTableEntry = NULL;
            }

            /* return the routing table entry (or NULL, if error allocating) */
            return newRoutingTableEntry;
        }
    }

    /**
    * Set the routing table entry for the given logical address.
    *
    * @param logicalAddress The logical address to set the table entry for.
    *                       Valid values are 32 through 255.
    * \note Logical address 255 is reserved for future use and should not be
    *       used. See Section 10.3.3n of the SpaceWire standard for more
    *       information.
    * @param routingTableEntry A reference to a RoutingTableEntry pobject which
    *                          contains the values to set for the routing table
    *                          entry for the given logical address.
    *
    * @return 1 if routing table entry successfully set, otherwise 0.
    *
    */
    int SetRoutingTableEntry(U8 logicalAddress,
                            RoutingTableEntry& routingTableEntry)
    {
        /* get the STAR_CFG_GAR_ENTRY struct from the RoutingTableEntry object */
        STAR_CFG_GAR_ENTRY *pTableEntryStruct =
                                    routingTableEntry.GetAsSTAR_CFG_GAR_ENTRY();

        /* call 'c' API to set the routing table entry */
        return CFG_ROUTER_setRoutingTableEntry(idOfOwningDevice, logicalAddress,
                                               *pTableEntryStruct);
    }

    /**
    * Sets the value of the network identity register.
    *
    * This is a 32 bit value that can be used to identify the device, typically
    * set by a network manager. It may also be used for any other purpose. This
    * value has no effect on the operation of the device.
    *
    * @param value The value to set the network identity to.
    *
    * @return 1 if the register was successfully set, else 0.
    *
    */
    int SetNetworkIdentity(REGISTER value)
    {
        /* call 'c' API to set the network identity */
        return CFG_ROUTER_setNetworkIdentity(idOfOwningDevice, value);
    }

    /**
    * Get the value of the network identity register.
    *
    * This is a 32 bit value that can be used to identify the device, typically
    * set by a network manager. It may also be used for any other purpose. This
    * value has no effect on the operation of the device.
    *
    * @param[out] value Value of the register, set by this function.
    *
    * @return 1 if the register's value was successfully obtained, otherwise 0.
    *
    */
    int GetNetworkIdentity(_Out_ REGISTER *value)
    {
        /* call 'c' API to get the network identity */
        return CFG_ROUTER_getNetworkIdentity(idOfOwningDevice, value);
    }

    /**
    * Sets the value of the general purpose register.
    *
    * This is a user defined 32 bit value that can be set by the user as required
    * for their purposes. This value has no effect on the operation of the
    * device.
    *
    * @param value The value to set the general purpose register to.
    *
    * @return 1 if the register was successfully set, else 0.
    *
    */
    int SetGeneralPurpose(REGISTER value)
    {
        /* call 'c' API to set the register value */
        return CFG_ROUTER_setGeneralPurpose(idOfOwningDevice, value);
    }

    /**
    * Get the value of the general purpose register.
    *
    * This is a user defined 32 bit value that can be set by the user as required
    * for their purposes. This value has no effect on the operation of the device.
    *
    * @param[out] value Value of the register, set by this function.
    *
    * @return 1 if the register's value was successfully obtained, otherwise 0.
    *
    */
    int GetGeneralPurpose(_Out_ REGISTER *value)
    {
        /* call 'c' API to get the register's value */
        return CFG_ROUTER_getGeneralPurpose(idOfOwningDevice, value);
    }

    /**
    * Get which output ports time-codes are forwarded on.
    *
    * @param portMask A bitmask which specifies the ports that time-code
    *                 distribution is enabled on, and which will be set by this
    *                 function.
    *
    * \note Bit 1 corresponds to port 1.
    *
    * @return 1 if the port mask was successfully obtained, otherwise 0.
    *
    */
    int GetTimeCodeDistributionPorts(_Out_ U32 *portMask)
    {
        /* call 'c' API to get the time-code distribution ports */
        return CFG_ROUTER_getTimeCodeDistributionPorts(idOfOwningDevice,
                                                        portMask);
    }

    /**
    * Set which output ports time-codes are forwarded on.
    *
    * @param portMask A bitmask of the ports that time-code distribution should
    *                 be enabled on. Valid ports to forward time-codes on are
    *                 1 through 11.
    *
    * \note Bit 1 corresponds to port 1.
    *       Not all external ports allow time-code forwarding. Check your
    *       device's user manual for more details.
    *
    * @return 1 if the ports are successfully set, otherwise 0.
    *
    */
    int SetTimeCodeDistributionPorts(U32 portMask)
    {
        /* call 'c' API to set the time-code distribution ports */
        return CFG_ROUTER_setTimeCodeDistributionPorts(idOfOwningDevice,
                                                        portMask);
    }

    /**
    * Get the time-code flag interpretation mode:
    *
    * 0 - Time-code control bit flags are distributed with valid time-code
    * values regardless of the value of the time-code control flags.
    *
    * 1 - When the time-code control bit flags are "00" then valid time-codes
    * are distributed. When the time-code control flags are not "00" then the
    * time-code is discarded and the internal time-code register is not updated.
    *
    * @param[out] mode The time-code flag mode, set by this function.
    *
    * @return 1 if time-code flag mode successfully obtained, otherwise 0.
    *
    */
    int GetTimeCodeFlagMode(_Out_ U8 *mode)
    {
        /* call 'c' API */
        return CFG_ROUTER_getTimeCodeFlagMode(idOfOwningDevice, mode);
    }

    /**
    * Set the time-code flag interpretation mode:
    *
    * 0 - Time-code control bit flags are distributed with valid time-code
    * values regardless of the value of the time-code control flags.
    *
    * 1 - When the time-code control bit flags are "00" then valid time-codes
    * are distributed. When the time-code control flags are not "00" then the
    * time-code is discarded and the internal time-code register is not updated.
    *
    * @param mode The time-code flag mode to set.
    *
    * @return 1 if time-code flag mode successfully set, otherwise 0.
    *
    */
    int SetTimeCodeFlagMode(U8 mode)
    {
        /* call 'c' API */
        return CFG_ROUTER_setTimeCodeFlagMode(idOfOwningDevice, mode);
    }

    /**
    * Get the current value of a router's internal time-code counter.
    *
    * @param[out] value The time-code value obtained.
    *
    * @return 1 if time-code value successfully obtained, otherwise 0.
    *
    */
    int GetTimeCodeValue(_Out_ U8 *value)
    {
        /* call 'c' API */
        return CFG_ROUTER_getTimeCodeValue(idOfOwningDevice, value);
    }

    /**
    * Get the global settings for a router.
    *
    * @return A pointer to a new RouterGlobalState object which provides the
    *         router's global settings, or NULL if failed to obtain global
    *         settings or if a memory allocation failure occurred.
    *
    * \note The object returned must be freed when no longer required by using
    *       the C++ delete operator.
    *
    */
    RouterGlobalState* GetGlobalSettings()
    {
        /* declare struct required by 'c' API to hold router's global state */
        STAR_CFG_ROUTER_GLOBAL_STATE routerState;

        /* call 'c' API to get the router's global settings */
        int result = CFG_ROUTER_getRouterGlobalSettings(idOfOwningDevice,
                                                        &routerState);

        /* if failed to get router's global state */
        if (result == 0)
        {
            /* return error / null */
            return NULL;
        }
        else
        {
            /* create a new RouterGlobalState object  */
            RouterGlobalState *routerGlobalState = NULL;
            try
            {
                routerGlobalState = new RouterGlobalState(routerState);
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory allocation failure */
                routerGlobalState = NULL;
            }

            /* return the routing table entry (or NULL, if error allocating) */
            return routerGlobalState;
        }
    }

    /**
    * Set the global settings for a router.
    *
    * @param routerGlobalState A reference to a RouterGlobalState object which
    *                          contains the values to set for the router's
    *                          global settings.
    *
    * @return 1 if global settings successfully set, otherwise 0.
    *
    */
    int SetGlobalSettings(RouterGlobalState& routerGlobalState)
    {
        /* get the STAR_CFG_ROUTER_GLOBAL_STATE struct from the
         * RouterGlobalState object. */
        STAR_CFG_ROUTER_GLOBAL_STATE *pStateStruct =
                        routerGlobalState.GetAsSTAR_CFG_ROUTER_GLOBAL_STATE();

        /* call 'c' API to set the global settings */
        return CFG_ROUTER_setRouterGlobalSettings(idOfOwningDevice,
                                                  *pStateStruct);
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

