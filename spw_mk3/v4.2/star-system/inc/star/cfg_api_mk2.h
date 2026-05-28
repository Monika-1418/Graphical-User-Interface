/**
 * \file cfg_api_mk2.h
 *
 * \brief Functions used to configure STAR-Dundee Mk2 compatible devices.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */

#ifndef CFG_API_MK2_H
#define CFG_API_MK2_H

#include "star-api.h"
#include "cfg_api_mk2_types.h"
#include "cfg_api_remote.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Maximum length for the string representation of a hardware build date */
#define STAR_CFG_MK2_BUILD_DATE_STR_MAX_LEN 256

/** Maximum length for the string representation of a hardware build date */
#define STAR_CFG_MK2_VERSION_STR_MAX_LEN 256

/** The name of the module, used for logging purposes. */
#define CFG_API_MK2_MODULE_NAME     "CFG-API_MK2"

/**
 * \ingroup hardwareMk2
 *
 * Reads information about the hardware version of a device and updates the
 * STAR_CFG_MK2_HARDWARE_INFO structure pointed to by the passed in pointer to
 * contain the received version information
 *
 *
 * @param       deviceID    Identifier for the Mk2 compatible device to get
 *                          hardware info from.
 * @param[out]  pInfo       Pointer to a STAR_CFG_MK2_HARDWARE_INFO structure
 *                          that will be updated to contain hardware information
 *                          for the device.
 *
 * @return  1 if the hardware version was successfully read.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getHardwareInfo(
                                    STAR_DEVICE_ID deviceID,
                                    _Out_ STAR_CFG_MK2_HARDWARE_INFO *pInfo);

/**
 * \ingroup hardwareMk2
 *
 * Creates a string representation of a STAR_CFG_MK2_HARDWARE_INFO structure.
 *
 *
 * @param   info            Hardware information obtained from a a call to
 *                          CFG_MK2_getHardwareInfo().
 * @param[out] pVersion     String of length #STAR_CFG_MK2_VERSION_STR_MAX_LEN
 *                          that will be updated to contain a null-terminated
 *                          string representation of the hardware version.
 * @param[out] pBuildDate   String of length #STAR_CFG_MK2_VERSION_STR_MAX_LEN
 *                          that will be updated to contain a null-terminated
 *                          string representation of the hardware build date.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
void STAR_API_CC CFG_MK2_hardwareInfoToString(
                                STAR_CFG_MK2_HARDWARE_INFO info,
                                _Out_z_cap_c_(STAR_CFG_MK2_VERSION_STR_MAX_LEN) char* pVersion,
                                _Out_z_cap_c_(STAR_CFG_MK2_BUILD_DATE_STR_MAX_LEN) char* pBuildDate);

/**
 * \ingroup timecodeMk2
 *
 * Enables the device as a time-code master.
 *
 * @param   deviceID    The Mk2 compatible device to enable as a time-code master.
 *
 * @return  1 if the device was successfully set as a time code master, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_enableTimeCodeMaster(STAR_DEVICE_ID deviceID);

/**
 * \ingroup timecodeMk2
 *
 * Gets whether the device has been enabled as a time-code master.
 *
 * @param   deviceID    The Mk2 compatible device to check.
 * @param   pEnabled    User supplied value that will be updated to 1
 *                      if the device is enabled as a time-code master, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v1_5
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getTimeCodeMasterEnabled(STAR_DEVICE_ID deviceID, int *pEnabled);

/**
 * \ingroup timecodeMk2
 *
 * Disables the device as a time-code master.
 *
 * @param   deviceID    The Mk2 compatible device to disable as  a time-code master
 *
 * @return  1 if the device was successfully unset as a time code master, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_disableTimeCodeMaster(STAR_DEVICE_ID deviceID);

/**
 * \ingroup timecodeMk2
 *
 * Enables external time-code selection for the device.  When external time-code
 * selection is enabled and a time-code is transmitted from an application, the
 * value specified for the time-code is used.  Note that the time-code will only
 * be transmitted by the device if it is the next valid time-code.
 *
 * @param deviceID The Mk2 compatible device to enable external time-code
 *                 selection for.
 *
 * @return  1 if external time-code selection was successfully enabled for the
 *          device, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_3
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_enableExternalTimeCodeSelection(
    STAR_DEVICE_ID deviceID);

/**
 * \ingroup timecodeMk2
 *
 * Gets whether external time-code selection has been enabled for the device.
 * When external time-code selection is disabled and a time-code is transmitted
 * from an application, the value specified for the time-code is ignored, and
 * the next valid time-code is transmitted by the device.  When external time-code
 * selection is enabled and a time-code is transmitted from an application, the
 * value specified for the time-code is used.  Note that the time-code will only
 * be transmitted by the device if it is the next valid time-code.
 *
 * @param   deviceID    The Mk2 compatible device to check.
 * @param   pEnabled    User supplied value that will be updated to 1
 *                      if external time-code selection is enabled for the
 *                      device, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_3
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getExternalTimeCodeSelectionEnabled(
    STAR_DEVICE_ID deviceID, int *pEnabled);

/**
 * \ingroup timecodeMk2
 *
 * Disables external time-code selection for the device.  When external
 * time-code selection is disabled and a time-code is transmitted from an
 * application, the value specified for the time-code is ignored, and the next
 * valid time-code is transmitted by the device.
 *
 * @param deviceID The Mk2 compatible device to disable external time-code
 *                 selection for.
 *
 * @return  1 if external time-code selection was successfully disabled for the
 *          device, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_3
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_disableExternalTimeCodeSelection(STAR_DEVICE_ID deviceID);

/**
 * \ingroup timecodeMk2
 *
 * Sets the period between time-code master ticks. This is the number of
 * microseconds between time-codes.
 *
 * @param   deviceID    Device to set time-code period for
 * @param   period      The period to be set in microseconds
 *
 * @return  1 if the time-code period was successfully set, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_setTimeCodePeriod(STAR_DEVICE_ID deviceID,
                                                          U32 period);
/**
 * \ingroup timecodeMk2
 *
 * Gets the period between time-code master ticks. This is the number of
 * microseconds between time-codes.
 *
 * @param   deviceID    Device to get time-code period from
 * @param[out]  pPeriod A pointer to a variable that will be updated to contain
 *                      the period in microseconds
 *
 * @return  1 if the time-code period was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getTimeCodePeriod(STAR_DEVICE_ID deviceID,
                                                          _Out_ U32* pPeriod);

/**
 * \ingroup timecodeMk2
 *
 * Enables time-code counter bypass mode for the device.  When time-code counter
 * bypass mode is enabled, any time-code which is received will be forwarded out
 * of the other ports on the router, without checking against the current value
 * of the counter. The time-code register will be updated on each received
 * time-code.
 *
 * \note This function is currently only supported by \ref PCIe devices with
 * at least hardware version v1.12.
 *
 * @param deviceID The Mk2 compatible device to enable time-code counter
 *                 bypass mode for.
 *
 * @return  1 if time-code counter bypass mode was successfully enabled for the
 *          device, else 0.
 *
 * \versionAdded \ref changes_v3_1
 *
 * \devicesSupported \ref PCIe (version 1.12 and greater)
 */
