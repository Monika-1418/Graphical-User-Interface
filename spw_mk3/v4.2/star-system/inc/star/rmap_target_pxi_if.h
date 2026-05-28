/**
* \file rmap_target_pxi_if.h
*
* \brief Functions used to configure and control the RMAP target capabilities of the STAR-Dundee
*        PXI Interface devices.
*
* \author STAR-Dundee Ltd.\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Copyright &copy; 2016 STAR-Dundee Ltd.
*/

#ifndef RMAP_TARGET_PXI_IF_H
#define RMAP_TARGET_PXI_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rmap_target_types.h"

/**
* \ingroup rmapTargetConf
*
* Gets the RMAP target status for a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to get status from.
* @param pStatus        Pointer to a value to be updated with the target's status.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getStatus(STAR_DEVICE_ID deviceId, U32 target,
    TARGET_STATUS *pStatus);

/**
* \ingroup rmapTargetConf
*
* Sets the address offset for a specific target.
* The address offset determines where the target's memory region begins.
*
* @param deviceId       Device containing the target.
* @param target         Target to set address offset for.
* @param offset         Address offset in MBytes.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_setAddressOffset(STAR_DEVICE_ID deviceId, U32 target,
    U32 offset);

/**
* \ingroup rmapTargetConf
*
* Gets the address offset from a specific target.
* The address offset determines where the target's memory region begins.
*
* @param deviceId       Device containing the target.
* @param target         Target to get address offset from.
* @param pOffset        Pointer to a value to be updated with the target's address offset.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getAddressOffset(STAR_DEVICE_ID deviceId, U32 target,
    U32 *pOffset);

/**
* \ingroup rmapTargetConf
*
* Sets the authorisation control mode for a specific target.
* Authorisation can be set to either TARGET_AUTH_MODE_MANUAL or TARGET_AUTH_MODE_AUTOMATIC. When
* set to TARGET_AUTH_MODE_MANUAL, the authorisation of RMAP commands is done manually by the
* user application. When set to TARGET_AUTH_MODE_AUTOMATIC, the authorisation of RMAP commands is
* done automatically using the expected parameter fields.
*
* @param deviceId       Device containing the target.
* @param target         Target to set authorisation mode for.
* @param mode           Authorisation mode.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_setAuthControlMode(STAR_DEVICE_ID deviceId, U32 target,
    TARGET_AUTH_MODE mode);

/**
* \ingroup rmapTargetConf
*
* Gets the authorisation control mode from a specific target.
* Authorisation can be set to either TARGET_AUTH_MODE_MANUAL or TARGET_AUTH_MODE_AUTOMATIC. When
* set to TARGET_AUTH_MODE_MANUAL, the authorisation of RMAP commands is done manually by the
* user application. When set to TARGET_AUTH_MODE_AUTOMATIC, the authorisation of RMAP commands is
* done automatically using the expected parameter fields.
*
* @param deviceId       Device containing the target.
* @param target         Target to get authorisation mode from.
* @param pMode          Pointer to a value to be updated with the target's authorisation mode.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getAuthControlMode(STAR_DEVICE_ID deviceId, U32 target,
    TARGET_AUTH_MODE *pMode);

/**
* \ingroup rmapTargetConf
*
* Sets the authorised target logical address range for a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to set authorised logical address range for.
* @param lowest         Lowest target logical address authorised.
* @param highest        Highest target logical address authorised.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_setAuthLogicalAddressRange(STAR_DEVICE_ID deviceId, U32 target,
    U8 lowest, U8 highest);

/**
* \ingroup rmapTargetConf
*
* Gets the authorised target logical address range from a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to get authorised logical address range from.
* @param pLowest        Pointer to a value to be updated with the lowest target logical
*                       address authorised.
* @param pHighest       Pointer to a value to be updated with the highest target logical
*                       address authorised.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getAuthLogicalAddressRange(STAR_DEVICE_ID deviceId, U32 target,
    U8 *pLowest, U8 *pHighest);

/**
* \ingroup rmapTargetConf
*
* Sets the authorised protocol ID for a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to set authorised protocol ID for.
* @param protocolId     Authorised protocol ID.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_setAuthProtocolId(STAR_DEVICE_ID deviceId, U32 target,
    U8 protocolId);

/**
* \ingroup rmapTargetConf
*
* Gets the authorised protocol ID from a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to get authorised protocol ID from.
* @param pProtocolId    Pointer to a value to be updated with the target's authorised protocol ID.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getAuthProtocolId(STAR_DEVICE_ID deviceId, U32 target,
    U8 *pProtocolId);

/**
* \ingroup rmapTargetConf
*
* Sets the authorised RMAP commands for a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to set authorised RMAP commands for.
* @param commands       Authorised commands mask.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_setAuthCommands(STAR_DEVICE_ID deviceId, U32 target,
    TARGET_AUTH_CMD_MASK commands);

/**
* \ingroup rmapTargetConf
*
* Gets the authorised RMAP commands from a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to get authorised RMAP commands from.
* @param pCommands      Pointer to a value to be updated with the target's authorised commands.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getAuthCommands(STAR_DEVICE_ID deviceId, U32 target,
    TARGET_AUTH_CMD_MASK *pCommands);

/**
* \ingroup rmapTargetConf
*
* Sets the authorised key range for a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to set authorised key range for.
* @param lowest         Lowest key authorised.
* @param highest        Highest key authorised.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_setAuthKeyRange(STAR_DEVICE_ID deviceId, U32 target,
    U8 lowest, U8 highest);

/**
* \ingroup rmapTargetConf
*
* Gets the authorised key range from a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to get authorised key range from.
* @param pLowest        Pointer to a value to be updated with the lowest key authorised.
* @param pHighest       Pointer to a value to be updated with the highest key authorised.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getAuthKeyRange(STAR_DEVICE_ID deviceId, U32 target,
    U8 *pLowest, U8 *pHighest);

/**
* \ingroup rmapTargetConf
*
* Sets the authorised memory address range for a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to set authorised memory address range for.
* @param lowerBoundary  Lowest memory address authorised.
* @param upperBoundary  Highest memory address authorised.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_setAuthMemoryAddressRange(STAR_DEVICE_ID deviceId, U32 target,
    U32 lowerBoundary, U32 upperBoundary);

/**
* \ingroup rmapTargetConf
*
* Gets the authorised memory address range from a specific target.
*
* @param deviceId       Device containing the target.
* @param target         Target to get authorised memory address range from.
* @param pLowerBoundary Pointer to a value to be updated with the lowest memory
                        address authorised.
* @param pUpperBoundary Pointer to a value to be updated with the highest memory
*                       address authorised.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getAuthMemoryAddressRange(STAR_DEVICE_ID deviceId, U32 target,
    U32 *pLowerBoundary, U32 *pUpperBoundary);

/**
* \ingroup rmapTargetConf
*
* Sets the RMAP interface mode for a specific port.
*
* @param deviceId       Device containing the port.
* @param port           Number of the port (6 - 9).
* @param mode           Interface mode of the port.
*
* @return      1 if the port's RMAP interface mode was successfully modified,
*              else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_setInterfaceMode(STAR_DEVICE_ID deviceId, U32 port,
    IF_MODE mode);

/**
* \ingroup rmapTargetConf
*
* Gets the RMAP interface mode for a specific port.
*
* @param deviceId       Device containing the port.
* @param port           Number of the port (6 - 9).
* @param pMode          Pointer to a value to be updated with the port's interface mode.
*
* @return      1 if the port's RMAP interface mode was successfully modified,
*              else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getInterfaceMode(STAR_DEVICE_ID deviceId, U32 port,
    IF_MODE *pMode);

/**
* \ingroup rmapTargetManualAuth
*
* Gets whether or not there is an RMAP command waiting to be authorised.
*
* @param deviceId       Device containing the target.
* @param target         Target to check whether authorisation is required.
*
* @return      1 if the target has a command waiting to be authorised,
*              else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_isAuthRequired(STAR_DEVICE_ID deviceId, U32 target);

/**
* \ingroup rmapTargetManualAuth
*
* Gets the parameters of the RMAP command waiting to be authorised.
*
* @param deviceId       Device containing the target.
* @param target         Target to get the waiting RMAP command parameters from.
* @param pCommand       Pointer to a value to be updated with the RMAP command parameters.
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getWaitingCommand(STAR_DEVICE_ID deviceId, U32 target,
    RmapCommandParameters *pCommand);

/**
* \ingroup rmapTargetManualAuth
*
* Authorises a waiting RMAP command.
*
* @param deviceId       Device containing the target.
* @param target         Target to authorise a waiting RMAP command for.
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_authoriseWaitingCommand(STAR_DEVICE_ID deviceId,
    U32 target);

/**
* \ingroup rmapTargetManualAuth
*
* Rejects a waiting RMAP command.
*
* @param deviceId       Device containing the target.
* @param target         Target to reject a waiting RMAP command for.
* @param reason         Reason for rejecting the waiting RMAP command.
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_rejectWaitingCommand(STAR_DEVICE_ID deviceId, U32 target,
    REJECTION_REASON reason);

/**
* \ingroup rmapTargetNotifications
*
* Set the notifications that are enabled for a target.
*
* @param deviceId       Device containing the target.
* @param target         Target to set enabled notifications for.
* @param notifications  Enabled notifications mask
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_setEnabledNotifications(STAR_DEVICE_ID deviceId, U32 target,
    NOTIF_TYPE_MASK notifications);

/**
* \ingroup rmapTargetNotifications
*
* Get the notifications that are enabled for a target.
*
* @param deviceId       Device containing the target.
* @param target         Target to get enabled notifications from.
* @param pNotifications Pointer to a value to be updated with the enabled notifications.
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_getEnabledNotifications(STAR_DEVICE_ID deviceId, U32 target,
    NOTIF_TYPE_MASK *pNotifications);

/**
* \ingroup rmapTargetNotifications
*
* Register a notification listener function.
*
* @param deviceId       Device containing the target.
* @param target         Target to register a notification listener function for.
* @param type           Type of notification to register the listener function for.
* @param listenerFunc   Pointer to a notification listener function.
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_registerNotificationListener(STAR_DEVICE_ID deviceId,
    U32 target, NOTIF_TYPE type, NotifListenerFunc listenerFunc);

/**
* \ingroup rmapTargetNotifications
*
* Register a notification listener with context function.
*
* @param deviceId       Device containing the target.
* @param target         Target to register a notification listener function for.
* @param type           Type of notification to register the listener function for.
* @param listenerFunc   Pointer to a notification listener with context function.
* @param pContext       Pointer to a user-supplied context.
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_4
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_registerNotificationListenerWithContext(
    STAR_DEVICE_ID deviceId, U32 target, NOTIF_TYPE type,
    NotifListenerWithContextFunc listenerFunc, void *pContext);

/**
* \ingroup rmapTargetNotifications
*
* Unregister a notification listener function.
*
* @param deviceId       Device containing the target.
* @param target         Target to unregister a notification listener with context function for.
* @param type           Type of notification to unregister the listener with context function for.
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_unregisterNotificationListener(STAR_DEVICE_ID deviceId,
    U32 target, NOTIF_TYPE type);

/**
* \ingroup rmapTargetNotifications
*
* Unregister a notification listener with context function.
*
* @param deviceId       Device containing the target.
* @param target         Target to unregister a notification listener with context function for.
* @param type           Type of notification to unregister the listener with context function for.
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_4
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_unregisterNotificationListenerWithContext(
    STAR_DEVICE_ID deviceId, U32 target, NOTIF_TYPE type);

/**
* \ingroup rmapTargetNotifications
*
* Start receiving notifications for a device.
*
* @param deviceId       Device to start receiving notifications for.
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_startReceivingNotifications(STAR_DEVICE_ID deviceId);

/**
* \ingroup rmapTargetNotifications
*
* Stop receiving notifications for a device.
*
* @param deviceId       Device to stop receiving notifications for.
*
* @return      1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_stopReceivingNotifications(STAR_DEVICE_ID deviceId);

/**
* \ingroup rmapTargetConf
*
* Reads an area of RMAP target memory into a user-supplied buffer.
*
* @param deviceId       Device containing the target.
* @param target         Target to read memory from.
* @param address        The address in the target to read from.
* @param length         The length to read.
* @param pBuffer        Pointer to a buffer to read memory into.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_readMemory(STAR_DEVICE_ID deviceId, U32 target,
    U32 address, U32 length, unsigned char *pBuffer);

/**
* \ingroup rmapTargetConf
*
* Writes an area of RMAP target memory from a user-supplied buffer.
*
* @param deviceId       Device containing the target.
* @param target         Target to write memory to.
* @param address        The address in the target to write to.
* @param length         The length to write.
* @param pBuffer        Pointer to a buffer to write memory from.
*
* @return       1 if the operation was successful, else 0.
*
* \versionAdded \ref changes_v3_0
*
* \devicesSupported \ref PXI (with RMAP Target)
*/
int STAR_API_CC RMAP_TARGET_PXI_IF_writeMemory(STAR_DEVICE_ID deviceId, U32 target,
    U32 address, U32 length, const unsigned char *pBuffer);

#ifdef __cplusplus
}
#endif

#endif // RMAP_TARGET_PXI_IF_H
