/**
 * \file pcie_link_port.hpp
 *
 * \brief Represents a Link Port on a PCIe device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a Link Port on a PCIe device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_PCIe_CONFIG
 */

#ifndef __DEVICE_PCIe_LINK_PORT_CPP__
#define __DEVICE_PCIe_LINK_PORT_CPP__

#include "mk2_link_port.hpp"
#include "clock_rate_params.hpp"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace pcie {

/**
* Represents a Link Port on a PCIe device.
*/
class PCIeLinkPort : public Mk2LinkPort
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PCIeLinkPort() : Mk2LinkPort()
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
    PCIeLinkPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl) : Mk2LinkPort(
                                                     idOfOwningDevice,
                                                     type,
                                                     portNum,
                                                     statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of PCIeLinkPort objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    *
    */
    PCIeLinkPort(const PCIeLinkPort& portToCopyFrom) : Mk2LinkPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of PCIeLinkPort
    * objects to have their contents assigned to each other.
    *
    */
    PCIeLinkPort& operator=(const PCIeLinkPort& portBeingAssigned)
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
    virtual ~PCIeLinkPort()
    {
        /* no resources to free at present */
    }

    /**
    * Gets the base transmit clock parameters for the link.
    *
    * \note This function is currently for use with PCIe devices before
    * hardwareversion v1.11 and SPLT devices before hardware version v2.0. \n
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
    int GetBaseTransmitClock(ClockRateParams& clockParams)
    {
        /* call 'c' API to get the base transmit clock params for this link */
        return CFG_MK2_getBaseTransmitClock(owningDeviceForPort, portNumber,
                           clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the base transmit clock rate on thelink. The transmit rate of
    * a link is given by:
    *
    *  \f[
    *   Link Clock Rate = \frac{100 \mathrm{MHz} \times MULTIPIER}{DIVISOR}  \times 2
    *  \f]
    *
    * The \f$MULTIPIER\f$ and \f$DIVISOR\f$ are properties of the
    * \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    * structure, passed as a parameter. The output clock rate must be between
    * 5 MHz and 200 MHz.
    *
    * \note This function is currently for use with PCIe devices before
    * hardwareversion v1.11 and SPLT devices before hardware version v2.0. \n
    * For later versions use SetTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which contains the clock parameters to set.
    *
    * @return 1 if base transmit clock frequency parameters were successfully
    *         set, otherwise 0.
    *
    */
    int SetBaseTransmitClock(ClockRateParams& clockParams)
    {
        /* get the STAR_CFG_MK2_BASE_TRANSMIT_CLOCK struct from the clock
         * params provided. */
        STAR_CFG_MK2_BASE_TRANSMIT_CLOCK *pParams =
                          clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK();

        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_MK2_setBaseTransmitClock(owningDeviceForPort, portNumber,
                                            *pParams);
    }

    /**
    * Gets the transmit clock parameters for a given link.
    *
    * \note This function is currently for use with PCIe devices from hardware
    * version v1.11 and SPLT devices from hardware version v2.0.
    * For earlier versions use GetBaseTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which will receive the clock parameters
    *                    obtained.
    *
    * @return 1 if transmit clock frequency parameters were successfully
    *         obtained, otherwise 0.
    *
    */
    int GetTransmitClock(ClockRateParams& clockParams)
    {
        /* call 'c' API to get the transmit clock params for this link */
        return CFG_MK2_getTransmitClock(owningDeviceForPort, portNumber,
            clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the transmit clock rate on the link. The transmit rate of a link
    * is given by:
    *
    *  \f[
    *   Link Clock Rate = \frac{100 \mathrm{MHz} \times MULTIPIER}{DIVISOR}  \times 2
    *  \f]
    *
    * The \f$MULTIPIER\f$ and \f$DIVISOR\f$ are properties of the
    * \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    * structure, passed as a parameter. The output clock rate will be between 1
    * MHz and 200 MHz giving data rates between 2 Mbit/s and 400 Mbit/s.
    *
    * Note that for data rates below 10 Mbit/s, an exact rate may not be
    * achievable and it may be rounded up or down slightly.
    *
    * \note This function is currently for use with PCIe devices from hardware
    * version v1.11 and SPLT devices from hardware version v2.0.
    * For earlier versions use SetBaseTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which contains the clock parameters to set.
    *
    * @return 1 if base transmit clock frequency parameters were successfully
    *         set, otherwise 0.
    *
    */
    int SetTransmitClock(ClockRateParams& clockParams)
    {
        /* get the STAR_CFG_MK2_BASE_TRANSMIT_CLOCK struct from the clock
         * params provided. */
        STAR_CFG_MK2_BASE_TRANSMIT_CLOCK *pParams =
                          clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK();

        /* call 'c' API to set the transmit clock params for this link */
        return CFG_MK2_setTransmitClock(owningDeviceForPort, portNumber,
            *pParams);
    }

    /**
    * Selectively enables time-code notifications on a the channel attached to
    * the port.
    *
    * @return 1 if time-code events were successfully enabled, else 0.
    *
    */
    int EnableTimeCodeEvents()
    {
        /* call 'c' API to enable time-code events on the port */
        return CFG_MK2_enableTimeCodeEventsOnPort(owningDeviceForPort,
            portNumber);
    }

    /**
    * Gets whether time-code events are enabled on the port.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      time-code events are enabled, else 0.
    *
    * @return 1 if the information was successfully obtained, else 0.
    *
    */
    int GetTimeCodeEventsEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API to get time-code events enabled for the port */
        return CFG_MK2_getTimeCodeEventsOnPortEnabled(owningDeviceForPort,
            portNumber, pEnabled);
    }

    /**
    * Disables time-code events on the port.
    *
    * @return 1 if time-code events were successfully disabled, else 0.
    *
    */
    int DisableTimeCodeEvents()
    {
        /* call 'c' API to disable time-code events on the port */
        return CFG_MK2_disableTimeCodeEventsOnPort(owningDeviceForPort,
            portNumber);
    }
};

/* end namespace tags */
} /* pcie */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