int STAR_API_CC CFG_MK2_enableTimeCodeCounterBypassMode(
    STAR_DEVICE_ID deviceID);

/**
 * \ingroup timecodeMk2
 *
 * Gets whether time-code counter bypass mode has been enabled for the device.
 * When time-code counter bypass mode is disabled, any time-code which is received
 * will be checked against against the current value of the counter for validity
 * before being forwarded. When time-code counter bypass mode is enabled, any
 * time-code which is received will be forwarded out of the other ports on the
 * router, without checking against the current value of the counter. The time-code
 * register will be updated on each received time-code.
 *
 * \note This function is currently only supported by \ref PCIe devices with
 * at least hardware version v1.12.
 *
 * @param   deviceID    The Mk2 compatible device to check.
 * @param   pEnabled    User supplied value that will be updated to 1
 *                      if time-code counter bypass mode is enabled for the
 *                      device, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v3_1
 *
 * \devicesSupported \ref PCIe (version 1.12 and greater)
 */
int STAR_API_CC CFG_MK2_getTimeCodeCounterBypassModeEnabled(
    STAR_DEVICE_ID deviceID, int *pEnabled);

/**
 * \ingroup timecodeMk2
 *
 * Disables time-code counter bypass mode for the device.  When time-code counter
 * bypass mode is disabled, any time-code which is received will be checked against
 * the current value of the counter for validity before being forwarded.
 *
 * \note This function is currently only supported by \ref PCIe devices with
 * at least hardware version v1.12.
 *
 * @param deviceID The Mk2 compatible device to disable time-code counter
 *                 bypass mode for.
 *
 * @return  1 if time-code counter bypass mode was successfully disabled for the
 *          device, else 0.
 *
 * \versionAdded \ref changes_v3_1
 *
 * \devicesSupported \ref PCIe (version 1.12 and greater)
 */
int STAR_API_CC CFG_MK2_disableTimeCodeCounterBypassMode(STAR_DEVICE_ID deviceID);

