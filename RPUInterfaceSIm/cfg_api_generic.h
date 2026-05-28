/**
 * \file cfg_api_generic.h
 *
 * \brief Types used with the STAR-Dundee Generic Configuration API 
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

#ifndef CFG_API_GENERIC_H
#define CFG_API_GENERIC_H

#include "star-api.h"
#include "cfg_api_remote.h"
#include "cfg_api_mk2.h"
#include "cfg_api_mk2_types.h"
#include "cfg_api_brick_mk2_types.h"
#include "cfg_api_brick_mk3_types.h"
#include "cfg_api_router_types.h"
#include "cfg_api_generic_common.h"

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------
/**
* \ingroup API_Defines
*
* Positive retvals are success, negative retvals are errors.
* Values > 0 should be interpreted on a per function basis.
*/
#define CFG_STATUS_SUCCESS                                              (0)

/**
* \ingroup API_Defines
*
* Macro to allow testing for success or failure.
*/
#define CFG_SUCCESS(status)                                             ((status) >= CFG_STATUS_SUCCESS ? 1 : 0)

/* Return values -1 to -255 are reserved for RMAP status (inverted from +ve to -ve) */

/**
* \ingroup API_Defines
*
* The API call has failed.
*/
#define CFG_STATUS_FAILURE                                              (-256)

/**
* \ingroup API_Defines
*
* The API call is not compatible with this device type.
*/
#define CFG_STATUS_NOT_COMPATIBLE_WITH_THIS_DEVICE_TYPE                 (-257)

/**
* \ingroup API_Defines
*
* The API call is not compatible with this device's FPGA version.
* The FPGA can be updated to allow this additional functionality.
*/
#define CFG_STATUS_NOT_COMPATIBLE_WITH_THIS_DEVICES_FPGA_VERSION        (-258)

/**
* \ingroup API_Defines
*
* Failed to get "Device Information" for this device.
*/
#define CFG_STATUS_GET_DEVICE_INFO_FAILURE                              (-259)

/**
* \ingroup API_Defines
*
* The Time-code distribution port mask is invalid.
*/
#define CFG_STATUS_TIME_CODE_DISTRIBUTION_PORT_MASK_IS_INVALID          (-260)

/**
* \ingroup API_Defines
*
* The Time-code flag mode is invalid.
*/
#define CFG_STATUS_TIME_CODE_FLAG_MODE_IS_INVALID                       (-261)

/**
* \ingroup API_Defines
*
* The Time-code period is invalid.
*/
#define CFG_STATUS_TIME_CODE_PERIOD_IS_INVALID                          (-262)

/**
* \ingroup API_Defines
*
* The Time-code port is invalid.
*/
#define CFG_STATUS_TIME_CODE_PORT_IS_INVALID                            (-263)

/**
* \ingroup API_Defines
*
* The Identify Source port is invalid.
*/
#define CFG_STATUS_IDENTIFY_SOURCE_PORT_IS_INVALID                      (-264)

/**
* \ingroup API_Defines
*
* The Interface Mode port is invalid.
*/
#define CFG_STATUS_INTERFACE_MODE_PORT_IS_INVALID                       (-265)

/**
* \ingroup API_Defines
*
* The port is invalid.
*/
#define CFG_STATUS_PORT_IS_INVALID                                      (-266)

/**
* \ingroup API_Defines
*
* The logical address is invalid.
*/
#define CFG_STATUS_LOGICAL_ADDRESS_IS_INVALID                           (-267)

/**
* \ingroup API_Defines
*
* The SpaceWire link is invalid.
*/
#define CFG_STATUS_SPACEWIRE_LINK_IS_INVALID                            (-268)

/**
* \ingroup API_Defines
*
* The measured port speed is invalid.
*/
#define CFG_STATUS_MEASURED_SPEED_PORT_IS_INVALID                       (-269)

/**
* \ingroup API_Defines
*
* The Speed Change Events port is invalid.
*/
#define CFG_STATUS_SPEED_CHANGE_EVENTS_PORT_IS_INVALID                  (-270)

/**
* \ingroup API_Defines
*
* The State Change Events port is invalid.
*/
#define CFG_STATUS_STATE_CHANGE_EVENTS_PORT_IS_INVALID                  (-271)

/**
* \ingroup API_Defines
*
* The Port Routing port is invalid.
*/
#define CFG_STATUS_PORT_ROUTING_PORT_IS_INVALID                         (-272)

/**
* \ingroup API_Defines
*
* The Inject Error port is invalid.
*/
#define CFG_STATUS_INJECT_ERROR_PORT_IS_INVALID                         (-273)

/**
* \ingroup API_Defines
*
* The Precision Transmit Rate port is invalid.
*/
#define CFG_STATUS_PRECISION_TRANSMIT_RATE_IS_INVALID                   (-274)

/**
* \ingroup API_Defines
*
* The Periodic Action port is invalid.
*/
#define CFG_STATUS_PERIODIC_ACTION_PORT_IS_INVALID                      (-275)

/**
* \ingroup API_Defines
*
* The Timestamp Events port is invalid.
*/
#define CFG_STATUS_TIMESTAMP_EVENTS_PORT_IS_INVALID                     (-276)

/**
* \ingroup API_Defines
*
* The Clock link is invalid.
*/
#define CFG_STATUS_CLOCK_LINK_IS_INVALID                                (-277)

/**
* \ingroup API_Defines
*
* The Link Rate divider is invalid.
*/
#define CFG_STATUS_LINK_RATE_DIVIDER_IS_INVALID                         (-278)

/**
* \ingroup API_Defines
*
* The Transmit Clock link is invalid.
*/
#define CFG_STATUS_TRANSMIT_CLOCK_LINK_IS_INVALID                       (-279)

/**
* \ingroup API_Defines
*
* The Bit-rate is invalid.
*/
#define CFG_STATUS_BIT_RATE_IS_INVALID                                  (-280)

/**
* \ingroup API_Defines
*
* The Router Timeout mode is invalid.
*/
#define CFG_STATUS_ROUTER_TIMEOUT_MODE_IS_INVALID                       (-281)

/**
* \ingroup API_Defines
*
* The Router Timeout period is invalid.
*/
#define CFG_STATUS_ROUTER_TIMEOUT_PERIOD_IS_INVALID                     (-282)

/**
* \ingroup API_Defines
*
* The Router Disable-on-Silence is invalid.
*/
#define CFG_STATUS_ROUTER_DISABLE_ON_SILENCE_IS_INVALID                 (-283)

/**
* \ingroup API_Defines
*
* The Router Start-on-Request is invalid.
*/
#define CFG_STATUS_ROUTER_START_ON_REQUEST_IS_INVALID                   (-284)

/**
* \ingroup API_Defines
*
* The Router Enable-Self-Addressing is invalid.
*/
#define CFG_STATUS_ROUTER_ENABLE_SELF_ADDRESSING_IS_INVALID             (-285)

/**
* \ingroup API_Defines
*
* The Group Adaptive Routing port mask is invalid.
*/
#define CFG_STATUS_GROUP_ADAPTIVE_ROUTING_PORT_MASK_IS_INVALID          (-286)

/**
* \ingroup API_Defines
*
* The Group Adaptive Routing priority is invalid.
*/
#define CFG_STATUS_GROUP_ADAPTIVE_ROUTING_PRIORITY_IS_INVALID           (-287)

/**
* \ingroup API_Defines
*
* The Group Adaptive Routing delete header is invalid.
*/
#define CFG_STATUS_GROUP_ADAPTIVE_ROUTING_DELETE_HEADER_IS_INVALID      (-288)

