/**
 * \file triggering_example.c
 *
 * \brief Functions for the Triggering API example program.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Contains the bulk of functions used by the STAR-System Triggering API 
 * example program. Provided as an example for developers wishing to write 
 * programs using the Triggering API.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 */

#include "utility.h"

#include "triggering_brick_mk3.h"
#include "triggering_pxi_if.h"
#include "triggering_pxi_ro.h"
#include "triggering_pcie.h"

#define CYCLES_PER_SECOND       60000000

/**
 * Enum containing the different possible menu choices for the application.
 */
typedef enum
{
    MENU_CHOICE_TX_TC_ON_EXT_TRIGGER    = 1,
    MENU_CHOICE_TX_TC_ON_COUNTER,
    MENU_CHOICE_PKT_TX_ON_EXT_TRIGGER,
    MENU_CHOICE_PKT_TX_ON_COUNTER,
    MENU_CHOICE_PKT_TX_ON_TC,
    MENU_CHOICE_MULTIPLE_PKT_TX_ON_EXT_TRIGGER,
    MENU_CHOICE_MULTIPLE_PKT_TX_ON_TC,
    MENU_CHOICE_TIMED_PKT_TX_ON_TC,
    MENU_CHOICE_STOP_RECEIVING,
    MENU_CHOICE_PRINT_TRIGGER_CONF,
    MENU_CHOICE_RESET,
    MENU_CHOICE_EXIT,
    MENU_CHOICE_INVALID
} MENU_CHOICE;

/**
 * Perform transmit time-code on external trigger activity.
 *
 * @param deviceId the device id to setup triggering for.
 */
void txTcOnExtTrigger(STAR_DEVICE_ID deviceId)
{
    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF ||
        gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    reset(deviceId);
    printf("\n----- Trigger Example (Transmit time-code on external trigger) "
        "-----\n");

    if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Enable external trigger 0 edge detect mode */
        TRIGGER_BRICK_MK3_enableExtTriggerEdgeDetectMode(deviceId, 0);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal 
           trigger 0 to be set */
        TRIGGER_BRICK_MK3_setExtTriggerInputEvents(deviceId, 0, 0, 
            EXT_TRIGGER_EVENT_IN);

        /* Internal trigger 0 causes TIME_CODE_ACTION_RX action on external 
           trigger 0 */
        TRIGGER_BRICK_MK3_setTimeCodeOutputActions(deviceId, 0, 0, 
            TIME_CODE_ACTION_TX);

        /* Enable internal trigger 0 */
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 0);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Enable external trigger 0 edge detect mode */
        TRIGGER_PXI_IF_enableExtTriggerEdgeDetectMode(deviceId, 0);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal 
           trigger 0 to be set */
        TRIGGER_PXI_IF_setExtTriggerInputEvents(deviceId, 0, 0, 
            EXT_TRIGGER_EVENT_IN);

        /* Internal trigger 0 causes TIME_CODE_ACTION_RX action on external 
           trigger 0 */
        TRIGGER_PXI_IF_setTimeCodeOutputActions(deviceId, 0, 0, 
            TIME_CODE_ACTION_TX);

        /* Enable internal trigger 0 */
        TRIGGER_PXI_IF_enableTrigger(deviceId, 0);
    }

    printf("Time-codes will now be transmitted when external trigger 0 is "
        "pulsed.\n\n");
}

/**
 * Perform transmit time-code when counter changes.
 *
 * @param deviceId the device id to setup triggering for.
 */
void txTcOnCounter(STAR_DEVICE_ID deviceId)
{
    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    reset(deviceId);
    printf("\n----- Trigger Example (Transmit time-code on timer) -----\n");

    if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Set timer 0 reload value to 1 second */
        TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, 0, CYCLES_PER_SECOND);

        /* Enable timer auto reload */
        TRIGGER_BRICK_MK3_enableCounterAutoReload(deviceId, 0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to 
           be set */
        TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 0, 0, 
            COUNTER_EVENT_RELOAD);

        /* Internal trigger 0 causes TIME_CODE_ACTION_TX action on time-code 
           engine 0 */
        TRIGGER_BRICK_MK3_setTimeCodeOutputActions(deviceId, 0, 0, 
            TIME_CODE_ACTION_TX);

        /* Enable internal trigger 0 */
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 0);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Set timer 0 reload value to 1 second */
        TRIGGER_PXI_IF_setCounterReloadValue(deviceId, 0, CYCLES_PER_SECOND);

        /* Enable timer auto reload */
        TRIGGER_PXI_IF_enableCounterAutoReload(deviceId, 0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to 
           be set */
        TRIGGER_PXI_IF_setCounterInputEvents(deviceId, 0, 0, 
            COUNTER_EVENT_RELOAD);

        /* Internal trigger 0 causes TIME_CODE_ACTION_TX action on time-code 
           engine 0 */
        TRIGGER_PXI_IF_setTimeCodeOutputActions(deviceId, 0, 0, 
            TIME_CODE_ACTION_TX);

        /* Enable internal trigger 0 */
        TRIGGER_PXI_IF_enableTrigger(deviceId, 0);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* Set timer 0 reload value to 1 second */
        TRIGGER_PXI_ROUTER_setCounterReloadValue(deviceId, 0, 
            CYCLES_PER_SECOND);

        /* Enable timer auto reload */
        TRIGGER_PXI_ROUTER_enableCounterAutoReload(deviceId, 0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to 
           be set */
        TRIGGER_PXI_ROUTER_setCounterInputEvents(deviceId, 0, 0, 
            COUNTER_EVENT_RELOAD);

        /* Internal trigger 0 causes TIME_CODE_ACTION_TX action on time-code 
           engine 0 */
        TRIGGER_PXI_ROUTER_setTimeCodeOutputActions(deviceId, 0, 0, 
            TIME_CODE_ACTION_TX);

        /* Enable internal trigger 0 */
        TRIGGER_PXI_ROUTER_enableTrigger(deviceId, 0);
    }

    printf("Time-codes will now be transmitted every second.\n\n");
}

