/**
 * \file cfg_api_brick_mk2_types.h
 *
 * \brief Types used with the STAR-Dundee Brick Mk2 Configuration API.
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

#ifndef CFG_API_BRICK_MK2_TYPES_H
#define CFG_API_BRICK_MK2_TYPES_H



#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup brickLinks
 *
 * Frequencies a link on a Brick Mk2 compatible device may run at. These are
 * divided by a divider set by CFG_MK2_setLinkRateDivider() to generate the
 * desired link speed.
 */
typedef enum
{
    /** 120MHz */
    STAR_CFG_BRICK_MK2_LINK_FREQ_120 = 0,

    /** 130MHz */
    STAR_CFG_BRICK_MK2_LINK_FREQ_130 = 1,

    /** 140MHz */
    STAR_CFG_BRICK_MK2_LINK_FREQ_140 = 2,

    /** 150MHz */
    STAR_CFG_BRICK_MK2_LINK_FREQ_150 = 3,

    /** 160MHz */
    STAR_CFG_BRICK_MK2_LINK_FREQ_160 = 4,

    /** 180MHz */
    STAR_CFG_BRICK_MK2_LINK_FREQ_180 = 5,

    /** 200MHz */
    STAR_CFG_BRICK_MK2_LINK_FREQ_200 = 6

} STAR_CFG_BRICK_MK2_LINK_FREQ;



/**
 * Structure used to indicate which errors should be generated when calling
 * CFG_BRICK_MK2_injectErrors().  If the element is set to a non-zero value,
 * an error of that type will be injected.
 */
typedef struct
{
    /** Whether a parity error should be injected. */
    char parityError;

    /** Whether an escape error should be injected. */
    char escapeError;

    /** Whether an extra FCT should be inserted to cause an error. */
    char insertFCT;

    /** Whether an FCT should be suppressed to cause an error. */
    char suppressFCT;

    /** Whether credit should be incremented to cause an error. */
    char incrementCredit;

    /** Whether credit should be decremented to cause an error. */
    char decrementCredit;

    /** Whether a disconnect should occur. */
    char Disconnect;

} STAR_CFG_BRICK_MK2_ERRORS;


/**
 * \ingroup brickLinkSpeed
 *
 * The units in Kbits/s used to represent the link speed returned when calling
 * CFG_BRICK_MK2_getMeasuredLinkSpeed().
 */
#define STAR_CFG_BRICK_MK2_LINK_SPEED_UNITS_KBPS    200


#ifdef __cplusplus
}
#endif



#endif  /* CFG_API_BRICK_MK2_TYPES_H */