/**
* \ingroup API_Defines
*
* The Group Adaptive Routing address is invalid.
*/
#define CFG_STATUS_GROUP_ADAPTIVE_ROUTING_ADDRESS_IS_INVALID            (-289)

/**
* \ingroup API_Defines
*
* The SpaceWire link tri-state flag is invalid.
*/
#define CFG_STATUS_SPACEWIRE_LINK_TRISTATE_IS_INVALID                   (-290)

/**
* \ingroup API_Defines
*
* The SpaceWire link disable flag is invalid.
*/
#define CFG_STATUS_SPACEWIRE_LINK_DISABLE_IS_INVALID                    (-291)

/**
* \ingroup API_Defines
*
* The SpaceWire link start flag is invalid.
*/
#define CFG_STATUS_SPACEWIRE_LINK_START_IS_INVALID                      (-292)

/**
* \ingroup API_Defines
*
* The SpaceWire link autostart flag is invalid.
*/
#define CFG_STATUS_SPACEWIRE_LINK_AUTOSTART_IS_INVALID                  (-293)

/**
* \ingroup API_Defines
*
* The SpaceWire link running flag is invalid.
*/
#define CFG_STATUS_SPACEWIRE_LINK_RUNNING_IS_INVALID                    (-294)

/**
* \ingroup API_Defines
*
* The SpaceWire link state is invalid.
*/
#define CFG_STATUS_SPACEWIRE_LINK_STATE_IS_INVALID                      (-295)

/**
* \ingroup API_Defines
*
* The Port Routing address is invalid.
*/
#define CFG_STATUS_PORT_ROUTING_ADDRESS_IS_INVALID                      (-296)

/**
* \ingroup API_Defines
*
* The Inject Error error is invalid.
*/
#define CFG_STATUS_INJECT_ERROR_ERROR_IS_INVALID                        (-297)

/**
* \ingroup API_Defines
*
* The Inject Errors parity error is invalid.
*/
#define CFG_STATUS_INJECT_ERRORS_PARITY_ERROR_IS_INVALID                (-298)

/**
* \ingroup API_Defines
*
* The Inject Errors escape error is invalid.
*/
#define CFG_STATUS_INJECT_ERRORS_ESCAPE_ERROR_IS_INVALID                (-299)

/**
* \ingroup API_Defines
*
* The Inject Errors insert FCT error is invalid.
*/
#define CFG_STATUS_INJECT_ERRORS_INSERT_FCT_ERROR_IS_INVALID            (-300)

/**
* \ingroup API_Defines
*
* The Inject Errors suppress FCT error is invalid.
*/
#define CFG_STATUS_INJECT_ERRORS_SUPPRESS_FCT_ERROR_IS_INVALID          (-301)

/**
* \ingroup API_Defines
*
* The Inject Errors increment credit error is invalid.
*/
#define CFG_STATUS_INJECT_ERRORS_INCREMENT_CREDIT_ERROR_IS_INVALID      (-302)

/**
* \ingroup API_Defines
*
* The Inject Errors decrement credit error is invalid.
*/
#define CFG_STATUS_INJECT_ERRORS_DECREMENT_CREDIT_ERROR_IS_INVALID      (-303)

/**
* \ingroup API_Defines
*
* The Inject Errors disconnect error is invalid.
*/
#define CFG_STATUS_INJECT_ERRORS_DISCONNECT_ERROR_IS_INVALID            (-304)

/**
* \ingroup API_Defines
*
* The Periodic Action is invalid.
*/
#define CFG_STATUS_PERIODIC_ACTION_ACTION_IS_INVALID                    (-305)

/**
* \ingroup API_Defines
*
* The Timestamp method is invalid.
*/
#define CFG_STATUS_TIMESTAMP_METHOD_IS_INVALID                          (-306)

/**
* \ingroup API_Defines
*
*  The Pulse Generator frequency is invalid.
*/
#define CFG_STATUS_PULSE_GENERATOR_FREQUENCY_IS_INVALID                 (-307)

/**
* \ingroup API_Defines
*
* The Clock Rate multiplier is invalid.
*/
#define CFG_STATUS_CLOCK_RATE_MULTIPLIER_IS_INVALID                     (-308)

/**
* \ingroup API_Defines
*
* The Clock Rate divisor is invalid.
*/
#define CFG_STATUS_CLOCK_RATE_DIVISOR_IS_INVALID                        (-309)

/**
* \ingroup API_Defines
*
* The Clock Rate parameters are invalid.
*/
#define CFG_STATUS_CLOCK_RATE_PARAMETERS_ARE_INVALID                    (-310)

/**
* \ingroup API_Defines
*
* Can't get Clock Rate parameters from bit-rate.
*/
#define CFG_STATUS_CANT_GET_CLOCK_RATE_PARAMETERS_FROM_BIT_RATE         (-311)

/**
* \ingroup API_Defines
*
* The pointer parameter is NULL.
*/
#define CFG_STATUS_POINTER_PARAMETER_IS_NULL                            (-312)

/**
* \ingroup API_Defines
*
* The Transmit bit-rate is invalid.
*/
#define CFG_STATUS_TRANSMIT_BIT_RATE_IS_INVALID                         (-313)

