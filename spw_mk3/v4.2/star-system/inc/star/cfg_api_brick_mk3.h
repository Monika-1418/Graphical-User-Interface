/**
 * \file cfg_api_brick_mk3.h
 *
 * \brief Functions used to configure STAR-Dundee Brick Mk3 and compatible
 *        devices.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Copyright &copy; 2014 STAR-Dundee Ltd.
 */

#ifndef CFG_API_BRICK_MK3_H
#define CFG_API_BRICK_MK3_H



#ifdef __cplusplus
extern "C" {
#endif


#include "star-api.h"
#include "cfg_api_mk2_types.h"
#include "cfg_api_brick_mk3_types.h"
#include "cfg_api_router_types.h"
#include "cfg_api_router.h"


/**
 * \ingroup brickMk3Links
 *
 * Sets the base transmit clock rate on a given link of a Brick Mk3 device:
 *
 *  \f[
 *   Base Clock Rate = \frac{100 \mathrm{MHz} \times MULTIPIER}{DIVISOR}
 *  \f]
 *
 * The \f$MULTIPIER\f$ and \f$DIVISOR\f$ are properties of the
 * {@link #STAR_CFG_MK2_BASE_TRANSMIT_CLOCK} structure, passed as a parameter.
 * The output clock rate must be between 5 MHz and 200 MHz.
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
 * \note This function is currently for use with \ref BrickMk3 devices before
 * hardware version v1.01. \n
 * For later versions use CFG_BRICK_MK3_setTransmitClock(). \n \n
 * The \ref PCIMk2, \ref PCIe, \ref SPLT, \ref BrickMk2 and \ref RouterMk2S do
 * not support this function. For \ref PCIMk2 devices, please use
 * CFG_PCIMK2_setLinkClockFrequency() instead.  For \ref PCIe and \ref SPLT
 * devices, please use either CFG_MK2_setBaseTransmitClock() or
 * CFG_MK2_setTransmitClock() depending on hardware version.  For \ref BrickMk2
 * and \ref RouterMk2S devices, please use
 * CFG_BRICK_MK2_setLinkClockFrequency(). \n \n
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param     deviceID         Device to modify a link's base transmit clock
 *                             rate on.
 * @param     linkNum          Link to have its base transmit clock rate
 *                             modified.
 * @param     clockRateParams  Structure containing the base
 *                             transmit clock rate parameters to be set.
 *
 * @return 1 if link's Base transmit clock frequency was successfully set, else
 *         0.
 *
 * \versionAdded \ref changes_v3_0_beta1
 *
 * \devicesSupported \ref BrickMk3 (earlier than version 1.01)
 */
int STAR_API_CC CFG_BRICK_MK3_setBaseTransmitClock(STAR_DEVICE_ID deviceID,
    U8 linkNum, STAR_CFG_MK2_BASE_TRANSMIT_CLOCK clockRateParams);

/**
 * \ingroup brickMk3Links
 *
 * Gets the base transmit clock rate parameters for a given link of a Brick Mk3
 * device.
 *
 * \note This function is currently for use with \ref BrickMk3 devices before
 * hardware version v1.01. \n
 * For later versions use CFG_BRICK_MK3_getTransmitClock(). \n \n
 * The \ref PCIMk2, \ref PCIe, \ref SPLT, \ref BrickMk2 and \ref RouterMk2S do
 * not support this function. For \ref PCIMk2 devices, please use
 * CFG_PCIMK2_getLinkClockFrequency() instead.  For \ref PCIe and \ref SPLT
 * devices, please use either CFG_MK2_getBaseTransmitClock() or
 * CFG_MK2_getTransmitClock() depending on hardware version.  For \ref BrickMk2
 * and \ref RouterMk2S devices, please use
 * CFG_BRICK_MK2_getLinkClockFrequency(). \n \n
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param      deviceID         Device to get the base transmit clock rate from.
 * @param      linkNum          Link to get base transmit clock rate for.
 * @param[out] pClockRateParams A pointer to an existing structure that will be
 *                              updated to contain the given link's base
 *                              transmit clock rate parameters.
 *
 * @return 1 if the base transmit clock frequency parameters were successfully
 *         obtained, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta1
 *
 * \devicesSupported \ref BrickMk3 (earlier than version 1.01)
 */
int STAR_API_CC CFG_BRICK_MK3_getBaseTransmitClock(STAR_DEVICE_ID deviceID,
    U8 linkNum, _Out_ STAR_CFG_MK2_BASE_TRANSMIT_CLOCK *pClockRateParams);

/**
 * \ingroup brickMk3Links
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
 * \note This function is currently for use with \ref BrickMk3 devices from hardware version v1.01.
 * For earlier versions use CFG_BRICK_MK3_setBaseTransmitClock(). \n \n
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
 * \devicesSupported \ref BrickMk3 (version 1.01 or greater)
 */
int STAR_API_CC CFG_BRICK_MK3_setTransmitClock(STAR_DEVICE_ID deviceID,
    U8 linkNum, STAR_CFG_MK2_BASE_TRANSMIT_CLOCK clockRateParams);

/**
 * \ingroup brickMk3Links
 *
 * Gets the transmit clock rate parameters for a given link.
 *
 * \note This function is currently for use with \ref BrickMk3 devices from hardware version v1.01.
 * For earlier versions use CFG_BRICK_MK3_getBaseTransmitClock(). \n \n
 * No check is made by this function to ensure it is being used with a compatible device.
 *
 * @param      deviceID         Device to get the transmit clock rate from.
 * @param      linkNum          Link to get the transmit clock rate for.
 * @param[out] pClockRateParams Pointer to value that will be updated with the given link's
 *                              transmit clock rate parameters.
 *
 * @return  1 if transmit clock frequency parameters were successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v3_0_beta7
 *
 * \devicesSupported \ref BrickMk3 (version 1.01 or greater)
 */
int STAR_API_CC CFG_BRICK_MK3_getTransmitClock(STAR_DEVICE_ID deviceID,
    U8 linkNum, _Out_ STAR_CFG_MK2_BASE_TRANSMIT_CLOCK *pClockRateParams);

/**
 * \ingroup brickMk3Timestamping
 *
 * Sets the timestamping method to use for the given device.
 *
 * \note This function is currently for use with \ref BrickMk3 devices from
 * hardware version v1.02 and \ref PXI devices.
 * \n\n
 * No check is made by this function to ensure it is being used with a
 * compatible device.
 * \n\n
 * The device will trigger on the rising edge of the external pulse but the
 * Triggering API can be used to change it to trigger on the falling edge using
 * e.g. TRIGGER_BRICK_MK3_enableExtTriggerEdgeDetectMode() and
 * TRIGGER_BRICK_MK3_enableExtTriggerInvert().
 *
 * @param deviceID        Device to set the timestamping method for.
 * @param timestampMethod Timestamp method to enable for device.
 *
 * @return 1 if timestamp method was successfully set, else 0.
 *
 * \versionAdded \ref changes_v3_6
 *
 * \devicesSupported \ref BrickMk3 (version 1.02 or greater),
 *                   \ref PXI
 */
int STAR_API_CC CFG_BRICK_MK3_setTimestampMethod(STAR_DEVICE_ID deviceID,
    STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD timestampMethod);

/**
 * \ingroup brickMk3Timestamping
 *
 * Gets the timestamping method that is currently in use for the given device.
 *
 * \note This function is currently for use with \ref BrickMk3 devices from
 * hardware version v1.02 and \ref PXI devices.
 * \n\n
 * No check is made by this function to ensure it is being used with a
 * compatible device.
 *
 * @param      deviceID         Device to get the timestamping method for.
 * @param[out] pTimestampMethod Pointer to value that will be updated with the
 *                              given link's timestamp method.
 *
 * @return 1 if timestamp method was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v3_6
 *
 * \devicesSupported \ref BrickMk3 (version 1.02 or greater),
 *                   \ref PXI
 */
int STAR_API_CC CFG_BRICK_MK3_getTimestampMethod(STAR_DEVICE_ID deviceID,
    _Out_ STAR_CFG_BRICK_MK3_TIMESTAMP_METHOD *pTimestampMethod);

/**
 * \ingroup brickMk3Timestamping
 *
 * Selectively enables receive timestamp events on a given port.  Enabling
 * receive timestamp events will result in the timestamp that the last packet
 * was received at to be provided as a STAR_STREAM_ITEM_TYPE_RX_TIMESTAMP.
 *
 * \note This function is currently for use with \ref BrickMk3 devices from
 * hardware version v1.02 and \ref PXI devices.
 * \n\n
 * No check is made by this function to ensure it is being used with a
 * compatible device.
 *
 * @param deviceID Device to enable receive timestamp events for a port on.
 * @param portNum  Port to enable receive timestamp events on.
 *
 * @return 1 if receive timestamp packets were successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v3_6
 *
 * \devicesSupported \ref BrickMk3 (version 1.02 or greater),
 *                   \ref PXI
 */
int STAR_API_CC CFG_BRICK_MK3_enableRxTimestampEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup brickMk3Timestamping
 *
 * Gets whether receive timestamp events are enabled on a specific port.
 *
 * \note This function is currently for use with \ref BrickMk3 devices from
 * hardware version v1.02 and \ref PXI devices.
 * \n\n
 * No check is made by this function to ensure it is being used with a
 * compatible device.
 *
 * @param      deviceID Device to get whether receive timestamp events are
 *                      enabled for a port.
 * @param      portNum  Port to get receive timestamp events for.
 * @param[out] pEnabled User supplied value that will be updated to 1 if
 *                      receive timestamp events are enabled, else 0.
 *
 * @return 1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v3_6
 *
 * \devicesSupported \ref BrickMk3 (version 1.02 or greater),
 *                   \ref PXI
 */
int STAR_API_CC CFG_BRICK_MK3_getRxTimestampEventsOnPortEnabled(
    STAR_DEVICE_ID deviceID, U8 portNum, _Out_ int *pEnabled);

/**
 * \ingroup brickMk3Timestamping
 *
 * Selectively disables receive timestamp events on a given port.  Disabling
 * receive timestamp events will result in no timestamp information being
 * provided after the packet.
 *
 * \note This function is currently for use with \ref BrickMk3 devices from
 * hardware version v1.02 and \ref PXI devices.
 * \n\n
 * No check is made by this function to ensure it is being used with a
 * compatible device.
 *
 * @param deviceID Device to disable receive timestamp events for a port on.
 * @param portNum  Port to disable receive timestamp events on.
 *
 * @return 1 if receive timestamp events were successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v3_6
 *
 * \devicesSupported \ref BrickMk3 (version 1.02 or greater),
 *                   \ref PXI
 */
int STAR_API_CC CFG_BRICK_MK3_disableRxTimestampEventsOnPort(
    STAR_DEVICE_ID deviceID, U8 portNum);

/**
 * \ingroup brickMk3Timestamping
 *
 * Sets the current timestamp value for the given device which will be
 * incremented on the next synchronisation pulse.
 *
 * \note This function is currently for use with \ref BrickMk3 devices from
 * hardware version v1.02 and \ref PXI devices.
 * \n\n
 * No check is made by this function to ensure it is being used with a
 * compatible device.
 *
 * @param deviceID Device to set the timestamp value for.
 * @param value    Timestamp value to set for device.
 *
 * @return 1 if timestamp value was successfully set, else 0.
 *
 * \versionAdded \ref changes_v3_6
 *
 * \devicesSupported \ref BrickMk3 (version 1.02 or greater),
 *                   \ref PXI
 */
int STAR_API_CC CFG_BRICK_MK3_setTimestampValue(STAR_DEVICE_ID deviceID,
    U32 value);

/**
 * \ingroup brickMk3Timestamping
 *
 * Gets the current timestamp value.
 *
 * \note This function is currently for use with \ref BrickMk3 devices from
 * hardware version v1.02 and \ref PXI devices.
 * \n\n
 * No check is made by this function to ensure it is being used with a
 * compatible device.
 *
 * @param      deviceID Device to get the timestamp value for.
 * @param[out] pValue   Pointer to value that will be updated with the
 *                              given link's timestamp value.
 *
 * @return 1 if timestamp value was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v3_6
 *
 * \devicesSupported \ref BrickMk3 (version 1.02 or greater),
 *                   \ref PXI
 */
int STAR_API_CC CFG_BRICK_MK3_getTimestampValue(STAR_DEVICE_ID deviceID,
    _Out_ U32 *pValue);

/**
 * \ingroup brickMk3Timestamping
 *
 * Sets the frequency of each generated pulse.
 *
 * \note This function is currently for use with \ref BrickMk3 devices from
 * hardware version v1.02 and \ref PXI devices.
 * \n\n
 * No check is made by this function to ensure it is being used with a
 * compatible device.
 *
 * @param deviceID  Device to set the pulse generator frequency for.
 * @param frequency The frequency value to set.
 *
 * @return 1 if frequency was successfully set, else 0.
 *
 * \versionAdded \ref changes_v3_6
 *
 * \devicesSupported \ref BrickMk3 (version 1.02 or greater),
 *                   \ref PXI
 */
int STAR_API_CC CFG_BRICK_MK3_setPulseGeneratorFrequency(
    STAR_DEVICE_ID deviceID, STAR_CFG_BRICK_MK3_PULSE_FREQ frequency);

/**
 * \ingroup brickMk3Timestamping
 *
 * Gets the frequency of each generated pulse.
 *
 * \note This function is currently for use with \ref BrickMk3 devices from
 * hardware version v1.02 and \ref PXI devices.
 * \n\n
 * No check is made by this function to ensure it is being used with a
 * compatible device.
 *
 * @param      deviceID   Device to get the pulse generator frequency value for.
 * @param[out] pFrequency Pointer to value that will be updated with the
 *                        frequency between each pulse cycle.
 *
 * @return 1 if frequency value was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v3_6
 *
 * \devicesSupported \ref BrickMk3 (version 1.02 or greater),
 *                   \ref PXI
 */
int STAR_API_CC CFG_BRICK_MK3_getPulseGeneratorFrequency(
    STAR_DEVICE_ID deviceID, _Out_ STAR_CFG_BRICK_MK3_PULSE_FREQ *pFrequency);

#ifdef __cplusplus
}
#endif



#endif  /* CFG_API_BRICK_MK3_H */


