/**
 * \file triggering_example.cpp
 *
 * \brief Implementation of class which provides an example of the Triggering
 *        API.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Implementation of class which provides an example of the C++ version of the
 * Triggering API.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 */

#include "triggering_example.h"

#include "device_list.hpp"

#include "trigger_factory.hpp"

#include "triggering_types.h"
#include "triggering_utility.h"

/* Define counter frequency */
const U32 CYCLES_PER_SECOND = 60000000;

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::triggering;
using namespace stardundee::com::starsystem::triggering::pcie;
using namespace stardundee::com::starsystem::triggering::brickmk3;
using namespace stardundee::com::starsystem::triggering::pxi;

/**
 * Enum containing the different possible menu choices for the application.
 */
typedef enum
{
    MENU_CHOICE_TX_TC_ON_EXT_TRIGGER = 1,
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
 * Prompts the user and returns the menu choice that the user selected.
 *
 * @return selected menu choice value.
 */
MENU_CHOICE GetMenuChoice(void)
{
    char buffer[32];
    unsigned int choice;

    /* show menu to user */
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

    /* validate chosen number */
    if (!fgets(buffer, 32, stdin)) return MENU_CHOICE_INVALID;

    /* get menu choice value  */
    if (!sscanf(buffer, "%u", &choice) ||
        choice >= MENU_CHOICE_INVALID) return MENU_CHOICE_INVALID;

    /* return the chosen value */
    return (MENU_CHOICE)choice;
}

/**
 * Gets the first device of requested type and returns the device instance or
 * asks the user to choose one if more than one device exists.
 *
 * @return device instance representing selected device.
 */
static Device * GetDevice()
{
    char buffer[256];
    unsigned int chosen;
    int status;

    /* list devices that support triggering API */
    U32 deviceTypes[5] = { STAR_DEVICE_BRICK_MK3,
                           STAR_DEVICE_PXI_INTERFACE,
                           STAR_DEVICE_PXI_RMAP,
                           STAR_DEVICE_PXI_ROUTER_12,
                           STAR_DEVICE_PCIE };

    /* populate device list with all devices of specified type */
    DeviceList deviceList;
    U32 deviceCount = deviceList.PopulateForTypes(deviceTypes, 5);

    /* initialise selected device to null */
    Device *pSelectedDevice = NULL;

    /* if more than one device is available */
    if (deviceCount > 1)
    {
        U32 x;

        printf("The following devices are available that support triggering:"
            "\n\n");

        /* for each device */
        for (x = 0; x < deviceCount; x++)
        {
            Device *pDevice = deviceList[x];

            /* display its name */
            if (pDevice->GetDeviceType() != STAR_DEVICE_UNKNOWN)
            {
                char deviceName[STAR_SPW_DEVICE_NAME_MAX_LENGTH];
                char deviceSerial[STAR_SPW_DEVICE_SERIAL_NUMBER_MAX_LENGTH];

                size_t deviceNameLength = pDevice->GetDeviceName(deviceName);
                size_t deviceSerialLength = pDevice->GetDeviceSerialNumber(
                    deviceSerial);

                if (deviceNameLength > 0 && deviceSerialLength > 0)
                {
                    printf("\t%u - %s with serial number %s\n", x, deviceName,
                        deviceSerial);
                }
                else
                {
                    printf("\t%u - Unknown SpaceWire Device\n", x);
                }
            }
            else
            {
                printf("\t%u - Unable to access device\n", x);
            }
        }

        printf("\n");

        /* ask the user which device to use */
        do
        {
            printf("Please select which device to use: ");
            fflush(stdout);

            /* read selected device number */
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

            /* get the selected device */
            pSelectedDevice = deviceList.CopyDeviceAt(chosen);
        }
        while (pSelectedDevice == NULL);
    }
    /* else if just one device is available */
    else if (deviceCount == 1)
    {
        /* select the first device */
        pSelectedDevice = deviceList.CopyDeviceAt(0);
    }

    /* if valid device was selected */
    if (pSelectedDevice != NULL)
    {
        char deviceName[STAR_SPW_DEVICE_NAME_MAX_LENGTH];
        char deviceSerial[STAR_SPW_DEVICE_SERIAL_NUMBER_MAX_LENGTH];

        /* get device name and serial */
        pSelectedDevice->GetDeviceName(deviceName);
        pSelectedDevice->GetDeviceSerialNumber(deviceSerial);

        /* print selected device */
        printf("%s selected with serial number %s\n\n", deviceName,
            deviceSerial);
    }

    /* return the selected device */
    return pSelectedDevice;
}

/**
 * Returns the corresponding triggering device type for the given STAR-System
 * device type.
 *
 * @param deviceType the device type to get triggering device type for.
 *
 * @return triggering device type.
 */
TRIGGER_DEVICE GetTriggerDeviceType(U32 deviceType)
{
    if (deviceType == STAR_DEVICE_BRICK_MK3)
    {
        return TRIGGER_DEVICE_BRICK_MK3;
    }
    else if ((deviceType == STAR_DEVICE_PXI_INTERFACE) ||
        (deviceType == STAR_DEVICE_PXI_RMAP))
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
 * Perform transmit time-code on external trigger activity.
 *
 * @param pDevice the device to setup triggering for.
 */
void TxTcOnExtTrigger(Device *pDevice)
{
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    if (triggerDevice == TRIGGER_DEVICE_PCIE_IF ||
        triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);

    triggeringUtility.Reset(pDevice);
    printf("\n----- Trigger Example (Transmit time-code on external trigger) "
        "-----\n");

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
        printf("\n Test not supported by this device.\n");
        return;
    }

    if (triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* enable external trigger 0 edge detect mode */
        pBrickMk3TriggerConfiguration->EnableExtTriggerEdgeDetectMode(0);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal
         * trigger 0 to be set
         */
        pBrickMk3TriggerEventManager->SetExtTriggerInputEvents(0, 0,
            EXT_TRIGGER_EVENT_IN);

        /* internal trigger 0 causes TIME_CODE_ACTION_RX action on external
         * trigger 0
         */
        pBrickMk3TriggerActionManager->SetTimeCodeOutputActions(0, 0,
            TIME_CODE_ACTION_TX);

        /* enable internal trigger 0 */
        pBrickMk3TriggerConfiguration->EnableTrigger(0);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* enable external trigger 0 edge detect mode */
        pPXITriggerConfiguration->EnableExtTriggerEdgeDetectMode(0);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal
         * trigger 0 to be set
         */
        pPXITriggerEventManager->SetExtTriggerInputEvents(0, 0,
            EXT_TRIGGER_EVENT_IN);

        /* internal trigger 0 causes TIME_CODE_ACTION_RX action on external
         * trigger 0
         */
        pPXITriggerActionManager->SetTimeCodeOutputActions(0, 0,
            TIME_CODE_ACTION_TX);

        /* enable internal trigger 0 */
        pPXITriggerConfiguration->EnableTrigger(0);
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;

    printf("Time-codes will now be transmitted when external trigger 0 is "
        "pulsed.\n\n");
}

/**
 * Perform transmit time-code when counter changes.
 *
 * @param pDevice the device to setup triggering for.
 */
void TxTcOnCounter(Device *pDevice)
{
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    if (triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);

    triggeringUtility.Reset(pDevice);
    printf("\n----- Trigger Example (Transmit time-code on timer) -----\n");

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
        printf("\n Test not supported by this device.\n");
        return;
    }

    if (triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* set timer 0 reload value to 1 second */
        pBrickMk3TriggerConfiguration->SetCounterReloadValue(0,
            CYCLES_PER_SECOND);

        /* enable timer auto reload */
        pBrickMk3TriggerConfiguration->EnableCounterAutoReload(0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to
           be set
           */
        pBrickMk3TriggerEventManager->SetCounterInputEvents(0, 0,
            COUNTER_EVENT_RELOAD);

        /* internal trigger 0 causes TIME_CODE_ACTION_TX action on time-code
           engine 0
           */
        pBrickMk3TriggerActionManager->SetTimeCodeOutputActions(0, 0,
            TIME_CODE_ACTION_TX);

        /* enable internal trigger 0 */
        pBrickMk3TriggerConfiguration->EnableTrigger(0);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* set timer 0 reload value to 1 second */
        pPXITriggerConfiguration->SetCounterReloadValue(0, CYCLES_PER_SECOND);

        /* enable timer auto reload */
        pPXITriggerConfiguration->EnableCounterAutoReload(0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to
           be set
           */
        pPXITriggerEventManager->SetCounterInputEvents(0, 0,
            COUNTER_EVENT_RELOAD);

        /* internal trigger 0 causes TIME_CODE_ACTION_TX action on time-code
           engine 0
           */
        pPXITriggerActionManager->SetTimeCodeOutputActions(0, 0,
            TIME_CODE_ACTION_TX);

        /* enable internal trigger 0 */
        pPXITriggerConfiguration->EnableTrigger(0);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* get PXI Router triggering utilities */
        PXIRouterTriggerConfiguration *pPXIRouterTriggerConfiguration =
            (PXIRouterTriggerConfiguration *)pTriggerConfiguration;
        PXIRouterTriggerEventManager *pPXIRouterTriggerEventManager =
            (PXIRouterTriggerEventManager *)pTriggerEventManager;
        PXIRouterTriggerActionManager *pPXIRouterTriggerActionManager =
            (PXIRouterTriggerActionManager *)pTriggerActionManager;

        /* set timer 0 reload value to 1 second */
        pPXIRouterTriggerConfiguration->SetCounterReloadValue(0,
            CYCLES_PER_SECOND);

        /* enable timer auto reload */
        pPXIRouterTriggerConfiguration->EnableCounterAutoReload(0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to
           be set
           */
        pPXIRouterTriggerEventManager->SetCounterInputEvents(0, 0,
            COUNTER_EVENT_RELOAD);

        /* internal trigger 0 causes TIME_CODE_ACTION_TX action on time-code
           engine 0
           */
        pPXIRouterTriggerActionManager->SetTimeCodeOutputActions(0, 0,
            TIME_CODE_ACTION_TX);

        /* enable internal trigger 0 */
        pPXIRouterTriggerConfiguration->EnableTrigger(0);
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;

    printf("Time-codes will now be transmitted every second.\n\n");
}

/**
 * Perform transmit packet on external trigger activity.
 *
 * @param pDevice the device to setup triggering for.
 */
void PktTxOnExtTrigger(Device *pDevice)
{
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    if (triggerDevice == TRIGGER_DEVICE_PCIE_IF ||
        triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);

    triggeringUtility.Reset(pDevice);
    printf("\n----- Trigger Example (Packet transmit on external trigger) "
        "-----\n");

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
        printf("\n Test not supported by this device.\n");
        return;
    }

    if (triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* enable external trigger 0 edge detect mode */
        pBrickMk3TriggerConfiguration->EnableExtTriggerEdgeDetectMode(0);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal
         * trigger 0 to be set
         */
        pBrickMk3TriggerEventManager->SetExtTriggerInputEvents(0, 0,
            EXT_TRIGGER_EVENT_IN);

        /* enable port transmit packet mode on port 1 */
        pBrickMk3TriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pBrickMk3TriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 8);

        /* enable internal trigger 0 */
        pBrickMk3TriggerConfiguration->EnableTrigger(0);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;

        /* enable external trigger 0 edge detect mode */
        pPXITriggerConfiguration->EnableExtTriggerEdgeDetectMode(0);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal
         * trigger 0 to be set
         */
        pPXITriggerEventManager->SetExtTriggerInputEvents(0, 0,
            EXT_TRIGGER_EVENT_IN);

        /* enable port transmit packet mode on port 1 */
        pPXITriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pTriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 8);

        /* enable internal trigger 0 */
        pPXITriggerConfiguration->EnableTrigger(0);
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;

    printf("8 packets have been queued and will now be transmitted on port 1 "
        "when external trigger 0 is pulsed.\n\n");
}

/**
 * Perform transmit packet when counter changes.
 *
 * @param pDevice the device to setup triggering for.
 */
void PktTxOnCounter(Device *pDevice)
{
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);

    triggeringUtility.Reset(pDevice);
    printf("\n----- Trigger Example (Packet transmit on timer) -----\n");

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
        printf("\n Test not supported by this device.\n");
        return;
    }

    if (triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* set timer 0 reload value to 1 second */
        pBrickMk3TriggerConfiguration->SetCounterReloadValue(0,
            CYCLES_PER_SECOND);

        /* enable timer auto reload */
        pBrickMk3TriggerConfiguration->EnableCounterAutoReload(0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to
         * be set
         */
        pBrickMk3TriggerEventManager->SetCounterInputEvents(0, 0,
            COUNTER_EVENT_RELOAD);

        /* enable port transmit packet mode on port 1 */
        pBrickMk3TriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pBrickMk3TriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 8);

        /* Enable internal trigger 0 */
        pBrickMk3TriggerConfiguration->EnableTrigger(0);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* set timer 0 reload value to 1 second */
        pPXITriggerConfiguration->SetCounterReloadValue(0, CYCLES_PER_SECOND);

        /* enable timer auto reload */
        pPXITriggerConfiguration->EnableCounterAutoReload(0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to
         * be set
         */
        pPXITriggerEventManager->SetCounterInputEvents(0, 0,
            COUNTER_EVENT_RELOAD);

        /* enable port transmit packet mode on port 1 */
        pPXITriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPXITriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 8);

        /* enable internal trigger 0 */
        pPXITriggerConfiguration->EnableTrigger(0);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* get PXI Router triggering utilities */
        PXIRouterTriggerConfiguration *pPXIRouterTriggerConfiguration =
            (PXIRouterTriggerConfiguration *)pTriggerConfiguration;
        PXIRouterTriggerEventManager *pPXIRouterTriggerEventManager =
            (PXIRouterTriggerEventManager *)pTriggerEventManager;
        PXIRouterTriggerActionManager *pPXIRouterTriggerActionManager =
            (PXIRouterTriggerActionManager *)pTriggerActionManager;

        /* set timer 0 reload value to 1 second */
        pPXIRouterTriggerConfiguration->SetCounterReloadValue(0,
            CYCLES_PER_SECOND);

        /* enable timer auto reload */
        pPXIRouterTriggerConfiguration->EnableCounterAutoReload(0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to
         * be set
         */
        pPXIRouterTriggerEventManager->SetCounterInputEvents(0, 0,
            COUNTER_EVENT_RELOAD);

        /* enable port transmit packet mode on port 1 */
        pPXIRouterTriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPXIRouterTriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 8);

        /* enable internal trigger 0 */
        pPXIRouterTriggerConfiguration->EnableTrigger(0);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* get PCIe triggering utilities */
        PCIeTriggerConfiguration *pPCIeTriggerConfiguration =
            (PCIeTriggerConfiguration *)pTriggerConfiguration;
        PCIeTriggerEventManager *pPCIeTriggerEventManager =
            (PCIeTriggerEventManager *)pTriggerEventManager;
        PCIeTriggerActionManager *pPCIeTriggerActionManager =
            (PCIeTriggerActionManager *)pTriggerActionManager;

        /* set timer 0 reload value to 1 second */
        pPCIeTriggerConfiguration->SetCounterReloadValue(0, CYCLES_PER_SECOND);

        /* enable timer auto reload */
        pPCIeTriggerConfiguration->EnableCounterAutoReload(0);

        /* COUNTER_EVENT_RELOAD event on timer 0 causes internal trigger 0 to
         * be set
         */
        pPCIeTriggerEventManager->SetCounterInputEvents(0, 0,
            COUNTER_EVENT_RELOAD);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPCIeTriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 8);

        /* enable internal trigger 0 */
        pPCIeTriggerConfiguration->EnableTrigger(0);
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;

    printf("8 packets have been queued and will now be transmitted every "
        "second.\n\n");
}

/**
 * Perform transmit packet when time-code is received.
 *
 * @param pDevice the device to setup triggering for.
 */
void PktTxOnTc(Device *pDevice)
{
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    if (triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);

    triggeringUtility.Reset(pDevice);
    printf("\n----- Trigger Example (Packet transmit on time-code) -----\n");

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
        printf("\n Test not supported by this device.\n");
        return;
    }

    if (triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal
         * trigger 0 to be set
         */
        pBrickMk3TriggerEventManager->SetTimeCodeInputEvents(0, 0,
            TIME_CODE_EVENT_TICK);

        /* Enable port transmit packet mode on port 1 */
        pBrickMk3TriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pBrickMk3TriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 8);

        /* enable internal trigger 0 */
        pBrickMk3TriggerConfiguration->EnableTrigger(0);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal
         * trigger 0 to be set
         */
        pPXITriggerEventManager->SetTimeCodeInputEvents(0, 0,
            TIME_CODE_EVENT_TICK);

        /* enable port transmit packet mode on port 1 */
        pPXITriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPXITriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 8);

        /* enable internal trigger 0 */
        pPXITriggerConfiguration->EnableTrigger(0);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* get PXI Router triggering utilities */
        PXIRouterTriggerConfiguration *pPXIRouterTriggerConfiguration =
            (PXIRouterTriggerConfiguration *)pTriggerConfiguration;
        PXIRouterTriggerEventManager *pPXIRouterTriggerEventManager =
            (PXIRouterTriggerEventManager *)pTriggerEventManager;
        PXIRouterTriggerActionManager *pPXIRouterTriggerActionManager =
            (PXIRouterTriggerActionManager *)pTriggerActionManager;

        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal
         * trigger 0 to be set
         */
        pPXIRouterTriggerEventManager->SetTimeCodeInputEvents(0, 0,
            TIME_CODE_EVENT_TICK);

        /* enable port transmit packet mode on port 1 */
        pPXIRouterTriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPXIRouterTriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 8);

        /* enable internal trigger 0 */
        pPXIRouterTriggerConfiguration->EnableTrigger(0);
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;

    printf("8 packets have been queued and will now be transmitted when "
        "time-codes are received.\n\n");
}