//-------------------------------------
/**
 * \ingroup API_User
 *
 * This function is used to update a device's information held in the shared memory.
 * This is only required for remote devices if they have been removed and replaced.
 *
 * @param       deviceID        Device to update the information for.
 *
 * @return  1 if the device's information was successfully updated, else 0.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_updateDeviceInfo(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_User
 *
 * Gets the value of the network identity register. This is a 32 bit value that
 * can be used to identify the device, typically set by a network manager. It may
 * also be used for any other purpose.
 * This value has no effect on the operation of the router.
 *
 * @param       deviceID        Device to get the network identity from.
 * @param[out]  pValue          Value of the register.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getNetworkIdentity(STAR_DEVICE_ID deviceID, _Out_ REGISTER* pValue);

/**
 * \ingroup API_User
 *
 * Sets the value of the network identity register. This is a 32 bit value that
 * can be used to identify the device, typically set by a network manager. It may
 * also be used for any other purpose. This value has no effect on the operation
 * of the router.
 *
 * @param   deviceID        Device to set the network identity for.
 * @param   value           Value to set the network identity to.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_setNetworkIdentity(STAR_DEVICE_ID deviceID, REGISTER value);

/**
 * \ingroup API_User
 *
 * This function is used to obtain which output ports time-codes are forwarded on.
 *
 * @param       deviceID        Device to get the time-code distribution ports for.
 * @param[out]  pPortMask       Bit mask of ports that time code distribution should
 *                              be enabled on.
 *                              \note Bit 1 corresponds to port 1.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getTimeCodeDistributionPorts(STAR_DEVICE_ID deviceID,
                                                 _Out_ U32* pPortMask);
/**
 * \ingroup API_User
 *
 * This function is used to specify which output ports time-codes are forwarded on.
 *
 * @param       deviceID        Device to set the time-code distribution ports for.
 * @param       portMask        Bit mask of ports that time code distribution should
 *                              be enabled on.  Valid ports to forward time-codes on
 *                              are 1 through to 13 depending on the device.
 *                              \note Bit 1 corresponds to port 1.
 *                              \note Not all external ports allow time-code forwarding.
 *                                    Check your device's user manual for more details.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_setTimeCodeDistributionPorts(STAR_DEVICE_ID deviceID,
                                                 U32 portMask);
/**
 * \ingroup API_User
 *
 * Gets the value of the general purpose register. This is a user defined 32 bit
 * value that can be set by the user as required for their purposes.
 * This value has no effect on the operation of the router.
 *
 * @param       deviceID        Device to get the network identity from.
 * @param[out]  pValue          Value of the register
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getGeneralPurpose(STAR_DEVICE_ID deviceID, _Out_ REGISTER* pValue);

/**
 * \ingroup API_User
 *
 * Sets the value of the general purpose register. This is a user defined 32 bit
 * value that can be set by the user as required for their purposes.
 * This value has no effect on the operation of the router.
 *
 * @param   deviceID        Device to set the general purpose register on.
 * @param   value           Value to set the general purpose register to.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_setGeneralPurpose(STAR_DEVICE_ID deviceID, REGISTER value);

//-------------------------------------
/**
 * \ingroup API_Hardware
 *
 * Reads information about the hardware version of a device and updates the
 * STAR_CFG_FPGA_INFO structure pointed to by the passed in pointer to
 * contain the received version information
 *
 * @param       deviceID    Identifier for the device to get hardware info from.
 * @param[out]  pFPGAInfo   Pointer to a STAR_CFG_FPGA_INFO structure
 *                          that will be updated to contain hardware information
 *                          for the device.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getFPGAInfo(STAR_DEVICE_ID deviceID,
                                _Out_ STAR_CFG_FPGA_INFO* pFPGAInfo);
/**
 * \ingroup API_Hardware
 *
 * Creates a string representation of a STAR_CFG_FPGA_INFO structure.
 *
 * @param      deviceID     Identifier for the device to get the info string from.
 * @param[in]  pFPGAInfo    FPGA information obtained from a call to CFG_getFPGAInfo().
 * @param[out] pVersion     String of length #STAR_CFG_MK2_VERSION_STR_MAX_LEN
 *                          that will be updated to contain a null-terminated
 *                          string representation of the hardware version.
 * @param[out] pBuildDate   String of length #STAR_CFG_MK2_VERSION_STR_MAX_LEN
 *                          that will be updated to contain a null-terminated
 *                          string representation of the hardware build date.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
void STAR_API_CC CFG_FPGAInfoToString(STAR_DEVICE_ID deviceID, _In_ STAR_CFG_FPGA_INFO* pFPGAInfo,
                                      _Out_z_cap_c_(STAR_CFG_MK2_VERSION_STR_MAX_LEN) char* pVersion,
                                      _Out_z_cap_c_(STAR_CFG_MK2_BUILD_DATE_STR_MAX_LEN) char* pBuildDate);
/**
 * \ingroup API_Hardware
 *
 * Flashes the front panel LEDs of a device.  This can be used to identify the
 * physical device to which a device identifier refers to.
 *
 * @param   deviceID    Device to have its LEDs flashed.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_identify(STAR_DEVICE_ID deviceID);

//-------------------------------------
/**
 * \ingroup API_PortStatusControl
 *
 * Clears all errors on a given port.
 * \note Errors on a port are latched until cleared with this function.
 *
 * @param   deviceID        Device to clear port errors on.
 * @param   portNum         The port to have its errors cleared.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_clearPortErrors(STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup API_PortStatusControl
 *
 * Gets any errors present on the config port. These are errors that arise when malformed or invalid
 * configuration commands are sent to the configuration port.
 *
 * \note Errors on the configuration port are latched until cleared with CFG_clearPortErrors().
 *
 * @param       portStatusControl   Port Status/Control value obtained from a call to CFG_getPortStatusControl().
 * @param[out]  pErrors             User provided error structure that will be updated to show any errors present on
 *                                  the configuration port.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getConfigPortErrors(PORT_STATUS_CONTROL portStatusControl,
                                        _Out_ STAR_CFG_CONFIG_PORT_ERRORS* pErrors);
/**
 * \ingroup API_PortStatusControl
 *
 * Sets the state of a SpaceWire Link.
 *
 * @param      deviceID            Device to have its link state set.
 * @param      linkNum             The link to set state for.
 * @param[in]  pLinkStatus         The values within this structure are used
 *                                 to set the state of the SpaceWire Link.
 *                                 \note The linkState member of this structure
 *                                 is ignored by this function.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_setSpaceWireLinkStatus(STAR_DEVICE_ID deviceID, U8 linkNum,
                                           _In_ STAR_CFG_SPW_LINK_STATUS* pLinkStatus);

/**
 * \ingroup API_PortStatusControl
 *
 * Starts a SpaceWire link by setting the start bit, and clearing the disable bit.
 *
 * @param   deviceID            Device to have its link started.
 * @param   linkNum             The link to start.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_startLink(STAR_DEVICE_ID deviceID, U8 linkNum);

/**
 * \ingroup API_PortStatusControl
 *
 * Stops a SpaceWire link by clearing the start bit, and setting the disable bit.
 * \note If auto-start is enabled the link will start again
 *                              as soon as it receives NULLs.
 *
 * @param   deviceID            Device to have its link stopped.
 * @param   linkNum             The link to stop.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_stopLink(STAR_DEVICE_ID deviceID, U8 linkNum);

/**
 * \ingroup API_PortStatusControl
 *
 * Gets any errors present on a SpaceWire link.
 *
 * \note Errors on a port are latched until cleared with CFG_clearPortErrors().
 *
 * @param       portStatusControl   Port Status/Control value obtained from a call to CFG_getPortStatusControl().
 * @param[out]  pErrors             User provided error structure that will be updated to show any errors present on
 *                                  the SpaceWire link.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getSpaceWireLinkErrors(PORT_STATUS_CONTROL portStatusControl,
                                           _Out_ STAR_CFG_SPW_LINK_ERRORS* pErrors);
/**
 * \ingroup API_PortStatusControl
 *
 * Gets the status of a SpaceWire Link.
 *
 *
 * @param       portStatusControl   Port Status/Control value obtained from a call to CFG_getPortStatusControl().
 * @param[out]  pStatus             User provided error structure that will be updated to show the status of the
 *                                  the SpaceWire link.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getSpaceWireLinkStatus(PORT_STATUS_CONTROL portStatusControl,
                                           _Out_ STAR_CFG_SPW_LINK_STATUS* pStatus);
/**
 * \ingroup API_PortStatusControl
 *
 * Gets the value of a port or link's status/control register, which can then be used
 * by the various functions within the Port Status and Control group
 *
 * @param       deviceID             Device to obtain info from.
 * @param       portNum              The port number to determine the type of.
 * @param[out]  pPortStatusControl   Value that will be updated with the ports
 *                                   status/control value
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getPortStatusControl(STAR_DEVICE_ID deviceID, U8 portNum,
                                         _Out_ PORT_STATUS_CONTROL* pPortStatusControl);
/**
 * \ingroup API_PortStatusControl
 *
 * Identifies the type of port attributed to a given port number.
 *
 * @param   portStatusControl   Port Status/Control value obtained from a call to CFG_getPortStatusControl().
 *
 * @return  Type of the port
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
STAR_CFG_PORT_TYPE STAR_API_CC CFG_getPortType(PORT_STATUS_CONTROL portStatusControl);

/**
 * \ingroup API_PortStatusControl
 *
 * Gets any errors present on an external port.
 *
 * \note Errors on a port are latched until cleared with CFG_ROUTER_clearPortErrors().
 *
 * @param       portStatusControl   Port Status/Control value obtained from a call to CFG_getPortStatusControl().
 * @param[out]  pErrors             User provided error structure that will be updated to show any errors present on
 *                                  the external port.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getExternalPortErrors(PORT_STATUS_CONTROL portStatusControl,
                                          _Out_ STAR_CFG_EXTERNAL_PORT_ERRORS* pErrors);
/**
 * \ingroup API_PortStatusControl
 *
 * Gets the status of an external port.
 *
 * @param       portStatusControl   Port Status/Control value obtained from a call to CFG_getPortStatusControl().
 * @param[out]  pStatus             User provided error structure that will be updated to show the status of
 *                                  the external port.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getExternalPortStatus(PORT_STATUS_CONTROL portStatusControl,
                                          _Out_ STAR_CFG_EXTERNAL_PORT_STATUS* pStatus);
/**
 * \ingroup API_PortStatusControl
 *
 * Identifies the output port to which the source port is currently connected whilst
 * routing is in operation.
 *
 * @param   portStatusControl   Port Status/Control value obtained from a call to CFG_getPortStatusControl().
 *
 * @return  Output port connected.  This will have a value of 31 if there is no current port connected.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getPortConnection(PORT_STATUS_CONTROL portStatusControl);

//-------------------------------------
/**
 * \ingroup API_DeviceIdentification
 *
 * Reads the network discovery information of a device. This information
 * can be used by a network manager to determine the layout of the network.
 *
 * @param       deviceID        Device to obtain info from.
 *
 * @param[out]  pInfo           Structure that will be updated to contain the
 *                              network discovery information for the device.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getNetworkDiscoveryInfo(STAR_DEVICE_ID deviceID,
                                            _Out_ STAR_CFG_NETWORK_DISCOVERY_INFO* pInfo);
/**
 * \ingroup API_DeviceIdentification
 *
 * Reads the device identifier information of a device, i.e. the Manufacturer ID,
 * Chip type and version.
 *
 * @param       deviceID        Device to obtain info from.
 *
 * @param[out]  pInfo           Structure that will be updated to contain the
 *                              Identification information for the device.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getDeviceIdentificationInfo(STAR_DEVICE_ID deviceID,
                                                _Out_ STAR_CFG_DEVICE_IDENTIFIER_INFO* pInfo);
/**
 * \ingroup API_DeviceIdentification
 *
 * If the manufacturer is known, this function obtains a string representation
 * of the manufacturers name.
 *
 * @param[in]   pDeviceIdentifierInfo       Pointer to device identification information obtained
 *                                          from a call to CFG_getDeviceIdentificationInfo().
 *
 *
 * @param[out]  pManufacturerStr            User allocated zero terminated string of max length
 *                                          \ref STAR_CFG_MANUFACTURER_STR_MAX_LEN that will be updated
 *                                          with the manufacturers name, if known.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
void STAR_API_CC CFG_getDeviceManufacturerAsString(_In_ STAR_CFG_DEVICE_IDENTIFIER_INFO* pDeviceIdentifierInfo,
                                                   _Out_z_cap_c_(STAR_CFG_MANUFACTURER_STR_MAX_LEN) char* pManufacturerStr);
/**
 * \ingroup API_DeviceIdentification
 *
 * If the manufacturer and device type is known, this function obtains a string representation
 * of the device's type.
 *
 * @param[in]   pDeviceIdentifierInfo   Pointer to device identification information obtained
 *                                      from a call to CFG_getDeviceIdentificationInfo().
 *
 *
 * @param[out]  pDeviceTypeStr          User allocated zero terminated string of max length
 *                                      \ref STAR_CFG_DEVICE_STR_MAX_LEN that will be updated
 *                                      with the device name, if known.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
void STAR_API_CC CFG_getDeviceTypeAsString(_In_ STAR_CFG_DEVICE_IDENTIFIER_INFO* pDeviceIdentifierInfo,
                                           _Out_z_cap_c_(STAR_CFG_DEVICE_STR_MAX_LEN) char* pDeviceTypeStr);
//-------------------------------------
/**
 * \ingroup APIConfiguration
 *
 * Gets the router's global settings.
 *
 * @param       deviceID        Device to get global settings from.
 * @param[out]  pState          Global settings for the router.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getRouterGlobalSettings(STAR_DEVICE_ID deviceID,
                                            _Out_ STAR_CFG_ROUTER_GLOBAL_STATE* pState);
/**
 * \ingroup APIConfiguration
 *
 * Sets the router's global settings.
 *
 * @param       deviceID        Device to set global settings for.
 * @param[in]   pState          Global settings for the router.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_setRouterGlobalSettings(STAR_DEVICE_ID deviceID,
                                            _In_ STAR_CFG_ROUTER_GLOBAL_STATE* pState);
/**
 * \ingroup APIConfiguration
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
 * @param[out]  pMode       Time code flag mode,
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getTimeCodeFlagMode(STAR_DEVICE_ID deviceID,
                                        _Out_ U8* pMode);
/**
 * \ingroup APIConfiguration
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
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_setTimeCodeFlagMode(STAR_DEVICE_ID deviceID,
                                        U8 mode);
/**
 * \ingroup APIConfiguration
 *
 * Obtains the current value of the router's internal time-code counter.
 * *
 * @param       deviceID    Device to get the time-code flag mode from.
 * @param[out]  pValue      Time-code value,
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getTimeCodeValue(STAR_DEVICE_ID deviceID,
                                     _Out_ U8* pValue);
//-------------------------------------
/**
 * \ingroup API_GAR
 *
 * Gets a routing table entry for a given logical address.
 *
 * @param       deviceID        Device to get GAR table entry for.
 * @param       logicalAddress  Logical address to get GAR table entry for.
 * @param[out]  pEntry          The GAR table entry for the given logical address.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getRoutingTableEntry(STAR_DEVICE_ID deviceID, U8 logicalAddress,
                                         _Out_ STAR_CFG_GAR_ENTRY* pEntry);
/**
 * \ingroup API_GAR
 *
 * Sets a routing table entry for a given logical address.
 *
 * @param       deviceID        Device to set GAR table entry on.
 * @param       logicalAddress  Logical address to set GAR table entry for.
 *                              Valid values are 32 through 255. \note Logical address
 *                              255 is reserved for future use and should not be used.
 *                              See section 10.3.3n of the SpaceWire standard for
 *                              more information.
 * @param[in]   pEntry          The GAR table entry for the given logical address.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_setRoutingTableEntry(STAR_DEVICE_ID deviceID, U8 logicalAddress,
                                         _In_ STAR_CFG_GAR_ENTRY* pEntry);
//-------------------------------------
/**
 * \ingroup API_InterfaceMode
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
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getPortRoutingAddress(STAR_DEVICE_ID deviceID, U8 portNum, _Out_ U8* pAddress);

/**
 * \ingroup API_InterfaceMode
 *
 * Sets the address a packet received on a given port should be routed to when
 * interface mode is enabled.
 *
 * \note This is an advanced feature and not necessary for normal usage.
 *
 * @param       deviceID     Device to have one of its ports port routing address changed.
 * @param       portNum      Port to have its port routing address modified.
 * @param       address      New port routing address.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_setPortRoutingAddress(STAR_DEVICE_ID deviceID, U8 portNum, U8 address);

/**
 * \ingroup API_InterfaceMode
 *
 * Disables identification of source ports for interface mode globally.
 *
 * @param   deviceID    Device to disable source port identification on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_disableIdentifySource(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_InterfaceMode
 *
 * When interface mode is enabled on a port, this function globally enables the addition
 * of a leading byte to each received packet indicating which port the packet was
 * received on. For each source port on which this behaviour is desired, a call to
 * CFG_enableIdentifySourceOnPort() must be made.
 *
 *
 * @param   deviceID          Device to enable source port identification on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_enableIdentifySource(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_InterfaceMode
 *
 * Gets whether identify source is enabled.
 *
 * @param      deviceID        Device to check.
 * @param[out] pEnabled        User supplied value that will be updated to 1
 *                             if identify source is enabled, else 0.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getIdentifySourceEnabled(STAR_DEVICE_ID deviceID,
                                             _Out_ int* pEnabled);
/**
 * \ingroup API_InterfaceMode
 *
 * Disables source port identification for a given port.
 *
 * @param   deviceID     Device to disable source port identification for a given port on.
 * @param   portNum      Port to disable source identification on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_disableIdentifySourceOnPort(STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup API_InterfaceMode
 *
 * Enables identification of source port during interface mode for a given port.
 * Interface mode (CFG_enableInterfaceMode()) and Identify Source
 * (CFG_enableIdentifySource()) must be enabled globally for this to have an effect.
 *
 * @param   deviceID     Device to enable source port identification for a given port on.
 * @param   portNum      Port to enable source identification on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_enableIdentifySourceOnPort(STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup API_InterfaceMode
 *
 * Gets whether identify source is enabled for a specific port.
 *
 * \note \ref PCIMk2 and \ref PCIe devices prior to version 1.07 have a bug
 *       which means that the value returned in pEnabled is always 0.
 *
 * @param      deviceID    Device to check.
 * @param      portNum     Port to check.
 * @param[out] pEnabled    User supplied value that will be updated to 1
 *                         if identify source is enabled, else 0.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getIdentifySourceOnPortEnabled(STAR_DEVICE_ID deviceID, U8 portNum,
                                                   _Out_ int* pEnabled);
/**
 * \ingroup API_InterfaceMode
 *
 * Disables interface mode on a device.
 * When interface mode is disabled, the device operates in routing mode.
 *
 * @param   deviceID    Device to disable interface mode on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_disableInterfaceMode(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_InterfaceMode
 *
 * In interface mode a packet which is received on an external port, from a SpaceWire
 * link or from the configuration port will be routed to the port specified by
 * the port routing register of the port (set by CFG_setPortRoutingAddress()).
 *
 * @param   deviceID    Device to enable interface mode on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_enableInterfaceMode(STAR_DEVICE_ID deviceID);

 /**
 * \ingroup API_InterfaceMode
 *
 * Gets whether interface mode has been enabled on a device.
 *
 * @param      deviceID    The device to check.
 * @param[out] pEnabled    User supplied value that will be updated to 1
 *                         if interface mode is enabled, else 0.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getInterfaceModeEnabled(STAR_DEVICE_ID deviceID,
                                            _Out_ int* pEnabled);
 /**
 * \ingroup API_InterfaceMode
 *
 * Selectively disables interface mode on a given port.
 * When interface mode is disabled, the device operates in routing mode.
 *
 * @param   deviceID       Device to disable interface mode for a port on.
 * @param   portNum        Port to disable interface mode on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_disableInterfaceModeOnPort(STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup API_InterfaceMode
 *
 * Selectively enables interface mode on a given port. Interface mode must be enabled
 * globally (CFG_enableInterfaceMode()) for interface mode on a port to be enabled.
 *
 * @param deviceID          Device to enable interface mode for a port on.
 * @param portNum           Port to enable interface mode on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_enableInterfaceModeOnPort(STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup API_InterfaceMode
 *
 * Gets whether interface mode is enabled on a specific port.
 *
 * \note \ref PCIMk2 and \ref PCIe devices prior to version 1.07 have a bug
 *       which means that the value returned in pEnabled is always 0.
 *
 * @param      deviceID        Device to get interface mode enabled for a port on.
 * @param      portNum         Port to get information for.
 * @param[out] pEnabled        User supplied value that will be updated to 1
 *                             if interface mode is enabled, else 0.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getInterfaceModeOnPortEnabled(STAR_DEVICE_ID deviceID, U8 portNum,
                                                  _Out_ int* pEnabled);
//-------------------------------------
/**
 * \ingroup API_Timecodes
 *
 * Disables the device as a time-code master.
 *
 * @param   deviceID    The device to disable as a time-code master.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_disableTimeCodeMaster(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_Timecodes
 *
 * Enables the device as a time-code master.
 *
 * @param   deviceID    The device to enable as a time-code master.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_enableTimeCodeMaster(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_Timecodes
 *
 * Gets whether the device has been enabled as a time-code master.
 *
 * @param      deviceID    The device to check.
 * @param[out] pEnabled    User supplied value that will be updated to 1
 *                         if the device is enabled as a time-code master, else 0.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getTimeCodeMasterEnabled(STAR_DEVICE_ID deviceID,
                                             _Out_ int* pEnabled);
/**
 * \ingroup API_Timecodes
 *
 * Disables external time-code selection for the device.  When external
 * time-code selection is disabled and a time-code is transmitted from an
 * application, the value specified for the time-code is ignored, and the next
 * valid time-code is transmitted by the device.
 *
 * @param   deviceID    The device to disable external time-code selection for.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_disableExternalTimeCodeSelection(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_Timecodes
 *
 * Enables external time-code selection for the device.  When external time-code
 * selection is enabled and a time-code is transmitted from an application, the
 * value specified for the time-code is used. Note that the time-code will only
 * be transmitted by the device if it is the next valid time-code.
 *
 * @param   deviceID    The device to enable external time-code selection for.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_enableExternalTimeCodeSelection(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_Timecodes
 *
 * Gets whether external time-code selection has been enabled for the device.
 * When external time-code selection is disabled and a time-code is transmitted
 * from an application, the value specified for the time-code is ignored, and
 * the next valid time-code is transmitted by the device.  When external time-code
 * selection is enabled and a time-code is transmitted from an application, the
 * value specified for the time-code is used. Note that the time-code will only
 * be transmitted by the device if it is the next valid time-code.
 *
 * @param      deviceID    The device to check.
 * @param[out] pEnabled    User supplied value that will be updated to 1
 *                         if external time-code selection is enabled for the
 *                         device, else 0.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getExternalTimeCodeSelectionEnabled(STAR_DEVICE_ID deviceID,
                                                        _Out_ int* pEnabled);
/**
 * \ingroup API_Timecodes
 *
 * Sets the period between time-code master ticks. This is the number of
 * microseconds between time-codes.
 *
 * @param       deviceID    Device to set time-code period for
 * @param       period      The period to be set in microseconds
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_setTimeCodePeriod(STAR_DEVICE_ID deviceID,
                                      U32 period);
/**
 * \ingroup API_Timecodes
 *
 * Gets the period between time-code master ticks. This is the number of
 * microseconds between time-codes.
 *
 * @param       deviceID    Device to get time-code period from
 * @param[out]  pPeriod     A pointer to a variable that will be updated to
 *                          contain the period in microseconds
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getTimeCodePeriod(STAR_DEVICE_ID deviceID,
                                      _Out_ U32* pPeriod);
//PCIe only
/**
 * \ingroup API_Timecodes
 *
 * Disables time-code counter bypass mode for the device. When time-code counter
 * bypass mode is disabled, any time-code which is received will be checked against
 * the current value of the counter for validity before being forwarded.
 *
 * \note This function is currently only supported by \ref PCIe devices.
 *
 * @param   deviceID    The device to disable time-code counter bypass mode for.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PCIe
 */
