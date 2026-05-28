/**
 * \file link_events.c
 *
 * \brief Example usage of link speed and state change events.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example code demonstrating how to receive link speed and
 * state change events.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd
*/

#include <stdio.h>

#include "star-api.h"
#include "cfg_api_mk2.h"
#include "cfg_api_brick_mk2.h"
#include "cfg_api_router.h"
#include "cfg_api_brick_mk3.h"
#include "cfg_api_pxi.h"
#include "cfg_api_pci_mk2.h"
#include "cfg_api_generic.h"
#include "ui.h"

//--------------------------------------

#define TEST_ERROR_STATUS(Status,sText,sError)    \
if (Status == 0)            \
{                           \
    HandleError(sError);    \
    return (1);             \
}                           \
else                        \
{                           \
    printf("%s", sText);    \
}

#define TEST_FUNC_STATUS(Status,sText,sError)    \
if (Status == 0)            \
{                           \
    printf("%s", sError);   \
    return (1);             \
}                           \
else                        \
{                           \
    printf("%s", sText);    \
}

#define TEST_SUB_FUNC_STATUS(Status,sText,sError)    \
if (Status == 0)            \
{                           \
    printf("%s", sError);   \
    return (0);             \
}                           \
else                        \
{                           \
    printf("%s", sText);    \
}

#define TEST_GENERIC_ERROR_STATUS(Status,sText,sError)  \
if (!CFG_SUCCESS(Status))                               \
{                           \
    HandleError(sError);    \
    return (1);             \
}                           \
else                        \
{                           \
    printf("%s", sText);    \
}

#define TEST_GENERIC_FUNC_STATUS(Status,sText,sError)   \
if (!CFG_SUCCESS(Status))                               \
{                           \
    printf("%s", sError);   \
    return (1);             \
}                           \
else                        \
{                           \
    printf("%s", sText);    \
}

#define TEST_GENERIC_SUB_FUNC_STATUS(Status,sText,sError)   \
if (!CFG_SUCCESS(Status))                               \
{                           \
    printf("%s", sError);   \
    return (0);             \
}                           \
else                        \
{                           \
    printf("%s", sText);    \
}

//--------------------------------------

#if defined(_WIN32)
#include <windows.h>
#elif (defined(__linux__) || defined(__CYGWIN__) || defined(__QNX__) || defined(__rtems__) || defined(__vxworks))
#include <pthread.h>
#include <unistd.h>
#endif

//--------------------------------------

void DelayMS(U32 _DelayMS)
{
#if defined(_WIN32)
    Sleep(_DelayMS);
#elif (defined(__linux__) || defined(__CYGWIN__) || defined(__QNX__) || defined(__rtems__))
    usleep(_DelayMS * 1000);
#elif defined(__vxworks)
    taskDelay(_DelayMS);
#endif
}

//--------------------------------------

void HandleError(const char* _psText)
{
    printf("%s", _psText);
    printf("\nPress Enter to exit...\n");
    getchar();
}

//--------------------------------------

int SetLinkSpeed(STAR_DEVICE_ID _DeviceID, U8 _Port, U8 _Divisor)
{
    int Status = CFG_setTransmitSignallingRate(_DeviceID, _Port, 200 / _Divisor);
    TEST_GENERIC_SUB_FUNC_STATUS(Status, "", "");
    return (Status);
}

//--------------------------------------

int PrintLinkSpeed(STAR_DEVICE_ID _DeviceID, U8 _TxPort, U8 _RxPort, const char* _psText)
{
    U16 LinkSpeed;
    int Status = CFG_getMeasuredLinkSpeed(_DeviceID, _RxPort, &LinkSpeed);
    if (Status >= 0)
    {
        U32 TempLinkSpeed = (LinkSpeed * STAR_CFG_MK2_LINK_SPEED_UNITS_KBPS);
        printf(_psText, _TxPort, _RxPort, _RxPort, TempLinkSpeed / 1000, (TempLinkSpeed % 1000) / 100);
    }
    return (Status);
}

//--------------------------------------

STAR_DEVICE_ID GetRemoteDeviceID(STAR_DEVICE_ID _DeviceID,U8 _RemoteChannel)
{
    unsigned char aPath[] = { 0,254 };
    unsigned char aRetPath[] = { 254 };

    STAR_SPACEWIRE_ADDRESS* pPath = STAR_createAddress(aPath, sizeof(aPath));
    STAR_SPACEWIRE_ADDRESS* pRetPath = STAR_createAddress(aRetPath, sizeof(aRetPath));

    STAR_DEVICE_ID RemoteDeviceID = STAR_CFG_createRemoteDeviceIdentifier(
        _DeviceID, _RemoteChannel, NULL, pPath, pRetPath);

    STAR_destroyAddress(pRetPath);
    STAR_destroyAddress(pPath);

    return (RemoteDeviceID);
}

