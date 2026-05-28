/**
 * \file splt_link_port.hpp
 *
 * \brief Represents a Link Port on a SPLT device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a Link Port on a SPLT device.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 *
 * \ingroup CPP_SPLT_CONFIG
 */

#ifndef __DEVICE_SPLT_LINK_PORT_CPP__
#define __DEVICE_SPLT_LINK_PORT_CPP__

#include "pcie_link_port.hpp"
#include "clock_rate_params.hpp"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace splt {

/**
* Represents a Link Port on a SPLT device.
*/
class SPLTLinkPort : public stardundee::com::starsystem::deviceconfig::mk2devices::pcie::PCIeLinkPort
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    SPLTLinkPort() : PCIeLinkPort()
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
    SPLTLinkPort(STAR_DEVICE_ID idOfOwningDevice, STAR_CFG_PORT_TYPE type,
        U8 portNum, PORT_STATUS_CONTROL statusControl) : PCIeLinkPort(
        idOfOwningDevice, type, portNum, statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of SPLTLinkPort objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    *
    */
    SPLTLinkPort(const SPLTLinkPort& portToCopyFrom) : PCIeLinkPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of SPLTLinkPort
    * objects to have their contents assigned to each other.
    *
    */
    SPLTLinkPort& operator=(const SPLTLinkPort& portBeingAssigned)
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
    virtual ~SPLTLinkPort()
    {
        /* no resources to free at present */
    }
};

/* end namespace tags */
} /* splt */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

