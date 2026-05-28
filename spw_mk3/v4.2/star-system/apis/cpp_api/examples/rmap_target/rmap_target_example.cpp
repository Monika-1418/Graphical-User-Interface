/**
 * \file rmap_target_example.cpp
 *
 * \brief Implementation of class which provides an example of the RMAP Target 
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
 * RMAP Target API being used to configure the RMAP targets and handle 
 * notifications.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 */

#include "rmap_target_example.h"

#include <stdio.h>

#include "device_list.hpp"
#include "channel.hpp"
#include "rmap_write_command_packet.hpp"
#include "packet.hpp"
#include "transmit_operation.hpp"
#include "pxi_device_configuration.hpp"
#include "rmap_target_factory.hpp"
#include "rmap_notification_callback.hpp"

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::rmap;
using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::transferoperations;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices::pxi;
using namespace stardundee::com::starsystem::rmaptarget;
using namespace stardundee::com::starsystem::rmaptarget::pxi;
using namespace stardundee::com::starsystem::rmaptarget::notifications;
using namespace stardundee::com::starsystem::rmaptarget::notifications::pxi;
using namespace stardundee::com::starsystem::rmaptarget::authorisation::pxi;

#ifdef _WIN32
#include "windows.h"

#define SLEEP(milliseconds) Sleep(milliseconds);
#else
#include <unistd.h>

#define SLEEP(milliseconds) usleep(milliseconds * 1000);
#endif

/* The four RMAP targets are accessed through external ports 6-9 */
#define TARGET_PORT_START       (6)
#define TARGET_PORT_END         (9)

/**
 * Enum containing the different possible menu choices for the application.
 */
typedef enum
{
    MENU_CHOICE_MANUAL_AUTH = 1,
    MENU_CHOICE_NOTIFICATIONS,
    MENU_CHOICE_EXIT,
    MENU_CHOICE_INVALID
} MENU_CHOICE;

/**
 * Class that handles RMAP notifications being received.
 */
class ExampleRMAPNotificationCallback : public RMAPNotificationCallback
{
public:
    void NotificationReceived(
        stardundee::com::starsystem::general::Device *pDevice,
        stardundee::com::starsystem::rmaptarget::notifications::
        RMAPNotification *pNotification)
    {
        /* get device id */
        STAR_DEVICE_ID deviceId = pDevice->GetDeviceID();

        /* if authorisation request notification */
        if (pNotification->GetNotificationType() == NOTIF_TYPE_AUTH_REQUEST)
        {
            /* get authorisation request notification */
            AuthorisationRequestNotification 
                *pAuthorisationRequestNotification = 
                (AuthorisationRequestNotification *)pNotification;

            /* get authorisation request parameters */
            U8 targetIndex = 
                pAuthorisationRequestNotification->GetTargetIndex();
            U8 currentTimeCode = 
                pAuthorisationRequestNotification->GetCurrentTimeCode();

            /* print authorisation request notification details */
            printf("Authorisation Request Notification\n");
            printf("----------------------------------\n");
            printf("Device ID: %d\n", deviceId);
            printf("Target index: %d\n", targetIndex);
            printf("Current time-code: %d\n", currentTimeCode);
            printf("\n");
        }
        /* else if command complete notification */
        else if (pNotification->GetNotificationType() == 
            NOTIF_TYPE_CMD_COMPLETE)
        {
            /* get command complete notification */
            CommandCompleteNotification *pCommandCompleteNotification = 
                (CommandCompleteNotification *)pNotification;

            /* get command complete parameters */
            U8 targetIndex = 
                pCommandCompleteNotification->GetTargetIndex();
            U8 currentTimeCode = pCommandCompleteNotification->
                GetCurrentTimeCode();
            U8 targetLogicalAddress = pCommandCompleteNotification->
                GetTargetLogicalAddress();
            U8 protocolId = pCommandCompleteNotification->GetProtocolId();
            U8 command = pCommandCompleteNotification->GetCommand();
            U8 key = pCommandCompleteNotification->GetKey();
            U8 initiatorLogicalAddress = pCommandCompleteNotification->
                GetInitiatorLogicalAddress();
            U16 transactionId = pCommandCompleteNotification->
                GetTransactionId();
            U8 extendedAddress = pCommandCompleteNotification->
                GetExtendedAddress();
            U32 address = pCommandCompleteNotification->GetAddress();
            U32 dataLength = pCommandCompleteNotification->GetDataLength();
            U8 status = pCommandCompleteNotification->GetStatus();
            
            /* print command complete notification details */
            printf("Command Complete Notification\n");
            printf("-----------------------------\n");
            printf("Device ID: %d\n", deviceId);
            printf("Target index: %d\n", targetIndex);
            printf("Current time-code: %d\n", currentTimeCode);
            printf("Target logical address: %d\n", targetLogicalAddress);
            printf("Protocol ID: %d\n", protocolId);
            printf("Command: %d\n", command);
            printf("Key: %d\n", key);
            printf("Initiator logical address: %d\n", initiatorLogicalAddress);
            printf("Transaction ID: %d\n", transactionId);
            printf("Extended address: %d\n", extendedAddress);
            printf("Address: %d\n", address);
            printf("Data length: %d\n", dataLength);
            printf("Status: %d\n", status);
            printf("\n");
        }
    }
};

