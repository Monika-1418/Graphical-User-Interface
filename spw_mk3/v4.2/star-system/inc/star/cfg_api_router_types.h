/**
 * \file  cfg_api_router_types.h
 *
 * \brief Types used with the STAR-Dundee Router API.
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

#ifndef CFG_API_ROUTER_TYPES_H
#define CFG_API_ROUTER_TYPES_H


#include "star-api.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup portStatusControl
 *
 * The type used to represent a port status and control register value.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef REGISTER PORT_STATUS_CONTROL;

/**
 * \ingroup deviceIdentifier
 *
 * Maximum length for the string representation of a manufacturer name.
 *
 * \versionAdded \ref changes_v0_8
 */
#define STAR_CFG_MANUFACTURER_STR_MAX_LEN 256

/**
 * \ingroup deviceIdentifier
 *
 * Maximum length for the string representation of a device type.
 *
 * \versionAdded \ref changes_v0_8
 */
#define STAR_CFG_DEVICE_STR_MAX_LEN 256

/**
 * \ingroup deviceIdentifier
 *
 * Information obtained from a Device's Identifier Register,
 * identifying the router's version and type.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
    /** Manufacturer ID number (STAR-Dundee: 1) */
    U16 manufacturerID;
    /** Identity code for the SpaceWire chip from the particular manufacturer */
    U8 chipType;
    /** Device version number */
    U8 versionNum;
}
STAR_CFG_DEVICE_IDENTIFIER_INFO;


/**
 * \ingroup deviceIdentifier
 *
 * Device types permitted within the network discovery register.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /** Router device */
    STAR_CFG_DEVICE_TYPE_ROUTER,
    /** Unknown device type*/
    STAR_CFG_DEVICE_TYPE_UNKNOWN,
    /** Invalid device type*/
    STAR_CFG_DEVICE_TYPE_INVALID
} STAR_CFG_DEVICE_TYPE;

/**
 * \ingroup deviceIdentifier
 *
 * Information obtained from a device's network discovery register.
 * Information that can be used to determine the network layout.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
    /** Type of device*/
    STAR_CFG_DEVICE_TYPE deviceType;

    /**
     * Indicates the input port number which was used to access the network
     * discovery register.
     */
    U8 returnPort;

    /**
     * Bitmask of ports which are in the run state.
     * \note Bit 1 corresponds to port 1.
     */
    U32 runningPortsMask;

    /** Count of running ports.*/
    U8 runningPortsCount;

    /** Count of the number of ports the device has.*/
    U8 portCount;

}
STAR_CFG_NETWORK_DISCOVERY_INFO;



/**
 * \ingroup portStatusControl
 *
 * Port types for Routing devices.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /** Configuration port */
    STAR_CFG_PORT_TYPE_CONFIGURATION,
    /** SpaceWire Link port */
    STAR_CFG_PORT_TYPE_LINK,
    /** External port*/
    STAR_CFG_PORT_TYPE_EXTERNAL,
    /** Invalid value. This should never occur*/
    STAR_CFG_PORT_TYPE_INVALID
}STAR_CFG_PORT_TYPE;


/**
 * \ingroup portStatusControl
 *
 * Errors that may be present on a device's configuration port.
 * If a member of this structure is set, then the error it represents
 * is present.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
     /** Count of errors present on the configuration port. */
    char errorCount;

    /**
     * The port timeout error bit is set when a timeout event is detected by the
     * configuration port routing logic.
     */
    char portTimeoutError;

    /** The Invalid header CRC bit is set when the header CRC is invalid. */
    char invalidHeaderCRC;

    /**
     * The invalid data CRC is set when the data field of the packet is
     * corrupted and the CRC does not match the internally generated CRC.
     */
    char invalidDataCRC;

    /**
     * The invalid destination key bit is set when the destination key in the
     * command packet is invalid.
     */
    char invalidDestinationKey;

    /**
     * The command not implemented bit is set when the command code is a valid
     * RMAP code but the command is not supported by the SpaceWire Router.
     */
    char commandNotImplemented;

    /**
     * The invalid data length bit is set when a data length error is detected.
     */
    char invalidDataLength;

    /**
     * The read modify write command data length is invalid. The expected length
     * is 8.
     */
    char invalidRMWDataLength;

    /**
     * The invalid destination logical address bit is set when the destination
     * logical address in the command packet is not the default value of 254.
     */
    char invalidDestinationLogicalAddress;

    /**
     * The early EOP bit is set when the command packet is terminated before the
     * end of packet with an EOP.
     */
    char earlyEOP;

    /**
     * The late EOP bit is set when the command packet is not terminated
     * correctly and trailing bytes are detected before the end of packet.
     */
    char lateEOP;

    /**
     * The early EEP bit is set when the command packet is terminated before the
     * end of packet with an EEP.
     */
    char earlyEEP;

    /**
     * The late EEP bit is set when the command packet is not terminated
     * correctly and trailing bytes are detected before the end of packet.
     */
    char lateEEP;

    /**
     * The verify buffer overrun error bit is set when a verified write command
     * is performed and the data length is not 4.
     */
    char verifyBufferOverrun;

    /**
     * The invalid register address bit is set when an unknown register address
     * is given in the command packet or a write is attempted to a read only
     * register.
     */
    char invalidRegisterAddress;

    /**
     * The unsupported protocol error bit is set when a command packet is
     * received with a protocol identifier which is not the RMAP protocol
     * identifier (0x01).
     */
    char unsupportedProtocol;

    /**
     * The source logical address error bit is set when an invalid source
     * logical address is received.
     */
    char sourceLogicalAddressError;

    /**
     * This bit is set when an invalid source address path is received.
     * \note This error is not used for the 10X.
     */
    char sourcePathAddressError;

    /** The RMAP command packet is too large. */
    char cargoTooLarge;

    /**
     * The command code is an unused command code or the packet type is invalid.
     */
    char unusedRMAPCommandOrPacketType;

}STAR_CFG_CONFIG_PORT_ERRORS;


