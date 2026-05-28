/**
 * \file star-test.c
 *
 * \brief Functions for the STAR-System Test program.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Contains the bulk of functions used by the STAR-System Test program. Provided
 * as an example for developers wishing to write programs using STAR-API, the
 * STAR-Dundee software API.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "star-test.h"
#include "star-api.h"
#include "cfg_api_router.h"
#include "cfg_api_mk2.h"
 

#define VERSION_INFO "star-system_test v2.0"

#include <errno.h>


/**
 * \mainpage
 *
 * The STAR-System Test provides example code to perform a number of common
 * tasks using STAR-System, the STAR-Dundee API.  These include transmitting and
 * receiving packets and displaying version information.
 */

/**
 * \defgroup StarSystemTester STAR-System Test
 * This section contains the functions and types provided in the STAR-System
 * Test program.
 */

/* Menu option codes */
#define MENU_DISPLAY_INFORMATION            1
#define MENU_LOOPBACK_SINGLE                2
#define MENU_LOOPBACK_MULTI                 3
#define MENU_LOOPBACK_DOUBLE_SINGLE         4
#define MENU_LOOPBACK_DOUBLE_MULTI          5

#define MENU_TRANSMIT_SINGLE                6
#define MENU_TRANSMIT_MULTI                 7
#define MENU_RECEIVE_SINGLE                 8
#define MENU_RECEIVE_MULTI                  9
#define MENU_TRANSMIT_FILE_WHOLE           10
#define MENU_RECEIVE_FILE_WHOLE            11
#define MENU_TRANSMIT_FILE_SPLIT           12
#define MENU_RECEIVE_FILE_SPLIT            13
#define MENU_RESET_DEVICE                  14
#define MENU_IDENTIFY_DEVICE               15

#define    MENU_EXIT                       0

/**
 * \ingroup StarSystemTester
 * Wait indefinitely.
 */
#define STAR_INFINITE -1

 
/**
 * \ingroup StarSystemTester
 * Display the options menu.
 */
void DisplayMenu(void)
{
    puts("\nSelect Option:");
    printf(" (%d)  Display Device and Version Information\n",
        MENU_DISPLAY_INFORMATION);
    printf(" (%d)  Loopback Test, Single Packet (Data Compare, Link Speed)\n",
        MENU_LOOPBACK_SINGLE);
    printf(" (%d)  Loopback Test, Multiple Packets (Data Compare, Link Speed)\n",
        MENU_LOOPBACK_MULTI);
    printf(" (%d)  Double Loopback Test, Single Packet (Data Compare, Link Speed)\n",
        MENU_LOOPBACK_DOUBLE_SINGLE);
    printf(" (%d)  Double Loopback Test, Multiple Packets (Data Compare, Link Speed)\n",
        MENU_LOOPBACK_DOUBLE_MULTI);
    printf(" (%d)  Transmit Single Packet (Link Speed)\n", MENU_TRANSMIT_SINGLE);
    printf(" (%d)  Transmit Multiple Packets (Link Speed)\n",
        MENU_TRANSMIT_MULTI);
    printf(" (%d)  Receive Packet (Link Speed)\n", MENU_RECEIVE_SINGLE);
    printf(" (%d)  Receive Multiple Packets (Link Speed)\n", MENU_RECEIVE_MULTI);
    printf("(%d)  Transmit From File, Single Packet\n", MENU_TRANSMIT_FILE_WHOLE);
    printf("(%d)  Receive To File, Single Packet \n", MENU_RECEIVE_FILE_WHOLE);
    printf("(%d)  Transmit From File, Multiple Packets \n", MENU_TRANSMIT_FILE_SPLIT);
    printf("(%d)  Receive To File, Multiple Packets (Link Speed) \n", MENU_RECEIVE_FILE_SPLIT);
    printf("(%d)  Reset Device\n", MENU_RESET_DEVICE);
    printf("(%d)  Identify Device\n", MENU_IDENTIFY_DEVICE);
    printf(" (%d)  Exit\n", MENU_EXIT);
    printf("Please Select Menu Option: ");
    fflush(stdout);
}

/**
 * \ingroup StarSystemTester
 * Display version information for all the API modules and drivers currently in
 * use, and the properties of all the currently connected devices.
 */
void DisplayInformation(void)
{
    STAR_VERSION_INFO *versions;
    STAR_CFG_MK2_HARDWARE_INFO fpgaVersion;
    int getFpgaResult = 0;

    U32 versionCount = 0U;

    STAR_DEVICE_ID* devices;
    U32 deviceCount = 0U;

    char *deviceStr;
    STAR_CHANNEL_MASK channelMask;
    unsigned int deviceNum, i;

    STAR_CFG_DEVICE_IDENTIFIER_INFO deviceInfo;
    char manufacturerStr[STAR_CFG_MANUFACTURER_STR_MAX_LEN];
    char deviceTypeStr[STAR_CFG_DEVICE_STR_MAX_LEN];
    STAR_DEVICE_TYPE configCapabilities;

    puts("\n\nMODULE VERSION INFORMATION");
    puts("==========================");

    /* Get the list of module versions */
    versions = STAR_getAllVersions(&versionCount);

    /* If there are modules present */
    if (versions != NULL)
    {
        /* For each module version item in the list */
        for (i = 0U; i < versionCount; i++)
        {
            /* Display the module's version information */
            printf("   Module name: %s\n",   versions[i].name);
            printf(" Module author: %s\n",   versions[i].author);
            printf("Module version: v%u.%u", versions[i].major,
                versions[i].minor);
            if (versions[i].edit != 0U)
            {
                printf("(%u)", versions[i].edit);
            }
            if (versions[i].patch != 0U)
            {
                printf("p%u", versions[i].patch);
            }
            puts("\n");
        }

        /* Dispose of the version list */
        STAR_destroyVersionInfoList(versions);
    }
    else
    {
        puts("No modules present.\n");
    }

    puts("\nDEVICE INFORMATION");
    puts("==================");

    /* Get the list of devices */
    devices = STAR_getDeviceList(&deviceCount);

    /* If there are devices present */
    if (devices != NULL)
    {
        /* For each device in the list */
        for (deviceNum = 0U; deviceNum < deviceCount; deviceNum++)
        {
            /* Get and display the device's name */
            deviceStr = STAR_getDeviceName(devices[deviceNum]);
            if (deviceStr == NULL)
            {
                puts("            Name: Unable to get device name!");
            }
            else
            {
                printf("            Name: %s\n", deviceStr);
                STAR_destroyString(deviceStr);
            }

            /* Get and display the device's type */
            deviceStr = STAR_getDeviceTypeAsString(devices[deviceNum]);
            if (deviceStr == NULL)
            {
                puts("     Device type: Unable to get device type!");
            }
            else
            {
                printf("     Device type: %s\n", deviceStr);
                STAR_destroyString(deviceStr);
            }

            /** Get and display the device's bus type */
            deviceStr = STAR_getDeviceBusTypeAsString(devices[deviceNum]);
            if (deviceStr == NULL)
            {
                puts("        Bus type: Unable to get bus type!");
            }
            else
            {
                printf("        Bus type: %s\n", deviceStr);
                STAR_destroyString(deviceStr);
            }

            /* Get and display the device's firmware version */
            versions = STAR_getDeviceFirmwareVersion(devices[deviceNum]);
            if (versions == NULL)
            {
                puts("Firmware version: Unable to get the firmware version!");
            }
            else
            {
                printf("Firmware version: v%u.%u", versions->major,
                    versions->minor);
                if (versions->edit != 0U)
                {
                    printf("(%u)", versions->edit);
                }
                if (versions->patch != 0U)
                {
                    printf("p%u", versions->patch);
                }
                puts("");
                STAR_destroyVersionInfo(versions);
            }

            /* If the device can be configured */
            configCapabilities = STAR_getDeviceConfigCapabilities(
                devices[deviceNum]);
            if (configCapabilities == STAR_DEVICE_CONFIG_SUPPORTED)
            {
                /* Get and display the device's FPGA version */
                getFpgaResult = CFG_MK2_getHardwareInfo(devices[deviceNum],
                    &fpgaVersion);
                if (getFpgaResult == 0)
                {
                    puts("    FPGA version: Unable to get the FPGA version!");
                }
                else
                {
                    printf("    FPGA version: v%u.%02u", fpgaVersion.major,
                        fpgaVersion.minor);
                    if (fpgaVersion.edit != 0U)
                    {
                        printf("(%u)", fpgaVersion.edit);
                    }
                    if (fpgaVersion.patch != 0U)
                    {
                        printf("p%u", fpgaVersion.patch);
                    }
                    puts("");
                }
            }

            /* Get and display the device's serial number */
            deviceStr = STAR_getDeviceSerialNumber(devices[deviceNum]);
            if (deviceStr == NULL)
            {
                puts("   Serial number: Unable to get serial number!");
            }
            else
            {
                printf("   Serial number: %s\n", deviceStr);
                STAR_destroyString(deviceStr);
            }

            /* Get and display the device's index */
            printf("           Index: %d\n",
                STAR_getDeviceIndex(devices[deviceNum]));

            /* Get and display the device's channels */
            printf("        Channels:");
            channelMask = STAR_getDeviceChannels(devices[deviceNum]);
            if (channelMask == 0U)
            {
                puts(" None");
            }
            else
            {
                for (i = 0U; i < 32U; i++)
                {
                    if (((channelMask >> i) & 1U) != 0U)
                    {
                        printf(" %u", i);
                    }
                }
                printf("\n");
            }

            if (configCapabilities == STAR_DEVICE_CONFIG_SUPPORTED)
            {
                memset(&deviceInfo, 0, sizeof(deviceInfo));
                if (CFG_ROUTER_getDeviceIdentificationInfo(devices[deviceNum],
                    &deviceInfo) == 0)
                {
                    strcpy(manufacturerStr, "Unable to get the manufacturer!");
                    strcpy(deviceTypeStr, "Unable to get the chip type!");
                }
                else
                {
                    CFG_ROUTER_getDeviceManufacturerAsString(&deviceInfo,
                        manufacturerStr);
                    CFG_ROUTER_getDeviceTypeAsString(&deviceInfo,
                        deviceTypeStr);
                }
                printf(" Manufacturer ID: %u\t(%s)\n",
                    deviceInfo.manufacturerID, manufacturerStr);
                printf("       Chip Type: %u\t(%s)\n", deviceInfo.chipType,
                    deviceTypeStr);
            }
            puts("\n");
        }

        /* Dispose of the device list */
        STAR_destroyDeviceList(devices);
    }
    else
    {
        puts("No devices present.\n");
    }

    puts("\n");
}

/**
 * \ingroup StarSystemTester
 * Prompts the user to choose which device to use for the test if more than one
 * device is available.
 *
 * @param descriptionStr the description of what the device is to be used for
 *                       (e.g. transmit or receive)
 * @param deviceType the type of device that can be opened
 *
 * @return the identifier of the device to be used for the test, or 0 if there
 *         was an error or no devices are present
 */
STAR_DEVICE_ID chooseDevice(const char * const descriptionStr,
    const STAR_DEVICE_TYPE deviceType)
{
    STAR_DEVICE_ID* devices;
    U32 deviceCount = 0U, i;
    unsigned int chosen;
    int status;

    STAR_DEVICE_ID deviceId;
    char *deviceName, s[256];

    /* Get the list of devices of the specified type */
    devices = STAR_getDeviceListForType(deviceType, &deviceCount);

    /* If there are no devices present */
    if (devices == NULL)
    {
        /* Return an error */
        puts("No SpaceWire devices detected!");
        return STAR_DEVICE_UNKNOWN;
    }

    /* Display the devices detected */
    if (deviceCount == 1U)
    {
        printf("One %s device detected:", descriptionStr);
    }
    else
    {
        printf("%u %s devices detected:\n", deviceCount, descriptionStr);
    }

    /* For each device */
    for (i = 0U; i < deviceCount; i++)
    {
        /* Display its name */
        if (devices[i] != STAR_DEVICE_UNKNOWN)
        {
            deviceName = STAR_getDeviceName(devices[i]);
            if (deviceName != NULL)
            {
                printf("\t%u - %s\n", i, deviceName);
                STAR_destroyString(deviceName);
            }
            else
            {
                printf("\t%u - Unknown SpaceWire Device\n", i);
            }
        }
        else
        {
            printf("\t%u - Unable to access device\n", i);
        }
    }

    /* If there's only 1 device on the list */
    if (deviceCount == 1U)
    {
        /* Return that device */
        deviceId = devices[0U];
        STAR_destroyDeviceList(devices);
        return deviceId;
    }

    /* Ask the user which device to use */
    printf("Please select which %s device to use: ", descriptionStr);
    fflush(stdout);
    if (fgets(s, 256, stdin) == NULL)
    {
        puts("No device number selected.");
        STAR_destroyDeviceList(devices);
        return STAR_DEVICE_UNKNOWN;
    }
    status = sscanf(s, "%u", &chosen);
    if ((status == 0) || (chosen > (deviceCount - 1U)))
    {
        puts("Incorrect device number selected.");
        STAR_destroyDeviceList(devices);
        return STAR_DEVICE_UNKNOWN;
    }

    /* Return the chosen device */
    deviceId = devices[chosen];
    STAR_destroyDeviceList(devices);
    return deviceId;
}

