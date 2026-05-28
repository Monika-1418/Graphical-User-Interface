/**
* \file triggering_types.h
*
* \brief Types used with the STAR-Dundee Triggering API
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

#ifndef TRIGGERING_TYPES_H
#define TRIGGERING_TYPES_H

#include <star-api.h>

/**
 * Hardware capabilities
 *
 * \versionAdded \ref changes_v3_0
 */
typedef struct TRIGGER_MATRIX
{
    U32 extTriggers;
    U32 counters;
    U32 ports;
    U32 timeCodes;
    U32 triggers;
} TRIGGER_MATRIX;

/**
 * \ingroup triggeringEvents
 *
 * External trigger events
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    /** Event occurs when the external trigger has received input */
    EXT_TRIGGER_EVENT_IN        = 1 << 0,

    /** No events */
    EXT_TRIGGER_EVENT_NONE      = 0,

    /** All events */
    EXT_TRIGGER_EVENT_ALL       = 0x1

} EXT_TRIGGER_EVENT;

/**
 * \ingroup triggeringEvents
 *
 * External trigger event mask
 *
 * \versionAdded \ref changes_v3_0
 */
typedef U32 EXT_TRIGGER_EVENT_MASK;

/**
 * \ingroup triggeringActions
 *
 * External trigger actions
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    /** Output the external trigger */
    EXT_TRIGGER_ACTION_OUT      = 1 << 0,

    /** No actions */
    EXT_TRIGGER_ACTION_NONE     = 0,

    /** All actions */
    EXT_TRIGGER_ACTION_ALL      = 0x1

} EXT_TRIGGER_ACTION;

/**
 * \ingroup triggeringEvents
 *
 * External trigger action mask
 *
 * \versionAdded \ref changes_v3_0
 */
typedef U32 EXT_TRIGGER_ACTION_MASK;

/**
 * \ingroup triggeringEvents
 *
 * Counter events
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    /** Event occurs when the counter decrements */
    COUNTER_EVENT_COUNT           = 1 << 0,

    /** Event occurs when the counter reaches zero (once) */
    COUNTER_EVENT_ZERO_SINGLE     = 1 << 1,

    /** Event is active while the counter is equal to zero */
    COUNTER_EVENT_ZERO            = 1 << 2,

    /** Event occurs when the counter reloads */
    COUNTER_EVENT_RELOAD          = 1 << 3,

    /** No events */
    COUNTER_EVENT_NONE            = 0,

    /** All events */
    COUNTER_EVENT_ALL             = 0xf
} COUNTER_EVENT;

/**
 * \ingroup triggeringEvents
 *
 * Counter event mask
 *
 * \versionAdded \ref changes_v3_0
 */
typedef U32 COUNTER_EVENT_MASK;

/**
 * \ingroup triggeringActions
 *
 * Counter actions
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    /** Decrement the counter */
    COUNTER_ACTION_COUNT          = 1 << 0,

    /** Reload the counter */
    COUNTER_ACTION_RELOAD         = 1 << 1,

    /** Start the counter */
    COUNTER_ACTION_START          = 1 << 2,

    /** Stop the counter */
    COUNTER_ACTION_STOP           = 1 << 3,

    /** No actions */
    COUNTER_ACTION_NONE           = 0,

    /** All actions */
    COUNTER_ACTION_ALL            = 0xf
} COUNTER_ACTION;

/**
 * \ingroup triggeringEvents
 *
 * Counter action mask
 *
 * \versionAdded \ref changes_v3_0
 */
typedef U32 COUNTER_ACTION_MASK;

/**
 * \ingroup triggeringEvents
 *
 * Port events
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    /** Event occurs when the port receives an SOP */
    PORT_EVENT_RX_SOP           = 1 << 0,

    /** Event occurs when the port receives an EOP */
    PORT_EVENT_RX_EOP           = 1 << 1,

    /** Event occurs when the port receives an EEP */
    PORT_EVENT_RX_EEP           = 1 << 2,

    /** Event occurs when the port transmits an SOP */
    PORT_EVENT_TX_SOP           = 1 << 3,

    /** Event occurs when the port transmits an EOP */
    PORT_EVENT_TX_EOP           = 1 << 4,

    /** Event is active when the port has a packet queued for transmission */
    PORT_EVENT_TX_PKT_PENDING   = 1 << 5,

    /** Event is active when the link attached to the port is running */
    PORT_EVENT_RUNNING          = 1 << 6,

    /** Event occurs when the port detects a parity error */
    PORT_EVENT_PARITY_ERROR     = 1 << 7,

    /** Event occurs when the port detects an escape error */
    PORT_EVENT_ESCAPE_ERROR     = 1 << 8,

    /** Event occurs when the port detects a credit error */
    PORT_EVENT_CREDIT_ERROR     = 1 << 9,

    /** Event occurs when the port disconnects */
    PORT_EVENT_DISCONNECT       = 1 << 10,

    /** Event occurs when the port receives a time-code */
    PORT_EVENT_RX_TIME_CODE     = 1 << 11,

    /** Event occurs when the port transmits a time-code */
    PORT_EVENT_TX_TIME_CODE     = 1 << 12,

    /** No events */
    PORT_EVENT_NONE             = 0,

    /** All events */
    PORT_EVENT_ALL              = 0x1fff,
    PORT_EVENT_ALL_PCIE         = 0xfff
} PORT_EVENT;