/**
 * Sends a random command on the given device to the specified target.
 *
 * @param pDevice the device to send random command to.
 * @param target the target to send random command to.
 */
void SendRandomCommand(Device *pDevice, U32 target)
{
    U8 buffer[1024];
    U8 cmdPath[2], repPath[1];
    U32 length, address;
    U8 iniLa, tarLa, key;
    TransferOperation::TransferStatus status;

    /* set the RMAP command parameters to random values */
    length = rand() % 1024;
    address = rand();
    iniLa = 32 + (rand() % 222);
    tarLa = 32 + (rand() % 222);
    key = rand();

    /* set the buffer to 0xAB, repeating */
    memset(buffer, 0xAB, 1024);

    /* set the command and reply paths */
    cmdPath[0] = 6 + target, cmdPath[1] = tarLa;
    repPath[0] = iniLa;

    /* open a channel */
    Channel channel = pDevice->OpenChannel(Channel::DIRECTION_OUT, 4, 1);
    if (!channel.IsOpen())
    {
        printf("Failed to open channel 4\n");
        return;
    }

    /* build the command */
    RMAPWriteCommandPacket writeCommandPacket;
    writeCommandPacket.CreateWriteCommandPacket(cmdPath, 2, repPath, 1, 0, 0, 
        1, key, 0, address, 0, buffer, length, 1);
    
    /* create the stream item */
    Packet packet;
    U32 commandLength = writeCommandPacket.GetRawDataLen();
    U8 *pCommand = (U8 *)writeCommandPacket.GetRawDataPtr();
    if(!packet.CreatePacket((Address *)NULL, pCommand, commandLength, 
        Packet::EOP_TYPE_EOP))
    {
        writeCommandPacket.Dispose();
        printf("Failed to create stream item\n");
        return;
    }

    /* get pointer to packet required for transmit operation */
    Packet *pPacket = &packet;

    /* create the transfer operation */
    TransmitOperation transmitOperation;
    if(!transmitOperation.CreateTxOperation((StreamItem **)&pPacket, 1))
    {
        packet.DestroyPacket();
        writeCommandPacket.Dispose();
        printf("Failed to create transfer operation\n");
        return;
    }

    /* submit the transfer operation */
    if (!channel.SubmitTransferOperation(&transmitOperation))
    {
        packet.DestroyPacket();
        writeCommandPacket.Dispose();
        printf("Failed to submit transfer operation\n");
        return;
    }
    
    /* wait on the transfer operation completing */
    status = transmitOperation.WaitOnTransferOperationCompletion(-1);
    if (status != TransmitOperation::TRANSFER_STATUS_COMPLETE)
    {
        packet.DestroyPacket();
        writeCommandPacket.Dispose();
        printf("Failed to transmit the command\n");
        return;
    }

    packet.DestroyPacket();
    writeCommandPacket.Dispose();
    channel.CloseChannel();
}

/**
 * Resets the RMAP target parameters on the specified device.
 *
 * @param pDevice the device to reset.
 */
