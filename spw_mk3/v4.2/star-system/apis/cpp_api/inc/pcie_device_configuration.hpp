/**
 * \file pcie_device_configuration.hpp
 *
 * \brief Provides the configuration for a PCIe device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides the configuration for a PCIe device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_PCIe_CONFIG
 */

#ifndef __PCIe_CONFIGURATION_CPP__
#define __PCIe_CONFIGURATION_CPP__

#include "mk2_device_configuration.hpp"
#include "clock_rate_params.hpp"
#include "pcie_link_port.hpp"
#include "pcie_external_port.hpp"
#include "pcie_config_port.hpp"
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
 * to PCIe devices.
 **/
namespace pcie
{

/**
* Provides the configuration for a PCIe device.
*/
class PCIeDeviceConfiguration : public Mk2DeviceConfiguration
{
protected:
    /**
    * Create an instance of a PCIeLinkPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a PCIeLinkPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateLinkPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        PCIeLinkPort *linkPort = NULL;

        try
        {
            linkPort = new PCIeLinkPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the PCIeLinkPort object*/
            linkPort = NULL;
        }

        /* return new PCIeLinkPort object, or NULL if failed to allocate */
        return linkPort;
    }

    /**
    * Create an instance of a PCIeExternalPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a PCIeExternalPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateExternalPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        PCIeExternalPort *externalPort = NULL;

        try
        {
            externalPort = new PCIeExternalPort(id, type, portNumber,
                                            portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the PCIeExternalPort object*/
            externalPort = NULL;
        }

        /* return new PCIeExternalPort object, or NULL if failed to allocate */
        return externalPort;
    }

    /**
    * Create an instance of a PCIeConfigPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a PCIeConfigPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateConfigPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        PCIeConfigPort *configPort = NULL;

        try
        {
            configPort = new PCIeConfigPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the PCIeConfigPort object*/
            configPort = NULL;
        }

        /* return new PCIeConfigPort object, or NULL if failed to allocate */
        return (Port*)configPort;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PCIeDeviceConfiguration() : Mk2DeviceConfiguration()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this configuration state is
    *                 associated with and describes.
    */
    explicit PCIeDeviceConfiguration(STAR_DEVICE_ID deviceID) :
                             Mk2DeviceConfiguration(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~PCIeDeviceConfiguration()
    {
    }

    /**
    * Gets the base transmit clock parameters for a given link.
    *
    * \note This function is currently for use with PCIe devices before
    * hardwareversion v1.11 and SPLT devices before hardware version v2.0. \n
    * For later versions use GetTransmitClock(). \n \n
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param linkNum The link to get the base transmit clock rate for.
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which will receive the clock parameters
    (                    obtained.
    *
    * @return 1 if base transmit clock frequency parameters were successfully
    *         obtained, otherwise 0.
    *
    */
    int GetBaseTransmitClock(U8 linkNum, ClockRateParams& clockParams)
    {
        /* call 'c' API to get the base transmit clock params for this link */
        return CFG_MK2_getBaseTransmitClock(idOfOwningDevice, linkNum,
                           clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the base transmit clock rate on a given link. The transmit rate of
    * a link is given by:
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
    * \note This function is currently for use with PCIe devices before
    * hardwareversion v1.11 and SPLT devices before hardware version v2.0. \n
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
    int SetBaseTransmitClock(U8 linkNum, ClockRateParams& clockParams)
    {
        /* get the STAR_CFG_MK2_BASE_TRANSMIT_CLOCK struct from the clock
         * params provided. */
        STAR_CFG_MK2_BASE_TRANSMIT_CLOCK *pParams =
                          clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK();

        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_MK2_setBaseTransmitClock(idOfOwningDevice, linkNum,
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
    * @param linkNum The link to get the transmit clock rate for.
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which will receive the clock parameters
    *                    obtained.
    *
    * @return 1 if transmit clock frequency parameters were successfully
    *         obtained, otherwise 0.
    *
    */
    int GetTransmitClock(U8 linkNum, ClockRateParams& clockParams)
    {
        /* call 'c' API to get the transmit clock params for this link */
        return CFG_MK2_getTransmitClock(idOfOwningDevice, linkNum,
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
    * structure, passed as a parameter. The output clock rate will be between
    * 1 MHz and 200 MHz giving data rates between 2 Mbit/s and 400 Mbit/s.
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
    * @param linkNum The link to set the transmit clock rate for.
    * @param clockParams A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams ClockRateParams \endlink
    *                    object which contains the clock parameters to set.
    *
    * @return 1 if base transmit clock frequency parameters were successfully
    *         set, otherwise 0.
    *
    */
    int SetTransmitClock(U8 linkNum, ClockRateParams& clockParams)
    {
        /* get the STAR_CFG_MK2_BASE_TRANSMIT_CLOCK struct from the clock
         * params provided. */
        STAR_CFG_MK2_BASE_TRANSMIT_CLOCK *pParams =
                          clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK();

        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_MK2_setTransmitClock(idOfOwningDevice, linkNum, *pParams);
    }

    /**
    * Selectively enables time-code notifications on a the channel attached to
    * a given port.
    *
    * @param portNumber The port to enable time-code events on.
    *
    * @return 1 if time-code events were successfully enabled, else 0.
    *
    */
    virtual int EnableTimeCodeEventsOnPort(U8 portNumber)
    {
        /* call 'c' API to enable time-code events on the given port */
        return CFG_MK2_enableTimeCodeEventsOnPort(idOfOwningDevice,
            portNumber);
    }

    /**
    * Gets whether time-code events are enabled on a given port.
    *
    * @param portNumber The port to check.
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      time-code events are enabled, else 0.
    *
    * @return 1 if the information was successfully obtained, else 0.
    *
    */
    virtual int GetTimeCodeEventsEnabledOnPort(U8 portNumber,
        _Out_ int *pEnabled)
    {
        /* call 'c' API to get time-code events enabled for the given port */
        return CFG_MK2_getTimeCodeEventsOnPortEnabled(idOfOwningDevice,
            portNumber, pEnabled);
    }

    /**
    * Disables time-code events on a given port.
    *
    * @param portNumber The port to disable time-code events on.
    *
    * @return 1 if time-code events were successfully disabled, else 0.
    *
    */
    virtual int DisableTimeCodeEventsOnPort(U8 portNumber)
    {
        /* call 'c' API to disable time-code events on the given port */
        return CFG_MK2_disableTimeCodeEventsOnPort(idOfOwningDevice,
            portNumber);
    }

    /**
    * Enables time-code counter bypass mode for the device. When time-code
    * counter bypass mode is enabled, any time-code which is received will be
    * forwarded out of the other ports on the router, without checking against
    * the current value of the counter. The time-code register will be updated
    * on each received time-code.
    *
    * \note This function is currently only supported by PCIe devices with
    * at least hardware version v1.12.
    *
    * @return 1 if time-code counter bypass mode was successfully enabled for
    *         the device, else 0.
    *
    */
    int EnableTimeCodeCounterBypassMode()
    {
        /* call 'c' API to enable time-code counter bypass mode */
        return CFG_MK2_enableTimeCodeCounterBypassMode(idOfOwningDevice);
    }

    /**
    * Gets whether time-code counter bypass mode has been enabled for the
    * device. When time-code counter bypass mode is disabled, any time-code
    * which is received will be checked against against the current value of
    * the counter for validity before being forwarded. When time-code counter
    * bypass mode is enabled, any time-code which is received will be forwarded
    * out of the other ports on the router, without checking against the
    * current value of the counter. The time-code register will be updated on
    * each received time-code.
    *
    * \note This function is currently only supported by PCIe devices with
    * at least hardware version v1.12.
    *
    * @param pEnabled User supplied value that will be updated to 1 if
    *                 time-code counter bypass mode is enabled for the device,
    *                 else 0.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    *
    */
    int GetTimeCodeCounterBypassModeEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API to get time-code counter bypass mode enabled */
        return CFG_MK2_getTimeCodeCounterBypassModeEnabled(idOfOwningDevice,
            pEnabled);
    }

    /**
    * Disables time-code counter bypass mode for the device. When time-code
    * counter bypass mode is disabled, any time-code which is received will be
    * checked against the current value of the counter for validity before
    * being forwarded.
    *
    * \note This function is currently only supported by PCIe devices with
    * at least hardware version v1.12.
    *
    * @return 1 if time-code counter bypass mode was successfully disabled for
    *         the device, else 0.
    *
    */
    int DisableTimeCodeCounterBypassMode()
    {
        /* call 'c' API to disable time-code counter bypass mode */
        return CFG_MK2_disableTimeCodeCounterBypassMode(idOfOwningDevice);
    }
};

/* end namespace tags */
} /* pcie */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