/**
 * Perform transmit multiple packets on external trigger activity.
 *
 * @param pDevice the device to setup triggering for.
 */
void MultiplePktTxOnExtTrigger(Device *pDevice)
{
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    if (triggerDevice == TRIGGER_DEVICE_PCIE_IF ||
        triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);

    triggeringUtility.Reset(pDevice);
    printf("\n----- Trigger Example (Multiple packet transmit on external "
        "trigger) -----\n");

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
        printf("\n Test not supported by this device.\n");
        return;
    }

    if (triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* enable timer trigger count on timer 0 so the timer only counts on
         * triggers
         */
        pBrickMk3TriggerConfiguration->EnableCounterTriggerCount(0);

        /* set timer 0 reload value to 3 */
        pBrickMk3TriggerConfiguration->SetCounterReloadValue(0, 3);

        /* enable external trigger 0 edge detect mode */
        pBrickMk3TriggerConfiguration->EnableExtTriggerEdgeDetectMode(0);

        /* enable port transmit packet mode on port 1 */
        pBrickMk3TriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal
         * trigger 0 to be set
         */
        pBrickMk3TriggerEventManager->SetExtTriggerInputEvents(0, 0,
            EXT_TRIGGER_EVENT_IN);

        /* external trigger 0 causes COUNTER_ACTION_RELOAD action on timer 0 */
        pBrickMk3TriggerActionManager->SetCounterOutputActions(0, 0,
            COUNTER_ACTION_RELOAD);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pBrickMk3TriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* PORT_EVENT_TX_EOP event causes internal trigger 1 to be set */
        pBrickMk3TriggerEventManager->SetPortInputEvents(1, 1,
            PORT_EVENT_TX_EOP);

        /* internal trigger 1 causes COUNTER_ACTION_COUNT action on timer 0 */
        pBrickMk3TriggerActionManager->SetCounterOutputActions(0, 1,
            COUNTER_ACTION_COUNT);

        /* COUNTER_EVENT_COUNT event on timer 0 causes internal trigger 2 to be
         * set
         */
        pBrickMk3TriggerEventManager->SetCounterInputEvents(0, 2,
            COUNTER_EVENT_COUNT);

        /* internal trigger 2 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pBrickMk3TriggerActionManager->SetPortOutputActions(1, 2,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 32);

        /* enable internal triggers 0, 1 and 2 */
        pBrickMk3TriggerConfiguration->EnableTrigger(0);
        pBrickMk3TriggerConfiguration->EnableTrigger(1);
        pBrickMk3TriggerConfiguration->EnableTrigger(2);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* enable timer trigger count on timer 0 so the timer only counts on
         * triggers
         */
        pPXITriggerConfiguration->EnableCounterTriggerCount(0);

        /* set timer 0 reload value to 3 */
        pPXITriggerConfiguration->SetCounterReloadValue(0, 3);

        /* enable external trigger 0 edge detect mode */
        pPXITriggerConfiguration->EnableExtTriggerEdgeDetectMode(0);

        /* enable port transmit packet mode on port 1 */
        pPXITriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* EXT_TRIGGER_EVENT_IN event on external trigger 0 causes internal
         * trigger 0 to be set
         */
        pPXITriggerEventManager->SetExtTriggerInputEvents(0, 0,
            EXT_TRIGGER_EVENT_IN);

        /* external trigger 0 causes COUNTER_ACTION_RELOAD action on timer 0 */
        pPXITriggerActionManager->SetCounterOutputActions(0, 0,
            COUNTER_ACTION_RELOAD);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPXITriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* PORT_EVENT_TX_EOP event causes internal trigger 1 to be set */
        pPXITriggerEventManager->SetPortInputEvents(1, 1, PORT_EVENT_TX_EOP);

        /* internal trigger 1 causes COUNTER_ACTION_COUNT action on timer 0 */
        pPXITriggerActionManager->SetCounterOutputActions(0, 1,
            COUNTER_ACTION_COUNT);

        /* COUNTER_EVENT_COUNT event on timer 0 causes internal trigger 2 to be
         * set
         */
        pPXITriggerEventManager->SetCounterInputEvents(0, 2,
            COUNTER_EVENT_COUNT);

        /* internal trigger 2 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPXITriggerActionManager->SetPortOutputActions(1, 2,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 32);

        /* enable internal triggers 0, 1 and 2 */
        pPXITriggerConfiguration->EnableTrigger(0);
        pPXITriggerConfiguration->EnableTrigger(1);
        pPXITriggerConfiguration->EnableTrigger(2);
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;

    printf("32 packets have been queued and will now be transmitted in groups "
        "of four on port 1 when external trigger 0 is pulsed.\n\n");
}