/**
 * \ingroup StarSystemTester
 * Prompts the user to choose which channel on the specified device to use for
 * the test if more than one channel is available on the device.
 *
 * @param descriptionStr the description of what the device is to be used for
 *                       (e.g. transmit or receive)
 * @param deviceId the identifier of the device from which the channel will be
 *                 selected
 *
 * @return the number of the channel to be used for the test, or 0 if there was
 *         an error or no channels are present on the device
 */
unsigned char chooseChannel(const char * const descriptionStr,
    const STAR_DEVICE_ID deviceId)
{
    STAR_CHANNEL_MASK channelMask;
    unsigned int channelNumber;
    unsigned char channelCount;
    int status;
    unsigned char i;
    char s[256];

    /* Get the channels present on the device */
    channelMask = STAR_getDeviceChannels(deviceId);
    if ((channelMask == 0U) || (channelMask == 1U))
    {
        printf("ERROR: The %s device doesn't appear to have any valid channels.\n",
            descriptionStr);
        return 0U;
    }

    /* Determine the number of channels on the device, */
    /* ignoring the configuration channel */
    channelCount = 0U;
    channelNumber = 0U;
    for (i = 1U; i < 32U; i++)
    {
        if (((channelMask >> i) & 1U) != 0U)
        {
            channelCount++;
            channelNumber = i;
        }
    }

    /* If there's only one channel on the device, use this channel */
    if (channelCount == 1U)
    {
        printf("Using channel %u as the %s channel, as this is the only channel on the device.\n",
            channelNumber, descriptionStr);
        return (unsigned char)channelNumber;
    }

    /* Display the available channels */
    printf("Enter %s channel (", descriptionStr);
    for (i = 1U; i < 32U; i++)
    {
        /* If the channel exists */
        if (((channelMask >> i) & 1U) != 0U)
        {
            printf("%u", i);
            channelCount--;
            if (channelCount == 1U)
            {
                printf(" or ");
            }
            else if (channelCount > 1U)
            {
                printf(", ");
            }
            else
            {
                break;
            }
        }
    }
    printf("): ");
    fflush(stdout);

    /* Read in the channel to use */
    if (fgets(s, 256, stdin) == NULL)
    {
        puts("\nERROR: No channel specified");
        return 0U;
    }
    status = sscanf(s, "%u", &channelNumber);
    if ((status == 0) || (((1U << channelNumber) & channelMask) == 0U))
    {
        puts("\nERROR: The channel specified is not present");
        return 0U;
    }

    return (unsigned char)channelNumber;
}

/**
 * \ingroup StarSystemTester
 * Prompts the user to choose which device and channel to use for the test, if
 * multiple devices and/or channels are available.
 *
 * @param descriptionStr the description of what the device is to be used for
 *                       (e.g. transmit or receive)
 * @param pChannelId a pointer to a variable which will be updated to contain
 *                   the identifier of the channel to be used for the test
 * @param direction the direction in which traffic is to be sent over the
 *                  channel
 *
 * @return whether a device and a channel was successfully selected
 */
int chooseDeviceAndChannel(const char * const descriptionStr,
    STAR_CHANNEL_ID * const pChannelId,
    const STAR_CHANNEL_DIRECTION direction)
{
    STAR_DEVICE_ID deviceId;
    unsigned char channelNumber;

    deviceId = chooseDevice(descriptionStr, STAR_DEVICE_TXRX_SUPPORTED);
    if (deviceId == STAR_DEVICE_UNKNOWN)
    {
        return 0;
    }

    channelNumber = chooseChannel(descriptionStr, deviceId);
    if (channelNumber == 0U)
    {
        return 0;
    }

    /* Open the channel */
    *pChannelId = STAR_openChannelToLocalDevice(deviceId, direction,
        channelNumber, TRUE);
    if ((*pChannelId) == 0U)
    {
        printf("\nERROR: Unable to open %s channel\n", descriptionStr);
        return 0;
    }
    
    printf("deviceId--%d, channelNumber--%d,pChannelId--%d ",deviceId,channelNumber, *pChannelId);

    puts("");

    return 1;
}

/**
 * \ingroup StarSystemTester
 * Prompts the user to specify the path address to be added to the front of the
 * packets to be transmitted for a test. 
 *
 * @return the SpaceWire address entered, or NULL if there was an error
 */
STAR_SPACEWIRE_ADDRESS *getTransmitPathAddress()
{
    char s[256];
    STAR_SPACEWIRE_ADDRESS *pAddress;
    unsigned char newPath[256];
    U16 pathLen = 0U;
    char *pos = (char *)s;
    unsigned long value;

    /* Ask the user to enter the path to add to the front of packets */
    puts("Enter an optional path to add to the front of the packets to be sent:");
    puts("(Values should be in hex, separated by a space, i.e.: 01 0f 02)");

    /* Read in the path */
  /*  if (fgets(s, 256, stdin) == NULL)
    {
        puts("No address entered");
        return NULL;
    }

    // Read until end of buffer or line feed 
    while ((*pos !='\0') && (pathLen < 256U) && (*pos != '\n'))
    {
        errno = 0;
        value = strtoul(pos, &pos, 16);
        if (errno != 0)
        {
            puts("Invalid value entered in address");
            return NULL;
        }

        newPath[pathLen] = (unsigned char)value;
        pathLen++;
    }
    */
    newPath[0]=0x01;
    pathLen=1;
    /* Create a SpaceWire address from the path */
    pAddress = STAR_createAddress(newPath, pathLen);

    /* Return the completed address */
    return pAddress;
}

/**
 * \ingroup StarSystemTester
 * Prompts the user to specify the packet size to be used for a test.
 *
 * @param pPacketSize a pointer to a variable which will be updated to contain
 *                    the packet size specified
 *
 * @return whether the user selected a valid packet size
 */
int getPacketSize(unsigned long * const pPacketSize)
{
    char s[256];
    int status;

    printf("Enter buffer size for each packet in bytes: ");
    fflush(stdout);
    if (fgets(s, 256, stdin) == NULL)
    {
        puts("\nERROR: No buffer size specified");
        return 0;
    }
    status = sscanf(s, "%lu", pPacketSize);
    if ((status == 0) || (*pPacketSize == 0U))
    {
        puts("\nERROR: Invalid buffer size specified");
        return 0;
    }

    return 1;
}

/**
 * \ingroup StarSystemTester
 * Prompts the user to choose whether to check the received data.
 *
 * @param pCompare a pointer to a variable which will be updated to indicate
 *                 whether the data should be compared
 *
 * @return whether the user selected a valid option
 */
int chooseCheckData(int *pCompare)
{
    char s[256];

    printf("Would you like to check received data for errors? (y/n): ");
    fflush(stdout);
    if (fgets(s, 256, stdin) == NULL)
    {
        puts("ERROR: No value selected");
        return 0;
    }
    if ((s[0U] == 'y') || (s[0U] == 'Y') || (s[0U] == 't') || (s[0U] == 'T') ||
        (s[0U] == '1'))
    {
        *pCompare = 1;
    }
    else if ((s[0U] == 'n') || (s[0U] == 'N') || (s[0U] == 'f') ||
        (s[0U] == 'F') || (s[0U] == '0'))
    {
        *pCompare = 0;
    }
    else
    {
        puts("ERROR: Invalid value selected");
        return 0;
    }

    return 1;
}

/**
 * \ingroup StarSystemTester
 * Prompts the user to specify the number of times to run a test.
 *
 * @param pLoopCount a pointer to a variable which will be updated to contain
 *                    the specified value for the number of times the test
 *                    should be run
 *
 * @return whether the user selected a valid number of loops
 */
int getLoopCount(unsigned long * const pLoopCount)
{
    char s[256];
    int status;

    printf("Enter the number of times to run the test: ");
    fflush(stdout);
    if (fgets(s, 256, stdin) == NULL)
    {
        puts("\nERROR: No loop count specified");
        return 0;
    }
    status = sscanf(s, "%lu", pLoopCount);
    if ((status == 0) || (*pLoopCount == 0U))
    {
        puts("\nERROR: Invalid loop count specified");
        return 0;
    }

    return 1;
}

/**
 * \ingroup StarSystemTester
 * Prompts the user to specify the number of packets to transmit and receive in
 * a test.
 *
 * @param pPacketCount a pointer to a variable which will be updated to contain
 *                    the specified value for the number of packets to transmit
 *                    and receive in the test
 *
 * @return whether the user selected a valid number of packets
 */
int getPacketCount(unsigned long * const pPacketCount)
{
    char s[256];
    int status;

    printf("Enter the number of packets to transmit/receive in the test: ");
    fflush(stdout);
    if (fgets(s, 256, stdin) == NULL)
    {
        puts("\nERROR: No packet count specified");
        return 0;
    }
    status = sscanf(s, "%lu", pPacketCount);
    if ((status == 0) || (*pPacketCount == 0U))
    {
        puts("\nERROR: Invalid packet count specified");
        return 0;
    }

    return 1;
}

/**
 * \ingroup StarSystemTester
 * Compare the contents of received traffic against a previously created buffer.
 *
 * @param pTransferOp the completed receive operation containing the packets to
 *                    look at
 * @param packetCount the number of packets which should be in the received
 *                    operation
 * @param packetSize the size of each packet which should be received
 * @param pBuffer the buffer that the received packets should be compared
 *                against
 *
 * @return the number of errors in the received packets
 */
unsigned long comparePackets(STAR_TRANSFER_OPERATION * const pTransferOp,
    const unsigned long packetCount, const unsigned long packetSize,
    char * const pBuffer)
{
    unsigned long errorCount = 0U, i;
    unsigned int rxPacketCount;
    STAR_STREAM_ITEM *pRxStreamItem;
    char *pRxBuffer;
    U32 rxPacketLength;

    /* Get the number of traffic items received */
    rxPacketCount = STAR_getTransferItemCount(pTransferOp);
    if (rxPacketCount != packetCount)
    {
        printf("\nERROR expected to receive %lu packets but received %u.\n",
            packetCount, rxPacketCount);
        errorCount++;
    }
    else
    {
        /* For each traffic item received */
        for (i = 0U; i < rxPacketCount; i++)
        {
            /* Get the packet */
            pRxStreamItem = STAR_getTransferItem(pTransferOp, i);
            if ((pRxStreamItem == NULL) || (pRxStreamItem->itemType !=
                    STAR_STREAM_ITEM_TYPE_SPACEWIRE_PACKET) ||
                (pRxStreamItem->item == NULL))
            {
                printf("\nERROR received an unexpected traffic type, or empty traffic item in item %lu\n",
                    i);
                errorCount++;
            }
            else
            {
                pRxBuffer = (char *)STAR_getPacketData(
                    (STAR_SPACEWIRE_PACKET *)pRxStreamItem->item,
                    &rxPacketLength);
		printf("Data---%d\n",pRxBuffer[i]);
		printf("Data---%d\n",pRxBuffer[i+1]);
		printf("Data---%d\n",pRxBuffer[i+2]);
		printf("Data---%d\n",pRxBuffer[i+3]);
		printf("Data---%d\n",pRxBuffer[i+4]);
		printf("Data---%d\n",pRxBuffer[i+5]);
		printf("Data---%d\n",pRxBuffer[i+6]);
		printf("Data---%d\n",pRxBuffer[i+7]);
                if ((pRxBuffer == NULL) || (rxPacketLength != packetSize))
                {
                    printf("\nERROR received a packet of length %u, expected length %lu in item %lu\n",
                        rxPacketLength, packetSize, i);
                    errorCount++;
                }
                else
                {   
                    /* Compare the buffers and increment the error count if */
                    /* the buffers do not match */
                    errorCount += BufferCompareChar(pBuffer + (packetSize * i),
                        pRxBuffer, packetSize);
                }
                if (pRxBuffer != NULL)
                {
                    STAR_destroyPacketData((unsigned char *)pRxBuffer);
                }
            }
        }
    }

    /* Return the error count */
    return errorCount;
}

