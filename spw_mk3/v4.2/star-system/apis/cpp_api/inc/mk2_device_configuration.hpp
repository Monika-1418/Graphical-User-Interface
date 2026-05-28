/**
 * \file mk2_device_configuration.hpp
 *
 * \brief Holds the configuration state of a Mk2 compatible device.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds the configuration state of a Mk2 compatible device.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_MK2_CONFIG
 */

#ifndef __STAR_ROUTER_MK2_CONFIGURATION_STATE_CPP__
#define __STAR_ROUTER_MK2_CONFIGURATION_STATE_CPP__

#include "device_configuration.hpp"
#include "clock_rate_params.hpp"
#include "cfg_api_mk2.h"
#include "hardware_info.hpp"
#include "mk2_link_port.hpp"
#include "mk2_external_port.hpp"
#include "mk2_config_port.hpp"
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
/**
 * Classes required to perform device configuration tasks which are specific
 * to any of the Mk2 family of devices.
 **/
namespace mk2devices {

/**
* Holds the configuration state of a Mk2 compatible device.
*/
class Mk2DeviceConfiguration : public DeviceConfiguration
{
public:
    /** String constants, e.g. maximum expected length of strings provided as
      * parameters into functions. */
    enum StringConstants
    {
        /** maxmimum expected length of device version string */
        STR_VERSION_LEN = STAR_CFG_MK2_VERSION_STR_MAX_LEN,
        /** maxmimum expected length of device build date string */
        STR_BUILD_DATE_LEN = STAR_CFG_MK2_BUILD_DATE_STR_MAX_LEN
    };

protected:
    /**
    * Create an instance of a Mk2LinkPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a Mk2LinkPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateLinkPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        Mk2LinkPort *linkPort = NULL;

        try
        {
            linkPort = new Mk2LinkPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the Mk2LinkPort object*/
            linkPort = NULL;
        }

