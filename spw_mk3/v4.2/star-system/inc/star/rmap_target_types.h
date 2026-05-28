/**
* \file rmap_target_types.h
*
* \brief Types used with the STAR-Dundee RMAP Target API.
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

#ifndef RMAP_TARGET_TYPES_H
#define RMAP_TARGET_TYPES_H

#include <star-api.h>

/**
* \ingroup rmapTargetConf
*
* Hardware capabilities.
*
* \versionAdded \ref changes_v3_0
*/
typedef struct TARGET_ENGINE
{
    U32 numTargets;
    U32 maxAddressOffset;
    U32 minPort;
    U32 maxPort;
} TARGET_ENGINE;

/**
* \ingroup rmapTargetConf
*
* Method of authorising incoming RMAP commands.
*
* \versionAdded \ref changes_v3_0
*/
typedef enum
{
    /** When set to manual, each incoming command must be authorised or rejected by software */
    TARGET_AUTH_MODE_MANUAL                 = 0,

    /** When set to automatic, each incoming command is automatically authorised or rejected
        based on the authorised parameter values */
    TARGET_AUTH_MODE_AUTOMATIC              = 1
} TARGET_AUTH_MODE;

/**
* \ingroup rmapTargetConf
*
* Status of the RMAP target
*
* \versionAdded \ref changes_v3_0
*/
typedef enum
{
    TARGET_STATUS_SUCCESS                   = 0,
    TARGET_STATUS_GENERAL_ERROR             = 1,
    TARGET_STATUS_HEADER_EOP_ERROR          = 2,
    TARGET_STATUS_HEADER_EEP_ERROR          = 3,
    TARGET_STATUS_PID_ERROR                 = 4,
    TARGET_STATUS_REPLY_ERROR               = 5,
    TARGET_STATUS_HEADER_CRC_ERROR          = 6,
    TARGET_STATUS_HEADER_EEP_AFTER_CRC      = 7,
    TARGET_STATUS_PACKET_TYPE_ERROR         = 8,
    TARGET_STATUS_COMMAND_TYPE_ERROR        = 9,
    TARGET_STATUS_RMW_DATALEN_ERROR         = 10,
    TARGET_STATUS_CARGO_TOO_LARGE           = 11,
    TARGET_STATUS_KEY_ERROR                 = 12,
    TARGET_STATUS_LOGICAL_ADDR_ERROR        = 13,
    TARGET_STATUS_AUTHORISED_ERROR          = 14,
    TARGET_STATUS_VERIFY_BUFFER_OVERRUN     = 15,
    TARGET_STATUS_DATA_CRC_ERROR            = 16,
    TARGET_STATUS_BUS_ERROR                 = 17,
    TARGET_STATUS_DATA_EOP_ERROR            = 18,
    TARGET_STATUS_DATA_EEP_ERROR            = 19,
    TARGET_STATUS_DATA_EEP_AFTER_CRC        = 20
} TARGET_STATUS;

/**
* \ingroup rmapTargetConf
*
* Command types which are authorised by the target.
*
* \versionAdded \ref changes_v3_0
*/
typedef enum
{
    TARGET_AUTH_CMD_READ                    = (1 << 0),
    TARGET_AUTH_CMD_READ_INCR               = (1 << 1),
    TARGET_AUTH_CMD_READ_MODIFY_WRITE       = (1 << 2),
    TARGET_AUTH_CMD_WRITE                   = (1 << 3),
    TARGET_AUTH_CMD_WRITE_INCR              = (1 << 4),
    TARGET_AUTH_CMD_WRITE_REPLY             = (1 << 5),
    TARGET_AUTH_CMD_WRITE_INCR_REPLY        = (1 << 6),
    TARGET_AUTH_CMD_WRITE_VERIFY            = (1 << 7),
    TARGET_AUTH_CMD_WRITE_VERIFY_INCR       = (1 << 8),
    TARGET_AUTH_CMD_WRITE_VERIFY_REPLY      = (1 << 9),
    TARGET_AUTH_CMD_WRITE_VERIFY_INCR_REPLY = (1 << 10),
    TARGET_AUTH_CMD_ALL                     = (0x7FF),
    TARGET_AUTH_CMD_NONE                    = (0)
} TARGET_AUTH_CMD;

/**
* \ingroup rmapTargetConf
*
* Mask describing which commands are authorised.
*
* \versionAdded \ref changes_v3_0
*/
typedef U32 TARGET_AUTH_CMD_MASK;

/**
* \ingroup rmapTargetManualAuth
*
* Parameters of an RMAP command.
*
* \versionAdded \ref changes_v3_0
*/
typedef struct RmapCommandParameters
{
    U8 targetLogicalAddress;
    U8 command;
    U8 key;
    U8 protocolId;
    U8 extendedAddress;
    U32 dataLength;
    U32 address;
    U8 initiatorLogicalAddress;
    U16 transactionId;
} RmapCommandParameters;