/**
 * \ingroup StarSystemTester
 * Get the combined length of received traffic, and check for errors.
 *
 * @param pTransferOp the completed receive operation containing the packets to
 *                    look at
 * @param packetCount the number of packets which should be in the received
 *                    operation
 *
 * @return the length of the received packets
 */
unsigned long getPacketLengths(STAR_TRANSFER_OPERATION * const pTransferOp,
    const unsigned long packetCount)
{
    unsigned long rxPacketLength = 0U, i;
    unsigned int rxPacketCount;
    STAR_STREAM_ITEM *pRxStreamItem;

    /* Get the number of traffic items received */
    rxPacketCount = STAR_getTransferItemCount(pTransferOp);
    if (rxPacketCount != packetCount)
    {
        printf("\nERROR expected to receive %lu packets but received %u.\n",
            packetCount, rxPacketCount);
    }

    /* For each traffic item received */
    for (i = 0U; i < rxPacketCount; i++)
    {
        /* Get the packet */
        pRxStreamItem = STAR_getTransferItem(pTransferOp, i);
        if ((pRxStreamItem == NULL) || (pRxStreamItem->itemType !=
                STAR_STREAM_ITEM_TYPE_SPACEWIRE_PACKET) ||
            (pRxStreamItem->item == NULL))
        {
            printf("\nERROR received an unexpected traffic type, or empty traffic item in item %lu\n",
                i);
        }
        else
        {
            rxPacketLength += STAR_getPacketLength(
                (STAR_SPACEWIRE_PACKET *)pRxStreamItem->item);
        }
    }

    /* Return the total packet length */
    return rxPacketLength;
}

/**
 * \ingroup StarSystemTester
 * Display the results of a test.
 *
 * @param start the time at which the test started
 * @param finish the time at which the test finished
 * @param compared whether the received data was compared
 * @param byteSize the size of the packet sent in each loop
 * @param loopCount the number of packets sent in each loop
 * @param loopCount the number of loops that were performed
 * @param errorCount the number of errors encountered
 * @param descriptionStr a description of the test
 */
void DisplayResults(const clock_t start, const clock_t finish,
    const int compared, const unsigned long byteSize,
    const unsigned long packetCount, const unsigned long loopCount,
    const unsigned long errorCount, const char * const descriptionStr)
{
    double bitsSent, duration;

    puts("Test complete.");

    /* Calculate the time taken and the number of bits sent */
    duration = (double)(finish - start) / TIME_DIVIDER;
    bitsSent = (double)byteSize * (double)8 * (double)packetCount *
        (double)loopCount;

    /* Display the data rates */
    printf("\n**** %s, Results **** \n", descriptionStr);
    printf("\tTime Taken = %-9.5g Seconds\n", duration);
    printf("\tAverage Speed = %-9.2E bits/s (%-3.2f Mbits/s)\n\n",
        bitsSent / duration, (bitsSent / duration) / 1000000.0);

    /* Display the number of errors encountered */
    if (compared != 0)
    {
        printf("Total Errors = 0x%-7lx \n", errorCount);
    }

    /* Display whether the test was successful */
    if (errorCount == 0U)
    {
        puts("Test successful.\n");
    }
    else
    {
        puts("Test failed.\n");
    }
}

/**
 * \ingroup StarSystemTester
 * Tests the SpaceWire device using a loop back test.  Packets of random data
 * are transmitted on one link and received on a second link.
 *
 * This test is timed so as to test link speed. Optionally, the data is then
 * compared to check for errors.
 */
void LoopBack_SinglePacket(void)
{
    STAR_CHANNEL_ID rxChannelId = 0U, txChannelId = 0U;
    char *pTxBuffer = NULL;
    STAR_TRANSFER_OPERATION *pTxTransferOp = NULL, *pRxTransferOp = NULL;
    STAR_STREAM_ITEM *pTxStreamItem = NULL;
    unsigned long loopCount = 0U, errorCount = 0U, byteSize = 0U, testNum;
    clock_t start, finish;
    int compare=0, i=0, j=0,k=0;
    STAR_TRANSFER_STATUS rxStatus, txStatus;
    STAR_SPACEWIRE_ADDRESS *pAddressPath;
    const unsigned long packetCount=1U;
    unsigned int rxPacketCount;
    STAR_STREAM_ITEM *pRxStreamItem;
    char *pRxBuffer;
    U32 rxPacketLength;
    U16 linkSpeed;
 
    SetConfiguration();

    /* Select the transmit device and channel to be used */
    if (chooseDeviceAndChannel("transmit", &txChannelId,
        STAR_CHANNEL_DIRECTION_OUT) == 0)
    {
        return;
    }

    /* Select the path to be added to the front of packets transmitted */
    pAddressPath = getTransmitPathAddress();
    printf("pAddressPath--%d\n",pAddressPath);

    /* Select the receive device and channel to be used */
    if (chooseDeviceAndChannel("receive", &rxChannelId,
        STAR_CHANNEL_DIRECTION_IN) == 0)
    {
        goto LoopBack_SinglePacket_Finish;
    }
    
   
    
    /* Get packet size */
   /* if (getPacketSize(&byteSize) == 0)
    {
        goto LoopBack_SinglePacket_Finish;
    }*/
   
    byteSize=31;
    
    /* Get loop count */
   /* if (getLoopCount(&loopCount) == 0)
    {
        goto LoopBack_SinglePacket_Finish;
    }*/
    loopCount=1;
    /* Compare? */
   /* if (chooseCheckData(&compare) == 0)
    {
        goto LoopBack_SinglePacket_Finish;
    }*/

    /* Allocate memory for the transmit buffer */
    pTxBuffer = (char *)calloc(1U, byteSize);
    if (pTxBuffer == NULL)
    {
        puts("\nERROR: Unable to allocate memory for transmit buffer");
        goto LoopBack_SinglePacket_Finish;
    }

    /* Fill transmit buffer with random data */
    //FillBufferRandomChar(pTxBuffer, byteSize, DATA_TYPE_RANDOM);
    
 
 
  /*  pTxBuffer[0]=0x01;
   pTxBuffer[1]=0x76;
   pTxBuffer[2]=0xff;
   pTxBuffer[3]=0x77;
   pTxBuffer[4]=0x04;
   pTxBuffer[5]=0x7e;
   pTxBuffer[6]=0x00;
   pTxBuffer[7]=0x4a;
   byteSize=8;*/
 
  pTxBuffer[0]=0x01;
   pTxBuffer[1]=0x76;
   pTxBuffer[2]=0xff;
   pTxBuffer[3]=0x77;
   pTxBuffer[4]=0x04;
   pTxBuffer[5]=0x7e;
   pTxBuffer[6]=0xA9;
   pTxBuffer[7]=0x52;
   pTxBuffer[8]=0x00;
   pTxBuffer[9]=0x00;
   pTxBuffer[10]=0x53;
   pTxBuffer[11]=0x00;
   pTxBuffer[12]=0x00;
   pTxBuffer[13]=0x54;
   pTxBuffer[14]=0x03;
   pTxBuffer[15]=0xFF;
   pTxBuffer[16]=0x55;
   pTxBuffer[17]=0x00;
   pTxBuffer[18]=0x0F;
   pTxBuffer[19]=0x56;
   pTxBuffer[20]=0x00;
   pTxBuffer[21]=0x04;
   pTxBuffer[22]=0x60;
   pTxBuffer[23]=0x00;
   pTxBuffer[24]=0x7A;
   pTxBuffer[25]=0x00;
   pTxBuffer[26]=0x51;
   pTxBuffer[27]=0x00;
   pTxBuffer[28]=0xC8;
   pTxBuffer[29]=0x75;
   pTxBuffer[30]=0x01;  
    byteSize=31;  
     printf("Sent Commands--\n");
    for(k=0;k<byteSize;k++)
    printf("%02X \n",pTxBuffer[k]);


    
    /* Create receive operation to receive 1 packet */
    pRxTransferOp = STAR_createRxOperation(packetCount, STAR_RECEIVE_PACKETS);
    if (pRxTransferOp == NULL)
    {
        puts("\nERROR: Unable to create receive operation");
        goto LoopBack_SinglePacket_Finish;
    }

        /* Create the packet to be transmitted */
    pTxStreamItem = STAR_createPacket(pAddressPath, (U8 *)pTxBuffer, byteSize,
        STAR_EOP_TYPE_EOP);
    if (pTxStreamItem == NULL)
    {
        puts("\nERROR: Unable to create the packet to be transmitted");
        goto LoopBack_SinglePacket_Finish;
    }

    
    /* Create the transmit transfer operation for the packet */
    pTxTransferOp = STAR_createTxOperation(&pTxStreamItem, packetCount);
    if (pTxTransferOp == NULL)
    {
        puts("\nERROR: Unable to create the transfer operation to be transmitted");
        goto LoopBack_SinglePacket_Finish;
    }

    puts("Running Single-Packet Loopback Test...");

    /* Start time */
    start = GET_TIME();

    for (testNum = 0U; testNum < loopCount; testNum++)
    {
      
              /* Submit the receive operation */
        if (STAR_submitTransferOperation(rxChannelId, pRxTransferOp) == 0)
        {
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1U);
            goto LoopBack_SinglePacket_Finish;
        }
        else
	  printf("Receive operation transferred...\n");
      
              /* Submit the transmit operation */
        if (STAR_submitTransferOperation(txChannelId, pTxTransferOp) == 0)
        {
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1U);
            goto LoopBack_SinglePacket_Finish;
        }
        else
	  printf("Send operation transferred...\n");



        /* Wait on the transmit operation completing */
        txStatus = STAR_waitOnTransferOperationCompletion(pTxTransferOp,
            1000);
        if(txStatus != STAR_TRANSFER_STATUS_COMPLETE)
        {
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1U);
            goto LoopBack_SinglePacket_Finish;
        }
        else
	{printf("Send operation over...\n"); 
	  PerformTickIn();
	  
	}
 
        /* Wait on the receive operation completing */
        rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp,
            2000);
        if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
        {
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1U);
            goto LoopBack_SinglePacket_Finish;
        }
          else
	   printf("Receive operation over...\n");

      /*Print Received Data */
          
         rxPacketCount = STAR_getTransferItemCount(pRxTransferOp);
    if (rxPacketCount != packetCount)
    {
        printf("\nERROR expected to receive %lu packets but received %u.\n",
            packetCount, rxPacketCount);
        errorCount++;
    }
    else
    {
        /* For each traffic item received */
        for (i = 0U; i < rxPacketCount; i++)
        {
          pRxStreamItem = STAR_getTransferItem(pRxTransferOp, i);
          pRxBuffer = (char *)STAR_getPacketData(
                    (STAR_SPACEWIRE_PACKET *)pRxStreamItem->item,
                    &rxPacketLength);
	   printf("Received Data length is %d and HK data is--\n",rxPacketLength-1);
	  for(j=0;j<rxPacketLength;j++)
	  {//printf("%02X\t ",pRxBuffer[j]); if(j%10 ==0)printf("\n");
	    
	  }
	 
	}
    }
	   /*Print Received Data Over */
	  
        /* If the received packet is to be compared */
        if (compare != 0)
        {
            /* Compare the received packet to the buffer transmitted */
            errorCount += comparePackets(pRxTransferOp, 1U, byteSize,
                pTxBuffer);
        }
    }

    /* End time */
    finish = GET_TIME();

    /* Display the results */
    DisplayResults(start, finish, compare, byteSize, 1U, loopCount, errorCount,
        "Single-Packet Loopback Test");
    

LoopBack_SinglePacket_Finish:

    /* Dispose of the transfer operations */
    if (pTxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pTxTransferOp);
    }
    if (pRxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pRxTransferOp);
    }

    /* Destroy the packet transmitted */
    if (pTxStreamItem != NULL)
    {
        STAR_destroyStreamItem(pTxStreamItem);
    }

    /* Free the transmit buffer */
    if (pTxBuffer != NULL)
    {
        free(pTxBuffer);
    }

    /* Free the address path */
    if (pAddressPath != NULL)
    {
        STAR_destroyAddress(pAddressPath);
    }

    /* Close the channels */
    if (rxChannelId != 0U)
    {
        STAR_closeChannel(rxChannelId);
    }
    if (txChannelId != 0U)
    {
        STAR_closeChannel(txChannelId);
    }
    sleep(3);
}

