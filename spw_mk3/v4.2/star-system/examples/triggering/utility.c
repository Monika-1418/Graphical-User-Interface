/**
 * \file utility.c
 *
 * \brief Utility functions for the Triggering API program.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Contains various utility functions used by the Triggering API example.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 */

#include "utility.h"
#include "triggering_brick_mk3.h"
#include "triggering_pcie.h"
#include "triggering_pxi_if.h"
#include "triggering_pxi_ro.h"

TRIGGER_DEVICE gTriggerDevice;

/**
 * Resets all external triggers for the given device.
 *
 * @param deviceId the device to reset external triggers for.
 */
void resetExtTriggers(STAR_DEVICE_ID deviceId)
{
    U32 i, j;

    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* No external triggers on PCIe */
        return;
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Reset each external trigger on Brick Mk3 */
        for (i = 0; i < NUM_EXT_TRIGGERS_BRICK_MK3; ++i)
        {
            TRIGGER_BRICK_MK3_disableExtTriggerEdgeDetectMode(deviceId, i);
            TRIGGER_BRICK_MK3_disableExtTriggerInvert(deviceId, i);
            TRIGGER_BRICK_MK3_disableExtTriggerOutput(deviceId, i);
            TRIGGER_BRICK_MK3_setExtTriggerExtend(deviceId, i, 0);

            for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
            {
                TRIGGER_BRICK_MK3_setExtTriggerInputEvents(deviceId, i, j, 
                    EXT_TRIGGER_EVENT_NONE);
                TRIGGER_BRICK_MK3_setExtTriggerOutputActions(deviceId, i, j, 
                    EXT_TRIGGER_ACTION_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Reset each external trigger on PXI */
        for (i = 0; i < NUM_EXT_TRIGGERS_PXI_INTERFACE; ++i)
        {
            TRIGGER_PXI_IF_disableExtTriggerEdgeDetectMode(deviceId, i);
            TRIGGER_PXI_IF_disableExtTriggerInvert(deviceId, i);
            TRIGGER_PXI_IF_disableExtTriggerOutput(deviceId, i);
            TRIGGER_PXI_IF_setExtTriggerExtend(deviceId, i, 0);

            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                TRIGGER_PXI_IF_setExtTriggerInputEvents(deviceId, i, j, 
                    EXT_TRIGGER_EVENT_NONE);
                TRIGGER_PXI_IF_setExtTriggerOutputActions(deviceId, i, j, 
                    EXT_TRIGGER_ACTION_NONE);
            }
        }
    }
}

/**
 * Resets all counters for the given device.
 *
 * @param deviceId the device to reset counters for.
 */
void resetCounters(STAR_DEVICE_ID deviceId)
{
    U32 i, j;

    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* Reset each counter on PCIe */
        for (i = 0; i < NUM_COUNTERS_PCIE; ++i)
        {
            TRIGGER_PCIE_IF_disableCounterAutoReload(deviceId, i);
            TRIGGER_PCIE_IF_disableCounterTriggerCount(deviceId, i);
            TRIGGER_PCIE_IF_disableCounterStartMode(deviceId, i);
            TRIGGER_PCIE_IF_disableCounterStartStopMode(deviceId, i);
            TRIGGER_PCIE_IF_disableCounterLoadZero(deviceId, i);
            TRIGGER_PCIE_IF_setCounterReloadValue(deviceId, i, 0xffffffff);

            for (j = 0; j < NUM_TRIGGERS_PCIE; ++j)
            {
                TRIGGER_PCIE_IF_setCounterInputEvents(deviceId, i, j, 
                    COUNTER_EVENT_NONE);
                TRIGGER_PCIE_IF_setCounterOutputActions(deviceId, i, j, 
                    COUNTER_ACTION_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Reset each counter on Brick Mk3 */
        for (i = 0; i < NUM_COUNTERS_BRICK_MK3; ++i)
        {
            TRIGGER_BRICK_MK3_disableCounterAutoReload(deviceId, i);
            TRIGGER_BRICK_MK3_disableCounterTriggerCount(deviceId, i);
            TRIGGER_BRICK_MK3_disableCounterStartMode(deviceId, i);
            TRIGGER_BRICK_MK3_disableCounterStartStopMode(deviceId, i);
            TRIGGER_BRICK_MK3_disableCounterLoadZero(deviceId, i);
            TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, i, 0xffffffff);

            for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
            {
                TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, i, j, 
                    COUNTER_EVENT_NONE);
                TRIGGER_BRICK_MK3_setCounterOutputActions(deviceId, i, j, 
                    COUNTER_ACTION_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Reset each counter on PXI */
        for (i = 0; i < NUM_COUNTERS_PXI_INTERFACE; ++i)
        {
            TRIGGER_PXI_IF_disableCounterAutoReload(deviceId, i);
            TRIGGER_PXI_IF_disableCounterTriggerCount(deviceId, i);
            TRIGGER_PXI_IF_disableCounterStartMode(deviceId, i);
            TRIGGER_PXI_IF_disableCounterStartStopMode(deviceId, i);
            TRIGGER_PXI_IF_disableCounterLoadZero(deviceId, i);
            TRIGGER_PXI_IF_setCounterReloadValue(deviceId, i, 0xffffffff);

            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                TRIGGER_PXI_IF_setCounterInputEvents(deviceId, i, j, 
                    COUNTER_EVENT_NONE);
                TRIGGER_PXI_IF_setCounterOutputActions(deviceId, i, j, 
                    COUNTER_ACTION_NONE);
            }
        }
    }  
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* Reset each counter on PXI Router */
        for (i = 0; i < NUM_COUNTERS_PXI_ROUTER; ++i)
        {
            TRIGGER_PXI_ROUTER_disableCounterAutoReload(deviceId, i);
            TRIGGER_PXI_ROUTER_disableCounterTriggerCount(deviceId, i);
            TRIGGER_PXI_ROUTER_disableCounterStartMode(deviceId, i);
            TRIGGER_PXI_ROUTER_disableCounterStartStopMode(deviceId, i);
            TRIGGER_PXI_ROUTER_disableCounterLoadZero(deviceId, i);
            TRIGGER_PXI_ROUTER_setCounterReloadValue(deviceId, i, 0xffffffff);

            for (j = 0; j < NUM_TRIGGERS_PXI_ROUTER; ++j)
            {
                TRIGGER_PXI_ROUTER_setCounterInputEvents(deviceId, i, j, 
                    COUNTER_EVENT_NONE);
                TRIGGER_PXI_ROUTER_setCounterOutputActions(deviceId, i, j, 
                    COUNTER_ACTION_NONE);
            }
        }
    }
}

/**
 * Resets all events and actions on each port for the given device.
 *
 * @param deviceId the device to reset ports for.
 */
void resetPorts(STAR_DEVICE_ID deviceId)
{
    U32 i, j;

    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* Reset each port on PCIe */
        for (i = 0; i < NUM_PORTS_PCIE; ++i)
        {
            for (j = 0; j < NUM_TRIGGERS_PCIE; ++j)
            {
                TRIGGER_PCIE_IF_setPortInputEvents(deviceId, i + 1, j, 
                    PORT_EVENT_NONE);
                TRIGGER_PCIE_IF_setPortOutputActions(deviceId, i + 1, j, 
                    PORT_ACTION_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Reset each port on Brick Mk3 */
        for (i = 0; i < NUM_PORTS_BRICK_MK3; ++i)
        {
            TRIGGER_BRICK_MK3_disablePortTransmitPacketMode(deviceId, i + 1);

            for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
            {
                TRIGGER_BRICK_MK3_setPortInputEvents(deviceId, i + 1, j, 
                    PORT_EVENT_NONE);
                TRIGGER_BRICK_MK3_setPortOutputActions(deviceId, i + 1, j, 
                    PORT_ACTION_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Reset each port on PXI */
        for (i = 0; i < NUM_PORTS_PXI_INTERFACE; ++i)
        {
            TRIGGER_PXI_IF_disablePortTransmitPacketMode(deviceId, i + 1);

            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                TRIGGER_PXI_IF_setPortInputEvents(deviceId, i + 1, j, 
                    PORT_EVENT_NONE);
                TRIGGER_PXI_IF_setPortOutputActions(deviceId, i + 1, j, 
                    PORT_ACTION_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* Reset each port on PXI Router */
        for (i = 0; i < NUM_PORTS_PXI_ROUTER; ++i)
        {
            TRIGGER_PXI_ROUTER_disablePortTransmitPacketMode(deviceId, i + 1);

            for (j = 0; j < NUM_TRIGGERS_PXI_ROUTER; ++j)
            {
                TRIGGER_PXI_ROUTER_setPortInputEvents(deviceId, i + 1, j, 
                    PORT_EVENT_NONE);
                TRIGGER_PXI_ROUTER_setPortOutputActions(deviceId, i + 1, j, 
                    PORT_ACTION_NONE);
            }
        }
    }
}

/**
 * Resets each time-code interface for the given device.
 *
 * @param deviceId the device to reset time-codes for.
 */
void resetTimeCodes(STAR_DEVICE_ID deviceId)
{
    U32 i, j;

    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* No time-code interface on PCIe */
        return;
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Reset each time-code interface on Brick Mk3 */
        for (i = 0; i < NUM_TIME_CODES_BRICK_MK3; ++i)
        {
            for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
            {
                TRIGGER_BRICK_MK3_setTimeCodeInputEvents(deviceId, i, j, 
                    TIME_CODE_EVENT_NONE);
                TRIGGER_BRICK_MK3_setTimeCodeOutputActions(deviceId, i, j, 
                    TIME_CODE_ACTION_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Reset each time-code interface on PXI */
        for (i = 0; i < NUM_TIME_CODES_PXI_INTERFACE; ++i)
        {
            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                TRIGGER_PXI_IF_setTimeCodeInputEvents(deviceId, i, j, 
                    TIME_CODE_EVENT_NONE);
                TRIGGER_PXI_IF_setTimeCodeOutputActions(deviceId, i, j, 
                    TIME_CODE_ACTION_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* Reset each time-code interface on PXI Router */
        for (i = 0; i < NUM_TIME_CODES_PXI_ROUTER; ++i)
        {
            for (j = 0; j < NUM_TRIGGERS_PXI_ROUTER; ++j)
            {
                TRIGGER_PXI_ROUTER_setTimeCodeInputEvents(deviceId, i, j, 
                    TIME_CODE_EVENT_NONE);
                TRIGGER_PXI_ROUTER_setTimeCodeOutputActions(deviceId, i, j, 
                    TIME_CODE_ACTION_NONE);
            }
        }
    }
}

/**
 * Resets each trigger for the given device.
 *
 * @param deviceId the device to reset triggers for.
 */
void resetTriggers(STAR_DEVICE_ID deviceId)
{
    U32 i, j;

    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* Reset each trigger on PCIe */
        for (i = 0; i < NUM_TRIGGERS_PCIE; ++i)
        {
            TRIGGER_PCIE_IF_setTriggerInputMode(deviceId, i, 
                TRIGGER_INPUT_MODE_OR);
            TRIGGER_PCIE_IF_disableTrigger(deviceId, i);
            TRIGGER_PCIE_IF_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_COUNTER, 0);
            TRIGGER_PCIE_IF_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_PORT, 0);
            TRIGGER_PCIE_IF_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_TRIGGER, 0);
            TRIGGER_PCIE_IF_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_COUNTER, 0);
            TRIGGER_PCIE_IF_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_PORT, 0);
            TRIGGER_PCIE_IF_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_TRIGGER, 0);

            for (j = 0; j < NUM_TRIGGERS_PCIE; ++j)
            {
                TRIGGER_PCIE_IF_setTriggerInputEvents(deviceId, i, j, 
                    TRIGGER_EVENT_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Reset each trigger on Brick Mk3 */
        for (i = 0; i < NUM_TRIGGERS_BRICK_MK3; ++i)
        {
            TRIGGER_BRICK_MK3_setTriggerInputMode(deviceId, i, 
                TRIGGER_INPUT_MODE_OR);
            TRIGGER_BRICK_MK3_disableTrigger(deviceId, i);
            TRIGGER_BRICK_MK3_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            TRIGGER_BRICK_MK3_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_COUNTER, 0);
            TRIGGER_BRICK_MK3_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_PORT, 0);
            TRIGGER_BRICK_MK3_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_TIME_CODE, 0);
            TRIGGER_BRICK_MK3_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_TRIGGER, 0);
            TRIGGER_BRICK_MK3_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            TRIGGER_BRICK_MK3_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_COUNTER, 0);
            TRIGGER_BRICK_MK3_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_PORT, 0);
            TRIGGER_BRICK_MK3_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_TIME_CODE, 0);
            TRIGGER_BRICK_MK3_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_TRIGGER, 0);

            for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
            {
                TRIGGER_BRICK_MK3_setTriggerInputEvents(deviceId, i, j, 
                    TRIGGER_EVENT_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Reset each trigger on PXI */
        for (i = 0; i < NUM_TRIGGERS_PXI_INTERFACE; ++i)
        {
            TRIGGER_PXI_IF_setTriggerInputMode(deviceId, i, 
                TRIGGER_INPUT_MODE_OR);
            TRIGGER_PXI_IF_disableTrigger(deviceId, i);
            TRIGGER_PXI_IF_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            TRIGGER_PXI_IF_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_COUNTER, 0);
            TRIGGER_PXI_IF_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_PORT, 0);
            TRIGGER_PXI_IF_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_TIME_CODE, 0);
            TRIGGER_PXI_IF_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_TRIGGER, 0);
            TRIGGER_PXI_IF_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            TRIGGER_PXI_IF_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_COUNTER, 0);
            TRIGGER_PXI_IF_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_PORT, 0);
            TRIGGER_PXI_IF_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_TIME_CODE, 0);
            TRIGGER_PXI_IF_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_TRIGGER, 0);

            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                TRIGGER_PXI_IF_setTriggerInputEvents(deviceId, i, j, 
                    TRIGGER_EVENT_NONE);
            }
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* Reset each trigger on PXI Router */
        for (i = 0; i < NUM_TRIGGERS_PXI_ROUTER; ++i)
        {
            TRIGGER_PXI_ROUTER_setTriggerInputMode(deviceId, i, 
                TRIGGER_INPUT_MODE_OR);
            TRIGGER_PXI_ROUTER_disableTrigger(deviceId, i);
            TRIGGER_PXI_ROUTER_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            TRIGGER_PXI_ROUTER_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_COUNTER, 0);
            TRIGGER_PXI_ROUTER_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_PORT, 0);
            TRIGGER_PXI_ROUTER_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_TIME_CODE, 0);
            TRIGGER_PXI_ROUTER_setTriggerInvertMask(deviceId, i, 
                TRIGGER_TYPE_TRIGGER, 0);
            TRIGGER_PXI_ROUTER_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            TRIGGER_PXI_ROUTER_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_COUNTER, 0);
            TRIGGER_PXI_ROUTER_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_PORT, 0);
            TRIGGER_PXI_ROUTER_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_TIME_CODE, 0);
            TRIGGER_PXI_ROUTER_setTriggerAndMask(deviceId, i, 
                TRIGGER_TYPE_TRIGGER, 0);

            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                TRIGGER_PXI_ROUTER_setTriggerInputEvents(deviceId, i, j, 
                    TRIGGER_EVENT_NONE);
            }
        }
    }
}

