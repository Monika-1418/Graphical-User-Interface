/**
 * \file port.hpp
 *
 * \brief Base class for ports on a device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Base class for ports on a device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_PORT_BASE_CPP__
#define __DEVICE_PORT_BASE_CPP__

#include "cfg_api_router_types.h"
#include "cfg_api_router.h"
#include "port_errors.hpp"
#include "port_status.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Base class for ports on a device.
*/
class Port
{
public:
    /** Types of ports on routing devices. */
    enum PortType
    {
        /** configuration port */
        PORT_TYPE_CONFIGURATION = STAR_CFG_PORT_TYPE_CONFIGURATION,
        /** SpaceWire link port */
        PORT_TYPE_LINK = STAR_CFG_PORT_TYPE_LINK,
        /** external port */
        PORT_TYPE_EXTERNAL = STAR_CFG_PORT_TYPE_EXTERNAL,
        /** invalid value */
        PORT_TYPE_INVALID = STAR_CFG_PORT_TYPE_INVALID
    };

protected:
    /** the ID of device which owns the port */
    STAR_DEVICE_ID owningDeviceForPort;

    /** the type of port */
    STAR_CFG_PORT_TYPE portType;

    /** the port number */
    U8 portNumber;

    /** the value of the port's status control register */
    PORT_STATUS_CONTROL portStatusControl;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    Port() : owningDeviceForPort(0), portType(STAR_CFG_PORT_TYPE_INVALID),
             portNumber(0), portStatusControl(0)
    {
    }

    /**
    * Overloaded constructor. Initialise object to given value(s).
    *
    * @param idOfOwningDevice The ID of the device which this port forms a part
    *                         of.
    * @param type The type of port.
    * @param portNum The port number.
    * @param statusControl The port's status / control register value.
    *
    */
    Port(STAR_DEVICE_ID idOfOwningDevice, STAR_CFG_PORT_TYPE type, U8 portNum,
         PORT_STATUS_CONTROL statusControl)
         : owningDeviceForPort(idOfOwningDevice), portType(type),
             portNumber(portNum), portStatusControl(statusControl)
    {
    }

    /**
    * Copy constructor - to allow copies of Port objects to be made when being
    * passed by value in functions or returned by value from functions.
    *
    */
    Port(const Port& portToCopyFrom)
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of Port objects to
    * have their contents assigned to each other.
    *
    */
    Port& operator=(const Port& portBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &portBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        owningDeviceForPort = portBeingAssigned.owningDeviceForPort;
        portType = portBeingAssigned.portType;
        portNumber = portBeingAssigned.portNumber;
        portStatusControl = portBeingAssigned.portStatusControl;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    virtual ~Port()
    {
        /* no resources to free at present */
    }

    /**
    * Clear all errors on the port.
    *
    * \note Errors on a port are latched until cleared with this function.
    *
    * @return 1 if the port errors were successfully cleared, otherwise 0.
    *
    */
    int ClearErrors()
    {
        /* call 'c' API to clear errors on this port, based on the ID of the
         * device which owns it and the port's number */
        return CFG_ROUTER_clearPortErrors(owningDeviceForPort, portNumber);
    }

    /**
    * Get the port type.
    *
    * @return The port's type.
    */
    PortType GetType()
    {
        /* return the port's type */
        return (PortType)portType;
    }

    /**
    * Get the port's number
    *
    * @return The port number.
    *
    */
    U8 GetPortNumber()
    {
        return portNumber;
    }

    /**
    * Identifies the output port to which this port (the source port) is
    * currently connected to whilst routing is in operation.
    *
    * @return The number of the output port connected to.
    *
    * \note The port number returned can be provided to the \link stardundee::com::starsystem::deviceconfig::DeviceConfiguration::GetPort() DeviceConfiguration::GetPort()\endlink
    *       method on the DeviceConfiguration of the Device which owns the port
    *       in order to obtain an instance of a Port object (to access the
    *       Port's properties).
    *
    */
    U8 GetPortConnection()
    {
        /* call 'c' API to get port number of connected output port */
        return CFG_ROUTER_getPortConnection(portStatusControl);
    }

    /**
    * Get the value of the port's status control register.
    *
    * \note Within the C++ API you do not usually need to directly access the
    *       value of a port's status control register. This method is provided
    *       to support other functions within the API itself.
    *
    * @return The value of the port's status control register.
    *
    */
    PORT_STATUS_CONTROL GetPortStatusControl()
    {
        return portStatusControl;
    }

    /**
    * Get the id of the device which 'owns' the port.
    *
    * \note Within the C++ API you do not usually need to directly access a
    *       device's ID. This method is provided to support other functions
    *       functions within the API itself.
    *
    * @return The ID of the device which owns the port.
    *
    */
    STAR_DEVICE_ID GetIDOfOwningDevice()
    {
        return owningDeviceForPort;
    }

    /**
    * Get information about any errors present on the Port.
    *
    * @return An instance of a PortErrors object containing the error
    *         information, or NULL if failed to obtain error information OR
    *         if error information isn't available for this type of port.
    *
    * \note The object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    */
    virtual PortErrors* GetErrors() = 0;

    /**
    * Get the status information for the Port.
    *
    * @return An instance of a PortStatus object containing the status
    *         information, or NULL if failed to obtain status information OR
    *         if status information isn't available for this type of port.
    *
    * \note Currently, status information can only be obtained for LinkPort
    *       and ExternalPort.
    *
    * \note The object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    */
    virtual PortStatus* GetStatus() = 0;
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

