/**
 * \file brick_mk2_device_configuration.hpp
 *
 * \brief Provides the configuration for a Brick Mk2 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides the configuration for a Brick Mk2 device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_BRICK_MK2_CONFIG
 */

#ifndef __BRICK_Mk2_CONFIGURATION_CPP__
#define __BRICK_Mk2_CONFIGURATION_CPP__

#include "mk2_device_configuration.hpp"
#include "cfg_api_brick_mk2.h"
#include "brick_mk2_errors.hpp"
#include "brick_mk2_link_port.hpp"
#include "brick_mk2_external_port.hpp"
#include "brick_mk2_config_port.hpp"
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
 * to Brick Mk2 devices.
 **/
namespace brickmk2
{

/**
* Provides the configuration for a Brick Mk2 device.
*/
class BrickMk2DeviceConfiguration : public Mk2DeviceConfiguration
{
protected:
    /**
    * Create an instance of a BrickMk2LinkPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a BrickMk2LinkPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateLinkPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        BrickMk2LinkPort *linkPort = NULL;

        try
        {
            linkPort = new BrickMk2LinkPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);

            /* failed to allocate memory for the BrickMk2LinkPort object*/
            linkPort = NULL;
        }

        /* return new BrickMk2LinkPort object, or NULL if failed to allocate */
        return linkPort;
    }

    /**
    * Create an instance of a BrickMk2ExternalPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a BrickMk2ExternalPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateExternalPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        BrickMk2ExternalPort *externalPort = NULL;

        try
        {
            externalPort = new BrickMk2ExternalPort(id, type, portNumber,
                                            portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);

            /* failed to allocate memory for the BrickMk2ExternalPort object*/
            externalPort = NULL;
        }

        /* return new BrickMk2ExternalPort object, or NULL if failed to allocate */
        return externalPort;
    }

    /**
    * Create an instance of a BrickMk2ConfigPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a BrickMk2ConfigPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateConfigPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        BrickMk2ConfigPort *configPort = NULL;

        try
        {
            configPort = new BrickMk2ConfigPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);

            /* failed to allocate memory for the BrickMk2ConfigPort object*/
            configPort = NULL;
        }

        /* return new BrickMk2ConfigPort object, or NULL if failed to allocate */
        return (Port*)configPort;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    BrickMk2DeviceConfiguration() : Mk2DeviceConfiguration()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this configuration state is
    *                 associated with and describes.
    */
    explicit BrickMk2DeviceConfiguration(STAR_DEVICE_ID deviceID) :
                             Mk2DeviceConfiguration(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~BrickMk2DeviceConfiguration()
    {
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
    virtual int GetPortRoutingAddress(U8 portNumber, _Out_ U8 *pAddress)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_getPortRoutingAddress(idOfOwningDevice, portNumber,
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
    virtual int SetPortRoutingAddress(U8 portNumber, U8 address)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_setPortRoutingAddress(idOfOwningDevice, portNumber,
                                                   address);
    }

    /**
    * Get the clock frequency for a given link.
    *
    * @param linkNum Link to get clock frequency for.
    * @param[out] pLinkFreq Pointer to a value that will be updated with the
    *                       link's clock frequency.
    *
    * @return 1 clock frequency successfully obtained, otherwise 0.
    *
    */
    int GetLinkClockFrequency(U8 linkNum,
                              _Out_ BrickMk2LinkPort::LinkFrequency *pLinkFreq)
    {
        /* call 'c' API to get the clock frequency for this link */
        STAR_CFG_BRICK_MK2_LINK_FREQ linkFreq;
        int result = CFG_BRICK_MK2_getLinkClockFrequency(idOfOwningDevice,
                                                   linkNum, &linkFreq);

        /* store frequency in input variable provided */
        (*pLinkFreq) = (BrickMk2LinkPort::LinkFrequency)linkFreq;

        /* return result */
        return result;
    }

    /**
    * Set the clock frequency for a given link.
    *
    * @param linkNum Link to set clock frequency for.
    * @param[out] linkFreq Link clock frequency to be set.
    *
    * @return 1 clock frequency successfully set, otherwise 0.
    *
    */
    int SetLinkClockFrequency(U8 linkNum,
                              BrickMk2LinkPort::LinkFrequency linkFreq)
    {
        /* call 'c' API to get the clock frequency for this link */
        return CFG_BRICK_MK2_setLinkClockFrequency(idOfOwningDevice,
                              linkNum, (STAR_CFG_BRICK_MK2_LINK_FREQ)linkFreq);
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
    virtual int EnableInterfaceModeOnPort(U8 portNumber)
    {
        /* call 'c' API to enable interface mode on this port*/
        return CFG_BRICK_MK2_enableInterfaceModeOnPort(idOfOwningDevice,
                                                       portNumber);
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
    virtual int GetInterfaceModeEnabledOnPort(U8 portNumber, _Out_ int *pEnabled)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_getInterfaceModeOnPortEnabled(idOfOwningDevice,
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
    virtual int DisableInterfaceModeOnPort(U8 portNumber)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_disableInterfaceModeOnPort(idOfOwningDevice,
                                                        portNumber);
    }

    /**
    * Enable identification of the source port of a received packet on a given
    * port, when in interface mode.
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
    virtual int EnableSourceIdentificationOnPort(U8 portNumber)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_enableIdentifySourceOnPort(idOfOwningDevice,
                                                        portNumber);
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
    virtual int GetSourceIdentificationEnabledOnPort(U8 portNumber,
                                                    _Out_ int *pEnabled)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_getIdentifySourceOnPortEnabled(idOfOwningDevice,
                                                            portNumber,
                                                            pEnabled);
    }

    /**
    * Disable source identification for a given port.
    *
    * @param portNumber The number of the port to enable.
    *
    * @return 1 if source identification was successfully disabled, else 0.
    *
    */
    virtual int DisableSourceIdentificationOnPort(U8 portNumber)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_disableIdentifySourceOnPort(idOfOwningDevice,
                                                         portNumber);
    }

    /**
    * This function is not not supported on the Brick Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int InjectError(U8 portNumber, SPW_ERROR error)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(portNumber);
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(error);

        /* throw exception - function not supported for Brick Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * Inject the specified errors on a given port.
    *
    * @param portNumber The port to inject the errors on.
    * @param errors A reference to a \link stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2Errors BrickMk2Errors\endlink
    *               object which specifies the errors to inject on the given
    *               port.
    *
    * @return 1 if the errors were successfully injected, otherwise 0.
    *
    */
    int InjectErrors(U8 portNumber, BrickMk2Errors& errors)
    {
        /* call 'c' API */
        return CFG_BRICK_MK2_injectErrors(idOfOwningDevice, portNumber,
                                       errors.GetAsSTAR_CFG_BRICK_MK2_ERRORS());
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
        /* call 'c' API version */
        return CFG_BRICK_MK2_enableStateChangeEventsOnPort(idOfOwningDevice,
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
        /* call 'c' API version */
        return CFG_BRICK_MK2_getStateChangeEventsOnPortEnabled(idOfOwningDevice,
                                                               portNumber,
                                                               pEnabled);
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
        /* call 'c' API version */
        return CFG_BRICK_MK2_disableStateChangeEventsOnPort(idOfOwningDevice,
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
        /* call 'c' API version */
        return CFG_BRICK_MK2_enableSpeedChangeEventsOnPort(idOfOwningDevice,
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
        /* call 'c' API version */
        return CFG_BRICK_MK2_getSpeedChangeEventsOnPortEnabled(
                                                            idOfOwningDevice,
                                                            portNumber,
                                                            pEnabled);
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
        /* call 'c' API version */
        return CFG_BRICK_MK2_disableSpeedChangeEventsOnPort(idOfOwningDevice,
                                                            portNumber);
    }

    /**
    * Gets the current link speed measured on a given port.
    *
    * @param portNumber The port to get the measured link speed of.
    * @param[out] pLinkSpeed User supplied value that will be updated to contain
    *                        the measured link speed in 100 KHz units.
    *
    * @return 1 if the information was successfully obtained, else 0.
    *
    */
    int GetMeasuredLinkSpeed(U8 portNumber, _Out_ U16 *pLinkSpeed)
    {
        /* call 'c' API version */
        return CFG_MK2_getMeasuredLinkSpeed(idOfOwningDevice,
                                            portNumber,
                                            pLinkSpeed);
    }
};

/* end namespace tags */
} /* brickmk2 */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

