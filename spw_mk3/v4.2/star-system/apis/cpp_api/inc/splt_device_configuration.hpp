/**
 * \file splt_device_configuration.hpp
 *
 * \brief Provides the configuration for a SPLT device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides the configuration for a SPLT device.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 *
 * \ingroup CPP_SPLT_CONFIG
 */

#ifndef __SPLT_CONFIGURATION_CPP__
#define __SPLT_CONFIGURATION_CPP__

#include "pcie_device_configuration.hpp"
#include "clock_rate_params.hpp"
#include "splt_link_port.hpp"
#include "splt_external_port.hpp"
#include "splt_config_port.hpp"
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
 * to SPLT devices.
 **/
namespace splt
{

/**
* Provides the configuration for a SPLT device.
*/
class SPLTDeviceConfiguration : public stardundee::com::starsystem::deviceconfig::mk2devices::pcie::PCIeDeviceConfiguration
{
protected:
    /**
    * Create an instance of a SPLTLinkPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a SPLTLinkPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateLinkPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
        U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        SPLTLinkPort *linkPort = NULL;

        try
        {
            linkPort = new SPLTLinkPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the SPLTLinkPort object*/
            linkPort = NULL;
        }

        /* return new SPLTLinkPort object, or NULL if failed to allocate */
        return linkPort;
    }

    /**
    * Create an instance of a SPLTExternalPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a SPLTExternalPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateExternalPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        SPLTExternalPort *externalPort = NULL;

        try
        {
            externalPort = new SPLTExternalPort(id, type, portNumber,
                portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the SPLTExternalPort object*/
            externalPort = NULL;
        }

        /* return new SPLTExternalPort object, or NULL if failed to allocate */
        return externalPort;
    }

    /**
    * Create an instance of a SPLTConfigPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a SPLTConfigPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateConfigPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        SPLTConfigPort *configPort = NULL;

        try
        {
            configPort = new SPLTConfigPort(id, type, portNumber,
                portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the SPLTConfigPort object*/
            configPort = NULL;
        }

        /* return new SPLTConfigPort object, or NULL if failed to allocate */
        return (Port*)configPort;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    SPLTDeviceConfiguration() : PCIeDeviceConfiguration()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this configuration state is
    *                 associated with and describes.
    */
    explicit SPLTDeviceConfiguration(STAR_DEVICE_ID deviceID) :
        PCIeDeviceConfiguration(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~SPLTDeviceConfiguration()
    {
    }
};

/* end namespace tags */
} /* splt */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

