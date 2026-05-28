/**
 * \file brick_mk2_link_port.hpp
 *
 * \brief Represents a Link Port on a Brick Mk2 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a Link Port on a Brick Mk2 device.
 *
 * Copyright &copy; 2014 STAR-Dundee Ltd
 *
 * \ingroup CPP_BRICK_MK2_CONFIG
 */

#ifndef __DEVICE_BRICK_MK2_LINK_PORT_CPP__
#define __DEVICE_BRICK_MK2_LINK_PORT_CPP__

#include "mk2_link_port.hpp"
#include "cfg_api_brick_mk2.h"
#include "brick_mk2_errors.hpp"
#include "not_supported_exception.hpp"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace brickmk2 {

/**
* Represents a Link Port on a Brick Mk2 device.
*/
class BrickMk2LinkPort : public Mk2LinkPort
{
public:
    /** Frequencies that a link on a Brick Mk2 compatible device may run at.
      * These are divided by a divider set by SetLinkRateDivider() to generate
      * the desired link speed. */
    enum LinkFrequency
    {
        /** 120 MHz */
        LINK_FREQUENCY_120MHZ = STAR_CFG_BRICK_MK2_LINK_FREQ_120,
        /** 130 MHz */
        LINK_FREQUENCY_130MHZ = STAR_CFG_BRICK_MK2_LINK_FREQ_130,
        /** 140 MHz */
        LINK_FREQUENCY_140MHZ = STAR_CFG_BRICK_MK2_LINK_FREQ_140,
        /** 150 MHz */
        LINK_FREQUENCY_150MHZ = STAR_CFG_BRICK_MK2_LINK_FREQ_150,
        /** 160 MHz */
        LINK_FREQUENCY_160MHZ = STAR_CFG_BRICK_MK2_LINK_FREQ_160,
        /** 180 MHz */
        LINK_FREQUENCY_180MHZ = STAR_CFG_BRICK_MK2_LINK_FREQ_180,
        /** 200 MHz */
        LINK_FREQUENCY_200MHZ = STAR_CFG_BRICK_MK2_LINK_FREQ_200
    };

    /**
    * Default constructor. Initialise object to default state.
    */
    BrickMk2LinkPort() : Mk2LinkPort()
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
    BrickMk2LinkPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl) : Mk2LinkPort(idOfOwningDevice,
                                                     type,
                                                     portNum,
                                                     statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of BrickMk2LinkPort objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    *
    */
    BrickMk2LinkPort(const BrickMk2LinkPort& portToCopyFrom) : Mk2LinkPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of BrickMk2LinkPort
    * objects to have their contents assigned to each other.
    *
    */
    BrickMk2LinkPort& operator=(const BrickMk2LinkPort& portBeingAssigned)
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
    virtual ~BrickMk2LinkPort()
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

    /**
    * Get the clock frequency for the link.
    *
    * @param[out] pLinkFreq Pointer to a value that will be updated with the
    *                       link's clock frequency.
    *
    * @return 1 clock frequency successfully obtained, otherwise 0.
    *
    */
    int GetLinkClockFrequency(_Out_ LinkFrequency *pLinkFreq)
    {
        /* call 'c' API to get the clock frequency for this link */
        STAR_CFG_BRICK_MK2_LINK_FREQ linkFreq;
        int result =  CFG_BRICK_MK2_getLinkClockFrequency(owningDeviceForPort,
                                                   portNumber, &linkFreq);

        /* store frequency in input variable provided */
        (*pLinkFreq) = (LinkFrequency)linkFreq;

        /* return result */
        return result;
    }

    /**
    * Set the clock frequency for the link.
    *
    * @param[out] linkFreq Link clock frequency to be set.
    *
    * @return 1 clock frequency successfully set, otherwise 0.
    *
    */
    int SetLinkClockFrequency(LinkFrequency linkFreq)
    {
        /* call 'c' API to get the clock frequency for this link */
        return CFG_BRICK_MK2_setLinkClockFrequency(owningDeviceForPort,
                           portNumber, (STAR_CFG_BRICK_MK2_LINK_FREQ)linkFreq);
    }

    /**
    * Gets the current link speed measured on the port.
    *
    * @param[out] pLinkSpeed User supplied value that will be updated to contain
    *                        the measured link speed in 100 KHz units.
    *
    * @return 1 if the information was successfully obtained, else 0.
    *
    */
    int GetMeasuredLinkSpeed(_Out_ U16 *pLinkSpeed)
    {
        /* call 'c' API version */
        return CFG_MK2_getMeasuredLinkSpeed(owningDeviceForPort,
                                            portNumber,
                                            pLinkSpeed);
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
    int EnableStateChangeEvents()
    {
        /* call 'c' API version */
        return CFG_BRICK_MK2_enableStateChangeEventsOnPort(owningDeviceForPort,
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
    int GetStateChangeEventsEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API version */
        return CFG_BRICK_MK2_getStateChangeEventsOnPortEnabled(
                                                            owningDeviceForPort,
                                                            portNumber,
                                                            pEnabled);
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
    int DisableStateChangeEvents()
    {
        /* call 'c' API version */
        return CFG_BRICK_MK2_disableStateChangeEventsOnPort(owningDeviceForPort,
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
    int EnableSpeedChangeEvents()
    {
        /* call 'c' API version */
        return CFG_BRICK_MK2_enableSpeedChangeEventsOnPort(owningDeviceForPort,
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
    int GetSpeedChangeEventsEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API version */
        return CFG_BRICK_MK2_getSpeedChangeEventsOnPortEnabled(
                                                            owningDeviceForPort,
                                                            portNumber,
                                                            pEnabled);
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
    int DisableSpeedChangeEvents()
    {
        /* call 'c' API version */
        return CFG_BRICK_MK2_disableSpeedChangeEventsOnPort(owningDeviceForPort,
                                                             portNumber);
    }

    /**
    * This function is not not supported on the Brick Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int InjectError(SPW_ERROR error)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(error);

        /* throw exception - function not supported for Brick Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * Inject the specified errors on the port.
    *
    * @param errors A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2Errors BrickMk2Errors\endlink
    *               object which specifies the errors to inject on the given
    *               port.
    *
    * @return 1 if the errors were successfully injected, otherwise 0.
    *
    */
    int InjectErrors(BrickMk2Errors& errors)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_injectErrors(owningDeviceForPort, portNumber,
                                       errors.GetAsSTAR_CFG_BRICK_MK2_ERRORS());
    }
};

/* end namespace tags */
} /* brickmk2 */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

