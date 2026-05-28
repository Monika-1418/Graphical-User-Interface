/**
 * \file utility.h
 *
 * \brief Declarations of utility functions for the Triggering API program.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Declarations of various utility functions used by the Triggering API example.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <star-api.h>
#include "triggering_types.h"

extern TRIGGER_DEVICE gTriggerDevice;

#define NUM_EXT_TRIGGERS_BRICK_MK3      2
#define NUM_COUNTERS_BRICK_MK3          4
#define NUM_PORTS_BRICK_MK3             2
#define NUM_TIME_CODES_BRICK_MK3        1
#define NUM_TRIGGERS_BRICK_MK3          8

#define NUM_EXT_TRIGGERS_PXI_INTERFACE  4
#define NUM_COUNTERS_PXI_INTERFACE      4
#define NUM_PORTS_PXI_INTERFACE         4
#define NUM_TIME_CODES_PXI_INTERFACE    1
#define NUM_TRIGGERS_PXI_INTERFACE      8

#define NUM_EXT_TRIGGERS_PXI_ROUTER     0
#define NUM_COUNTERS_PXI_ROUTER         4
#define NUM_PORTS_PXI_ROUTER            12
#define NUM_TIME_CODES_PXI_ROUTER       1
#define NUM_TRIGGERS_PXI_ROUTER         4

#define NUM_EXT_TRIGGERS_PCIE           0
#define NUM_COUNTERS_PCIE               6
#define NUM_PORTS_PCIE                  3
#define NUM_TIME_CODES_PCIE             0
#define NUM_TRIGGERS_PCIE               6

#define NUM_EXT_TRIGGERS_MAX            6
#define NUM_COUNTERS_MAX                6
#define NUM_PORTS_MAX                   12
#define NUM_TIME_CODES_MAX              1
#define NUM_TRIGGERS_MAX                8

#if !defined(UNREFERENCED_PARAMETER)
    #define UNREFERENCED_PARAMETER(a)   ((void)(a))
#endif

void resetExtTriggers(STAR_DEVICE_ID deviceId);
void resetCounters(STAR_DEVICE_ID deviceId);
void resetPorts(STAR_DEVICE_ID deviceId);
void resetTimeCodes(STAR_DEVICE_ID deviceId);
void resetTriggers(STAR_DEVICE_ID deviceId);
void reset(STAR_DEVICE_ID deviceId);
void printTriggerConf(STAR_DEVICE_ID deviceId);
void queuePackets(STAR_DEVICE_ID deviceId, U32 count);

#endif // UTILITY_H

