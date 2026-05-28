/**
 * \file triggering_utility.cpp
 *
 * \brief Implementation of utility functions for the Triggering API program.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Implementation of various utility functions used by the Triggering API
 * example.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 */

#include "triggering_utility.h"

#include "trigger_factory.hpp"

#include "channel.hpp"
#include "packet.hpp"

using namespace stardundee::com::starsystem::triggering;
using namespace stardundee::com::starsystem::triggering::brickmk3;
using namespace stardundee::com::starsystem::triggering::pxi;
using namespace stardundee::com::starsystem::triggering::pcie;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::streamitems;

TriggeringUtility::TriggeringUtility(TRIGGER_DEVICE triggerDevice)
{
    m_triggerDevice = triggerDevice;
}

void TriggeringUtility::ResetExtTriggers(Device *pDevice)
{
    U32 i, j;

    /* create trigger utilities */
    TriggerConfiguration *pTriggerConfiguration =
        TriggerFactory::CreateTriggerConfiguration(pDevice);
    TriggerEventManager *pTriggerEventManager =
        TriggerFactory::CreateTriggerEventManager(pDevice);
    TriggerActionManager *pTriggerActionManager =
        TriggerFactory::CreateTriggerActionManager(pDevice);

    /* validate trigger utilities */
    if (pTriggerConfiguration == NULL ||
        pTriggerEventManager == NULL ||
        pTriggerActionManager == NULL)
    {
        printf("\n ResetExtTriggers not supported by this device.\n");
        return;
    }

    if (m_triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* no external triggers on PCIe */
        return;
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* reset each external trigger on Brick Mk3 */
        for (i = 0; i < NUM_EXT_TRIGGERS_BRICK_MK3; ++i)
        {
            pBrickMk3TriggerConfiguration->DisableExtTriggerEdgeDetectMode(i);
            pBrickMk3TriggerConfiguration->DisableExtTriggerInvert(i);
            pBrickMk3TriggerConfiguration->DisableExtTriggerOutput(i);
            pBrickMk3TriggerConfiguration->SetExtTriggerExtend(i, 0);

            for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
            {
                pBrickMk3TriggerEventManager->SetExtTriggerInputEvents(i, j,
                    EXT_TRIGGER_EVENT_NONE);
                pBrickMk3TriggerActionManager->SetExtTriggerOutputActions(i, j,
                    EXT_TRIGGER_ACTION_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* reset each external trigger on PXI */
        for (i = 0; i < NUM_EXT_TRIGGERS_PXI_INTERFACE; ++i)
        {
            pPXITriggerConfiguration->DisableExtTriggerEdgeDetectMode(i);
            pPXITriggerConfiguration->DisableExtTriggerInvert(i);
            pPXITriggerConfiguration->DisableExtTriggerOutput(i);
            pPXITriggerConfiguration->SetExtTriggerExtend(i, 0);

            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                pPXITriggerEventManager->SetExtTriggerInputEvents(i, j,
                    EXT_TRIGGER_EVENT_NONE);
                pPXITriggerActionManager->SetExtTriggerOutputActions(i, j,
                    EXT_TRIGGER_ACTION_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* no external triggers on PXI Router */
        return;
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;
}

void TriggeringUtility::ResetCounters(Device *pDevice)
{
    U32 i, j;

    /* create trigger utilities */
    TriggerConfiguration *pTriggerConfiguration =
        TriggerFactory::CreateTriggerConfiguration(pDevice);
    TriggerEventManager *pTriggerEventManager =
        TriggerFactory::CreateTriggerEventManager(pDevice);
    TriggerActionManager *pTriggerActionManager =
        TriggerFactory::CreateTriggerActionManager(pDevice);

    /* validate trigger utilities */
    if (pTriggerConfiguration == NULL ||
        pTriggerEventManager == NULL ||
        pTriggerActionManager == NULL)
    {
        printf("\n ResetCounters not supported by this device.\n");
        return;
    }

    if (m_triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* get PCIe triggering utilities */
        PCIeTriggerConfiguration *pPCIeTriggerConfiguration =
            (PCIeTriggerConfiguration *)pTriggerConfiguration;
        PCIeTriggerEventManager *pPCIeTriggerEventManager =
            (PCIeTriggerEventManager *)pTriggerEventManager;
        PCIeTriggerActionManager *pPCIeTriggerActionManager =
            (PCIeTriggerActionManager *)pTriggerActionManager;

        /* reset each counter on PCIe */
        for (i = 0; i < NUM_COUNTERS_PCIE; ++i)
        {
            pPCIeTriggerConfiguration->DisableCounterAutoReload(i);
            pPCIeTriggerConfiguration->DisableCounterTriggerCount(i);
            pPCIeTriggerConfiguration->DisableCounterStartMode(i);
            pPCIeTriggerConfiguration->DisableCounterStartStopMode(i);
            pPCIeTriggerConfiguration->DisableCounterLoadZero(i);
            pPCIeTriggerConfiguration->SetCounterReloadValue(i, 0xffffffff);

            for (j = 0; j < NUM_TRIGGERS_PCIE; ++j)
            {
                pPCIeTriggerEventManager->SetCounterInputEvents(i, j,
                    COUNTER_EVENT_NONE);
                pPCIeTriggerActionManager->SetCounterOutputActions(i, j,
                    COUNTER_ACTION_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* reset each counter on Brick Mk3 */
        for (i = 0; i < NUM_COUNTERS_BRICK_MK3; ++i)
        {
            pBrickMk3TriggerConfiguration->DisableCounterAutoReload(i);
            pBrickMk3TriggerConfiguration->DisableCounterTriggerCount(i);
            pBrickMk3TriggerConfiguration->DisableCounterStartMode(i);
            pBrickMk3TriggerConfiguration->DisableCounterStartStopMode(i);
            pBrickMk3TriggerConfiguration->DisableCounterLoadZero(i);
            pBrickMk3TriggerConfiguration->SetCounterReloadValue(i, 0xffffffff);

            for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
            {
                pBrickMk3TriggerEventManager->SetCounterInputEvents(i, j,
                    COUNTER_EVENT_NONE);
                pBrickMk3TriggerActionManager->SetCounterOutputActions(i, j,
                    COUNTER_ACTION_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* reset each counter on PXI */
        for (i = 0; i < NUM_COUNTERS_PXI_INTERFACE; ++i)
        {
            pPXITriggerConfiguration->DisableCounterAutoReload(i);
            pPXITriggerConfiguration->DisableCounterTriggerCount(i);
            pPXITriggerConfiguration->DisableCounterStartMode(i);
            pPXITriggerConfiguration->DisableCounterStartStopMode(i);
            pPXITriggerConfiguration->DisableCounterLoadZero(i);
            pPXITriggerConfiguration->SetCounterReloadValue(i, 0xffffffff);

            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                pPXITriggerEventManager->SetCounterInputEvents(i, j,
                    COUNTER_EVENT_NONE);
                pPXITriggerActionManager->SetCounterOutputActions(i, j,
                    COUNTER_ACTION_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* get PXI Router triggering utilities */
        PXIRouterTriggerConfiguration *pPXIRouterTriggerConfiguration =
            (PXIRouterTriggerConfiguration *)pTriggerConfiguration;
        PXIRouterTriggerEventManager *pPXIRouterTriggerEventManager =
            (PXIRouterTriggerEventManager *)pTriggerEventManager;
        PXIRouterTriggerActionManager *pPXIRouterTriggerActionManager =
            (PXIRouterTriggerActionManager *)pTriggerActionManager;

        /* reset each counter on PXI Router */
        for (i = 0; i < NUM_COUNTERS_PXI_ROUTER; ++i)
        {
            pPXIRouterTriggerConfiguration->DisableCounterAutoReload(i);
            pPXIRouterTriggerConfiguration->DisableCounterTriggerCount(i);
            pPXIRouterTriggerConfiguration->DisableCounterStartMode(i);
            pPXIRouterTriggerConfiguration->DisableCounterStartStopMode(i);
            pPXIRouterTriggerConfiguration->DisableCounterLoadZero(i);
            pPXIRouterTriggerConfiguration->SetCounterReloadValue(i,
                0xffffffff);

            for (j = 0; j < NUM_TRIGGERS_PXI_ROUTER; ++j)
            {
                pPXIRouterTriggerEventManager->SetCounterInputEvents(i, j,
                    COUNTER_EVENT_NONE);
                pPXIRouterTriggerActionManager->SetCounterOutputActions(i, j,
                    COUNTER_ACTION_NONE);
            }
        }
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;
}

/**
 * Resets all events and actions on each port for the given device.
 *
 * @param pDevice the device to reset ports for.
 */
void TriggeringUtility::ResetPorts(Device *pDevice)
{
    U32 i, j;

    /* create trigger utilities */
    TriggerConfiguration *pTriggerConfiguration =
        TriggerFactory::CreateTriggerConfiguration(pDevice);
    TriggerEventManager *pTriggerEventManager =
        TriggerFactory::CreateTriggerEventManager(pDevice);
    TriggerActionManager *pTriggerActionManager =
        TriggerFactory::CreateTriggerActionManager(pDevice);

    /* validate trigger utilities */
    if (pTriggerConfiguration == NULL ||
        pTriggerEventManager == NULL ||
        pTriggerActionManager == NULL)
    {
        printf("\n ResetPorts not supported by this device.\n");
        return;
    }

    if (m_triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* get PCIe triggering utilities */
        PCIeTriggerEventManager * pPCIeTriggerEventManager =
            (PCIeTriggerEventManager *)pTriggerEventManager;
        PCIeTriggerActionManager * pPCIeTriggerActionManager =
            (PCIeTriggerActionManager *)pTriggerActionManager;

        /* reset each port on PCIe */
        for (i = 0; i < NUM_PORTS_PCIE; ++i)
        {
            for (j = 0; j < NUM_TRIGGERS_PCIE; ++j)
            {
                pPCIeTriggerEventManager->SetPortInputEvents(i + 1, j,
                    PORT_EVENT_NONE);
                pPCIeTriggerActionManager->SetPortOutputActions(i + 1, j,
                    PORT_ACTION_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* reset each port on Brick Mk3 */
        for (i = 0; i < NUM_PORTS_BRICK_MK3; ++i)
        {
            pBrickMk3TriggerConfiguration->DisablePortTransmitPacketMode(i + 1);

            for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
            {
                pBrickMk3TriggerEventManager->SetPortInputEvents(i + 1, j,
                    PORT_EVENT_NONE);
                pBrickMk3TriggerActionManager->SetPortOutputActions(i + 1, j,
                    PORT_ACTION_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* reset each port on PXI */
        for (i = 0; i < NUM_PORTS_PXI_INTERFACE; ++i)
        {
            pPXITriggerConfiguration->DisablePortTransmitPacketMode(i + 1);

            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                pPXITriggerEventManager->SetPortInputEvents(i + 1, j,
                    PORT_EVENT_NONE);
                pPXITriggerActionManager->SetPortOutputActions(i + 1, j,
                    PORT_ACTION_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* get PXI Router triggering utilities */
        PXIRouterTriggerConfiguration *pPXIRouterTriggerConfiguration =
            (PXIRouterTriggerConfiguration *)pTriggerConfiguration;
        PXIRouterTriggerEventManager *pPXIRouterTriggerEventManager =
            (PXIRouterTriggerEventManager *)pTriggerEventManager;
        PXIRouterTriggerActionManager *pPXIRouterTriggerActionManager =
            (PXIRouterTriggerActionManager *)pTriggerActionManager;

        /* reset each port on PXI */
        for (i = 0; i < NUM_PORTS_PXI_INTERFACE; ++i)
        {
            pPXIRouterTriggerConfiguration->DisablePortTransmitPacketMode(
                i + 1);

            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                pPXIRouterTriggerEventManager->SetPortInputEvents(i + 1, j,
                    PORT_EVENT_NONE);
                pPXIRouterTriggerActionManager->SetPortOutputActions(i + 1, j,
                    PORT_ACTION_NONE);
            }
        }
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;
}

void TriggeringUtility::ResetTimeCodes(Device *pDevice)
{
    U32 i, j;

    /* create trigger utilities */
    TriggerEventManager *pTriggerEventManager =
        TriggerFactory::CreateTriggerEventManager(pDevice);
    TriggerActionManager *pTriggerActionManager =
        TriggerFactory::CreateTriggerActionManager(pDevice);

    /* validate trigger utilities */
    if (pTriggerEventManager == NULL ||
        pTriggerActionManager == NULL)
    {
        printf("\n ResetTimeCodes not supported by this device.\n");
        return;
    }

    if (m_triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* no time-code interface on PCIe */
        return;
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* reset each time-code interface on Brick Mk3 */
        for (i = 0; i < NUM_TIME_CODES_BRICK_MK3; ++i)
        {
            for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
            {
                pBrickMk3TriggerEventManager->SetTimeCodeInputEvents(i, j,
                    TIME_CODE_EVENT_NONE);
                pBrickMk3TriggerActionManager->SetTimeCodeOutputActions(i, j,
                    TIME_CODE_ACTION_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* reset each time-code interface on PXI */
        for (i = 0; i < NUM_TIME_CODES_PXI_INTERFACE; ++i)
        {
            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                pPXITriggerEventManager->SetTimeCodeInputEvents(i, j,
                    TIME_CODE_EVENT_NONE);
                pPXITriggerActionManager->SetCounterOutputActions(i, j,
                    TIME_CODE_ACTION_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* get PXI Router triggering utilities */
        PXIRouterTriggerEventManager *pPXIRouterTriggerEventManager =
            (PXIRouterTriggerEventManager *)pTriggerEventManager;
        PXIRouterTriggerActionManager *pPXIRouterTriggerActionManager =
            (PXIRouterTriggerActionManager *)pTriggerActionManager;

        /* reset each time-code interface on PXI Router */
        for (i = 0; i < NUM_TIME_CODES_PXI_ROUTER; ++i)
        {
            for (j = 0; j < NUM_TRIGGERS_PXI_ROUTER; ++j)
            {
                pPXIRouterTriggerEventManager->SetTimeCodeInputEvents(i, j,
                    TIME_CODE_EVENT_NONE);
                pPXIRouterTriggerActionManager->SetCounterOutputActions(i, j,
                    TIME_CODE_ACTION_NONE);
            }
        }
    }

    /* cleanup resources */
    delete pTriggerEventManager;
    delete pTriggerActionManager;
}

void TriggeringUtility::ResetTriggers(Device *pDevice)
{
    U32 i, j;

    /* create trigger utilities */
    TriggerConfiguration *pTriggerConfiguration =
        TriggerFactory::CreateTriggerConfiguration(pDevice);
    TriggerEventManager *pTriggerEventManager =
        TriggerFactory::CreateTriggerEventManager(pDevice);

    /* validate trigger utilities */
    if (pTriggerConfiguration == NULL ||
        pTriggerEventManager == NULL)
    {
        printf("\n ResetTriggers not supported by this device.\n");
        return;
    }

    if (m_triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* get PCIe triggering utilities */
        PCIeTriggerConfiguration *pPCIeTriggerConfiguration =
            (PCIeTriggerConfiguration *)pTriggerConfiguration;
        PCIeTriggerEventManager *pPCIeTriggerEventManager =
            (PCIeTriggerEventManager *)pTriggerEventManager;

        /* reset each trigger on PCIe */
        for (i = 0; i < NUM_TRIGGERS_PCIE; ++i)
        {
            pPCIeTriggerConfiguration->SetTriggerInputMode(i,
                TRIGGER_INPUT_MODE_OR);
            pPCIeTriggerConfiguration->DisableTrigger(i);
            pPCIeTriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_COUNTER, 0);
            pPCIeTriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_PORT, 0);
            pPCIeTriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_TRIGGER, 0);
            pPCIeTriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_COUNTER, 0);
            pPCIeTriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_PORT, 0);
            pPCIeTriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_TRIGGER, 0);

            for (j = 0; j < NUM_TRIGGERS_PCIE; ++j)
            {
                pPCIeTriggerEventManager->SetTriggerInputEvents(i, j,
                    TRIGGER_EVENT_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;

        /* reset each trigger on Brick Mk3 */
        for (i = 0; i < NUM_TRIGGERS_BRICK_MK3; ++i)
        {
            pBrickMk3TriggerConfiguration->SetTriggerInputMode(i,
                TRIGGER_INPUT_MODE_OR);
            pBrickMk3TriggerConfiguration->DisableTrigger(i);
            pBrickMk3TriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            pBrickMk3TriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_COUNTER, 0);
            pBrickMk3TriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_PORT, 0);
            pBrickMk3TriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_TIME_CODE, 0);
            pBrickMk3TriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_TRIGGER, 0);
            pBrickMk3TriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            pBrickMk3TriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_COUNTER, 0);
            pBrickMk3TriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_PORT, 0);
            pBrickMk3TriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_TIME_CODE, 0);
            pBrickMk3TriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_TRIGGER, 0);

            for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
            {
                pBrickMk3TriggerEventManager->SetTriggerInputEvents(i, j,
                    TRIGGER_EVENT_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;

        /* reset each trigger on PXI */
        for (i = 0; i < NUM_TRIGGERS_PXI_INTERFACE; ++i)
        {
            pPXITriggerConfiguration->SetTriggerInputMode(i,
                TRIGGER_INPUT_MODE_OR);
            pPXITriggerConfiguration->DisableTrigger(i);
            pPXITriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            pPXITriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_COUNTER, 0);
            pPXITriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_PORT, 0);
            pPXITriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_TIME_CODE, 0);
            pPXITriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_TRIGGER, 0);
            pPXITriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            pPXITriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_COUNTER, 0);
            pPXITriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_PORT, 0);
            pPXITriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_TIME_CODE, 0);
            pPXITriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_TRIGGER, 0);

            for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
            {
                pPXITriggerEventManager->SetTriggerInputEvents(i, j,
                    TRIGGER_EVENT_NONE);
            }
        }
    }
    else if (m_triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* get PXI Router triggering utilities */
        PXIRouterTriggerConfiguration *pPXIRouterTriggerConfiguration =
            (PXIRouterTriggerConfiguration *)pTriggerConfiguration;
        PXIRouterTriggerEventManager *pPXIRouterTriggerEventManager =
            (PXIRouterTriggerEventManager *)pTriggerEventManager;

        /* reset each trigger on PXI Router */
        for (i = 0; i < NUM_TRIGGERS_PXI_ROUTER; ++i)
        {
            pPXIRouterTriggerConfiguration->SetTriggerInputMode(i,
                TRIGGER_INPUT_MODE_OR);
            pPXIRouterTriggerConfiguration->DisableTrigger(i);
            pPXIRouterTriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            pPXIRouterTriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_COUNTER, 0);
            pPXIRouterTriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_PORT, 0);
            pPXIRouterTriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_TIME_CODE, 0);
            pPXIRouterTriggerConfiguration->SetTriggerInvertMask(i,
                TRIGGER_TYPE_TRIGGER, 0);
            pPXIRouterTriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_EXT_TRIGGER, 0);
            pPXIRouterTriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_COUNTER, 0);
            pPXIRouterTriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_PORT, 0);
            pPXIRouterTriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_TIME_CODE, 0);
            pPXIRouterTriggerConfiguration->SetTriggerAndMask(i,
                TRIGGER_TYPE_TRIGGER, 0);

            for (j = 0; j < NUM_TRIGGERS_PXI_ROUTER; ++j)
            {
                pPXIRouterTriggerEventManager->SetTriggerInputEvents(i, j,
                    TRIGGER_EVENT_NONE);
            }
        }
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
}

void TriggeringUtility::Reset(Device *pDevice)
{
    /* reset all configuration for triggering API */
    ResetExtTriggers(pDevice);
    ResetCounters(pDevice);
    ResetPorts(pDevice);
    ResetTimeCodes(pDevice);
    ResetTriggers(pDevice);
}

void TriggeringUtility::QueuePackets(Device *pDevice, U32 count)
{
    char buffer[4];
    U32 i;

    /* open channel 1 to transmit out of */
    Channel channel = pDevice->OpenChannel(Channel::DIRECTION_OUT, 1, 1);
    if(!channel.IsOpen())
    {
        return;
    }

    /* transmit given number of packets */
    for (i = 0; i < count; ++i)
    {
        memset(buffer, i, 4);
        channel.TransmitPacket(buffer, 4, Packet::EOP_TYPE_EOP, -1);
    }

    /* close the channel */
    channel.CloseChannel();
}