void Reset(Device *pDevice)
{
    int i;

    /* initialise RMAP utilities */
    PXIRMAPTargetConfiguration *pPXIRMAPTargetConfiguration = 
        (PXIRMAPTargetConfiguration *)RMAPTargetFactory::
        CreateRMAPTargetConfiguration(pDevice);
    PXIRMAPTargetNotificationManager *pPXIRMAPTargetNotificationManager = 
        (PXIRMAPTargetNotificationManager *)RMAPTargetFactory::
        CreateRMAPTargetNotificationManager(pDevice);

    /* reset the RMAP parameters for each target */
    for (i = 0; i < 4; ++i)
    {
        pPXIRMAPTargetConfiguration->SetInterfaceMode(TARGET_PORT_START + i,
            IF_MODE_RMAP_ENABLED);
        pPXIRMAPTargetConfiguration->SetAuthControlMode(i,
            TARGET_AUTH_MODE_AUTOMATIC);
        pPXIRMAPTargetConfiguration->SetAuthCommands(i, TARGET_AUTH_CMD_ALL);
        pPXIRMAPTargetConfiguration->SetAuthKeyRange(i, 0, 255);
        pPXIRMAPTargetConfiguration->SetAuthLogicalAddressRange(i, 32, 255);
        pPXIRMAPTargetConfiguration->SetAuthProtocolId(i, 1);
        pPXIRMAPTargetConfiguration->SetAuthMemoryAddressRange(i, 0,
            0xffffffff);
        pPXIRMAPTargetConfiguration->SetAddressOffset(i, 0);
        pPXIRMAPTargetNotificationManager->SetEnabledNotifications(i,
            NOTIF_TYPE_NONE);
        pPXIRMAPTargetNotificationManager->UnregisterNotificationListener(i,
            NOTIF_TYPE_AUTH_REQUEST);
        pPXIRMAPTargetNotificationManager->UnregisterNotificationListener(i,
            NOTIF_TYPE_CMD_COMPLETE);
        pPXIRMAPTargetNotificationManager->StopReceivingNotifications();
    }

    /* cleanup resources */
    delete pPXIRMAPTargetConfiguration;
    delete pPXIRMAPTargetNotificationManager;
}

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
    printf("Please select:\n");
    printf("%d. Manual RMAP command authorisation example.\n",
        MENU_CHOICE_MANUAL_AUTH);
    printf("%d. Command complete notifications example.\n",
        MENU_CHOICE_NOTIFICATIONS);
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
 * Performs manual RMAP authorisation for the given device.
 *
 * @param pDevice device to perform manual RMAP authorisation example on.
 */
void ManualAuthExample(Device *pDevice)
{
    char buffer[32];
    unsigned int choice;
    RMAPCommandParameters command;

    PXIRMAPTargetConfiguration *pPXIRMAPTargetConfiguration = 
        (PXIRMAPTargetConfiguration *)RMAPTargetFactory::
        CreateRMAPTargetConfiguration(pDevice);
    PXIRMAPTargetManualAuthorisationManager
        *pPXIRMAPTargetManualAuthorisationManager = 
        (PXIRMAPTargetManualAuthorisationManager *)RMAPTargetFactory::
        CreateRMAPTargetManualAuthorisationManager(pDevice);

    /* disable router timeouts so RMAP commands aren't truncated whilst
     * awaiting authorisation
     */
    STAR_CFG_ROUTER_GLOBAL_STATE globalState;
    CFG_ROUTER_getRouterGlobalSettings(pDevice->GetDeviceID(), &globalState);
    globalState.timeoutMode = STAR_CFG_TIMEOUT_MODE_BLOCKING;
    CFG_ROUTER_setRouterGlobalSettings(pDevice->GetDeviceID(), globalState);

    /* reset the RMAP targets */
    Reset(pDevice);

    /* disable the RMAP target for target 3 (port 9) so the channel can be used
     * to transmit commands
     */
    pPXIRMAPTargetConfiguration->SetInterfaceMode(9, IF_MODE_RMAP_DISABLED);

    /* enable manual authorisation mode for target 0 */
    pPXIRMAPTargetConfiguration->SetAuthControlMode(0, TARGET_AUTH_MODE_MANUAL);

    printf("\nGenerating randomised RMAP command for target 0...\n");

    /* send a random command to target 0 */
    SendRandomCommand(pDevice, 0);

    /* get the waiting command */
    pPXIRMAPTargetManualAuthorisationManager->GetWaitingCommand(0, &command);

    /* print the command parameters */
    printf("\nParameters: tar LA = 0x%x, command = 0x%x, key = 0x%x, address = 0x%x, length = 0x%x, ini LA = 0x%x\n",
        command.GetTargetLogicalAddress(), command.GetCommand(), 
        command.GetKey(), command.GetAddress(), command.GetDataLength(), 
        command.GetInitiatorLogicalAddress());

    /* print the authorisation menu */
    printf("1. Authorise.\n");
    printf("2. Reject.\n");
    printf("> ");

    if (!fgets(buffer, 32, stdin)) choice = 0;

    if (!sscanf(buffer, "%u", &choice) ||
        choice >= MENU_CHOICE_INVALID) choice = 0;

    /* authorise or reject the command */
    if (choice == 1)
    {
        printf("Authorising command...\n");
        pPXIRMAPTargetManualAuthorisationManager->AuthoriseWaitingCommand(0);
    }
    else if (choice == 2)
    {
        printf("Rejecting command...\n");
        pPXIRMAPTargetManualAuthorisationManager->RejectWaitingCommand(0,
            REJECTION_REASON_OTHER);
    }

    /* cleanup resources */
    delete pPXIRMAPTargetConfiguration;
    delete pPXIRMAPTargetManualAuthorisationManager;
}

