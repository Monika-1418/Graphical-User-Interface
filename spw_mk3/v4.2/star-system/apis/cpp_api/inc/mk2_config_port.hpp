/**
 * \file mk2_config_port.hpp
 *
 * \brief Represents a Configuration Port on a Mk2 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a Configuration Port on a Mk2 device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_MK2_CONFIG
 */

#ifndef __DEVICE_MK2_CONFIG_PORT_CPP__
#define __DEVICE_MK2_CONFIG_PORT_CPP__

#include "config_port.hpp"

namespace stardundee { namespace com { namespace starsystem {
              namespace deviceconfig { namespace mk2devices {

/**
* Represents a Configuration Port on a Mk2 device.
*/
class Mk2ConfigPort : public ConfigPort
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    Mk2ConfigPort() : ConfigPort()
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
    Mk2ConfigPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl)
               : ConfigPort(idOfOwningDevice, type, portNum, statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of Mk2ConfigPort objects to be made when
    * being passed by value in functions or returned by value from functions.
    *
    */
    Mk2ConfigPort(const Mk2ConfigPort& portToCopyFrom) : ConfigPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of Mk2ConfigPort
    * objects to have their contents assigned to each other.
    *
    */
    Mk2ConfigPort& operator=(const Mk2ConfigPort& portBeingAssigned)
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
    virtual ~Mk2ConfigPort()
    {
        /* no resources to free at present */
    }
};

/* end namespace tags */
} /* mk2devices */ } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