/**
 * \ingroup triggeringEvents
 *
 * Port event mask
 *
 * \versionAdded \ref changes_v3_0
 */
typedef U32 PORT_EVENT_MASK;

/**
 * \ingroup triggeringActions
 *
 * Port actions
 *
 * \versionAdded \ref changes_v3_0
 * \versionChanged \ref changes_v4_00_beta4
 */
typedef enum
{
    /** Transmit a pending packet */
    PORT_ACTION_TRANSMIT_PKT    = 1 << 0,

    /** Disconnect the port */
    PORT_ACTION_DISCONNECT      = 1 << 1,

    /** Inject a parity error */
    PORT_ACTION_PARITY_ERROR    = 1 << 2,

    /** Inject an escape error */
    PORT_ACTION_ESCAPE_ERROR    = 1 << 3,

    /** Insert an FCT */
    PORT_ACTION_INSERT_FCT      = 1 << 4,

    /** Suppress an FCT */
    PORT_ACTION_SUPPRESS_FCT    = 1 << 5,

    /** Increment credit */
    PORT_ACTION_INCR_CREDIT     = 1 << 6,

    /** Decrement credit */
    PORT_ACTION_DECR_CREDIT     = 1 << 7,

    /**
     * Stop reception
     *
     * \versionAdded \ref changes_v3_7
     *
     * \devicesSupported \ref BrickMk3 (v1.03 or later).
     */
    PORT_ACTION_STOP_RECEPTION  = 1 << 8,

    /**
     * Disable LVDS drivers
     *
     * \versionAdded \ref changes_v4_00_beta4
     *
     * \devicesSupported \ref BrickMk3 (v1.05 or later),
     *          \ref PXI (v1.05 or later),
     *          \ref PXIRouter (v1.05 or later).
     */
    PORT_ACTION_DISABLE_LVDS    = 1 << 9,

    /** No actions */
    PORT_ACTION_NONE            = 0,

    /** All actions */
    PORT_ACTION_ALL             = 0x3ff,
    PORT_ACTION_ALL_PCIE        = 0x1ff
} PORT_ACTION;

/**
 * \ingroup triggeringEvents
 *
 * Port action mask
 *
 * \versionAdded \ref changes_v3_0
 */
typedef U32 PORT_ACTION_MASK;

/**
 * \ingroup triggeringEvents
 *
 * Time-code events
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    /** Event occurs when the next valid time-code is received */
    TIME_CODE_EVENT_TICK        = 1 << 0,

    /** No events */
    TIME_CODE_EVENT_NONE        = 0,

    /** All events */
    TIME_CODE_EVENT_ALL         = 0x1

} TIME_CODE_EVENT;

/**
 * \ingroup triggeringEvents
 *
 * Time-code event mask
 *
 * \versionAdded \ref changes_v3_0
 */
typedef U32 TIME_CODE_EVENT_MASK;

/**
 * \ingroup triggeringActions
 *
 * Time-code actions
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    /** Transmit a time-code */
    TIME_CODE_ACTION_TX         = 1 << 0,

    /** No actions */
    TIME_CODE_ACTION_NONE       = 0,

    /** All actions */
    TIME_CODE_ACTION_ALL        = 0x1

} TIME_CODE_ACTION;

/**
 * \ingroup triggeringEvents
 *
 * Time-code action mask
 *
 * \versionAdded \ref changes_v3_0
 */
typedef U32 TIME_CODE_ACTION_MASK;

/**
 * \ingroup triggeringEvents
 *
 * Internal trigger events
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    /** Event occurs when the internal trigger has received input */
    TRIGGER_EVENT_IN            = 1 << 0,

    /** No events */
    TRIGGER_EVENT_NONE          = 0,

    /** All events */
    TRIGGER_EVENT_ALL           = 0x1

} TRIGGER_EVENT;

/**
 * \ingroup triggeringEvents
 *
 * Trigger event mask
 *
 * \versionAdded \ref changes_v3_0
 */
typedef U32 TRIGGER_EVENT_MASK;

/**
 * \ingroup triggeringConf
 *
 * Internal trigger input modes
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    /** Trigger is a sum of its inputs */
    TRIGGER_INPUT_MODE_OR       = 0,

    /** Trigger is a product of its inputs */
    TRIGGER_INPUT_MODE_AND      = 1

} TRIGGER_INPUT_MODE;

/**
 * Trigger types
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    TRIGGER_TYPE_EXT_TRIGGER    = 0,
    TRIGGER_TYPE_COUNTER        = 1,
    TRIGGER_TYPE_PORT           = 2,
    TRIGGER_TYPE_TIME_CODE      = 3,
    TRIGGER_TYPE_TRIGGER        = 4,
} TRIGGER_TYPE;

/**
 * Trigger devices
 *
 * \versionAdded \ref changes_v3_0
 */
typedef enum
{
    TRIGGER_DEVICE_INVALID          = 0,
    TRIGGER_DEVICE_BRICK_MK3        = 1,
    TRIGGER_DEVICE_PXI_IF           = 2,
    TRIGGER_DEVICE_PXI_ROUTER       = 3,
    TRIGGER_DEVICE_PCIE_IF          = 4,
} TRIGGER_DEVICE;



#endif // TRIGGERING_TYPES_H
