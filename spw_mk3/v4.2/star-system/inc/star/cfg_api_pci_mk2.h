/**
 * \file cfg_api_pci_mk2.h
 *
 * \brief Functions used to configure STAR-Dundee PCI Mk2 devices.
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

#ifndef CFG_API_PCI_MK2_H
#define CFG_API_PCI_MK2_H


#include "star-api.h"
#include "cfg_api_pci_mk2_types.h"
#include "cfg_api_remote.h"


#ifdef __cplusplus
extern "C" {
#endif


/** The name of the module, used for logging purposes. */
#define CFG_API_PCI_MK2_MODULE_NAME     "CFG-API_PCI-MK2"


/**
 * \ingroup links
 *
 * Sets the Link Clock Frequency on a given link. The transmit rate of a link is
 * given by:
 *
 *  \f[
 *  Link Transmit Rate = \frac{Link Clock Rate}{Link Clock Rate Divider}
 *  \f]

 * Where \f$Link Clock Rate\f$ is linkFreq and \f$Link Clock Rate Divider\f$ is set by CFG_MK2_setLinkRateDivider().
 *
 * \note This function is for use with \ref PCIMk2 devices only.  For \ref PCIe
 *       and SPLT devices please use CFG_MK2_setBaseTransmitClock() instead.
 *       For \ref BrickMk2 and \ref RouterMk2S devices please use
 *       CFG_BRICK_MK2_setLinkClockFrequency().  For \ref BrickMk3 devices,
 *       please use CFG_BRICK_MK3_setBaseTransmitClock().
 *
 * @param deviceID         Device to modify a links frequency on.
 * @param linkNum          Link to have its frequency modified.
 * @param linkFreq         Link clock frequency to be set.
 *
 * @return  1 if source frequency was successfully set, else 0.
 *
 * \versionAdded \ref changes_v0_8
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC CFG_PCIMK2_setLinkClockFrequency(STAR_DEVICE_ID deviceID,
                                                                    U8 linkNum,
                                                                    STAR_CFG_PCIMK2_LINK_FREQ linkFreq);

/**
 * \ingroup links
 *
 * Gets the Link Clock Frequency for a given link.
 *
 * \note This function is for use with \ref PCIMk2 devices only.  For \ref PCIe
 *       and SPLT devices please use CFG_MK2_getBaseTransmitClock() instead.
 *       For \ref BrickMk2 and \ref RouterMk2S devices please use
 *       CFG_BRICK_MK2_getLinkClockFrequency().  For \ref BrickMk3 devices,
 *       please use CFG_BRICK_MK3_getBaseTransmitClock().
*
 * @param      deviceID         Device to get the links frequency from.
 * @param      linkNum          Link to get frequency for.
 * @param[out] pLinkFreq         Pointer to value that will be updated with the given links frequency.
 *
 * @return  1 if source frequency was successfully obtained, else 0.
 *
 * \versionAdded \ref changes_v0_8
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC CFG_PCIMK2_getLinkClockFrequency(STAR_DEVICE_ID deviceID,
                                                                    U8 linkNum,
                                                                    STAR_CFG_PCIMK2_LINK_FREQ* pLinkFreq);



#ifdef __cplusplus
}
#endif


#endif  /* CFG_API_PCI_MK2_H */