/**
 * Perform transmit packet on external trigger activity.
 *
 * @param deviceId the device id to setup triggering for.
 */
void pktTxOnExtTrigger(STAR_DEVICE_ID deviceId)
{
    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF ||
        gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    reset(deviceId);
    printf("\n----- Trigger Example (Packet transmit on external trigger) "
        "-----\n");

    if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Enable external trigger 0 edge detect mode */
        TRIGGER_BRICK_MK3_enableExtTriggerEdgeDetectMode(deviceId, 0);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal 
           trigger 0 to be set */
        TRIGGER_BRICK_MK3_setExtTriggerInputEvents(deviceId, 0, 0, 
            EXT_TRIGGER_EVENT_IN);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_BRICK_MK3_enablePortTransmitPacketMode(deviceId, 1);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_BRICK_MK3_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 8);

        /* Enable internal trigger 0 */
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 0);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Enable external trigger 0 edge detect mode */
        TRIGGER_PXI_IF_enableExtTriggerEdgeDetectMode(deviceId, 0);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal 
           trigger 0 to be set */
        TRIGGER_PXI_IF_setExtTriggerInputEvents(deviceId, 0, 0, 
            EXT_TRIGGER_EVENT_IN);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_PXI_IF_enablePortTransmitPacketMode(deviceId, 1);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_IF_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 8);

        /* Enable internal trigger 0 */
        TRIGGER_PXI_IF_enableTrigger(deviceId, 0);
    }

    printf("8 packets have been queued and will now be transmitted on port 1 "
        "when external trigger 0 is pulsed.\n\n");
}

/**
 * Perform transmit packet when counter changes.
 *
 * @param deviceId the device id to setup triggering for.
 */
void pktTxOnCounter(STAR_DEVICE_ID deviceId)
{
    reset(deviceId);
    printf("\n----- Trigger Example (Packet transmit on timer) -----\n");

    if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Set timer 0 reload value to 1 second */
        TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, 0, CYCLES_PER_SECOND);

        /* Enable timer auto reload */
        TRIGGER_BRICK_MK3_enableCounterAutoReload(deviceId, 0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to 
           be set */
        TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 0, 0, 
            COUNTER_EVENT_RELOAD);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_BRICK_MK3_enablePortTransmitPacketMode(deviceId, 1);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_BRICK_MK3_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 8);

        /* Enable internal trigger 0 */
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 0);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Set timer 0 reload value to 1 second */
        TRIGGER_PXI_IF_setCounterReloadValue(deviceId, 0, CYCLES_PER_SECOND);

        /* Enable timer auto reload */
        TRIGGER_PXI_IF_enableCounterAutoReload(deviceId, 0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to 
           be set */
        TRIGGER_PXI_IF_setCounterInputEvents(deviceId, 0, 0, 
            COUNTER_EVENT_RELOAD);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_PXI_IF_enablePortTransmitPacketMode(deviceId, 1);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_IF_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 8);

        /* Enable internal trigger 0 */
        TRIGGER_PXI_IF_enableTrigger(deviceId, 0);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* Set timer 0 reload value to 1 second */
        TRIGGER_PXI_ROUTER_setCounterReloadValue(deviceId, 0, 
            CYCLES_PER_SECOND);

        /* Enable timer auto reload */
        TRIGGER_PXI_ROUTER_enableCounterAutoReload(deviceId, 0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to 
           be set */
        TRIGGER_PXI_ROUTER_setCounterInputEvents(deviceId, 0, 0, 
            COUNTER_EVENT_RELOAD);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_PXI_ROUTER_enablePortTransmitPacketMode(deviceId, 1);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_ROUTER_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 8);

        /* Enable internal trigger 0 */
        TRIGGER_PXI_ROUTER_enableTrigger(deviceId, 0);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* Set timer 0 reload value to 1 second */
        TRIGGER_PCIE_IF_setCounterReloadValue(deviceId, 0, CYCLES_PER_SECOND);

        /* Enable timer auto reload */
        TRIGGER_PCIE_IF_enableCounterAutoReload(deviceId, 0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to 
           be set */
        TRIGGER_PCIE_IF_setCounterInputEvents(deviceId, 0, 0, 
            COUNTER_EVENT_RELOAD);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PCIE_IF_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 8);

        /* Enable internal trigger 0 */
        TRIGGER_PCIE_IF_enableTrigger(deviceId, 0);
    }

    printf("8 packets have been queued and will now be transmitted every "
        "second.\n\n");
}

/**
 * Perform transmit packet when time-code is received.
 *
 * @param deviceId the device id to setup triggering for.
 */
void pktTxOnTc(STAR_DEVICE_ID deviceId)
{
    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    reset(deviceId);
    printf("\n----- Trigger Example (Packet transmit on time-code) -----\n");

    if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal 
           trigger 0 to be set */
        TRIGGER_BRICK_MK3_setTimeCodeInputEvents(deviceId, 0, 0, 
            TIME_CODE_EVENT_TICK);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_BRICK_MK3_enablePortTransmitPacketMode(deviceId, 1);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_BRICK_MK3_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 8);

        /* Enable internal trigger 0 */
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 0);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal 
           trigger 0 to be set */
        TRIGGER_PXI_IF_setTimeCodeInputEvents(deviceId, 0, 0, 
            TIME_CODE_EVENT_TICK);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_PXI_IF_enablePortTransmitPacketMode(deviceId, 1);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_IF_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 8);

        /* Enable internal trigger 0 */
        TRIGGER_PXI_IF_enableTrigger(deviceId, 0);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal 
           trigger 0 to be set */
        TRIGGER_PXI_ROUTER_setTimeCodeInputEvents(deviceId, 0, 0, 
            TIME_CODE_EVENT_TICK);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_PXI_ROUTER_enablePortTransmitPacketMode(deviceId, 1);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_ROUTER_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 8);

        /* Enable internal trigger 0 */
        TRIGGER_PXI_ROUTER_enableTrigger(deviceId, 0);
    }

    printf("8 packets have been queued and will now be transmitted when "
        "time-codes are received.\n\n");
}