/**
 * Sends continuous random commands and prints out the notification details.
 *
 * @param pDdevice device to run notification example on.
 */
void NotificationsExample(Device *pDevice)
{
    U32 target;
    
    PXIRMAPTargetConfiguration *pPXIRMAPTargetConfiguration = 
        (PXIRMAPTargetConfiguration *)RMAPTargetFactory::
        CreateRMAPTargetConfiguration(pDevice);
    PXIRMAPTargetNotificationManager *pPXIRMAPTargetNotificationManager = 
        (PXIRMAPTargetNotificationManager *)RMAPTargetFactory::
        CreateRMAPTargetNotificationManager(pDevice);

    ExampleRMAPNotificationCallback notificationCallback;

    /* reset the RMAP targets */
    Reset(pDevice);

    /* disable the RMAP target for target 3 (port 9) so the channel can be used
     * to transmit commands
     */
    pPXIRMAPTargetConfiguration->SetInterfaceMode(9, IF_MODE_RMAP_DISABLED);

    /* enable notifications for targets 0-2 */
    pPXIRMAPTargetNotificationManager->SetEnabledNotifications(0,
        NOTIF_TYPE_CMD_COMPLETE);
    pPXIRMAPTargetNotificationManager->SetEnabledNotifications(1,
        NOTIF_TYPE_CMD_COMPLETE);
    pPXIRMAPTargetNotificationManager->SetEnabledNotifications(2,
        NOTIF_TYPE_CMD_COMPLETE);

    /* set the command complete notification listener for targets 0-2 */
    pPXIRMAPTargetNotificationManager->RegisterNotificationListener(0,
        NOTIF_TYPE_CMD_COMPLETE, &notificationCallback);
    pPXIRMAPTargetNotificationManager->RegisterNotificationListener(1,
        NOTIF_TYPE_CMD_COMPLETE, &notificationCallback);
    pPXIRMAPTargetNotificationManager->RegisterNotificationListener(2,
        NOTIF_TYPE_CMD_COMPLETE, &notificationCallback);

    /* start receiving notifications for the device */
    pPXIRMAPTargetNotificationManager->StartReceivingNotifications();

    printf("\nGenerating randomised RMAP commands for target 0-2...\n");

    /* send random commands to targets 0-2 every 200 milliseconds */
    while (1)
    {
        target = rand() % 3;
        SendRandomCommand(pDevice, target);
        SLEEP(200);
    }

    /* cleanup resources */
    delete pPXIRMAPTargetConfiguration;
    delete pPXIRMAPTargetNotificationManager;
}

/**
 * Gets the first PXI with RMAP Target device and returns the device instance.
 *
 * @return device instance representing first PXI with RMAP Target.
 */
static Device * GetDevice()
{
    /* populate device list with all PXI with RMAP Target devices */
    DeviceList deviceList;
    U32 deviceCount = deviceList.PopulateForType(STAR_DEVICE_PXI_RMAP);

    /* initialise first device to null */
    Device *pFirstDevice = NULL;

    /* select the first device */
    if (deviceCount > 0)
    {
        pFirstDevice = deviceList.CopyDeviceAt(0);
    }

    /* return first PXI with RMAP Target device */
    return pFirstDevice;
}

int RMAPTargetExample::DoRMAPTargetExample()
{
    /* print the program header */
    printf("----- RMAP Target API Examples -----\n");

    /* select first PXI with RMAP Target device */
    Device *pDevice = GetDevice();
    if (pDevice == NULL)
    {
        printf("No devices were found.\n");
        return 0;
    }

    /* reset RMAP targets */
    Reset(pDevice);
    
    /* this example requires interface mode to be disabled */
    PXIDeviceConfiguration *pDeviceConfiguration =
        (PXIDeviceConfiguration *)pDevice->GetDeviceConfiguration();
    pDeviceConfiguration->DisableInterfaceMode();
    delete pDeviceConfiguration;

    /* loop menu */
    MENU_CHOICE choice = MENU_CHOICE_INVALID;
    do
    {
        choice = GetMenuChoice();
        switch (choice)
        {
        /* manual authorisation example */
        case MENU_CHOICE_MANUAL_AUTH:
            ManualAuthExample(pDevice);
            break;
        /* notifications example */
        case MENU_CHOICE_NOTIFICATIONS:
            NotificationsExample(pDevice);
            break;

        /* handle unused cases */
        case MENU_CHOICE_INVALID: break;
        case MENU_CHOICE_EXIT: break;
        }
    }
    while (choice != MENU_CHOICE_EXIT);

    /* dispose of device */
    delete pDevice;

    return 0;
}