/**
 * Resets the triggering API configuration for the given device.
 *
 * @param deviceId the device to reset triggering API configuration for.
 */
void reset(STAR_DEVICE_ID deviceId)
{
    /* Reset all configuration for triggering API */
    resetExtTriggers(deviceId);
    resetCounters(deviceId);
    resetPorts(deviceId);
    resetTimeCodes(deviceId);
    resetTriggers(deviceId);
}

/**
 * Prints the currently configured triggering API configuration.
 *
 * @param deviceId the device to print triggering API configuration for.
 */
void printTriggerConf(STAR_DEVICE_ID deviceId)
{
    U32 i;
    unsigned int val;
    TRIGGER_INPUT_MODE triggerInputMode;
    printf("\n----- Printing Trigger Configuration -----\n");

    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        for (i = 0; i < NUM_COUNTERS_PCIE; ++i)
        {
            printf("Counter %u:\n", i);
            TRIGGER_PCIE_IF_getCounterAutoReloadEnabled(deviceId, i, &val);
            printf("    Auto reload: %u\n", val);
            TRIGGER_PCIE_IF_getCounterTriggerCountEnabled(deviceId, i, &val);
            printf("    Trigger count: %u\n", val);
            TRIGGER_PCIE_IF_getCounterStartModeEnabled(deviceId, i, &val);
            printf("    Start mode: %u\n", val);
            TRIGGER_PCIE_IF_getCounterStartStopModeEnabled(deviceId, i, &val);
            printf("    Start stop mode: %u\n", val);
            TRIGGER_PCIE_IF_getCounterLoadZeroEnabled(deviceId, i, &val);
            printf("    Load zero: %u\n", val);
            TRIGGER_PCIE_IF_getCounterReloadValue(deviceId, i, &val);
            printf("    Reload value: %u\n", val);
        }
        for (i = 0; i < NUM_PORTS_PCIE; ++i)
        {
            printf("Port %d:\n", i + 1);
        }
        for (i = 0; i < NUM_TRIGGERS_PCIE; ++i)
        {
            printf("Trigger %d:\n", i);
            TRIGGER_PCIE_IF_getTriggerInputMode(deviceId, i,
                &triggerInputMode);
            printf("    Input mode: %u\n", triggerInputMode);
            TRIGGER_PCIE_IF_getTriggerEnabled(deviceId, i, &val);
            printf("    Enabled: %u\n", val);
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        for (i = 0; i < NUM_EXT_TRIGGERS_BRICK_MK3; ++i)
        {
            printf("External trigger %u:\n", i);
            TRIGGER_BRICK_MK3_getExtTriggerEdgeDetectModeEnabled(deviceId, i,
                &val);
            printf("    Edge detect mode: %u\n", val);
            TRIGGER_BRICK_MK3_getExtTriggerInvertEnabled(deviceId, i, &val);
            printf("    Invert: %u\n", val);
            TRIGGER_BRICK_MK3_getExtTriggerOutputEnabled(deviceId, i, &val);
            printf("    Output: %u\n", val);
            TRIGGER_BRICK_MK3_getExtTriggerExtend(deviceId, i, &val);
            printf("    Extend: %u\n", val);
        }
        for (i = 0; i < NUM_COUNTERS_BRICK_MK3; ++i)
        {
            printf("Counter %u:\n", i);
            TRIGGER_BRICK_MK3_getCounterAutoReloadEnabled(deviceId, i, &val);
            printf("    Auto reload: %u\n", val);
            TRIGGER_BRICK_MK3_getCounterTriggerCountEnabled(deviceId, i, &val);
            printf("    Trigger count: %u\n", val);
            TRIGGER_BRICK_MK3_getCounterStartModeEnabled(deviceId, i, &val);
            printf("    Start mode: %u\n", val);
            TRIGGER_BRICK_MK3_getCounterStartStopModeEnabled(deviceId, i, &val);
            printf("    Start stop mode: %u\n", val);
            TRIGGER_BRICK_MK3_getCounterLoadZeroEnabled(deviceId, i, &val);
            printf("    Load zero: %u\n", val);
            TRIGGER_BRICK_MK3_getCounterReloadValue(deviceId, i, &val);
            printf("        Reload value: %u\n", val);
        }
        for (i = 0; i < NUM_PORTS_BRICK_MK3; ++i)
        {
            printf("Port %d:\n", i + 1);
            TRIGGER_BRICK_MK3_getPortTransmitPacketModeEnabled(deviceId, i + 1,
                &val);
            printf("    Transmit packet mode: %u\n", val);
        }
        for (i = 0; i < NUM_TIME_CODES_BRICK_MK3; ++i)
        {
            printf("Time-code %d:\n", i);
        }
        for (i = 0; i < NUM_TRIGGERS_BRICK_MK3; ++i)
        {
            printf("Trigger %d:\n", i);
            TRIGGER_BRICK_MK3_getTriggerInputMode(deviceId, i,
                &triggerInputMode);
            printf("    Input mode: %u\n", triggerInputMode);
            TRIGGER_BRICK_MK3_getTriggerEnabled(deviceId, i, &val);
            printf("    Enabled: %u\n", val);
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        for (i = 0; i < NUM_EXT_TRIGGERS_PXI_INTERFACE; ++i)
        {
            printf("External trigger %u:\n", i);
            TRIGGER_PXI_IF_getExtTriggerEdgeDetectModeEnabled(deviceId, i,
                &val);
            printf("    Edge detect mode: %u\n", val);
            TRIGGER_PXI_IF_getExtTriggerInvertEnabled(deviceId, i, &val);
            printf("    Invert: %u\n", val);
            TRIGGER_PXI_IF_getExtTriggerOutputEnabled(deviceId, i, &val);
            printf("    Output: %u\n", val);
            TRIGGER_PXI_IF_getExtTriggerExtend(deviceId, i, &val);
            printf("    Extend: %u\n", val);
        }
        for (i = 0; i < NUM_COUNTERS_PXI_INTERFACE; ++i)
        {
            printf("Counter %u:\n", i);
            TRIGGER_PXI_IF_getCounterAutoReloadEnabled(deviceId, i, &val);
            printf("    Auto reload: %u\n", val);
            TRIGGER_PXI_IF_getCounterTriggerCountEnabled(deviceId, i, &val);
            printf("    Trigger count: %u\n", val);
            TRIGGER_PXI_IF_getCounterStartModeEnabled(deviceId, i, &val);
            printf("    Start mode: %u\n", val);
            TRIGGER_PXI_IF_getCounterStartStopModeEnabled(deviceId, i, &val);
            printf("    Start stop mode: %u\n", val);
            TRIGGER_PXI_IF_getCounterLoadZeroEnabled(deviceId, i, &val);
            printf("    Load zero: %u\n", val);
            TRIGGER_PXI_IF_getCounterReloadValue(deviceId, i, &val);
            printf("    Reload value: %u\n", val);
        }
        for (i = 0; i < NUM_PORTS_PXI_INTERFACE; ++i)
        {
            printf("Port %d:\n", i + 1);
            TRIGGER_PXI_IF_getPortTransmitPacketModeEnabled(deviceId, i + 1,
                &val);
            printf("    Transmit packet mode: %u\n", val);
        }
        for (i = 0; i < NUM_TIME_CODES_PXI_INTERFACE; ++i)
        {
            printf("Time-code %d:\n", i);
        }
        for (i = 0; i < NUM_TRIGGERS_PXI_INTERFACE; ++i)
        {
            printf("Trigger %d:\n", i);
            TRIGGER_PXI_IF_getTriggerInputMode(deviceId, i, &triggerInputMode);
            printf("    Input mode: %u\n", triggerInputMode);
            TRIGGER_PXI_IF_getTriggerEnabled(deviceId, i, &val);
            printf("    Enabled: %u\n", val);
        }
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        for (i = 0; i < NUM_COUNTERS_PXI_ROUTER; ++i)
        {
            printf("Counter %u:\n", i);
            TRIGGER_PXI_ROUTER_getCounterAutoReloadEnabled(deviceId, i, &val);
            printf("    Auto reload: %u\n", val);
            TRIGGER_PXI_ROUTER_getCounterTriggerCountEnabled(deviceId, i, &val);
            printf("    Trigger count: %u\n", val);
            TRIGGER_PXI_ROUTER_getCounterStartModeEnabled(deviceId, i, &val);
            printf("    Start mode: %u\n", val);
            TRIGGER_PXI_ROUTER_getCounterStartStopModeEnabled(deviceId, i, 
                &val);
            printf("    Start stop mode: %u\n", val);
            TRIGGER_PXI_ROUTER_getCounterLoadZeroEnabled(deviceId, i, &val);
            printf("    Load zero: %u\n", val);
            TRIGGER_PXI_ROUTER_getCounterReloadValue(deviceId, i, &val);
            printf("    Reload value: %u\n", val);
        }
        for (i = 0; i < NUM_PORTS_PXI_ROUTER; ++i)
        {
            printf("Port %d:\n", i + 1);
            TRIGGER_PXI_ROUTER_getPortTransmitPacketModeEnabled(deviceId, 
                i + 1, &val);
            printf("    Transmit packet mode: %u\n", val);
        }
        for (i = 0; i < NUM_TIME_CODES_PXI_ROUTER; ++i)
        {
            printf("Time-code %d:\n", i);
        }
        for (i = 0; i < NUM_TRIGGERS_PXI_ROUTER; ++i)
        {
            printf("Trigger %d:\n", i);
            TRIGGER_PXI_ROUTER_getTriggerInputMode(deviceId, i, &triggerInputMode);
            printf("    Input mode: %u\n", triggerInputMode);
            TRIGGER_PXI_ROUTER_getTriggerEnabled(deviceId, i, &val);
            printf("    Enabled: %u\n", val);
        }
    }

    printf("\n");
}

/**
 * Queues a 4-byte packet to be transmitted over channel 1.
 *
 * @param deviceId the device to queue packets for.
 * @param count the number of packets to be queued.
 */
void queuePackets(STAR_DEVICE_ID deviceId, U32 count)
{
    STAR_CHANNEL_ID channel;
    char buffer[4];
    U32 i;

    /* Open channel 1 to transmit out of */
    channel = STAR_openChannelToLocalDevice(deviceId, 
        STAR_CHANNEL_DIRECTION_OUT, 1, 1);
    if (!channel) return;

    /* Transmit given number of packets */
    for (i = 0; i < count; ++i)
    {
        memset(buffer, i, 4);
        STAR_transmitPacket(channel, buffer, 4, STAR_EOP_TYPE_EOP, -1);
    }

    /* Close the channel */
    STAR_closeChannel(channel);
}