//--------------------------------------

int EnableLinkEvents(STAR_DEVICE_ID _DeviceID, U8 _Port)
{
    int Status = CFG_enableSpeedChangeEventsOnPort(_DeviceID,_Port);
    TEST_GENERIC_SUB_FUNC_STATUS(Status,"","");
    Status = CFG_enableStateChangeEventsOnPort(_DeviceID,_Port);
    TEST_GENERIC_SUB_FUNC_STATUS(Status,"","");
    return (Status);
}

//--------------------------------------

int DisableLinkEvents(STAR_DEVICE_ID _DeviceID, U8 _Port)
{
    int Status = CFG_disableSpeedChangeEventsOnPort(_DeviceID,_Port);
    TEST_GENERIC_SUB_FUNC_STATUS(Status,"","");
    Status = CFG_disableStateChangeEventsOnPort(_DeviceID,_Port);
    TEST_GENERIC_SUB_FUNC_STATUS(Status,"","");
    return (Status);
}

//--------------------------------------

int WaitEvents(STAR_CHANNEL_ID _ChannelID,STAR_TRANSFER_OPERATION* _pLinkEventRxOperation)
{
    int Status = 0;
    U32 Index;

    STAR_TRANSFER_STATUS TransferOperationStatus;
    do
    {
        TransferOperationStatus = STAR_waitOnTransferOperationCompletion(_pLinkEventRxOperation,2000);
        switch (TransferOperationStatus)
        {
            case STAR_TRANSFER_STATUS_COMPLETE:
            {
                U32 ItemCount = STAR_getTransferItemCount(_pLinkEventRxOperation);
                for (Index=0;Index<ItemCount;Index++)
                {
                    STAR_STREAM_ITEM* pLinkEvent = STAR_getTransferItem(_pLinkEventRxOperation,Index);
                    STAR_STREAM_ITEM_TYPE EventType = (STAR_STREAM_ITEM_TYPE)pLinkEvent->itemType;
                    if (EventType == STAR_STREAM_ITEM_TYPE_LINK_STATE_EVENT)
                    {
                        STAR_LINK_STATE_EVENT* pLinkStateEvent = (STAR_LINK_STATE_EVENT*)pLinkEvent->item;
                        U8 Port = STAR_getLinkStateEventPort(pLinkStateEvent);

                        int bIsLinkRunning = STAR_isLinkStateEventLinkRunning(pLinkStateEvent);
                        if (bIsLinkRunning == FALSE)
                        {
                            printf("\nPort %i state event:  Link not running.", Port);
                        }
                        else
                        {
                            printf("\nPort %i state event:  Link running.", Port);
                        }
                    }
                    else if (EventType == STAR_STREAM_ITEM_TYPE_LINK_SPEED_EVENT)
                    {
                        STAR_LINK_SPEED_EVENT* pLinkSpeedEvent = (STAR_LINK_SPEED_EVENT*)pLinkEvent->item;
                        U8 Port = STAR_getLinkSpeedEventPort(pLinkSpeedEvent);
                        U32 NewLinkSpeed = STAR_getLinkSpeedEventSpeed(pLinkSpeedEvent);
                        printf("\nPort %i speed event:  Link speed %u.%u Mbit/s", Port, NewLinkSpeed / 1000000, (NewLinkSpeed % 1000000) / 100000);
                    }
                }
                break;
            }
            case STAR_TRANSFER_STATUS_STARTED:
            {
                printf("\nNo more events\n");
                Status = STAR_cancelTransferOperation(_pLinkEventRxOperation);
                TEST_SUB_FUNC_STATUS(Status,"","");
                break;
            }
            case STAR_TRANSFER_STATUS_NOT_STARTED:
            {
                printf("\nXfer not started and link speed event did not occur");
                Status = STAR_cancelTransferOperation(_pLinkEventRxOperation);
                TEST_SUB_FUNC_STATUS(Status,"","");
                break;
            }
            case STAR_TRANSFER_STATUS_CANCELLED:
            {
                //printf("\nCancelled");
                break;
            }
            case STAR_TRANSFER_STATUS_ERROR:
            default:
            {
                printf("\nError occurred while waiting for link speed event");
                break;
            }
        }

        if (TransferOperationStatus != STAR_TRANSFER_STATUS_STARTED)
        {
            Status = STAR_submitTransferOperation(_ChannelID,_pLinkEventRxOperation);
            TEST_SUB_FUNC_STATUS(Status,"","\nSTAR_submitTransferOperation Status = 0.");
        }
    }
    while (TransferOperationStatus != STAR_TRANSFER_STATUS_STARTED);

    return (Status);
}

