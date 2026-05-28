/**
 * \file  cfg_api_router.h
 *
 * \brief Functions used to configure STAR-Dundee routing devices
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

#ifndef CFG_API_ROUTER_H
#define CFG_API_ROUTER_H

#include "star-api.h"
#include "cfg_api_remote.h"
#include "cfg_api_router_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup deviceIdentifier
 *
 * Reads the device identifier information of a device, i.e., the Manufacturer ID,
 * Chip type and version.
 *
 * @param       deviceID        Device to obtain info from.
 *
 * @param[out]  pInfo           Structure that will be updated to contain the
 *                              Identification information for the device.
 *
 * @return      1 if device identifier info was successfully read, else 0.
 */
int STAR_API_CC CFG_ROUTER_getDeviceIdentificationInfo(
                                    STAR_DEVICE_ID deviceID,
                                    _Out_ STAR_CFG_DEVICE_IDENTIFIER_INFO *pInfo);

/**
 * \ingroup deviceIdentifier
 *
 * If the manufacturer is known, this function obtains a string representation
 * of the manufacturers name.
 *
 * @param[in]   pDeviceIdentifierInfo       Pointer to device identification information obtained
 *                                          from a call to CFG_ROUTER_getDeviceIdentificationInfo().
 *
 *
 * @param[out]  manufacturerStr             User allocated zero terminated string of max length
 *                                          STAR_CFG_MANUFACTURER_STR_MAX_LEN that will be updated
 *                                          with the manufacturers name, if known.
 */
void STAR_API_CC CFG_ROUTER_getDeviceManufacturerAsString(
                                    STAR_CFG_DEVICE_IDENTIFIER_INFO* pDeviceIdentifierInfo,
                                    _Out_z_cap_c_(STAR_CFG_MANUFACTURER_STR_MAX_LEN) char* manufacturerStr);

/**
 * \ingroup deviceIdentifier
 *
 * If the manufacturer and device type is known, this function obtains a string representation
 * of the device's type.
 *
 * @param[in]       pDeviceIdentifierInfo   Pointer to device identification information obtained
 *                                          from a call to CFG_ROUTER_getDeviceIdentificationInfo().
 *
 *
 * @param[out]  deviceTypeStr               User allocated zero terminated string of max length
 *                                          STAR_CFG_DEVICE_STR_MAX_LEN that will be updated
 *                                          with the device name, if known.
 */
void STAR_API_CC CFG_ROUTER_getDeviceTypeAsString(
                                    STAR_CFG_DEVICE_IDENTIFIER_INFO* pDeviceIdentifierInfo,
                                    _Out_z_cap_c_(STAR_CFG_DEVICE_STR_MAX_LEN) char* deviceTypeStr);

/**
 * \ingroup deviceIdentifier
 *
 * Reads the network discovery information of a device. This information
 * can be used by a network manager to determine the layout of the network.
 *
 * @param       deviceID        Device to obtain info from.
 *
 * @param[out]  pInfo           Structure that will be updated to contain the
 *                              network discovery information for the device.
 *
 * @return      1 if device identifier info was successfully read, else 0.
 */
int STAR_API_CC CFG_ROUTER_getNetworkDiscoveryInfo(
                                    STAR_DEVICE_ID deviceID,
                                    _Out_ STAR_CFG_NETWORK_DISCOVERY_INFO* pInfo);

/**
 *
 * \ingroup portStatusControl
 *
 *
 * Gets the value of a port or link's status/control register, which can then be used
 * by the various function within the Port Status and Control Group
 *
 * @param      deviceID         Device to obtain info from.
 * @param      portNum              The port number to determine the type of.
 * @param[out] portStatusControl   Value that will be updated with the ports
 *                              status/control value
 *
 * @return  1 if device port type info was successfully read, else 0.
 *
 */
int STAR_API_CC CFG_ROUTER_getPortStatusControl(
                                        STAR_DEVICE_ID deviceID,
                                        U8 portNum,
                                        _Out_ PORT_STATUS_CONTROL* portStatusControl);

/**
 * \ingroup portStatusControl
 *
 * Identifies the type of port attributed to a given port number.
 *
 * @param   portStatusControl   Port Status/Control value obtained from a call to CFG_ROUTER_getPortStatusControl().
 *
 * @return  Type of the port
 */
STAR_CFG_PORT_TYPE STAR_API_CC CFG_ROUTER_getPortType(PORT_STATUS_CONTROL portStatusControl);

