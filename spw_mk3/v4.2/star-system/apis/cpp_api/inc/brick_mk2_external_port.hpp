/**
 * \file brick_mk2_external_port.hpp
 *
 * \brief Represents an External Port on a Brick Mk2 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents an External Port on a Brick Mk2 device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_BRICK_MK2_CONFIG
 */

#ifndef __DEVICE_BRICK_MK2_EXTERNAL_PORT_CPP__
#define __DEVICE_BRICK_MK2_EXTERNAL_PORT_CPP__

#include "mk2_external_port.hpp"
#include "cfg_api_brick_mk2.h"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace brickmk2 {

/**
* Represents an External Port on a Brick Mk2 device.
*/
class BrickMk2ExternalPort : public Mk2ExternalPort
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    BrickMk2ExternalPort() : Mk2ExternalPort()
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
    BrickMk2ExternalPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl) : Mk2ExternalPort(
                                                     idOfOwningDevice,
                                                     type,
                                                     portNum,
                                                     statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of BrickMk2ExternalPort objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    BrickMk2ExternalPort(const BrickMk2ExternalPort& portToCopyFrom)
                                                 : Mk2ExternalPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * BrickMk2ExternalPort objects to have their contents assigned to each
    * other.
    *
    */
    BrickMk2ExternalPort& operator=(const BrickMk2ExternalPort&
                                          portBeingAssigned)
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
    virtual ~BrickMk2ExternalPort()
    {
        /* no resources to free at present */
    }

    /**
    * Gets the address that a packet received on a specified Port should be
    * routed to when interface mode is enabled.
    *
    * \note This is an advanced feature and not necessary for normal usage.
    *       PCI/PCI Mk2 and PCIe devices prior to version 1.07 have a bug which
    *       means that the value returned in pAddress is always 0.
    *
    * @param[out] pAddress Pointer to a value that will be updated to contain
    *                      the address.
    *
    * @return 1 if the address could be obtained, else 0.
    *
    */
    virtual int GetPortRoutingAddress(_Out_ U8 *pAddress)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_getPortRoutingAddress(owningDeviceForPort,
                                                   portNumber, pAddress);
    }

    /**
    * Sets the address that a packet received on a specified port should be
    * routed to when interface mode is enabled.
    *
    * \note This is an advanced feature and not necessary for normal usage.
    *
    * @param address The routing address for the port.
    *
    * @return 1 if the address could be set, else 0.
    *
    */
    virtual int SetPortRoutingAddress(U8 address)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_setPortRoutingAddress(owningDeviceForPort,
                                                   portNumber, address);
    }

    /**
    * Enable interface mode on the port.
    *
    * \note Interface mode must be enabled globally on the device which owns
    *       this port (by calling EnableInterfaceMode() on the device's
    *       configuration state) in order for interface mode on a specific port
    *       to be enabled.
    *
    *
    * @return 1 if interface mode was successfully enabled, else 0.
    *
    */
    virtual int EnableInterfaceMode()
    {
        /* call 'c' API to enable interface mode on this port*/
        return CFG_BRICK_MK2_enableInterfaceModeOnPort(owningDeviceForPort,
                                                 portNumber);
    }

    /**
    * Gets whether interface mode has been enabled on the port.
    *
    * \note PCI/PCI Mk2 and PCIe devices prior to version 1.07 have a bug which
    *       means that the value returned in pEnabled is always 0.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      interface mode is enabled, else 0.
    *
    * @return 1 if the information could be obtained for the port, else 0.
    *
    */
    virtual int GetInterfaceModeEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_getInterfaceModeOnPortEnabled(owningDeviceForPort,
                                                     portNumber,
                                                     pEnabled);
    }

    /**
    * Disable interface mode on the port.
    *
    * When interface mode is disabled the device operates in routing mode.
    *
    *
    * @return 1 if interface mode was successfully disabled, else 0.
    *
    */
    virtual int DisableInterfaceMode()
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_disableInterfaceModeOnPort(owningDeviceForPort,
                                                  portNumber);
    }

    /**
    * Enables source identification during interface made for the port.
    *
    * Interface mode and source identification must be enabled globally (i.e.
    * on the device which owns this port, by calling its EnableInterfaceMode()
    * and EnableSourceIdentification() methods) in order for this setting to
    * have effect.
    *
    *
    * @return 1 if source identification was successfully enabled, else 0.
    *
    */
    virtual int EnableSourceIdentification()
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_enableIdentifySourceOnPort(owningDeviceForPort,
                                                  portNumber);
    }

    /**
    * Gets whether source identification has been enabled on the port.
    *
    * \note PCI/PCI Mk2 and PCIe devices prior to version 1.07 have a bug which
    *       means that the value returned in pEnabled is always 0.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      identify source is enabled, else 0.
    *
    * @return 1 if the information could be obtained, else 0.
    *
    */
    virtual int GetSourceIdentificationEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_getIdentifySourceOnPortEnabled(owningDeviceForPort,
                                                      portNumber,
                                                      pEnabled);
    }

    /**
    * Disable source identification for the port.
    *
    * @return 1 if source identification was successfully disabled, else 0.
    *
    */
    virtual int DisableSourceIdentification()
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_disableIdentifySourceOnPort(owningDeviceForPort,
                                                   portNumber);
    }
};

/* end namespace tags */
} /* brickmk2 */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