/**
 * \ingroup portStatusControl
 *
 * Errors that may be present on a SpaceWire Link.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
     /** Count of errors present on the link port*/
    char errorCount;


    /**
     * Packet received with an invalid address, either due to an unknown port,
     * or an invalid logical address.
     */
    char packetAddress;

    /**
     * The port has become blocked for a period of time. A packet could not be
     * routed to a destination port before the port timeout occurred.
     */
    char portTimeout;

    /**
     * A disconnect error occurred on the link. No activity occurred on the link
     * for the disconnect timeout period.
     */
    char disconnect;

    /** A parity was detected on a character parity bit. */
    char parity;

    /**
     * An invalid escape code was received on the link. (ESC-ESC, ESC-EOP, or
     * ESC-EEP).
     */
    char escape;

    /** A credit error occurred on the link. */
    char credit;

    /**
     * A character sequence error occurred on the link. A time-code or data
     * character was received before the first FCT.
     */
    char characterSequence;


}STAR_CFG_SPW_LINK_ERRORS;

/**
 * \ingroup portStatusControl
 *
 * The state of the interface state machine in the SpaceWire link.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /** Error Reset */
    STAR_CFG_SPW_LINK_STATE_ERROR_RESET,

    /** Error Wait */
    STAR_CFG_SPW_LINK_STATE_ERROR_WAIT,

    /** Ready */
    STAR_CFG_SPW_LINK_STATE_READY,

    /** Started */
    STAR_CFG_SPW_LINK_STATE_STARTED,

    /** Connecting */
    STAR_CFG_SPW_LINK_STATE_CONNECTING,

    /** Run */
    STAR_CFG_SPW_LINK_STATE_RUN,

     /** Invalid value */
    STAR_CFG_SPW_LINK_STATE_INVALID

}STAR_CFG_SPW_LINK_STATE;

/**
 * \ingroup portStatusControl
 *
 * Status of a SpaceWire link.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
    /** When set, the SpaceWire link LVDS drivers are in tri-state mode. */
    char triState;

    /**
     * When set then the SpaceWire link will be disabled as defined in the
     * SpaceWire standard. The SpaceWire port will not start  and will not
     * respond to any attempt to make a connection by the other end of the link.
     */
    char disable;

    /**
     * When set then the SpaceWire link will initiate start-up as defined in the
     * SpaceWire standard. The SpaceWire port will try to make a connection with
     * the other end of the link.
     */
    char start;

    /**
     * When set the SpaceWire link will auto-start as defined in the SpaceWire
     * standard. The SpaceWire port will wait until the other end of the link
     * tries to make a connection (sending NULLs) and will then automatically
     * start.
     */
    char autoStart;

    /** Set when the SpaceWire interface state machine is in the Run state. */
    char running;

    /**  State of the interface state machine. */
    STAR_CFG_SPW_LINK_STATE linkState;

}STAR_CFG_SPW_LINK_STATUS;