/**
 * Perform transmit multiple packets when a time-code is received.
 *
 * @param pDevice the device to setup triggering for.
 */
void MultiplePktTxOnTc(Device *pDevice)
{
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    if (triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);

    triggeringUtility.Reset(pDevice);
    printf("\n----- Trigger Example (Multiple packet transmit on time-code) "
        "-----\n");

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
        printf("\n Test not supported by this device.\n");
        return;
    }

    if (triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* enable timer trigger count on timer 0 so the timer only counts on
         * triggers
         */
        pBrickMk3TriggerConfiguration->EnableCounterTriggerCount(0);

        /* set timer 0 reload value to 3 */
        pBrickMk3TriggerConfiguration->SetCounterReloadValue(0, 3);

        /* enable port transmit packet mode on port 1 */
        pBrickMk3TriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal
         * trigger 0 to be set
         */
        pBrickMk3TriggerEventManager->SetTimeCodeInputEvents(0, 0,
            TIME_CODE_EVENT_TICK);

        /* internal trigger 0 causes COUNTER_ACTION_RELOAD action on timer 0 */
        pBrickMk3TriggerActionManager->SetCounterOutputActions(0, 0,
            COUNTER_ACTION_RELOAD);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pBrickMk3TriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* PORT_EVENT_TX_EOP event causes internal trigger 1 to be set */
        pBrickMk3TriggerEventManager->SetPortInputEvents(1, 1,
            PORT_EVENT_TX_EOP);

        /* internal trigger 1 causes COUNTER_ACTION_COUNT action on timer 0 */
        pBrickMk3TriggerActionManager->SetCounterOutputActions(0, 1,
            COUNTER_ACTION_COUNT);

        /* COUNTER_EVENT_COUNT event on timer 0 causes internal trigger 2 to be
         * set
         */
        pBrickMk3TriggerEventManager->SetCounterInputEvents(0, 2,
            COUNTER_EVENT_COUNT);

        /* internal trigger 2 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pBrickMk3TriggerActionManager->SetPortOutputActions(1, 2,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 32);

        /* enable internal triggers 0, 1 and 2 */
        pBrickMk3TriggerConfiguration->EnableTrigger(0);
        pBrickMk3TriggerConfiguration->EnableTrigger(1);
        pBrickMk3TriggerConfiguration->EnableTrigger(2);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* enable timer trigger count on timer 0 so the timer only counts on
         * triggers
         */
        pPXITriggerConfiguration->EnableCounterTriggerCount(0);

        /* set timer 0 reload value to 3 */
        pPXITriggerConfiguration->SetCounterReloadValue(0, 3);

        /* enable port transmit packet mode on port 1 */
        pPXITriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal
         * trigger 0 to be set
         */
        pPXITriggerEventManager->SetTimeCodeInputEvents(0, 0,
            TIME_CODE_EVENT_TICK);

        /* internal trigger 0 causes COUNTER_ACTION_RELOAD action on timer 0 */
        pPXITriggerActionManager->SetCounterOutputActions(0, 0,
            COUNTER_ACTION_RELOAD);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPXITriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* PORT_EVENT_TX_EOP event causes internal trigger 1 to be set */
        pPXITriggerEventManager->SetPortInputEvents(1, 1,
            PORT_EVENT_TX_EOP);

        /* internal trigger 1 causes COUNTER_ACTION_COUNT action on timer 0 */
        pPXITriggerActionManager->SetCounterOutputActions(0, 1,
            COUNTER_ACTION_COUNT);

        /* COUNTER_EVENT_COUNT event on timer 0 causes internal trigger 2 to be
         * set
         */
        pPXITriggerEventManager->SetCounterInputEvents(0, 2,
            COUNTER_EVENT_COUNT);

        /* internal trigger 2 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPXITriggerActionManager->SetPortOutputActions(1, 2,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 32);

        /* enable internal triggers 0, 1 and 2 */
        pPXITriggerConfiguration->EnableTrigger(0);
        pPXITriggerConfiguration->EnableTrigger(1);
        pPXITriggerConfiguration->EnableTrigger(2);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* get PXI Router triggering utilities */
        PXIRouterTriggerConfiguration *pPXIRouterTriggerConfiguration =
            (PXIRouterTriggerConfiguration *)pTriggerConfiguration;
        PXIRouterTriggerEventManager *pPXIRouterTriggerEventManager =
            (PXIRouterTriggerEventManager *)pTriggerEventManager;
        PXIRouterTriggerActionManager *pPXIRouterTriggerActionManager =
            (PXIRouterTriggerActionManager *)pTriggerActionManager;

        /* enable timer trigger count on timer 0 so the timer only counts on
         * triggers
         */
        pPXIRouterTriggerConfiguration->EnableCounterTriggerCount(0);

        /* set timer 0 reload value to 3 */
        pPXIRouterTriggerConfiguration->SetCounterReloadValue(0, 3);

        /* enable port transmit packet mode on port 1 */
        pPXIRouterTriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* TIME_CODE_EVENT_TICK event on time-code engine 0 causes internal
         * trigger 0 to be set
         */
        pPXIRouterTriggerEventManager->SetTimeCodeInputEvents(0, 0,
            TIME_CODE_EVENT_TICK);

        /* internal trigger 0 causes COUNTER_ACTION_RELOAD action on timer 0 */
        pPXIRouterTriggerActionManager->SetCounterOutputActions(0, 0,
            COUNTER_ACTION_RELOAD);

        /* internal trigger 0 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPXIRouterTriggerActionManager->SetPortOutputActions(1, 0,
            PORT_ACTION_TRANSMIT_PKT);

        /* PORT_EVENT_TX_EOP event causes internal trigger 1 to be set */
        pPXIRouterTriggerEventManager->SetPortInputEvents(1, 1,
            PORT_EVENT_TX_EOP);

        /* internal trigger 1 causes COUNTER_ACTION_COUNT action on timer 0 */
        pPXIRouterTriggerActionManager->SetCounterOutputActions(0, 1,
            COUNTER_ACTION_COUNT);

        /* COUNTER_EVENT_COUNT event on timer 0 causes internal trigger 2 to be
         * set
         */
        pPXIRouterTriggerEventManager->SetCounterInputEvents(0, 2,
            COUNTER_EVENT_COUNT);

        /* internal trigger 2 causes PORT_ACTION_TRANSMIT_PKT action on
         * port 1
         */
        pPXIRouterTriggerActionManager->SetPortOutputActions(1, 2,
            PORT_ACTION_TRANSMIT_PKT);

        /* queue up some packets */
        triggeringUtility.QueuePackets(pDevice, 32);

        /* enable internal triggers 0, 1 and 2 */
        pPXIRouterTriggerConfiguration->EnableTrigger(0);
        pPXIRouterTriggerConfiguration->EnableTrigger(1);
        pPXIRouterTriggerConfiguration->EnableTrigger(2);
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;

    printf("32 packets have been queued and will now be transmitted in groups "
        "of four on port 1 when time-codes are received.\n\n");
}