int STAR_API_CC CFG_disableTimeCodeCounterBypassMode(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_Timecodes
 *
 * Enables time-code counter bypass mode for the device.  When time-code counter
 * bypass mode is enabled, any time-code which is received will be forwarded out
 * of the other ports on the router, without checking against the current value
 * of the counter. The time-code register will be updated on each received
 * time-code.
 *
 * \note This function is currently only supported by \ref PCIe devices.
 *
 * @param   deviceID    The device to enable time-code counter bypass mode for.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PCIe
 */
int STAR_API_CC CFG_enableTimeCodeCounterBypassMode(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_Timecodes
 *
 * Gets whether time-code counter bypass mode has been enabled for the device.
 * When time-code counter bypass mode is disabled, any time-code which is received
 * will be checked against against the current value of the counter for validity
 * before being forwarded. When time-code counter bypass mode is enabled, any
 * time-code which is received will be forwarded out of the other ports on the
 * router, without checking against the current value of the counter. The time-code
 * register will be updated on each received time-code.
 *
 * \note This function is currently only supported by \ref PCIe devices.
 *
 * @param      deviceID    The device to check.
 * @param[out] pEnabled    User supplied value that will be updated to 1
 *                         if time-code counter bypass mode is enabled for the
 *                         device, else 0.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PCIe
 */
int STAR_API_CC CFG_getTimeCodeCounterBypassModeEnabled(STAR_DEVICE_ID deviceID,
                                                        _Out_ int* pEnabled);
//-------------------------------------
/**
 * \ingroup API_Events
 *
 * Selectively disables speed change events on a given port.  Disabling speed
 * change events will result in speed change event traffic no longer being
 * received on channel 0 when the link speed changes.
 *
 * \note On the \ref PCIe, link speed events are received on the channel
 *       associated with the port rather than channel 0.
 *
 * @param   deviceID            Device to disable speed change events for a port on.
 * @param   portNum             Port to disable speed change events on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_disableSpeedChangeEventsOnPort(STAR_DEVICE_ID deviceID,
                                                   U8 portNum);
/**
 * \ingroup API_Events
 *
 * Selectively enables speed change events on a given port.  Enabling speed
 * change events will result in speed change event traffic being received on
 * channel 0 when the link speed changes.
 *
 * \note On the \ref PCIe, link speed events are received on the channel
 *       associated with the port rather than channel 0.
 *
 * @param   deviceID            Device to enable speed change events for a port on.
 * @param   portNum             Port to enable speed change events on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_enableSpeedChangeEventsOnPort(STAR_DEVICE_ID deviceID,
                                                  U8 portNum);
/**
 * \ingroup API_Events
 *
 * Gets whether speed change events are enabled on a specific port.
 *
 * @param      deviceID            Device to get whether speed change events are
 *                                 enabled for a port.
 * @param      portNum             Port to get information for.
 * @param[out] pEnabled            User supplied value that will be updated to 1
 *                                 if speed change events are enabled, else 0.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getSpeedChangeEventsOnPortEnabled(STAR_DEVICE_ID deviceID,
                                                      U8 portNum,
                                                      _Out_ int* pEnabled);
/**
 * \ingroup API_Events
 *
 * Selectively disables state change events on a given port.  Disabling state
 * change events will result in state change event traffic no longer being
 * received on channel 0 when the link changes to running or disconnected.
 *
 * \note On the \ref PCIe, state events are received on the channel associated
 *       with the port rather than channel 0.
 *
 * @param   deviceID            Device to disable state change events for a port on.
 * @param   portNum             Port to disable state change events on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_disableStateChangeEventsOnPort(STAR_DEVICE_ID deviceID,
                                                   U8 portNum);
/**
 * \ingroup API_Events
 *
 * Selectively enables state change events on a given port.  Enabling state
 * change events will result in state change event traffic being received on
 * channel 0 when the link changes to running or disconnected.
 *
 * \note On the \ref PCIe, state events are received on the channel associated
 *       with the port rather than channel 0.
 *
 * @param   deviceID            Device to enable state change events for a port on.
 * @param   portNum             Port to enable state change events on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_enableStateChangeEventsOnPort(STAR_DEVICE_ID deviceID,
                                                  U8 portNum);
/**
 * \ingroup API_Events
 *
 * Gets whether state change events are enabled on a specific port.
 *
 * @param      deviceID            Device to get whether state change events are
 *                                 enabled for a port.
 * @param      portNum             Port to get information for.
 * @param[out] pEnabled            User supplied value that will be updated to 1
 *                                 if state change events are enabled, else 0.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getStateChangeEventsOnPortEnabled(STAR_DEVICE_ID deviceID,
                                                      U8 portNum,
                                                      _Out_ int* pEnabled);
//PCIe and PXI
/**
 * \ingroup API_Events
 *
 * Selectively disables time-code notifications on a the channel attached to a
 * given port.
 *
 * \note This function is currently compatible with
 *       \ref PCIe devices and \ref PXI devices.
 *
 * @param   deviceID  Device to disable time-code notifications for a port on.
 * @param   portNum   Port to disable time-code notifications on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PCIe,
 *                   \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_disableTimeCodeEventsOnPort(STAR_DEVICE_ID deviceID,
                                                U8 portNum);
/**
 * \ingroup API_Events
 *
 * Selectively enables time-code notifications on a the channel attached to a
 * given port.
 *
 * \note This function is currently compatible with
 *       \ref PCIe devices and \ref PXI devices.
 *
 * @param   deviceID  Device to enable time-code notifications for a port on.
 * @param   portNum   Port to enable time-code notifications on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PCIe,
 *                   \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_enableTimeCodeEventsOnPort(STAR_DEVICE_ID deviceID,
                                               U8 portNum);
/**
 * \ingroup API_Events
 *
 * Gets whether time-code notifications are enabled on a specific port.
 *
 * \note This function is currently compatible with
 *       \ref PCIe devices and \ref PXI devices.
 *
 * @param      deviceID  Device to get whether time-code notifications are
 *                       enabled for a port.
 * @param      portNum   Port to get information for.
 * @param[out] pEnabled  User supplied value that will be updated to 1
 *                       if time-code notifications on port are enabled, else 0.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PCIe,
 *                   \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_getTimeCodeEventsOnPortEnabled(STAR_DEVICE_ID deviceID,
                                                   U8 portNum,
                                                   _Out_ int* pEnabled);
//-------------------------------------
//All but PCI Mk2
/**
 * \ingroup API_LinkSpeed
 *
 * Gets the current link speed measured on a specific port.
 *
 * @param      deviceID            Device to get the measured link speed for a port.
 * @param      portNum             Port to get information for.
 * @param[out] pLinkSpeed          User supplied value that will be updated to contain
 *                                 the measured link speed in 100 Kbit/s units, see
 *                                 #STAR_CFG_MK2_LINK_SPEED_UNITS_KBPS.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getMeasuredLinkSpeed(STAR_DEVICE_ID deviceID,
                                         U8 portNum,
                                         _Out_ U16* pLinkSpeed);
//All devices
/**
 * \ingroup API_Links
 *
 * Gets the transmit bit rate for a given link.
 *
 * \note This function is compatible with all devices.
 *
 * @param      deviceID             Device to get the transmit bit rate from.
 * @param      linkNum              Link to get the transmit bit rate for.
 * @param[out] pSignallingRate      Pointer to value that will be updated with the given link's
 *                                  transmit bit rate in Mbit/s.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_getTransmitSignallingRate(STAR_DEVICE_ID deviceID, U8 linkNum,
                                              _Out_ U32* pSignallingRate);
/**
 * \ingroup API_Links
 *
 * Sets the transmit bit rate on a given link. The transmit bit rate of a link is given
 * in Mbit/s. The bit rate will be between 2 Mbit/s and 400 Mbit/s inclusive, although
 * 400 Mbit/s may not be achievable with every device type.
 *
 * \note This function is compatible with all devices. However, depending upon the
 *       capabilities of the device, an exact bit rate may not be acheivable.
 *
 * \note For the SpaceWire PXI 12 Port Router device, clocks are shared by pairs of
 *       links. For example, setting the transmit bit rate for link 1 will also
 *       affect link 2. Similarly, links 3 and 4, 5 and 6, 7 and 8, 9 and 10, and
 *       11 and 12 share clocks. Note that the odd-numbered link must be used to
 *       set the transmit clock frequency for each pair of links.
 *
 * @param   deviceID            Device to modify a link's transmit bit rate on.
 * @param   linkNum             Link to have its transmit bit rate modified.
 * @param   signallingRate      Transmit bit rate in Mbit/s.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_setTransmitSignallingRate(STAR_DEVICE_ID deviceID, U8 linkNum,
                                              U32 signallingRate);

//Router Mk2S only
/**
 * \ingroup API_PrecisionLinks
 *
 * Disables precision transmit rate.  Precision transmit rate allows the
 * transmit rate of the device to be expressed accurately in Mbit/s as a
 * floating point number using the function CFG_setPrecisionTransmitRate().
 * Note that after being disabled there may be a short delay (less than 250
 * microseconds) before precision transmit rate is actually disabled.
 * Call CFG_getPrecisionTransmitRateInUse to determine if precision transmit
 * rate is no longer in use.
 *
 * \note This function is currently compatible with \ref RouterMk2S devices.
 *
 * @param       deviceID      Device to disable precision transmit rate on.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_disablePrecisionTransmitRate(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_PrecisionLinks
 *
 * Enables precision transmit rate.  Precision transmit rate allows the
 * transmit rate of the device to be expressed accurately in Mbit/s as a
 * floating point number using the function CFG_setPrecisionTransmitRate().
 * Note that after being enabled it can take up to 250 microseconds before
 * precision transmit rate is actually used.
 * Call CFG_getPrecisionTransmitRateInUse to determine if precision transmit
 * rate is actually in use.
 *
 * \note This function is currently compatible with \ref RouterMk2S devices.
 *
 * @param       deviceID      Device to enable precision transmit rate on.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_enablePrecisionTransmitRate(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_PrecisionLinks
 *
 * Determine if precision transmit rate is enabled. Precision transmit rate
 * allows the transmit rate of the device to be expressed accurately in Mbit/s
 * as a floating point number using the function CFG_setPrecisionTransmitRate().
 * Note that although precision transmit rate may be enabled, it can take up to
 * 250 microseconds before precision transmit rate is actually used.
 * Call CFG_getPrecisionTransmitRateInUse to determine if precision transmit
 * rate is actually in use.
 *
 * \note This function is currently compatible with \ref RouterMk2S devices.
 *
 * @param       deviceID    Device to get whether precision transmit rate is enabled.
 * @param[out]  pEnabled    User supplied value that will be updated to 1 if precision
 *                          transmit rate is enabled, else 0.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_getPrecisionTransmitRateEnabled(STAR_DEVICE_ID deviceID,
                                                    _Out_ int* pEnabled);
/**
 * \ingroup API_PrecisionLinks
 *
 * Determine whether precision transmit rate is in use. After being enabled or
 * disabled, it can take up to 250 microseconds before precision transmit is
 * actually used or not used. This function determines whether precision
 * transmit rate is currently in use and can be used to determine if an enable
 * or disable operation has completed.
 *
 * \note This function is currently compatible with \ref RouterMk2S devices.
 *
 * @param       deviceID    Device to get whether precision transmit rate is in use.
 * @param[out]  pInUse      User supplied value that will be updated to 1 if precision
 *                          transmit rate is in use, else 0.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_getPrecisionTransmitRateInUse(STAR_DEVICE_ID deviceID,
                                                  _Out_ int* pInUse);

/**
 * \ingroup API_PrecisionLinks
 *
 * Get the current precision transmit rate on a device in Mbit/s.
 * Precision transmit rate allows the transmit rate of the device to be
 * expressed accurately in Mbit/s as a floating point number using the
 * function CFG_setPrecisionTransmitRate().
 *
 * \note This function is currently compatible with \ref RouterMk2S devices.
 *
 * @param       deviceID        Device to get the precision transmit rate.
 * @param[out]  pTransmitRate   User supplied value that will be updated to
 *                              contain the precision transmit rate in Mbit/s.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_getPrecisionTransmitRate(STAR_DEVICE_ID deviceID,
                                             _Out_ double* pTransmitRate);
/**
 * \ingroup API_PrecisionLinks
 *
 * Set the current precision transmit rate on a device in Mbit/s.
 * Precision transmit rate allows the transmit rate of the device to be
 * expressed accurately in Mbit/s as a floating point number.
 *
 * \note This function is currently compatible with \ref RouterMk2S devices.
 *
 * @param       deviceID        Device to set the precision transmit rate for.
 * @param       transmitRate    The precision transmit rate in Mbit/s to set.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_setPrecisionTransmitRate(STAR_DEVICE_ID deviceID,
                                             double transmitRate);
//-------------------------------------
//Brick Mk2, Brick Mk3, Router Mk2S, PCI Mk2, PCIe, SPLT and PXI
/**
 * \ingroup API_ErrorInjection
 *
 * Immediately injects the specified error on a given device's port.
 *
 * \note This function is compatible \ref BrickMk2, \ref BrickMk3,
 *       \ref RouterMk2S, \ref PCIMk2, \ref PCIe, \ref SPLT and \ref PXI
 *       devices.
 *
 * \note Error injection makes use of the same on-board registers as periodic
 *       actions on devices which support it, therefore error injection cannot
 *       be used on those devices when periodic actions are in progress.
 *
 * @param   deviceID            Identifier of the device to have the error injected on.
 * @param   portNum             Port the error is to be injected on.
 * @param   error               SpaceWire error to be injected.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref BrickMk3,
 *                   \ref PCIMk2,
 *                   \ref PCIe,
 *                   \ref SPLT
 *                   \ref PXI,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_injectError(STAR_DEVICE_ID deviceID,
                                U8 portNum, SPW_ERROR error);

//Brick Mk2, Brick Mk3 and Router Mk2S
/**
 * \ingroup API_ErrorInjection
 *
 * Inject the specified errors on a given port.
 *
 * \note This function is compatible with \ref BrickMk2, \ref BrickMk3
 *       and \ref RouterMk2S devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which errors are supported by each device.
 *
 * @param     deviceID            Device to inject the errors on.
 * @param     portNum             Port to inject the errors on.
 * @param[in] pErrors             Pointer to structure specifying which errors are to be
 *                                injected.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref RouterMk2S,
 *                   \ref BrickMk2,
 *                   \ref PXIRouter,
 *                   \ref STARFireMk3
 */
int STAR_API_CC CFG_injectErrors(STAR_DEVICE_ID deviceID, U8 portNum,
                                 _In_ STAR_CFG_BRICK_MK2_ERRORS* pErrors);
//-------------------------------------
//Brick Mk3, PXI Interface and RMAP and GbE Brick.
/**
 * \ingroup API_Timestamping
 *
 * Selectively disables receive timestamp events on a given port.  Disabling
 * receive timestamp events will result in no timestamp information being
 * provided after the packet.
 *
 * \note This function is currently compatible with \ref BrickMk3 devices
 * and \ref PXI Interface and RMAP devices.
 *
 * @param   deviceID        Device to disable receive timestamp events for a port on.
 * @param   portNum         Port to disable receive timestamp events on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref BrickMk3,
 *                   \ref PXI
 */
int STAR_API_CC CFG_disableRxTimestampEventsOnPort(STAR_DEVICE_ID deviceID,
                                                   U8 portNum);
/**
 * \ingroup API_Timestamping
 *
 * Selectively enables receive timestamp events on a given port.  Enabling
 * receive timestamp events will result in the timestamp that the last packet
 * was received at to be provided as a STAR_STREAM_ITEM_TYPE_RX_TIMESTAMP.
 *
 * \note This function is currently compatible with \ref BrickMk3 devices
 * and \ref PXI Interface and RMAP devices.
 *
 * @param   deviceID        Device to enable receive timestamp events for a port on.
 * @param   portNum         Port to enable receive timestamp events on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref BrickMk3,
 *                   \ref PXI
 */
int STAR_API_CC CFG_enableRxTimestampEventsOnPort(STAR_DEVICE_ID deviceID,
                                                  U8 portNum);
/**
 * \ingroup API_Timestamping
 *
 * Gets whether receive timestamp events are enabled on a specific port.
 *
 * \note This function is currently compatible with \ref BrickMk3 devices
 * and \ref PXI Interface and RMAP devices.
 *
 * @param       deviceID    Device to get whether receive timestamp events are
 *                          enabled for a port.
 * @param       portNum     Port to get receive timestamp events for.
 * @param[out]  pEnabled    User supplied value that will be updated to 1 if
 *                          receive timestamp events are enabled, else 0.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref BrickMk3,
 *                   \ref PXI
 */
int STAR_API_CC CFG_getRxTimestampEventsOnPortEnabled(STAR_DEVICE_ID deviceID,
                                                      U8 portNum,
                                                      _Out_ int* pEnabled);
/**
 * \ingroup API_Timestamping
 *
 * Gets the frequency of each generated pulse.
 *
 * \note This function is currently compatible with \ref BrickMk3 devices
 * and \ref PXI Interface and RMAP devices.
 *
 * @param       deviceID    Device to get the pulse generator frequency value for.
 * @param[out]  pFrequency  Pointer to value that will be updated with the
 *                          frequency between each pulse cycle.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref BrickMk3,
 *                   \ref PXI
 */
int STAR_API_CC CFG_getPulseGeneratorFrequency(STAR_DEVICE_ID deviceID,
                                               _Out_ STAR_CFG_BRICK_MK3_PULSE_FREQ* pFrequency);
/**
 * \ingroup API_Timestamping
 *
 * Sets the frequency of each generated pulse.
 *
 * \note This function is currently compatible with \ref BrickMk3 devices
 * and \ref PXI Interface and RMAP devices.
 *
 * @param       deviceID    Device to set the pulse generator frequency for.
 * @param       frequency   The frequency value to set.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref BrickMk3,
 *                   \ref PXI
 */
int STAR_API_CC CFG_setPulseGeneratorFrequency(STAR_DEVICE_ID deviceID,
                                               STAR_CFG_BRICK_MK3_PULSE_FREQ frequency);

/**
 * \ingroup API_Timestamping
 *
 * Gets the timestamping method that is currently in use for the given device.
 *
 * \note This function is currently compatible with \ref BrickMk3 devices
 * and \ref PXI Interface and RMAP devices.
 *
 * @param       deviceID            Device to get the timestamping method for.
 * @param[out]  pMethod             Pointer to value that will be updated with the
 *                                  given link's timestamp method.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref BrickMk3,
 *                   \ref PXI
 */
int STAR_API_CC CFG_getTimestampMethod(STAR_DEVICE_ID deviceID,
                                       _Out_ STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD* pMethod);

/**
 * \ingroup API_Timestamping
 *
 * Sets the timestamping method to use for the given device.
 *
 * \note This function is currently compatible with \ref BrickMk3 devices
 * and \ref PXI Interface and RMAP devices.
 *
 * The Brick Mk3 will trigger on the rising edge of the external pulse but the
 * Triggering API can be used to change it to trigger on the falling edge using
 * TRIGGER_BRICK_MK3_enableExtTriggerEdgeDetectMode() and
 * TRIGGER_BRICK_MK3_enableExtTriggerInvert().
 *
 * @param       deviceID            Device to set the timestamping method for.
 * @param       method              Timestamp method to enable for device.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref BrickMk3,
 *                   \ref PXI
 */
int STAR_API_CC CFG_setTimestampMethod(STAR_DEVICE_ID deviceID,
                                       STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD method);

/**
 * \ingroup API_Timestamping
 *
 * Gets the current timestamp value.
 *
 * \note This function is currently compatible with \ref BrickMk3 devices
 * and \ref PXI Interface and RMAP devices.
 *
 * @param       deviceID            Device to get the timestamp value for.
 * @param[out]  pValue              Pointer to value that will be updated with
 *                                  the given link's timestamp value.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref BrickMk3,
 *                   \ref PXI
 */
int STAR_API_CC CFG_getTimestampValue(STAR_DEVICE_ID deviceID, _Out_ U32* pValue);

/**
 * \ingroup API_Timestamping
 *
 * Sets the current timestamp value for the given device which will be
 * incremented on the next synchronisation pulse.
 *
 * \note This function is currently compatible with \ref BrickMk3 devices
 * and \ref PXI Interface and RMAP devices.
 *
 * @param       deviceID        Device to set the timestamp value for.
 * @param       value           Timestamp value to set for device.
 *
 * @return      0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref BrickMk3,
 *                   \ref PXI
 */
int STAR_API_CC CFG_setTimestampValue(STAR_DEVICE_ID deviceID, U32 value);

//-------------------------------------
//PCI Mk2 only
/**
 * \ingroup API_Periodic
 *
 * Gets a device's internal clock rate.
 *
 * \note This function is currently only compatible with \ref PCIMk2 devices.
 *
 * @param   deviceID      Identifier of the device to query.
 *
 * @return  The device's clock rate in Hz, or zero if the device does not support
 * periodic actions.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC CFG_getDeviceClockRate(STAR_DEVICE_ID deviceID);

/**
 * \ingroup API_Periodic
 *
 * Starts a periodic action on a given device's port. To stop a periodic action
 * use CFG_stopPeriodicAction().
 *
 * \note This function is currently only compatible with \ref PCIMk2 devices.
 *
 * \note Periodic actions make use of the same on-board registers as error
 *       injection, therefore error injection cannot be used when periodic
 *       actions are in progress.
 *
 * @param   deviceID    Identifier of the device to have periodic action created for.
 * @param   port        Port the periodic action is to be performed on.
 * @param   count       Number of device clock cycles between action repititions.
 *                      See \ref CFG_getDeviceClockRate() for obtaining the
 *                      device's clock rate.
 * @param   action      Action to be performed.  This can be \ref SPW_TRANSMIT_PACKET
 *                      or any of the \ref SPW_ERROR values.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC CFG_startPeriodicAction(STAR_DEVICE_ID deviceID,
                                        unsigned char port,
                                        U32 count,
                                        SPW_ACTION action);
/**
 * \ingroup API_Periodic
 *
 * Stops a periodic action on a given device's port. Periodic actions are started
 * by calling CFG_startPeriodicAction().
 *
 * \note This function is currently only compatible with \ref PCIMk2 devices.
 *
 * @param   deviceID    Identifier of the device performing the periodic action.
 * @param   port        Port the periodic action is being performed on.
 *
 * @return  0 for success and a negative value on error.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC CFG_stopPeriodicAction(STAR_DEVICE_ID deviceID,
                                       unsigned char port);
//-------------------------------------
#ifdef __cplusplus
}
#endif

#endif  /* CFG_API_GENERIC_H */
