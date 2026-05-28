/**
 * \file router_mk2s_external_port.hpp
 *
 * \brief Represents an External Port on a Router Mk2S device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents an External Port on a Router Mk2S device.
 *
 * Copyright &copy; 2014 STAR-Dundee Ltd
 *
 * \ingroup CPP_ROUTER_MK2S_CONFIG
 */

#ifndef __DEVICE_ROUTER_MK2S_EXTERNAL_PORT_CPP__
#define __DEVICE_ROUTER_MK2S_EXTERNAL_PORT_CPP__

#include "brick_mk2_external_port.hpp"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace routermk2s {

/**
* Represents an External Port on a Router Mk2S device.
*/
class RouterMk2SExternalPort : public stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2ExternalPort
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RouterMk2SExternalPort() : stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2ExternalPort()
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
    RouterMk2SExternalPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl)
               : stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2ExternalPort(idOfOwningDevice, type, portNum, statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of RouterMk2SExternalPort objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    RouterMk2SExternalPort(const RouterMk2SExternalPort& portToCopyFrom)
        : stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2ExternalPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * RouterMk2SExternalPort objects to have their contents assigned to each
    * other.
    *
    */
    RouterMk2SExternalPort& operator=(
                               const RouterMk2SExternalPort& portBeingAssigned)
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
    virtual ~RouterMk2SExternalPort()
    {
        /* no resources to free at present */
    }
};

/* end namespace tags */
} /* routermk2s */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

