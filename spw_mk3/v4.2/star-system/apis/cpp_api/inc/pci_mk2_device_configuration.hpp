/**
 * \file pci_mk2_device_configuration.hpp
 *
 * \brief Provides the configuration for a PCI Mk2 device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides the configuration for a PCI Mk2 device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_PCI_MK2_CONFIG
 */

#ifndef __PCI_Mk2_CONFIGURATION_CPP__
#define __PCI_Mk2_CONFIGURATION_CPP__

#include "mk2_device_configuration.hpp"
#include "cfg_api_pci_mk2.h"
#include "pci_mk2_link_port.hpp"
#include "pci_mk2_config_port.hpp"
#include "pci_mk2_external_port.hpp"
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
 * to PCI Mk2 devices.
 **/
namespace pcimk2
{

/**
* Provides the configuration for a PCI Mk2 device.
*/
class PCIMk2DeviceConfiguration : public Mk2DeviceConfiguration
{
protected:
    /**
    * Create an instance of a PCIMk2LinkPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a PCIMk2LinkPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateLinkPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        PCIMk2LinkPort *linkPort = NULL;

        try
        {
            linkPort = new PCIMk2LinkPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the PCIMk2LinkPort object*/
            linkPort = NULL;
        }

        /* return new PCIMk2LinkPort object, or NULL if failed to allocate */
        return linkPort;
    }

    /**
    * Create an instance of a PCIMk2ExternalPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a PCIMk2ExternalPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateExternalPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        PCIMk2ExternalPort *externalPort = NULL;

        try
        {
            externalPort = new PCIMk2ExternalPort(id, type, portNumber,
                                            portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the PCIMk2ExternalPort object*/
            externalPort = NULL;
        }

        /* return new PCIMk2ExternalPort object, or NULL if failed to allocate */
        return externalPort;
    }

    /**
    * Create an instance of a PCIMk2ConfigPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a PCIMk2ConfigPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateConfigPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        PCIMk2ConfigPort *configPort = NULL;

        try
        {
            configPort = new PCIMk2ConfigPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the PCIMk2ConfigPort object*/
            configPort = NULL;
        }

        /* return new PCIMk2ConfigPort object, or NULL if failed to allocate */
        return (Port*)configPort;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PCIMk2DeviceConfiguration() : Mk2DeviceConfiguration()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this configuration state is
    *                 associated with and describes.
    */
    explicit PCIMk2DeviceConfiguration(STAR_DEVICE_ID deviceID) :
                             Mk2DeviceConfiguration(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~PCIMk2DeviceConfiguration()
    {
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int EnableStateChangeEventsOnPort(U8 portNumber)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(portNumber);

        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int GetStateChangeEventsEnabledOnPort(U8 portNumber, _Out_ int *pEnabled)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(portNumber);
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(pEnabled);

        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int DisableStateChangeEventsOnPort(U8 portNumber)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(portNumber);

        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int EnableSpeedChangeEventsOnPort(U8 portNumber)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(portNumber);

        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int GetSpeedChangeEventsEnabledOnPort(U8 portNumber, _Out_ int *pEnabled)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(portNumber);
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(pEnabled);

        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int DisableSpeedChangeEventsOnPort(U8 portNumber)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(portNumber);

        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCI Mk2.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int GetMeasuredLinkSpeed(U8 portNum, _Out_ U16 *pLinkSpeed)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(portNum);
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(pLinkSpeed);

        /* throw exception - function not supported for PCI Mk2 */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * Get the clock frequency for a given link.
    *
    * @param linkNum The link to get the frequency of.
    * @param[out] pLinkFreq Pointer to a value that will be updated with the
    *                       link's clock frequency.
    *
    * @return 1 clock frequency successfully obtained, otherwise 0.
    *
    */
    int GetLinkClockFrequency(U8 linkNum,
                              _Out_ PCIMk2LinkPort::LinkFrequency *pLinkFreq)
    {
        /* call 'c' API to get the clock frequency for this link */
        STAR_CFG_PCIMK2_LINK_FREQ linkFreq;
        int result = CFG_PCIMK2_getLinkClockFrequency(idOfOwningDevice, linkNum,
                                                &linkFreq);

        /* store frequency in input variable provided */
        (*pLinkFreq) = (PCIMk2LinkPort::LinkFrequency)linkFreq;

        /* return result */
        return result;
    }

    /**
    * Set the clock frequency for a given link.
    *
    * @param linkNum The link to set the frequency of.
    * @param[out] linkFreq Link clock frequency to be set.
    *
    * @return 1 clock frequency successfully set, otherwise 0.
    *
    */
    int SetLinkClockFrequency(U8 linkNum,
                              PCIMk2LinkPort::LinkFrequency linkFreq)
    {
        /* call 'c' API to get the clock frequency for this link */
        return CFG_PCIMK2_setLinkClockFrequency(idOfOwningDevice, linkNum,
                                           (STAR_CFG_PCIMK2_LINK_FREQ)linkFreq);
    }
};

/* end namespace tags */
} /* pcimk2 */ } /* mk2devices */  } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

