/**
 * \file rmap_target_example.c
 *
 * \brief Functions for the RMAP Target example program.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Contains the bulk of functions used by the STAR-System RMAP Target example
 * program. Provided as an example for developers wishing to write programs
 * using the RMAP Target API.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 */

#include "rmap_target_pxi_if.h"

#include <stdlib.h>

#include "rmap_packet_library.h"
#include "cfg_api_generic.h"

#ifdef _WIN32
    #include "windows.h"

    #define SLEEP(milliseconds) Sleep(milliseconds);
#else
    #include <unistd.h>

    #define SLEEP(milliseconds) usleep(milliseconds * 1000);
#endif

#if !defined(UNREFERENCED_PARAMETER)
    #define UNREFERENCED_PARAMETER(a)   ((void)(a))
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
 * Callback function called when a notification is received from the target.
 */
void STAR_API_CC notifListener(STAR_DEVICE_ID deviceId, U32 target,
    void *pNotif, void *pContext)
{
    int i;

    /* Cast the notification parameter to an array of bytes */
    U8 *notif = (U8*)pNotif;

    /* Cast the context variable to a U32 */
    U32 context = *((U32*)pContext);

    /* Print the device ID and the target index */
    printf("Device %u (Target = %u, Context = %u): ", deviceId, target,
        context);

    /* Print each of the notification bytes */
    /* See the STAR-System documentation for the notification structure */
    for (i = 0; i < 22; ++i)
    {
        printf("0x%02x ", notif[i]);
    }
    printf("\n");
}

/**
 * Sends a random command on the given device id to the specified target.
 *
 * @param deviceId the device to send random command to.
 * @param target the target to send random command to.
 */
void sendRandomCommand(STAR_DEVICE_ID deviceId, U32 target)
{
    U8 buffer[1024];
    U8 cmdPath[2], repPath[1];
    U32 length, address;
    U8 iniLa, tarLa, key;
    STAR_CHANNEL_ID channelId;
    STAR_STREAM_ITEM *streamItem;
    STAR_TRANSFER_OPERATION *transferOp;
    STAR_TRANSFER_STATUS status;
    void *command;
    unsigned long rawPacketLength;

    /* Set the RMAP command parameters to random values */
    length = rand() % 1024;
    address = rand();
    iniLa = 32 + (rand() % 222);
    tarLa = 32 + (rand() % 222);
    key = rand();

    /* Set the buffer to 0xAB, repeating */
    memset(buffer, 0xAB, 1024);

    /* Set the command and reply paths */
    cmdPath[0] = 6 + target, cmdPath[1] = tarLa;
    repPath[0] = iniLa;

    /* Open a channel */
    channelId = STAR_openChannelToLocalDevice(deviceId, STAR_CHANNEL_DIRECTION_OUT, 4, 1);
    if (!channelId)
    {
        printf("Failed to open channel 4\n");
        return;
    }

    /* Build the command */
    command = RMAP_BuildWriteCommandPacket(cmdPath, 2, repPath, 1, 0, 0, 1, key, 0,
        address, 0, buffer, length, &rawPacketLength, NULL, 1);

    /* Create the stream item */
    streamItem = STAR_createPacket(0, command, rawPacketLength, STAR_EOP_TYPE_EOP);
    if (!streamItem)
    {
        RMAP_FreeBuffer(command);
        printf("Failed to create stream item\n");
        return;
    }

    /* Create the transfer operation */
    transferOp = STAR_createTxOperation(&streamItem, 1);
    if (!transferOp)
    {
        STAR_destroyStreamItem(streamItem);
        RMAP_FreeBuffer(command);
        printf("Failed to create transfer operation\n");
        return;
    }

    /* Submit the transfer operation */
    if (!STAR_submitTransferOperation(channelId, transferOp))
    {
        STAR_destroyStreamItem(streamItem);
        RMAP_FreeBuffer(command);
        printf("Failed to submit transfer operation\n");
        return;
    }

    /* Wait on the transfer operation completing */
    status = STAR_waitOnTransferOperationCompletion(transferOp, -1);
    if (status != STAR_TRANSFER_STATUS_COMPLETE)
    {
        STAR_destroyStreamItem(streamItem);
        RMAP_FreeBuffer(command);
        printf("Failed to transmit the command\n");
        return;
    }

    STAR_destroyStreamItem(streamItem);
    RMAP_FreeBuffer(command);
    STAR_closeChannel(channelId);
}

