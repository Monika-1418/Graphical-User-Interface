/**
 * \file router_mk2s_device_configuration.hpp
 *
 * \brief Provides the configuration for a Router Mk2S device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides the configuration for a Router Mk2S device.
 *
 * Copyright &copy; 2014 STAR-Dundee Ltd
 *
 * \ingroup CPP_ROUTER_MK2S_CONFIG
 */

#ifndef __ROUTER_Mk2S_CONFIGURATION_CPP__
#define __ROUTER_Mk2S_CONFIGURATION_CPP__

#include "brick_mk2_device_configuration.hpp"
#include "cfg_api_router_mk2s.h"
#include "router_mk2s_link_port.hpp"
#include "router_mk2s_config_port.hpp"
#include "router_mk2s_external_port.hpp"
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
 * to Router Mk2s devices.
 **/
namespace routermk2s
{

/**
* Provides the configuration for a Router Mk2S device.
*/
class RouterMk2SDeviceConfiguration : public stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2DeviceConfiguration
{
protected:
    /**
    * Create an instance of a RouterMk2SLinkPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a RouterMk2SLinkPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateLinkPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        RouterMk2SLinkPort *linkPort = NULL;

        try
        {
            linkPort = new RouterMk2SLinkPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the RouterMk2SLinkPort object*/
            linkPort = NULL;
        }

        /* return new RouterMk2SLinkPort object, or NULL if failed to allocate */
        return linkPort;
    }

    /**
    * Create an instance of a RouterMk2SExternalPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a RouterMk2SExternalPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateExternalPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        RouterMk2SExternalPort *externalPort = NULL;

        try
        {
            externalPort = new RouterMk2SExternalPort(id, type, portNumber,
                                            portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the RouterMk2SExternalPort object*/
            externalPort = NULL;
        }

        /* return new RouterMk2SExternalPort object, or NULL if failed to allocate */
        return externalPort;
    }

    /**
    * Create an instance of a RouterMk2SConfigPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a RouterMk2SConfigPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateConfigPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        RouterMk2SConfigPort *configPort = NULL;

        try
        {
            configPort = new RouterMk2SConfigPort(id, type, portNumber,
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
    RouterMk2SDeviceConfiguration() : stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2::BrickMk2DeviceConfiguration()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this configuration state is
    *                 associated with and describes.
    */
    explicit RouterMk2SDeviceConfiguration(STAR_DEVICE_ID deviceID) :
        BrickMk2DeviceConfiguration(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~RouterMk2SDeviceConfiguration()
    {
    }

    /**
    * Enables precision transmit rate on this Router Mk2S device.
    *
    * Precision transmit rate allows the transmit rate of the device to be
    * expressed accurately in Mbit/s as a floating point number using the
    * SetPrecisionTransmitRate() function. Note that after being enabled it can
    * take up to 250 microseconds before precision transmit rate is actually
    * used. Call GetPrecisionTransmitRateInUse() to determine if precision
    * transmit rate is actually in use.
    *
    * @return 1 if precision transmit rate was successfully enabled, else 0.
    *
    */
    int EnablePrecisionTransmitRate()
    {
        /* call 'c' API */
        return CFG_ROUTER_MK2S_enablePrecisionTransmitRate(idOfOwningDevice);
    }

    /**
    * Determine if precision transmit rate is enabled on this Router Mk2S device.
    *
    * Precision transmit rate allows the transmit rate of the device to be
    * expressed accurately in Mbit/s as a floating point number using the
    * SetPrecisionTransmitRate() function. Note that although precision transmit
    * rate may be enabled, it may not yet be in use. It can take up to 250
    * microseconds before precision transmit rate is actually used. Call
    * GetPrecisionTransmitRateInUse() to determine if precision transmit rate is
    * actually in use.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      precision transmit rate is enabled, else 0.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    *
    */
    int GetPrecisionTransmitRateEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API */
        return CFG_ROUTER_MK2S_getPrecisionTransmitRateEnabled(idOfOwningDevice,
                                                               pEnabled);
    }

    /**
    * Disables precision transmit rate on this Router Mk2S device.
    *
    * Precision transmit rate allows the transmit rate of the device to be
    * expressed accurately in Mbit/s as a floating point number using the
    * SetPrecisionTransmitRate() function. Note that after being disabled there
    * can be a short delay (less than 250 microseconds) before precision
    * transmit rate is actually disabled. Call GetPrecisionTransmitRateInUse()
    * to determine if precision transmit rate is no longer in use.
    *
    * @return 1 if precision transmit rate was successfully disabled, else 0.
    *
    */
    int DisablePrecisionTransmitRate()
    {
        /* call 'c' API */
        return CFG_ROUTER_MK2S_disablePrecisionTransmitRate(idOfOwningDevice);
    }

    /**
    * Determine whether precision transmit rate is in use on this device.
    *
    * After being enabled or disabled, it can take up to 250 microseconds before
    * precision transmit is actually used or not used. This function determines
    * whether precision transmit rate is currently in use and can be used to
    * determine if an enable or disable operation has completed.
    *
    * @param[out] pInUse User supplied value that will be updated to 1 if
    *                    precision transmit rate is in use, else 0.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    *
    */
    int GetPrecisionTransmitRateInUse(_Out_ int *pInUse)
    {
        /* call 'c' API */
        return CFG_ROUTER_MK2S_getPrecisionTransmitRateInUse(idOfOwningDevice,
                                                             pInUse);
    }

    /**
    * Get the current precision transmit rate on the device in Mbit/s.
    *
    * Precision transmit rate allows the transmit rate of the device to be
    * expressed accurately in Mbit/s as a floating point number using the
    * SetPrecisionTransmitRate() function.
    *
    * @param[out] pRate User supplied value that will be updated to contain the
    *                    precision transmit rate in Mbit/s.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    *
    */
    int GetPrecisionTransmitRate(_Out_ double *pRate)
    {
        /* call 'c' API */
        return CFG_ROUTER_MK2S_getPrecisionTransmitRate(idOfOwningDevice,
                                                        pRate);
    }

    /**
    * Set the current precision transmit rate on the device in Mbit/s.
    *
    * Precision transmit rate allows the transmit rate of the device to be
    * expressed accurately in Mbit/s as a floating point number.
    *
    * @param transmitRate The precision transmit rate to set in Mbit/s.
    *
    * @return 1 if the rate for the device was successfully set, else 0.
    *
    */
    int SetPrecisionTransmitRate(double transmitRate)
    {
        /* call 'c' API */
        return CFG_ROUTER_MK2S_setPrecisionTransmitRate(idOfOwningDevice,
                                                        transmitRate);
    }
};

/* end namespace tags */
} /* routermk2s */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

