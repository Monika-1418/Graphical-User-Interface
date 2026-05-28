/**
 * \file link_port.hpp
 *
 * \brief Represents a (SpaceWire) Link Port on a device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a (SpaceWire) Link Port on a device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_LINK_PORT_CPP__
#define __DEVICE_LINK_PORT_CPP__

#include "port.hpp"
#include "link_port_errors.hpp"
#include "link_port_status.hpp"
#include "cfg_api_router.h"
#include <new>
#include "cpp_api_macros.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Represents a (SpaceWire) Link Port on a device.
*/
class LinkPort : public Port
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    LinkPort() : Port()
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
    LinkPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl) : Port(idOfOwningDevice, type,
                                                     portNum, statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of LinkPort objects to be made when
    * being passed by value in functions or returned by value from functions.
    *
    */
    LinkPort(const LinkPort& portToCopyFrom) : Port()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of LinkPort objects
    * to have their contents assigned to each other.
    *
    */
    LinkPort& operator=(const LinkPort& portBeingAssigned)
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
    virtual ~LinkPort()
    {
        /* no resources to free at present */
    }

    /**
    * Gets any errors present on this link port.
    *
    * \note Errors on a port are latched until cleared with the ClearErrors()
    *       method.
    *
    * @return A pointer to a new LinkPortErrors object which contains various
    *         fields to indicate whether different types of errors have occurred
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
        /* declare struct required by 'c' API to hold link port errors */
        STAR_CFG_SPW_LINK_ERRORS linkPortErrors;

        /* call 'c' API to get any link port errors */
        int result = CFG_ROUTER_getSpaceWireLinkErrors(portStatusControl,
                                                      &linkPortErrors);

        /* if port status control provided doesn't correspond to a link port */
        if (result == 0)
        {
            /* return error / NULL */
            return NULL;
        }

        /* create and return a new LinkPortErrors object with the error info */
        LinkPortErrors *pErrors = NULL;
        try
        {
            pErrors = new LinkPortErrors(linkPortErrors);
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
    * Get the status of the link.
    *
    * @return A pointer to a new LinkStatus object which contains the
    *         status information for the link, or NULL if failed to
    *         allocate memory for the object OR if an error occurred retrieving
    *         the status information for the port.
    *
    * \note The object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    virtual PortStatus* GetStatus()
    {
        /* declare struct required by 'c' API to hold link status */
        STAR_CFG_SPW_LINK_STATUS linkStatus;

        /* call 'c' API to get the link's status */
        int result = CFG_ROUTER_getSpaceWireLinkStatus(portStatusControl,
                                                      &linkStatus);

        /* if port status control provided doesn't correspond to a link port */
        if (result == 0)
        {
            /* return error / NULL */
            return NULL;
        }

        /* create and return a new LinkStatus object with the status info */
        LinkStatus *pStatus = NULL;
        try
        {
            pStatus = new LinkStatus(linkStatus);
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory, return error */
            return NULL;
        }

        return pStatus;
    }

    /**
    * Set the status of the link.
    *
    * @param linkStatus A pointer to a LinkStatus object which contains the
    *                   values to use to set the state of the link.
    *
    * \note The 'linkState' member / field of this object is ignored by this
    *       function.
    *
    * @return 1 if the link state was set successfully, otherwise 0.
    *
    */
    int SetSpaceWireLinkStatus(LinkStatus *linkStatus)
    {
        /* if this isn't a link port */
        if (this->GetType() != (PortType)STAR_CFG_PORT_TYPE_LINK)
        {
            /* return error / 0 */
            return 0;
        }

        /* get the STAR_CFG_SPW_LINK_STATUS struct from the link status object */
        STAR_CFG_SPW_LINK_STATUS *pLinkStatusStruct =
                                    linkStatus->GetAsSTAR_CFG_SPW_LINK_STATUS();

        /* call 'c' API to set the state of this link */
        return CFG_ROUTER_setSpaceWireLinkStatus(owningDeviceForPort,
                                                 portNumber,
                                                 *pLinkStatusStruct);
    }

    /**
    * Start the link by setting the start bit and clearing the disable bit.
    *
    * @return 1 if the link was successfully started, otherwise 0.
    *
    */
    int StartLink()
    {
        /* call 'c' API to start the given link */
        return CFG_ROUTER_startLink(owningDeviceForPort, portNumber);
    }

    /**
    * Stops the link by clearing the start bit and setting the disable bit.
    *
    * \note If auto-start is enabled the link will start again as soon as it
    *       receives NULLs.
    *
    * @return 1 if the link was successfully stopped, otherwise 0.
    *
    */
    int StopLink()
    {
        /* call 'c' API to stop the given link */
        return CFG_ROUTER_stopLink(owningDeviceForPort, portNumber);
    }

    /**
    * Get whether time-code distribution is enabled on this port.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      time-code distribution is enabled on this port,
    *                      else 0.
    *
    * @return 1 if the time-code status was successfully obtained, otherwise 0.
    *
    */
    int GetTimeCodeDistributionEnabled(_Out_ int *pEnabled)
    {
        U32 portMask;
        int result;

        /* call 'c' API to get the time-code distribution ports on the device
         * which owns this port */
        result = CFG_ROUTER_getTimeCodeDistributionPorts(owningDeviceForPort,
                                                         &portMask);

        /* if failed to obtain value of time-code distribution ports from
         * device */
        if (result == 0)
        {
            /* return error */
            return 0;
        }

        /* if this port is one of those upon which time-code distribution is
         * enabled */
        if (((int)portMask & (1 << (int)this->GetPortNumber())))
        {
            /* set enabled to true */
            (*pEnabled) = 1;
        }
        else
        {
            /* set enabled to false */
            (*pEnabled) = 0;
        }

        /* return success */
        return 1;
    }

    /**
    * Enables time-code distribution on this port.
    *
    * @return 1 if the time-code status was successfully changed, otherwise 0.
    *
    */
    int EnableTimeCodeDistribution()
    {
        U32 portMask;
        int result;

        /* call 'c' API to get the time-code distribution ports on the device
         * which owns this port */
        result = CFG_ROUTER_getTimeCodeDistributionPorts(owningDeviceForPort,
            &portMask);

        /* if failed to obtain value of time-code distribution ports from
         * device */
        if (result == 0)
        {
            /* return error */
            return 0;
        }

        /* enable time-code distribution on this port */
        result = CFG_ROUTER_setTimeCodeDistributionPorts(owningDeviceForPort,
            portMask | (1 << (int)this->GetPortNumber()));

        /* if failed to set value of time-code distribution ports */
        if (result == 0)
        {
            /* return error */
            return 0;
        }

        /* return success */
        return 1;
    }

    /**
    * Disables time-code distribution on this port.
    *
    * @return 1 if the time-code status was successfully changed, otherwise 0.
    *
    */
    int DisableTimeCodeDistribution()
    {
        U32 portMask;
        int result;

        /* call 'c' API to get the time-code distribution ports on the device
         * which owns this port */
        result = CFG_ROUTER_getTimeCodeDistributionPorts(owningDeviceForPort,
            &portMask);

        /* if failed to obtain value of time-code distribution ports from
         * device */
        if (result == 0)
        {
            /* return error */
            return 0;
        }

        /* disable time-code distribution on this port */
        result = CFG_ROUTER_setTimeCodeDistributionPorts(owningDeviceForPort,
            portMask & ~(1 << (int)this->GetPortNumber()));

        /* if failed to set value of time-code distribution ports */
        if (result == 0)
        {
            /* return error */
            return 0;
        }

        /* return success */
        return 1;
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