/**
 * \ingroup portStatusControl
 *
 * Identifies the output port to which the source port is currently connected whilst
 * routing is in operation.
 *
 * @param   portStatusControl   Port Status/Control value obtained from a call to CFG_ROUTER_getPortStatusControl().
 *
 * @return  Output port connected.  This will have a value of 31 if there is no current port is connected.
 */
U8 STAR_API_CC CFG_ROUTER_getPortConnection(PORT_STATUS_CONTROL portStatusControl);

/**
 * \ingroup portStatusControl
 *
 * Clears all errors on a given port.
 * \note Errors on a port are latched until cleared with this function.
 *
 * @param   deviceID        Device to clear port errors on.
 * @param   portNum         The port to have its errors cleared.
 *
 * @return  1 if the port port errors were successfully cleared, else 0.
 */
int STAR_API_CC CFG_ROUTER_clearPortErrors(
                                        STAR_DEVICE_ID deviceID,
                                        U8 portNum);

/**
 * \ingroup portStatusControl
 *
 * Gets any errors present on the config port. These are errors that arise when malformed or invalid
 * configuration commands are sent to the configuration port.
 *
 * \note Errors on the configuration port are latched until cleared with CFG_ROUTER_clearPortErrors().
 *
 * @param       portStatusControl   Port Status/Control value obtained from a call to CFG_ROUTER_getPortStatusControl().
 * @param[out]  errors              User provided error structure that will be updated to show any errors present on
 *                                  the configuration port.
 *
 * @return  0 if the portStatusControl value passed in is not for a configuration port, otherwise 1.
 */
int STAR_API_CC CFG_ROUTER_getConfigPortErrors(PORT_STATUS_CONTROL portStatusControl,
                                                             _Out_ STAR_CFG_CONFIG_PORT_ERRORS* errors);

/**
 * \ingroup portStatusControl
 *
 * Gets any errors present on a SpaceWire link.
 *
 * \note Errors on a port are latched until cleared with CFG_ROUTER_clearPortErrors().
 *
 * @param       linkStatusControl   Port Status/Control value obtained from a call to CFG_ROUTER_getPortStatusControl().
 * @param[out]  errors              User provided error structure that will be updated to show any errors present on
 *                                  the SpaceWire link.
 *
 * @return  0 if the portStatusControl value passed in is not for a SpaceWire link, otherwise 1.
 */
int STAR_API_CC CFG_ROUTER_getSpaceWireLinkErrors(PORT_STATUS_CONTROL linkStatusControl,
                                                               _Out_ STAR_CFG_SPW_LINK_ERRORS* errors);

/**
 * \ingroup portStatusControl
 *
 * Gets the status of a SpaceWire Link.
 *
 *
 * @param       linkStatusControl   Port Status/Control value obtained from a call to CFG_ROUTER_getPortStatusControl().
 * @param[out]  status              User provided error structure that will be updated to show the status of the
 *                                  the SpaceWire link.
 *
 * @return  0 if the portStatusControl value passed in is not for a SpaceWire link port, otherwise 1.
 */
int STAR_API_CC CFG_ROUTER_getSpaceWireLinkStatus(PORT_STATUS_CONTROL linkStatusControl,
                                                               _Out_ STAR_CFG_SPW_LINK_STATUS* status);

/**
 * \ingroup portStatusControl
 *
 * Sets the state of a SpaceWire Link.
 *
 * @param   deviceID            Device to have its link state set.
 * @param   linkNum             The link to set state for.
 * @param   linkStatus          The values within this structure are used
 *                              to set the state of the SpaceWire Link.
 *                              \note The linkState member of this structure
 *                              is ignored by this function.
 *
 * @return  1 if the link state was successfully set, else 0.
 *
 */
int STAR_API_CC CFG_ROUTER_setSpaceWireLinkStatus(
                                        STAR_DEVICE_ID deviceID,
                                        U8 linkNum,
                                        STAR_CFG_SPW_LINK_STATUS linkStatus);

/**
 * \ingroup portStatusControl
 *
 * Starts a SpaceWire link by setting the start bit, and clearing the disable bit.
 *
 * @param   deviceID            Device to have its link started.
 * @param   linkNum             The link to start.

 *
 * @return  1 if the link was successfully started, else 0.
 *
 */
