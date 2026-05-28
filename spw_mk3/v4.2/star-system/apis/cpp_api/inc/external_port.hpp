/**
 * \file external_port.hpp
 *
 * \brief Represents an External Port on a device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents an External Port on a device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_EXTERNAL_PORT_CPP__
#define __DEVICE_EXTERNAL_PORT_CPP__

#include "port.hpp"
#include "external_port_errors.hpp"
#include "external_port_status.hpp"
#include <new>
#include "cpp_api_macros.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Represents an External Port on a device.
*/
class ExternalPort : public Port
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    ExternalPort() : Port()
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
    ExternalPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl) : Port(idOfOwningDevice, type,
                                                     portNum, statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of ExternalPort objects to be made when
    * being passed by value in functions or returned by value from functions.
    *
    */
    ExternalPort(const ExternalPort& portToCopyFrom) : Port()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of ExternalPort
    * objects to have their contents assigned to each other.
    *
    */
    ExternalPort& operator=(const ExternalPort& portBeingAssigned)
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
    virtual ~ExternalPort()
    {
        /* no resources to free at present */
    }

    /**
    * Gets any external port errors present on the port.
    *
    * \note Errors on a port are latched until cleared with the ClearErrors()
    *       method.
    *
    * @return A pointer to a new ExternalPortErrors object which contains various
    *         fields to indicate whether different types of errors have
    *         occurred, or NULL if failed to allocate memory for the object OR
    *         if an error occurred retrieving the error information for the
    *         port.
    *
    * \note The object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    virtual PortErrors* GetErrors()
    {
        /* declare struct required by 'c' API to hold external port errors */
        STAR_CFG_EXTERNAL_PORT_ERRORS externalPortErrors;

        /* call 'c' API to get any external port errors */
        int result = CFG_ROUTER_getExternalPortErrors(portStatusControl,
                                                      &externalPortErrors);

        /* if port status control provided doesn't correspond to an external
         * port */
        if (result == 0)
        {
            /* return error / NULL */
            return NULL;
        }

        /* create and return a new ExternalPortErrors object with the error
         * info */
        ExternalPortErrors *pErrors = NULL;
        try
        {
            pErrors = new ExternalPortErrors(externalPortErrors);
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
    * Get the status of the external port.
    *
    * @return A pointer to a new ExternalPortStatus object which contains the
    *         status information for the external port, or NULL if failed to
    *         allocate memory for the object OR if an error occurred retrieving
    *         the status information for the port.
    *
    * \note The object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    virtual PortStatus* GetStatus()
    {
        /* declare struct required by 'c' API to hold external port status */
        STAR_CFG_EXTERNAL_PORT_STATUS externalPortStatus;

        /* call 'c' API to get the external port's status */
        int result = CFG_ROUTER_getExternalPortStatus(portStatusControl,
                                                      &externalPortStatus);

        /* if port status control provided doesn't correspond to an external
         * port */
        if (result == 0)
        {
            /* return error / NULL */
            return NULL;
        }

        /* create and return a new ExternalPortStatus object with the status
         * info */
        ExternalPortStatus *pStatus = NULL;
        try
        {
            pStatus = new ExternalPortStatus(externalPortStatus);
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory, return error */
            return NULL;
        }

        return pStatus;
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

