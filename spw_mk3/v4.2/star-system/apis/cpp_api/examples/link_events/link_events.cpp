/**
 * \file link_events.cpp
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
 * Copyright &copy; 2019 STAR-Dundee Ltd.
 *
 */

#include "link_events.h"

#ifdef _WIN32
#include "windows.h"

#define SLEEP(milliseconds) Sleep(milliseconds);
#else
#include <unistd.h>

#define SLEEP(milliseconds) usleep(milliseconds * 1000);
#endif

#include "star_system.hpp"
#include "device_list.hpp"
#include "device_configuration.hpp"
#include "remote_device.hpp"
#include "address.hpp"
#include "link_port.hpp"
#include "link_port_status.hpp"
#include "channel.hpp"
#include "receive_operation.hpp"
#include "brick_mk2_device_configuration.hpp"
#include "brick_mk3_device_configuration.hpp"
#include "pcie_device_configuration.hpp"
#include "pxi_device_configuration.hpp"
#include "pci_mk2_device_configuration.hpp"
#include "splt_device_configuration.hpp"

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::transferoperations;
using namespace stardundee::com::starsystem::deviceconfig;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices::brickmk2;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices::brickmk3;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices::pcie;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices::pxi;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices::pcimk2;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices::splt;

#define NUM_DEVICE_TYPES    (14)

bool LinkEvents::AreLinkEventsOnChannel0Only(U32 deviceType)
{
    /* check if device is one that only supports link events on channel 0 */
    if ((deviceType == STAR_DEVICE_BRICK_MK2) ||
        (deviceType == STAR_DEVICE_BRICK_MK3) ||
        (deviceType == STAR_DEVICE_ROUTER_MK2S) ||
        (deviceType == STAR_DEVICE_SPLT) ||
        (deviceType == STAR_DEVICE_STAR_FIRE_MK3))
    {
        return true;
    }
    return false;
}

Device* LinkEvents::ChooseDevice()
{
    int status;
    char deviceName[Device::STR_DEVICE_NAME_LEN];
    size_t stringLen = 0;
    U32 deviceCount = 0;
    DeviceList deviceList;
    Device* deviceSelected = NULL;
    U32 i;
    unsigned int chosen;
    char s[256];

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

    deviceList.PopulateForTypes(aDeviceTypes, NUM_DEVICE_TYPES);

    /* get the list of devices present for all drivers */
    deviceCount = deviceList.Populate();

    /* if there are no devices present */
    if (!deviceCount)
    {
        /* report no devices available */
        puts("No SpaceWire devices detected!");

        /* return null / empty device */
        return NULL;
    }

    /* display the number of devices detected */
    if (deviceCount == 1)
    {
        printf("One device detected:");
    }
    else
    {
        printf("%d devices detected:\n", deviceCount);
    }

    /* for each device */
    for (i = 0; i < deviceCount; i++)
    {
        /* if device's ID is valid */
        if (deviceList[i]->GetDeviceID())
        {
            /* get the device's name */
            stringLen = deviceList[i]->GetDeviceName(deviceName);

            /* if device name available */
            if (stringLen)
            {
                /* display it */
                printf("\t%d - %s\n", i, deviceName);
            }
            else
            {
                /* display error msg */
                printf("\t%d - Unknown SpaceWire Device\n", i);
            }
        }
        else
        {
            /* display - invalid or inaccessible device */
            printf("\t%d - Unable to access device\n", i);
        }
    }

    /* if there's only 1 device on the list */
    if (deviceCount == 1)
    {
        /* make this the selected device to return */

        /* make a clone/copy of the device on the list which is being
         * selected (the device list itself has to be freed later)
         */
        deviceSelected = deviceList.CopyDeviceAt(0);
    }
    else
    {
        /* ask the user which device to use */
        printf("Please select which device to use: ");

        /* if failed to get device number */
        if (!fgets(s, 256, stdin))
        {
            /* display error message */
            puts("No device number selected.");
        }
        else
        {
            /* attempt to parse selection entered */
            status = sscanf(s, "%u", &chosen);

            /* if failed to read selection entered or invalid selection */
            if ((!status) || (chosen > deviceCount - 1))
            {
                /* display error message */
                puts("Incorrect device number selected.");
            }
            else
            {
                /* set selected device to match the user's chosen selection */

                /* make a clone/copy of the relevant device on the list (the
                 * device list itself has to be freed later)
                 */
                deviceSelected = deviceList.CopyDeviceAt(chosen);
            }
        }
    }

    /* dispose of the device list */
    /* \note This is not required, the DeviceList object destructor will
     * free the contents of the list if they haven't been disposed of
     * already.
     */
    deviceList.Empty();

    /* return the selected device */
    return deviceSelected;
}

