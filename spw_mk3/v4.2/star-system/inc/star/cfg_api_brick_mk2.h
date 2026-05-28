/**
 * \file cfg_api_brick_mk2.h
 *
 * \brief Functions used to configure STAR-Dundee Brick Mk2 and compatible
 *        devices.
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

#ifndef CFG_API_BRICK_MK2_H
#define CFG_API_BRICK_MK2_H



#ifdef __cplusplus
extern "C" {
#endif


#include "star-api.h"
#include "cfg_api_brick_mk2_types.h"


/**
 * \ingroup brickInterfaceMode
 *
 * Selectively enables interface mode on a given port of a supported USB device.
 * Interface mode must be enabled globally (CFG_MK2_enableInterfaceMode()) for
 * interface mode on a port to be enabled.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID          Device to enable interface mode for a port on.
 * @param portNum           Port to enable interface mode on.
 *
 * @return  1 if interface mode was successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_enableInterfaceModeOnPort(STAR_DEVICE_ID deviceID,
    U8 portNum);

/**
 * \ingroup brickInterfaceMode
 *
 * Gets whether interface mode is enabled on a specific port of a supported USB
 * device.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID          Device to get interface mode enabled for a port on.
 * @param portNum           Port to get information for.
 * @param pEnabled          User supplied value that will be updated to 1
 *                          if interface mode is enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_getInterfaceModeOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup brickInterfaceMode
 *
 * Selectively disables interface mode on a given port of a supported USB
 * device.  When interface mode is disabled, the device operates in routing
 * mode.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID          Device to disable interface mode for a port on.
 * @param portNum           Port to disable interface mode on.
 *
 * @return  1 if interface mode was successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_disableInterfaceModeOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup brickInterfaceMode
 *
 * Enables identification of the source port of a received packet on a given
 * port, when in interface mode.  Interface mode (CFG_MK2_enableInterfaceMode())
 * and Identify Source (CFG_MK2_enableIdentifySource()) must be enabled globally
 * for this to have an effect.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID     Device to enable source port identification for a given
 *                     port on.
 * @param portNum      Port to enable source identification on.
 *
 * @return  1 if source identification was successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_enableIdentifySourceOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup brickInterfaceMode
 *
 * Gets whether identify source is enabled for a specific port.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID    Device to check.
 * @param portNum     Port to check.
 * @param pEnabled    User supplied value that will be updated to 1
 *                    if identify source is enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_getIdentifySourceOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup brickInterfaceMode
 *
 * Disables source port identification for a given port.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID     Device to disable source port identification for a given
 *                     port on.
 * @param portNum      Port to disable source identification on.
 *
 * @return  1 if source identification was successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_disableIdentifySourceOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup brickInterfaceMode
 *
 * Sets the address a packet received on a given port should be routed to when
 * interface mode is enabled.
 *
 * \note This is an advanced feature and not necessary for normal usage.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID     Device to have one of its port's port routing address
 *                     changed.
 * @param portNum      Port to have its port routing address modified.
 * @param address      New port routing address.
 *
 * @return  1 if the address could be set, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_setPortRoutingAddress(STAR_DEVICE_ID deviceID,
    U8 portNum, U8 address);

/**
 * \ingroup brickInterfaceMode
 *
 * Gets the address a packet received on a given port should be routed to when
 * interface mode is enabled.
 *
 * \note This is an advanced feature and not necessary for normal usage.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param       deviceID     Device to get port routing address from.
 * @param       portNum      Port to get port routing address from.
 * @param[out]  pAddress     Pointer to value that will be updated to contain
 *                           the address.
 *
 * @return  1 if the address could be obtained, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_getPortRoutingAddress(STAR_DEVICE_ID deviceID,
    U8 portNum, _Out_ U8* pAddress);

/**
 * \ingroup brickLinks
 *
 * Sets the Link Clock Frequency on a given link. The transmit rate of a link is
 * given by:
 *
 *  \f[
 *  Link Transmit Rate = \frac{Link Clock Rate}{Link Clock Rate Divider}
 *  \f]

 * Where \f$Link Clock Rate\f$ is linkFreq and \f$Link Clock Rate Divider\f$ is
 * set by CFG_MK2_setLinkRateDivider().
 *
 * \note This function is for use with \ref BrickMk2 and \ref RouterMk2S devices
 *       only.  For \ref PCIe and \ref SPLT devices please use
 *       CFG_MK2_setBaseTransmitClock().  For \ref PCIMk2 devices please use
 *       CFG_PCIMK2_setLinkClockFrequency().  For \ref BrickMk3 devices, please
 *       use CFG_BRICK_MK3_setBaseTransmitClock().
 *
 * @param deviceID         Device to modify a link's frequency on.
 * @param linkNum          Link to have its frequency modified.
 * @param linkFreq         Link clock frequency to be set.
 *
 * @return  1 if source frequency was successfully set, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S
 */