/**
 * Resets the RMAP target parameters on the specified device id.
 *
 * @param deviceId the device to reset.
 */
void reset(STAR_DEVICE_ID deviceId)
{
    int i;

    /* Reset the RMAP parameters for each target */
    for (i = 0; i < 4; ++i)
    {
        RMAP_TARGET_PXI_IF_setInterfaceMode(deviceId, TARGET_PORT_START + i,
            IF_MODE_RMAP_ENABLED);
        RMAP_TARGET_PXI_IF_setAuthControlMode(deviceId, i,
            TARGET_AUTH_MODE_AUTOMATIC);
        RMAP_TARGET_PXI_IF_setAuthCommands(deviceId, i, TARGET_AUTH_CMD_ALL);
        RMAP_TARGET_PXI_IF_setAuthKeyRange(deviceId, i, 0, 255);
        RMAP_TARGET_PXI_IF_setAuthLogicalAddressRange(deviceId, i, 32, 255);
        RMAP_TARGET_PXI_IF_setAuthProtocolId(deviceId, i, 1);
        RMAP_TARGET_PXI_IF_setAuthMemoryAddressRange(deviceId, i, 0,
            0xffffffff);
        RMAP_TARGET_PXI_IF_setAddressOffset(deviceId, i, 0);
        RMAP_TARGET_PXI_IF_setEnabledNotifications(deviceId, i,
            NOTIF_TYPE_NONE);
        RMAP_TARGET_PXI_IF_unregisterNotificationListener(deviceId, i,
            NOTIF_TYPE_AUTH_REQUEST);
        RMAP_TARGET_PXI_IF_unregisterNotificationListener(deviceId, i,
            NOTIF_TYPE_CMD_COMPLETE);
        RMAP_TARGET_PXI_IF_stopReceivingNotifications(deviceId);
    }
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
    printf("Please select:\n");
    printf("%d. Manual RMAP command authorisation example.\n",
        MENU_CHOICE_MANUAL_AUTH);
    printf("%d. Command complete notifications example.\n",
        MENU_CHOICE_NOTIFICATIONS);
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
 * Performs manual RMAP authorisation for the given device id.
 *
 * @param deviceId device id to perform manual RMAP authorisation example on.
 */
void manualAuthExample(STAR_DEVICE_ID deviceId)
{
    char buffer[32];
    unsigned int choice;
    RmapCommandParameters command;

    /* Disable router timeouts so RMAP commands aren't truncated whilst
       awaiting authorisation */
    STAR_CFG_ROUTER_GLOBAL_STATE globalState;
    CFG_getRouterGlobalSettings(deviceId, &globalState);
    globalState.timeoutMode = STAR_CFG_TIMEOUT_MODE_BLOCKING;
    CFG_setRouterGlobalSettings(deviceId, &globalState);

    /* Reset the RMAP targets */
    reset(deviceId);

    /* Disable the RMAP target for target 3 (port 9) so the channel can be used
       to transmit commands */
    RMAP_TARGET_PXI_IF_setInterfaceMode(deviceId, 9, IF_MODE_RMAP_DISABLED);

    /* Enable manual authorisation mode for target 0 */
    RMAP_TARGET_PXI_IF_setAuthControlMode(deviceId, 0, TARGET_AUTH_MODE_MANUAL);

    printf("\nGenerating randomised RMAP command for target 0...\n");

    /* Send a random command to target 0 */
    sendRandomCommand(deviceId, 0);

    /* Get the waiting command */
    RMAP_TARGET_PXI_IF_getWaitingCommand(deviceId, 0, &command);

    /* Print the command parameters */
    printf("\nParameters: tar LA = 0x%x, command = 0x%x, key = 0x%x, address = 0x%x, length = 0x%x, ini LA = 0x%x\n",
        command.targetLogicalAddress, command.command, command.key,
        command.address, command.dataLength, command.initiatorLogicalAddress);

    /* Print the authorisation menu */
    printf("1. Authorise.\n");
    printf("2. Reject.\n");
    printf("> ");

    if (!fgets(buffer, 32, stdin)) choice = 0;

    if (!sscanf(buffer, "%u", &choice) ||
        choice >= MENU_CHOICE_INVALID) choice = 0;

    /* Authorise or reject the command */
    if (choice == 1)
    {
        printf("Authorising command...\n");
        RMAP_TARGET_PXI_IF_authoriseWaitingCommand(deviceId, 0);
    }
    else if (choice == 2)
    {
        printf("Rejecting command...\n");
        RMAP_TARGET_PXI_IF_rejectWaitingCommand(deviceId, 0,
            REJECTION_REASON_OTHER);
    }
}

/**
 * Sends continuous random commands and prints out the notification details.
 *
 * @param deviceId device id to run notification example on.
 */
void notificationsExample(STAR_DEVICE_ID deviceId)
{
    U32 target;
    U32 context0, context1, context2;

    /* Reset the RMAP targets */
    reset(deviceId);

    /* Disable the RMAP target for target 3 (port 9) so the channel can be used
       to transmit commands */
    RMAP_TARGET_PXI_IF_setInterfaceMode(deviceId, 9, IF_MODE_RMAP_DISABLED);

    /* Enable notifications for targets 0-2 */
    RMAP_TARGET_PXI_IF_setEnabledNotifications(deviceId, 0,
        NOTIF_TYPE_CMD_COMPLETE);
    RMAP_TARGET_PXI_IF_setEnabledNotifications(deviceId, 1,
        NOTIF_TYPE_CMD_COMPLETE);
    RMAP_TARGET_PXI_IF_setEnabledNotifications(deviceId, 2,
        NOTIF_TYPE_CMD_COMPLETE);

    /* Set the context variables */
    context0 = 123;
    context1 = 456;
    context2 = 789;

    /* Set the command complete notification listener for targets 0-2 */
    RMAP_TARGET_PXI_IF_registerNotificationListenerWithContext(deviceId, 0,
        NOTIF_TYPE_CMD_COMPLETE, notifListener, &context0);
    RMAP_TARGET_PXI_IF_registerNotificationListenerWithContext(deviceId, 1,
        NOTIF_TYPE_CMD_COMPLETE, notifListener, &context1);
    RMAP_TARGET_PXI_IF_registerNotificationListenerWithContext(deviceId, 2,
        NOTIF_TYPE_CMD_COMPLETE, notifListener, &context2);

    /* Start receiving notifications for the device */
    RMAP_TARGET_PXI_IF_startReceivingNotifications(deviceId);

    printf("\nGenerating randomised RMAP commands for target 0-2...\n");

    /* Send random commands to targets 0-2 every 200 milliseconds */
    while (1)
    {
        target = rand() % 3;
        sendRandomCommand(deviceId, target);
        SLEEP(200);
    }

}

/**
 * Gets the first PXI with RMAP Target device and returns the device id.
 *
 * @return device id of first PXI with RMAP Target.
 */
STAR_DEVICE_ID getDeviceId(void)
{
    U32 count;
    STAR_DEVICE_ID *devices;
    STAR_DEVICE_ID deviceId;

    /* Get the first PXI RMAP Target device */
    devices = STAR_getDeviceListForType(STAR_DEVICE_PXI_RMAP, &count);
    deviceId = count ? devices[0] : 0;
    STAR_destroyDeviceList(devices);
    return deviceId;
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

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    /* Print the program header */
    printf("----- RMAP Target API Examples -----\n");

    deviceId = getDeviceId();
    if (!deviceId)
    {
        printf("No devices were found.\n");
        return 0;
    }

    /* Reset RMAP targets */
    reset(deviceId);

    /* This example requires interface mode to be disabled */
    CFG_disableInterfaceMode(deviceId);

    /* Loop menu */
    choice = MENU_CHOICE_INVALID;
    do
    {
        choice = getMenuChoice();
        switch (choice)
        {
        /* Manual authorisation example */
        case MENU_CHOICE_MANUAL_AUTH: manualAuthExample(deviceId); break;
        /* Notifications example */
        case MENU_CHOICE_NOTIFICATIONS: notificationsExample(deviceId); break;

        /* Handle unused cases */
        case MENU_CHOICE_INVALID: break;
        case MENU_CHOICE_EXIT: break;
        }
    } while (choice != MENU_CHOICE_EXIT);

    return 0;
}
