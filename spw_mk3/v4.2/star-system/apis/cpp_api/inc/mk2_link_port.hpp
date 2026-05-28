/**
 * \file mk2_link_port.hpp
 *
 * \brief Represents a Link Port on a Mk2 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a Link Port on a Mk2 device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_MK2_CONFIG
 */

#ifndef __DEVICE_MK2_LINK_PORT_CPP__
#define __DEVICE_MK2_LINK_PORT_CPP__

#include "link_port.hpp"
#include "cfg_api_mk2.h"

namespace stardundee { namespace com { namespace starsystem {
              namespace deviceconfig { namespace mk2devices {

/**
* Represents a Link Port on a Mk2 device.
*/
class Mk2LinkPort : public LinkPort
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    Mk2LinkPort() : LinkPort()
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
    Mk2LinkPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl) : LinkPort(idOfOwningDevice, type,
                                                     portNum, statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of Mk2LinkPort objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    *
    */
    Mk2LinkPort(const Mk2LinkPort& portToCopyFrom) : LinkPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of Mk2LinkPort
    * objects to have their contents assigned to each other.
    *
    */
    Mk2LinkPort& operator=(const Mk2LinkPort& portBeingAssigned)
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
    virtual ~Mk2LinkPort()
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
        return CFG_MK2_getPortRoutingAddress(owningDeviceForPort, portNumber,
                                              pAddress);
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
        return CFG_MK2_setPortRoutingAddress(owningDeviceForPort, portNumber,
                                              address);
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
        return CFG_MK2_enableInterfaceModeOnPort(owningDeviceForPort,
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
        return CFG_MK2_getInterfaceModeOnPortEnabled(owningDeviceForPort,
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
        return CFG_MK2_disableInterfaceModeOnPort(owningDeviceForPort,
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
        return CFG_MK2_enableIdentifySourceOnPort(owningDeviceForPort,
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
        return CFG_MK2_getIdentifySourceOnPortEnabled(owningDeviceForPort,
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
        return CFG_MK2_disableIdentifySourceOnPort(owningDeviceForPort,
                                                   portNumber);
    }

    /**
    * Sets the link rate divider for the link.
    *
    * \note It is never necessary to set a link rate divider higher than 100 as
    *       the minimum transmit rate permitted by SpaceWire is 2 Mbit/s (200 Mbit/s /
    *       100).
    *
    *       If an odd number (other than 1) is specified as the divider
    *       parameter, the previous even integer will be used instead. For
    *       example: if a divider value of 3 is specified, the actual divider
    *       set shall be 2.
    *
    * @param divider Value of new divider. Valid input: 1, Even numbers 2
    *                through 126.
    *
    * @return 1 if divider was successfully set, otherwise 0.
    *
    */
    int SetLinkRateDivider(U8 divider)
    {
        /* call 'c' API to set link rate divider for the link */
        return CFG_MK2_setLinkRateDivider(owningDeviceForPort, portNumber,
                                          divider);

    }

    /**
    * Gets the link rate divider for the link.
    *
    * @param[out] divider Value of the divider.
    *
    * @return 1 if link rate divider successfully obtained, otherwise 0.
    *
    */
    int GetLinkRateDivider(_Out_ U8* divider)
    {
        /* call 'c' API to get the link rate divider for the link */
        return CFG_MK2_getLinkRateDivider(owningDeviceForPort, portNumber,
                                          divider);
    }

    /**
    * Enables state change events on the port.
    *
    * Enabling state change events will result in state change event traffic
    * being received on channel 0 when the link changes to running or
    * disconnected.
    *
    * @return 1 if state change events were successfully enabled, else 0.
    *
    */
    virtual int EnableStateChangeEvents()
    {
        /* call 'c' API version */
        return CFG_MK2_enableStateChangeEventsOnPort(owningDeviceForPort,
            portNumber);
    }

    /**
    * Gets whether state change events are enabled on the port.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      state change events are enabled, else 0.
    *
    * @return 1 if the information was successfully obtained, else 0.
    *
    */
    virtual int GetStateChangeEventsEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API version */
        return CFG_MK2_getStateChangeEventsOnPortEnabled(owningDeviceForPort,
            portNumber, pEnabled);
    }

    /**
    * Disables state change events on the port.
    *
    * Disabling state change events will result in state change event traffic
    * no longer being received on channel 0 when the link changes to running or
    * disconnected.
    *
    * @return 1 if state change events were successfully disabled, else 0.
    *
    */
    virtual int DisableStateChangeEvents()
    {
        /* call 'c' API version */
        return CFG_MK2_disableStateChangeEventsOnPort(owningDeviceForPort,
            portNumber);
    }

    /**
    * Enables speed change events on the port.
    *
    * Enabling speed change events will result in speed change event traffic
    * being received on channel 0 when the link speed changes.
    *
    * @return 1 if speed change events were successfully enabled, else 0.
    *
    */
    virtual int EnableSpeedChangeEvents()
    {
        /* call 'c' API version */
        return CFG_MK2_enableSpeedChangeEventsOnPort(owningDeviceForPort,
            portNumber);
    }

    /**
    * Gets whether speed change events are enabled on the port.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      speed change events are enabled, else 0.
    *
    * @return 1 if the information was successfully obtained, else 0.
    *
    */
    virtual int GetSpeedChangeEventsEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API version */
        return CFG_MK2_getSpeedChangeEventsOnPortEnabled(owningDeviceForPort,
            portNumber, pEnabled);
    }

    /**
    * Disables speed change events on the port.
    *
    * Disabling speed change events will result in speed change event traffic
    * no longer being received on channel 0 when the link speed changes.
    *
    * @return 1 if speed change events were successfully disabled, else 0.
    *
    */
    virtual int DisableSpeedChangeEvents()
    {
        /* call 'c' API version */
        return CFG_MK2_disableSpeedChangeEventsOnPort(owningDeviceForPort,
            portNumber);
    }

    /**
    * Gets the current link speed measured on a specific port.
    *
    * @param pLinkSpeed[out] User supplied value that will be updated to
    *                        contain the measured link speed in 100 Kbit/s
    *                        units, see #STAR_CFG_MK2_LINK_SPEED_UNITS_KBPS.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    */
    virtual int GetMeasuredLinkSpeed(_Out_ U16 *pLinkSpeed)
    {
        /* call 'c' API version to get measured link speed */
        return CFG_MK2_getMeasuredLinkSpeed(owningDeviceForPort, portNumber,
            pLinkSpeed);
    }

    /**
    * Immediately injects the specified error on the port.
    *
    * @param errors SpaceWire error to be injected.
    *
    * @return 1 if the error information was successfully sent to the device,
    *           else 0.
    *
    */
    virtual int InjectError(SPW_ERROR error)
    {
        /* call 'c' API to inject the error for the link */
        return CFG_MK2_injectError(owningDeviceForPort, portNumber, error);
    }
};

/* end namespace tags */
} /* mk2devices */ } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