/**
 * Perform timed packet transmit when a time-code is received.
 *
 * @param pDevice the device to setup triggering for.
 */
void TimedPktTxOnTc(Device *pDevice)
{
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    if (triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);

    triggeringUtility.Reset(pDevice);
    printf("\n----- Trigger Example (Timed Packet Transmit on Time-Code) "
        "-----\n");

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
        printf("\n Test not supported by this device.\n");
        return;
    }

    if (triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 triggering utilities */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
        BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager =
            (BrickMk3TriggerEventManager *)pTriggerEventManager;
        BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager =
            (BrickMk3TriggerActionManager *)pTriggerActionManager;

        /* setup timer 0 to transmit time-codes every second and reload/start
         * timer 1
         */
        pBrickMk3TriggerConfiguration->SetCounterReloadValue(0,
            CYCLES_PER_SECOND);
        pBrickMk3TriggerConfiguration->EnableCounterAutoReload(0);
        pBrickMk3TriggerEventManager->SetCounterInputEvents(0, 0,
            COUNTER_EVENT_RELOAD);
        pBrickMk3TriggerActionManager->SetTimeCodeOutputActions(0, 0,
            TIME_CODE_ACTION_TX);
        pBrickMk3TriggerActionManager->SetCounterOutputActions(1, 0,
            COUNTER_ACTION_RELOAD);
        pBrickMk3TriggerActionManager->SetCounterOutputActions(1, 0,
            COUNTER_ACTION_START);

        /* setup timer 1 to expire every 200 ms and enable start/stop mode */
        pBrickMk3TriggerConfiguration->SetCounterReloadValue(1,
            CYCLES_PER_SECOND / 5);
        pBrickMk3TriggerConfiguration->EnableCounterAutoReload(1);
        pBrickMk3TriggerConfiguration->EnableCounterStartStopMode(1);

        /* setup timer 1 to decrement timer 2 (packet counter timer) */
        pBrickMk3TriggerEventManager->SetCounterInputEvents(1, 1,
            COUNTER_EVENT_ZERO_SINGLE);
        pBrickMk3TriggerActionManager->SetCounterOutputActions(2, 1,
            COUNTER_ACTION_COUNT);

        /* setup timer 2 to count from 4 to 0 and enable trigger count mode */
        pBrickMk3TriggerConfiguration->SetCounterReloadValue(2, 4);
        pBrickMk3TriggerConfiguration->EnableCounterAutoReload(2);
        pBrickMk3TriggerConfiguration->EnableCounterTriggerCount(2);

        /* setup timer 2 to transmit packets */
        pBrickMk3TriggerEventManager->SetCounterInputEvents(2, 2,
            COUNTER_EVENT_COUNT);
        pBrickMk3TriggerActionManager->SetPortOutputActions(1, 2,
            PORT_ACTION_TRANSMIT_PKT);

        /* setup timer 2 to stop timer 1 when it hits 0 */
        pBrickMk3TriggerEventManager->SetCounterInputEvents(2, 3,
            COUNTER_EVENT_ZERO_SINGLE);
        pBrickMk3TriggerActionManager->SetCounterOutputActions(1, 3,
            COUNTER_ACTION_STOP);

        /* enable port transmit packet mode on port 1 */
        pBrickMk3TriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* reload timers */
        pBrickMk3TriggerConfiguration->ForceCounterReload(0);
        pBrickMk3TriggerConfiguration->ForceCounterReload(1);
        pBrickMk3TriggerConfiguration->ForceCounterReload(2);

        /* enable internal triggers 0-3 */
        pBrickMk3TriggerConfiguration->EnableTrigger(0);
        pBrickMk3TriggerConfiguration->EnableTrigger(1);
        pBrickMk3TriggerConfiguration->EnableTrigger(2);
        pBrickMk3TriggerConfiguration->EnableTrigger(3);

        triggeringUtility.QueuePackets(pDevice, 32);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI triggering utilities */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;
        PXITriggerEventManager *pPXITriggerEventManager =
            (PXITriggerEventManager *)pTriggerEventManager;
        PXITriggerActionManager *pPXITriggerActionManager =
            (PXITriggerActionManager *)pTriggerActionManager;

        /* setup timer 0 to transmit time-codes every second and reload/start
         * timer 1
         */
        pPXITriggerConfiguration->SetCounterReloadValue(0, CYCLES_PER_SECOND);
        pPXITriggerConfiguration->EnableCounterAutoReload(0);
        pPXITriggerEventManager->SetCounterInputEvents(0, 0,
            COUNTER_EVENT_RELOAD);
        pPXITriggerActionManager->SetTimeCodeOutputActions(0, 0,
            TIME_CODE_ACTION_TX);
        pPXITriggerActionManager->SetCounterOutputActions(1, 0,
            COUNTER_ACTION_RELOAD);
        pPXITriggerActionManager->SetCounterOutputActions(1, 0,
            COUNTER_ACTION_START);

        /* setup timer 1 to expire every 200 ms and enable start/stop mode */
        pPXITriggerConfiguration->SetCounterReloadValue(1,
            CYCLES_PER_SECOND / 5);
        pPXITriggerConfiguration->EnableCounterAutoReload(1);
        pPXITriggerConfiguration->EnableCounterStartStopMode(1);

        /* setup timer 1 to decrement timer 2 (packet counter timer) */
        pPXITriggerEventManager->SetCounterInputEvents(1, 1,
            COUNTER_EVENT_ZERO_SINGLE);
        pPXITriggerActionManager->SetCounterOutputActions(2, 1,
            COUNTER_ACTION_COUNT);

        /* setup timer 2 to count from 4 to 0 and enable trigger count mode */
        pPXITriggerConfiguration->SetCounterReloadValue(2, 4);
        pPXITriggerConfiguration->EnableCounterAutoReload(2);
        pPXITriggerConfiguration->EnableCounterTriggerCount(2);

        /* setup timer 2 to transmit packets */
        pPXITriggerEventManager->SetCounterInputEvents(2, 2,
            COUNTER_EVENT_COUNT);
        pPXITriggerActionManager->SetPortOutputActions(1, 2,
            PORT_ACTION_TRANSMIT_PKT);

        /* setup timer 2 to stop timer 1 when it hits 0 */
        pPXITriggerEventManager->SetCounterInputEvents(2, 3,
            COUNTER_EVENT_ZERO_SINGLE);
        pPXITriggerActionManager->SetCounterOutputActions(1, 3,
            COUNTER_ACTION_STOP);

        /* enable port transmit packet mode on port 1 */
        pPXITriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* reload timers */
        pPXITriggerConfiguration->ForceCounterReload(0);
        pPXITriggerConfiguration->ForceCounterReload(1);
        pPXITriggerConfiguration->ForceCounterReload(2);

        /* enable internal triggers 0-3 */
        pPXITriggerConfiguration->EnableTrigger(0);
        pPXITriggerConfiguration->EnableTrigger(1);
        pPXITriggerConfiguration->EnableTrigger(2);
        pPXITriggerConfiguration->EnableTrigger(3);

        triggeringUtility.QueuePackets(pDevice, 32);
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* get PXI Router triggering utilities */
        PXIRouterTriggerConfiguration *pPXIRouterTriggerConfiguration =
            (PXIRouterTriggerConfiguration *)pTriggerConfiguration;
        PXIRouterTriggerEventManager *pPXIRouterTriggerEventManager =
            (PXIRouterTriggerEventManager *)pTriggerEventManager;
        PXIRouterTriggerActionManager *pPXIRouterTriggerActionManager =
            (PXIRouterTriggerActionManager *)pTriggerActionManager;

        /* setup timer 0 to transmit time-codes every second and reload/start
         * timer 1
         */
        pPXIRouterTriggerConfiguration->SetCounterReloadValue(0,
            CYCLES_PER_SECOND);
        pPXIRouterTriggerConfiguration->EnableCounterAutoReload(0);
        pPXIRouterTriggerEventManager->SetCounterInputEvents(0, 0,
            COUNTER_EVENT_RELOAD);
        pPXIRouterTriggerActionManager->SetTimeCodeOutputActions(0, 0,
            TIME_CODE_ACTION_TX);
        pPXIRouterTriggerActionManager->SetCounterOutputActions(1, 0,
            COUNTER_ACTION_RELOAD);
        pPXIRouterTriggerActionManager->SetCounterOutputActions(1, 0,
            COUNTER_ACTION_START);

        /* setup timer 1 to expire every 200 ms and enable start/stop mode */
        pPXIRouterTriggerConfiguration->SetCounterReloadValue(1,
            CYCLES_PER_SECOND / 5);
        pPXIRouterTriggerConfiguration->EnableCounterAutoReload(1);
        pPXIRouterTriggerConfiguration->EnableCounterStartStopMode(1);

        /* setup timer 1 to decrement timer 2 (packet counter timer) */
        pPXIRouterTriggerEventManager->SetCounterInputEvents(1, 1,
            COUNTER_EVENT_ZERO_SINGLE);
        pPXIRouterTriggerActionManager->SetTimeCodeOutputActions(2, 1,
            COUNTER_ACTION_COUNT);

        /* setup timer 2 to count from 4 to 0 and enable trigger count mode */
        pPXIRouterTriggerConfiguration->SetCounterReloadValue(2, 4);
        pPXIRouterTriggerConfiguration->EnableCounterAutoReload(2);
        pPXIRouterTriggerConfiguration->EnableCounterTriggerCount(2);

        /* setup timer 2 to transmit packets */
        pPXIRouterTriggerEventManager->SetCounterInputEvents(2, 2,
            COUNTER_EVENT_COUNT);
        pPXIRouterTriggerActionManager->SetPortOutputActions(1, 2,
            PORT_ACTION_TRANSMIT_PKT);

        /* setup timer 2 to stop timer 1 when it hits 0 */
        pPXIRouterTriggerEventManager->SetCounterInputEvents(2, 3,
            COUNTER_EVENT_ZERO_SINGLE);
        pPXIRouterTriggerActionManager->SetCounterOutputActions(1, 3,
            COUNTER_ACTION_STOP);

        /* enable port transmit packet mode on port 1 */
        pPXIRouterTriggerConfiguration->EnablePortTransmitPacketMode(1);

        /* reload timers */
        pPXIRouterTriggerConfiguration->ForceCounterReload(0);
        pPXIRouterTriggerConfiguration->ForceCounterReload(1);
        pPXIRouterTriggerConfiguration->ForceCounterReload(2);

        /* enable internal triggers 0-3 */
        pPXIRouterTriggerConfiguration->EnableTrigger(0);
        pPXIRouterTriggerConfiguration->EnableTrigger(1);
        pPXIRouterTriggerConfiguration->EnableTrigger(2);
        pPXIRouterTriggerConfiguration->EnableTrigger(3);

        triggeringUtility.QueuePackets(pDevice, 32);
    }

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;

    printf("32 packets have been queued and will now be transmitted in groups "
        "of 4 at 200 ms intervals when time-codes are transmitted.\n\n");
}