//--------------------------------------

void PrintDeviceType(STAR_DEVICE_ID _DeviceID)
{
    char* psDeviceType = STAR_getDeviceTypeAsString(_DeviceID);
    if (psDeviceType == NULL)
    {
        puts("Testing an unknown device type");
    }
    else
    {
        printf("\nTesting the %s\n", psDeviceType);
        STAR_destroyString(psDeviceType);
    }
}

//--------------------------------------

int DisableTriState(STAR_DEVICE_ID _DeviceID, U8 _Port)
{
    PORT_STATUS_CONTROL PortStatusControl;
    STAR_CFG_SPW_LINK_STATUS LinkStatus;

    int Status = CFG_getPortStatusControl(_DeviceID, _Port, &PortStatusControl);
    TEST_GENERIC_SUB_FUNC_STATUS(Status, "", "");

    Status = CFG_getSpaceWireLinkStatus(PortStatusControl, &LinkStatus);
    TEST_GENERIC_SUB_FUNC_STATUS(Status, "", "");

    LinkStatus.triState = FALSE;
    Status = CFG_setSpaceWireLinkStatus(_DeviceID, _Port, &LinkStatus);
    TEST_GENERIC_SUB_FUNC_STATUS(Status, "", "");

    return (Status);
}

//--------------------------------------

int bAreLinkEventsOnChannel0Only(U32 _DeviceType)
{
    if ((_DeviceType == STAR_DEVICE_BRICK_MK2) ||
        (_DeviceType == STAR_DEVICE_BRICK_MK3) ||
        (_DeviceType == STAR_DEVICE_ROUTER_MK2S) ||
        (_DeviceType == STAR_DEVICE_SPLT) ||
        (_DeviceType == STAR_DEVICE_STAR_FIRE_MK3))
    {
        return (TRUE);
    }
    return (FALSE);
}

//--------------------------------------

#define NUM_DEVICE_TYPES    (14)