/**
* \ingroup rmapTargetManualAuth
*
* Reason for rejecting a waiting RMAP command.
*
* \versionAdded \ref changes_v3_0
*/
typedef enum
{
    REJECTION_REASON_LOGICAL_ADDRESS,
    REJECTION_REASON_KEY,
    REJECTION_REASON_PROTOCOL_ID,
    REJECTION_REASON_OTHER
} REJECTION_REASON;

/**
* \ingroup rmapTargetConf
*
* Control whether or not the RMAP target is enabled for a port.
*
* \versionAdded \ref changes_v3_0
*/
typedef enum
{
    /** When RMAP is disabled, the port acts as a normal SpaceWire interface */
    IF_MODE_RMAP_DISABLED           = 0,

    /** When RMAP is enabled, the port acts as an RMAP target */
    IF_MODE_RMAP_ENABLED            = 1
} IF_MODE;

/**
* \ingroup rmapTargetNotifications
*
* Types of notifications.
*
* \versionAdded \ref changes_v3_0
*/
typedef enum
{
    /** Authorisation request notification */
    NOTIF_TYPE_AUTH_REQUEST         = (1 << 0),

    /** Command complete notification */
    NOTIF_TYPE_CMD_COMPLETE         = (1 << 2),

    /** All notifications */
    NOTIF_TYPE_ALL                  = (0x5),

    /** No notifications */
    NOTIF_TYPE_NONE                 = (0)
} NOTIF_TYPE;

/**
* \ingroup rmapTargetNotifications
*
* Mask describing which notifications are enabled
*
* \versionAdded \ref changes_v3_0
*/
typedef U32 NOTIF_TYPE_MASK;

/**
* \ingroup rmapTargetNotifications
*
* Notification listener function pointer:
*     target is the index of the target which generated the notification.
*     pNotif is a pointer to the notification packet which has one of two formats
*     depending on the notification type.
*
* Authorisation request notification:\n
*     Byte [0]          : 0xFE\n
*     Byte [1]          : 0xF0\n
*     Byte [2]          : 0x10 (Notification type)\n
*     Byte [3]          : Notification packet cargo length\n
*     Byte [4]          : Target index\n
*     Byte [5]          : Current time-code at time of event
*
* Command complete notification:\n
*     Byte [0]          : 0xFE\n
*     Byte [1]          : 0xF0\n
*     Byte [2]          : 0x12 (Notification type)\n
*     Byte [3]          : Notification packet cargo length\n
*     Byte [4]          : Target index\n
*     Byte [5]          : Current time-code at time of event\n
*     Byte [6]          : Target logical address\n
*     Byte [7]          : Protocol ID\n
*     Byte [8]          : Command\n
*     Byte [9]          : Key\n
*     Byte [10]         : Initiator logical address\n
*     Byte [11 - 12]    : Transaction ID (MSB first)\n
*     Byte [13]         : Extended address\n
*     Byte [14 - 17]    : Address (MSB first)\n
*     Byte [18 - 20]    : Data length (MSB first)\n
*     Byte [21]         : Status
*
* \versionAdded \ref changes_v3_0
*/
typedef void(STAR_API_CC *NotifListenerFunc)(U32 target, void *pNotif);

/**
* \ingroup rmapTargetNotifications
*
* Notification listener with context function pointer:
*     deviceId is the ID of the device that issued the notification
*     target is the index of the target which generated the notification.
*     pNotif is a pointer to the notification packet which has one of two formats
*     depending on the notification type.
*     pContext is a pointer to a user supplied context variable
*
* Authorisation request notification:\n
*     Byte [0]          : 0xFE\n
*     Byte [1]          : 0xF0\n
*     Byte [2]          : 0x10 (Notification type)\n
*     Byte [3]          : Notification packet cargo length\n
*     Byte [4]          : Target index\n
*     Byte [5]          : Current time-code at time of event
*
* Command complete notification:\n
*     Byte [0]          : 0xFE\n
*     Byte [1]          : 0xF0\n
*     Byte [2]          : 0x12 (Notification type)\n
*     Byte [3]          : Notification packet cargo length\n
*     Byte [4]          : Target index\n
*     Byte [5]          : Current time-code at time of event\n
*     Byte [6]          : Target logical address\n
*     Byte [7]          : Protocol ID\n
*     Byte [8]          : Command\n
*     Byte [9]          : Key\n
*     Byte [10]         : Initiator logical address\n
*     Byte [11 - 12]    : Transaction ID (MSB first)\n
*     Byte [13]         : Extended address\n
*     Byte [14 - 17]    : Address (MSB first)\n
*     Byte [18 - 20]    : Data length (MSB first)\n
*     Byte [21]         : Status
*
* \versionAdded \ref changes_v3_0
*/
typedef void(STAR_API_CC *NotifListenerWithContextFunc)(STAR_DEVICE_ID deviceId, U32 target,
    void *pNotif, void *pContext);

#endif // RMAP_TARGET_TYPES_H
