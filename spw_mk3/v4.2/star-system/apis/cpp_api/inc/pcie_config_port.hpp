/**
 * \file pcie_config_port.hpp
 *
 * \brief Represents a Configuration Port on a PCIe device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a Configuration Port on a PCIe device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_PCIe_CONFIG
 */

#ifndef __DEVICE_PCIe_CONFIG_PORT_CPP__
#define __DEVICE_PCIe_CONFIG_PORT_CPP__

#include "mk2_config_port.hpp"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace pcie {

/**
* Represents a Configuration Port on a PCIe device.
*/
class PCIeConfigPort : public Mk2ConfigPort
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PCIeConfigPort() : Mk2ConfigPort()
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
    PCIeConfigPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl)
               : Mk2ConfigPort(idOfOwningDevice, type, portNum, statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of PCIeConfigPort objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    */
    PCIeConfigPort(const PCIeConfigPort& portToCopyFrom) : Mk2ConfigPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of PCIeConfigPort
    * objects to have their contents assigned to each other.
    *
    */
    PCIeConfigPort& operator=(const PCIeConfigPort& portBeingAssigned)
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
    virtual ~PCIeConfigPort()
    {
        /* no resources to free at present */
    }
};

/* end namespace tags */
} /* pcie */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

