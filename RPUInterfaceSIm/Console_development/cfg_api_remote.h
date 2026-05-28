/**
 * \file cfg_api_remote.h
 *
 * \brief Functions used to create, configure and destroy paths to remote
 *      devices.
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

#ifndef CFG_API_REMOTE_H
#define CFG_API_REMOTE_H

#include "star-api.h"


#ifdef __cplusplus
extern "C" {
#endif


/**
 * \ingroup remote
 *
 * Creates a Remote Device Identifier. Remote device Identifiers are used to describe to the
 * Configuration API how to reach a remote device.
 *
 * @param deviceId          Local Device communication with the remote device is initiated on.
 * @param localChannel      The number of the channel on the local device on
 *                          which communication with the remote device is made.
 *                          This channel cannot be used for any other purpose
 *                          when a Device Configuration operation is being
 *                          performed.
 * @param name              The name to be used to identify the remote device
 * @param pathTo            Path to the remote device.  This should be the path
 *                          to the remote device's configuration port (port 0),
 *                          including a default logical address.  This means
 *                          that the path will normally end with "0 254".
 * @param returnPath        Return path from the remote device to reach the
 *                          localChannel.  Configuration response packets are
 *                          automatically sent out of the port on which the
 *                          command is received, so this port number is not
 *                          required.  The path should also be terminated with a
 *                          default logical address.  This means that
 *                          <code>returnPath</code> is normally one byte shorter
 *                          than <code>pathTo</code> and ends with "254".
 *
 * @return                  Device ID for the remote device.
 */
STAR_DEVICE_ID STAR_API_CC
STAR_CFG_createRemoteDeviceIdentifier(STAR_DEVICE_ID deviceId,
                                 unsigned char localChannel,
                                 char* name,
                                 STAR_SPACEWIRE_ADDRESS* pathTo,
                                 STAR_SPACEWIRE_ADDRESS* returnPath);

/**
 * \ingroup remote
 *
 * Destroys a Remote Device Identifier.
 *
 * @param remoteDeviceId    The device identifier of the remote device to be destroyed.
 *
 * @return                  Whether the remote device identifier was successfully destroyed.
 */
int STAR_API_CC
STAR_CFG_destroyRemoteDeviceIdentifier(STAR_DEVICE_ID remoteDeviceId);

/**
 * \ingroup remote
 *
 * Gets an array of all remote devices that have been created by all processes.
 *
 * @param[out] count        Pointer to user allocated variable that will be updated to
 *                          contain the count of items in the returned array.
 *
 * @return                  Array of remote device description identifiers.
 *           \note This function returns a snapshot of the current state of the system
 *                 and is not automatically updated. This array must be freed using
 *                 STAR_CFG_destroyRemoteDeviceDescriptionList() when no longer required.
 */
_Ret_opt_cap_(*count) STAR_DEVICE_ID* STAR_API_CC
STAR_CFG_getRemoteDeviceDescriptionList(_Out_ U32* count);

/**
 * \ingroup remote
 * Frees a remote device description list previously created by a call to
 * STAR_CFG_getRemoteDeviceDescriptionList().
 *
 * @param pRemoteDeviceDescriptionList The list to be freed.
 *
 */
void STAR_API_CC STAR_CFG_destroyRemoteDeviceDescriptionList(
    _Post_ptr_invalid_ STAR_DEVICE_ID *pRemoteDeviceDescriptionList);

/**
 * \ingroup remote
 *
 * Gets the number of the local device channel used to communicate with the remote device on.
 *
 * @param       remoteDeviceId      Remote device to get information for.
 * @param[out]  localChannel        Value that will be updated with the local channel.
 *
 * @return  1 on success, else 0.
 */
int STAR_API_CC
STAR_CFG_getRemoteDeviceLocalChannel(STAR_DEVICE_ID remoteDeviceId, unsigned char* localChannel);

/**
 * \ingroup remote
 *
 * Gets the local device used to communicate with the remote device on.
 *
 * @param       remoteDeviceId      Remote device to get information for.
 * @param[out]  localDeviceID       Value that will be updated with the local device ID.
 *
 * @return  1 on success, else 0.
 */
int STAR_API_CC
STAR_CFG_getRemoteDeviceLocalDevice(STAR_DEVICE_ID remoteDeviceId, STAR_DEVICE_ID* localDeviceID);

/**
 * \ingroup remote
 *
 * Gets the path to the specified remote device. The address should be destroyed when it is no
 * longer required using STAR_destroyAddress().
 *
 * @param       remoteDeviceId      Remote device to get information for.
 * @param[out]  pPath               Value that will be updated with pointer to the address.
 *
 * @return  1 on success, else 0.
 */
int STAR_API_CC
STAR_CFG_getRemoteDevicePath(STAR_DEVICE_ID remoteDeviceId, _Out_opt_ STAR_SPACEWIRE_ADDRESS** pPath);

/**
 * \ingroup remote
 *
 * Gets the return path from to the specified remote device. The address should be destroyed when it is no
 * longer required using STAR_destroyAddress().
 *
 * @param       remoteDeviceId      Remote device to get information for.
 * @param[out]  pRetPath            Value that will be updated with pointer to the address.
 *
 * @return  1 on success, else 0
 */
int STAR_API_CC
STAR_CFG_getRemoteDeviceRetPath(STAR_DEVICE_ID remoteDeviceId, _Out_opt_  STAR_SPACEWIRE_ADDRESS** pRetPath);

/**
 * \ingroup remote
 *
 * \deprecated  Replaced by STAR_CFG_getRemoteDeviceDescriptionNameString().
 *
 * Gets the name of the remote device description identified by a given identifier.
 * The required length of a buffer to hold this string is returned.
 * To get the required length of the buffer, pass null as the nameStr
 * parameter.
 *
 * If the buffer supplied is too small then the device name string
 * will be truncated.
 *
 * @param deviceId          The remote device description to check.
 * @param[out] nameStr      A caller supplied buffer that will be updated to contain the
 *                          remote device description's name.
 *                          If this is set to NULL then the function returns the buffer
 *                          size required to hold the string, including null terminator.
 * @param strLen            The length of the caller supplied buffer.
 *
 * @return                  The actual length of the remote device description string excluding the null
 *                          terminator, or 0 if an error occurred.
 */
DEPRECATED(size_t STAR_API_CC
STAR_CFG_getRemoteDeviceDescriptionName(STAR_DEVICE_ID deviceId,
                   _Out_opt_z_cap_(strLen) char *nameStr,
                    size_t strLen));

/**
 * \ingroup remote
 *
 * Gets the name of the remote device description identified by a given
 * identifier.
 *
 * The string returned should be freed by calling STAR_destroyString().
 *
 * @param deviceId          The remote device description to check.
 *
 * @return                  Device name as a null terminated string.
 */
_Check_return_ _Ret_opt_z_ char * STAR_API_CC
STAR_CFG_getRemoteDeviceDescriptionNameString(STAR_DEVICE_ID deviceId);


#ifdef __cplusplus
}
#endif


#endif /* CFG_API_REMOTE_H */