/**
 * \ingroup hardwareMk2
 *
 * Flashes the front panel LEDs of a device.  This can be used to identify the
 * physical device to which a device identifier refers to.
 *
 * @param   deviceID    Device to have its LEDs flashed.
 *
 * @return  1 if the device successfully identified itself, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_identify(STAR_DEVICE_ID deviceID);

/**
 * \ingroup interfaceModeMk2
 *
 * In interface mode a packet which is received on an external port, from a SpaceWire
 * link or from the configuration port will be routed to the port specified by
 * the port routing register of the port (set by CFG_MK2_setPortRoutingAddress()).
 *
 *
 * @param deviceID          Device to enable interface mode on.
 *
 * @return  1 if interface mode was successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
 int STAR_API_CC CFG_MK2_enableInterfaceMode(STAR_DEVICE_ID deviceID);

 /**
 * \ingroup interfaceModeMk2
 *
 * Gets whether interface mode has been enabled on a device.
 *
 * @param   deviceID    The Mk2 compatible device to check.
 * @param   pEnabled    User supplied value that will be updated to 1
 *                      if interface mode is enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0
 *
 * \versionAdded \ref changes_v1_5
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getInterfaceModeEnabled(STAR_DEVICE_ID deviceID, int *pEnabled);

/**
 * \ingroup interfaceModeMk2
 *
 * Selectively enables interface mode on a given port. Interface mode must be enabled
 * globally (CFG_MK2_enableInterfaceMode()) for interface mode on a port to be enabled.
 *
 *
 * @param deviceID          Device to enable interface mode for a port on.
 * @param portNum           Port to enable interface mode on.
 *
 * @return  1 if interface mode was successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref PCIMk2,
 *          \ref PCIe,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_enableInterfaceModeOnPort(STAR_DEVICE_ID deviceID,
                                                                   U8 portNum);

/**
 * \ingroup interfaceModeMk2
 *
 * Gets whether interface mode is enabled on a specific port.
 *
 * \note \ref PCIMk2 and \ref PCIe devices prior to version 1.07 have a bug
 *       which means that the value returned in pEnabled is always 0.
 *
 * @param deviceID          Device to get interface mode enabled for a port on.
 * @param portNum           Port to get information for.
 * @param   pEnabled        User supplied value that will be updated to 1
 *                          if interface mode is enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v1_5
 *
 * \devicesSupported \ref PCIMk2,
 *          \ref PCIe,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getInterfaceModeOnPortEnabled(STAR_DEVICE_ID deviceID,
                                         U8 portNum,
                                         int *pEnabled);
/**
 * \ingroup interfaceModeMk2
 *
 * Disables interface mode on a device.
 * When interface mode is disabled, the device operates in routing mode.
 *
 * @param deviceID          Device to disable interface mode on.
 *
 * @return  1 if interface mode was successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_disableInterfaceMode(STAR_DEVICE_ID deviceID);

/**
 * \ingroup interfaceModeMk2
 *
 * Selectively disables interface mode on a given port.
 * When interface mode is disabled, the device operates in routing mode.
 *
 * @param deviceID          Device to disable interface mode for a port on.
 * @param portNum           Port to disable interface mode on.
 *
 * @return  1 if interface mode was successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref PCIMk2,
 *          \ref PCIe,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_disableInterfaceModeOnPort(STAR_DEVICE_ID deviceID,
                                                                   U8 portNum);

/**
 * \ingroup interfaceModeMk2
 *
 * When interface mode is enabled on a port, this function globally enables the addition
 * of a leading byte to each received packet indicating which port the packet was
 * received on. For each source port on which this behaviour is desired, a call to
 * CFG_MK2_enableIdentifySourceOnPort() must be made.
 *
 *
 * @param deviceID          Device to enable source port identification on.
 *
 * @return  1 if source identification was successfully enabled globally, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_enableIdentifySource(STAR_DEVICE_ID deviceID);

/**
 * \ingroup interfaceModeMk2
 *
 * Gets whether identify source is enabled.
 *
 * @param deviceID          Device to check.
 * @param   pEnabled        User supplied value that will be updated to 1
 *                          if identify source is enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v1_5
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getIdentifySourceEnabled(STAR_DEVICE_ID deviceID,
                                                 int *pEnabled);

/**
 * \ingroup interfaceModeMk2
 *
 * Disables identification of source ports for interface mode globally.
 *
 * @param deviceID    Device to disable source port identification on.
 *
 * @return  1 if source identification was successfully disabled globally, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_disableIdentifySource(STAR_DEVICE_ID deviceID);

/**
 * \ingroup interfaceModeMk2
 *
 * Enables identification of source port during interface mode for a given port.
 * Interface mode (CFG_MK2_enableInterfaceMode()) and Identify Source
 * (CFG_MK2_enableIdentifySource()) must be enabled globally for this to
 * have an effect.
 *
 * @param deviceID     Device to enable source port identification for a given port on.
 * @param portNum      Port to enable source identification on.
 *
 * @return  1 if source identification was successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref PCIMk2,
 *          \ref PCIe,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_enableIdentifySourceOnPort(STAR_DEVICE_ID deviceID,
                                                                    U8 portNum);
/**
 * \ingroup interfaceModeMk2
 *
 * Gets whether identify source is enabled for a specific port.
 *
 * \note \ref PCIMk2 and \ref PCIe devices prior to version 1.07 have a bug
 *       which means that the value returned in pEnabled is always 0.
 *
 * @param deviceID    Device to check.
 * @param portNum     Port to check.
 * @param pEnabled    User supplied value that will be updated to 1
 *                    if identify source is enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v1_5
 *
 * \devicesSupported \ref PCIMk2,
 *          \ref PCIe,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getIdentifySourceOnPortEnabled(STAR_DEVICE_ID deviceID,
                                                 U8 portNum,
                                                 int *pEnabled);
/**
 * \ingroup interfaceModeMk2
 *
 * Disables source port identification for a given port.
 *
 * @param deviceID     Device to disable source port identification for a given port on.
 * @param portNum      Port to disable source identification on.
 *
 * @return  1 if source identification was successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref PCIMk2,
 *          \ref PCIe,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_disableIdentifySourceOnPort(STAR_DEVICE_ID deviceID,
                                                                     U8 portNum);

/**
 * \ingroup interfaceModeMk2
 *
 * Sets the address a packet received on a given port should be routed to when
 * interface mode is enabled.
 *
 * \note This is an advanced feature and not necessary for normal usage.
 *
 * @param deviceID     Device to have one of its ports port routing address changed.
 * @param portNum      Port to have its port routing address modified.
 * @param address      New port routing address.
 *
 * @return  1 if the address could be set, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref PCIMk2,
 *          \ref PCIe,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_setPortRoutingAddress(STAR_DEVICE_ID deviceID,
                                                               U8 portNum,
                                                               U8 address);

/**
 * \ingroup interfaceModeMk2
 *
 * Gets the address a packet received on a given port should be routed to when
 * interface mode is enabled.
 *
 * \note This is an advanced feature and not necessary for normal usage.
 * \note \ref PCIMk2 and \ref PCIe devices prior to version 1.07 have a bug
 *       which means that the value returned in pAddress is always 0.
 *
 * @param       deviceID     Device to get port routing address from.
 * @param       portNum      Port to get port routing address from.
 * @param[out]  pAddress     Pointer to value that will be updated to contain the address.
 *
 * @return  1 if the address could be obtained, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref PCIMk2,
 *          \ref PCIe,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getPortRoutingAddress(STAR_DEVICE_ID deviceID,
                                                               U8 portNum,
                                                              _Out_ U8* pAddress);

/**
 * \ingroup linksMk2
 *
 * Sets the Link rate divider for a given link. The transmit rate of a link is
 * given by:
 *
 *  \f[
 *  Link Transmit Rate = \frac{Base Clock Rate}{Link Clock Rate Divider}  \times 2
 *  \f]

 * Where \f$Base Clock Rate\f$ is set by the appropriate function (e.g.
 * CFG_MK2_setBaseTransmitClock() for a \ref PCIe device) and
 * \f$Link Clock Rate Divider =\f$ divider.
 *
 * \note It is never necessary to set a link rate divider higher than 100, as the minimum
 *       transmit rate permitted by SpaceWire is 2 Mbit/s (i.e. 200 Mbit/s / 100).
 *
 * \note If an odd number (other than 1) is specified as the divider parameter, the previous
 *       even integer will be used instead. For example: if a divider value of 3 is specified,
 *       the actual divider set shall be 2.
 *
 * \note Link rate divider is limited to 64 on PCIe hardware versions
 *       prior to v1.11. We recommend upgrading to the latest firmware.
 *       If using the latest firmware then it is necessary to use
 *       CFG_MK2_setTransmitClock() rather than CFG_MK2_setBaseTransmitClock().
 *       The CFG_MK2_setLinkRateDivider() function does not apply to PCIe
 *       hardware v1.11 or later and SPLT hardware v2.0 or later.
 *
 * @param deviceID         Device to modify a links divider on.
 * @param linkNum          Link to have its divider modified.
 * @param divider          Value of the new divider. Valid input: 1, Even numbers 2 through 126.
 *
 * @return  1 if divider was successfully set, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_setLinkRateDivider(STAR_DEVICE_ID deviceID,
                                                            U8 linkNum,
                                                            U8 divider);

/**
 * \ingroup linksMk2
 *
 * Gets the Link rate divider for a given link.
 *
 * @param      deviceID         Device to get a links divider from..
 * @param      linkNum          Link to get divider for.
 * @param[out] pDivider         Value of the divider.
 *
 * @return  1 if was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIMk2,
 *          \ref PCIe,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getLinkRateDivider(STAR_DEVICE_ID deviceID,
                                                            U8 linkNum,
                                                            _Out_ U8* pDivider);

/**
 * \ingroup linksMk2
 *
 * Sets the base transmit clock rate on a given link:
 *
 *  \f[
 *   Base Clock Rate = \frac{100 \mathrm{MHz} \times MULTIPIER}{DIVISOR}
 *  \f]
 *
 * The \f$MULTIPIER\f$ and \f$DIVISOR\f$ are properties of the
 * #STAR_CFG_MK2_BASE_TRANSMIT_CLOCK structure, passed as a parameter.  The
 * output clock rate must be between 5 MHz and 200 MHz.
 *
 * The transmit rate of the link is given by:
 *
 *  \f[
 *   Link Transmit Rate = \frac{Base Clock Rate}{Link Clock Rate Divider}  \times 2
 *  \f]
 *
 * The \f$Link Clock Rate Divider\f$ can be set using the
 * CFG_MK2_setLinkRateDivider() function.
 *
 * \note This function is currently for use with \ref PCIe devices before
 * hardware version v1.11 and \ref SPLT devices before hardware version v2.0. \n
 * For later versions use CFG_MK2_setTransmitClock(). \n \n
 * The \ref PCIMk2, \ref BrickMk2, \ref RouterMk2S and
 * \ref BrickMk3 do not support this function. For \ref PCIMk2 devices,
 * please use CFG_PCIMK2_setLinkClockFrequency() instead.  For
 * \ref BrickMk2 and \ref RouterMk2S devices, please use
 * CFG_BRICK_MK2_setLinkClockFrequency().  For \ref BrickMk3 devices,
 * please use CFG_BRICK_MK3_setBaseTransmitClock() or
 * CFG_BRICK_MK3_setTransmitClock() depending on hardware version. \n \n
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param deviceID         Device to modify a link's base transmit clock rate on.
 * @param linkNum          Link to have its base transmit clock rate modified.
 * @param clockRateParams  Base transmit clock rate parameters to be set.
 *
 * @return  1 if link's Base transmit clock frequency was successfully set, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref PCIe (versions prior to 1.11),
 *          \ref SPLT (versions prior to 2.0)
 */