int STAR_API_CC CFG_ROUTER_startLink(
                                        STAR_DEVICE_ID deviceID,
                                        U8 linkNum);

/**
 * \ingroup portStatusControl
 *
 * Stops a SpaceWire link by clearing the start bit, and setting the disable bit.
 * \note If auto-start is enabled the link will start again
 *                              as soon as it receives NULLs.
 *
 * @param   deviceID            Device to have its link stopped.
 * @param   linkNum             The link to stop.

 *
 * @return  1 if the link was successfully stopped, else 0.
 *
 */
int STAR_API_CC CFG_ROUTER_stopLink(
                                        STAR_DEVICE_ID deviceID,
                                        U8 linkNum);

/**
 * \ingroup portStatusControl
 *
 * Gets any errors present on an External port.
 *
 * \note Errors on a port are latched until cleared with CFG_ROUTER_clearPortErrors().
 *
 * @param       portStatusControl   Port Status/Control value obtained from a call to CFG_ROUTER_getPortStatusControl().
 * @param[out]  errors              User provided error structure that will be updated to show any errors present on
 *                                  the external port.
 *
 * @return  0 if the portStatusControl value passed in is not for an external port, otherwise 1.
 */
int STAR_API_CC CFG_ROUTER_getExternalPortErrors(PORT_STATUS_CONTROL portStatusControl,
                                                              _Out_ STAR_CFG_EXTERNAL_PORT_ERRORS* errors);

/**
 * \ingroup portStatusControl
 *
 * Gets the status of an External port.
 *
 * @param       portStatusControl   Port Status/Control value obtained from a call to CFG_ROUTER_getPortStatusControl().
 * @param[out]  status              User provided error structure that will be updated to show the status of
 *                                  the external port.
 *
 * @return  0 if the portStatusControl value passed in is not for an external port, otherwise 1.
 */
int STAR_API_CC CFG_ROUTER_getExternalPortStatus(PORT_STATUS_CONTROL portStatusControl,
                                                              _Out_ STAR_CFG_EXTERNAL_PORT_STATUS* status);

/**
 * \ingroup gar
 *
 * Gets a routing table entry for a given logical address.
 *
 * @param       deviceID        Device to get GAR table entry for.
 * @param       logicalAddress  Logical address to get GAR table entry for.
 * @param[out]  entry           The GAR table entry for the given logical address.
 *
 * @return  1 if the entry was successfully obtained from the device, else 0.
 */
int STAR_API_CC CFG_ROUTER_getRoutingTableEntry(STAR_DEVICE_ID deviceID,
                                                              U8 logicalAddress,
                                                              _Out_ STAR_CFG_GAR_ENTRY* entry);

/**
 * \ingroup gar
 *
 * Sets a routing table entry for a given logical address.
 *
 * @param       deviceID        Device to set GAR table entry on.
 * @param       logicalAddress  Logical address to set GAR table entry for.
 *                              Valid values are 32 through 255. \note Logical address
 *                              255 is reserved for future use and should not be used.
 *                              See section 10.3.3n of the SpaceWire standard for
 *                              more information.
 * @param   entry           The GAR table entry for the given logical address.
 *
 * @return  1 if the entry was successfully set, else 0.
 */
int STAR_API_CC CFG_ROUTER_setRoutingTableEntry(STAR_DEVICE_ID deviceID,
                                                              U8 logicalAddress,
                                                              STAR_CFG_GAR_ENTRY entry);

/**
 * \ingroup user
 * Sets the value of the network identity register. This is a 32 bit value that
 * can be used to identify the device, typically set by a network manager. It may
 * also be used for any other purpose. This value has no effect on the operation
 * of the router.
 *
 * @param   deviceID        Device to set the network identity for.
 * @param   value           Value to set the network identity to.
 *
 * @return  1 if the register was successfully set, else 0.
 */
int STAR_API_CC CFG_ROUTER_setNetworkIdentity(STAR_DEVICE_ID deviceID,
                                                            REGISTER value);

/**
 * \ingroup user
 * Gets the value of the network identity register. This is a 32 bit value that
 * can be used to identify the device, typically set by a network manager. It may
 * also be used for any other purpose.
 * This value has no effect on the operation of the router.
 *
 * @param       deviceID        Device to get the network identity from.
 * @param[out]  value           Value of the register.
 *
 * @return  1 if the register was successfully obtained, else 0.
 */
