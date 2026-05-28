/**
 * \file cfg_api_generic_common.h
 *
 * \brief Additional types used with the STAR-Dundee Generic Configuration API
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

#ifndef CFG_API_GENERIC_COMMON_H
#define CFG_API_GENERIC_COMMON_H

#include "star-api.h"
#include "cfg_api_remote.h"
#include "cfg_api_mk2_types.h"
#include "cfg_api_pci_mk2_types.h"
#include "cfg_api_brick_mk2_types.h"
#include "cfg_api_brick_mk3_types.h"
#include "cfg_api_router_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------

/**
 * \ingroup API_Hardware
 *
 * Defines a structure used to store a device's FPGA version info.
 */
typedef struct
{
    /** FPGA major version number */
    U8  major;
    /** FPGA minor version number */
    U8  minor;
    /** FPGA edit number */
    U16 edit;   /* Keep this as a U16 */
    /** FPGA patch number */
    U8  patch;
    /** FPGA build year */
    U16 year;
    /** FPGA build month */
    U8  month;
    /** FPGA build day */
    U8  day;
    /** FPGA build hour */
    U8  hour;
    /** FPGA build minute */
    U8  minute;
} STAR_CFG_FPGA_INFO;

//-------------------------------------

#ifdef __cplusplus
}
#endif

#endif  /* CFG_API_GENERIC_COMMON_H */