int STAR_API_CC CFG_MK2_setBaseTransmitClock(STAR_DEVICE_ID deviceID,
                                             U8 linkNum,
                                             STAR_CFG_MK2_BASE_TRANSMIT_CLOCK clockRateParams);

/**
 * \ingroup linksMk2
 *
 * Gets the  base transmit clock rate parameters for a given link.
 *
 * \note This function is currently for use with \ref PCIe devices before
 * hardwareversion v1.11 and \ref SPLT devices before hardware version v2.0. \n
 * For later versions use CFG_MK2_getTransmitClock(). \n \n
 * The \ref PCIMk2, \ref BrickMk2, \ref RouterMk2S and
 * \ref BrickMk3 do not support this function. For \ref PCIMk2 devices,
 * please use CFG_PCIMK2_getLinkClockFrequency() instead.  For
 * \ref BrickMk2 and \ref RouterMk2S devices, please use
 * CFG_BRICK_MK2_getLinkClockFrequency().  For \ref BrickMk3 devices,
 * please use CFG_BRICK_MK3_getBaseTransmitClock() or
 * CFG_BRICK_MK3_getTransmitClock() depending on hardware version. \n \n
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param      deviceID         Device to get the base transmit clock rate from.
 * @param      linkNum          Link to get base transmit clock rate for.
 * @param[out] pClockRateParams Pointer to value that will be updated with the given link's
 *                              Base transmit clock rate parameters.
 *
 * @return  1 if Base transmit clock frequency parameters were successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v1_2
 *
 * \devicesSupported \ref PCIe (versions prior to 1.11),
 *          \ref SPLT (versions prior to 2.0)
 */