int LinkEvents::WaitForEvents(Channel *pChannel,
    ReceiveOperation *pRxOperation)
{
    int status = 0;
    U32 index;

    /* wait for incoming link events */
    TransferOperation::TransferStatus transferOperationStatus;
    do
    {
        transferOperationStatus =
            pRxOperation->WaitOnTransferOperationCompletion(2000);
        switch (transferOperationStatus)
        {
            case TransferOperation::TRANSFER_STATUS_COMPLETE:
            {
                /* loop over link events and print their status */
                U32 itemCount = pRxOperation->GetTransferItemCount();
                for (index = 0; index < itemCount; index++)
                {
                    StreamItem *pStreamItem = pRxOperation->GetTransferItem(
                        index);

                    if (pStreamItem->GetStreamItemType() ==
                        StreamItem::STREAM_ITEM_TYPE_LINK_STATE_EVENT)
                    {
                        LinkStateEvent *pLinkStateEvent =
                            (LinkStateEvent *)pStreamItem;
                        int isLinkRunning = pLinkStateEvent->IsLinkRunning();
                        if (isLinkRunning == FALSE)
                        {
                            printf("\nLink state event:  Link not running.");
                        }
                        else
                        {
                            printf("\nLink state event:  Link running.");
                        }
                    }
                    else if (pStreamItem->GetStreamItemType() ==
                        StreamItem::STREAM_ITEM_TYPE_LINK_SPEED_EVENT)
                    {
                        LinkSpeedEvent *pLinkSpeedEvent =
                            (LinkSpeedEvent *)pStreamItem;
                        U32 newLinkSpeed = pLinkSpeedEvent->GetLinkSpeed();
                        printf(
                            "\nLink speed event:  New Link Speed %u.%u Mbit/s",
                            newLinkSpeed / 1000000,
                            (newLinkSpeed % 1000000) / 100000);
                    }

                    delete pStreamItem;
                }
                break;
            }
            case TransferOperation::TRANSFER_STATUS_STARTED:
            {
                printf("\nNo more events\n");
                status = pRxOperation->CancelTransferOperation();
                break;
            }
            case
            TransferOperation::TRANSFER_STATUS_NOT_STARTED:
            {
                printf(
                    "\nTransfer not started and link speed event did not "
                    "occur");
                status = pRxOperation->CancelTransferOperation();
                break;
            }
            case TransferOperation::TRANSFER_STATUS_CANCELLED:
            {
                break;
            }
            case TransferOperation::TRANSFER_STATUS_ERROR:
            default:
            {
                printf("\nError occurred while waiting for link speed event");
                break;
            }
        }

        if (transferOperationStatus !=
            TransferOperation::TRANSFER_STATUS_STARTED)
        {
            status = pChannel->SubmitTransferOperation(pRxOperation);
        }
    }
    while (transferOperationStatus !=
        TransferOperation::TRANSFER_STATUS_STARTED);

    return status;
}

