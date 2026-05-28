/**
 * \file ui.h
 *
 * \brief User Interface functions used by example applications.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the declarations of the UI functions that
 * are common acrosss the example applications.
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */

#ifndef STAR_UI_H
#define STAR_UI_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup starUI
 * Prompts the user to choose which device to use for the test if more than one
 * device is available.
 *
 * @param aDeviceTypes An array of device types which are appropriate for the test
 *
 * @param NumRequestedTypes The number of entries in the _aDeviceTypes array
 *
 * @return the identifier of the device to be used for the test, or 0 if there
 *         was an error or no devices are present
 */
STAR_DEVICE_ID STAR_UI_chooseDevice(STAR_DEVICE_TYPE aDeviceTypes[], U32 NumRequestedTypes);

/**
 * \ingroup starUI
 * Prompts the user to choose which channel on the specified device to use for
 * the test if more than one channel is available on the device.
 *
 * @param descriptionStr the description of what the device is to be used for
 *                       (e.g. transmit or receive)
 * @param deviceId the identifier of the device from which the channel will be
 *                 selected
 *
 * @return the number of the channel to be used for the test, or 0 if there was
 *         an error or no channels are present on the device
 */
unsigned char STAR_UI_chooseChannel(STAR_DEVICE_ID deviceId);

/**
 * \ingroup starUI
 * Prompts the user to choose which device and channel to use for the test, if
 * multiple devices and/or channels are available.
 *
 * @param aDeviceTypes An array of device types which are appropriate for the test
 *
 * @param NumRequestedTypes The number of entries in the _aDeviceTypes array
 *
 * @param pDeviceId a pointer to a variable which will be updated to contain
 *                   the identifier of the device to be used for the test
 * @param pChannelId a pointer to a variable which will be updated to contain
 *                   the identifier of the channel to be used for the test
 * @param direction the direction in which traffic is to be sent over the
 *                  channel
 *
 * @return whether a device and a channel was successfully selected
 */
int STAR_UI_chooseDeviceAndChannel(STAR_DEVICE_TYPE aDeviceTypes[], U32 NumRequestedTypes,
    STAR_DEVICE_ID *pDeviceId,
    STAR_CHANNEL_ID *pChannelId,
    STAR_CHANNEL_DIRECTION direction);

/**
 * \ingroup starUI
 * Prompts the user to specify the path address to be added to the front of the
 * packets to be transmitted for a test.
 *
 * @return the SpaceWire address entered, or NULL if there was an error. The
 *         address should be disposed of using {link #STAR_destroyAddress}
 *         when it is no longer needed.
 */
STAR_SPACEWIRE_ADDRESS *STAR_UI_getAddress();

#ifdef __cplusplus
}
#endif

#endif
