/**
 * \file config_port.hpp
 *
 * \brief Represents a Configuration Port on a device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a Configuration Port on a device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_CONFIG_PORT_CPP__
#define __DEVICE_CONFIG_PORT_CPP__

#include "port.hpp"
#include "config_port_errors.hpp"
#include <new>
#include "cpp_api_macros.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Represents a Configuration Port on a device.
*/
class ConfigPort : public Port
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    ConfigPort() : Port()
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
    ConfigPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl) : Port(idOfOwningDevice, type,
                                                     portNum, statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of ConfigPort objects to be made when
    * being passed by value in functions or returned by value from functions.
    *
    */
    ConfigPort(const ConfigPort& portToCopyFrom) : Port()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of ConfigPort objects
    * to have their contents assigned to each other.
    *
    */
    ConfigPort& operator=(const ConfigPort& portBeingAssigned)
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
    virtual ~ConfigPort()
    {
        /* no resources to free at present */
    }

    /**
    * Gets any configuration errors present on the port. These are errors that
    * arise when malformed or invalid configuration commands are sent to the
    * configuration port.
    *
    * \note Errors on the configuration port are latched until cleared with the
    *       ClearErrors() method.
    *
    * @return A pointer to a new ConfigPortErrors object which contains various
    *         fields to indicate whether different types of errors have
    *         occurred, or NULL if failed to allocate memory for the object OR
    *         if an error occurred retreiving the error information for the
    *         port.
    *
    * \note The object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    virtual PortErrors* GetErrors()
    {
        /* declare struct required by 'c' API to hold config port errors */
        STAR_CFG_CONFIG_PORT_ERRORS configPortErrors;

        /* call 'c' API to get any config errors */
        int result = CFG_ROUTER_getConfigPortErrors(portStatusControl,
                                                    &configPortErrors);

        /* if port status control provided doesn't correspond to a config port */
        if (result == 0)
        {
            /* return error / NULL */
            return NULL;
        }

        /* create and return a new ConfigPortErrors object with the error info */
        ConfigPortErrors *pErrors = NULL;
        try
        {
            pErrors = new ConfigPortErrors(configPortErrors);
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory, return error */
            return NULL;
        }

        return pErrors;
    }

    /**
    * Get the status information for the Port.
    *
    * @return An instance of a new PortStatus object containing the status
    *         information, or NULL if failed to obtain status information OR
    *         if status information isn't available for this type of port.
    *
    * \note The object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    */
    virtual PortStatus* GetStatus()
    {
        /* status information currently isn't provided for Config ports,
         * return NULL */
        return NULL;
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