int STAR_API_CC CFG_MK2_getBaseTransmitClock(STAR_DEVICE_ID deviceID,
                                             U8 linkNum,
                                             _Out_ STAR_CFG_MK2_BASE_TRANSMIT_CLOCK* pClockRateParams);

/**
 * \ingroup linksMk2
 *
 * Sets the transmit clock rate on a given link. The transmit rate of a link is
 * given by:
 *
 *  \f[
 *   Link Transmit Rate = \frac{100 \mathrm{MHz} \times MULTIPIER}{DIVISOR}  \times 2
 *  \f]
 *
 * The \f$MULTIPIER\f$ and \f$DIVISOR\f$ are properties of the
 * #STAR_CFG_MK2_BASE_TRANSMIT_CLOCK structure, passed as a parameter.  The
 * output clock rate will be between 1 MHz and 200 MHz giving data rates between
 * 2 Mbit/s and 400 Mbit/s.
 *
 * Note that for data rates below 10 Mbit/s, an exact rate may not be achievable
 * and it may be rounded up or down slightly.
 *
 * \note This function is currently for use with \ref PCIe devices from hardware
 * version v1.11 and \ref SPLT devices from hardware version v2.0.
 * For earlier versions use CFG_MK2_setBaseTransmitClock(). \n \n
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param deviceID         Device to modify a link's transmit clock rate on.
 * @param linkNum          Link to have its transmit clock rate modified.
 * @param clockRateParams  Transmit clock rate parameters to be set.
 *
 * @return  1 if link's transmit clock frequency was successfully set, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta7
 *
 * \devicesSupported \ref PCIe (versions 1.11 and greater),
 *          \ref SPLT (versions 2.0 and greater)
 */
int STAR_API_CC CFG_MK2_setTransmitClock(STAR_DEVICE_ID deviceID, U8 linkNum,
                                             STAR_CFG_MK2_BASE_TRANSMIT_CLOCK clockRateParams);

/**
 * \ingroup linksMk2
 *
 * Gets the transmit clock rate parameters for a given link.
 *
 * \note This function is currently for use with \ref PCIe devices from hardware
 * version v1.11 and \ref SPLT devices from hardware version v2.0.
 * For earlier versions use CFG_MK2_getBaseTransmitClock(). \n \n
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param      deviceID         Device to get the transmit clock rate from.
 * @param      linkNum          Link to get the transmit clock rate for.
 * @param[out] pClockRateParams  Pointer to value that will be updated with the given link's
 *                              transmit clock rate parameters.
 *
 * @return  1 if transmit clock frequency parameters were successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta7
 *
 * \devicesSupported \ref PCIe (versions 1.11 and greater),
 *          \ref SPLT (versions 2.0 and greater)
 */