/**
 * Perform transmit multiple packets on external trigger activity.
 *
 * @param deviceId the device id to setup triggering for.
 */
void multiplePktTxOnExtTrigger(STAR_DEVICE_ID deviceId)
{
    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF ||
        gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    reset(deviceId);
    printf("\n----- Trigger Example (Multiple packet transmit on external "
        "trigger) -----\n");

    if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Enable timer trigger count on timer 0 so the timer only counts on 
           triggers */
        TRIGGER_BRICK_MK3_enableCounterTriggerCount(deviceId, 0);

        /* Set timer 0 reload value to 3 */
        TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, 0, 3);

        /* Enable external trigger 0 edge detect mode */
        TRIGGER_BRICK_MK3_enableExtTriggerEdgeDetectMode(deviceId, 0);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_BRICK_MK3_enablePortTransmitPacketMode(deviceId, 1);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal 
           trigger 0 to be set */
        TRIGGER_BRICK_MK3_setExtTriggerInputEvents(deviceId, 0, 0, 
            EXT_TRIGGER_EVENT_IN);

        /* External trigger 0 causes COUNTER_ACTION_RELOAD action on timer 0 */
        TRIGGER_BRICK_MK3_setCounterOutputActions(deviceId, 0, 0, 
            COUNTER_ACTION_RELOAD);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_BRICK_MK3_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* PORT_EVENT_TX_EOP event causes internal trigger 1 to be set */
        TRIGGER_BRICK_MK3_setPortInputEvents(deviceId, 1, 1, 
            PORT_EVENT_TX_EOP);

        /* Internal trigger 1 causes COUNTER_ACTION_COUNT action on timer 0 */
        TRIGGER_BRICK_MK3_setCounterOutputActions(deviceId, 0, 1, 
            COUNTER_ACTION_COUNT);

        /* COUNTER_EVENT_COUNT event on timer 0 causes internal trigger 2 to be 
        set */
        TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 0, 2, 
            COUNTER_EVENT_COUNT);

        /* Internal trigger 2 causes PORT_ACTION_TRANSMIT_PKT action on 
        port 1 */
        TRIGGER_BRICK_MK3_setPortOutputActions(deviceId, 1, 2, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 32);

        /* Enable internal triggers 0, 1 and 2 */
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 0);
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 1);
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 2);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Enable timer trigger count on timer 0 so the timer only counts on 
           triggers */
        TRIGGER_PXI_IF_enableCounterTriggerCount(deviceId, 0);

        /* Set timer 0 reload value to 3 */
        TRIGGER_PXI_IF_setCounterReloadValue(deviceId, 0, 3);

        /* Enable external trigger 0 edge detect mode */
        TRIGGER_PXI_IF_enableExtTriggerEdgeDetectMode(deviceId, 0);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_PXI_IF_enablePortTransmitPacketMode(deviceId, 1);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal 
           trigger 0 to be set */
        TRIGGER_PXI_IF_setExtTriggerInputEvents(deviceId, 0, 0, 
            EXT_TRIGGER_EVENT_IN);

        /* External trigger 0 causes COUNTER_ACTION_RELOAD action on timer 0 */
        TRIGGER_PXI_IF_setCounterOutputActions(deviceId, 0, 0, 
            COUNTER_ACTION_RELOAD);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_IF_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* PORT_EVENT_TX_EOP event causes internal trigger 1 to be set */
        TRIGGER_PXI_IF_setPortInputEvents(deviceId, 1, 1, PORT_EVENT_TX_EOP);

        /* Internal trigger 1 causes COUNTER_ACTION_COUNT action on timer 0 */
        TRIGGER_PXI_IF_setCounterOutputActions(deviceId, 0, 1, 
            COUNTER_ACTION_COUNT);

        /* COUNTER_EVENT_COUNT event on timer 0 causes internal trigger 2 to be 
           set */
        TRIGGER_PXI_IF_setCounterInputEvents(deviceId, 0, 2, 
            COUNTER_EVENT_COUNT);

        /* Internal trigger 2 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_IF_setPortOutputActions(deviceId, 1, 2, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 32);

        /* Enable internal triggers 0, 1 and 2 */
        TRIGGER_PXI_IF_enableTrigger(deviceId, 0);
        TRIGGER_PXI_IF_enableTrigger(deviceId, 1);
        TRIGGER_PXI_IF_enableTrigger(deviceId, 2);
    }

    printf("32 packets have been queued and will now be transmitted in groups "
        "of four on port 1 when external trigger 0 is pulsed.\n\n");
}

/**
 * Perform transmit multiple packets when a time-code is received.
 *
 * @param deviceId the device id to setup triggering for.
 */