/**
 * \ingroup StarSystemTester
 * Performs a loopback test with multiple packets.  Multiple packets of random
 * data are transmitted out of one link (using a  single transfer operation) and
 * received at a second link (again using a single transfer operation).
 *
 * This test is timed so as to test link speed. Optionally, the data is then
 * compared to check for errors.
 */
void LoopBack_MultiPacket(void)
{
    STAR_CHANNEL_ID rxChannelId = 0U, txChannelId = 0U;
    unsigned long loopCount, errorCount = 0U, byteSize, testNum;
    unsigned long i, packetCount = 0U;
    int compare;
    char *pTxBuffer = NULL;
    STAR_STREAM_ITEM **ppTxStreamItems = NULL;
    STAR_TRANSFER_OPERATION *pTxTransferOp = NULL, *pRxTransferOp = NULL;
    STAR_TRANSFER_STATUS rxStatus, txStatus;
    clock_t start, finish;
    STAR_SPACEWIRE_ADDRESS *pAddressPath;

    /* Select the transmit device and channel to be used */
    if (chooseDeviceAndChannel("transmit", &txChannelId,
        STAR_CHANNEL_DIRECTION_OUT) == 0)
    {
        return;
    }

    /* Select the path to be added to the front of packets transmitted */
    pAddressPath = getTransmitPathAddress();

    /* Select the receive device and channel to be used */
    if (chooseDeviceAndChannel("receive", &rxChannelId,
        STAR_CHANNEL_DIRECTION_IN) == 0)
    {
        goto LoopBack_MultiPacket_Finish;
    }

    /* Get packet size */
    if (getPacketSize(&byteSize) == 0)
    {
        goto LoopBack_MultiPacket_Finish;
    }

    /* Get packet count */
    if (getPacketCount(&packetCount) == 0)
    {
        goto LoopBack_MultiPacket_Finish;
    }

    /* Get loop count */
    if (getLoopCount(&loopCount) == 0)
    {
        goto LoopBack_MultiPacket_Finish;
    }

    /* Compare? */
    if (chooseCheckData(&compare) == 0)
    {
        goto LoopBack_MultiPacket_Finish;
    }

    /* Allocate memory for the transmit buffer */
    pTxBuffer = (char *)calloc(1U, byteSize * packetCount);
    if (pTxBuffer == NULL)
    {
        puts("\nERROR: Unable to allocate memory for transmit buffer");
        goto LoopBack_MultiPacket_Finish;
    }

    /* Fill transmit buffer with random data */
    FillBufferRandomChar(pTxBuffer, byteSize * packetCount, DATA_TYPE_RANDOM);

    /* Create receive operation to receive the packets */
    pRxTransferOp = STAR_createRxOperation(packetCount, STAR_RECEIVE_PACKETS);
    if (pRxTransferOp == NULL)
    {
        puts("\nERROR: Unable to create receive operation");
        goto LoopBack_MultiPacket_Finish;
    }

    /* Allocate memory for the transmit stream item pointers */
    ppTxStreamItems = (STAR_STREAM_ITEM **)calloc(packetCount,
        sizeof(STAR_STREAM_ITEM *));
    if (ppTxStreamItems == NULL)
    {
        puts("\nERROR: Unable to allocate memory for transmit stream items");
        goto LoopBack_MultiPacket_Finish;
    }

    /* For each packet */
    for (i = 0U; i < packetCount; i++)
    {
        /* Create the stream item */
        ppTxStreamItems[i] = STAR_createPacket(pAddressPath,
            (U8 *)pTxBuffer + (byteSize * i), byteSize, STAR_EOP_TYPE_EOP);
        if (ppTxStreamItems[i] == NULL)
        {
            printf("\nERROR: Unable to create packet %lu to be transmitted\n", i);
            goto LoopBack_MultiPacket_Finish;
        }
    }

    /* Create the transmit transfer operation for the packets */
    pTxTransferOp = STAR_createTxOperation(ppTxStreamItems, packetCount);
    if (pTxTransferOp == NULL)
    {
        puts("\nERROR: Unable to create the transfer operation to be transmitted");
        goto LoopBack_MultiPacket_Finish;
    }

    puts("Running Multi-Packet Loopback Test...");

    /* Start time */
    start = GET_TIME();

    for (testNum = 0U; testNum < loopCount; testNum++)
    {
        /* Submit the receive operation */
        if (STAR_submitTransferOperation(rxChannelId,
            pRxTransferOp) == 0)
        {
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1U);
            goto LoopBack_MultiPacket_Finish;
        }

        /* Submit the transmit operation */
        if (STAR_submitTransferOperation(txChannelId, pTxTransferOp) == 0)
        {
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1U);
            goto LoopBack_MultiPacket_Finish;
        }

        /* Wait on the transmit operation completing */
        txStatus = STAR_waitOnTransferOperationCompletion(pTxTransferOp,
            STAR_INFINITE);
        if(txStatus != STAR_TRANSFER_STATUS_COMPLETE)
        {
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1U);
            goto LoopBack_MultiPacket_Finish;
        }

        /* Wait on the receive operation completing */
        rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp,
            STAR_INFINITE);
        if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
        {
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1U);
            goto LoopBack_MultiPacket_Finish;
        }

        /* If the received packets are to be compared */
        if (compare != 0)
        {
            /* Compare the received packets to the buffer transmitted */
            errorCount += comparePackets(pRxTransferOp, packetCount, byteSize,
                pTxBuffer);
        }
    }

    /* End time */
    finish = GET_TIME();

    /* Display the results */
    DisplayResults(start, finish, compare, byteSize, packetCount, loopCount,
        errorCount, "Multiple-Packet Loopback Test");

LoopBack_MultiPacket_Finish:

    /* Dispose of the transfer operations */
    if (pTxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pTxTransferOp);
    }
    if (pRxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pRxTransferOp);
    }

    /* Destroy the packets transmitted */
    if (ppTxStreamItems != NULL)
    {
        for (i = 0U; i < packetCount; i++)
        {
            if (ppTxStreamItems[i] != NULL)
            {
                STAR_destroyStreamItem(ppTxStreamItems[i]);
            }
        }
        free(ppTxStreamItems);
    }

    /* Free the transmit buffer */
    if (pTxBuffer != NULL)
    {
        free(pTxBuffer);
    }

    /* Free the address path */
    if (pAddressPath != NULL)
    {
        STAR_destroyAddress(pAddressPath);
    }

    /* Close the opened channels */
    if (rxChannelId != 0U)
    {
        STAR_closeChannel(rxChannelId);
    }
    if (txChannelId != 0U)
    {
        STAR_closeChannel(txChannelId);
    }
}

/**
 * \ingroup StarSystemTester
 * Performs a Double loopback test with single packets.  Single packets of
 * random data are transmitted out of two links simultaneously (using a single
 * transfer operation per link) and received at either end simultaneously (again
 * using a single transfer operation per link).
 *
 * This test is timed so as to test link speed.  Optionally, the data is then
 * compared to check for errors.
 *
 * This method makes use of multiple transfer operation submission.
 */
void LoopBackDouble_SinglePacket(void)
{
    STAR_CHANNEL_ID channelId[2] = { 0U, 0U };
    unsigned long loopCount, errorCount = 0U, byteSize, testNum, i;
    int compare;
    char *pTxBuffer[2] = { NULL, NULL };
    STAR_STREAM_ITEM *pTxStreamItem[2] = { NULL, NULL };
    STAR_TRANSFER_OPERATION *pTxTransferOp[2] = { NULL, NULL };
    STAR_TRANSFER_OPERATION *pRxTransferOp[2] = { NULL, NULL };
    STAR_TRANSFER_OPERATION *pTransferOpList[2][2] = { {NULL} };
    STAR_TRANSFER_STATUS rxStatus, txStatus;
    clock_t start, finish;
    STAR_SPACEWIRE_ADDRESS *pAddressPath[2] = { NULL, NULL };

    /* Select the first device and channel to be used */
    if (chooseDeviceAndChannel("first", &channelId[0U],
        STAR_CHANNEL_DIRECTION_INOUT) == 0)
    {
        return;
    }

    /* Select the first path to be added to the front of packets transmitted */
    pAddressPath[0U] = getTransmitPathAddress();

    /* Select the second device and channel to be used */
    if (chooseDeviceAndChannel("second", &channelId[1U],
        STAR_CHANNEL_DIRECTION_INOUT) == 0)
    {
        goto LoopBackDouble_SinglePacket_Finish;
    }

    /* Select the second path to be added to the front of packets transmitted */
    pAddressPath[1U] = getTransmitPathAddress();

    /* Get packet size */
    if (getPacketSize(&byteSize) == 0)
    {
        goto LoopBackDouble_SinglePacket_Finish;
    }

    /* Get loop count */
    if (getLoopCount(&loopCount) == 0)
    {
        goto LoopBackDouble_SinglePacket_Finish;
    }

    /* Compare? */
    if (chooseCheckData(&compare) == 0)
    {
        goto LoopBackDouble_SinglePacket_Finish;
    }

    /* For each channel */
    for (i = 0U; i < 2U; i++)
    {
        /* Allocate memory for the transmit buffer */
        pTxBuffer[i] = (char *)calloc(1U, byteSize);
        if (pTxBuffer[i] == NULL)
        {
            puts("\nERROR: Unable to allocate memory for transmit buffer");
            goto LoopBackDouble_SinglePacket_Finish;
        }

        /* Fill transmit buffer with random data */
        FillBufferRandomChar(pTxBuffer[i], byteSize, DATA_TYPE_RANDOM);

        /* Create receive operation to receive 1 packet */
        pRxTransferOp[i] = STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);
        if (pRxTransferOp[i] == NULL)
        {
            puts("\nERROR: Unable to create receive operation");
            goto LoopBackDouble_SinglePacket_Finish;
        }

        /* Create the packet to be transmitted */
        pTxStreamItem[i] = STAR_createPacket(pAddressPath[i],
            (U8 *)pTxBuffer[i], byteSize, STAR_EOP_TYPE_EOP);
        if (pTxStreamItem[i] == NULL)
        {
            puts("\nERROR: Unable to create the packet to be transmitted");
            goto LoopBackDouble_SinglePacket_Finish;
        }

        /* Create the transmit transfer operation for the packet */
        pTxTransferOp[i] = STAR_createTxOperation(&pTxStreamItem[i], 1U);
        if (pTxTransferOp[i] == NULL)
        {
            puts("\nERROR: Unable to create the transfer operation to be transmitted");
            goto LoopBackDouble_SinglePacket_Finish;
        }

        /* Add transfer operation to list for channel */
        pTransferOpList[i][0U] = pRxTransferOp[i];
        pTransferOpList[i][1U] = pTxTransferOp[i];
    }

    puts("Starting Single-Packet Double Loopback Test...");

    /* Start time */
    start = GET_TIME();

    for (testNum = 0U; testNum < loopCount; testNum++)
    {
        /* Submit the receive operations */

        /* First Channel Rx */
        if (STAR_submitTransferOperation(channelId[0U],
            pTransferOpList[0U][0U]) == 0)
        {
            printf("\nERROR occurred submitting operation list.  Test %lu failed.\n",
                    testNum + 1U);
            goto LoopBackDouble_SinglePacket_Finish;
        }
        /* Second Channel Rx */
        if (STAR_submitTransferOperation(channelId[1U],
            pTransferOpList[1U][0U]) == 0)
        {
            printf("\nERROR occurred submitting operation list.  Test %lu failed.\n",
                    testNum + 1U);
            goto LoopBackDouble_SinglePacket_Finish;
        }

        /* Submit the transmit operations */

        /* First Channel Tx */
        if (STAR_submitTransferOperation(channelId[0U],
            pTransferOpList[0U][1U]) == 0)
        {
            printf("\nERROR occurred submitting operation list.  Test %lu failed.\n",
                    testNum + 1U);
            goto LoopBackDouble_SinglePacket_Finish;
        }
        /* Second Channel Tx */
        if (STAR_submitTransferOperation(channelId[1U],
            pTransferOpList[1U][1U]) == 0)
        {
            printf("\nERROR occurred submitting operation list.  Test %lu failed.\n",
                    testNum + 1U);
            goto LoopBackDouble_SinglePacket_Finish;
        }

        /* For each channel */
        for (i = 0U; i < 2U; i++)
        {
            /* Wait on the transmit operation completing */
            txStatus = STAR_waitOnTransferOperationCompletion(pTxTransferOp[i],
                STAR_INFINITE);
            if (txStatus != STAR_TRANSFER_STATUS_COMPLETE)
            {
                printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                    testNum + 1U);
                goto LoopBackDouble_SinglePacket_Finish;
            }
        }

        /* For each channel */
        for (i = 0U; i < 2U; i++)
        {
            /* Wait on the receive operation completing */
            rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp[i],
                STAR_INFINITE);
            if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
            {
                printf("\nERROR occurred during receive.  Test %lu failed.\n",
                    testNum + 1U);
                goto LoopBackDouble_SinglePacket_Finish;
            }
        }

        if (compare != 0)
        {
            /* Compare the received packets to the buffer transmitted */
            errorCount += comparePackets(pRxTransferOp[1U], 1U, byteSize,
                pTxBuffer[0U]);
            errorCount += comparePackets(pRxTransferOp[0U], 1U, byteSize,
                pTxBuffer[1U]);
        }
    }

    /* End time */
    finish = GET_TIME();

    /* Display the results */
    DisplayResults(start, finish, compare, byteSize, 1U, loopCount, errorCount,
        "Single-Packet Double Loopback Test");

