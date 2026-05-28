/**
 * \file cfg_api_mk2_types.h
 *
 * \brief Types used with the STAR-Dundee Mk2 compatible device configuration API.
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
#ifndef CFG_API_MK2_TYPES_H
#define CFG_API_MK2_TYPES_H


/**
 * \ingroup hardwareMk2
 *
 * Hardware version, and synthesis date of the FPGA code.
 *
 * \versionAdded \ref changes_v1_2
 */
typedef struct
{
    /** Major version number. */
    U8 major;

    /** Minor version number. */
    U8 minor;

    /** Version number edit. */
    U8 edit;

    /** Version number patch. */
    U8 patch;

    /** Year value of time and date. */
    U16 year;

    /** Month value of time and date. */
    U8  month;

    /** Day value of time and date. */
    U8  day;

    /** Hour value of time and date. */
    U8  hour;

    /** Minute value of time and date. */
    U8  minute;

}STAR_CFG_MK2_HARDWARE_INFO;

/**
 * \ingroup linksMk2
 *
 * Base transmit clock rate control properties.
 *
 * \versionAdded \ref changes_v1_2
 */
typedef struct
{
    /** Multiplier value. Valid range 2:256 inclusive. */
    U16 multiplier;

    /** Divisor value. Valid range 1:256 inclusive.*/
    U16 divisor;

}STAR_CFG_MK2_BASE_TRANSMIT_CLOCK;

/**
 * \ingroup errInjectMk2
 *
 * Errors that can be injected onto the SpaceWire link.
 *
 * \versionAdded \ref changes_v2_0
 */
typedef enum {
    /** Causes the SpaceWire link to disconnect. */
    SPW_ERROR_DISCONNECT   = 1,

    /** Inserts a parity error on the next SpaceWire character to be
      * transmitted by flipping the parity bit. */
    SPW_ERROR_PARITY       = 2,

    /** Transmits an ESCAPE_ESCAPE code.*/
    SPW_ERROR_ESCAPE       = 3,

    /** Inserts an FCT character on the next character to be sent. This can be
      * used to force FCT errors on the SpaceWire link. */
    SPW_ERROR_INSERT_FCT   = 4,

    /** Ignore the next FCT character received on the link. This can be used to
      * simulate the loss of an FCT character. */
    SPW_ERROR_SUPPRESS_FCT = 5,

    /** Increments the transmit credit available to send data by 8. This can be
      * used to force a data overflow in the opposite end of the link, as too
      * much data will be transmitted. */
    SPW_ERROR_INCREMENT_CREDIT = 6,

    /** Decrements the transmit credit count by 1. */
    SPW_ERROR_DECREMENT_CREDIT = 7
} SPW_ERROR;

/**
 * \ingroup periodicAction
 *
 * Actions which can be preformed at specified periodic intervals.
 *
 * \versionAdded \ref changes_v3_0_beta3
 */
typedef enum {
    /** Transmits a SpaceWire packet.*/
    SPW_TRANSMIT_PACKET       = 0
} SPW_ACTION;

/**
 * \ingroup linkSpeedMk2
 *
 * The units in Kbit/s used to represent the link speed returned when calling
 * CFG_MK2_getMeasuredLinkSpeed().
 *
 * \versionAdded \ref changes_v3_0_beta8
 */
#define STAR_CFG_MK2_LINK_SPEED_UNITS_KBPS    100

#endif /*CFG_API_MK2_TYPES_H */