void multiplePktTxOnTc(STAR_DEVICE_ID deviceId)
{
    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    reset(deviceId);
    printf("\n----- Trigger Example (Multiple packet transmit on time-code) "
        "-----\n");

    if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Enable timer trigger count on timer 0 so the timer only counts on 
           triggers */
        TRIGGER_BRICK_MK3_enableCounterTriggerCount(deviceId, 0);

        /* Set timer 0 reload value to 3 */
        TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, 0, 3);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_BRICK_MK3_enablePortTransmitPacketMode(deviceId, 1);

        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal 
           trigger 0 to be set */
        TRIGGER_BRICK_MK3_setTimeCodeInputEvents(deviceId, 0, 0, 
            TIME_CODE_EVENT_TICK);

        /* Internal trigger 0 causes COUNTER_ACTION_RELOAD action on timer 0 */
        TRIGGER_BRICK_MK3_setCounterOutputActions(deviceId, 0, 0, 
            COUNTER_ACTION_RELOAD);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_BRICK_MK3_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* PORT_EVENT_TX_EOP event causes internal trigger 1 to be set */
        TRIGGER_BRICK_MK3_setPortInputEvents(deviceId, 1, 1, 
            PORT_EVENT_TX_EOP);

        /* Internal trigger 1 causes COUNTER_ACTION_COUNT action on timer 0 */
        TRIGGER_BRICK_MK3_setCounterOutputActions(deviceId, 0, 1, 
            COUNTER_ACTION_COUNT);

        /* COUNTER_EVENT_COUNT event on timer 0 causes internal trigger 2 to be 
           set */
        TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 0, 2, 
            COUNTER_EVENT_COUNT);

        /* Internal trigger 2 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_BRICK_MK3_setPortOutputActions(deviceId, 1, 2, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 32);

        /* Enable internal triggers 0, 1 and 2 */
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 0);
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 1);
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 2);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Enable timer trigger count on timer 0 so the timer only counts on 
           triggers */
        TRIGGER_PXI_IF_enableCounterTriggerCount(deviceId, 0);

        /* Set timer 0 reload value to 3 */
        TRIGGER_PXI_IF_setCounterReloadValue(deviceId, 0, 3);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_PXI_IF_enablePortTransmitPacketMode(deviceId, 1);

        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal 
           trigger 0 to be set */
        TRIGGER_PXI_IF_setTimeCodeInputEvents(deviceId, 0, 0, 
            TIME_CODE_EVENT_TICK);

        /* Internal trigger 0 causes COUNTER_ACTION_RELOAD action on timer 0 */
        TRIGGER_PXI_IF_setCounterOutputActions(deviceId, 0, 0, 
            COUNTER_ACTION_RELOAD);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_IF_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* PORT_EVENT_TX_EOP event causes internal trigger 1 to be set */
        TRIGGER_PXI_IF_setPortInputEvents(deviceId, 1, 1, 
            PORT_EVENT_TX_EOP);

        /* Internal trigger 1 causes COUNTER_ACTION_COUNT action on timer 0 */
        TRIGGER_PXI_IF_setCounterOutputActions(deviceId, 0, 1, 
            COUNTER_ACTION_COUNT);

        /* COUNTER_EVENT_COUNT event on timer 0 causes internal trigger 2 to be 
           set */
        TRIGGER_PXI_IF_setCounterInputEvents(deviceId, 0, 2, 
            COUNTER_EVENT_COUNT);

        /* Internal trigger 2 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_IF_setPortOutputActions(deviceId, 1, 2, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 32);

        /* Enable internal triggers 0, 1 and 2 */
        TRIGGER_PXI_IF_enableTrigger(deviceId, 0);
        TRIGGER_PXI_IF_enableTrigger(deviceId, 1);
        TRIGGER_PXI_IF_enableTrigger(deviceId, 2);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* Enable timer trigger count on timer 0 so the timer only counts on 
           triggers */
        TRIGGER_PXI_ROUTER_enableCounterTriggerCount(deviceId, 0);

        /* Set timer 0 reload value to 3 */
        TRIGGER_PXI_ROUTER_setCounterReloadValue(deviceId, 0, 3);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_PXI_ROUTER_enablePortTransmitPacketMode(deviceId, 1);

        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal 
           trigger 0 to be set */
        TRIGGER_PXI_ROUTER_setTimeCodeInputEvents(deviceId, 0, 0, 
            TIME_CODE_EVENT_TICK);

        /* Internal trigger 0 causes COUNTER_ACTION_RELOAD action on timer 0 */
        TRIGGER_PXI_ROUTER_setCounterOutputActions(deviceId, 0, 0, 
            COUNTER_ACTION_RELOAD);

        /* Internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_ROUTER_setPortOutputActions(deviceId, 1, 0, 
            PORT_ACTION_TRANSMIT_PKT);

        /* PORT_EVENT_TX_EOP event causes internal trigger 1 to be set */
        TRIGGER_PXI_ROUTER_setPortInputEvents(deviceId, 1, 1, 
            PORT_EVENT_TX_EOP);

        /* Internal trigger 1 causes COUNTER_ACTION_COUNT action on timer 0 */
        TRIGGER_PXI_ROUTER_setCounterOutputActions(deviceId, 0, 1, 
            COUNTER_ACTION_COUNT);

        /* COUNTER_EVENT_COUNT event on timer 0 causes internal trigger 2 to be 
           set */
        TRIGGER_PXI_ROUTER_setCounterInputEvents(deviceId, 0, 2, 
            COUNTER_EVENT_COUNT);

        /* Internal trigger 2 causes PORT_ACTION_TRANSMIT_PKT action on 
           port 1 */
        TRIGGER_PXI_ROUTER_setPortOutputActions(deviceId, 1, 2, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Queue up some packets */
        queuePackets(deviceId, 32);

        /* Enable internal triggers 0, 1 and 2 */
        TRIGGER_PXI_ROUTER_enableTrigger(deviceId, 0);
        TRIGGER_PXI_ROUTER_enableTrigger(deviceId, 1);
        TRIGGER_PXI_ROUTER_enableTrigger(deviceId, 2);
    }

    printf("32 packets have been queued and will now be transmitted in groups "
        "of four on port 1 when time-codes are received.\n\n");
}

