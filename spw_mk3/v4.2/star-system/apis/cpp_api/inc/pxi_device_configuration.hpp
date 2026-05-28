/**
 * \file pxi_device_configuration.hpp
 *
 * \brief Provides the configuration for a PXI device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides the configuration for a PXI device.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 *
 * \ingroup CPP_PXI_CONFIG
 */

#ifndef __PXI_CONFIGURATION_CPP__
#define __PXI_CONFIGURATION_CPP__

#include "mk2_device_configuration.hpp"
#include "clock_rate_params.hpp"
#include "cfg_api_pxi.h"
#include "pxi_link_port.hpp"
#include "pxi_config_port.hpp"
#include "pxi_external_port.hpp"
#include "cpp_api_macros.hpp"
#include "not_supported_exception.hpp"
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
 * to PXI devices.
 **/
namespace pxi
{

/**
* Provides the configuration for a PXI device.
*/
class PXIDeviceConfiguration : public stardundee::com::starsystem::deviceconfig::mk2devices::Mk2DeviceConfiguration
{
protected:
    /**
    * Create an instance of a PXILinkPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a PXILinkPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateLinkPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        PXILinkPort *linkPort = NULL;

        try
        {
            linkPort = new PXILinkPort(id, type, portNumber, portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the PXILinkPort object*/
            linkPort = NULL;
        }