int __cdecl main()
{
    STAR_DEVICE_ID DeviceID;
    STAR_DEVICE_ID RemoteDeviceID;
    STAR_CHANNEL_ID ChannelID;
    STAR_CFG_FPGA_INFO FPGAInfo;
    STAR_TRANSFER_OPERATION* pLinkEventRxOperation;
    STAR_CHANNEL_MASK ChannelMask;
    STAR_CHANNEL_MASK UsableChannelsMask;

    U32 DeviceType;
    int Status;
    U8 TxPort;
    U8 RxPort;
    U8 Channel;
    U8 RemoteChannel;
    U32 Index;
    char sBuffer[96];

    char sVersion[STAR_CFG_MK2_VERSION_STR_MAX_LEN];
    char sBuildDate[STAR_CFG_MK2_BUILD_DATE_STR_MAX_LEN];

    STAR_DEVICE_TYPE aDeviceTypes[NUM_DEVICE_TYPES];
    aDeviceTypes[0] = STAR_DEVICE_BRICK_MK2;
    aDeviceTypes[1] = STAR_DEVICE_ROUTER_MK2S;
    aDeviceTypes[2] = STAR_DEVICE_BRICK_MK3;
    aDeviceTypes[3] = STAR_DEVICE_PCIE;
    aDeviceTypes[4] = STAR_DEVICE_PXI_INTERFACE;
    aDeviceTypes[5] = STAR_DEVICE_PXI_RMAP;
    aDeviceTypes[6] = STAR_DEVICE_PXI_ROUTER_12;
    aDeviceTypes[7] = STAR_DEVICE_SPLT;
    aDeviceTypes[8] = STAR_DEVICE_PCI_MK2;
    aDeviceTypes[9] = STAR_DEVICE_CPCI_MK2;
    aDeviceTypes[10] = STAR_DEVICE_STAR_FIRE_MK3;
    aDeviceTypes[11] = STAR_DEVICE_PXI_INTERFACE_MK2;
    aDeviceTypes[12] = STAR_DEVICE_PXI_RMAP_MK2;
    aDeviceTypes[13] = STAR_DEVICE_PXI_ROUTER_MK2;

    STAR_setApplicationName("STAR-System Link Events Example Application");

    // Select device to configure
    DeviceID = STAR_UI_chooseDevice(aDeviceTypes,NUM_DEVICE_TYPES);
    TEST_ERROR_STATUS(DeviceID,"","\nChosen device has invalid ID");

    DeviceType = STAR_getDeviceType(DeviceID);
    TEST_ERROR_STATUS(DeviceType,"","\nCan't get device type");

    PrintDeviceType(DeviceID);

    // Get hardware info
    Status = CFG_getFPGAInfo(DeviceID, &FPGAInfo);
    TEST_GENERIC_ERROR_STATUS(Status,"","\nCan't get device FPGA Info");
    CFG_FPGAInfoToString(DeviceID, &FPGAInfo, sVersion, sBuildDate);

    // Display the hardware info
    printf("\nVersion: %s", sVersion);
    printf("\nBuildDate: %s\n", sBuildDate);

    if (DeviceType == STAR_DEVICE_PCIE)
    {
        if (!((FPGAInfo.major > 1) || ((FPGAInfo.major == 1) && (FPGAInfo.minor >= 11))))
        {
            TEST_ERROR_STATUS(0, "", "\nPlease upgrade this device's firmware\n");
        }
    }
    else if ((DeviceType == STAR_DEVICE_PCI_MK2) ||
             (DeviceType == STAR_DEVICE_CPCI_MK2))
    {
        if (!((FPGAInfo.major > 1) || ((FPGAInfo.major == 1) && (FPGAInfo.minor >= 10))))
        {
            TEST_ERROR_STATUS(0, "", "\nPlease upgrade this device's firmware\n");
        }
    }
    else if ((DeviceType == STAR_DEVICE_PXI_INTERFACE) ||
             (DeviceType == STAR_DEVICE_PXI_RMAP) ||
             (DeviceType == STAR_DEVICE_PXI_ROUTER_12))
    {
        if (!((FPGAInfo.major > 1) || ((FPGAInfo.major == 1) && (FPGAInfo.minor >= 1))))
        {
            TEST_ERROR_STATUS(0,"","\nPlease upgrade this device's firmware\n");
        }
    }

//----
    Status = CFG_disableInterfaceMode(DeviceID);
    TEST_GENERIC_ERROR_STATUS(Status,"","\nCan't disable interface mode");
    DelayMS(3000);                                                  // Waiting for Config Service to close channel 0

    ChannelMask = STAR_getDeviceChannels(DeviceID);
    UsableChannelsMask = (ChannelMask >> 1);                        // Step over channel 0

    RemoteChannel = 0;

    /* To receive link speed events on some devices, we must open a connection
    to device channel 0. This will prevent local configuration operations
    from being performed until we close this channel again, so we create a
    remote device for configuration operations, using a different channel, if
    necessary. */
    if (bAreLinkEventsOnChannel0Only(DeviceType) == TRUE)
    {
        Channel = 0;
        RemoteChannel = 1;

        RemoteDeviceID = GetRemoteDeviceID(DeviceID, RemoteChannel);
        TEST_ERROR_STATUS(RemoteDeviceID, "", "\nCan't get remote device ID");
    }
    else
    {
        RemoteDeviceID = DeviceID;
    }

    Index = 1;
    while (UsableChannelsMask != 0)
    {
        UsableChannelsMask = (UsableChannelsMask >> 1);

        if ((Index % 2) == 1)
        {
            TxPort = Index;
            RxPort = (Index + 1);
        }
        else
        {
            TxPort = Index;
            RxPort = (Index - 1);
        }
        Index++;

        if (bAreLinkEventsOnChannel0Only(DeviceType) == FALSE)
        {
            Channel = RxPort;
        }

        printf("\n\nTx port %d, Rx port %d, Channel %d\n", TxPort,RxPort,Channel);

        if ((DeviceType == STAR_DEVICE_PXI_INTERFACE) ||
            (DeviceType == STAR_DEVICE_PXI_RMAP) ||
            (DeviceType == STAR_DEVICE_PXI_ROUTER_12) ||
            (DeviceType == STAR_DEVICE_PXI_INTERFACE_MK2) ||
            (DeviceType == STAR_DEVICE_PXI_RMAP_MK2) ||
            (DeviceType == STAR_DEVICE_PXI_ROUTER_MK2))
        {
            DisableTriState(RemoteDeviceID, TxPort);
            DisableTriState(RemoteDeviceID, RxPort);
        }

        Status = EnableLinkEvents(RemoteDeviceID, RxPort);    // Enable Link Speed and Link State Events
        TEST_GENERIC_ERROR_STATUS(Status, "", "\nCan't enable link events");

        ChannelID = STAR_openChannelToLocalDevice(DeviceID,STAR_CHANNEL_DIRECTION_IN,Channel,TRUE);
        TEST_ERROR_STATUS(ChannelID, "", "\nCan't open channel to local device");

        pLinkEventRxOperation = STAR_createRxOperation(1,(STAR_RECEIVE_MASK)(STAR_RECEIVE_LINK_STATE_EVENTS | STAR_RECEIVE_LINK_SPEED_EVENTS));
        TEST_ERROR_STATUS(pLinkEventRxOperation, "", "\nSTAR_createRxOperation returned NULL pointer");

        Status = STAR_submitTransferOperation(ChannelID,pLinkEventRxOperation);
        TEST_ERROR_STATUS(Status, "", "\nCan't submit transfer operation");

        Status = CFG_startLink(RemoteDeviceID, TxPort);           // Remote in to start link
        sprintf(sBuffer, "\nStart link from port %i to port %i", TxPort, RxPort);
        TEST_GENERIC_ERROR_STATUS(Status, sBuffer, "\nCan't start link");

        Status = WaitEvents(ChannelID, pLinkEventRxOperation);
        TEST_ERROR_STATUS(Status, "", "\nWaitEvents failed");

        Status = SetLinkSpeed(RemoteDeviceID, TxPort, 2);
        sprintf(sBuffer, "\nSet port %i tx speed : 100 Mbit/s", TxPort);
        TEST_GENERIC_ERROR_STATUS(Status, sBuffer, "\nCan't set link speed");

        Status = WaitEvents(ChannelID, pLinkEventRxOperation);
        TEST_ERROR_STATUS(Status, "", "\nWaitEvents failed");

        Status = SetLinkSpeed(RemoteDeviceID, TxPort, 1);
        sprintf(sBuffer, "\nSet port %i tx speed : 200 Mbit/s", TxPort);
        TEST_GENERIC_ERROR_STATUS(Status, sBuffer, "\nCan't set link speed");

        Status = WaitEvents(ChannelID, pLinkEventRxOperation);
        TEST_ERROR_STATUS(Status, "", "\nWaitEvents failed");

        Status = STAR_disposeTransferOperation(pLinkEventRxOperation);
        TEST_ERROR_STATUS(Status, "", "\nCan't dispose transfer operation");

        Status = STAR_closeChannel(ChannelID);
        TEST_ERROR_STATUS(Status, "", "\nCan't close channel");

        Status = DisableLinkEvents(RemoteDeviceID, RxPort);   // Disable Link Speed and Link State Events
        TEST_GENERIC_ERROR_STATUS(Status, "", "\nCan't disable link events");

        Status = PrintLinkSpeed(RemoteDeviceID, TxPort, RxPort, "\nLink from port %i to port %i running.      Link speed measured at port %i : %d.%d Mbit/s");
        TEST_GENERIC_ERROR_STATUS(Status, "", "\nCan't measure link speed");

        Status = CFG_stopLink(RemoteDeviceID, TxPort);
        TEST_GENERIC_ERROR_STATUS(Status, "", "\nCan't stop link");
        DelayMS(1000);                                                  // Wait for link to stop

        Status = PrintLinkSpeed(RemoteDeviceID, TxPort, RxPort, "\nLink from port %i to port %i stopped.      Link speed measured at port %i : %d.%d Mbit/s");
        TEST_GENERIC_ERROR_STATUS(Status, "", "\nCan't measure link speed");
    }

    if (RemoteChannel != 0)
    {
        Status = STAR_CFG_destroyRemoteDeviceIdentifier(RemoteDeviceID);
        TEST_ERROR_STATUS(Status, "", "\nCan't destroy remote device ID");
    }

    Status = CFG_enableInterfaceMode(DeviceID);
    TEST_GENERIC_ERROR_STATUS(Status, "", "\nCan't enable interface mode");

    printf("\n\nPress Enter to exit...\n");
    getchar();
    return (0);
}

//--------------------------------------