/**
 * Perform timed packet transmit when a time-code is received.
 *
 * @param deviceId the device id to setup triggering for.
 */
void timedPktTxOnTc(STAR_DEVICE_ID deviceId)
{
    if (gTriggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    reset(deviceId);
    printf("\n----- Trigger Example (Timed Packet Transmit on Time-Code) "
        "-----\n");

    if (gTriggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* Setup timer 0 to transmit time-codes every second and reload/start 
           timer 1 */
        TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, 0, CYCLES_PER_SECOND);
        TRIGGER_BRICK_MK3_enableCounterAutoReload(deviceId, 0);
        TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 0, 0, 
            COUNTER_EVENT_RELOAD);
        TRIGGER_BRICK_MK3_setTimeCodeOutputActions(deviceId, 0, 0, 
            TIME_CODE_ACTION_TX);
        TRIGGER_BRICK_MK3_setCounterOutputActions(deviceId, 1, 0, 
            COUNTER_ACTION_RELOAD);
        TRIGGER_BRICK_MK3_setCounterOutputActions(deviceId, 1, 0, 
            COUNTER_ACTION_START);

        /* Setup timer 1 to expire every 200 ms and enable start/stop mode */
        TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, 1, 
            CYCLES_PER_SECOND / 5);
        TRIGGER_BRICK_MK3_enableCounterAutoReload(deviceId, 1);
        TRIGGER_BRICK_MK3_enableCounterStartStopMode(deviceId, 1);

        /* Setup timer 1 to decrement timer 2 (packet counter timer) */
        TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 1, 1, 
            COUNTER_EVENT_ZERO_SINGLE);
        TRIGGER_BRICK_MK3_setCounterOutputActions(deviceId, 2, 1, 
            COUNTER_ACTION_COUNT);

        /* Setup timer 2 to count from 4 to 0 and enable trigger count mode */
        TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, 2, 4);
        TRIGGER_BRICK_MK3_enableCounterAutoReload(deviceId, 2);
        TRIGGER_BRICK_MK3_enableCounterTriggerCount(deviceId, 2);

        /* Setup timer 2 to transmit packets */
        TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 2, 2, 
            COUNTER_EVENT_COUNT);
        TRIGGER_BRICK_MK3_setPortOutputActions(deviceId, 1, 2, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Setup timer 2 to stop timer 1 when it hits 0 */
        TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 2, 3, 
            COUNTER_EVENT_ZERO_SINGLE);
        TRIGGER_BRICK_MK3_setCounterOutputActions(deviceId, 1, 3, 
            COUNTER_ACTION_STOP);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_BRICK_MK3_enablePortTransmitPacketMode(deviceId, 1);

        /* Reload timers */
        TRIGGER_BRICK_MK3_forceCounterReload(deviceId, 0);
        TRIGGER_BRICK_MK3_forceCounterReload(deviceId, 1);
        TRIGGER_BRICK_MK3_forceCounterReload(deviceId, 2);

        /* Enable internal triggers 0-3 */
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 0);
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 1);
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 2);
        TRIGGER_BRICK_MK3_enableTrigger(deviceId, 3);

        queuePackets(deviceId, 32);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* Setup timer 0 to transmit time-codes every second and reload/start 
           timer 1 */
        TRIGGER_PXI_IF_setCounterReloadValue(deviceId, 0, CYCLES_PER_SECOND);
        TRIGGER_PXI_IF_enableCounterAutoReload(deviceId, 0);
        TRIGGER_PXI_IF_setCounterInputEvents(deviceId, 0, 0, 
            COUNTER_EVENT_RELOAD);
        TRIGGER_PXI_IF_setTimeCodeOutputActions(deviceId, 0, 0, 
            TIME_CODE_ACTION_TX);
        TRIGGER_PXI_IF_setCounterOutputActions(deviceId, 1, 0, 
            COUNTER_ACTION_RELOAD);
        TRIGGER_PXI_IF_setCounterOutputActions(deviceId, 1, 0, 
            COUNTER_ACTION_START);

        /* Setup timer 1 to expire every 200 ms and enable start/stop mode */
        TRIGGER_PXI_IF_setCounterReloadValue(deviceId, 1, 
            CYCLES_PER_SECOND / 5);
        TRIGGER_PXI_IF_enableCounterAutoReload(deviceId, 1);
        TRIGGER_PXI_IF_enableCounterStartStopMode(deviceId, 1);

        /* Setup timer 1 to decrement timer 2 (packet counter timer) */
        TRIGGER_PXI_IF_setCounterInputEvents(deviceId, 1, 1, 
            COUNTER_EVENT_ZERO_SINGLE);
        TRIGGER_PXI_IF_setCounterOutputActions(deviceId, 2, 1, 
            COUNTER_ACTION_COUNT);

        /* Setup timer 2 to count from 4 to 0 and enable trigger count mode */
        TRIGGER_PXI_IF_setCounterReloadValue(deviceId, 2, 4);
        TRIGGER_PXI_IF_enableCounterAutoReload(deviceId, 2);
        TRIGGER_PXI_IF_enableCounterTriggerCount(deviceId, 2);

        /* Setup timer 2 to transmit packets */
        TRIGGER_PXI_IF_setCounterInputEvents(deviceId, 2, 2, 
            COUNTER_EVENT_COUNT);
        TRIGGER_PXI_IF_setPortOutputActions(deviceId, 1, 2, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Setup timer 2 to stop timer 1 when it hits 0 */
        TRIGGER_PXI_IF_setCounterInputEvents(deviceId, 2, 3, 
            COUNTER_EVENT_ZERO_SINGLE);
        TRIGGER_PXI_IF_setCounterOutputActions(deviceId, 1, 3, 
            COUNTER_ACTION_STOP);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_PXI_IF_enablePortTransmitPacketMode(deviceId, 1);

        /* Reload timers */
        TRIGGER_PXI_IF_forceCounterReload(deviceId, 0);
        TRIGGER_PXI_IF_forceCounterReload(deviceId, 1);
        TRIGGER_PXI_IF_forceCounterReload(deviceId, 2);

        /* Enable internal triggers 0-3 */
        TRIGGER_PXI_IF_enableTrigger(deviceId, 0);
        TRIGGER_PXI_IF_enableTrigger(deviceId, 1);
        TRIGGER_PXI_IF_enableTrigger(deviceId, 2);
        TRIGGER_PXI_IF_enableTrigger(deviceId, 3);

        queuePackets(deviceId, 32);
    }
    else if (gTriggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* Setup timer 0 to transmit time-codes every second and reload/start 
           timer 1 */
        TRIGGER_PXI_ROUTER_setCounterReloadValue(deviceId, 0, 
            CYCLES_PER_SECOND);
        TRIGGER_PXI_ROUTER_enableCounterAutoReload(deviceId, 0);
        TRIGGER_PXI_ROUTER_setCounterInputEvents(deviceId, 0, 0, 
            COUNTER_EVENT_RELOAD);
        TRIGGER_PXI_ROUTER_setTimeCodeOutputActions(deviceId, 0, 0, 
            TIME_CODE_ACTION_TX);
        TRIGGER_PXI_ROUTER_setCounterOutputActions(deviceId, 1, 0, 
            COUNTER_ACTION_RELOAD);
        TRIGGER_PXI_ROUTER_setCounterOutputActions(deviceId, 1, 0, 
            COUNTER_ACTION_START);

        /* Setup timer 1 to expire every 200 ms and enable start/stop mode */
        TRIGGER_PXI_ROUTER_setCounterReloadValue(deviceId, 1, 
            CYCLES_PER_SECOND / 5);
        TRIGGER_PXI_ROUTER_enableCounterAutoReload(deviceId, 1);
        TRIGGER_PXI_ROUTER_enableCounterStartStopMode(deviceId, 1);

        /* Setup timer 1 to decrement timer 2 (packet counter timer) */
        TRIGGER_PXI_ROUTER_setCounterInputEvents(deviceId, 1, 1, 
            COUNTER_EVENT_ZERO_SINGLE);
        TRIGGER_PXI_ROUTER_setCounterOutputActions(deviceId, 2, 1, 
            COUNTER_ACTION_COUNT);

        /* Setup timer 2 to count from 4 to 0 and enable trigger count mode */
        TRIGGER_PXI_ROUTER_setCounterReloadValue(deviceId, 2, 4);
        TRIGGER_PXI_ROUTER_enableCounterAutoReload(deviceId, 2);
        TRIGGER_PXI_ROUTER_enableCounterTriggerCount(deviceId, 2);

        /* Setup timer 2 to transmit packets */
        TRIGGER_PXI_ROUTER_setCounterInputEvents(deviceId, 2, 2, 
            COUNTER_EVENT_COUNT);
        TRIGGER_PXI_ROUTER_setPortOutputActions(deviceId, 1, 2, 
            PORT_ACTION_TRANSMIT_PKT);

        /* Setup timer 2 to stop timer 1 when it hits 0 */
        TRIGGER_PXI_ROUTER_setCounterInputEvents(deviceId, 2, 3, 
            COUNTER_EVENT_ZERO_SINGLE);
        TRIGGER_PXI_ROUTER_setCounterOutputActions(deviceId, 1, 3, 
            COUNTER_ACTION_STOP);

        /* Enable port transmit packet mode on port 1 */
        TRIGGER_PXI_ROUTER_enablePortTransmitPacketMode(deviceId, 1);

        /* Reload timers */
        TRIGGER_PXI_ROUTER_forceCounterReload(deviceId, 0);
        TRIGGER_PXI_ROUTER_forceCounterReload(deviceId, 1);
        TRIGGER_PXI_ROUTER_forceCounterReload(deviceId, 2);

        /* Enable internal triggers 0-3 */
        TRIGGER_PXI_ROUTER_enableTrigger(deviceId, 0);
        TRIGGER_PXI_ROUTER_enableTrigger(deviceId, 1);
        TRIGGER_PXI_ROUTER_enableTrigger(deviceId, 2);
        TRIGGER_PXI_ROUTER_enableTrigger(deviceId, 3);

        queuePackets(deviceId, 32);
    }

    printf("32 packets have been queued and will now be transmitted in groups "
        "of 4 at 200 ms intervals when time-codes are transmitted.\n\n");
}