LoopBackDouble_SinglePacket_Finish:

    /* For each channel */
    for (i = 0U; i < 2U; i++)
    {
        /* Dispose of the transfer operations */
        STAR_disposeTransferOperation(pTransferOpList[i][0U]);
        STAR_disposeTransferOperation(pTransferOpList[i][1U]);

        /* Destroy the packet transmitted */
        if (pTxStreamItem[i] != NULL)
        {
            STAR_destroyStreamItem(pTxStreamItem[i]);
        }

        /* Free the transmit buffer */
        if (pTxBuffer[i] != NULL)
        {
            free(pTxBuffer[i]);
        }

        /* Free the address path */
        if (pAddressPath[i] != NULL)
        {
            STAR_destroyAddress(pAddressPath[i]);
        }

        /* Close the channel */
        if (channelId[i] != 0U)
        {
            STAR_closeChannel(channelId[i]);
        }
    }
}

/**
 * \ingroup StarSystemTester
 * Performs a Double loopback test with multiple packets. Multiple packets of
 * random data are transmitted out of two links simultaneously (using a single
 * transfer operation per link) and received at either end simultaneously (again
 * using a single transfer operation per link).
 *
 * This test is timed so as to test link speed.  Optionally, the data is then
 * compared to check for errors.
 */
void LoopBackDouble_MultiPacket(void)
{
    STAR_CHANNEL_ID channelId[2] = { 0U, 0U };
    unsigned long loopCount, errorCount = 0U, byteSize, testNum, i;
    unsigned long packetNum, packetCount = 0U;
    int compare;
    char *pTxBuffer[2] = { NULL, NULL };
    STAR_STREAM_ITEM **ppTxStreamItems[2] = { NULL, NULL };
    STAR_TRANSFER_OPERATION *pTxTransferOp[2] = { NULL, NULL };
    STAR_TRANSFER_OPERATION *pRxTransferOp[2] = { NULL, NULL };
    STAR_TRANSFER_OPERATION *pTransferOpList[2][2] = { {NULL} };
    STAR_TRANSFER_STATUS rxStatus, txStatus;
    clock_t start, finish;
    STAR_SPACEWIRE_ADDRESS *pAddressPath[2] = { NULL, NULL };

    /* Select the first device and channel to be used */
    if (chooseDeviceAndChannel("first", &channelId[0U],
        STAR_CHANNEL_DIRECTION_INOUT) == 0)
    {
        return;
    }

    /* Select the first path to be added to the front of packets transmitted */
    pAddressPath[0U] = getTransmitPathAddress();

    /* Select the second device and channel to be used */
    if (chooseDeviceAndChannel("second", &channelId[1U],
        STAR_CHANNEL_DIRECTION_INOUT) == 0)
    {
        goto LoopBackDouble_MultiPacket_Finish;
    }

    /* Select the second path to be added to the front of packets transmitted */
    pAddressPath[1U] = getTransmitPathAddress();

    /* Get packet size */
    if (getPacketSize(&byteSize) == 0)
    {
        goto LoopBackDouble_MultiPacket_Finish;
    }

    /* Get packet count */
    if (getPacketCount(&packetCount) == 0)
    {
        goto LoopBackDouble_MultiPacket_Finish;
    }

    /* Get loop count */
    if (getLoopCount(&loopCount) == 0)
    {
        goto LoopBackDouble_MultiPacket_Finish;
    }

    /* Compare? */
    if (chooseCheckData(&compare) == 0)
    {
        goto LoopBackDouble_MultiPacket_Finish;
    }

    /* For each channel */
    for (i = 0U; i < 2U; i++)
    {
        /* Allocate memory for the transmit buffer */
        pTxBuffer[i] = (char *)calloc(1U, byteSize * packetCount);
        if (pTxBuffer[i] == NULL)
        {
            puts("\nERROR: Unable to allocate memory for transmit buffer");
            goto LoopBackDouble_MultiPacket_Finish;
        }

        /* Fill transmit buffer with random data */
        FillBufferRandomChar(pTxBuffer[i], byteSize * packetCount,
            DATA_TYPE_RANDOM);

        /* Create receive operation to receive the packets */
        pRxTransferOp[i] = STAR_createRxOperation(packetCount,
            STAR_RECEIVE_PACKETS);
        if (pRxTransferOp[i] == NULL)
        {
            puts("\nERROR: Unable to create receive operation");
            goto LoopBackDouble_MultiPacket_Finish;
        }

        /* Allocate memory for the transmit stream item pointers */
        ppTxStreamItems[i] = (STAR_STREAM_ITEM **)calloc(packetCount,
            sizeof(STAR_STREAM_ITEM *));
        if (ppTxStreamItems[i] == NULL)
        {
            puts("\nERROR: Unable to allocate memory for transmit stream items");
            goto LoopBackDouble_MultiPacket_Finish;
        }

        /* For each packet */
        for (packetNum = 0U; packetNum < packetCount; packetNum++)
        {
            /* Create the packet to be transmitted */
            ppTxStreamItems[i][packetNum] = STAR_createPacket(pAddressPath[i],
                (U8 *)pTxBuffer[i] + (byteSize * packetNum), byteSize,
                STAR_EOP_TYPE_EOP);
            if (ppTxStreamItems[i][packetNum] == NULL)
            {
                printf("\nERROR: Unable to create the packet %lu to be transmitted\n",
                    packetNum);
                goto LoopBackDouble_MultiPacket_Finish;
            }
        }

        /* Create the transmit transfer operation for the packets */
        pTxTransferOp[i] = STAR_createTxOperation(ppTxStreamItems[i],
            packetCount);
        if (pTxTransferOp[i] == NULL)
        {
            puts("\nERROR: Unable to create the transfer operation to be transmitted");
            goto LoopBackDouble_MultiPacket_Finish;
        }

        /* Add transfer operation to list for channel */
        pTransferOpList[i][0U] = pRxTransferOp[i];
        pTransferOpList[i][1U] = pTxTransferOp[i];
    }

    puts("Running Multi-Packet Double Loopback Test...");

    /* Start time */
    start = GET_TIME();

    for (testNum = 0U; testNum < loopCount; testNum++)
    {
        /* Submit the receive operations */

        /* First Channel Rx */
        if (STAR_submitTransferOperation(channelId[0U],
            pTransferOpList[0U][0U]) == 0)
        {
            printf("\nERROR occurred submitting operation list.  Test %lu failed.\n",
                    testNum + 1U);
            goto LoopBackDouble_MultiPacket_Finish;
        }
        /* Second Channel Rx */
        if (STAR_submitTransferOperation(channelId[1U],
            pTransferOpList[1U][0U]) == 0)
        {
            printf("\nERROR occurred submitting operation list.  Test %lu failed.\n",
                    testNum + 1U);
            goto LoopBackDouble_MultiPacket_Finish;
        }

        /* Submit the transmit operations */

        /* First Channel Tx */
        if (STAR_submitTransferOperation(channelId[0U],
            pTransferOpList[0U][1U]) == 0)
        {
            printf("\nERROR occurred submitting operation list.  Test %lu failed.\n",
                    testNum + 1U);
            goto LoopBackDouble_MultiPacket_Finish;
        }
        /* Second Channel Tx */
        if (STAR_submitTransferOperation(channelId[1U],
            pTransferOpList[1U][1U]) == 0)
        {
            printf("\nERROR occurred submitting operation list.  Test %lu failed.\n",
                    testNum + 1U);
            goto LoopBackDouble_MultiPacket_Finish;
        }

        /* For each channel */
        for (i = 0U; i < 2U; i++)
        {
            /* Wait on the transmit operation completing */
            txStatus = STAR_waitOnTransferOperationCompletion(pTxTransferOp[i],
                STAR_INFINITE);
            if (txStatus != STAR_TRANSFER_STATUS_COMPLETE)
            {
                printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                    testNum + 1U);
                goto LoopBackDouble_MultiPacket_Finish;
            }
        }

        /* For each channel */
        for (i = 0U; i < 2U; i++)
        {
            /* Wait on the receive operation completing */
            rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp[i],
                STAR_INFINITE);
            if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
            {
                printf("\nERROR occurred during receive.  Test %lu failed - status %d.\n",
                    testNum + 1U, rxStatus);
                goto LoopBackDouble_MultiPacket_Finish;
            }
        }

        if (compare != 0)
        {
            /* Compare the received packets to the buffer transmitted */
            errorCount += comparePackets(pRxTransferOp[1U], packetCount,
                byteSize, pTxBuffer[0U]);
            errorCount += comparePackets(pRxTransferOp[0U], packetCount,
                byteSize, pTxBuffer[1U]);
        }
    }

    /* End time */
    finish = GET_TIME();

    /* Display the results */
    DisplayResults(start, finish, compare, byteSize, packetCount, loopCount,
        errorCount, "Multi-Packet Double Loopback Test");

LoopBackDouble_MultiPacket_Finish:

    /* For each channel */
    for (i = 0U; i < 2U; i++)
    {
        /* Dispose of transfer operations, */
        STAR_disposeTransferOperation(pTransferOpList[i][0U]);
        STAR_disposeTransferOperation(pTransferOpList[i][1U]);

        /* Destroy the packets transmitted */
        if (ppTxStreamItems[i] != NULL)
        {
            for (packetNum = 0U; packetNum < packetCount; packetNum++)
            {
                if (ppTxStreamItems[i][packetNum] != NULL)
                {
                    STAR_destroyStreamItem(ppTxStreamItems[i][packetNum]);
                }
            }
            free(ppTxStreamItems[i]);
        }

        /* Free the transmit buffer */
        if (pTxBuffer[i] != NULL)
        {
            free(pTxBuffer[i]);
        }

        /* Free the address path */
        if (pAddressPath[i] != NULL)
        {
            STAR_destroyAddress(pAddressPath[i]);
        }

        /* Close the channel */
        if (channelId[i] != 0U)
        {
            STAR_closeChannel(channelId[i]);
        }
    }
}

/**
 * \ingroup StarSystemTester
 * Transmits a single SpaceWire packet using one transfer operation.
 */