/**
 * Prints the currently configured triggering API configuration.
 *
 * @param pDevice the device to print triggering API configuration for.
 */
void PrintTriggerConf(Device *pDevice)
{
    U32 j;
    unsigned int val;
    TRIGGER_INPUT_MODE triggerInputMode;
    printf("\n----- Printing Trigger Configuration -----\n");

    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    /* create trigger configuration */
    TriggerConfiguration *pTriggerConfiguration =
        TriggerFactory::CreateTriggerConfiguration(pDevice);

    /* validate trigger configuration */
    if (pTriggerConfiguration == NULL)
    {
        printf("\n PrintTriggerConf not supported by this device.\n");
        return;
    }

    if (triggerDevice == TRIGGER_DEVICE_PCIE_IF)
    {
        /* get PCIe trigger configuration */
        PCIeTriggerConfiguration *pPCIeTriggerConfiguration =
            (PCIeTriggerConfiguration *)pTriggerConfiguration;

        for (j = 0; j < NUM_COUNTERS_PCIE; ++j)
        {
            printf("Counter %u:\n", j);
            pPCIeTriggerConfiguration->GetCounterAutoReloadEnabled(j, &val);
            printf("    Auto reload: %u\n", val);
            pPCIeTriggerConfiguration->GetCounterTriggerCountEnabled(j, &val);
            printf("    Trigger count: %u\n", val);
            pPCIeTriggerConfiguration->GetCounterStartModeEnabled(j, &val);
            printf("    Start mode: %u\n", val);
            pPCIeTriggerConfiguration->GetCounterStartStopModeEnabled(j,
                &val);
            printf("    Start stop mode: %u\n", val);
            pPCIeTriggerConfiguration->GetCounterLoadZeroEnabled(j, &val);
            printf("    Load zero: %u\n", val);
            pPCIeTriggerConfiguration->GetCounterReloadValue(j, &val);
            printf("    Reload value: %u\n", val);
        }
        for (j = 0; j < NUM_PORTS_PCIE; ++j)
        {
            printf("Port %d:\n", j + 1);
        }
        for (j = 0; j < NUM_TRIGGERS_PCIE; ++j)
        {
            printf("Trigger %d:\n", j);
            pPCIeTriggerConfiguration->GetTriggerInputMode(j,
                &triggerInputMode);
            printf("    Input mode: %u\n", triggerInputMode);
            pPCIeTriggerConfiguration->GetTriggerEnabled(j, &val);
            printf("    Enabled: %u\n", val);
        }
    }
    else if (triggerDevice == TRIGGER_DEVICE_BRICK_MK3)
    {
        /* get Brick Mk3 trigger configuration */
        BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
            (BrickMk3TriggerConfiguration *)pTriggerConfiguration;

        for (j = 0; j < NUM_EXT_TRIGGERS_BRICK_MK3; ++j)
        {
            printf("External trigger %u:\n", j);
            pBrickMk3TriggerConfiguration->GetExtTriggerEdgeDetectModeEnabled(j,
                &val);
            printf("    Edge detect mode: %u\n", val);
            pBrickMk3TriggerConfiguration->GetExtTriggerInvertEnabled(j, &val);
            printf("    Invert: %u\n", val);
            pBrickMk3TriggerConfiguration->GetExtTriggerOutputEnabled(j, &val);
            printf("    Output: %u\n", val);
            pBrickMk3TriggerConfiguration->GetExtTriggerExtend(j, &val);
            printf("    Extend: %u\n", val);
        }
        for (j = 0; j < NUM_COUNTERS_BRICK_MK3; ++j)
        {
            printf("Counter %u:\n", j);
            pBrickMk3TriggerConfiguration->GetCounterAutoReloadEnabled(j, &val);
            printf("    Auto reload: %u\n", val);
            pBrickMk3TriggerConfiguration->GetCounterTriggerCountEnabled(j,
                &val);
            printf("    Trigger count: %u\n", val);
            pBrickMk3TriggerConfiguration->GetCounterStartModeEnabled(j, &val);
            printf("    Start mode: %u\n", val);
            pBrickMk3TriggerConfiguration->GetCounterStartStopModeEnabled(j,
                &val);
            printf("    Start stop mode: %u\n", val);
            pBrickMk3TriggerConfiguration->GetCounterLoadZeroEnabled(j, &val);
            printf("    Load zero: %u\n", val);
            pBrickMk3TriggerConfiguration->GetCounterReloadValue(j, &val);
            printf("    Reload value: %u\n", val);
        }
        for (j = 0; j < NUM_PORTS_BRICK_MK3; ++j)
        {
            printf("Port %d:\n", j + 1);
            pBrickMk3TriggerConfiguration->GetPortTransmitPacketModeEnabled(
                j + 1, &val);
            printf("    Transmit packet mode: %u\n", val);
        }
        for (j = 0; j < NUM_TIME_CODES_BRICK_MK3; ++j)
        {
            printf("Time-code %d:\n", j);
        }
        for (j = 0; j < NUM_TRIGGERS_BRICK_MK3; ++j)
        {
            printf("Trigger %d:\n", j);
            pBrickMk3TriggerConfiguration->GetTriggerInputMode(j,
                &triggerInputMode);
            printf("    Input mode: %u\n", triggerInputMode);
            pBrickMk3TriggerConfiguration->GetTriggerEnabled(j, &val);
            printf("    Enabled: %u\n", val);
        }
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_IF)
    {
        /* get PXI trigger configuration */
        PXITriggerConfiguration *pPXITriggerConfiguration =
            (PXITriggerConfiguration *)pTriggerConfiguration;

        for (j = 0; j < NUM_EXT_TRIGGERS_PXI_INTERFACE; ++j)
        {
            printf("External trigger %u:\n", j);
            pPXITriggerConfiguration->GetExtTriggerEdgeDetectModeEnabled(j,
                &val);
            printf("    Edge detect mode: %u\n", val);
            pPXITriggerConfiguration->GetExtTriggerInvertEnabled(j, &val);
            printf("    Invert: %u\n", val);
            pPXITriggerConfiguration->GetExtTriggerOutputEnabled(j, &val);
            printf("    Output: %u\n", val);
            pPXITriggerConfiguration->GetExtTriggerExtend(j, &val);
            printf("    Extend: %u\n", val);
        }
        for (j = 0; j < NUM_COUNTERS_PXI_INTERFACE; ++j)
        {
            printf("Counter %u:\n", j);
            pPXITriggerConfiguration->GetCounterAutoReloadEnabled(j, &val);
            printf("    Auto reload: %u\n", val);
            pPXITriggerConfiguration->GetCounterTriggerCountEnabled(j, &val);
            printf("    Trigger count: %u\n", val);
            pPXITriggerConfiguration->GetCounterStartModeEnabled(j, &val);
            printf("    Start mode: %u\n", val);
            pPXITriggerConfiguration->GetCounterStartStopModeEnabled(j,
                &val);
            printf("    Start stop mode: %u\n", val);
            pPXITriggerConfiguration->GetCounterLoadZeroEnabled(j, &val);
            printf("    Load zero: %u\n", val);
            pPXITriggerConfiguration->GetCounterReloadValue(j, &val);
            printf("    Reload value: %u\n", val);
        }
        for (j = 0; j < NUM_PORTS_PXI_INTERFACE; ++j)
        {
            printf("Port %d:\n", j + 1);
            pPXITriggerConfiguration->GetPortTransmitPacketModeEnabled(
                j + 1, &val);
            printf("    Transmit packet mode: %u\n", val);
        }
        for (j = 0; j < NUM_TIME_CODES_PXI_INTERFACE; ++j)
        {
            printf("Time-code %d:\n", j);
        }
        for (j = 0; j < NUM_TRIGGERS_PXI_INTERFACE; ++j)
        {
            printf("Trigger %d:\n", j);
            pPXITriggerConfiguration->GetTriggerInputMode(j,
                &triggerInputMode);
            printf("    Input mode: %u\n", triggerInputMode);
            pPXITriggerConfiguration->GetTriggerEnabled(j, &val);
            printf("    Enabled: %u\n", val);
        }
    }
    else if (triggerDevice == TRIGGER_DEVICE_PXI_ROUTER)
    {
        /* get PXI Router trigger configuration */
        PXIRouterTriggerConfiguration *pPXIRouterTriggerConfiguration =
            (PXIRouterTriggerConfiguration *)pTriggerConfiguration;

        for (j = 0; j < NUM_COUNTERS_PXI_ROUTER; ++j)
        {
            printf("Counter %u:\n", j);
            pPXIRouterTriggerConfiguration->GetCounterAutoReloadEnabled(j, 
                &val);
            printf("    Auto reload: %u\n", val);
            pPXIRouterTriggerConfiguration->GetCounterTriggerCountEnabled(j,
                &val);
            printf("    Trigger count: %u\n", val);
            pPXIRouterTriggerConfiguration->GetCounterStartModeEnabled(j, &val);
            printf("    Start mode: %u\n", val);
            pPXIRouterTriggerConfiguration->GetCounterStartStopModeEnabled(j,
                &val);
            printf("    Start stop mode: %u\n", val);
            pPXIRouterTriggerConfiguration->GetCounterLoadZeroEnabled(j, &val);
            printf("    Load zero: %u\n", val);
            pPXIRouterTriggerConfiguration->GetCounterReloadValue(j, &val);
            printf("    Reload value: %u\n", val);
        }
        for (j = 0; j < NUM_PORTS_PXI_ROUTER; ++j)
        {
            printf("Port %d:\n", j + 1);
            pPXIRouterTriggerConfiguration->GetPortTransmitPacketModeEnabled(
                j + 1, &val);
            printf("    Transmit packet mode: %u\n", val);
        }
        for (j = 0; j < NUM_TIME_CODES_PXI_ROUTER; ++j)
        {
            printf("Time-code %d:\n", j);
        }
        for (j = 0; j < NUM_TRIGGERS_PXI_ROUTER; ++j)
        {
            printf("Trigger %d:\n", j);
            pPXIRouterTriggerConfiguration->GetTriggerInputMode(j,
                &triggerInputMode);
            printf("    Input mode: %u\n", triggerInputMode);
            pPXIRouterTriggerConfiguration->GetTriggerEnabled(j, &val);
            printf("    Enabled: %u\n", val);
        }
    }

    /* cleanup resources */
    delete pTriggerConfiguration;

    printf("\n");
}

