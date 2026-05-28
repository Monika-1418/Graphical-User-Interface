/**
 * \file brick_mk3_external_port.hpp
 *
 * \brief Represents an External Port on a Brick Mk3 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents an External Port on a Brick Mk3 device.
 *
 * Copyright &copy; 2014 STAR-Dundee Ltd
 *
 * \ingroup CPP_BRICK_MK3_CONFIG
 */

#ifndef __DEVICE_BRICK_MK3_EXTERNAL_PORT_CPP__
#define __DEVICE_BRICK_MK3_EXTERNAL_PORT_CPP__

#include "brick_mk2_external_port.hpp"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace brickmk3 {

/**
* Represents an External Port on a Brick Mk3 device.
*/
class BrickMk3ExternalPort : public stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2ExternalPort
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    BrickMk3ExternalPort() : BrickMk2ExternalPort()
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
    BrickMk3ExternalPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl)
               : BrickMk2ExternalPort(idOfOwningDevice, type, portNum,
               statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of BrickMk3ExternalPort objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    BrickMk3ExternalPort(const BrickMk3ExternalPort& portToCopyFrom)
        : BrickMk2ExternalPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * BrickMk3ExternalPort objects to have their contents assigned to each
    * other.
    *
    */
    BrickMk3ExternalPort& operator=(
        const BrickMk3ExternalPort& portBeingAssigned)
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
    virtual ~BrickMk3ExternalPort()
    {
        /* no resources to free at present */
    }
};

/* end namespace tags */
} /* brickmk3 */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