int STAR_API_CC CFG_MK2_getTransmitClock(STAR_DEVICE_ID deviceID, U8 linkNum,
                                            _Out_ STAR_CFG_MK2_BASE_TRANSMIT_CLOCK* pClockRateParams);

/**
 * \ingroup errInjectMk2
 *
 * Immediately injects the specified error on a given device's port.
 *
 * \note This function is currently only compatible with \ref PCIMk2, \ref SPLT
 *       and \ref PCIe devices. No check is made by this function to ensure it
 *       is being used with a compatible device.  For \ref BrickMk2,
 *       \ref RouterMk2S and \ref BrickMk3 devices, please use
 *       CFG_BRICK_MK2_injectErrors().
 *
 * \note Error injection makes use of the same on-board registers as periodic
 *       actions on devices which support it, therefore error injection cannot
 *       be used on those devices when periodic actions are in progress.
 *
 * @param deviceID      Identifier of the device to have the error injected on.
 * @param port          Port the error is to be injected on.
 * @param error         SpaceWire error to be injected.
 *
 * @return      1 if the error information was successfully sent to the device,
 *              else 0.
 *
 * \versionAdded \ref changes_v2_0
 *
 * \devicesSupported \ref PCIMk2,
 *          \ref PCIe,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_injectError(STAR_DEVICE_ID deviceID,
                                        unsigned char port,
                                        SPW_ERROR error);

/**
 * \ingroup periodicMk2
 *
 * Starts a periodic action on a given device's port.  To stop a periodic action
 * use CFG_MK2_stopPeriodicAction().
 *
 * \note This function is currently only compatible with \ref PCIMk2 devices. No
 *       check is made by this function to ensure it is being used with a
 *       compatible device.
 *
 * \note Periodic actions make use of the same on-board registers as error
 *       injection, therefore error injection cannot be used when periodic actions
 *       are in progress.
 *
 * @param deviceID      Identifier of the device to have periodic action created for.
 * @param port          Port the periodic action is to be performed on.
 * @param count         Number of device clock cycles between action repititions.
 *                      See \ref CFG_MK2_getDeviceClockRate() for obtaining the
 *                      device's clock rate.
 * @param action        Action to be performed.  This can be \ref SPW_TRANSMIT_PACKET
 *                      or any of the \ref SPW_ERROR values.
 *
 * @return      1 if the periodic action was successfully started,
 *              else 0.
 *
 * \versionAdded \ref changes_v3_0_beta3
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC CFG_MK2_startPeriodicAction(STAR_DEVICE_ID deviceID,
                                            unsigned char port,
                                            U32 count,
                                            SPW_ACTION action);

/**
 * \ingroup periodicMk2
 *
 * Stops a periodic action on a given device's port.  Periodic actions are started
 * by calling CFG_MK2_startPeriodicAction().
 *
 * \note This function is currently only compatible with \ref PCIMk2 devices. No
 *       check is made by this function to ensure it is being used with a
 *       compatible device.
 *
 * @param deviceID      Identifier of the device performing the periodic action.
 * @param port          Port the periodic action is being performed on.
 *
 * @return      1 if the periodic action was successfully stopped,
 *              else 0.
 *
 * \versionAdded \ref changes_v3_0_beta3
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC CFG_MK2_stopPeriodicAction(STAR_DEVICE_ID deviceID,
                                           unsigned char port);

/**
 * \ingroup periodicMk2
 *
 * Gets a device's internal clock rate.
 *
 * @param deviceID      Identifier of the device to query.
 *
 * @return      The device's clock rate in Hz, or zero if the device does not
 *              support periodic actions.
 *
 * \versionAdded \ref changes_v3_0_beta3
 *
 * \devicesSupported \ref PCIMk2
 */
U32 STAR_API_CC CFG_MK2_getDeviceClockRate(STAR_DEVICE_ID deviceID);

/**
 * \ingroup mk2Events
 *
 * Selectively enables state change events on a given port.  Enabling state
 * change events will result in state change event traffic being received on
 * channel 0 when the link changes to running or disconnected.
 *
 * \note This function is currently only compatible with \ref PCIe devices from
 *       hardware version v1.10 edit 4, and \ref SPLT devices from hardware
 *       version v2.0. No check is made by this function to ensure it is being
 *       used with a compatible device.
 *
 * \note On the \ref PCIe state events are received on the channel associated
 *       with the port rather than channel 0.
 *
 * @param deviceID          Device to enable state change events for a port on.
 * @param portNum           Port to enable state change events on.
 *
 * @return  1 if state change events were successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v3_0
 *
 * \devicesSupported \ref PCIe (version 1.10e4 and greater),
 *          \ref SPLT (version 2.0 and greater)
 */