/**
 * Perform stop receiving for 5 seconds every 10 seconds.
 *
 * @param pDevice the device to setup triggering for.
 */
void StopReceiving(Device *pDevice)
{
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    if (triggerDevice != TRIGGER_DEVICE_BRICK_MK3)
    {
        printf("\n Test not supported by this device.\n");
        return;
    }

    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);
    triggeringUtility.Reset(pDevice);

    printf("\n----- Trigger Example (Stop Receiving on Port 2 For 5 Seconds "
        "Every 10 Seconds) -----\n");

    /* create trigger utilities */
    TriggerConfiguration *pTriggerConfiguration =
        TriggerFactory::CreateTriggerConfiguration(pDevice);
    TriggerEventManager *pTriggerEventManager =
        TriggerFactory::CreateTriggerEventManager(pDevice);
    TriggerActionManager *pTriggerActionManager =
        TriggerFactory::CreateTriggerActionManager(pDevice);

    /* get Brick Mk3 trigger utilities */
    BrickMk3TriggerConfiguration *pBrickMk3TriggerConfiguration =
        (BrickMk3TriggerConfiguration *)pTriggerConfiguration;
    BrickMk3TriggerEventManager *pBrickMk3TriggerEventManager = 
        (BrickMk3TriggerEventManager *)pTriggerEventManager;
    BrickMk3TriggerActionManager *pBrickMk3TriggerActionManager = 
        (BrickMk3TriggerActionManager *)pTriggerActionManager;

    /* setup counter 0 to fire every 10 seconds and reload counter 1 */
    pBrickMk3TriggerConfiguration->SetCounterReloadValue(0, 
        10 * CYCLES_PER_SECOND);
    pBrickMk3TriggerConfiguration->EnableCounterAutoReload(0);
    pBrickMk3TriggerEventManager->SetCounterInputEvents(0, 0, 
        COUNTER_EVENT_RELOAD);
    pBrickMk3TriggerActionManager->SetCounterOutputActions(1, 0,
        COUNTER_ACTION_RELOAD);

    /* setup counter 1 to fire a single-shot 5 second timer which, while not 
     * zero, causes the STOP_RECEPTION action on port 2
     */
    pBrickMk3TriggerConfiguration->SetCounterReloadValue(1, 
        5 * CYCLES_PER_SECOND);
    pBrickMk3TriggerConfiguration->DisableCounterAutoReload(1);
    pBrickMk3TriggerEventManager->SetCounterInputEvents(1, 1, 
        COUNTER_EVENT_ZERO);
    pBrickMk3TriggerActionManager->SetPortOutputActions(2, 1,
        PORT_ACTION_STOP_RECEPTION);

    /* Setup internal trigger 1 to receive inverted input from counter 1 */
    pBrickMk3TriggerConfiguration->SetTriggerInvertMask(1,
        TRIGGER_TYPE_COUNTER, 0x2);

    /* enable internal triggers 0-1 */
    pBrickMk3TriggerConfiguration->EnableTrigger(0);
    pBrickMk3TriggerConfiguration->EnableTrigger(1);

    /* cleanup resources */
    delete pTriggerConfiguration;
    delete pTriggerEventManager;
    delete pTriggerActionManager;

    printf("Reception will now be stopped for 5 seconds every 10 seconds.\n\n");
}