void Transmit_SinglePacket(void)
{
    STAR_CHANNEL_ID txChannelId = 0U;
    char *pTxBuffer = NULL;
    STAR_TRANSFER_OPERATION *pTxTransferOp = NULL;
    STAR_STREAM_ITEM *pTxStreamItem = NULL;
    unsigned long loopCount, byteSize, testNum; 
    clock_t start, finish;
    STAR_TRANSFER_STATUS txStatus;
    STAR_SPACEWIRE_ADDRESS *pAddressPath;

    /* Select the transmit device and channel to be used */
    if (chooseDeviceAndChannel("transmit", &txChannelId,
        STAR_CHANNEL_DIRECTION_OUT) == 0)
    {
        return;
    }

    /* Select the path to be added to the front of packets transmitted */
    pAddressPath = getTransmitPathAddress();
     
    printf("pAddressPath-%d\n",pAddressPath->pPath);
    
    /* Get packet size */
    if (getPacketSize(&byteSize) == 0)
    {
        goto Transmit_SinglePacket_Finish;
    }

    /* Get loop count */
    if (getLoopCount(&loopCount) == 0)
    {
        goto Transmit_SinglePacket_Finish;
    }

    /* Allocate memory for the transmit buffer */
    pTxBuffer = (char *)calloc(1U, byteSize);
    if (pTxBuffer == NULL)
    {
        puts("\nERROR: Unable to allocate memory for transmit buffer");
        goto Transmit_SinglePacket_Finish;
    }

    /* Fill transmit buffer with random data, receive buffer with zeros */
   
    // FillBufferRandomChar(pTxBuffer, byteSize, DATA_TYPE_RANDOM);
   pTxBuffer[0]=0x02;
   pTxBuffer[1]=0x76;
   pTxBuffer[2]=0xff;
   pTxBuffer[3]=0x77;
   pTxBuffer[4]=0x04;
   pTxBuffer[5]=0x7e;
   pTxBuffer[6]=0x00;
   pTxBuffer[7]=0x4A;
    byteSize=20;
        int i=0;
    for(i=0;i<byteSize;i++)
    printf("%d \n",pTxBuffer[i]);
    /* Create the packet to be transmitted */
    pTxStreamItem = STAR_createPacket(pAddressPath, (U8 *)pTxBuffer, byteSize,
        STAR_EOP_TYPE_EOP);
    if (pTxStreamItem == NULL)
    {
        puts("\nERROR: Unable to create the packet to be transmitted");
        goto Transmit_SinglePacket_Finish;
    }

    /* Create the transmit transfer operation for the packet */
    pTxTransferOp = STAR_createTxOperation(&pTxStreamItem, 1U);
    printf("pTxTransferOp--%d\n",*pTxTransferOp);
    if (pTxTransferOp == NULL)
    {
        puts("\nERROR: Unable to create the transfer operation to be transmitted");
        goto Transmit_SinglePacket_Finish;
    }

    puts("Starting Single-Packet Transmit Test...");

    /* Start time */
    start = GET_TIME();

    for (testNum = 0U; testNum < loopCount; testNum++)
    {
        /* Submit the transmit operation */
        if (STAR_submitTransferOperation(txChannelId, pTxTransferOp) == 0)
        {
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1U);
            goto Transmit_SinglePacket_Finish;
        }

        /* Wait on the transmit operation completing */
        txStatus = STAR_waitOnTransferOperationCompletion(pTxTransferOp,
            STAR_INFINITE);
        if (txStatus != STAR_TRANSFER_STATUS_COMPLETE)
        {
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1U);
            goto Transmit_SinglePacket_Finish;
        }
    }

    /* End time */
    finish = GET_TIME();

    /* Display the results */
    DisplayResults(start, finish, 0, byteSize, 1U, loopCount, 0U,
        "Single-Packet Transmit Test");

Transmit_SinglePacket_Finish:

    /* Dispose of the transfer operation */
    if (pTxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pTxTransferOp);
    }

    /* Destroy the packet transmitted */
    if (pTxStreamItem != NULL)
    {
        STAR_destroyStreamItem(pTxStreamItem);
    }

    /* Free the transmit buffer */
    if (pTxBuffer != NULL)
    {
        free(pTxBuffer);
    }

    /* Free the address */
    if (pAddressPath != NULL)
    {
       STAR_destroyAddress(pAddressPath);
    }

    /* Close the channel */
    if (txChannelId != 0u)
    {
        STAR_closeChannel(txChannelId);
    }
}

/**
 * \ingroup StarSystemTester
 * Transmits multiple SpaceWire packets using a single transfer operation.
 */
void Transmit_MultiPacket(void)
{
    STAR_CHANNEL_ID txChannelId = 0U;
    unsigned long loopCount, byteSize, testNum, packetCount = 0U, i;
    char *pTxBuffer = NULL;
    STAR_STREAM_ITEM **ppTxStreamItems = NULL;
    STAR_TRANSFER_OPERATION *pTxTransferOp = NULL;
    STAR_TRANSFER_STATUS txStatus;
    clock_t start, finish;
    STAR_SPACEWIRE_ADDRESS *pAddressPath;

    /* Select the transmit device and channel to be used */
    if (chooseDeviceAndChannel("transmit", &txChannelId,
        STAR_CHANNEL_DIRECTION_OUT) == 0)
    {
        return;
    }

    /* Select the path to be added to the front of packets transmitted */
    pAddressPath = getTransmitPathAddress();

    /* Get packet size */
    if (getPacketSize(&byteSize) == 0)
    {
        goto Transmit_MultiPacket_Finish;
    }

    /* Get packet count */
    if (getPacketCount(&packetCount) == 0)
    {
        goto Transmit_MultiPacket_Finish;
    }

    /* Get loop count */
    if (getLoopCount(&loopCount) == 0)
    {
        goto Transmit_MultiPacket_Finish;
    }

    /* Allocate memory for the transmit buffer */
    pTxBuffer = (char *)calloc(1U, byteSize * packetCount);
    if (pTxBuffer == NULL)
    {
        puts("\nERROR: Unable to allocate memory for transmit buffer");
        goto Transmit_MultiPacket_Finish;
    }

    /* Fill transmit buffer with random data */
    FillBufferRandomChar(pTxBuffer, byteSize * packetCount, DATA_TYPE_RANDOM);

    /* Allocate memory for the transmit stream item pointers */
    ppTxStreamItems = (STAR_STREAM_ITEM **)calloc(packetCount,
        sizeof(STAR_STREAM_ITEM *));
    if (ppTxStreamItems == NULL)
    {
        puts("\nERROR: Unable to allocate memory for transmit stream items");
        goto Transmit_MultiPacket_Finish;
    }

    /* For each packet */
    for (i = 0U; i < packetCount; i++)
    {
        /* Create the stream item */
        ppTxStreamItems[i] = STAR_createPacket(pAddressPath,
            (U8 *)pTxBuffer + (byteSize * i), byteSize, STAR_EOP_TYPE_EOP);
        if (ppTxStreamItems[i] == NULL)
        {
            printf("\nERROR: Unable to create packet %lu to be transmitted\n",
                i);
            goto Transmit_MultiPacket_Finish;
        }
    }

    /* Create the transmit transfer operation for the packets */
    pTxTransferOp = STAR_createTxOperation(ppTxStreamItems, packetCount);
    if (pTxTransferOp == NULL)
    {
        puts("\nERROR: Unable to create the transfer operation to be transmitted");
        goto Transmit_MultiPacket_Finish;
    }

    puts("Starting Multi-Packet Transmit Test...");

    /* Start time */
    start = GET_TIME();

    for (testNum = 0U; testNum < loopCount; testNum++)
    {
        /* Submit the transmit operation */
        if (STAR_submitTransferOperation(txChannelId,  pTxTransferOp) == 0)
        {
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1U);
            goto Transmit_MultiPacket_Finish;
        }

        /* Wait on the transmit operation completing */
        txStatus = STAR_waitOnTransferOperationCompletion(pTxTransferOp,
            STAR_INFINITE);
        if (txStatus != STAR_TRANSFER_STATUS_COMPLETE)
        {
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1U);
            goto Transmit_MultiPacket_Finish;
        }
    }

    /* End time */
    finish = GET_TIME();

    /* Display the results */
    DisplayResults(start, finish, 0, byteSize, packetCount, loopCount, 0U,
        "Multiple-Packet Transmit Test");

Transmit_MultiPacket_Finish:

    /* Dispose of the transfer operation */
    if (pTxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pTxTransferOp);
    }

    /* Destroy the packets transmitted */
    if (ppTxStreamItems != NULL)
    {
        for (i = 0U; i < packetCount; i++)
        {
            if (ppTxStreamItems[i] != NULL)
            {
                STAR_destroyStreamItem(ppTxStreamItems[i]);
            }
        }
        free(ppTxStreamItems);
    }

    /* Free the transmit buffer */
    if (pTxBuffer != NULL)
    {
        free(pTxBuffer);
    }

    /* Free the address */
    if (pAddressPath != NULL)
    {
        STAR_destroyAddress(pAddressPath);
    }

    /* Close the channel */
    if (txChannelId != 0U)
    {
        STAR_closeChannel(txChannelId);
    }
}

/**
 * \ingroup StarSystemTester
 * Receives a single SpaceWire packet using one transfer operation.
 */
void Receive_SinglePacket(void)
{
    STAR_CHANNEL_ID rxChannelId = 0U;
    STAR_TRANSFER_OPERATION *pRxTransferOp = NULL;
    unsigned long loopCount, rxByteSize = 0U, testNum;
    clock_t start, finish;
    STAR_TRANSFER_STATUS rxStatus;

    /* Select the receive device and channel to be used */
    if (chooseDeviceAndChannel("receive", &rxChannelId,
        STAR_CHANNEL_DIRECTION_IN) == 0)
    {
        goto Receive_SinglePacket_Finish;
    }

    /* Get loop count */
    if (getLoopCount(&loopCount) == 0)
    {
        goto Receive_SinglePacket_Finish;
    }

    /* Create receive operation to receive 1 packet */
    pRxTransferOp = STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);
    printf("pRxTransferOp--%d\n",*pRxTransferOp);
    if (pRxTransferOp == NULL)
    {
        puts("\nERROR: Unable to create receive operation");
        goto Receive_SinglePacket_Finish;
    }

    puts("Running Single-Packet Receive Test...");

    /* Start time (this should be updated when the first packet is received) */
    start = GET_TIME();

    for (testNum = 0U; testNum < loopCount; testNum++)
    {
        /* Submit the receive operation */
        if (STAR_submitTransferOperation(rxChannelId, pRxTransferOp) == 0)
        {
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1U);
            goto Receive_SinglePacket_Finish;
        }

        /* Wait on the receive operation completing */
        rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp,
            STAR_INFINITE);
        if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
        {
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1U);
            goto Receive_SinglePacket_Finish;
        }

        /* Start time after first packet received */
        if (testNum == 0U)
        {
            start = GET_TIME();
        }
        else
        {
            rxByteSize += getPacketLengths(pRxTransferOp, 1U); 
        }
    }

    /* End time */
    finish = GET_TIME();

    /* Display the results */
    DisplayResults(start, finish, 0, rxByteSize, 1U, loopCount, 0U,
        "Single-Packet Receive Test");

            

Receive_SinglePacket_Finish:

    /* Dispose of the transfer operation */
    if (pRxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pRxTransferOp);
    }

    /* Close the channel */
    if (rxChannelId != 0U)
    {
        STAR_closeChannel(rxChannelId);
    }
}

/**
 * \ingroup StarSystemTester
 * Receives multiple SpaceWire packets using a single transfer operation.
 */
void Receive_MultiPacket(void)
{
    STAR_CHANNEL_ID rxChannelId = 0U;
    unsigned long loopCount, rxByteSize = 0U, testNum, packetCount;
    STAR_TRANSFER_OPERATION *pRxTransferOp = NULL;
    STAR_TRANSFER_STATUS rxStatus;
    clock_t start, finish;

    /* Select the receive device and channel to be used */
    if (chooseDeviceAndChannel("receive", &rxChannelId,
        STAR_CHANNEL_DIRECTION_IN) == 0)
    {
        goto Receive_MultiPacket_Finish;
    }

    /* Get packet count */
    if (getPacketCount(&packetCount) == 0)
    {
        goto Receive_MultiPacket_Finish;
    }

    /* Get loop count */
    if (getLoopCount(&loopCount) == 0)
    {
        goto Receive_MultiPacket_Finish;
    }

    /* Create receive operation to receive the packets */
    pRxTransferOp = STAR_createRxOperation(packetCount, STAR_RECEIVE_PACKETS);
    if (pRxTransferOp == NULL)
    {
        puts("\nERROR: Unable to create receive operation");
        goto Receive_MultiPacket_Finish;
    }

    puts("Running Multiple-Packet Receive Test...");

    /* Start time */
    /* (this should be updated when the first packets are received) */
    start = GET_TIME();

    for (testNum = 0U; testNum < loopCount; testNum++)
    {
        /* Submit the receive operation */
        if (STAR_submitTransferOperation(rxChannelId, pRxTransferOp) == 0)
        {
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1U);
            goto Receive_MultiPacket_Finish;
        }

        /* Wait on the receive operation completing */
        rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp,
            STAR_INFINITE);
        if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
        {
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1U);
            goto Receive_MultiPacket_Finish;
        }

        /* Start time after first packet received */
        if (testNum == 0U)
        {
            start = GET_TIME();
        }
        else
        {
            rxByteSize += getPacketLengths(pRxTransferOp, packetCount);
        }
    }

    /* End time */
    finish = GET_TIME();

    /* Display the results */
    DisplayResults(start, finish, 0, rxByteSize, packetCount, loopCount, 0U,
        "Multi-Packet Receive Test");

