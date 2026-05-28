/**
 * \file brick_mk3_link_port.hpp
 *
 * \brief Represents a Link Port on a Brick Mk3 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a Link Port on a Brick Mk3 device.
 *
 * Copyright &copy; 2014 STAR-Dundee Ltd
 *
 * \ingroup CPP_BRICK_MK3_CONFIG
 */

#ifndef __DEVICE_BRICK_MK3_LINK_PORT_CPP__
#define __DEVICE_BRICK_MK3_LINK_PORT_CPP__

#include "brick_mk2_link_port.hpp"
#include "mk2_link_port.hpp"
#include "clock_rate_params.hpp"
#include "cfg_api_brick_mk3.h"

namespace stardundee { namespace com { namespace starsystem {
namespace deviceconfig { namespace mk2devices { namespace brickmk3 {

/**
* Represents a Link Port on a Brick Mk3 device.
*/
class BrickMk3LinkPort : public stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2LinkPort
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    BrickMk3LinkPort() : BrickMk2LinkPort()
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
    BrickMk3LinkPort(STAR_DEVICE_ID idOfOwningDevice,
               STAR_CFG_PORT_TYPE type,
               U8 portNum,
               PORT_STATUS_CONTROL statusControl) : BrickMk2LinkPort(
               idOfOwningDevice, type, portNum, statusControl)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Copy constructor - to allow copies of BrickMk3LinkPort objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    *
    */
    BrickMk3LinkPort(const BrickMk3LinkPort& portToCopyFrom) :
        BrickMk2LinkPort()
    {
        /* copy contents of object being copied into this object */
        owningDeviceForPort = portToCopyFrom.owningDeviceForPort;
        portType = portToCopyFrom.portType;
        portNumber = portToCopyFrom.portNumber;
        portStatusControl = portToCopyFrom.portStatusControl;
    }

    /**
    * Overloaded assignment operator - to allow instances of BrickMk3LinkPort
    * objects to have their contents assigned to each other.
    *
    */
    BrickMk3LinkPort& operator=(const BrickMk3LinkPort& portBeingAssigned)
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
    virtual ~BrickMk3LinkPort()
    {
        /* no resources to free at present */
    }

    /**
    * Gets the base transmit clock parameters for the link.
    *
    * \note This function is currently for use with Brick Mk3 devices before
    * hardware version v1.01. \n
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
        return CFG_BRICK_MK3_getBaseTransmitClock(owningDeviceForPort,
                portNumber,
                clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the base transmit clock rate on this link. The transmit rate of a
    * link is given by:
    *
    *  \f[
    *   Link Clock Rate = \frac{100 \mathrm{MHz} \times MULTIPIER}{DIVISOR} \times 2
    *  \f]
    *
    * The \f$MULTIPIER\f$ and \f$DIVISOR\f$ are properties of the
    * \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    * class, passed as a parameter.
    * The output clock rate must be between 5 MHz and 200 MHz.
    *
    * \note This function is currently for use with Brick Mk3 devices before
    * hardware version v1.01. \n
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
    int SetBaseTransmitClock(
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_BRICK_MK3_setBaseTransmitClock(owningDeviceForPort,
                portNumber,
                *clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Gets the transmit clock rate parameters for the link.
    *
    * \note This function is currently for use with Brick Mk3 devices from
    * hardware version v1.01.
    * For earlier versions use GetBaseTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param[out] pClockRateParams Pointer to value that will be updated with
    *                              the given link's transmit clock rate
    *                              parameters.
    *
    * @return 1 if transmit clock frequency parameters were successfully
    *         obtained, else 0.
    *
    */
    int GetTransmitClock(
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to get the transmit clock params for this link */
        return CFG_BRICK_MK3_getTransmitClock(owningDeviceForPort, portNumber,
            clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the transmit clock rate on the link. The transmit rate of a link is
    * given by:
    *
    *  \f[
    *   Link Clock Rate = \frac{100 \mathrm{MHz} \times MULTIPIER}{DIVISOR}  \times 2
    *  \f]
    *
    * The \f$MULTIPIER\f$ and \f$DIVISOR\f$ are properties of the
    * \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    * class, passed as a parameter. The output clock rate will be between 1 MHz
    * and 200 MHz giving data rates between 2 Mbit/s and 400 Mbit/s.
    *
    * Note that for data rates below 10 Mbit/s, an exact rate may not be
    * achievable and it may be rounded up or down slightly.
    *
    * \note This function is currently for use with Brick Mk3 devices from
    * hardware version v1.01.
    * For earlier versions use SetBaseTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which contains the clock parameters to set.
    *
    * @return 1 if transmit clock frequency parameters were successfully set,
    *         otherwise 0.
    *
    */
    int SetTransmitClock(
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_BRICK_MK3_setTransmitClock(owningDeviceForPort, portNumber,
            *clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }
};

/* end namespace tags */
} /* brickmk3 */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