int LinkEvents::SetLinkSpeed(Device *pDevice, U32 deviceType,
    HardwareInfo *pHardwareInfo, U8 port, U8 divisor)
{
    int status = 0;

    DeviceConfiguration *pDeviceConfiguration =
        pDevice->GetDeviceConfiguration();

    /* if Brick Mk2 or Router Mk2S device */
    if ((deviceType == STAR_DEVICE_BRICK_MK2) ||
        (deviceType == STAR_DEVICE_ROUTER_MK2S))
    {
        BrickMk2DeviceConfiguration *pBrickMk2Config =
            (BrickMk2DeviceConfiguration *)pDeviceConfiguration;

        status = pBrickMk2Config->SetLinkClockFrequency(port,
            BrickMk2LinkPort::LINK_FREQUENCY_200MHZ);
        if (status)
        {
            status = pBrickMk2Config->SetLinkRateDivider(port, divisor);
        }
    }
    /* else if PCIe device */
    else if (deviceType == STAR_DEVICE_PCIE)
    {
        ClockRateParams clockRateParams;
        clockRateParams.SetMultiplier(2);
        clockRateParams.SetDivisor(2 * divisor);

        PCIeDeviceConfiguration *pPCIeConfig =
            (PCIeDeviceConfiguration *)pDeviceConfiguration;

        if ((pHardwareInfo->GetMajor() > 1) ||
            ((pHardwareInfo->GetMajor() == 1) &&
            (pHardwareInfo->GetMinor() >= 11)))
        {
            status = pPCIeConfig->SetTransmitClock(port, clockRateParams);
        }
        else
        {
            status = pPCIeConfig->SetBaseTransmitClock(port, clockRateParams);
            if (status)
            {
                status = pPCIeConfig->SetLinkRateDivider(port, 1);
            }
        }
    }
    /* else if PCI Mk2 or cPCI Mk2 device */
    else if ((deviceType == STAR_DEVICE_PCI_MK2) ||
        (deviceType == STAR_DEVICE_CPCI_MK2))
    {
        PCIMk2DeviceConfiguration *pPCIMk2Config =
            (PCIMk2DeviceConfiguration *)pDeviceConfiguration;

        status = pPCIMk2Config->SetLinkClockFrequency(port,
            PCIMk2LinkPort::LINK_FREQUENCY_200MHZ);
        if (status)
        {
            status = pPCIMk2Config->SetLinkRateDivider(port, divisor);
        }
    }
    /* else if SPLT device */
    else if (deviceType == STAR_DEVICE_SPLT)
    {
        SPLTDeviceConfiguration *pSPLTConfig =
            (SPLTDeviceConfiguration *)pDeviceConfiguration;

        ClockRateParams clockRateParams;
        clockRateParams.SetMultiplier(2);
        clockRateParams.SetDivisor(2 * divisor);

        status = pSPLTConfig->SetTransmitClock(port, clockRateParams);
    }
    /* else if Brick Mk3 device */
    else if (deviceType == STAR_DEVICE_BRICK_MK3)
    {
        BrickMk3DeviceConfiguration *pBrickMk3Config =
            (BrickMk3DeviceConfiguration *)pDeviceConfiguration;

        ClockRateParams clockRateParams;
        clockRateParams.SetMultiplier(2);
        clockRateParams.SetDivisor(2 * divisor);

        if ((pHardwareInfo->GetMajor() > 1) ||
            ((pHardwareInfo->GetMajor() == 1) &&
            (pHardwareInfo->GetMinor() >= 1)))
        {
            status = pBrickMk3Config->SetTransmitClock(port, clockRateParams);
        }
        else
        {
            status = pBrickMk3Config->SetBaseTransmitClock(port,
                clockRateParams);
            if (status)
            {
                status = pBrickMk3Config->SetLinkRateDivider(port, 1);
            }
        }
    }
    /* else if STAR Fire Mk3 device */
    else if (deviceType == STAR_DEVICE_STAR_FIRE_MK3)
    {
        BrickMk3DeviceConfiguration *pBrickMk3Config =
            (BrickMk3DeviceConfiguration *)pDeviceConfiguration;

        ClockRateParams clockRateParams;
        clockRateParams.SetMultiplier(2);
        clockRateParams.SetDivisor(2 * divisor);

        status = pBrickMk3Config->SetTransmitClock(port, clockRateParams);
    }
    /* else if PXI Interface device */
    else if ((deviceType == STAR_DEVICE_PXI_INTERFACE) ||
        (deviceType == STAR_DEVICE_PXI_RMAP))
    {
        PXIDeviceConfiguration *pPXIConfig =
            (PXIDeviceConfiguration *)pDeviceConfiguration;

        ClockRateParams clockRateParams;
        clockRateParams.SetMultiplier(2);
        clockRateParams.SetDivisor(2 * divisor);

        if ((pHardwareInfo->GetMajor() > 1) ||
            ((pHardwareInfo->GetMajor() == 1) &&
            (pHardwareInfo->GetMinor() >= 1)))
        {
            status = pPXIConfig->SetTransmitClock(port, clockRateParams);
        }
        else
        {
            status = pPXIConfig->SetBaseTransmitClock(port, clockRateParams);
            if (status)
            {
                status = pPXIConfig->SetLinkRateDivider(1);
            }
        }
    }
    /* else if PXI Router device */
    else if (deviceType == STAR_DEVICE_PXI_ROUTER_12)
    {
        PXIDeviceConfiguration *pPXIConfig =
            (PXIDeviceConfiguration *)pDeviceConfiguration;

        ClockRateParams clockRateParams;
        clockRateParams.SetMultiplier(2);
        clockRateParams.SetDivisor(2 * divisor);

        if ((port > 0) && ((port % 2) == 0))
        {
            port--;
        }

        if ((pHardwareInfo->GetMajor() > 1) ||
            ((pHardwareInfo->GetMajor() == 1) &&
            (pHardwareInfo->GetMinor() >= 1)))
        {
            status = pPXIConfig->SetTransmitClock(port, clockRateParams);
        }
        else
        {
            status = pPXIConfig->SetBaseTransmitClock(port, clockRateParams);
            if (status)
            {
                status = pPXIConfig->SetLinkRateDivider(1);
            }
        }
    }
    /* else if PXI Interface Mk2 device */
    else if ((deviceType == STAR_DEVICE_PXI_INTERFACE_MK2) ||
        (deviceType == STAR_DEVICE_PXI_RMAP_MK2))
    {
        PXIDeviceConfiguration *pPXIConfig =
            (PXIDeviceConfiguration *)pDeviceConfiguration;

        ClockRateParams clockRateParams;
        clockRateParams.SetMultiplier(2);
        clockRateParams.SetDivisor(2 * divisor);

        status = pPXIConfig->SetTransmitClock(port, clockRateParams);
    }
    /* else if PXI Router Mk2 device */
    else if (deviceType == STAR_DEVICE_PXI_ROUTER_MK2)
    {
        PXIDeviceConfiguration *pPXIConfig =
            (PXIDeviceConfiguration *)pDeviceConfiguration;

        ClockRateParams clockRateParams;
        clockRateParams.SetMultiplier(2);
        clockRateParams.SetDivisor(2 * divisor);

        if ((port > 0) && ((port % 2) == 0))
        {
            port--;
        }

        status = pPXIConfig->SetTransmitClock(port, clockRateParams);
    }

    delete pDeviceConfiguration;

    return status;
}