/**
 * Perform stop receiving for 5 seconds every 10 seconds.
 *
 * @param deviceId the device id to setup triggering for.
 */
void stopReceiving(STAR_DEVICE_ID deviceId)
{
    if (gTriggerDevice != TRIGGER_DEVICE_BRICK_MK3)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    reset(deviceId);
    printf("\n----- Trigger Example (Stop Receiving on Port 2 For 5 Seconds "
        "Every 10 Seconds) -----\n");

    /* Setup counter 0 to fire every 10 seconds and reload counter 1 */
    TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, 0,
        10 * CYCLES_PER_SECOND);
    TRIGGER_BRICK_MK3_enableCounterAutoReload(deviceId, 0);
    TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 0, 0,
        COUNTER_EVENT_RELOAD);
    TRIGGER_BRICK_MK3_setCounterOutputActions(deviceId, 1, 0,
        COUNTER_ACTION_RELOAD);

    /* Setup counter 1 to fire a single-shot 5 second timer which, while    */
    /* not zero, causes the STOP_RECEPTION action on port 2                 */
    TRIGGER_BRICK_MK3_setCounterReloadValue(deviceId, 1,
        5 * CYCLES_PER_SECOND);
    TRIGGER_BRICK_MK3_disableCounterAutoReload(deviceId, 1);
    TRIGGER_BRICK_MK3_setCounterInputEvents(deviceId, 1, 1,
        COUNTER_EVENT_ZERO);
    TRIGGER_BRICK_MK3_setPortOutputActions(deviceId, 2, 1,
        PORT_ACTION_STOP_RECEPTION);

    /* Setup internal trigger 1 to receive inverted input from counter 1 */
    TRIGGER_BRICK_MK3_setTriggerInvertMask(deviceId, 1,
        TRIGGER_TYPE_COUNTER, 0x2);

    /* Enable internal triggers 0-1 */
    TRIGGER_BRICK_MK3_enableTrigger(deviceId, 0);
    TRIGGER_BRICK_MK3_enableTrigger(deviceId, 1);

    printf("Reception will now be stopped for 5 seconds every 10 seconds.\n\n");
}

