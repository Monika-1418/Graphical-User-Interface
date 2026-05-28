/**
 * \file brick_mk3_device_configuration.hpp
 *
 * \brief Provides the configuration for a Brick Mk3 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides the configuration for a Brick Mk3 device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_BRICK_MK3_CONFIG
 */

#ifndef __BRICK_MK3_CONFIGURATION_CPP__
#define __BRICK_MK3_CONFIGURATION_CPP__

#include "brick_mk2_device_configuration.hpp"
#include "cfg_api_brick_mk3.h"
#include "brick_mk3_link_port.hpp"
#include "brick_mk3_config_port.hpp"
#include "brick_mk3_external_port.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee
{
namespace com
{
namespace starsystem
{
namespace deviceconfig
{
namespace mk2devices
{
/**
 * Classes required to perform device configuration tasks which are specific
 * to Brick Mk3 devices.
 **/
namespace brickmk3
{

/**
* Provides the configuration for a Brick Mk3 device.
*/
class BrickMk3DeviceConfiguration : public stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2DeviceConfiguration
{
protected:
    /**
    * Create an instance of a BrickMk3LinkPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a BrickMk3LinkPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateLinkPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
        U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        BrickMk3LinkPort *linkPort = NULL;

        try
        {
            linkPort = new BrickMk3LinkPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);

            /* failed to allocate memory for the BrickMk3LinkPort object */
            linkPort = NULL;
        }

        /* return new BrickMk3LinkPort object, or NULL if failed to allocate */
        return linkPort;
    }

    /**
    * Create an instance of a BrickMk3ExternalPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a BrickMk3ExternalPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateExternalPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        BrickMk3ExternalPort *externalPort = NULL;

        try
        {
            externalPort = new BrickMk3ExternalPort(id, type, portNumber,
                                            portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the BrickMk3ExternalPort object*/
            externalPort = NULL;
        }

        /* return new BrickMk3ExternalPort object, or NULL if failed to allocate */
        return externalPort;
    }

    /**
    * Create an instance of a BrickMk3ConfigPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a BrickMk3ConfigPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateConfigPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        BrickMk3ConfigPort *configPort = NULL;

        try
        {
            configPort = new BrickMk3ConfigPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the BrickMk3ConfigPort object*/
            configPort = NULL;
        }

        /* return new BrickMk3ConfigPort object, or NULL if failed to allocate */
        return (Port*)configPort;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    BrickMk3DeviceConfiguration() : BrickMk2DeviceConfiguration()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this configuration state is
    *                 associated with and describes.
    */
    explicit BrickMk3DeviceConfiguration(STAR_DEVICE_ID deviceID) :
        BrickMk2DeviceConfiguration(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~BrickMk3DeviceConfiguration()
    {
    }

    /**
    * Gets the base transmit clock parameters for a given link.
    *
    * \note This function is currently for use with Brick Mk3 devices before
    * hardware version v1.01. \n
    * For later versions use GetTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param linkNum The link to get the base transmit clock rate for.
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which will receive the clock parameters
    *                    obtained.
    *
    * @return 1 if base transmit clock frequency parameters were successfully
    *         obtained, otherwise 0.
    *
    */
    int GetBaseTransmitClock(U8 linkNum,
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to get the base transmit clock params for this link */
        return CFG_BRICK_MK3_getBaseTransmitClock(idOfOwningDevice, linkNum,
            clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the base transmit clock parameters for a given link. The transmit
    * rate of a link is given by:
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
    * @param linkNum The link to set the base transmit clock rate for.
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which contains the clock parameters to set.
    *
    * @return 1 if base transmit clock frequency parameters were successfully
    *         set, otherwise 0.
    *
    */
    int SetBaseTransmitClock(U8 linkNum,
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_BRICK_MK3_setBaseTransmitClock(idOfOwningDevice, linkNum,
            *clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Gets the transmit clock rate parameters for a given link.
    *
    * \note This function is currently for use with Brick Mk3 devices from
    * hardware version v1.01.
    * For earlier versions use GetBaseTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param linkNum Link to get the transmit clock rate for.
    * @param[out] pClockRateParams Pointer to value that will be updated with
    *                              the given link's transmit clock rate
    *                              parameters.
    *
    * @return 1 if transmit clock frequency parameters were successfully
    *         obtained, else 0.
    *
    */
    int GetTransmitClock(U8 linkNum,
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to get the transmit clock params for this link */
        return CFG_BRICK_MK3_getTransmitClock(idOfOwningDevice, linkNum,
            clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the transmit clock rate on a given link. The transmit rate of a link
    * is given by:
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
    * @param linkNum The link to set the base transmit clock rate for.
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which contains the clock parameters to set.
    *
    * @return 1 if transmit clock frequency parameters were successfully set,
    *         otherwise 0.
    *
    */
    int SetTransmitClock(U8 linkNum,
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_BRICK_MK3_setTransmitClock(idOfOwningDevice, linkNum,
            *clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the timestamping method to use for the given device.
    *
    * \note This function is currently for use with \ref BrickMk3 devices from
    * hardware version v1.02.
    * \n\n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    * \n\n
    * The Brick Mk3 will trigger on the rising edge of the external pulse but 
    * the Triggering API can be used to change it to trigger on the falling 
    * edge using BrickMk3TriggerConfiguration::EnableExtTriggerEdgeDetectMode() 
    * and BrickMk3TriggerConfiguration::EnableExtTriggerInvert().
    *
    * @param timestampMethod Timestamp method to enable for device.
    *
    * @return 1 if timestamp method was successfully set, else 0.
    *
    */
    int SetTimestampMethod(STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD timestampMethod)
    {
        /* call 'c' API to set the timestamp method */
        return CFG_BRICK_MK3_setTimestampMethod(idOfOwningDevice, 
            timestampMethod);
    }

    /**
    * Gets the timestamping method that is currently in use for the given device.
    *
    * \note This function is currently for use with \ref BrickMk3 devices from
    * hardware version v1.02.
    * \n\n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param[out] pTimestampMethod Pointer to value that will be updated with 
    *                              the given link's timestamp method.
    *
    * @return 1 if timestamp method was successfully obtained, else 0.
    */
    int GetTimestampMethod(
        _Out_ STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD *pTimestampMethod)
    {
        /* call 'c' API to get the timestamp method */
        return CFG_BRICK_MK3_getTimestampMethod(idOfOwningDevice, 
            pTimestampMethod);
    }

    /**
    * Selectively enables receive timestamp events on a given port.  Enabling
    * receive timestamp events will result in the timestamp that the last packet
    * was received at to be provided as a STAR_STREAM_ITEM_TYPE_RX_TIMESTAMP.
    *
    * \note This function is currently for use with \ref BrickMk3 devices from
    * hardware version v1.02.
    * \n\n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param portNum Port to enable receive timestamp events on.
    *
    * @return 1 if receive timestamp packets were successfully enabled, else 0.
    */
    int EnableRxTimestampEventsOnPort(U8 portNum)
    {
        /* call 'c' API to enable receive timestamp events on port */
        return CFG_BRICK_MK3_enableRxTimestampEventsOnPort(idOfOwningDevice, 
            portNum);
    }

    /**
    * Gets whether receive timestamp events are enabled on a specific port.
    *
    * \note This function is currently for use with \ref BrickMk3 devices from
    * hardware version v1.02.
    * \n\n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param portNum Port to get receive timestamp events for.
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      receive timestamp events are enabled, else 0.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    */
    int GetRxTimestampEventsOnPortEnabled(U8 portNum, _Out_ int *pEnabled)
    {
        /* call 'c' API to get receive timestamp events on port enabled */
        return CFG_BRICK_MK3_getRxTimestampEventsOnPortEnabled(
            idOfOwningDevice, portNum, pEnabled);
    }

    /**
    * Selectively disables receive timestamp events on a given port.  Disabling
    * receive timestamp events will result in no timestamp information being
    * provided after the packet.
    *
    * \note This function is currently for use with \ref BrickMk3 devices from
    * hardware version v1.02.
    * \n\n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param portNum Port to disable receive timestamp events on.
    *
    * @return 1 if receive timestamp events were successfully disabled, else 0.
    */
    int DisableRxTimestampEventsOnPort(U8 portNum)
    {
        /* call 'c' API to disable receive timestamp events on port enabled */
        return CFG_BRICK_MK3_disableRxTimestampEventsOnPort(idOfOwningDevice, 
            portNum);
    }

    /**
    * Sets the current timestamp value for the given device which will be
    * incremented on the next synchronisation pulse.
    *
    * \note This function is currently for use with \ref BrickMk3 devices from
    * hardware version v1.02.
    * \n\n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param value Timestamp value to set for device.
    *
    * @return 1 if timestamp value was successfully set, else 0.
    */
    int SetTimestampValue(U32 value)
    {
        /* call 'c' API to set timestamp value */
        return CFG_BRICK_MK3_setTimestampValue(idOfOwningDevice, value);
    }

    /**
    * Gets the current timestamp value.
    *
    * \note This function is currently for use with \ref BrickMk3 devices from
    * hardware version v1.02.
    * \n\n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param[out] pTimestampMethod Pointer to value that will be updated with 
    *                              the given link's timestamp value.
    *
    * @return 1 if timestamp value was successfully obtained, else 0.
    */
    int GetTimestampValue(_Out_ U32 *pValue)
    {
        /* call 'c' API to get timestamp value */
        return CFG_BRICK_MK3_getTimestampValue(idOfOwningDevice, pValue);
    }

    /**
    * Sets the frequency of each generated pulse.
    *
    * \note This function is currently for use with \ref BrickMk3 devices from
    * hardware version v1.02.
    * \n\n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param frequency The frequency value to set.
    *
    * @return 1 if frequency was successfully set, else 0.
    */
    int SetPulseGeneratorFrequency(STAR_CFG_BRICK_MK3_PULSE_FREQ frequency)
    {
        /* call 'c' API to set pulse generator frequency */
        return CFG_BRICK_MK3_setPulseGeneratorFrequency(idOfOwningDevice, 
            frequency);
    }

    /**
    * Gets the frequency of each generated pulse.
    *
    * \note This function is currently for use with \ref BrickMk3 devices from
    * hardware version v1.02.
    * \n\n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param[out] pFrequency Pointer to value that will be updated with the
    *                        frequency between each pulse cycle.
    *
    * @return 1 if frequency value was successfully obtained, else 0.
    */
    int GetPulseGeneratorFrequency(
        _Out_ STAR_CFG_BRICK_MK3_PULSE_FREQ *pFrequency)
    {
        /* call 'c' API to get pulse generator frequency */
        return CFG_BRICK_MK3_getPulseGeneratorFrequency(idOfOwningDevice, 
            pFrequency);
    }
};

/* end namespace tags */
} /* brickmk3 */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