Receive_MultiPacket_Finish:

    /* Dispose of the transfer operation */
    if (pRxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pRxTransferOp);
    }

    /* Close the channel */
    if (rxChannelId != 0U)
    {
        STAR_closeChannel(rxChannelId);
    }
}

/**
 * Gets a yes or no answer from user
 *
 * @return 1 if user input Y, 0 if user input N
 */
int ConfirmYes(void)
{
    char response[256] = { '\0' };
    size_t responseLen = 0U;

    for (;;)
    {
        if (fgets(response, 256, stdin) == NULL)
        {
            puts("ERROR: Invalid Input");
            return 0;
        }

         /* Strip newline */
        responseLen = strlen(response);
        if (response[responseLen - 1U] == '\n')
        {
            response[responseLen - 1U] = '\0';
        }

        if ((strcmp(response, "y") == 0) || (strcmp(response,"Y") == 0))
        {
            return 1;
        }
        else if ((strcmp(response, "n") == 0)  || (strcmp(response, "N") == 0))
        {
            return 0;
        }
        else
        {
            puts("Please enter Y/N");
        }
    }
}

/**
 * \ingroup StarSystemTester
 *
 *  Transmits a file whole, as one packet. Not preceded by a header packet.
 */
void TransmitFile_Whole(void)
{
    STAR_CHANNEL_ID txChannelId = 0U;
    char *pTxBuffer = NULL;
    STAR_TRANSFER_OPERATION *pTxTransferOp = NULL;
    STAR_STREAM_ITEM *pTxStreamItem = NULL;
    STAR_TRANSFER_STATUS txStatus;
    STAR_SPACEWIRE_ADDRESS *pAddressPath;
    char sFile[256];
    int fileSize = 0;

    size_t sFileLen;

    /* Select the transmit device and channel to be used */
    if (chooseDeviceAndChannel("transmit", &txChannelId,
        STAR_CHANNEL_DIRECTION_OUT) == 0)
    {
        return;
    }

    /* Select the path to be added to the front of packets transmitted */
    pAddressPath = getTransmitPathAddress();

    /* Get the File name */
    printf("Enter name of file to transmit: ");
    fflush(stdout);
    if (fgets(sFile, 256, stdin) == NULL)
    {
        puts("ERROR: Invalid Input");
        return;
    }

    /* Strip newline */
    sFileLen = strlen(sFile);
    if (sFile[sFileLen - 1U] == '\n')
    {
        sFile[sFileLen - 1U] = '\0';
    }

    printf("Filename = \"%s\"\n", sFile);
    printf("Continue with these values ? (Y/N): ");
    fflush(stdout);

    if (ConfirmYes() == 0)
    {
        puts("File Transfer aborted");
        goto Transmit_File_Whole_Finish;
    }

    fileSize = SizeOfFile(sFile);
    if (fileSize <= 0)
    {
        puts("File size invalid, aborting transfer.");
        goto Transmit_File_Whole_Finish;
    }

    /* Allocate memory to read file into */
    pTxBuffer = (char *)malloc(fileSize);
    if (pTxBuffer == NULL)
    {
        puts("ERROR: Could not allocate buffer for file");
        goto Transmit_File_Whole_Finish;
    }

    /* Read the file */
    if (file_read_chunk(sFile, pTxBuffer, 0U, fileSize) == 0)
    {
        puts("ERROR: Could not read file");
        goto Transmit_File_Whole_Finish;
    }

   pTxStreamItem = STAR_createPacket(pAddressPath, (U8 *)pTxBuffer, fileSize,
        STAR_EOP_TYPE_EOP);
    if (pTxStreamItem == NULL)
    {
        puts("ERROR: Unable to create the packet to be transmitted");
        goto Transmit_File_Whole_Finish;
    }

    /* Create the transmit transfer operation for the packet */
    pTxTransferOp = STAR_createTxOperation(&pTxStreamItem, 1U);
    if (pTxTransferOp == NULL)
    {
        puts("ERROR: Unable to create the transfer operation to be transmitted");
        goto Transmit_File_Whole_Finish;
    }

    puts("Starting File Transmit...");

    if (STAR_submitTransferOperation(txChannelId, pTxTransferOp) == 0)
    {
        puts("ERROR occurred during transmit");
        goto Transmit_File_Whole_Finish;
    }

    /* Wait on the transmit operation completing */
    txStatus = STAR_waitOnTransferOperationCompletion(pTxTransferOp,
        STAR_INFINITE);
    if (txStatus != STAR_TRANSFER_STATUS_COMPLETE)
    {
        puts("ERROR occurred during transmit");
        goto Transmit_File_Whole_Finish;
    }

    puts("Complete.");

Transmit_File_Whole_Finish:
    /* Dispose of the transfer operation */
    if (pTxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pTxTransferOp);
    }

    /* Destroy the packet transmitted */
    if (pTxStreamItem != NULL)
    {
        STAR_destroyStreamItem(pTxStreamItem);
    }

    /* Free the transmit buffer */
    if (pTxBuffer != NULL)
    {
        free(pTxBuffer);
    }

    /* Free the address */
    if (pAddressPath != NULL)
    {
        STAR_destroyAddress(pAddressPath);
    }

    /* Close the channel */
    if (txChannelId != 0U)
    {
        STAR_closeChannel(txChannelId);
    }
}

/**
 * \ingroup StarSystemTester
 * Receives a file which has been transmitted whole, as one packet without a
 * size header.
 *
 */
void ReceiveFile_Whole(void)
{
    STAR_CHANNEL_ID rxChannelId = 0U;
    STAR_TRANSFER_OPERATION *pRxTransferOp = NULL;
    STAR_TRANSFER_STATUS rxStatus;
    char sFile[256];
    int writeStatus;
    STAR_SPACEWIRE_PACKET *pReceivedPacket = NULL;
    size_t sFileLen;
    U8 *receivedData = NULL;
    unsigned int receivedDataLen = 0U;

    /* Select the receive device and channel to be used */
    if (chooseDeviceAndChannel("receive", &rxChannelId,
        STAR_CHANNEL_DIRECTION_IN) == 0)
    {
        goto Receive_File_Whole_Finish;
    }

    /* Get the File name */
    printf("Enter name of file to write data to: ");
    fflush(stdout);
    if (fgets(sFile, 256, stdin) == NULL)
    {
        puts("ERROR: Invalid Input");
        return;
    }

    /* Strip newline */
    sFileLen = strlen(sFile);
    if (sFile[sFileLen - 1U] == '\n')
    {
        sFile[sFileLen - 1U] = '\0';
    }

    printf("Filename = \"%s\"\n", sFile);
    printf("Continue with these values ? (Y/N): ");
    fflush(stdout);

    if (ConfirmYes() == 0)
    {
        printf("File Transfer aborted\n");
        goto Receive_File_Whole_Finish;
    }

    puts("Running File Receive...");

    /* Create receive operation to receive 1 packet */
    pRxTransferOp = STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);
    if (pRxTransferOp == NULL)
    {
        puts("ERROR: Unable to create receive operation");
        goto Receive_File_Whole_Finish;
    }

    /* Submit the receive operation */
    if (STAR_submitTransferOperation(rxChannelId, pRxTransferOp) == 0)
    {
        puts("ERROR: Could not submit receive operation");
        goto Receive_File_Whole_Finish;
    }

    /* Wait on the receive operation completing */
    rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp,
        STAR_INFINITE);
    if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
    {
        printf("ERROR: Error of %d occurred during receive: \n", rxStatus);
        goto Receive_File_Whole_Finish;
    }

    /* Get received data*/
    pReceivedPacket = (STAR_SPACEWIRE_PACKET*)STAR_getTransferItem(
        pRxTransferOp, 0U)->item;

    receivedData = STAR_getPacketData(pReceivedPacket, &receivedDataLen);
    if (receivedData == NULL)
    {
        puts("ERROR: No data received");
        goto Receive_File_Whole_Finish;
    }

    writeStatus = file_append_chunk(receivedData, receivedDataLen, sFile);

    STAR_destroyPacketData(receivedData);

    if (writeStatus == 0)
    {
        puts("ERROR: Could not write file");
        goto Receive_File_Whole_Finish;
    }

    puts("Complete.");

Receive_File_Whole_Finish:

    /* Dispose of the transfer operation */
    if (pRxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pRxTransferOp);
    }

    /* Close the channel */
    if (rxChannelId != 0U)
    {
        STAR_closeChannel(rxChannelId);
    }
}

/**
 * \ingroup StarSystemTester
 * Transmits a file as multiple packets preceded by a header packet indicating
 * file size.
 *
 */
void TransmitFile_Split(void)
{
    STAR_CHANNEL_ID txChannelId = 0U;
    unsigned long chunkSize, dataPacketCount = 0U, i;
    U8 *pTxBuffer = NULL;
    STAR_TRANSFER_OPERATION *pTxTransferOp = NULL;
    STAR_STREAM_ITEM** packets = NULL;
    STAR_TRANSFER_STATUS txStatus;
    STAR_SPACEWIRE_ADDRESS *pAddressPath;
    char sFile[256], sChunkSize[256];
    int status;
    int fileSize = 0;
    U32 fileSizeToSend = 0U;
    unsigned int offset = 0U;
    size_t sFileLen;

    /* Select the transmit device and channel to be used */
    if (chooseDeviceAndChannel("transmit", &txChannelId,
        STAR_CHANNEL_DIRECTION_OUT) == 0)
    {
        return;
    }

    /* Select the path to be added to the front of packets transmitted */
    pAddressPath = getTransmitPathAddress();

    /* Get the File name */
    printf("Enter name of file to transmit: ");
    fflush(stdout);
    if (fgets(sFile, 256, stdin) == NULL)
    {
        puts("ERROR: Invalid Input");
        return;
    }

    /* Strip newline */
    sFileLen = strlen(sFile);
    if (sFile[sFileLen - 1U] == '\n')
    {
        sFile[sFileLen - 1U] = '\0';
    }

    printf("Filename = \"%s\"\n", sFile);
    printf("Continue with these values ? (Y/N): ");
    fflush(stdout);

    if (ConfirmYes() == 0)
    {
        printf("File Transfer aborted\n");
        goto Transmit_File_Packets_Finish;
    }

    /* Allocate memory to read file into */
    fileSize = SizeOfFile(sFile);
    if (fileSize <= 0)
    {
        printf("File size invalid, aborting transfer.\n");
        goto Transmit_File_Packets_Finish;
    }

    /* Read the file into a buffer */
    pTxBuffer = (U8 *)malloc(fileSize);
    if (pTxBuffer == NULL)
    {
        puts("ERROR: Could not allocate buffer for file");
        goto Transmit_File_Packets_Finish;
    }

    if (file_read_chunk(sFile, (char*)pTxBuffer, 0U, fileSize) == 0)
    {
        puts("ERROR: Could not read file");
        goto Transmit_File_Packets_Finish;
    }

    /* Get packet size */
    printf("Enter maximum size of packets to transmit, in bytes: ");
    fflush(stdout);
    if (fgets(sChunkSize, 256, stdin) == NULL)
    {
        puts("ERROR: Invalid input");
        goto Transmit_File_Packets_Finish;
    }
    status = sscanf(sChunkSize, "%lu", &chunkSize);
    if (status == 0)
    {
        puts("ERROR: Invalid input");
        goto Transmit_File_Packets_Finish;
    }

    /* Calculate number of packets to send */
    dataPacketCount = ((fileSize / chunkSize) +
        (((fileSize % chunkSize) > 0U) ? 1U : 0U));

    /* Prepare packets for transmission */
    packets = (STAR_STREAM_ITEM **)calloc(dataPacketCount + 1U,
        sizeof(STAR_STREAM_ITEM *));
    if (packets == NULL)
    {
        puts("ERROR: Could not allocate buffer for packets");
        goto Transmit_File_Packets_Finish;
    }

    /* Initial four byte packet size */
    CopyNumberToMemory(&fileSizeToSend, fileSize, sizeof(U32));
    packets[0U] = STAR_createPacket(pAddressPath, (U8*)&fileSizeToSend,
        sizeof(U32), STAR_EOP_TYPE_EOP);

    /* Data packets*/
    for (i = 1U; i < dataPacketCount; i++)
    {
        packets[i] = STAR_createPacket(pAddressPath, pTxBuffer + offset,
            chunkSize, STAR_EOP_TYPE_EOP);
        offset+= chunkSize;
    }

    /* Final Data packet*/
    if ((fileSize % chunkSize) > 0)
    {
        packets[dataPacketCount] = STAR_createPacket(pAddressPath,
            pTxBuffer + offset, fileSize % chunkSize, STAR_EOP_TYPE_EOP);
    }
    else
    {
        packets[dataPacketCount] = STAR_createPacket(pAddressPath,
            pTxBuffer + offset, chunkSize, STAR_EOP_TYPE_EOP);
    }

    pTxTransferOp = STAR_createTxOperation(packets, dataPacketCount + 1U);

    puts("Starting File Transmit...");

    if (STAR_submitTransferOperation(txChannelId, pTxTransferOp) == 0)
    {
        puts("ERROR: Could not submit transmit operation");
        goto Packet_Cleanup;
    }

     /* Wait on the transmit operation completing */
    txStatus = STAR_waitOnTransferOperationCompletion(pTxTransferOp,
        STAR_INFINITE);
    if (txStatus != STAR_TRANSFER_STATUS_COMPLETE)
    {
        printf("ERROR: Error %d occurred during transmit\n", txStatus);
        goto Packet_Cleanup;
    }

    puts("Complete.");

Packet_Cleanup:
    /* Cleanup packets */
    for (i = 0U; i <= dataPacketCount; i++)
    {
        STAR_destroyStreamItem(packets[i]);
    }

Transmit_File_Packets_Finish:

    free(packets);

    /* Dispose of the transfer operation */
    if (pTxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pTxTransferOp);
    }

    /* Free the transmit buffer */
    if (pTxBuffer != NULL)
    {
        free(pTxBuffer);
    }

    /* Free the address path */
    if (pAddressPath != NULL)
    {
        STAR_destroyAddress(pAddressPath);
    }

    /* Close the channel */
    if (txChannelId != 0U)
    {
        STAR_closeChannel(txChannelId);
    }
}