/**
 * Resets the triggering configuration for the given device.
 *
 * @param pDevice the device to reset triggering configuration for.
 */
void ResetTriggerConf(Device *pDevice)
{
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);
    triggeringUtility.Reset(pDevice);

    printf("\n----- Trigger Example (Reset triggers) -----\n");
    printf("Triggers have been reset.\n\n");
}

int TriggeringExample::DoTriggeringExample()
{
    MENU_CHOICE choice;

    /* print the program header */
    printf("----- Trigger API Examples -----\n");

    /* select first device of chosen type */
    Device *pDevice = GetDevice();
    if (pDevice == NULL)
    {
        printf("No devices were found.\n");
        return 0;
    }

    /* get type of trigger device */
    TRIGGER_DEVICE triggerDevice = GetTriggerDeviceType(
        pDevice->GetDeviceType());

    /* create new triggering utility */
    TriggeringUtility triggeringUtility = TriggeringUtility(triggerDevice);

    /* reset triggers */
    triggeringUtility.Reset(pDevice);

    /* loop menu */
    choice = MENU_CHOICE_INVALID;
    do
    {
        choice = GetMenuChoice();
        switch (choice)
        {
            /* time-code on external trigger (not supported on PCIe or PXI
             * Router)
             */
            case MENU_CHOICE_TX_TC_ON_EXT_TRIGGER:
                TxTcOnExtTrigger(pDevice);
                break;
            /* time-code on counter (not supported on PCIe) */
            case MENU_CHOICE_TX_TC_ON_COUNTER:
                TxTcOnCounter(pDevice);
                break;
            /* transmit packet on external trigger (not supported on PCIe or
             * PXI Router)
             */
            case MENU_CHOICE_PKT_TX_ON_EXT_TRIGGER:
                PktTxOnExtTrigger(pDevice);
                break;
            /* transmit packet on counter */
            case MENU_CHOICE_PKT_TX_ON_COUNTER:
                PktTxOnCounter(pDevice);
                break;
            /* transmit packet on time-code (not supported on PCIe) */
            case MENU_CHOICE_PKT_TX_ON_TC:
                PktTxOnTc(pDevice);
                break;
            /* transmit multiple packets on external trigger
             * (not supported on PCIe or PXI Router)
             */
            case MENU_CHOICE_MULTIPLE_PKT_TX_ON_EXT_TRIGGER:
                MultiplePktTxOnExtTrigger(pDevice);
                break;
            /* transmit multiple packets on time-code (not supported on PCIe) */
            case MENU_CHOICE_MULTIPLE_PKT_TX_ON_TC:
                MultiplePktTxOnTc(pDevice);
                break;
            /* timed packet transmit on time-code (not supported on PCIe) */
            case MENU_CHOICE_TIMED_PKT_TX_ON_TC:
                TimedPktTxOnTc(pDevice);
                break;
            /* stop receiving for 5 seconds every 10 seconds (only
             * supported on Brick Mk3)
             */
            case MENU_CHOICE_STOP_RECEIVING:
                StopReceiving(pDevice);
                break;
            /* print trigger configuration */
            case MENU_CHOICE_PRINT_TRIGGER_CONF:
                PrintTriggerConf(pDevice);
                break;
            /* reset trigger configuration */
            case MENU_CHOICE_RESET:
                ResetTriggerConf(pDevice);
                break;

            /* handle unused cases */
            case MENU_CHOICE_INVALID:
                break;
            case MENU_CHOICE_EXIT:
                break;
        }
    }
    while (choice != MENU_CHOICE_EXIT);

    /* reset triggers */
    triggeringUtility.Reset(pDevice);

    /* dispose of device */
    delete pDevice;

    return 0;
}