int STAR_API_CC CFG_MK2_enableStateChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup mk2Events
 *
 * Gets whether state change events are enabled on a specific port.
 *
 * \note This function is currently only compatible with \ref PCIe devices from
 *       hardware version v1.10 edit 4, and \ref SPLT devices from hardware
 *       version v2.0. No check is made by this function to ensure it is being
 *       used with a compatible device.
 *
 * @param deviceID          Device to get whether state change events are
 *                          enabled for a port.
 * @param portNum           Port to get information for.
 * @param pEnabled          User supplied value that will be updated to 1
 *                          if state change events are enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v3_0
 *
 * \devicesSupported \ref PCIe (version 1.10e4 and greater),
 *          \ref SPLT (version 2.0 and greater)
 */
int STAR_API_CC CFG_MK2_getStateChangeEventsOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup mk2Events
 *
 * Selectively disables state change events on a given port.  Disabling state
 * change events will result in state change event traffic no longer being
 * received on channel 0 when the link changes to running or disconnected.
 *
 * \note This function is currently only compatible with \ref PCIe devices from
 *       hardware version v1.10 edit 4, and \ref SPLT devices from hardware
 *       version v2.0. No check is made by this function to ensure it is being
 *       used with a compatible device.
 *
 * \note On the \ref PCIe state events are received on the channel associated
 *       with the port rather than channel 0.
 *
 * @param deviceID          Device to disable state change events for a port on.
 * @param portNum           Port to disable state change events on.
 *
 * @return  1 if state change events were successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v3_0
 *
 * \devicesSupported \ref PCIe (version 1.10e4 and greater),
 *          \ref SPLT (version 2.0 and greater)
 */
int STAR_API_CC CFG_MK2_disableStateChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup mk2Events
 *
 * Selectively enables speed change events on a given port.  Enabling speed
 * change events will result in speed change event traffic being received on
 * channel 0 when the link speed changes.
 *
 * \note This function is currently only compatible with \ref PCIe devices from
 *       hardware version v1.10 edit 4, and \ref SPLT devices from hardware
 *       version v2.0. No check is made by this function to ensure it is being
 *       used with a compatible device.
 *
 * \note On the \ref PCIe link speed events are received on the channel
 *       associated with the port rather than channel 0.
 *
 * @param deviceID          Device to enable speed change events for a port on.
 * @param portNum           Port to enable speed change events on.
 *
 * @return  1 if speed change events were successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v3_0
 *
 * \devicesSupported \ref PCIe (version 1.10e4 and greater),
 *          \ref SPLT (version 2.0 and greater)
 */
int STAR_API_CC CFG_MK2_enableSpeedChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup mk2Events
 *
 * Gets whether speed change events are enabled on a specific port.
 *
 * \note This function is currently only compatible with \ref PCIe devices from
 *       hardware version v1.10 edit 4, and \ref SPLT devices from hardware
 *       version v2.0. No check is made by this function to ensure it is being
 *       used with a compatible device.
 *
 * @param deviceID          Device to get whether speed change events are
 *                          enabled for a port.
 * @param portNum           Port to get information for.
 * @param pEnabled          User supplied value that will be updated to 1
 *                          if speed change events are enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v3_0
 *
 * \devicesSupported \ref PCIe (version 1.10e4 and greater),
 *          \ref SPLT (version 2.0 and greater)
 */
int STAR_API_CC CFG_MK2_getSpeedChangeEventsOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup mk2Events
 *
 * Selectively disables speed change events on a given port.  Disabling speed
 * change events will result in speed change event traffic no longer being
 * received on channel 0 when the link speed changes.
 *
 * \note This function is currently only compatible with \ref PCIe devices from
 *       hardware version v1.10 edit 4, and \ref SPLT devices from hardware
 *       version v2.0. No check is made by this function to ensure it is being
 *       used with a compatible device.
 *
 * \note On the \ref PCIe link speed events are received on the channel
 *       associated with the port rather than channel 0.
 *
 * @param deviceID          Device to disable speed change events for a port on.
 * @param portNum           Port to disable speed change events on.
 *
 * @return  1 if speed change events were successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v3_0
 *
 * \devicesSupported \ref PCIe (version 1.10e4 and greater),
 *          \ref SPLT (version 2.0 and greater)
 */
int STAR_API_CC CFG_MK2_disableSpeedChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup mk2Events
 *
 * Selectively enables time-code notifications on a the channel attached to a
 * given port.
 *
 * \note This function is currently only compatible with
 *       \ref PCIe devices from hardware version v1.10 edit 4.
 *       No check is made by this function to ensure it is being used with a
 *       compatible device.
 *
 * @param deviceID  Device to enable time-code notifications for a port
 *                  on.
 * @param portNum   Port to enable time-code notifications on.
 *
 * @return 1 if time-codes were successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v3_0
 *
 * \devicesSupported \ref PCIe (version 1.10e4 and greater)
 */
int STAR_API_CC CFG_MK2_enableTimeCodeEventsOnPort(STAR_DEVICE_ID deviceID,
    U8 portNum);

/**
 * \ingroup mk2Events
 *
 * Gets whether time-code notifications are enabled on a specific port.
 *
 * \note This function is currently only compatible with
 *       \ref PCIe devices from hardware version v1.10 edit 4.
 *       No check is made by this function to ensure it is being used with a
 *       compatible device.
 *
 * @param deviceID  Device to get whether time-code notifications are
 *                  enabled for a port.
 * @param portNum   Port to get information for.
 * @param pEnabled  User supplied value that will be updated to 1
 *                  if time-code notifications on port are enabled, else 0.
 *
 * @return 1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v3_0
 *
 * \devicesSupported \ref PCIe (version 1.10e4 and greater)
 */