int LinkEvents::PrintLinkSpeed(Device *pDevice, U8 port, const char *pText)
{
    Mk2DeviceConfiguration *pMk2Config =
        (Mk2DeviceConfiguration *)pDevice->GetDeviceConfiguration();

    /* get the measured link speed and output it */
    U16 measuredLinkSpeed;
    int status = pMk2Config->GetMeasuredLinkSpeed(port, &measuredLinkSpeed);
    if (status != 0)
    {
        U32 tempLinkSpeed =
            (measuredLinkSpeed * STAR_CFG_MK2_LINK_SPEED_UNITS_KBPS);
        printf(pText, tempLinkSpeed / 1000, (tempLinkSpeed % 1000) / 100);
    }

    delete pMk2Config;
    return status;
}

int LinkEvents::TestLinkEvents(Device *pDevice, Device *pRemoteDevice,
    Mk2DeviceConfiguration *pRemoteConfig,  U32 deviceType, U8 channelNumber,
    HardwareInfo *pHardwareInfo)
{
    int status = 1;

    U8 txPort;
    U8 rxPort;

    /* step over channel 0 */
    U32 channelMask = pDevice->GetDeviceChannels();
    U32 usableChannelsMask = (channelMask >> 1);

    /* loop over all usable channels */
    U32 index = 1;
    while (usableChannelsMask != 0)
    {
        /* shift to next channel */
        usableChannelsMask = (usableChannelsMask >> 1);

        /* update transmit and receive ports based on current index */
        if ((index % 2) == 1)
        {
            txPort = index;
            rxPort = (index + 1);
        }
        else
        {
            txPort = index;
            rxPort = (index - 1);
        }
        index++;

        /* update channel number if device supports time-codes on port */
        if (!AreLinkEventsOnChannel0Only(deviceType))
        {
            channelNumber = rxPort;
        }

        printf("\n\nTx port %d, Rx port %d, Channel %d\n", txPort, rxPort,
            channelNumber);

        /* if this is a PXI device */
        if ((deviceType == STAR_DEVICE_PXI_INTERFACE) ||
            (deviceType == STAR_DEVICE_PXI_RMAP) ||
            (deviceType == STAR_DEVICE_PXI_ROUTER_12) ||
            (deviceType == STAR_DEVICE_PXI_INTERFACE_MK2) ||
            (deviceType == STAR_DEVICE_PXI_RMAP_MK2) ||
            (deviceType == STAR_DEVICE_PXI_ROUTER_MK2))
        {
            /* disable tri-state for transmit port */
            LinkPort *pTxPort = (LinkPort *)pRemoteConfig->GetPort(txPort);
            LinkStatus *pTxPortStatus = (LinkStatus *)pTxPort->GetStatus();
            pTxPortStatus->SetTriState(0);
            status = pTxPort->SetSpaceWireLinkStatus(pTxPortStatus);
            if (!status)
            {
                printf("\nError disabling tri-state on port: %d\n",
                    pTxPort->GetPortNumber());
                return status;
            }

            /* disable tri-state for receive port */
            LinkPort *pRxPort = (LinkPort *)pRemoteConfig->GetPort(rxPort);
            LinkStatus *pRxPortStatus = (LinkStatus *)pRxPort->GetStatus();
            pRxPortStatus->SetTriState(0);
            status = pRxPort->SetSpaceWireLinkStatus(pRxPortStatus);
            if (!status)
            {
                printf("\nError disabling tri-state on port: %d\n",
                    pRxPort->GetPortNumber());
                return status;
            }
        }

        /* enable Link Speed and Link State events on receive port */
        status = pRemoteConfig->EnableSpeedChangeEventsOnPort(rxPort);
        if (!status)
        {
            printf("\nError enabling speed change events on port: %d\n",
                rxPort);
            return status;
        }
        status = pRemoteConfig->EnableStateChangeEventsOnPort(rxPort);
        if (!status)
        {
            printf("\nError enabling state change events on port: %d\n",
                rxPort);
            return status;
        }

        /* open the channel for receiving events on */
        Channel channel = pDevice->OpenChannel(Channel::DIRECTION_IN,
            channelNumber, 1);

        /* create receive operation for events */
        ReceiveOperation receiveOperation;
        status = receiveOperation.CreateRxOperation(1,
            (STAR_RECEIVE_MASK)(STAR_RECEIVE_LINK_STATE_EVENTS |
                STAR_RECEIVE_LINK_SPEED_EVENTS));
        if (!status)
        {
            printf("\nError creating receive operation\n");
            return status;
        }

        /* submit receive operation */
        status = channel.SubmitTransferOperation(&receiveOperation);
        if (!status)
        {
            printf("\nError submitting receive operation\n");
            return status;
        }

        /* start transmit port */
        status = pRemoteConfig->StartLink(txPort);
        if (!status)
        {
            printf("\nError starting link on port: %d\n", txPort);
            return status;
        }

        /* wait for events to be received */
        status = WaitForEvents(&channel, &receiveOperation);
        if (!status)
        {
            printf("\nError waiting for events\n");
            return status;
        }

        /* set link speed on transmit port to cause an event on receive port */
        status = SetLinkSpeed(pRemoteDevice, deviceType, pHardwareInfo, 
            txPort, 2);
        if (status)
        {
            printf("\nSet Link Speed : 100 Mbit/s\n");
        }
        else
        {
            printf("\nError setting link speed on port: %d\n", txPort);
            return status;
        }

        /* wait for events to be received */
        status = WaitForEvents(&channel, &receiveOperation);
        if (!status)
        {
            printf("\nError waiting for events\n");
            return status;
        }

        /* set link speed on transmit port to cause an event on receive port */
        status = SetLinkSpeed(pRemoteDevice, deviceType, pHardwareInfo, 
            txPort, 1);
        if (status)
        {
            printf("\nSet Link Speed : 200 Mbit/s\n");
        }
        else
        {
            printf("\nError setting link speed on port: %d\n", txPort);
            return status;
        }

        /* wait for events to be received */
        status = WaitForEvents(&channel, &receiveOperation);
        if (!status)
        {
            printf("\nError waiting for events\n");
            return status;
        }

        /* dispose of receive operation */
        status = receiveOperation.DisposeTransferOperation();
        if (!status)
        {
            printf("\nError disposing of transfer operation\n");
            return status;
        }

        /* close the channel */
        status = channel.CloseChannel();
        if (!status)
        {
            printf("\nError closing channel\n");
            return status;
        }

        /* disable Link Speed and Link State events */
        status = pRemoteConfig->DisableSpeedChangeEventsOnPort(rxPort);
        if (!status)
        {
            printf("\nError disabling speed change events on port: %d\n",
                rxPort);
            return status;
        }
        status = pRemoteConfig->DisableStateChangeEventsOnPort(rxPort);
        if (!status)
        {
            printf("\nError disabling state change events on port: %d\n",
                rxPort);
            return status;
        }

        /* print link speed before stopping link */
        status = PrintLinkSpeed(pRemoteDevice, rxPort,
            "\nLink ""running.      Measured link speed: %d.%d Mbit/s\n");

        /* stop the link */
        status = pRemoteConfig->StopLink(txPort);
        if (!status)
        {
            printf("\nError stopping link: %d\n", rxPort);
            return status;
        }

        /* wait for link to stop */
        SLEEP(1000);

        /* print link speed after stopping link */
        status = PrintLinkSpeed(pRemoteDevice, rxPort,
            "Link stopped.      Measured link speed: %d.%d Mbit/s\n");
    }

    return status;
}

