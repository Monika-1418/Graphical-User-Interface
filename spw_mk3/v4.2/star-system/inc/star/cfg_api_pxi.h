/**
 * \file cfg_api_pxi.h
 *
 * \brief Functions used to configure STAR-Dundee PXI
 *        devices.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Copyright &copy; 2015 STAR-Dundee Ltd.
 */

#ifndef CFG_API_PXI_H
#define CFG_API_PXI_H



#ifdef __cplusplus
extern "C" {
#endif


#include "star-api.h"
#include "cfg_api_mk2_types.h"


/**
 * \ingroup PXILinks
 *
 * Sets the Link rate divider for a PXI device. The transmit rate of a link is
 * given by:
 *
 *  \f[
 *  Link Transmit Rate = \frac{Base Clock Rate}{Link Clock Rate Divider}  \times 2
 *  \f]

 * Where \f$Base Clock Rate\f$ is set by the appropriate function (i.e.
 * CFG_PXI_setBaseTransmitClock()) and
 * \f$Link Clock Rate Divider =\f$ divider.
 *
 * \note It is never necessary to set a link rate divider higher than 100, as the minimum
 *       transmit rate permitted by SpaceWire is 2 Mbit/s (200 Mbit/s Max / 100).
 *
 * \note If an odd number (other than 1) is specified as the divider parameter, the previous
 *       even integer will be used instead. For example: if a divider value of 3 is specified,
 *       the actual divider set shall be 2.
 *
 * \note This function is currently for use with PXI devices before hardware version V1.1.
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param deviceID         Device to modify a links divider on.
 * @param divider          Value of the new divider. Valid input: 1, Even numbers 2 through 126.
 *
 * @return  1 if divider was successfully set, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta4
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_setLinkRateDivider(STAR_DEVICE_ID deviceID, U8 divider);

/**
 * \ingroup PXILinks
 *
 * Gets the Link rate divider for a PXI device.
 *
 * \note This function is currently for use with PXI devices before hardware version V1.1.
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param      deviceID         Device to get a links divider from..
 * @param[out] pDivider         Value of the divider.
 *
 * @return  1 if was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta4
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_getLinkRateDivider(STAR_DEVICE_ID deviceID, _Out_ U8* pDivider);

/**
 * \ingroup PXILinks
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
 * CFG_PXI_setLinkRateDivider() function.
 *
 * \note This function is currently for use with PXI devices before hardware version V1.1.
 * For later versions use CFG_PXI_setTransmitClock(). \n \n
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param deviceID         Device to modify a link's base transmit clock rate on.
 * @param linkNum          Link to have its base transmit clock rate modified.
 * @param clockRateParams  Base transmit clock rate parameters to be set.
 *
 * @return  1 if link's Base transmit clock frequency was successfully set, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta4
 *
 * \devicesSupported \ref PXI (prior to version 1.1),
 *                   \ref PXIRouter (prior to version 1.1)
 */
int STAR_API_CC CFG_PXI_setBaseTransmitClock(STAR_DEVICE_ID deviceID,
                                             U8 linkNum,
                                             STAR_CFG_MK2_BASE_TRANSMIT_CLOCK clockRateParams);

/**
 * \ingroup PXILinks
 *
 * Gets the  base transmit clock rate parameters for a given link.
 *
 * \note This function is currently for use with PXI devices before hardware version V1.1.
 * For later versions use CFG_PXI_getTransmitClock(). \n \n
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param      deviceID         Device to get the base transmit clock rate from.
 * @param      linkNum          Link to get base transmit clock rate for.
 * @param[out] clockRateParams  Pointer to value that will be updated with the given link's
 *                              Base transmit clock rate parameters.
 *
 * @return  1 if Base transmit clock frequency parameters were successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta4
 *
 * \devicesSupported \ref PXI (prior to version 1.1),
 *                   \ref PXIRouter (prior to version 1.1)
 */
int STAR_API_CC CFG_PXI_getBaseTransmitClock(STAR_DEVICE_ID deviceID,
                                             U8 linkNum,
                                             _Out_ STAR_CFG_MK2_BASE_TRANSMIT_CLOCK* clockRateParams);

/**
 * \ingroup PXILinks
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
 * 2 Mbit/s and 400 Mbit/s
 *
 * Note that for data rates below 10 Mbit/s, an exact rate may not be achievable
 * and it may be rounded up or down slightly.
 *
 * \note This function is currently for use with PXI devices from hardware version V1.1.
 * For earlier versions use CFG_PXI_setBaseTransmitClock(). \n \n
 * No check is made by this function to ensure it is being used with a compatible device. \n
 *
 * \note For the SpaceWire PXI 12 Port Router device, clocks are shared by pairs of
 *       links. For example, setting the transmit clock frequency for link 1 will
 *       also affect link 2. Similarly, links 3 and 4, 5 and 6, 7 and 8, 9 and 10,
 *       and 11 and 12 share clocks. Note that the odd-numbered link must be used
 *       to set the transmit clock frequency for each pair of links.
 *
 * @param deviceID         Device to modify a link's transmit clock rate on.
 * @param linkNum          Link to have its transmit clock rate modified.
 * @param clockRateParams  Transmit clock rate parameters to be set.
 *
 * @return  1 if link's transmit clock frequency was successfully set, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta7
 *
 * \devicesSupported \ref PXI (version 1.1 or greater),
 *                   \ref PXIRouter (version 1.1 or greater)
 */
int STAR_API_CC CFG_PXI_setTransmitClock(STAR_DEVICE_ID deviceID,
                                         U8 linkNum,
                                         STAR_CFG_MK2_BASE_TRANSMIT_CLOCK clockRateParams);

/**
 * \ingroup PXILinks
 *
 * Gets the transmit clock rate parameters for a given link.
 *
 * \note This function is currently for use with PXI devices from hardware version V1.1.
 * For earlier versions use CFG_PXI_getBaseTransmitClock(). \n \n
 * No check is made by this function to ensure it is being used with a compatible device. \n
 *
 * \note For the SpaceWire PXI 12 Port Router device, clocks are shared by pairs of
 *       links. For example, getting the transmit clock frequency for link 1 will
 *       return the transmit clock frequency also shared by link 2. Similarly, links 3
 *       and 4, 5 and 6, 7 and 8, 9 and 10, and 11 and 12 share clocks. Note that the
 *       odd-numbered link must be used to get the transmit clock frequency for each
 *       pair of links.
 *
 * @param      deviceID         Device to get the transmit clock rate from.
 * @param      linkNum          Link to get the transmit clock rate for.
 * @param[out] clockRateParams  Pointer to value that will be updated with the given link's
 *                              transmit clock rate parameters.
 *
 * @return  1 if transmit clock frequency parameters were successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta7
 *
 * \devicesSupported \ref PXI (version 1.1 or greater),
 *                   \ref PXIRouter (version 1.1 or greater)
 */
int STAR_API_CC CFG_PXI_getTransmitClock(STAR_DEVICE_ID deviceID,
                                         U8 linkNum,
                                         _Out_ STAR_CFG_MK2_BASE_TRANSMIT_CLOCK* clockRateParams);

/**
 * \ingroup errInjectPXI
 *
 * Immediately injects the specified error on a given device's port.
 *
 * \note This function is currently only compatible with \ref PXI devices.
 *       No check is made by this function to ensure it is being used with
 *       a compatible device.
 *
 * @param deviceID      Identifier of the device to have the error injected on.
 * @param port          Port the error is to be injected on.
 * @param error         SpaceWire error to be injected.
 *
 * @return      1 if the error information was successfully sent to the device,
 *              else 0.
 *
 * \versionAdded \ref changes_v3_0_beta4
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_injectError(STAR_DEVICE_ID deviceID,
                                    unsigned char port,
                                    SPW_ERROR error);

/**
 * \ingroup PXIInterfaceMode
 *
 * Selectively enables interface mode on a given port of a supported PXI device.
 * Interface mode must be enabled globally (CFG_MK2_enableInterfaceMode()) for
 * interface mode on a port to be enabled.
 *
 * @param deviceID          Device to enable interface mode for a port on.
 * @param portNum           Port to enable interface mode on.
 *
 * @return  1 if interface mode was successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_enableInterfaceModeOnPort(STAR_DEVICE_ID deviceID,
    U8 portNum);

/**
 * \ingroup PXIInterfaceMode
 *
 * Gets whether interface mode is enabled on a specific port of a supported PXI
 * device.
 *
 * @param deviceID          Device to get interface mode enabled for a port on.
 * @param portNum           Port to get information for.
 * @param pEnabled          User supplied value that will be updated to 1
 *                          if interface mode is enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_getInterfaceModeOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup PXIInterfaceMode
 *
 * Selectively disables interface mode on a given port of a supported PXI
 * device.  When interface mode is disabled, the device operates in routing
 * mode.
 *
 * @param deviceID          Device to disable interface mode for a port on.
 * @param portNum           Port to disable interface mode on.
 *
 * @return  1 if interface mode was successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_disableInterfaceModeOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup PXIInterfaceMode
 *
 * Enables identification of the source port of a received packet on a given
 * port, when in interface mode.  Interface mode (CFG_MK2_enableInterfaceMode())
 * and Identify Source (CFG_MK2_enableIdentifySource()) must be enabled globally
 * for this to have an effect.
 *
 * @param deviceID     Device to enable source port identification for a given
 *                     port on.
 * @param portNum      Port to enable source identification on.
 *
 * @return  1 if source identification was successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_enableIdentifySourceOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup PXIInterfaceMode
 *
 * Gets whether identify source is enabled for a specific port.
 *
 * @param deviceID    Device to check.
 * @param portNum     Port to check.
 * @param pEnabled    User supplied value that will be updated to 1
 *                    if identify source is enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_getIdentifySourceOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup PXIInterfaceMode
 *
 * Disables source port identification for a given port.
 *
 * @param deviceID     Device to disable source port identification for a given
 *                     port on.
 * @param portNum      Port to disable source identification on.
 *
 * @return  1 if source identification was successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_disableIdentifySourceOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup PXIInterfaceMode
 *
 * Sets the address a packet received on a given port should be routed to when
 * interface mode is enabled.
 *
 * \note This is an advanced feature and not necessary for normal usage.
 *
 * @param deviceID     Device to have one of its port's port routing address
 *                     changed.
 * @param portNum      Port to have its port routing address modified.
 * @param address      New port routing address.
 *
 * @return  1 if the address could be set, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_setPortRoutingAddress(STAR_DEVICE_ID deviceID,
    U8 portNum, U8 address);

/**
 * \ingroup PXIInterfaceMode
 *
 * Gets the address a packet received on a given port should be routed to when
 * interface mode is enabled.
 *
 * \note This is an advanced feature and not necessary for normal usage.
 *
 * @param       deviceID     Device to get port routing address from.
 * @param       portNum      Port to get port routing address from.
 * @param[out]  pAddress     Pointer to value that will be updated to contain
 *                           the address.
 *
 * @return  1 if the address could be obtained, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_getPortRoutingAddress(STAR_DEVICE_ID deviceID,
    U8 portNum, _Out_ U8* pAddress);

/**
 * \ingroup PXIEvents
 *
 * Selectively enables state change events on a given port.  Enabling state
 * change events will result in state change event traffic being received on
 * channel 0 when the link changes to running or disconnected.
 *
 * @param deviceID          Device to enable state change events for a port on.
 * @param portNum           Port to enable state change events on.
 *
 * @return  1 if state change events were successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_enableStateChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup PXIEvents
 *
 * Gets whether state change events are enabled on a specific port.
 *
 * @param deviceID          Device to get whether state change events are
 *                          enabled for a port.
 * @param portNum           Port to get information for.
 * @param pEnabled          User supplied value that will be updated to 1
 *                          if state change events are enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_getStateChangeEventsOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup PXIEvents
 *
 * Selectively disables state change events on a given port.  Disabling state
 * change events will result in state change event traffic no longer being
 * received on channel 0 when the link changes to running or disconnected.
 *
 * @param deviceID          Device to disable state change events for a port on.
 * @param portNum           Port to disable state change events on.
 *
 * @return  1 if state change events were successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_disableStateChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup PXIEvents
 *
 * Selectively enables speed change events on a given port.  Enabling speed
 * change events will result in speed change event traffic being received on
 * channel 0 when the link speed changes.
 *
 * @param deviceID          Device to enable speed change events for a port on.
 * @param portNum           Port to enable speed change events on.
 *
 * @return  1 if speed change events were successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_enableSpeedChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup PXIEvents
 *
 * Gets whether speed change events are enabled on a specific port.
 *
 * @param deviceID          Device to get whether speed change events are
 *                          enabled for a port.
 * @param portNum           Port to get information for.
 * @param pEnabled          User supplied value that will be updated to 1
 *                          if speed change events are enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_getSpeedChangeEventsOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup PXIEvents
 *
 * Selectively disables speed change events on a given port.  Disabling speed
 * change events will result in speed change event traffic no longer being
 * received on channel 0 when the link speed changes.
 *
 * @param deviceID          Device to disable speed change events for a port on.
 * @param portNum           Port to disable speed change events on.
 *
 * @return  1 if speed change events were successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v3_3
 *
 * \devicesSupported \ref PXI,
 *                   \ref PXIRouter
 */
int STAR_API_CC CFG_PXI_disableSpeedChangeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup PXIEvents
 *
 * Selectively enables time-code notifications on a channel attached to a
 * given port.
 *
 * \note This function is currently only compatible with
 *       \ref PXI Interface devices from hardware version v1.2 edit 3 and with
 *       \ref PXI Router devices from hardware version v1.2 edit 4.
 *       No check is made by this function to ensure it is being used with a
 *       compatible device.
 *
 * @param deviceID  Device to enable time-code notifications for a port
 *                  on.
 * @param portNum   Port to enable time-code notifications on.
 *
 * @return 1 if time-codes were successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PXI (all Mk2 versions and Mk1 version 1.2 edit 3 or greater),
 *                   \ref PXIRouter (all Mk2 versions and Mk1 version 1.2 edit 4 or greater)
 */
int STAR_API_CC CFG_PXI_enableTimeCodeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup PXIEvents
 *
 * Gets whether time-code notifications are enabled on a specific port.
 *
 * \note This function is currently only compatible with
 *       \ref PXI Interface devices from hardware version v1.2 edit 3 and with
 *       \ref PXI Router devices from hardware version v1.2 edit 4.
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
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PXI (all Mk2 versions and Mk1 version 1.2 edit 3 or greater),
 *                   \ref PXIRouter (all Mk2 versions and Mk1 version 1.2 edit 4 or greater)
 */
int STAR_API_CC CFG_PXI_getTimeCodeEventsOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup PXIEvents
 *
 * Selectively disables time-code notifications on a channel attached to a
 * given port.
 *
 * \note This function is currently only compatible with
 *       \ref PXI Interface devices from hardware version v1.2 edit 3 and with
 *       \ref PXI Router devices from hardware version v1.2 edit 4.
 *       No check is made by this function to ensure it is being used with a
 *       compatible device.
 *
 * @param deviceID  Device to disable time-code notifications for a port on.
 * @param portNum   Port to disable time-code notifications on.
 *
 * @return 1 if time-code notifications were successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v4_00
 *
 * \devicesSupported \ref PXI (all Mk2 versions and Mk1 version 1.2 edit 3 or greater),
 *                   \ref PXIRouter (all Mk2 versions and Mk1 version 1.2 edit 4 or greater)
 */
int STAR_API_CC CFG_PXI_disableTimeCodeEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

#ifdef __cplusplus
}
#endif



#endif  /* CFG_API_PXI_H */