/**
 * \ingroup StarSystemTester
 * Receives a file which has been transmitted as multiple packets preceded by
 * a header packet indicating file size
 *
 */
void ReceiveFile_Split(void)
{
    STAR_CHANNEL_ID rxChannelId = 0U;
    STAR_TRANSFER_OPERATION *pRxTransferOp = NULL;
    STAR_TRANSFER_STATUS rxStatus;
    char sFile[256];
    int writeStatus;
    STAR_SPACEWIRE_PACKET *pReceivedPacket = NULL;
    size_t sFileLen = 0U;
    clock_t start, finish;
    U8 *receivedData = NULL;
    unsigned int receivedDataLen = 0U, expectedFileSize = 0U;
    unsigned int actualFileSize = 0U;

    /* Select the receive device and channel to be used */
    if (chooseDeviceAndChannel("receive", &rxChannelId,
        STAR_CHANNEL_DIRECTION_IN) == 0)
    {
        goto Receive_File_Split_Finish;
    }

    /* Get the File name */
    printf("Enter name of file to write data to: ");
    fflush(stdout);
    if (fgets(sFile, 256, stdin) == NULL)
    {
        puts("ERROR: Invalid input");
        return;
    }

    /* Strip newline */
    sFileLen = strlen(sFile);
    if (sFile[sFileLen - 1U] == '\n')
    {
        sFile[sFileLen - 1U] = '\0';
    }

    printf("Filename = \"%s\"\n", sFile);
    printf("Continue with these values ? (Y/N): ");
    fflush(stdout);

    if (ConfirmYes() == 0)
    {
        puts("File Transfer aborted");
        goto Receive_File_Split_Finish;
    }

    puts("Running File Receive...");

    /* Create receive operation to receive Header packet */
    pRxTransferOp = STAR_createRxOperation(1, STAR_RECEIVE_PACKETS);
    if (pRxTransferOp == NULL)
    {
        puts("ERROR: Unable to create receive operation");
        goto Receive_File_Split_Finish;
    }

    /* Submit the receive operation */
    if (STAR_submitTransferOperation(rxChannelId, pRxTransferOp) == 0)
    {
        puts("ERROR: Could not submit receive operation");
        goto Receive_File_Split_Finish;
    }

    /* Wait on the receive operation completing */
    rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp,
        STAR_INFINITE);
    if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
    {
        printf("ERROR: Error %d occurred during receive\n", rxStatus);
        goto Receive_File_Split_Finish;
    }

    pReceivedPacket = (STAR_SPACEWIRE_PACKET *)STAR_getTransferItem(
        pRxTransferOp, 0U)->item;
    receivedData = STAR_getPacketData(pReceivedPacket, &receivedDataLen);
    if (receivedData == NULL)
    {
        puts("ERROR: No data received");
        goto Receive_File_Split_Finish;
    }

    CopyNumberFromMemory(&expectedFileSize, receivedData,
        sizeof(expectedFileSize));

    STAR_destroyPacketData(receivedData);

    /* Start the timer */
    start = GET_TIME();

    /* Read a packet at a time until we get all expected data */
    while(expectedFileSize > actualFileSize)
    {
        if (STAR_submitTransferOperation(rxChannelId, pRxTransferOp) == 0)
        {
            puts("ERROR: Could not submit receive operation");
            goto Receive_File_Split_Finish;
        }

        rxStatus = STAR_waitOnTransferOperationCompletion(pRxTransferOp,
            STAR_INFINITE);
        if (rxStatus != STAR_TRANSFER_STATUS_COMPLETE)
        {
            printf("ERROR: Error %d occurred during receive\n", rxStatus);
            goto Receive_File_Split_Finish;
        }

        pReceivedPacket = (STAR_SPACEWIRE_PACKET *)STAR_getTransferItem(
            pRxTransferOp, 0U)->item;

        receivedData = STAR_getPacketData(pReceivedPacket, &receivedDataLen);
        if (receivedData == NULL)
        {
            puts("ERROR: No data received");
            goto Receive_File_Split_Finish;
        }

        writeStatus = file_append_chunk(receivedData, receivedDataLen, sFile);

        STAR_destroyPacketData(receivedData);

        if (writeStatus == 0)
        {
            puts("ERROR: Could not write to file");
            goto Receive_File_Split_Finish;
        }

        actualFileSize+=receivedDataLen;
    }

    if (actualFileSize != expectedFileSize)
    {
        puts("ERROR: Expected and actual file size do not match");
    }

    /* End time */
    finish = GET_TIME();

    /* Display the results */
    DisplayResults(start, finish, 0, actualFileSize, 1U, 1U, 0U,
        "Receive File from multiple packets");

Receive_File_Split_Finish:

    /* Dispose of the transfer operation */
    if (pRxTransferOp != NULL)
    {
        STAR_disposeTransferOperation(pRxTransferOp);
    }

    /* Close the channel */
    if (rxChannelId != 0U)
    {
        STAR_closeChannel(rxChannelId);
    }
}

/**
 * \ingroup StarSystemTester
 * Resets a device
 */
void ResetDevice(void)
{
    STAR_DEVICE_ID deviceId;

    deviceId = chooseDevice("", STAR_DEVICE_ALL);
    if (deviceId == STAR_DEVICE_UNKNOWN)
    {
        return;
    }

    if (STAR_resetDevice(deviceId) == 0)
    {
        puts("ERROR: Unable to reset device");
    }
}

/**
 * \ingroup StarSystemTester
 * Identifies a device by flashing its LEDs
 */
void IdentifyDevice(void)
{
    STAR_DEVICE_ID deviceId;

    deviceId = chooseDevice("", STAR_DEVICE_CONFIG_SUPPORTED);
    if (deviceId == STAR_DEVICE_UNKNOWN)
    {
        return;
    }

    if (CFG_MK2_identify(deviceId) == 0)
    {
        puts("ERROR: Unable to identify device");
    }
}

/**
 * \ingroup StarSystemTester
 * Runs the program in interactive mode.
 */
void runInteractive(void)
{
    char s[256];
    int menuSelect;
    int bExit = 0;

    puts("STAR-System Test Program");
    puts("Copyright STAR-Dundee Ltd. (c) 2011-2017");
    puts("www.star-dundee.com");

    /* Loop until exit option is chosen */
    while (bExit == 0)
    {
        DisplayMenu();

        if (fgets(s, 256, stdin) == NULL)
        {
            puts("\nERROR: No value specified");
        }
        else if (sscanf(s, "%d", &menuSelect) == 0)
        {
            puts("\nERROR: Invalid value specified");
        }
        else
        {
            switch(menuSelect)
            {
            case MENU_DISPLAY_INFORMATION:
                DisplayInformation();
                break;

            case MENU_LOOPBACK_SINGLE:
                LoopBack_SinglePacket();
                break;

            case MENU_LOOPBACK_MULTI:
                LoopBack_MultiPacket();
                break;

            case MENU_LOOPBACK_DOUBLE_SINGLE:
                LoopBackDouble_SinglePacket();
                break;

            case MENU_LOOPBACK_DOUBLE_MULTI:
                LoopBackDouble_MultiPacket();
                break;

            case MENU_TRANSMIT_SINGLE:
                Transmit_SinglePacket();
                break;

            case MENU_TRANSMIT_MULTI:
                Transmit_MultiPacket();
                break;

            case MENU_RECEIVE_SINGLE:
                Receive_SinglePacket();
                break;

            case MENU_RECEIVE_MULTI:
                Receive_MultiPacket();
                break;

            case MENU_TRANSMIT_FILE_WHOLE:
                TransmitFile_Whole();
                break;

            case MENU_RECEIVE_FILE_WHOLE:
                ReceiveFile_Whole();
                break;

            case MENU_TRANSMIT_FILE_SPLIT:
                TransmitFile_Split();
                break;

            case MENU_RECEIVE_FILE_SPLIT:
                ReceiveFile_Split();
                break;

            case MENU_RESET_DEVICE:
                ResetDevice();
                break;

            case MENU_IDENTIFY_DEVICE:
                IdentifyDevice();
                break;

            case MENU_EXIT:
                puts("\nExiting SpaceWire test program");
                bExit = 1;
                break;

            default:
                puts("\nERROR: Incorrect menu option");
                break;
            }
        }
    }

    puts("Exiting...\n");
}

/**
 * \ingroup StarSystemTester
 * Displays usage for command line arguments.
 *
 * @param[in] argv   Array of arguments passed to the program
 * @param     error  Whether to send usage info to stderr (1) or stdout (0)
 */
void usage(char *argv[], const int error)
{
    puts("");

    if (error != 0)
    {
        fprintf(stderr, "usage: %s [-v][-help]\n", argv[0]);
    }
    else
    {
        fprintf(stdout, "usage: %s [-v][-help]\n", argv[0]);
    }
}

/**
 * \ingroup StarSystemTester
 * Displays program description and describes optional
 * command line arguments.
 */
void showHelp(void)
{
    puts("");
    puts("Description:");
    puts("  A program to display information about STAR-System,");
    puts("  the STAR-Dundee API stack, and perform basic transmit");
    puts("  and receive tests using attached hardware.");
    puts("");
    puts("Optional Arguments:");
    puts("");
    puts("  -v       Displays version information and exits.");
    puts("");
    puts("  -help    Displays this help message and exits.");
    puts("");
    puts("  When called without arguments, the program is run in");
    puts("  interactive mode.");
}

/**
 * \ingroup StarSystemTester
 * Main function, called when the program is started.
 *
 * @param argc the number of parameters passed to the program.
 * @param argv the array of arguments passed to the program.
 *
 * @return returns 0 on normal program ending, otherwise 1
 */
int __cdecl main(int argc, char *argv[])
{
    int i;
    int version = 0;  /* Value for the "-v" optional argument. */
    int help = 0;     /* Value for the "-help" optional argument. */

    /* If no args provided, interactive mode */
    if (argc <= 1)
    {
       runInteractive();
       return 0;
    }

    /* Currently we only expect 1 optional argument */
    if (argc > 2)
    {
        usage(argv, 1);
        return 1;
    }

    /* Process command line args*/
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            version = 1;
        }
        else if (strcmp(argv[i], "-help") == 0)
        {
            help = 1;
        }
    }

    if (help != 0)
    {
        usage(argv, 0);
        showHelp();
    }
    else if (version != 0)
    {
        puts(VERSION_INFO);
        puts("Copyright STAR-Dundee Ltd. (c) 2011-2017");
        puts("www.star-dundee.com");

        DisplayInformation();
    }
    else
    {
        usage(argv, 1);
        return 1;
    }

    return 0;
}