int STAR_API_CC CFG_ROUTER_getNetworkIdentity(STAR_DEVICE_ID deviceID,
                                                            _Out_ REGISTER* value);

/**
 * \ingroup user
 * Sets the value of the general purpose register. This is a user defined 32 bit
 * value that can be set by the user as required for their purposes.
 * This value has no effect on the operation of the router.
 *
 * @param   deviceID        Device to set the general purpose register on.
 * @param   value           Value to set the general purpose register to.
 *
 * @return  1 if the register was successfully set, else 0.
 */
int STAR_API_CC CFG_ROUTER_setGeneralPurpose(STAR_DEVICE_ID deviceID,
                                                            REGISTER value);

/**
 * \ingroup user
 * Gets the value of the general purpose register. This is a user defined 32 bit
 * value that can be set by the user as required for their purposes.
 * This value has no effect on the operation of the router.
 *
 * @param       deviceID        Device to get the network identity from.
 * @param[out]  value           Value of the register
 *
 * @return  1 if the register was successfully obtained, else 0.
 */
int STAR_API_CC CFG_ROUTER_getGeneralPurpose(STAR_DEVICE_ID deviceID,
                                                           _Out_ REGISTER* value);

/**
 * \ingroup user
 *
 * This function is used to specify which output ports time-codes are forwarded on.
 *
 * @param       deviceID        Device to set the time-code distribution ports for.
 * @param       portMask        Bit mask of ports that time code distribution should
 *                              be enabled on.  Valid ports to forward time-codes on
 *                              are 1 through 11.
 *                               \note Bit 1 corresponds to port 1.
 *                               \note Not all external ports allow time-code forwarding.
 *                                     Check your device's user manual for more details.
 *
 * @return  1 if the register was successfully obtained, else 0.
 */
int STAR_API_CC CFG_ROUTER_setTimeCodeDistributionPorts(STAR_DEVICE_ID deviceID,
                                                                      U32 portMask);

/**
 * \ingroup user
 *
 * This function is used to obtain which output ports time-codes are forwarded on.
 *
 * @param       deviceID        Device to get the time-code distribution ports for.
 * @param[out]  portMask        Bit mask of ports that time code distribution should
 *                              be enabled on.
 *                               \note Bit 1 corresponds to port 1.
 *
 * @return  1 if the register was successfully obtained, else 0.
 */
int STAR_API_CC CFG_ROUTER_getTimeCodeDistributionPorts(STAR_DEVICE_ID deviceID,
                                                        _Out_ U32* portMask);

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
 */
int STAR_API_CC CFG_ROUTER_setTimeCodeFlagMode(STAR_DEVICE_ID deviceID,
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
 * @param[out]  mode        Time code flag mode,
 *
 * @return  1 if the register was successfully obtained, else 0.
 */
int STAR_API_CC CFG_ROUTER_getTimeCodeFlagMode(STAR_DEVICE_ID deviceID,
                                                             _Out_ U8* mode);

/**
 * \ingroup configuration
 *
 * Obtains the current value of the router's internal time-code counter.
 * *
 * @param       deviceID    Device to get the time-code flag mode from.
 * @param[out]  value       Time-code value,
 *
 * @return  1 if the register was successfully obtained, else 0.
 */
int STAR_API_CC CFG_ROUTER_getTimeCodeValue(STAR_DEVICE_ID deviceID,
                                                          _Out_ U8* value);

/**
 * \ingroup configuration
 *
 * Sets the router's global settings.
 *
 * @param       deviceID        Device to set global settings for.
 * @param       state           Global settings for the router.
 *
 * @return  1 if the entry was successfully set, else 0.
 */
int STAR_API_CC CFG_ROUTER_setRouterGlobalSettings(STAR_DEVICE_ID deviceID,
                                                                 STAR_CFG_ROUTER_GLOBAL_STATE state);

/**
 * \ingroup configuration
 *
 * Gets the router's global settings.
 *
 * @param       deviceID        Device to get global settings from.
 * @param[out]      state           Global settings for the router.
 *
 * @return  1 if the entry was successfully obtained, else 0.
 */
int STAR_API_CC CFG_ROUTER_getRouterGlobalSettings(STAR_DEVICE_ID deviceID,
                                                                 _Out_ STAR_CFG_ROUTER_GLOBAL_STATE* state);

#ifdef __cplusplus
}
#endif

#endif  /* CFG_API_ROUTER_H */