int LinkEvents::TestDevice(Device *pDevice, Mk2DeviceConfiguration *pConfig)
{
    int status;

    UNREFERENCED_PARAMETER(pConfig);

    /* validate device and configuration */
    if (!pDevice && !pConfig)
    {
        return 0;
    }

    STARSystem::SetApplicationName(
        (char *)"STAR-System Link Events Example Application");

    /* get device type */
    char deviceTypeString[Device::STR_DEVICE_TYPE_LEN];
    pDevice->GetDeviceTypeAsString(deviceTypeString);

    /* print device type */
    printf("\nTesting the %s\n", deviceTypeString);

    /* get hardware info data */
    HardwareInfo hardwareInfo;
    status = pConfig->GetHardwareInfo(hardwareInfo);
    if (!status)
    {
        printf("\nError getting hardware info\n");
        return 0;
    }

    /* get hardware info string */
    char hardwareVersionString[Mk2DeviceConfiguration::STR_VERSION_LEN];
    char hardwareBuildDateString[Mk2DeviceConfiguration::STR_BUILD_DATE_LEN];
    status = pConfig->GetHardwareInfoAsString(hardwareVersionString,
        hardwareBuildDateString);
    if (!status)
    {
        printf("\nError getting hardware info\n");
        return 0;
    }

    /* display the hardware info */
    printf("\nVersion: %s", hardwareVersionString);
    printf("\nBuildDate: %s\n", hardwareBuildDateString);

    /* get the device type */
    STAR_DEVICE_TYPE deviceType = pDevice->GetDeviceType();

    /* validate device firmware version */
    if (deviceType == STAR_DEVICE_PCIE)
    {
        if (!((hardwareInfo.GetMajor() > 1) ||
            ((hardwareInfo.GetMajor() == 1) &&
            (hardwareInfo.GetMinor() >= 11))))
        {
            printf("\nPlease upgrade this device's firmware\n");
            return 0;
        }
    }
    else if ((deviceType == STAR_DEVICE_PCI_MK2) ||
        (deviceType == STAR_DEVICE_CPCI_MK2))
    {
        if (!((hardwareInfo.GetMajor() > 1) ||
            ((hardwareInfo.GetMajor() == 1) &&
            (hardwareInfo.GetMinor() >= 10))))
        {
            printf("\nPlease upgrade this device's firmware\n");
            return 0;
        }
    }
    else if ((deviceType == STAR_DEVICE_PXI_INTERFACE) ||
        (deviceType == STAR_DEVICE_PXI_RMAP) ||
        (deviceType == STAR_DEVICE_PXI_ROUTER_12))
    {
        if (!((hardwareInfo.GetMajor() > 1) ||
            ((hardwareInfo.GetMajor() == 1) &&
            (hardwareInfo.GetMinor() >= 1))))
        {
            printf("\nPlease upgrade this device's firmware\n");
            return 0;
        }
    }

    /* disable interface mode */
    status = pConfig->DisableInterfaceMode();
    if (!status)
    {
        printf("\nError disabling interface mode\n");
    }

    /* waiting for Config Service to close channel 0 */
    SLEEP(3000);

    /* pointer to remote device (or normal device if remote not required) */
    Device *pRemoteDevice;
    RemoteDevice remoteDevice;
    pRemoteDevice = (Device *)&remoteDevice;

    /* To receive link speed events on some devices, we must open a connection
     * to device channel 0. This will prevent local configuration operations
     * from being performed until we close this channel again, so we create a
     * remote device for configuration operations, using a different channel,
     * if necessary.
     */

    /* if this device reports link events on channel 0 */
    if (AreLinkEventsOnChannel0Only(deviceType))
    {
        unsigned char pathTo[] = { 0,254 };
        unsigned char returnPath[] = { 254 };

        Address pathToAddress;
        pathToAddress.CreateAddress(pathTo, 2);
        Address returnPathAddress;
        returnPathAddress.CreateAddress(returnPath, 1);

        /* create remote device */
        status = remoteDevice.CreateRemoteDevice(pDevice, 1, NULL,
            &pathToAddress, &returnPathAddress);
        if (!status)
        {
            printf("\nError creating remote device\n");
        }
    }
    else
    {
        /* use normal rather than remote as events not only on channel 0 */
        pRemoteDevice = pDevice;
    }

    if (status)
    {
        /* test events for the selected device */
        Mk2DeviceConfiguration *pRemoteDeviceConfig =
            (Mk2DeviceConfiguration *)pRemoteDevice->GetDeviceConfiguration();
        status = TestLinkEvents(pDevice, pRemoteDevice, 
            pRemoteDeviceConfig, deviceType, 0, &hardwareInfo);
        delete pRemoteDeviceConfig;
    }

    return status;
}

int LinkEvents::DoLinkEventsExample()
{
    int status;

    /* select device to configure */
    Device *pDevice = ChooseDevice();

    if (pDevice == NULL)
    {
        return 0;
    }

    /* get device configuration */
    Mk2DeviceConfiguration *pConfig = (Mk2DeviceConfiguration *)pDevice->
        GetDeviceConfiguration();

    /* run the link events test on the chosen device */
    status = TestDevice(pDevice, pConfig);

    /* clean-up resources */
    delete pConfig;
    delete pDevice;

    if (status)
    {
        return 0;
    }

    return 1;
}