        /* return new PXILinkPort object, or NULL if failed to allocate */
        return linkPort;
    }

    /**
    * Create an instance of a PXIExternalPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a PXIExternalPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateExternalPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        PXIExternalPort *externalPort = NULL;

        try
        {
            externalPort = new PXIExternalPort(id, type, portNumber,
                portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the PXIExternalPort object*/
            externalPort = NULL;
        }

        /* return new PXIExternalPort object, or NULL if failed to allocate */
        return externalPort;
    }

    /**
    * Create an instance of a PXIConfigPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a PXIConfigPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateConfigPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        PXIConfigPort *configPort = NULL;

        try
        {
            configPort = new PXIConfigPort(id, type, portNumber,
                portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the PXIConfigPort object*/
            configPort = NULL;
        }

        /* return new PXIConfigPort object, or NULL if failed to allocate */
        return (Port*)configPort;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PXIDeviceConfiguration() : stardundee::com::starsystem::deviceconfig::mk2devices::Mk2DeviceConfiguration()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this configuration state is
    *                 associated with and describes.
    */
    explicit PXIDeviceConfiguration(STAR_DEVICE_ID deviceID) :
        Mk2DeviceConfiguration(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~PXIDeviceConfiguration()
    {
    }

    /**
    * Gets the base transmit clock parameters for a given link.
    *
    * \note This function is currently for use with PXI devices before hardware
    * version V1.1.
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
        return CFG_PXI_getBaseTransmitClock(idOfOwningDevice, linkNum,
            clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the base transmit clock parameters for a given link. The transmit
    * rate of a link is given by:
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
    int SetBaseTransmitClock(U8 linkNum,
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_PXI_setBaseTransmitClock(idOfOwningDevice, linkNum,
            *clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Gets the transmit clock parameters for a given link.
    *
    * \note This function is currently for use with PXI devices from hardware
    * version V1.1.
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
    int GetTransmitClock(U8 linkNum,
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to get the transmit clock params for this link */
        return CFG_PXI_getTransmitClock(idOfOwningDevice, linkNum,
            clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * Sets the transmit clock parameters for a given link. The transmit rate of
    * a link is given by:
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
    int SetTransmitClock(U8 linkNum,
        stardundee::com::starsystem::deviceconfig::mk2devices::pcie::ClockRateParams& clockParams)
    {
        /* call 'c' API to set the base transmit clock params for this link */
        return CFG_PXI_setTransmitClock(idOfOwningDevice, linkNum,
            *clockParams.GetAsSTAR_CFG_MK2_BASE_TRANSMIT_CLOCK());
    }

    /**
    * This function is not not supported on the PXI.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int SetLinkRateDivider(U8 linkNum, U8 divider)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(linkNum);
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(divider);

        /* throw exception - function not supported for PXI */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PXI.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int GetLinkRateDivider(U8 linkNum, _Out_ U8* divider)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(linkNum);
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(divider);

        /* throw exception - function not supported for PXI */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * Sets the Link rate divider for a PXI device. The transmit rate of a link
    * is given by:
    *
    *  \f[
    *  \frac{Link Clock Rate}{Link Clock Rate Divider}
    *  \f]
    *
    * Where \f$Link Clock Rate\f$ is set by the appropriate function (i.e.
    * SetBaseTransmitClock()) and
    * \f$Link Clock Rate Divider =\f$ divider.
    *
    * \note It is never necessary to set a link rate divider higher than 100,
    *       as the minimum transmit rate permitted by SpaceWire is
    *       2 Mbit/s (200 Mbit/s Max / 100).
    *
    * \note If an odd number (other than 1) is specified as the divider
    *       parameter, the previous even integer will be used instead. For
    *       example: if a divider value of 3 is specified, the actual divider
    *       set shall be 2.
    *
    * \note This function is currently for use with PXI devices before hardware
    *       version V1.1. No check is made by this function to ensure it is
    *       being used with a compatible device.
    *
    * @param divider Value of the new divider. Valid input: 1, Even numbers 2
    *                through 126.
    *
    * @return 1 if divider was successfully set, else 0.
    */
    int SetLinkRateDivider(U8 divider)
    {
        /* call 'c' API to set the link rate divider */
        return CFG_PXI_setLinkRateDivider(idOfOwningDevice, divider);
    }

    /**
    * Gets the Link rate divider for a PXI device.
    *
    * \note This function is currently for use with PXI devices before hardware
    * version V1.1.
    * No check is made by this function to ensure it is being used with a
    * compatible device.
    *
    * @param[out] pDivider Value of the divider.
    *
    * @return 1 if was successfully obtained, else 0.
    */
    int GetLinkRateDivider(_Out_ U8 *pDivider)
    {
        /* call 'c' API to get the link rate divider */
        return CFG_PXI_getLinkRateDivider(idOfOwningDevice, pDivider);
    }

    /**
    * Gets the address that a packet received on a specified Port should be
    * routed to when interface mode is enabled.
    *
    * \note This is an advanced feature and not necessary for normal usage.
    *
    * @param portNumber The port to get the routing address from.
    * @param[out] pAddress Pointer to a value that will be updated to contain
    *                      the address.
    *
    * @return 1 if the address could be obtained, else 0.
    *
    */
    int GetPortRoutingAddress(U8 portNumber, _Out_ U8 *pAddress)
    {
        /* call 'c' API to get the port routing address */
        return CFG_PXI_getPortRoutingAddress(idOfOwningDevice, portNumber,
            pAddress);
    }

    /**
    * Sets the address that a packet received on a specified port should be
    * routed to when interface mode is enabled.
    *
    * \note This is an advanced feature and not necessary for normal usage.
    *
    * @param portNumber The port to set the routing address for.
    * @param address The routing address for the port.
    *
    * @return 1 if the address could be set, else 0.
    *
    */
    int SetPortRoutingAddress(U8 portNumber, U8 address)
    {
        /* call 'c' API to set the port routing address */
        return CFG_PXI_setPortRoutingAddress(idOfOwningDevice, portNumber,
            address);
    }

    /**
    * Enable interface mode on a given port.
    *
    * \note Interface mode must be enabled globally on the device which owns
    *       this port (by calling EnableInterfaceMode() on the device's
    *       configuration state) in order for interface mode on a specific port
    *       to be enabled.
    *
    * @param portNumber The number of the port to enable.
    *
    * @return 1 if interface mode was successfully enabled, else 0.
    *
    */
    int EnableInterfaceModeOnPort(U8 portNumber)
    {
        /* call 'c' API to enable interface mode on the given port */
        return CFG_PXI_enableInterfaceModeOnPort(idOfOwningDevice, portNumber);
    }

    /**
    * Gets whether interface mode has been enabled on a given port.
    *
    * @param portNumber The number of the port to check.
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      interface mode is enabled, else 0.
    *
    * @return 1 if the information could be obtained for the port, else 0.
    *
    */
    int GetInterfaceModeEnabledOnPort(U8 portNumber, _Out_ int *pEnabled)
    {
        /* call 'c' API to get interface mode on port enabled */
        return CFG_PXI_getInterfaceModeOnPortEnabled(idOfOwningDevice,
                                                     portNumber,
                                                     pEnabled);
    }

    /**
    * Disable interface mode on a given port.
    *
    * When interface mode is disabled the device operates in routing mode.
    *
    * @param portNumber The number of the port to disable.
    *
    * @return 1 if interface mode was successfully disabled, else 0.
    *
    */
    int DisableInterfaceModeOnPort(U8 portNumber)
    {
        /* call 'c' API to disable interface mode on the given port */
        return CFG_PXI_disableInterfaceModeOnPort(idOfOwningDevice, portNumber);
    }

    /**
    * Enables source identification during interface made for a given port.
    *
    * Interface mode and source identification must be enabled globally (i.e.
    * on the device which owns this port, by calling its EnableInterfaceMode()
    * and EnableSourceIdentification() methods) in order for this setting to
    * have effect.
    *
    * @param portNumber The number of the port to enable.
    *
    * @return 1 if source identification was successfully enabled, else 0.
    *
    */
    int EnableSourceIdentificationOnPort(U8 portNumber)
    {
        /* call 'c' API to enable source identification on the given port */
        return CFG_PXI_enableIdentifySourceOnPort(idOfOwningDevice, portNumber);
    }

    /**
    * Gets whether source identification has been enabled on a given port.
    *
    * @param portNumber The number of the port to check.
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      identify source is enabled, else 0.
    *
    * @return 1 if the information could be obtained, else 0.
    *
    */
    int GetSourceIdentificationEnabledOnPort(U8 portNumber, _Out_ int *pEnabled)
    {
        /* call 'c' API to get source identification on port enabled */
        return CFG_PXI_getIdentifySourceOnPortEnabled(idOfOwningDevice,
                                                      portNumber,
                                                      pEnabled);
    }

    /**
    * Disable source identification for a given port.
    *
    * @param portNumber The number of the port to disable.
    *
    * @return 1 if source identification was successfully disabled, else 0.
    *
    */
    int DisableSourceIdentificationOnPort(U8 portNumber)
    {
        /* call 'c' API to disable source identification on the given port */
        return CFG_PXI_disableIdentifySourceOnPort(idOfOwningDevice,
            portNumber);
    }

    /**
    * Immediately injects the specified error on a given device's port.
    *
    * @param portNumber Port the error is to be injected on.
    * @param errors SpaceWire error to be injected.
    *
    * @return 1 if the error information was successfully sent to the device,
    *           else 0.
    *
    */
    int InjectError(U8 portNumber, SPW_ERROR error)
    {
        /* call 'c' API to inject the error on the given port */
        return CFG_PXI_injectError(idOfOwningDevice, portNumber, error);
    }

    /**
    * Enables state change events on a given port.
    *
    * @param portNumber The port to enable state change events on.
    *
    * Enabling state change events will result in state change event traffic
    * being received on channel 0 when the link changes to running or
    * disconnected.
    *
    * @return 1 if state change events were successfully enabled, else 0.
    *
    */
    int EnableStateChangeEventsOnPort(U8 portNumber)
    {
        /* call 'c' API to enable state change events on the given port */
        return CFG_PXI_enableStateChangeEventsOnPort(idOfOwningDevice,
            portNumber);
    }

    /**
    * Gets whether state change events are enabled on a given port.
    *
    * @param portNumber The port to check.
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      state change events are enabled, else 0.
    *
    * @return 1 if the information was successfully obtained, else 0.
    *
    */
    int GetStateChangeEventsEnabledOnPort(U8 portNumber, _Out_ int *pEnabled)
    {
        /* call 'c' API to get state change events enabled on the given port */
        return CFG_PXI_getStateChangeEventsOnPortEnabled(idOfOwningDevice,
            portNumber, pEnabled);
    }

    /**
    * Disables state change events on a given port.
    *
    * @param portNumber The port to disable state change events on.
    *
    * Disabling state change events will result in state change event traffic
    * no longer being received on channel 0 when the link changes to running or
    * disconnected.
    *
    * @return 1 if state change events were successfully disabled, else 0.
    *
    */
    int DisableStateChangeEventsOnPort(U8 portNumber)
    {
        /* call 'c' API to disable state change events on the given port */
        return CFG_PXI_disableStateChangeEventsOnPort(idOfOwningDevice,
            portNumber);
    }

    /**
    * Enables speed change events on a given port.
    *
    * @param portNumber The port to enable speed change events on.
    *
    * Enabling speed change events will result in speed change event traffic
    * being received on channel 0 when the link speed changes.
    *
    * @return 1 if speed change events were successfully enabled, else 0.
    *
    */
    int EnableSpeedChangeEventsOnPort(U8 portNumber)
    {
        /* call 'c' API to enable speed change events on the given port */
        return CFG_PXI_enableSpeedChangeEventsOnPort(idOfOwningDevice,
            portNumber);
    }

    /**
    * Gets whether speed change events are enabled on a given port.
    *
    * @param portNumber The port to check.
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      speed change events are enabled, else 0.
    *
    * @return 1 if the information was successfully obtained, else 0.
    *
    */
    int GetSpeedChangeEventsEnabledOnPort(U8 portNumber, _Out_ int *pEnabled)
    {
        /* call 'c' API to get speed change events enabled on the given port */
        return CFG_PXI_getSpeedChangeEventsOnPortEnabled(idOfOwningDevice,
            portNumber, pEnabled);
    }

    /**
    * Disables speed change events on a given port.
    *
    * @param portNumber The port to disable speed change events on.
    *
    * Disabling speed change events will result in speed change event traffic
    * no longer being received on channel 0 when the link speed changes.
    *
    * @return 1 if speed change events were successfully disabled, else 0.
    *
    */
    int DisableSpeedChangeEventsOnPort(U8 portNumber)
    {
        /* call 'c' API to disable speed change events on the given port */
        return CFG_PXI_disableSpeedChangeEventsOnPort(idOfOwningDevice,
            portNumber);
    }
};

/* end namespace tags */
} /* brickmk3 */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

