/**
 * \file cfg_api_router_mk2s.h
 *
 * \brief Functions used to configure STAR-Dundee Router Mk2S devices.
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

#ifndef CFG_API_ROUTER_MK2S_H
#define CFG_API_ROUTER_MK2S_H



#ifdef __cplusplus
extern "C" {
#endif


#include "star-api.h"


/**
 * \ingroup routerMk2sLinks
 *
 * Enables precision transmit rate on a \ref RouterMk2S device.  Precision
 * transmit rate allows the transmit rate of the device to be expressed
 * accurately in Mbit/s as a floating point number using the function
 * CFG_ROUTER_MK2S_setPrecisionTransmitRate().  Note that after being enabled it
 * can take up to 250 microseconds before precision transmit rate is actually
 * used.  Call CFG_ROUTER_MK2S_getPrecisionTransmitRateInUse to determine if
 * precision transmit rate is actually in use.
 *
 * @param deviceID          Device to enable precision transmit rate on.
 *
 * @return  1 if precision transmit rate was successfully enabled, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_ROUTER_MK2S_enablePrecisionTransmitRate(
    STAR_DEVICE_ID deviceID);

/**
 * \ingroup routerMk2sLinks
 *
 * Determine if precision transmit rate is enabled on a Router Mk2S device.
 * Precision transmit rate allows the transmit rate of the device to be
 * expressed accurately in Mbit/s as a floating point number using the function
 * CFG_ROUTER_MK2S_setPrecisionTransmitRate().  Note that although precision
 * transmit rate may be enabled, it may not yet be in use.  It can take up to
 * 250 microseconds before precision transmit rate is actually used.  Call
 * CFG_ROUTER_MK2S_getPrecisionTransmitRateInUse to determine if precision
 * transmit rate is actually in use.
 *
 * @param deviceID Device to get whether precision transmit rate is enabled.
 * @param pEnabled User supplied value that will be updated to 1 if precision
 *                 transmit rate is enabled, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_ROUTER_MK2S_getPrecisionTransmitRateEnabled(
    STAR_DEVICE_ID deviceID, _Out_ int *pEnabled);

/**
 * \ingroup routerMk2sLinks
 *
 * Disables precision transmit rate on a \ref RouterMk2S device.  Precision
 * transmit rate allows the transmit rate of the device to be expressed
 * accurately in Mbit/s as a floating point number using the function
 * CFG_ROUTER_MK2S_setPrecisionTransmitRate().  Note that after being disabled
 * there may be a short delay (less than 250 microseconds) before precision
 * transmit rate is actually disabled.  Call
 * CFG_ROUTER_MK2S_getPrecisionTransmitRateInUse to determine if precision
 * transmit rate is no longer in use.
 *
 * @param deviceID          Device to disable precision transmit rate on.
 *
 * @return  1 if precision transmit rate was successfully disabled, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_ROUTER_MK2S_disablePrecisionTransmitRate(
    STAR_DEVICE_ID deviceID);

/**
 * \ingroup routerMk2sLinks
 *
 * Determine whether precision transmit rate is in use on a \ref RouterMk2S
 * device.  After being enabled or disabled, it can take up to 250 microseconds
 * before precision transmit is actually used or not used.  This function
 * determines whether precision transmit rate is currently in use and can be
 * used to determine if an enable or disable operation has completed.
 *
 * @param deviceID Device to get whether precision transmit rate is in use.
 * @param pInUse User supplied value that will be updated to 1 if precision
 *               transmit rate is in use, else 0.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_ROUTER_MK2S_getPrecisionTransmitRateInUse(
    STAR_DEVICE_ID deviceID, _Out_ int *pInUse);

/**
 * \ingroup routerMk2sLinks
 *
 * Get the current precision transmit rate on a \ref RouterMk2S device in
 * Mbit/s.  Precision transmit rate allows the transmit rate of the device to
 * be expressed accurately in Mbit/s as a floating point number using the
 * function CFG_ROUTER_MK2S_setPrecisionTransmitRate().
 *
 * @param deviceID Device to get the precision transmit rate.
 * @param pTransmitRate User supplied value that will be updated to contain the
 *                      precision transmit rate in Mbit/s.
 *
 * @return  1 if the information could be obtained from the device, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_ROUTER_MK2S_getPrecisionTransmitRate(
    STAR_DEVICE_ID deviceID, _Out_ double *pTransmitRate);

/**
 * \ingroup routerMk2sLinks
 *
 * Set the current precision transmit rate on a \ref RouterMk2S device in
 * Mbit/s.  Precision transmit rate allows the transmit rate of the device to
 * be expressed accurately in Mbit/s as a floating point number.
 *
 * @param deviceID Device to set the precision transmit rate for.
 * @param transmitRate the precision transmit rate in Mbit/s to set.
 *
 * @return  1 if the rate for the device was successfully set, else 0.
 *
 * \versionAdded \ref changes_v2_0_beta_1
 *
 * \devicesSupported \ref RouterMk2S
 */
int STAR_API_CC CFG_ROUTER_MK2S_setPrecisionTransmitRate(
    STAR_DEVICE_ID deviceID, double transmitRate);



#ifdef __cplusplus
}
#endif



#endif  /* CFG_API_ROUTER_MK2S_H */


