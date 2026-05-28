/**
 * \file pxi_link_port.hpp
 *
 * \brief Represents a Link Port on a PXI device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a Link Port on a PXI device.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 *
 * \ingroup CPP_PXI_CONFIG
 */

#ifndef __DEVICE_PXI_LINK_PORT_CPP__
#define __DEVICE_PXI_LINK_PORT_CPP__

#include "mk2_link_port.hpp"
#include "clock_rate_params.hpp"
#include "cfg_api_pxi.h"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace pxi {

/**
* Represents a Link Port on a PXI device.
*/
class PXILinkPort :
    public stardundee::com::starsystem::deviceconfig::mk2devices::Mk2LinkPort
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PXILinkPort() : Mk2LinkPort()
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
    PXILinkPort(STAR_DEVICE_ID idOfOwningDevice,
        STAR_CFG_PORT_TYPE type,
        U8 portNum,
        PORT_STATUS_CONTROL statusControl) :
        Mk2LinkPort(idOfOwningDevice, type, portNum, statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of PXILinkPort objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    *
    */
    PXILinkPort(const PXILinkPort& portToCopyFrom) : Mk2LinkPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of PXILinkPort
    * objects to have their contents assigned to each other.
    *
    */
    PXILinkPort& operator=(const PXILinkPort& portBeingAssigned)
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
    virtual ~PXILinkPort()
    {
        /* no resources to free at present */
    }

    /**
    * Gets the address that a packet received on a specified Port should be
    * routed to when interface mode is enabled.
    *
    * \note This is an advanced feature and not necessary for normal usage.
    *
    * @param[out] pAddress Pointer to a value that will be updated to contain
    *                      the address.
    *
    * @return 1 if the address could be obtained, else 0.
    *
    */
    int GetPortRoutingAddress(_Out_ U8 *pAddress)
    {
        /* call 'c' API to get the port routing address */
        return CFG_PXI_getPortRoutingAddress(owningDeviceForPort, portNumber,
            pAddress);
    }

    /**
    * Sets the address that a packet received on the should be routed to when
    * interface mode is enabled.
    *
    * \note This is an advanced feature and not necessary for normal usage.
    *
    * @param address The routing address for the port.
    *
    * @return 1 if the address could be set, else 0.
    *
    */
    int SetPortRoutingAddress(U8 address)
    {
        /* call 'c' API to set the port routing address */
        return CFG_PXI_setPortRoutingAddress(owningDeviceForPort, portNumber,
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
    * @return 1 if interface mode was successfully enabled, else 0.
    *
    */
    int EnableInterfaceMode()
    {
        /* call 'c' API to enable interface mode on the port */
        return CFG_PXI_enableInterfaceModeOnPort(owningDeviceForPort,
            portNumber);
    }

    /**
    * Gets whether interface mode has been enabled on the port.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      interface mode is enabled, else 0.
    *
    * @return 1 if the information could be obtained for the port, else 0.
    *
    */
    int GetInterfaceModeEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API to get interface mode on port enabled */
        return CFG_PXI_getInterfaceModeOnPortEnabled(owningDeviceForPort,
            portNumber, pEnabled);
    }

    /**
    * Disable interface mode on the port.
    *
    * When interface mode is disabled the device operates in routing mode.
    *
    * @return 1 if interface mode was successfully disabled, else 0.
    *
    */
    int DisableInterfaceMode()
    {
        /* call 'c' API to disable interface mode on the port */
        return CFG_PXI_disableInterfaceModeOnPort(owningDeviceForPort,
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
    * @return 1 if source identification was successfully enabled, else 0.
    *
    */
    int EnableSourceIdentification()
    {
        /* call 'c' API to enable source identification on the port */
        return CFG_PXI_enableIdentifySourceOnPort(owningDeviceForPort,
            portNumber);
    }

    /**
    * Gets whether source identification has been enabled on the port.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      identify source is enabled, else 0.
    *
    * @return 1 if the information could be obtained, else 0.
    *
    */
    int GetSourceIdentificationEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API to get source identification on port enabled */
        return CFG_PXI_getIdentifySourceOnPortEnabled(owningDeviceForPort,
            portNumber, pEnabled);
    }

    /**
    * Disable source identification for a given port.
    *
    * @return 1 if source identification was successfully disabled, else 0.
    *
    */
    int DisableSourceIdentification()
    {
        /* call 'c' API to disable source identification on the port */
        return CFG_PXI_disableIdentifySourceOnPort(owningDeviceForPort,
            portNumber);
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
        return CFG_PXI_enableStateChangeEventsOnPort(owningDeviceForPort,
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
        return CFG_PXI_getStateChangeEventsOnPortEnabled(owningDeviceForPort,
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
    int DisableStateChangeEvents()
    {
        /* call 'c' API version */
        return CFG_PXI_disableStateChangeEventsOnPort(owningDeviceForPort,
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
        return CFG_PXI_enableSpeedChangeEventsOnPort(owningDeviceForPort,
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
        return CFG_PXI_getSpeedChangeEventsOnPortEnabled(owningDeviceForPort,
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
    int DisableSpeedChangeEvents()
    {
        /* call 'c' API version */
        return CFG_PXI_disableSpeedChangeEventsOnPort(owningDeviceForPort,
            portNumber);
    }

    /**
    * Gets the base transmit clock parameters for this link.
    *
    * \note This function is currently for use with PXI devices before hardware
    * version V1.1.
    * For later versions use GetTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which will receive the clock parameters
    *                    obtained.
    *
    * @return 1 if base transmit clock frequency parameters were successfully
    *         obtained, otherwise 0.
    *
    */
    int GetBaseTransmitClock(
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to get the base transmit clock params for this link */
        return CFG_PXI_getBaseTransmitClock(owningDeviceForPort, portNumber,
            clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the base transmit clock parameters for the link. The transmit rate
    * of a link is given by:
    *
    *  \f[
    *   Link Clock Rate = \frac{100 \mathrm{MHz} \times MULTIPIER}{DIVISOR}  \times 2
    *  \f]
    *
    * The \f$MULTIPIER\f$ and \f$DIVISOR\f$ are properties of the
    * \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    * class, passed as a parameter. The output clock rate must be between 5 MHz
    * and 200 MHz.
    *
    * \note This function is currently for use with PXI devices before hardware
    * version V1.1.
    * For later versions use SetTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param linkNum The link to set the base transmit clock rate for.
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which contains the clock parameters to set.
    *
    * @return 1 if base transmit clock frequency parameters were successfully
    *         set, otherwise 0.
    *
    */
    int SetBaseTransmitClock(
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_PXI_setBaseTransmitClock(owningDeviceForPort, portNumber,
            *clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Gets the transmit clock parameters for this link.
    *
    * \note This function is currently for use with PXI devices from hardware
    * version V1.1.
    * For earlier versions use GetBaseTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which will receive the clock parameters obtained.
    *
    * @return 1 if transmit clock frequency parameters were successfully
    *         obtained, otherwise 0.
    *
    */
    int GetTransmitClock(
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to get the transmit clock params for this link */
        return CFG_PXI_getTransmitClock(owningDeviceForPort, portNumber,
            clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the transmit clock parameters for this link. The transmit rate of a
    * link is given by:
    *
    *  \f[
    *   Link Clock Rate = \frac{100 \mathrm{MHz} \times MULTIPIER}{DIVISOR}  \times 2
    *  \f]
    *
    * The \f$MULTIPIER\f$ and \f$DIVISOR\f$ are properties of the
    * \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    * class, passed as a parameter. The output clock rate will be between 1 MHz
    * and 200 MHz giving data rates between 2 Mbit/s and 400 Mbit/s
    *
    * Note that for data rates below 10 Mbit/s, an exact rate may not be
    * achievable and it may be rounded up or down slightly.
    *
    * \note This function is currently for use with PXI devices from hardware
    * version V1.1.
    * For earlier versions use SetBaseTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param linkNum The link to set the transmit clock rate for.
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which contains the clock parameters to set.
    *
    * @return 1 if transmit clock frequency parameters were successfully
    *         set, otherwise 0.
    *
    */
    int SetTransmitClock(
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_PXI_setTransmitClock(owningDeviceForPort, portNumber,
            *clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
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
    int InjectError(SPW_ERROR error)
    {
        /* call 'c' API to inject the error for the link */
        return CFG_PXI_injectError(owningDeviceForPort, portNumber, error);
    }
};

/* end namespace tags */
} /* pxi */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