/**
 * \ingroup portStatusControl
 *
 * Errors that may be present on an external port.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
     /** Count of errors present on the external port. */
    char errorCount;

    /**
     * Packet received with an invalid address, either due to an unknown port,
     * or an invalid logical address. This is also generated when an empty
     * packet is passed to the external port.
     */
    char packetAddress;

    /**
     * The port has become blocked for a period of time. A packet could not be
     * routed to a destination port before the port timeout occurred.
     */
    char portTimeout;

}STAR_CFG_EXTERNAL_PORT_ERRORS;

/**
 * \ingroup portStatusControl
 *
 * Status of an external port.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
    /**
     * The external port input buffer is empty.
     * \note The input buffer writes data to the SpaceWire router.
     */
    char inputBufferEmpty;

    /** The external port input buffer is full. */
    char inputBufferFull;

    /**
     * The external output port buffer is empty.
     * \note The output buffer writes data to the external device connected to
     * the external port.
     */
    char outputBufferEmpty;

    /** The external output port buffer is full. */
    char outputBufferFull;

}STAR_CFG_EXTERNAL_PORT_STATUS;


/**
 * \ingroup gar
 *
 * A Routing table entry.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
    /**
     * Bitmask of output ports the logical address will arbitrate for. Valid
     * bits set are 1 through 28.
     * \note Bit 1 corresponds to port 1
     * \note It is not possible to access the configuration port (0) through
     * logical addresses.
     */
    U32 portMask;

    /**
     * Packets with the logical address for this entry with the priority bit set
     * will be granted access to a particular output port in preference to
     * packets whose logical addresses in the routing table have their priority
     * bit set to zero.
     */
    char priority;

    /**
     * When set the leading header byte of the input packet will be removed
     * before it is transferred to the output port.
     */
    char deleteHeader;

    /**
     * When set this indicates that the corresponding logical address is
     * invalid. In this case, any packets arriving at the router with an invalid
     * address are spilt and an address error is reported in the port status
     * register.
     */
    char invalidAddress;

}
STAR_CFG_GAR_ENTRY;

/**
 * \ingroup configuration
 *
 * Length of port timeout.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /** 60-80us */
    STAR_CFG_PORT_TIMEOUT_100US     = 0x0,

    /** ~1.3ms */
    STAR_CFG_PORT_TIMEOUT_1MS       = 0x1,

    /** ~10ms */
    STAR_CFG_PORT_TIMEOUT_10MS      = 0x2,

    /** ~82ms */
    STAR_CFG_PORT_TIMEOUT_100MS     = 0x3,

    /** ~1.3s */
    STAR_CFG_PORT_TIMEOUT_1S        = 0x4

} STAR_CFG_PORT_TIMEOUT;


/**
 * \ingroup configuration
 *
 * Timeout mode used by the router. This affects how blocked packets will be
 * handled.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /**
     * Blocking allowed.
     * When blocking mode is enabled packets will wait forever to be routed
     * unless the packet is routed to a port that is not started. In this case
     * the packet will be discarded.
     */
    STAR_CFG_TIMEOUT_MODE_BLOCKING,

    /**
     * Watchdog timer mode.
     * When watchdog mode is enabled packets which are waiting to be routed at
     * source ports will be discarded after the timeout period. Packet tails
     * will also be discarded if the packet becomes blocked for the timeout
     * period. In this case the packet will be ended with an EEP.
     */
    STAR_CFG_TIMEOUT_MODE_WATCHDOG

}STAR_CFG_TIMEOUT_MODE;


/**
 * \ingroup configuration
 *
 * Global router settings.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
    /** Timeout mode */
    STAR_CFG_TIMEOUT_MODE timeoutMode;

    /** Timeout period */
    STAR_CFG_PORT_TIMEOUT timeoutPeriod;

    /**
     * If true, links will be disabled after the timeout period when data
     * transfer completes. The link will only be disabled if the link was
     * started automatically.
     */
    char disableOnSilence;

    /**
     * If true, links will be automatically started when they have data to
     * transfer. If the link cannot be started packets are discarded after the
     * timeout period.
     */
    char startOnRequest;

    /**
     * If true, a packet can be routed out of the port on which it arrived on.
     * This is for debugging purposes. If this is false and a packet is to be
     * routed through the same port an address error is reported and the packet
     * is discarded.
     * If false, and a group adaptive routing packet is received (a packet which
     * can be routed through two or more ports, dependent on the group adaptive
     * routing table contents) which can be routed through the port it arrived
     * on then the packet is routed through one of the other ports and not the
     * port on which the packet arrived on.  An address error is not reported.
     */
    char enableSelfAddressing;


}STAR_CFG_ROUTER_GLOBAL_STATE;





#ifdef __cplusplus
}
#endif



#endif  /* CFG_API_ROUTER_TYPES_H */