int STAR_API_CC CFG_MK2_getTimeCodeEventsOnPortEnabled(STAR_DEVICE_ID deviceID,
    U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup mk2Events
 *
 * Selectively disables time-code notifications on a the channel attached to a
 * given port.
 *
 * \note This function is currently only compatible with
 *       \ref PCIe devices from hardware version v1.10 edit 4, with
 *       No check is made by this function to ensure it is being used with a
 *       compatible device.
 *
 * @param deviceID  Device to disable time-code notifications for a port on.
 * @param portNum   Port to disable time-code notifications on.
 *
 * @return 1 if time-code notifications were successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v3_0
 *
 * \devicesSupported \ref PCIe (version 1.10e4 and greater)
 */
int STAR_API_CC CFG_MK2_disableTimeCodeEventsOnPort(STAR_DEVICE_ID deviceID,
    U8 portNum);

/**
 * \ingroup linkSpeedMk2
 *
 * Gets the current link speed measured on a specific port.
 *
 * @param deviceID          Device to get the measured link speed for a port.
 * @param portNum           Port to get information for.
 * @param pLinkSpeed        User supplied value that will be updated to contain
 *                          the measured link speed in 100 Kbit/s units, see
 *                          #STAR_CFG_MK2_LINK_SPEED_UNITS_KBPS.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta8
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PCIe,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S,
 *          \ref SPLT
 */
int STAR_API_CC CFG_MK2_getMeasuredLinkSpeed(STAR_DEVICE_ID deviceID,
    U8 portNum, _Out_ U16 *pLinkSpeed);

/**
 * \ingroup user
 *
 * This function is used to specify which output ports time-codes are forwarded on.
 *
 * @param       deviceID        Device to set the time-code distribution ports for.
 * @param       portMask        Bit mask of ports that time code distribution should
 *                              be enabled on.  Valid ports to forward time-codes on
 *                              are 1 through to 13 depending on the device.
 *                               \note Bit 1 corresponds to port 1.
 *                               \note Not all external ports allow time-code forwarding.
 *                                     Check your device's user manual for more details.
 *
 * @return  1 if the register was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v4_0_beta1
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S
 */
int STAR_API_CC CFG_MK2_setTimeCodeDistributionPorts(STAR_DEVICE_ID deviceID,
                                                                      U32 portMask);

/**
 * \ingroup user
 *
 * This function is used to obtain which output ports time-codes are forwarded on.
 *
 * @param       deviceID        Device to get the time-code distribution ports for.
 * @param[out]  pPortMask       Bit mask of ports that time code distribution should
 *                              be enabled on.
 *                               \note Bit 1 corresponds to port 1.
 *
 * @return  1 if the register was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v4_0_beta1
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S
 */
int STAR_API_CC CFG_MK2_getTimeCodeDistributionPorts(STAR_DEVICE_ID deviceID,
                                                        _Out_ U32* pPortMask);

/**
 * \ingroup configuration
 *
 * Sets the time-code flag interpretation mode:
 *
 * 0 - Time-code control bit flags are
 *     distributed with valid time-code values regardless
 *     of the value of the time-code control flags
 *
 * 1 - When the time-code control bit flags are
 *    "00" then valid time-codes are distributed. When
 *    the time-code control flags are not "00" then the
 *    time-code is discarded and the internal time-code
 *    register is not updated.
 *
 *
 * @param       deviceID    Device to set the time-code flag modes for.
 * @param       mode        Mode to set the time code flag mode to,
 *
 * @return  1 if the register was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v4_0_beta1
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S
 */
int STAR_API_CC CFG_MK2_setTimeCodeFlagMode(STAR_DEVICE_ID deviceID,
                                                             U8 mode);

/**
 * \ingroup configuration
 *
 * Obtains the time-code flag interpretation mode:
 *
 * 0 - Time-code control bit flags are
 *     distributed with valid time-code values regardless
 *     of the value of the time-code control flags
 *
 * 1 - When the time-code control bit flags are
 *    "00" then valid time-codes are distributed. When
 *    the time-code control flags are not "00" then the
 *    time-code is discarded and the internal time-code
 *    register is not updated.
 *
 *
 * @param       deviceID    Device to get the time-code flag mode from.
 * @param[out]  pMode        Time code flag mode,
 *
 * @return  1 if the register was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v4_0_beta1
 *
 * \devicesSupported \ref BrickMk2,
 *          \ref BrickMk3,
 *          \ref PXI,
 *          \ref PXIRouter,
 *          \ref RouterMk2S
 */
int STAR_API_CC CFG_MK2_getTimeCodeFlagMode(STAR_DEVICE_ID deviceID,
                                                             _Out_ U8* pMode);

#ifdef __cplusplus
}
#endif

#endif  /* CFG_API_MK2_H */