        /* return new Mk2LinkPort object, or NULL if failed to allocate */
        return linkPort;
    }

    /**
    * Create an instance of a Mk2ExternalPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a Mk2ExternalPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateExternalPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        Mk2ExternalPort *externalPort = NULL;

        try
        {
            externalPort = new Mk2ExternalPort(id, type, portNumber,
                                            portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the Mk2ExternalPort object*/
            externalPort = NULL;
        }

        /* return new Mk2ExternalPort object, or NULL if failed to allocate */
        return externalPort;
    }

    /**
    * Create an instance of a Mk2ConfigPort with the given values.
    *
    * @param id ID of device which owns the port
    * @param type The port's type
    * @param portNumber The port's number
    * @param portStatusControl The port's status control value
    *
    * @return An instance of a Mk2ConfigPort object, or NULL if failed to
    *         allocate memory for it.
    */
    virtual Port* CreateConfigPort(STAR_DEVICE_ID id, STAR_CFG_PORT_TYPE type,
                         U8 portNumber, PORT_STATUS_CONTROL portStatusControl)
    {
        Mk2ConfigPort *configPort = NULL;

        try
        {
            configPort = new Mk2ConfigPort(id, type, portNumber,
                                        portStatusControl);
        }
        catch(std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for the Mk2ConfigPort object*/
            configPort = NULL;
        }

        /* return new Mk2ConfigPort object, or NULL if failed to allocate */
        return (Port*)configPort;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    Mk2DeviceConfiguration() : DeviceConfiguration()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this configuration state is
    *                 associated with and describes.
    */
    explicit Mk2DeviceConfiguration(STAR_DEVICE_ID deviceID) :
                             DeviceConfiguration(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~Mk2DeviceConfiguration()
    {
    }

    /**
    * Sets the link rate divider for a given link.
    *
    * \note It is never necessary to set a link rate divider higher than 100 as
    *       the minimum transmit rate permitted by SpaceWire is 2 Mbit/s (200 Mbit/s /
    *       100).
    *
    *       If an odd number (other than 1) is specified as the divider
    *       parameter, the previous even integer will be used instead. For
    *       example: if a divider value of 3 is specified, the actual divider
    *       set shall be 2.
    *
    * @param divider Value of new divider. Valid input: 1, Even numbers 2
    *                through 126.
    * @param linkNum Link to set the divider for.
    *
    * @return 1 if divider was successfully set, otherwise 0.
    *
    */
    virtual int SetLinkRateDivider(U8 linkNum, U8 divider)
    {
        /* call 'c' API to set link rate divider for the given link */
        return CFG_MK2_setLinkRateDivider(idOfOwningDevice, linkNum,
                                          divider);

    }

    /**
    * Gets the link rate divider for a given link.
    *
    * @param[out] divider Value of the divider.
    * @param linkNum Link to get the divider for.
    *
    * @return 1 if link rate divider successfully obtained, otherwise 0.
    *
    */
    virtual int GetLinkRateDivider(U8 linkNum, _Out_ U8* divider)
    {
        /* call 'c' API to get the link rate divider for the given link */
        return CFG_MK2_getLinkRateDivider(idOfOwningDevice, linkNum,
                                          divider);
    }

    /**
    * Enable the device as a time-code master.
    *
    * @return 1 if successfully set as time-code master, otherwise 0.
    *
    */
    int EnableAsTimeCodeMaster()
    {
        /* call 'c' API */
        return CFG_MK2_enableTimeCodeMaster(idOfOwningDevice);
    }

    /**
    * Disable the device as a time-code master.
    *
    * @return 1 if successfully unset as time-code master, otherwise 0.
    *
    */
    int DisableAsTimeCodeMaster()
    {
        /* call 'c' API */
        return CFG_MK2_disableTimeCodeMaster(idOfOwningDevice);
    }

    /**
    * Gets whether the device has been enabled as a time-code master.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if the
    *                      device is enabled as a timecode master, else 0.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    *
    */
    int GetEnabledAsTimeCodeMaster(_Out_ int *pEnabled)
    {
        /* call 'c' API */
        return CFG_MK2_getTimeCodeMasterEnabled(idOfOwningDevice, pEnabled);
    }

    /**
    * Gets the period between time-code master ticks.
    *
    * This is the number of microseconds between time-codes.
    *
    * @param[out] period A pointer to a variable that will be updated to contain the
    *                    period in microseconds.
    *
    * @return 1 if the time-code period was successfully obtained, else 0.
    *
    */
    int GetTimeCodePeriod(_Out_ U32 *period)
    {
        /* call 'c' API */
        return CFG_MK2_getTimeCodePeriod(idOfOwningDevice, period);
    }

    /**
    * Sets the period between time-code master ticks.
    *
    * This is the number of microseconds between time-codes.
    *
    * @param period The period to be set in microseconds.
    *
    * @return 1 if the time-code period was successfully set, else 0.
    *
    */
    int SetTimeCodePeriod(U32 period)
    {
        /* call 'c' API */
        return CFG_MK2_setTimeCodePeriod(idOfOwningDevice, period);
    }

    /**
    * Obtains information about the hardware version of the device.
    *
    * @param hardwareInfo A reference to a HardwareInfo object which will be
    *                     updated to contain the hardware information obtained.
    *
    * @return 1 if hardware information successfully obtained, else 0.
    *
    */
    int GetHardwareInfo(HardwareInfo& hardwareInfo)
    {
        /* call 'c' API */
        return CFG_MK2_getHardwareInfo(idOfOwningDevice,
                                hardwareInfo.GetAsSTAR_CFG_MK2_HARDWARE_INFO());
    }

    /**
    * Get hardware version and build date for the device, as strings.
    *
    * @param[out] version A user supplied buffer of length
    *                     Mk2DeviceConfiguration::STR_VERSION_LEN which will be
    *                     updated to contain the hardware version as a
    *                     null-terminated string.
    *
    * @param[out] buildDate A user supplied buffer of length
    *                       Mk2DeviceConfiguration::STR_BUILD_DATE_LEN which
    *                       will be updated to contain the build date as a
    *                       null-terminated string.
    *
    * @return 1 if hardware information successfully obtained, else 0.
    *
    */
    int GetHardwareInfoAsString(_Out_z_cap_c_(STR_VERSION_LEN) char * version,
                            _Out_z_cap_c_(STR_BUILD_DATE_LEN) char * buildDate)
    {
        /* declare struct required by 'c' API to hold device hardware info */
        STAR_CFG_MK2_HARDWARE_INFO hardwareInfo;

        /* call 'c' API to get hardware info for the device*/
        int result = CFG_MK2_getHardwareInfo(idOfOwningDevice, &hardwareInfo);

        /* if failed to obtain hardware info for device */
        if (result == 0)
        {
            /* return error */
            return 0;
        }

        /* call 'c' API to get hardware info as a string */
        CFG_MK2_hardwareInfoToString(hardwareInfo, version, buildDate);

        /* return success */
        return 1;
    }

    /**
    * Flashes the front panel LEDs, in order to identify the device.
    *
    * @return 1 if the device was successfully identified, else 0.
    *
    */
    int Identify()
    {
        /* call 'c' API */
        return CFG_MK2_identify(idOfOwningDevice);
    }

    /**
    * Enables interface mode on the device.
    *
    * In interface mode, a packet which is received on an external port from a
    * SpaceWire link or from the configuration port will be routed to the port
    * specified by the port routing register of the port (which will have been
    * set previously by calling the SetPortRoutingAddress() method on the relevant
    * port).
    *
    * \note To selectively enable interface mode on a specific port, use
    *       EnableInterfaceMode() on the relevant Port object. Interface mode
    *       must be enabled globally on the device, for interface mode on a
    *       specific port to be enabled.
    *
    * @return 1 if interface mode was successfully enabled, else 0.
    *
    */
    int EnableInterfaceMode()
    {
        /* call 'c' API */
        return CFG_MK2_enableInterfaceMode(idOfOwningDevice);
    }

    /**
    * Disable interface mode on the device.
    *
    * When interface mode is disabled the device operates in routing mode.
    *
    * @return 1 if interface mode was successfully disabled, else 0.
    *
    */
    int DisableInterfaceMode()
    {
        /* call 'c' API */
        return CFG_MK2_disableInterfaceMode(idOfOwningDevice);
    }

    /**
    * Gets whether interface mode has been enabled on the device.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      interface mode is enabled, else 0.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    *
    */
    int GetInterfaceModeEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API */
        return CFG_MK2_getInterfaceModeEnabled(idOfOwningDevice, pEnabled);
    }

    /**
    * Enables the source identification setting on the device.
    *
    * When interface mode is enabled on a port, this function globally enables
    * the addition of a leading byte to each received packet indicating which
    * port the packet was received on.
    *
    * For each source port on which this behaviour is desired, a call must be
    * made to EnableSourceIdentification() on the relevant Port object.
    *
    * @return 1 if source identification was successfully enabled, else 0.
    *
    */
    int EnableSourceIdentification()
    {
        /* call 'c' API */
        return CFG_MK2_enableIdentifySource(idOfOwningDevice);
    }

    /**
    * Gets whether source identification has been enabled on the device.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      identify source is enabled, else 0.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    *
    */
    int GetSourceIdentificationEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API */
        return CFG_MK2_getIdentifySourceEnabled(idOfOwningDevice, pEnabled);
    }

    /**
    * Disable identification of source ports for this device.
    *
    * @return 1 if source identification was successfully disabled, else 0.
    *
    */
    int DisableSourceIdentification()
    {
        /* call 'c' API */
        return CFG_MK2_disableIdentifySource(idOfOwningDevice);
    }

    /**
    * Gets the address that a packet received on a specified Port should be
    * routed to when interface mode is enabled.
    *
    * \note This is an advanced feature and not necessary for normal usage.
    *       PCI/PCI Mk2 and PCIe devices prior to version 1.07 have a bug which
    *       means that the value returned in pAddress is always 0.
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
        return CFG_MK2_getPortRoutingAddress(idOfOwningDevice, portNumber,
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
        return CFG_MK2_setPortRoutingAddress(idOfOwningDevice, portNumber,
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
    virtual int EnableInterfaceModeOnPort(U8 portNumber)
    {
        /* call 'c' API to enable interface mode on the given port*/
        return CFG_MK2_enableInterfaceModeOnPort(idOfOwningDevice, portNumber);
    }

    /**
    * Gets whether interface mode has been enabled on a given port.
    *
    * \note PCI/PCI Mk2 and PCIe devices prior to version 1.07 have a bug which
    *       means that the value returned in pEnabled is always 0.
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
        return CFG_MK2_getInterfaceModeOnPortEnabled(idOfOwningDevice,
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
        return CFG_MK2_disableInterfaceModeOnPort(idOfOwningDevice, portNumber);
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
    virtual int EnableSourceIdentificationOnPort(U8 portNumber)
    {
        /* call 'c' API */
        return CFG_MK2_enableIdentifySourceOnPort(idOfOwningDevice, portNumber);
    }

    /**
    * Gets whether source identification has been enabled on a given port.
    *
    * \note PCI/PCI Mk2 and PCIe devices prior to version 1.07 have a bug which
    *       means that the value returned in pEnabled is always 0.
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
        return CFG_MK2_getIdentifySourceOnPortEnabled(idOfOwningDevice,
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
    virtual int DisableSourceIdentificationOnPort(U8 portNumber)
    {
        /* call 'c' API */
        return CFG_MK2_disableIdentifySourceOnPort(idOfOwningDevice, portNumber);
    }



    /**
    * Enables external time-code selection for the device.
    *
    * When external time-code selection is enabled and a time-code is
    * transmitted from an application, the value specified for the time-code is
    * used. Note that the time-code will only be transmitted by the device if
    * it is the next valid time-code.
    *
    * @return 1 if external time-code selection was successfully enabled for
    *         the device, else 0.
    *
    */
    int EnableExternalTimeCodeSelection()
    {
        /* call 'c' API */
        return CFG_MK2_enableExternalTimeCodeSelection(idOfOwningDevice);
    }

    /**
    *  Disables external time-code selection for the device.
    *
    * When external time-code selection is disabled and a time-code is
    * transmitted from an application, the value specified for the time-code is
    * ignored, and the next valid time-code is transmitted by the device.
    *
    * @return 1 if external time-code selection was successfully disabled for
    *         the device, else 0.
    *
    */
    int DisableExternalTimeCodeSelection()
    {
        /* call 'c' API */
        return CFG_MK2_disableExternalTimeCodeSelection(idOfOwningDevice);
    }

    /**
    * Gets whether external time-code selection has been enabled for the device.
    *
    * When external time-code selection is disabled and a time-code is
    * transmitted from an application, the value specified for the time-code is
    * ignored, and the next valid time-code is transmitted by the device. When
    * external time-code selection is enabled and a time-code is transmitted
    * from an application, the value specified for the time-code is used. Note
    * that the time-code will only be transmitted by the device if it is the
    * next valid time-code.
    *
    * @param[out] pEnabled User supplied value that will be updated to 1 if
    *                      external time-code selection is enabled, else 0.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    *
    */
    int GetExternalTimeCodeSelectionEnabled(_Out_ int *pEnabled)
    {
        /* call 'c' API */
        return CFG_MK2_getExternalTimeCodeSelectionEnabled(idOfOwningDevice,
                                                           pEnabled);
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
    virtual int InjectError(U8 portNumber, SPW_ERROR error)
    {
        /* call 'c' API to inject the error on the given port */
        return CFG_MK2_injectError(idOfOwningDevice, portNumber, error);
    }

    /**
    * Enables state change events on a given port.
    *
    * Enabling state change events will result in state change event traffic
    * being received on channel 0 when the link changes to running or
    * disconnected.
    *
    * @param portNumber The port to enable state change events on.
    *
    * @return 1 if state change events were successfully enabled, else 0.
    *
    */
    virtual int EnableStateChangeEventsOnPort(U8 portNumber)
    {
        /* call 'c' API version */
        return CFG_MK2_enableStateChangeEventsOnPort(idOfOwningDevice,
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
    virtual int GetStateChangeEventsEnabledOnPort(U8 portNumber,
        _Out_ int *pEnabled)
    {
        /* call 'c' API version */
        return CFG_MK2_getStateChangeEventsOnPortEnabled(idOfOwningDevice,
            portNumber, pEnabled);
    }

    /**
    * Disables state change events on a given port.
    *
    * Disabling state change events will result in state change event traffic
    * no longer being received on channel 0 when the link changes to running or
    * disconnected.
    *
    * @param portNumber The port to disable state change events on.
    *
    * @return 1 if state change events were successfully disabled, else 0.
    *
    */
    virtual int DisableStateChangeEventsOnPort(U8 portNumber)
    {
        /* call 'c' API version */
        return CFG_MK2_disableStateChangeEventsOnPort(idOfOwningDevice,
            portNumber);
    }

    /**
    * Enables speed change events on a given port.
    *
    * Enabling speed change events will result in speed change event traffic
    * being received on channel 0 when the link speed changes.
    *
    * @param portNumber The port to enable speed change events on.
    *
    * @return 1 if speed change events were successfully enabled, else 0.
    *
    */
    virtual int EnableSpeedChangeEventsOnPort(U8 portNumber)
    {
        /* call 'c' API version */
        return CFG_MK2_enableSpeedChangeEventsOnPort(idOfOwningDevice,
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
    virtual int GetSpeedChangeEventsEnabledOnPort(U8 portNumber,
        _Out_ int *pEnabled)
    {
        /* call 'c' API version */
        return CFG_MK2_getSpeedChangeEventsOnPortEnabled(idOfOwningDevice,
            portNumber, pEnabled);
    }

    /**
    * Disables speed change events on a given port.
    *
    * Disabling speed change events will result in speed change event traffic
    * no longer being received on channel 0 when the link speed changes.
    *
    * @param portNumber The port to disable speed change events on.
    *
    * @return 1 if speed change events were successfully disabled, else 0.
    *
    */
    virtual int DisableSpeedChangeEventsOnPort(U8 portNumber)
    {
        /* call 'c' API version */
        return CFG_MK2_disableSpeedChangeEventsOnPort(idOfOwningDevice,
            portNumber);
    }

    /**
    * Gets the current link speed measured on a specific port.
    *
    * @param portNum Port to get information for.
    * @param pLinkSpeed[out] User supplied value that will be updated to
    *                        contain the measured link speed in 100 Kbit/s
    *                        units, see #STAR_CFG_MK2_LINK_SPEED_UNITS_KBPS.
    *
    * @return 1 if the information could be obtained from the device, else 0.
    */
    virtual int GetMeasuredLinkSpeed(U8 portNum, _Out_ U16 *pLinkSpeed)
    {
        /* call 'c' API version to get measured link speed */
        return CFG_MK2_getMeasuredLinkSpeed(idOfOwningDevice, portNum,
            pLinkSpeed);
    }
};

/* end namespace tags */
} /* mk2devices */ } /* deviceconfig */
} /* starsystem */ } /* com */  } /* stardundee */

#endif