/**
 * Resets the triggering configuration for the given device id.
 *
 * @param deviceId the device id to reset triggering configuration for.
 */
void resetTriggerConf(STAR_DEVICE_ID deviceId)
{
    reset(deviceId);
    printf("\n----- Trigger Example (Reset triggers) -----\n");
    printf("Triggers have been reset.\n\n");
}

/**
 * Prompts the user and returns the menu choice that the user selected.
 *
 * @return selected menu choice value.
 */
MENU_CHOICE getMenuChoice(void)
{
    char buffer[32];
    unsigned int choice;

    /* Show menu to user */
    printf("Please select example to run:\n");
    printf("%d. Transmit time-code on external trigger.\n", 
        MENU_CHOICE_TX_TC_ON_EXT_TRIGGER);
    printf("%d. Transmit time-code on timer.\n", 
        MENU_CHOICE_TX_TC_ON_COUNTER);
    printf("%d. Transmit packet on external trigger.\n", 
        MENU_CHOICE_PKT_TX_ON_EXT_TRIGGER);
    printf("%d. Transmit packet on timer.\n", 
        MENU_CHOICE_PKT_TX_ON_COUNTER);
    printf("%d. Transmit packet on time-code.\n", 
        MENU_CHOICE_PKT_TX_ON_TC);
    printf("%d. Transmit multiple packets on external trigger.\n", 
        MENU_CHOICE_MULTIPLE_PKT_TX_ON_EXT_TRIGGER);
    printf("%d. Transmit multiple packets on time-code.\n", 
        MENU_CHOICE_MULTIPLE_PKT_TX_ON_TC);
    printf("%d. Transmit packets at intervals with time-code.\n", 
        MENU_CHOICE_TIMED_PKT_TX_ON_TC);
    printf("%d. Stop receiving for 5 seconds every 10 seconds.\n", 
        MENU_CHOICE_STOP_RECEIVING);
    printf("%d. Print trigger configuration.\n", 
        MENU_CHOICE_PRINT_TRIGGER_CONF);
    printf("%d. Reset triggers.\n", MENU_CHOICE_RESET);
    printf("%d. Exit\n", MENU_CHOICE_EXIT);
    printf("> ");

    /* Validate chosen number */
    if (!fgets(buffer, 32, stdin)) return MENU_CHOICE_INVALID;

    /* Get menu choice value  */
    if (!sscanf(buffer, "%u", &choice) || 
        choice >= MENU_CHOICE_INVALID) return MENU_CHOICE_INVALID;

    /* Return the chosen value */
    return choice;
}

/**
 * Selects the first device of the given type and returns its id or asks the 
 * user to choose one if more than one device exists.
 *
 * @return selected device id.
 */
STAR_DEVICE_ID getTriggerDeviceID()
{
    char *pDeviceName, *pDeviceSerial, buffer[256];
    unsigned int chosen;
    int status;

    /* List devices that support triggering API */
    U32 deviceTypes[5] = { STAR_DEVICE_BRICK_MK3, 
                           STAR_DEVICE_PXI_INTERFACE, 
                           STAR_DEVICE_PXI_RMAP, 
                           STAR_DEVICE_PXI_ROUTER_12, 
                           STAR_DEVICE_PCIE };

    /* Get available devices that support triggering */
    U32 deviceCount;
    STAR_DEVICE_ID* pDevices = STAR_getDeviceListForTypes(deviceTypes, 5, 
        &deviceCount);
    STAR_DEVICE_ID selectedDeviceID = STAR_DEVICE_UNKNOWN;

    /* If more than one device is available */
    if (deviceCount > 1)
    {
        U32 x;

        printf("The following devices are available that support triggering:"
            "\n\n");

        /* For each device */
        for (x = 0; x < deviceCount; x++)
        {
            /* Display its name */
            if (pDevices[x] != STAR_DEVICE_UNKNOWN)
            {
                pDeviceName = STAR_getDeviceName(pDevices[x]);
                pDeviceSerial = STAR_getDeviceSerialNumber(pDevices[x]);
                if (pDeviceName != NULL && pDeviceSerial != NULL)
                {
                    printf("\t%u - %s with serial number %s\n", x, pDeviceName, 
                        pDeviceSerial);
                }
                else
                {
                    printf("\t%u - Unknown SpaceWire Device\n", x);
                }

                STAR_destroyString(pDeviceName);
                STAR_destroyString(pDeviceSerial);
            }
            else
            {
                printf("\t%u - Unable to access device\n", x);
            }
        }

        printf("\n");

        /* Ask the user which device to use */
        do
        {
            printf("Please select which device to use: ");
            fflush(stdout);

            /* Read selected device number */
            if (fgets(buffer, 256, stdin) == NULL)
            {
                puts("No device number selected.");

                continue;
            }
            status = sscanf(buffer, "%u", &chosen);
            if ((status == 0) || (chosen > (deviceCount - 1U)))
            {
                puts("Invalid device number selected.");

                continue;
            }

            /* Get the selected device id */
            selectedDeviceID = pDevices[chosen];
        }
        while (selectedDeviceID == STAR_DEVICE_UNKNOWN);
    }
    /* Else if just one device is available */
    else if (deviceCount == 1)
    {
        /* Select the first device id */
        selectedDeviceID = pDevices[0];
    }

    /* If valid device was selected */
    if (selectedDeviceID != STAR_DEVICE_UNKNOWN)
    {
        /* Get device name and serial */
        pDeviceName = STAR_getDeviceName(selectedDeviceID);
        pDeviceSerial = STAR_getDeviceSerialNumber(selectedDeviceID);

        /* Print selected device */
        printf("%s selected with serial number %s\n\n", pDeviceName, 
            pDeviceSerial);

        /* Destroy device and serial */
        STAR_destroyString(pDeviceName);
        STAR_destroyString(pDeviceSerial);
    }

    STAR_destroyDeviceList(pDevices);

    /* Return the selected device id */
    return selectedDeviceID;
}