int STAR_API_CC CFG_BRICK_MK2_setLinkClockFrequency(STAR_DEVICE_ID deviceID,
    U8 linkNum, STAR_CFG_BRICK_MK2_LINK_FREQ linkFreq);

/**
 * \ingroup brickLinks
 *
 * Gets the Link Clock Frequency for a given link.
 *
 * \note This function is for use with \ref BrickMk2 and \ref RouterMk2S devices
 *       only.  For \ref PCIe and \ref SPLT devices please use
 *       CFG_MK2_getBaseTransmitClock().  For \ref PCIMk2 devices please use
 *       CFG_PCIMK2_setLinkClockFrequency().  For \ref BrickMk3 devices, please
 *       use CFG_BRICK_MK3_getBaseTransmitClock().
 *
 * @param      deviceID         Device to get the link's frequency from.
 * @param      linkNum          Link to get frequency for.
 * @param[out] pLinkFreq        Pointer to value that will be updated with the
 *                              given links frequency.
 *
 * @return  1 if source frequency was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S
 */
int STAR_API_CC CFG_BRICK_MK2_getLinkClockFrequency(STAR_DEVICE_ID deviceID,
    U8 linkNum, _Out_ STAR_CFG_BRICK_MK2_LINK_FREQ *pLinkFreq);

/**
 * \ingroup brickErrorInjection
 *
 * Inject the specified errors on a given port.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID Device to inject the errors on.
 * @param portNum  Port to inject the errors on.
 * @param pErrors  Pointer to structure specifying which errors are to be
 *                 injected.
 *
 * @return  1 if the errors were successfully injected, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_injectErrors(STAR_DEVICE_ID deviceID, U8 portNum,
    _In_ STAR_CFG_BRICK_MK2_ERRORS *pErrors);

/**
 * \ingroup brickEvents
 *
 * Selectively enables state change events on a given port.  Enabling state
 * change events will result in state change event traffic being received on
 * channel 0 when the link changes to running or disconnected.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID          Device to enable state change events for a port on.
 * @param portNum           Port to enable state change events on.
 *
 * @return  1 if state change events were successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_enableStateChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup brickEvents
 *
 * Gets whether state change events are enabled on a specific port.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID          Device to get whether state change events are
 *                          enabled for a port.
 * @param portNum           Port to get information for.
 * @param pEnabled          User supplied value that will be updated to 1
 *                          if state change events are enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_getStateChangeEventsOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup brickEvents
 *
 * Selectively disables state change events on a given port.  Disabling state
 * change events will result in state change event traffic no longer being
 * received on channel 0 when the link changes to running or disconnected.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID          Device to disable state change events for a port on.
 * @param portNum           Port to disable state change events on.
 *
 * @return  1 if state change events were successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_disableStateChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup brickEvents
 *
 * Selectively enables speed change events on a given port.  Enabling speed
 * change events will result in speed change event traffic being received on
 * channel 0 when the link speed changes.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID          Device to enable speed change events for a port on.
 * @param portNum           Port to enable speed change events on.
 *
 * @return  1 if speed change events were successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_enableSpeedChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup brickEvents
 *
 * Gets whether speed change events are enabled on a specific port.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID          Device to get whether speed change events are
 *                          enabled for a port.
 * @param portNum           Port to get information for.
 * @param pEnabled          User supplied value that will be updated to 1
 *                          if speed change events are enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_getSpeedChangeEventsOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup brickEvents
 *
 * Selectively disables speed change events on a given port.  Disabling speed
 * change events will result in speed change event traffic no longer being
 * received on channel 0 when the link speed changes.
 *
 * \note This function is used by \ref BrickMk2, \ref RouterMk2S
 *       and \ref BrickMk3 devices. Please refer to the \ref ConfigApiFuncs
 *       page for details of which functions are supported by each device.
 *
 * @param deviceID          Device to disable speed change events for a port on.
 * @param portNum           Port to disable speed change events on.
 *
 * @return  1 if speed change events were successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref BrickMk2,
 *                   \ref RouterMk2S,
 *                   \ref BrickMk3
 */
int STAR_API_CC CFG_BRICK_MK2_disableSpeedChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);


#ifdef __cplusplus
}
#endif



#endif  /* CFG_API_BRICK_MK2_H */