/**
 * Returns the corresponding triggering device type for the given STAR-System 
 * device type.
 *
 * @param deviceType the device type to get triggering device type for.
 *
 * @return triggering device type.
 */
TRIGGER_DEVICE getTriggerDeviceType(U32 deviceType)
{
    if (deviceType == STAR_DEVICE_BRICK_MK3)
    {
        return TRIGGER_DEVICE_BRICK_MK3;
    }
    else if (deviceType == STAR_DEVICE_PXI_INTERFACE || 
        deviceType == STAR_DEVICE_PXI_RMAP)
    {
        return TRIGGER_DEVICE_PXI_IF;
    }
    else if (deviceType == STAR_DEVICE_PXI_ROUTER_12)
    {
        return TRIGGER_DEVICE_PXI_ROUTER;
    }
    else if (deviceType == STAR_DEVICE_PCIE)
    {
        return TRIGGER_DEVICE_PCIE_IF;
    }

    return TRIGGER_DEVICE_INVALID;
}

/**
 * Main function, called when the program is started.
 *
 * @param argc the number of parameters passed to the program.
 * @param argv the array of arguments passed to the program.
 *
 * @return returns 0 on normal program ending, otherwise 1
 */
int main(int argc, char **argv)
{
    STAR_DEVICE_ID deviceId;
    MENU_CHOICE choice;
    U32 deviceType = STAR_DEVICE_UNKNOWN;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    /* Print the program header */
    printf("----- Trigger API Examples -----\n");

    /* Select the device to use */
    deviceId = getTriggerDeviceID();

    /* Check that a device was selected */
    if (deviceId == 0)
    {
        printf("No devices were found.\n");
        return 0;
    }

    /* Get type of device */
    deviceType = STAR_getDeviceType(deviceId);
    gTriggerDevice = getTriggerDeviceType(deviceType);

    /* Reset triggers */
    reset(deviceId);

    /* Loop menu */
    choice = MENU_CHOICE_INVALID;
    do
    {
        choice = getMenuChoice();
        switch (choice)
        {
            /* Time-code on external trigger (not supported on PCIe or 
               PXI Router) */
            case MENU_CHOICE_TX_TC_ON_EXT_TRIGGER:
                txTcOnExtTrigger(deviceId);
                break;
            /* Time-code on counter (not supported on PCIe) */
            case MENU_CHOICE_TX_TC_ON_COUNTER:
                txTcOnCounter(deviceId);
                break;
            /* Transmit packet on external trigger (not supported on PCIe or 
               PXI Router) */
            case MENU_CHOICE_PKT_TX_ON_EXT_TRIGGER:
                pktTxOnExtTrigger(deviceId);
                break;
            /* Transmit packet on counter */
            case MENU_CHOICE_PKT_TX_ON_COUNTER:
                pktTxOnCounter(deviceId);
                break;
            /* Transmit packet on time-code (not supported on PCIe) */
            case MENU_CHOICE_PKT_TX_ON_TC:
                pktTxOnTc(deviceId);
                break;
            /* Transmit multiple packets on external trigger 
               (not supported on PCIe or PXI Router) */
            case MENU_CHOICE_MULTIPLE_PKT_TX_ON_EXT_TRIGGER:
                multiplePktTxOnExtTrigger(deviceId);
                break;
            /* Transmit multiple packets on time-code (not supported on PCIe) */
            case MENU_CHOICE_MULTIPLE_PKT_TX_ON_TC:
                multiplePktTxOnTc(deviceId);
                break;
            /* Timed packet transmit on time-code (not supported on PCIe) */
            case MENU_CHOICE_TIMED_PKT_TX_ON_TC:
                timedPktTxOnTc(deviceId);
                break;
            /* Stop receiving for 5 seconds every 10 seconds (only
               supported on Brick Mk3) */
            case MENU_CHOICE_STOP_RECEIVING:
                stopReceiving(deviceId);
                break;
            /* Print trigger configuration */
            case MENU_CHOICE_PRINT_TRIGGER_CONF:
                printTriggerConf(deviceId);
                break;
            /* Reset trigger configuration */
            case MENU_CHOICE_RESET: resetTriggerConf(deviceId); break;

            /* Handle unused cases */
            case MENU_CHOICE_INVALID: break;
            case MENU_CHOICE_EXIT: break;
        }
    } while (choice != MENU_CHOICE_EXIT);

    /* Reset triggers */
    reset(deviceId);

    return 0;
}

