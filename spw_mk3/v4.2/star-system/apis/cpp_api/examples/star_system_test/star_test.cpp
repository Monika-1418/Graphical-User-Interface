/**
 * \file star_test.cpp
 *
 * \brief Implementation for STAR-System Test program.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Contains the implementation of the Star-System Test program.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "star_test.h"
#include "device_type_resolver.h"
#include "cpp_api_macros.hpp"
#include <cerrno>

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices;

/**
 * Runs the StarSystemTester program, using the command line arguments
 * provided.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv An array of command line arguments passed to the program.
 *
 * @return 0 if program completed normally, otherwise 1.
 *
 */
int StarSystemTest::Run(int argc, char *argv[])
{
    /* if no command line arguments provided, other than executable's name */
    if (argc == 1)
    {
        /* run the STAR-System test program */
        RunTestProgram();

        /* return success / normal program end */
        return 0;
    }

    /* if there is more than one optional command line argument */
    if (argc > 2)
    {
        /* only one optional argument at a time is expected */

        /* report correct usage of program */
        DisplayUsage(argv, true);

        /* return error code */
        return 1;
    }

    /* an optional command line argument has been provided, process it */
    int result = ProcessCommandLineArgument(argv);

    /* if fail code returned from processing command line arguments */
    if (result != 0)
    {
        /* return the fail code */
        return result;
    }

    /* end program normally */
    return 0;
}

/**
 * Run the main test program, providing options for the user to select
 * from to test different aspects of the STAR-System API.
 */
void StarSystemTest::RunTestProgram()
{
    char s[256];
    int menuSelect;
    char bExit = 0;

    puts("STAR-System Test Program");
    puts("Copyright STAR-Dundee Ltd. (c) 2013-2018");
    puts("www.star-dundee.com");

    /* loop until exit option is chosen */
    while (!bExit)
    {
        /* display the main program menu */
        DisplayMenu();

        if (!fgets(s, 256, stdin))
        {
            puts("\nERROR: No value specified");
        }
        else if (!sscanf(s, "%d", &menuSelect))
        {
            puts("\nERROR: Invalid value specified");
        }
        else
        {
            switch(menuSelect)
            {
            case MENU_DISPLAY_INFORMATION:
                DisplayAPIVersionAndDeviceInformation();
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
 * Display the main options menu for the Star System Test Program.
 */
void StarSystemTest::DisplayMenu(void)
{
    puts("\nSelect Option:");
    printf(" (%d)  Display Device and Version Information\n",
        MENU_DISPLAY_INFORMATION);
    printf(" (%d)  Loopback Test, Single Packet (Data Compare, Link Speed)\n",
        MENU_LOOPBACK_SINGLE);
    printf(" (%d)  Loopback Test, Multiple Packets (Data Compare,"
        " Link Speed)\n", MENU_LOOPBACK_MULTI);
    printf(" (%d)  Double Loopback Test, Single Packet (Data Compare,"
        " Link Speed)\n", MENU_LOOPBACK_DOUBLE_SINGLE);
    printf(" (%d)  Double Loopback Test, Multiple Packets (Data Compare,"
        " Link Speed)\n", MENU_LOOPBACK_DOUBLE_MULTI);
    printf(" (%d)  Transmit Single Packet (Link Speed)\n", 
        MENU_TRANSMIT_SINGLE);
    printf(" (%d)  Transmit Multiple Packets (Link Speed)\n",
        MENU_TRANSMIT_MULTI);
    printf(" (%d)  Receive Packet (Link Speed)\n", MENU_RECEIVE_SINGLE);
    printf(" (%d)  Receive Multiple Packets (Link Speed)\n", 
        MENU_RECEIVE_MULTI);
    printf("(%d)  Transmit From File, Single Packet\n", 
        MENU_TRANSMIT_FILE_WHOLE);
    printf("(%d)  Receive To File, Single Packet \n", 
        MENU_RECEIVE_FILE_WHOLE);
    printf("(%d)  Transmit From File, Multiple Packets \n", 
        MENU_TRANSMIT_FILE_SPLIT);
    printf("(%d)  Receive To File, Multiple Packets (Link Speed) \n", 
        MENU_RECEIVE_FILE_SPLIT);
    printf("(%d)  Reset Device\n", MENU_RESET_DEVICE);
    printf("(%d)  Identify Device\n", MENU_IDENTIFY_DEVICE);
    printf(" (%d)  Exit\n", MENU_EXIT);
    printf("Please Select Menu Option: ");
}

/**
 * Displays usage for command line arguments.
 *
 * @param[in] argv   Array of arguments passed to the program
 * @param     error  Whether to send usage info to stderr (true) or
 *                   stdout (false)
 */
void StarSystemTest::DisplayUsage(_In_ char *argv[], bool error)
{
    puts("");

    if(error)
        fprintf(stderr, "usage: %s [-v][-help]\n", argv[0]);
    else
        fprintf(stdout, "usage: %s [-v][-help]\n", argv[0]);
}



/**
 * Process an optional command line argument which has been
 * provided to the program.
 *
 * @param[in] argv Array of arguments passed to the program
 *
 * @return 0 if argument processed normally, otherwise 1.
 */
int StarSystemTest::ProcessCommandLineArgument(_In_ char *argv[])
{
    /* if 'version' argument specified */
    if (strcmp(argv[1], "-v") == 0)
    {
        /* display version */
        DisplayVersion();
    }
    /* else, if 'help' argument specified */
    else if (strcmp(argv[1], "-help") == 0)
    {
        /* display help */
        DisplayHelp(argv);
    }
    else
    {
        /* unrecognised argument */

        /* report usage */
        DisplayUsage(argv, true);

        /* return error  */
        return 1;
    }

    /* return normal completion code */
    return 0;
}

/**
 * Displays version information for the program.
 *
 * @param[in] argv   Array of arguments passed to the program
 */
void StarSystemTest::DisplayVersion()
{
    /* display version information for the program */
    puts(VERSION_INFO);
    puts("Copyright (C) 2013 STAR-Dundee Ltd.");
    puts("http://www.star-dundee.com/");


    /* display version information for all the STAR-System API modules and
     * drivers currently in use, and the properties of all the currently
     * connected devices.
     */
    DisplayAPIVersionAndDeviceInformation();
}

/**
 * Displays help for the program.
 *
 * @param[in] argv   Array of arguments passed to the program
 */
void StarSystemTest::DisplayHelp(_In_ char *argv[])
{
    /* display usage information */
    DisplayUsage(argv, false);

    /* display help information to user */
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
 * Display version information for all the STAR-System API modules and
 * drivers currently in use, and the properties of all the currently
 * connected devices.
 */
void StarSystemTest::DisplayAPIVersionAndDeviceInformation()
{
    /* display API info */
    DisplayAPIVersionAndModules();

    /* display device info */
    DisplayDeviceProperties();

    /* display driver info */
    DisplayDriverProperties();
}

/**
 * Display version information for all the STAR-System API modules and
 * drivers currently in use.
 */
void StarSystemTest::DisplayAPIVersionAndModules()
{
    /* display section header */
    puts("\n\nMODULE VERSION INFORMATION");
    puts("==========================");

    /* display API version */
    puts("\nAPI VERSION:");
    VersionInformation *pApiVersion = STARSystem::GetApiVersion();
    if (pApiVersion)
    {
        /* display the API version information */
        printf(" API name: %s\n", pApiVersion->GetName());
        printf("   Author: %s\n", pApiVersion->GetAuthor());
        printf("  Version: v%d.%d", pApiVersion->GetMajor(),
                                    pApiVersion->GetMinor());
        if (pApiVersion->GetEdit())
        {
            printf("(%d)", pApiVersion->GetEdit());
        }
        if (pApiVersion->GetPatch())
        {
            printf("p%d", pApiVersion->GetPatch());
        }
        puts("\n");

        /* free the version information */
        delete pApiVersion;
    }
    else
    {
        puts("Failed to obtain API version");
    }

    /* all versions */
    puts("\nALL MODULE VERSIONS:");

    /* get the list of all API module versions */
    U32 versionCount = 0;
    VersionInformation **versions = STARSystem::GetAllVersions(&versionCount);

    /* if there are modules present */
    if (versions != NULL)
    {
        /* for each module version item in the list */
        for (U32 i = 0; i < versionCount; i++)
        {
            /* display the module's version information */
            printf("   Module name: %s\n", versions[i]->GetName());
            printf(" Module author: %s\n", versions[i]->GetAuthor());
            printf("Module version: v%d.%d", versions[i]->GetMajor(),
                                             versions[i]->GetMinor());
            if (versions[i]->GetEdit())
            {
                printf("(%d)", versions[i]->GetEdit());
            }
            if (versions[i]->GetPatch())
            {
                printf("p%d", versions[i]->GetPatch());
            }
            puts("\n");
        }

        /* dispose of the version list */
        if (!STARSystem::DestroyVersionList(versions, versionCount))
        {
            puts("Failed to destroy list of versions!\n");
        }

        /* \note DestroyVersionList() is just a helper function. You can
         * free the version list yourself as per below:
         *
         *  for (int i=0; i<versionCount; i++)
         *  {
         *      if (versions[i] != NULL)
         *          delete versions[i];
         *  }
         *  delete [] versions;
         *
         */
    }
    else
    {
        puts("No modules present.\n");
    }
}

/**
 * Display version information and properties for all the currently
 * connected STAR devices.
 */
void StarSystemTest::DisplayDeviceProperties()
{
    char deviceName[Device::STR_DEVICE_NAME_LEN];
    char deviceType[Device::STR_DEVICE_TYPE_LEN];
    char deviceBusType[Device::STR_DEVICE_BUS_TYPE_LEN];
    char manufacturerStr[DeviceIdentifierInfo::STR_DEVICE_MANUFACTURER_LEN];
    char deviceTypeStr[DeviceIdentifierInfo::STR_DEVICE_TYPE_LEN];
    char serialNumber[Device::STR_DEVICE_SERIAL_LEN];
    size_t stringLen;
    DeviceConfiguration *pDeviceConfiguration = NULL;
    DeviceIdentifierInfo *pDeviceIdentifierInfo = NULL;
    DeviceList deviceList;
    U32 deviceCount;
    U32 i, j;
    VersionInformation *versionInfo = NULL, *pDriverVersion = NULL;
    U32 channelMask;
    Driver *pDeviceDriver = NULL;
    DriverFactory *pDriverFactory = NULL;

    /* display section header */
    puts("\nDEVICE INFORMATION");
    puts("==================");

    /* obtain the list of available STAR devices */
    deviceCount = deviceList.Populate();

    /* if there are devices present */
    if (deviceCount)
    {
        /* for each device in the list */
        for (i = 0; i < deviceCount; i++)
        {
            /* get and display the device's name */
            stringLen = deviceList[i]->GetDeviceName(deviceName);
            if (!stringLen)
            {
                puts("            Name: Unable to get device name!");
            }
            else
            {
                printf("            Name: %s\n", deviceName);
            }

            /* get and display the device's type */
            stringLen = deviceList[i]->GetDeviceTypeAsString(deviceType);
            if (!stringLen)
            {
                puts("     Device type: Unable to get device type!");
            }
            else
            {
                printf("     Device type: %s\n", deviceType);
            }

            /* get and display the device's bus type */
            stringLen = deviceList[i]->GetDeviceBusTypeAsString(deviceBusType);
            if (!stringLen)
            {
                puts("        Bus type: Unable to get bus type!");
            }
            else
            {
                printf("        Bus type: %s\n", deviceBusType);
            }

            /* get the device's firmware version */
            versionInfo = deviceList[i]->GetDeviceFirmwareVersion();

            /* if no firmware version found */
            if (versionInfo == NULL)
            {
                puts("Firmware version: Unable to get the firmware version!");
            }
            else
            {
                /* display the firmware version */
                printf("Firmware version: v%d.%d", versionInfo->GetMajor(),
                    versionInfo->GetMinor());
                if (versionInfo->GetEdit())
                {
                    printf("(%d)", versionInfo->GetEdit());
                }
                if (versionInfo->GetPatch())
                {
                    printf("p%d", versionInfo->GetPatch());
                }
                puts("");

                /* free the version information object obtained from the API */
                delete versionInfo;
                versionInfo = NULL;
            }

            /* get and display the device's serial number */
            stringLen = deviceList[i]->GetDeviceSerialNumber(serialNumber);
            if (!stringLen)
            {
                puts("   Serial number: Unable to get serial number!");
            }
            else
            {
                printf("   Serial number: %s\n", serialNumber);
            }

            /* get and display the device's index */
            printf("           Index: %d\n", deviceList[i]->GetDeviceIndex());

            /* get and display the device's channels */
            printf("        Channels:");
            channelMask = deviceList[i]->GetDeviceChannels();
            if (!channelMask)
            {
                puts(" None");
            }
            else
            {
                for (j = 0; j < 32; j++)
                {
                    if ((channelMask >> j) & 1)
                    {
                        printf(" %d", j);
                    }
                }
                printf("\n");
            }

            /* display the device manufacturer and chip type - this requires
             * accessing the configuration information for the device as
             * shown below
             */

            /* get the device's configuration */
            pDeviceConfiguration = deviceList[i]->GetDeviceConfiguration();

            /* if failed to obtain device configuration */
            if (!pDeviceConfiguration)
            {
                /* report error */
                puts("Failed to obtain device's configuration!");
            }
            else
            {
                /* get the device's identification information */
                pDeviceIdentifierInfo =
                           pDeviceConfiguration->GetDeviceIdentificationInfo();

                /* if failed to obtain device identification info */
                if (!pDeviceIdentifierInfo)
                {
                    /* report error */
                    puts("Failed to obtain device identification info!");
                }
                else
                {
                    /* get the manufacturer and device type as a string */
                    pDeviceIdentifierInfo->GetDeviceManufacturerAsString(
                                                               manufacturerStr);
                    pDeviceIdentifierInfo->GetDeviceTypeAsString(deviceTypeStr);

                    /* display the device information */
                    printf(" Manufacturer ID: %d\t(%s)\n",
                           pDeviceIdentifierInfo->GetManufacturerID(),
                           manufacturerStr);
                    printf("       Chip Type: %d\t(%s)\n",
                            pDeviceIdentifierInfo->GetChipType(),
                            deviceTypeStr);

                    /* free the device identifier info */
                    delete pDeviceIdentifierInfo;
                    pDeviceIdentifierInfo = NULL;
                }

                /* free the device configuration */
                delete pDeviceConfiguration;
                pDeviceConfiguration = NULL;
            }

            /* get the Driver for the device */
            puts("Retrieving driver for device...");

            /* get the driver factory */
            pDriverFactory = STARSystem::GetDriverFactory();

            /* if driver factory obtained */
            if (pDriverFactory)
            {
                /* get the driver for the device */
                pDeviceDriver = deviceList[i]->GetDeviceDriver(pDriverFactory);

                /* free the driver factory */
                delete pDriverFactory;
            }
            else
            {
                /* cannot obtain driver for device, ensure driver object is
                 * NULL
                 */
                pDeviceDriver = NULL;
            }

            /* if driver obtained */
            if (pDeviceDriver)
            {
                /* get and display the driver ID */
                printf("Driver ID: %d\n", pDeviceDriver->GetDriverID());

                /* get and display the driver type */
                printf("Driver Type: %d\n", pDeviceDriver->GetDriverType());

                /* display whether the driver is virtual or not */
                printf("Driver virtual?: ");
                if (pDeviceDriver->IsDriverVirtual())
                    printf("Yes\n");
                else
                    printf("No\n");

                /* get the driver version */
                pDriverVersion = pDeviceDriver->GetDriverVersion();

                /* if driver version obtained */
                if (pDriverVersion)
                {
                    /* display the driver version */
                    printf("Driver version: v%d.%d", pDriverVersion->GetMajor(),
                        pDriverVersion->GetMinor());
                    if (pDriverVersion->GetEdit())
                    {
                        printf("(%d)", pDriverVersion->GetEdit());
                    }
                    if (pDriverVersion->GetPatch())
                    {
                        printf("p%d", pDriverVersion->GetPatch());
                    }
                    puts("");

                    /* free the driver version object */
                    delete pDriverVersion;
                    pDriverVersion = NULL;
                }
                else
                {
                    /* report failure to obtain driver version */
                    puts("Unable to get the driver version!");
                }

                /* free the driver object */
                delete pDeviceDriver;
                pDeviceDriver = NULL;
            }
            else
            {
                /* report failure to obtain driver for device */
                puts("Failed to obtain driver for device");
            }

            puts("\n");
        }

        /* dispose of the device list */
        /* \note This is not required, the DeviceList object destructor will
         * free the contents of the list if they haven't been disposed of
         * already.
         */
        deviceList.Empty();
    }
    else
    {
        /* report that no devices are present */
        puts("No devices present.\n");
    }

    puts("\n");
}

/**
 * Display information for all available STAR drivers.
 */
void StarSystemTest::DisplayDriverProperties()
{
    U32 driverCount;
    Driver **drivers = NULL;
    int i;
    STAR_DRIVER_ID id;
    Driver::DriverType type;
    VersionInformation *versionInfo;
    Device **pDevices = NULL;
    U32 deviceCount;
    char deviceName[Device::STR_DEVICE_NAME_LEN];
    char deviceType[Device::STR_DEVICE_TYPE_LEN];
    size_t stringLen;

    /* display section header */
    puts("\nDRIVER INFORMATION");
    puts("==================");

    /* obtain the list of available STAR drivers */
    drivers = STARSystem::GetDriverList(1, 1, &driverCount);

    /* if there are drivers present */
    if (drivers != NULL)
    {
        /* for each driver in the list */
        for (i = 0; i < (int)driverCount; i++)
        {
            /* get and display the driver ID */
            id = drivers[i]->GetDriverID();
            printf("            Driver ID: %d\n", id);

            /* get and display the driver type */
            type = drivers[i]->GetDriverType();
            printf("            Type: %d\n", type);

            /* display whether the driver is virtual or not */
            printf("            Virtual?: ");
            if (drivers[i]->IsDriverVirtual())
                printf("Yes\n");
            else
                printf("No\n");

            /* get the device's version */
            versionInfo = drivers[i]->GetDriverVersion();

            /* if no version found */
            if (versionInfo == NULL)
            {
                puts("Driver version: Unable to get the driver version!");
            }
            else
            {
                /* display the driver version */
                printf("Driver version: v%d.%d", versionInfo->GetMajor(),
                    versionInfo->GetMinor());
                if (versionInfo->GetEdit())
                {
                    printf("(%d)", versionInfo->GetEdit());
                }
                if (versionInfo->GetPatch())
                {
                    printf("p%d", versionInfo->GetPatch());
                }
                puts("");

                /* free the version information object obtained from the API */
                delete versionInfo;
                versionInfo = NULL;
            }

            /* get any devices which are present for this driver */
            puts("Getting devices present for this driver...");
            pDevices = drivers[i]->GetDeviceListForDriver(&deviceCount);

            /* if there are devices present */
            if (pDevices != NULL)
            {
                /* for each device in the list */
                for (i = 0; i < (int)deviceCount; i++)
                {
                    printf("Device %d:\n", i+1);

                    /* get and display the device's name */
                    stringLen = pDevices[i]->GetDeviceName(deviceName);
                    if (!stringLen)
                    {
                        puts("Name: Unable to get device name!");
                    }
                    else
                    {
                        printf("Name: %s\n", deviceName);
                    }

                    /* get and display the device's type */
                    stringLen = pDevices[i]->GetDeviceTypeAsString(deviceType);
                    if (!stringLen)
                    {
                        puts("Device type: Unable to get device type!");
                    }
                    else
                    {
                        printf("Device type: %s\n", deviceType);
                    }
                }

                /* dispose of the device list */
                if (!STARSystem::DestroyDeviceList(pDevices, deviceCount))
                {
                    puts("Failed to destroy device list!");
                }

                /* \note DestroyDeviceList() is just a helper function. You can
                 * free the device list yourself as per below:
                 *
                 *  for (int i=0; i<deviceCount; i++)
                 *  {
                 *      if (pDevices[i] != NULL)
                 *          delete pDevices[i];
                 *  }
                 *  delete [] pDevices;
                 *
                 */
            }
            else
            {
                /* no devices present for this driver */
                puts("No devices present for this driver");
            }
        }

        /* dispose of the driver list */
        if (!STARSystem::DestroyDriverList(drivers, driverCount))
        {
            puts("Failed to destroy driver list!");
        }

        /* \note DestroyDriverList() is just a helper function. You can
         * free the driver list yourself as per below:
         *
         *  for (int i=0; i<driverCount; i++)
         *  {
         *      if (drivers[i] != NULL)
         *          delete drivers[i];
         *  }
         *  delete [] drivers;
         *
         */
    }
    else
    {
        puts("No drivers present.\n");
    }

    puts("\n");
}



/**
 * Tests the SpaceWire device using a loop back test. Packets of random data
 * are transmitted on one link and received on a second link.
 *
 * This test is timed so as to test link speed. Optionally, the data is then
 * compared to check for errors.
 */
void StarSystemTest::LoopBack_SinglePacket(void)
{
    /* declare necessary variables for the test */
    Channel txChannel, rxChannel;
    char *pTxBuffer = NULL;
    ReceiveOperation *pReceiveOp = NULL;
    TransmitOperation *pTransmitOp = NULL;
    Packet *pTransmitPacket = NULL;
    unsigned long loopCount, errorCount = 0, byteSize, testNum;
    clock_t start, finish;
    int compare;
    TransferOperation::TransferStatus rxStatus, txStatus;
    Address *pTransmitPathAddress = NULL;

    /* select the transmit device and channel to be used */
    if (!ChooseDeviceAndChannel("transmit", txChannel,
        Channel::DIRECTION_OUT))
    {
        /* end processing if no valid channel obtained */
        return;
    }

    /* Select the (optional) path to be added to the front of packets
     * being transmitted
     */
    pTransmitPathAddress = GetTransmitPathAddress();

    /* select the receive device and channel to be used */
    if (!ChooseDeviceAndChannel("receive", rxChannel,
        Channel::DIRECTION_IN))
    {
        /* jump to end of processing if no valid channel obtained */
        goto LoopBack_SinglePacket_Finish;
    }

    /* get the size of packet to use for the test */
    if (!GetPacketSize(&byteSize))
    {
        /* jump to end of processing if no valid packet size obtained */
        goto LoopBack_SinglePacket_Finish;
    }

    /* get number of times to run the test */
    if (!GetLoopCount(&loopCount))
    {
        /* jump to end of processing if no valid loop count obtained */
        goto LoopBack_SinglePacket_Finish;
    }

    /* check if the user would like to compare / check the validity of
     * the packet data received
     */
    if (!ChooseCheckData(&compare))
    {
        /* jump to end of processing if failed to obtain compare status */
        goto LoopBack_SinglePacket_Finish;
    }

    /* allocate memory for the transmit buffer */
    pTxBuffer = (char *)calloc(1, byteSize);

    /* if failed to allocate memory */
    if (!pTxBuffer)
    {
        /* report error and jump to end of processing */
        puts("\nERROR: Unable to allocate memory for transmit buffer");
        goto LoopBack_SinglePacket_Finish;
    }

    /* fill the transmit buffer with random data */
    FillBufferRandomChar(pTxBuffer, byteSize, DATA_TYPE_RANDOM);

    /* create a new receive operation to receive 1 packet */
    pReceiveOp = CreateReceiveOperation(1, ReceiveOperation::RECEIVE_PACKETS);

    /* if failed to create receive operation */
    if (pReceiveOp == NULL)
    {
        /* report error and jump to end of processing */
        puts("\nERROR: Unable to create receive operation");
        goto LoopBack_SinglePacket_Finish;
    }

    /* Create the packet to be transmitted */
    pTransmitPacket = CreatePacket(pTransmitPathAddress, (U8 *)pTxBuffer,
                                   byteSize, Packet::EOP_TYPE_EOP);

    /* if failed to create packet */
    if (pTransmitPacket == NULL)
    {
        /* report error and jump to end of processing */
        puts("\nERROR: Unable to create the packet to be transmitted");
        goto LoopBack_SinglePacket_Finish;
    }

    /* create a transmit operation for the packet */
    pTransmitOp = CreateTransmitOperation((StreamItem **)&pTransmitPacket, 1);

    /* if failed to create transmit op */
    if (!pTransmitOp)
    {
        /* report error and jump to end of processing */
        puts("\nERROR: Unable to create the transfer operation to be transmitted");
        goto LoopBack_SinglePacket_Finish;
    }

    /* display start of test header */
    puts("Running Single-Packet Loopback Test...");

    /* get the start time for the test */
    start = GET_TIME();

    /* for the number of tests required */
    for(testNum = 0; testNum < loopCount; testNum++)
    {
        /* attempt to submit the receive operation, ready to receive packets
         * on the receive channel specified
         */
        if (!rxChannel.SubmitTransferOperation(pReceiveOp))
        {
            /* report any errors submitting the receive */
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1);

            /* increment error count */
            errorCount++;

            /* jump to end of processing */
            goto LoopBack_SinglePacket_Finish;
        }

        /* attempt to submit the transmit operation, ready to transmit
         * packets on the transmit channel specified
         */
        if (!txChannel.SubmitTransferOperation(pTransmitOp))
        {
            /* report any errors submitting the transmit */
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1);

            /* increment error count */
            errorCount++;

            /* jump to end of processing */
            goto LoopBack_SinglePacket_Finish;
        }

        /* wait (indefinitely) on the transmit operation completing */
        txStatus = pTransmitOp->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

        /* if the transmit operation failed to complete */
        if(txStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
        {
            /* report error, increment error count and goto end of process */
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1);
            errorCount++;
            goto LoopBack_SinglePacket_Finish;
        }

        /* wait (indefinitely) on the receive operation completing */
        rxStatus = pReceiveOp->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

        /* if the receive operation failed to complete */
        if (rxStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
        {
            /* report error, increment error count and goto end of process */
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1);
            errorCount++;
            goto LoopBack_SinglePacket_Finish;
        }

        /* if the received packet is to be compared */
        if (compare)
        {
            /* compare the received packet to the buffer transmitted */
            errorCount += ComparePackets(pReceiveOp, 1, byteSize, pTxBuffer);
        }
    }

    /* get the end time of the test */
    finish = GET_TIME();

    /* display the test results */
    DisplayResults(start, finish, compare, byteSize, 1, loopCount, errorCount,
        "Single-Packet Loopback Test");

/* marker for clean-up steps */
LoopBack_SinglePacket_Finish:

    /* dispose of the transfer operations */
    if (pTransmitOp != NULL)
    {
        delete pTransmitOp;
    }
    if (pReceiveOp != NULL)
    {
        delete pReceiveOp;
    }

    /* destroy the packet transmitted */
    if (pTransmitPacket != NULL)
    {
        delete pTransmitPacket;
    }

    /* destroy the transmit path address, if any */
    if (pTransmitPathAddress != NULL)
    {
        delete pTransmitPathAddress;
    }

    /* free the transmit buffer */
    if (pTxBuffer != NULL)
    {
        free(pTxBuffer);
    }

    /* close the channels */
    rxChannel.CloseChannel();
    txChannel.CloseChannel();
}

/**
 * Prompts the user to choose which device and channel to use for the test,
 * if multiple devices and/or channels are available.
 *
 * @param descriptionStr the description of what the device is to be used for
 *                       (e.g. transmit or receive)
 * @param selectedChannel A reference to a Channel object which will be set
 *                        with information for the selected, opened channel.
 * @param direction the direction in which traffic is to be sent over the
 *                  channel
 *
 * @return whether a device and a channel was successfully selected
 */
int StarSystemTest::ChooseDeviceAndChannel(const char *descriptionStr,
                                  Channel& selectedChannel,
                                  Channel::ChannelDirection direction)
{
    Device *pDevice = NULL;
    unsigned char channelNumber = 0;
    Channel ch;

    /* get the device to choose a channel for */
    pDevice = ChooseDevice(descriptionStr);

    /* if no device found */
    if (pDevice == NULL)
    {
        /* return error / no channel */
        return 0;
    }

    /* get the channel to use from the device  */
    channelNumber = ChooseChannel(descriptionStr, pDevice);

    /* if no channel selected */
    if (!channelNumber)
    {
        /* free any resources created, e.g. Device object */
        delete pDevice;

        /* return error / no channel */
        return 0;
    }

    /* open the channel on the device */
    ch = pDevice->OpenChannel(direction, channelNumber, TRUE);

    /* if channel failed to open */
    if (!ch.IsOpen())
    {
        /* free any resources created, e.g. Device object */
        delete pDevice;

        /* report error */
        printf("\nERROR: Unable to open %s channel\n", descriptionStr);

        /* return error */
        return 0;
    }

    /* write blank line */
    puts("");

    /* free any resources created, e.g. Device object */
    delete pDevice;

    /* copy the channel selected into the Channel object provided */
    selectedChannel = ch;

    /* Alternatively: copy the ID of the opened channel into the Channel
     * object provided:
     *
     * selectedChannel.SetChannelID(ch.GetChannelID());
     */

    /* return success */
    return 1;
}

/**
 * Prompts the user to choose which device to use for the test if more than
 * one device is available.
 *
 * @param descriptionStr the description of what the device is to be used for
 *                       (e.g. transmit or receive)
 *
 * @return A pointer to a Device object representing the selected
 *         device, or NULL if no devices are available or an error occurred
 *         during processing.
 * 
 * \note The object returned must be freed when no longer required by using
 *       the C++ delete operator.
 */
Device* StarSystemTest::ChooseDevice(const char *descriptionStr)
{
    char deviceName[Device::STR_DEVICE_NAME_LEN];
    size_t stringLen = 0;
    U32 deviceCount = 0;
    DeviceList deviceList;
    Device *deviceSelected = NULL;
    int status;
    U32 i;
    unsigned int chosen;
    char s[256];

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
        printf("One %s device detected:", descriptionStr);
    }
    else
    {
        printf("%d %s devices detected:\n", deviceCount, descriptionStr);
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
        printf("Please select which %s device to use: ", descriptionStr);

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

/**
 * Prompts the user to choose which channel on a specified device to use for
 * the test if more than one channel is available.
 *
 * @param descriptionStr the description of what the device is to be used for
 *                       (e.g. transmit or receive)
 * @param device A Device from which the channel will be selected.
 *
 * @return the number of the channel to be used for the test, or 0 if there
 *         was an error or no channels are present on the device
 */
unsigned char StarSystemTest::ChooseChannel(const char *descriptionStr,
                                            Device* device)
{
    int status, i;
    char s[256];
    STAR_CHANNEL_MASK channelMask;
    unsigned int channelCount = 0;
    unsigned int channelNumber = 0;

    /* get the channels present on the device */
    channelMask = device->GetDeviceChannels();

    /* if no valid channels */
    if (channelMask == 0 || channelMask == 1)
    {
        /* display error */
        printf("ERROR: The %s device doesn't appear to have any valid "
                "channels.", descriptionStr);

        /* return no channels available */
        return 0;
    }

    /* determine the number of channels on the device */
    for (i = 1; i < 32; i++)
    {
        if ((channelMask >> i) & 1)
        {
            channelCount++;
            channelNumber = (unsigned int)i;
        }
    }

    /* if there's only one channel on the device, use this channel */
    if (channelCount == 1)
    {
        printf("Using channel %u as the %s channel, as this is the only "
                "channel on the device.", channelNumber, descriptionStr);
        return (unsigned char)channelNumber;
    }

    /* display the available channels, if more than one to select from */
    printf("Enter %s channel (", descriptionStr);
    for (i = 1; i < 32; i++)
    {
        /* if the channel exists */
        if ((channelMask >> i) & 1)
        {
            printf("%d", i);
            channelCount--;
            if (channelCount == 1)
            {
                printf(" or ");
            }
            else if (channelCount > 1)
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

    /* read in the channel to use */
    if (!fgets(s, 256, stdin))
    {
        puts("\nERROR: No channel specified");
        return 0;
    }
    status = sscanf(s, "%u", &channelNumber);
    if ((!status) || (!((1 << (int)channelNumber) & (int)channelMask)))
    {
        puts("\nERROR: The channel specified is not present");
        return 0;
    }

    /* return the channel number selected */
    return (unsigned char)channelNumber;
}

/**
 * Prompts the user to choose whether to check the received data.
 *
 * @param pCompare a pointer to a variable which will be updated to indicate
 *                 whether the data should be compared
 *
 * @return whether the user selected a valid option
 */
int StarSystemTest::ChooseCheckData(int *pCompare)
{
    char s[256];

    printf("Would you like to check received data for errors? (y/n): ");
    if (!fgets(s, 256, stdin))
    {
        puts("ERROR: No value selected");
        return 0;
    }
    if ((s[0] == 'y') || (s[0] == 'Y') || (s[0] == 't') || (s[0] == 'T') ||
        (s[0] == '1'))
    {
        *pCompare = 1;
    }
    else if ((s[0] == 'n') || (s[0] == 'N') || (s[0] == 'f') || (s[0] == 'F') ||
        (s[0] == '0'))
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
 * Prompts the user to specify the packet size to be used for a test.
 *
 * @param pPacketSize a pointer to a variable which will be updated to
 *                    contain the packet size specified
 *
 * @return whether the user selected a valid packet size
 */
int StarSystemTest::GetPacketSize(unsigned long *pPacketSize)
{
    char s[256];
    int status;

    printf("Enter buffer size for each packet in bytes: ");
    if (!fgets(s, 256, stdin))
    {
        puts("\nERROR: No buffer size specified");
        return 0;
    }
    status = sscanf(s, "%lu", pPacketSize);
    if ((!status) || (!(*pPacketSize)))
    {
        puts("\nERROR: Invalid buffer size specified");
        return 0;
    }

    return 1;
}

/**
 * Prompts the user to specify the number of times to run a test.
 *
 * @param pLoopCount a pointer to a variable which will be updated to contain
 *                    the specified value for the number of times the test
 *                    should be run
 *
 * @return whether the user selected a valid number of loops
 */
int StarSystemTest::GetLoopCount(unsigned long *pLoopCount)
{
    char s[256];
    int status;

    printf("Enter the number of times to run the test: ");
    if (!fgets(s, 256, stdin))
    {
        puts("\nERROR: No loop count specified");
        return 0;
    }
    status = sscanf(s, "%lu", pLoopCount);
    if ((!status) || (!(*pLoopCount)))
    {
        puts("\nERROR: Invalid loop count specified");
        return 0;
    }

    return 1;
}

/**
 * Prompts the user to specify the number of packets to transmit and receive in
 * a test.
 *
 * @param pPacketCount a pointer to a variable which will be updated to contain
 *                     the specified value for the number of packets to 
 *                     transmit and receive in the test
 *
 * @return whether the user selected a valid number of packets
 */
int StarSystemTest::GetPacketCount(unsigned long *pPacketCount)
{
    char s[256];
    int status;

    printf("Enter the number of packets to transmit/receive in the test: ");
    if (!fgets(s, 256, stdin))
    {
        puts("\nERROR: No packet count specified");
        return 0;
    }
    status = sscanf(s, "%lu", pPacketCount);
    if ((!status) || (!(*pPacketCount)))
    {
        puts("\nERROR: Invalid packet count specified");
        return 0;
    }

    return 1;
}

/**
 * Fills a buffer with different random data values.
 *
 * @param pBuffer A pointer to the buffer to fill.
 * @param size The length of the buffer.
 * @param dataType The type of data to fill the buffer with, e.g. zeros,
 *                 ones, random chars, etc.
 */
void StarSystemTest::FillBufferRandomChar(char *pBuffer, unsigned int size,
                                          int dataType)
{
    unsigned int n;
    switch (dataType)
    {
    case DATA_TYPE_0:           /* fill with zeros */
        memset(pBuffer, 0, size);
        break;

    case DATA_TYPE_1:           /* fill with ones */
        memset(pBuffer, 1, size);
        break;

    case DATA_TYPE_RANDOM:      /* fill with random values */
        for (n = 0; n < size; n++)
        {
            *(pBuffer + n) = (char) random32();
        }
        break;

    case DATA_TYPE_COUNT:       /* fill with simple count */
        for (n = 0; n < size; n++)
        {
            *(pBuffer + n) = (char)n;
        }
        break;

    case DATA_TYPE_NOT_COUNT:   /* fill with not of simple count */
        for (n = 0; n < size; n++)
        {
            *(pBuffer + n) = 0xFF - (char)n;
        }
        break;
    }
}

/**
 * Generates a 32-bit random number.
 *
 * @return A random, 32-bit number.
 */
unsigned int StarSystemTest::random32(void)
{
    unsigned int  value;

    value = random16();
    value *= 0x10000;
    value += random16();

    return value;
}

/**
 * Generates a 16-bit random number.
 *
 * @return A random, 16-bit number.
 */
unsigned int StarSystemTest::random16(void)
{
    unsigned int  mid_rand = (unsigned int)RAND_MAX/2;
    unsigned int  value = (unsigned int)rand();

    if ((unsigned int )rand() > mid_rand)
    {
        value += RAND_MAX;
    }

    return value;
}

/**
 * Create and return a pointer to a new ReceiveOperation.
 *
 * @param itemCount The maximum number of stream items to receive. This can
 *                  be -1 to receive an unlimited number of items. The size
 *                  of an individual stream item is not limited
 * \note If a receive operation receiving an unlimited number of items is not
 *       cancelled by the user, then the operation will eventually fail to
 *       allocate new memory for new incoming data, failing the operation.
 * @param mask A bitmask with flags set for the type of traffic one wishes to
 *             receive.
 *
 * @return A pointer to a new ReceiveOperation object, or NULL if failed to
 *         create and/or allocate memory for the receive operation.
 */
ReceiveOperation *StarSystemTest::CreateReceiveOperation(int itemCount,
                                          ReceiveOperation::ReceiveMask mask)
{
    /* declare NULL pointer to a new ReceiveOperation */
    ReceiveOperation *pReceiveOperation = NULL;

    /* try to create a new receive operation providing its constructor
     * with the necessary parameters
     */
    try
    {
        pReceiveOperation = new ReceiveOperation(itemCount, mask);
    }
    catch (std::bad_alloc& ba)
    {
        /* failed to allocate memory */
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
        pReceiveOperation = NULL;
    }
    catch (TransferOperationException& tOpEx)
    {
        /* failed to create Receive Operation */
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(tOpEx);
        pReceiveOperation = NULL;
    }

    /* return the new Receive Operation, or NULL */
    return pReceiveOperation;
}



/**
 * Create and return a pointer to a new Packet.
 *
 * @param[in] pAddress Optional pointer to a SpaceWire address.
 * @param[in] pData Optional pointer to data buffer.
 * \note The contents of this buffer are copied into data structures managed
 *       by the API. It is safe to dispose of this buffer after this function
 *       completes.
 * @param dataLen The length of the data buffer.
 * @param eopType End of packet marker type for the packet (may be none)
 *
 * @return A pointer to a new Packet object, or NULL if failed to create packet
 *         or allocate memory for it.
 */
Packet *StarSystemTest::CreatePacket(Address *pAddress,
                                        unsigned char *pData,
                                        unsigned int dataLen,
                                        Packet::EopType eopType)
{
    /* declare NULL pointer to a new Packet */
    Packet *pSpaceWirePacket = NULL;

    /* try to create a new Packet object, providing its constructor with
     * the necessary parameters
     */
    try
    {
        pSpaceWirePacket = new Packet((Address*)pAddress,
                                                pData, dataLen, eopType);
    }
    catch (std::bad_alloc& ba)
    {
        /* failed to allocate memory */
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
        pSpaceWirePacket = NULL;
    }
    catch (PacketException& spPackEx)
    {
        /* failed to create Packet */
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(spPackEx);
        pSpaceWirePacket = NULL;
    }

    /* return the new Packet, or NULL */
    return pSpaceWirePacket;
}

/**
 * Compare the contents of received traffic against a previously created buffer.
 *
 * @param pReceiveOp The completed receive operation containing the packets to
 *                   look at.
 * @param packetCount The number of packets which should be in the received
 *                    operation.
 * @param packetSize The size of each packet which should be received.
 * @param pBuffer The buffer that the received packets should be compared
 *                against.
 *
 * @return The number of errors in the received packets.
 */
unsigned long StarSystemTest::ComparePackets(ReceiveOperation* pReceiveOp,
                                             unsigned long packetCount,
                                             unsigned long packetSize,
                                             char *pBuffer)
{
    unsigned long errorCount = 0;
    int rxPacketCount = 0, i = 0;
    StreamItem *pRxStreamItem = NULL;
    Packet *pPacket = NULL;
    char *pRxBuffer = NULL;
    U32 rxPacketLength = 0;

    /* get the number of stream items received */
    rxPacketCount = (int)pReceiveOp->GetTransferItemCount();

    /* if number of items received isn't as expected */
    if ((unsigned)rxPacketCount != packetCount)
    {
        /* report and record error */
        printf("\nERROR expected to receive %lu packets but received %d.",
            packetCount, rxPacketCount);
        errorCount++;
    }
    else
    {
        /* for each stream item received */
        for (i = 0; i < rxPacketCount; i++)
        {
            /* get the next stream item */
            pRxStreamItem = pReceiveOp->GetTransferItem((unsigned int)i);

            /* if stream item isn't valid or isn't a packet */
            if ((pRxStreamItem == NULL) ||
                (pRxStreamItem->GetStreamItemType() !=
                StreamItem::STREAM_ITEM_TYPE_PACKET))
            {
                /* report and record error */
                printf("\nERROR received an unexpected traffic type, or "
                       "empty traffic item in item %d", i);
                errorCount++;
            }
            else
            {
                /* cast stream item to packet */
                pPacket = (Packet*)pRxStreamItem;

                /* get the packet's data */
                pRxBuffer = (char *)pPacket->GetPacketData(&rxPacketLength);

                /* if no data received or packet length isn't as expected */
                if ((!pRxBuffer) || (rxPacketLength != packetSize))
                {
                    /* report and record error */
                    printf("\nERROR received a packet of length %d, expected "
                           "length %lu in item %d", rxPacketLength, packetSize,
                           i);
                    errorCount++;
                }
                else
                {
                    /* compare the buffers and increment the error count if 
                     * the buffers do not match
                     */
                    errorCount = errorCount + BufferCompareChar(
                        pBuffer + packetSize * (unsigned long)i, pRxBuffer, 
                        packetSize);
                }

                /* free memory that was provided containing the packet data */
                pPacket->DestroyPacketData((unsigned char *)pRxBuffer);
            }

            /* free the packet stream item obtained */
            delete pRxStreamItem;
        }
    }

    /* Return the error count */
    return errorCount;
}

/**
 * Buffer compare function: Used instead of memcmp() for debugging.
 *
 * @param pBuffer1 Pointer to buffer1, to be compared with buffer2.
 * @param pBuffer2 Pointer to buffer2, to be compared with buffer1.
 * @param size The size or extent of the buffers to compare.
 *
 * @return The number of errors found during the compare.
 */
unsigned long StarSystemTest::BufferCompareChar(char *pBuffer1, char *pBuffer2,
                                unsigned long size)
{
    unsigned int errorCount = 0;
    unsigned long i;

    /* for the required buffer length / extent */
    for (i = 0; i < size; i++)
    {
        /* if the next element in buffer 1 isn't equal to that in the
         * equivalent position in buffer 2
         */
        if (*(pBuffer1 + i) != *(pBuffer2 + i))
        {
            /* report and record error */
            errorCount++;
            printf("Error in byte %8lu, should be 0x%2x actually 0x%2x\n", i,
                *(pBuffer1 + i), *(pBuffer2 + i));
        }
    }

    /* return the number of errors found */
    return errorCount;
}

/**
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
void StarSystemTest::DisplayResults(clock_t start, clock_t finish, int compared,
    unsigned long byteSize, unsigned long packetCount, unsigned long loopCount,
    unsigned long errorCount, const char *descriptionStr)
{
    double bitsSent, duration;

    puts("Test complete.");

    /* calculate the time taken and the number of bits sent */
    duration = (double)(finish - start) / TIME_DIVIDER;
    bitsSent = (double)byteSize * (double)8 * (double)packetCount *
        (double)loopCount;

    /* display the data rates */
    printf("\n**** %s, Results **** \n", descriptionStr);
    printf("\tTime Taken = %-9.5g Seconds\n", duration);
    printf("\tAverage Speed = %-9.2E bit/s (%-3.2f Mbit/s)\n\n",
        bitsSent / duration, bitsSent / duration / 1000000);

    /* display the number of errors encountered */
    if(compared)
    {
        printf("Total Errors = 0x%-7lx \n", errorCount);
    }

    /* display whether the test was successful */
    if (!errorCount)
    {
        puts("Test successful.\n");
    }
    else
    {
        puts("Test failed.\n");
    }
}

/**
 * Prompts the user to specify the path address to be added to the front of the
 * packets to be transmitted for a test.
 *
 * @return A pointer to a new Address object containing the path
 *         specified or NULL if a error occurred.
 *
 * \note The object returned must be freed when no longer required by using
 *       the C++ delete operator.
 */
Address* StarSystemTest::GetTransmitPathAddress()
{
    char s[256];
    Address *pAddress = NULL;
    unsigned char newPath[256];
    U16 pathLen = 0;
    char *pos = (char *)s;
    unsigned long value;

    /* ask the user to enter the path to add to the front of packets */
    puts("Enter an optional path to add to the front of the packets to be sent:");
    puts("(Values should be in hex, separated by a space, i.e.: 01 0f 02)");

    /* read in the path */
    if (!fgets(s, 256, stdin))
    {
        puts("No address entered");
        return NULL;
    }

    /* if empty string */
    if (strlen(s) == 0)
    {
        puts("No address entered");
        return NULL;
    }

    /* read until end of buffer or line feed */
    while ((*pos) && (pathLen < 256) && (*pos != '\n'))
    {
        errno = 0;
        value = strtoul(pos, &pos, 16);
        if (errno)
        {
            puts("Invalid value entered in address");
            return NULL;
        }

        newPath[pathLen] = (unsigned char)value;
        pathLen++;
    }

    /* attempt to create a new Address from the path specified */
    try
    {
        if (pathLen > 0)
        {
            pAddress = new Address(newPath, pathLen);
        }
    }
    catch (std::bad_alloc& ba)
    {
        /* memory allocation error */
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
        puts("ERROR: Could not create address");
        pAddress = NULL;
    }
    catch (AddressException& spWAE)
    {
        /* failed to create SpaceWire address */
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(spWAE);
        puts("ERROR: Could not create address");
        pAddress = NULL;
    }

    /* return the address (or NULL if error occurred or no path address) */
    return pAddress;
}

/**
 * Performs a loopback test with multiple packets.  Multiple packets of random
 * data are transmitted out of one link (using a  single transfer operation) and
 * received at a second link (again using a single transfer operation).
 *
 * This test is timed so as to test link speed. Optionally, the data is then
 * compared to check for errors.
 */
void StarSystemTest::LoopBack_MultiPacket()
{
    /* declare necessary variables for the test */
    Channel rxChannel, txChannel;
    unsigned long loopCount, errorCount = 0, byteSize, packetCount = 0;
    int compare, i, testNum;
    char *pTxBuffer = NULL;
    StreamItem **ppTransmitStreamItems = NULL;
    ReceiveOperation *pReceiveOp = NULL;
    TransmitOperation *pTransmitOp = NULL;
    TransferOperation::TransferStatus rxStatus, txStatus;
    clock_t start, finish;
    Address *pTransmitPathAddress = NULL;
    bool packetCreationFail;

    /* select the transmit device and channel to be used */
    if (!ChooseDeviceAndChannel("transmit", txChannel,
        Channel::DIRECTION_OUT))
    {
        /* end processing if no valid channel obtained */
        return;
    }

    /* select the (optional) path to be added to the front of packets
     * being transmitted
     */
    pTransmitPathAddress = GetTransmitPathAddress();

    /* select the receive device and channel to be used */
    if (!ChooseDeviceAndChannel("receive", rxChannel,
        Channel::DIRECTION_IN))
    {
        /* jump to end of processing if no valid channel obtained */
        goto LoopBack_MultiPacket_Finish;
    }

    /* get the size of packet to use for the test */
    if (!GetPacketSize(&byteSize))
    {
        /* jump to end of processing if no valid packet size obtained */
        goto LoopBack_MultiPacket_Finish;
    }

    /* get number of packets to send */
    if (!GetPacketCount(&packetCount))
    {
        /* jump to end of processing if no valid packet count obtained */
        goto LoopBack_MultiPacket_Finish;
    }

    /* get number of times to perform test */
    if (!GetLoopCount(&loopCount))
    {
        /* jump to end of processing if no valid test count obtained */
        goto LoopBack_MultiPacket_Finish;
    }

    /* check if the user would like to compare / check validity of the
     * packets sent and received */
    if (!ChooseCheckData(&compare))
    {
        /* jump to end of processing if failed to obtain compare status */
        goto LoopBack_MultiPacket_Finish;
    }

    /* allocate memory for the transmit buffer */
    pTxBuffer = (char *)calloc(1, byteSize * packetCount);

    /* if failed to allocate memory */
    if (!pTxBuffer)
    {
        /* report error, jump to end of processing */
        puts("\nERROR: Unable to allocate memory for transmit buffer");
        goto LoopBack_MultiPacket_Finish;
    }

    /* fill transmit buffer with random data */
    FillBufferRandomChar(pTxBuffer, byteSize * packetCount, DATA_TYPE_RANDOM);

    /* create a new receive operation to receive the packets */
    pReceiveOp = CreateReceiveOperation((int)packetCount,
                                        ReceiveOperation::RECEIVE_PACKETS);

    /* if failed to create receive op */
    if (pReceiveOp == NULL)
    {
        /* report error, jump to end of processing */
        puts("\nERROR: Unable to create receive operation");
        goto LoopBack_MultiPacket_Finish;
    }

    /* create array of pointers to stream items, to hold the packets which
     * will be transmitted
     */
    ppTransmitStreamItems = CreateArrayOfPointersToStreamItems((int)packetCount);

    /* if failed to allocate membory for array of stream items / packets */
    if (ppTransmitStreamItems == NULL)
    {
        /* report error, jump to end of processing */
        puts("\nERROR: Unable to allocate memory for transmit stream items");
        goto LoopBack_MultiPacket_Finish;
    }

    /* record if any packet creation fails occur in next stage */
    packetCreationFail = false;

    /* for the number of packets being sent */
    for (i = 0; i < (int)packetCount; i++)
    {
        /* create a new packet stream item */
        Packet *pPacket = NULL;
        pPacket = CreatePacket(pTransmitPathAddress,
                               (U8 *)pTxBuffer + (byteSize * (unsigned long)i),
                               (unsigned int)byteSize,
                               Packet::EOP_TYPE_EOP);

        /* add this to the array of pointers to stream items */
        ppTransmitStreamItems[i] = pPacket;

        /* if packet created is NULL (i.e. failed to create) */
        if (pPacket == NULL)
        {
            /* record a packet creation fail */
            packetCreationFail = true;

            /* report error */
            printf("\nERROR: Unable to create packet %d to be transmitted", i);

            /* complete processing early */
            break;
        }
    }

    /* if a packet creation failed during the previous stage */
    if (packetCreationFail)
    {
        /* jump to end of processing */
        goto LoopBack_MultiPacket_Finish;
    }

    /* create the transmit transfer operation for the packets */
    pTransmitOp = CreateTransmitOperation(ppTransmitStreamItems, (int)packetCount);

    /* if transmit operation failed to create */
    if (pTransmitOp == NULL)
    {
        /* report error, jump to end of processing */
        puts("\nERROR: Unable to create the transfer operation to be transmitted");
        goto LoopBack_MultiPacket_Finish;
    }

    /* display header for test results */
    puts("Running Multi-Packet Loopback Test...");

    /* get the start time for the test */
    start = GET_TIME();

    /* for the number of tests required */
    for(testNum = 0; testNum < (int)loopCount; testNum++)
    {
        /* submit the receive operation on the receive channel specified. */
        if (!rxChannel.SubmitTransferOperation(pReceiveOp))
        {
            /* report errors, increment error count, jump to end of process */
            printf("\nERROR occurred during receive.  Test %d failed.\n",
                testNum + 1);
            errorCount++;
            goto LoopBack_MultiPacket_Finish;
        }

        /* submit the transmit operation on the transmit channel specified. */
        if (!txChannel.SubmitTransferOperation(pTransmitOp))
        {
            /* report errors, increment error count, jump to end of process */
            printf("\nERROR occurred during transmit.  Test %d failed.\n",
                testNum + 1);
            errorCount++;
            goto LoopBack_MultiPacket_Finish;
        }

        /* wait (indefinitely) on the transmit operation completing */
        txStatus = pTransmitOp->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

        /* if the transmit operation failed to complete */
        if(txStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
        {
            /* report errors, increment error count, jump to end of process */
            printf("\nERROR occurred during transmit.  Test %d failed.\n",
                testNum + 1);
            errorCount++;
            goto LoopBack_MultiPacket_Finish;
        }

        /* wait (indefinitely) on the receive operation completing */
        rxStatus = pReceiveOp->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

        /* if the receive operation failed to complete */
        if (rxStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
        {
            /* report errors, increment error count, jump to end of process */
            printf("\nERROR occurred during receive.  Test %d failed.\n",
                testNum + 1);
            errorCount++;
            goto LoopBack_MultiPacket_Finish;
        }

        /* if the user specified that the received packets should be compared */
        if (compare)
        {
            /* compare the received packets to the buffer transmitted */
            errorCount += ComparePackets(pReceiveOp, packetCount, byteSize,
                                            pTxBuffer);
        }
    }

    /* get end time of test */
    finish = GET_TIME();

    /* display the test results */
    DisplayResults(start, finish, compare, byteSize, packetCount, loopCount,
        errorCount, "Multiple-Packet Loopback Test");

/* marker for clean-up stepss processing */
LoopBack_MultiPacket_Finish:

    /* dispose of the transfer operations */
    if (pTransmitOp != NULL)
    {
        delete pTransmitOp;
    }
    if (pReceiveOp != NULL)
    {
        delete pReceiveOp;
    }

    /* destroy the array of pointers to the packets / stream items which
     * were transmitted
     */
    if (ppTransmitStreamItems)
    {
        /* for each stream item pointer in the array */
        for (unsigned long i = 0; i < packetCount; i++)
        {
            /* if the item pointed to is valid (non-NULL) */
            if (ppTransmitStreamItems[i] != NULL)
            {
                /* delete the item pointed to */
                delete ppTransmitStreamItems[i];
            }
        }

        /* delete the array itself */
        delete [] ppTransmitStreamItems;
    }

    /* destroy the transmit path address, if any */
    if (pTransmitPathAddress != NULL)
    {
        delete pTransmitPathAddress;
    }

    /* free the transmit buffer */
    if (pTxBuffer != NULL)
    {
        free(pTxBuffer);
    }

    /* close the opened channels */
    rxChannel.CloseChannel();
    txChannel.CloseChannel();
}

/**
 * Create and return an array of pointers to stream items, of the specified
 * length.
 *
 * @param len The length of the array to create.
 *
 * @return An array of pointers to StreamItems, or NULL.
 */
StreamItem** StarSystemTest::CreateArrayOfPointersToStreamItems(int len)
{
    /* declare NULL pointer to array to be created */
    StreamItem **pArrayOfPointersToStreamItems = NULL;

    /* try to allocate memory for the array */
    try
    {
        pArrayOfPointersToStreamItems = new StreamItem*[len];

        /* initialise pointers to NULL */
        for (int i=0; i<len; i++)
            pArrayOfPointersToStreamItems[i] = NULL;
    }
    catch (std::bad_alloc& ba)
    {
        /* error allocating, ensure array is set to NULL */
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
        pArrayOfPointersToStreamItems = NULL;
    }

    /* return the array */
    return pArrayOfPointersToStreamItems;
}



/**
 * Create and return a pointer to a new TransmitOperation.
 *
 * @param ppStreamItems An array of pointers to StreamItem objects to be sent.
 * \note It is safe to free this array after the function is completed.
 * @param streamItemCount The number of StreamItems in the array.
 *
 * @return A pointer to a new TransmitOperation object, or NULL.
 */
TransmitOperation* StarSystemTest::CreateTransmitOperation(
                                                    StreamItem** ppStreamItems,
                                                    int streamItemCount)
{
    /* declare NULL pointer to a new TransmitOperation */
    TransmitOperation *pTransmitOperation = NULL;

    /* try to create a new transmit operation */
    try
    {
        pTransmitOperation = new TransmitOperation(ppStreamItems,
                                                (unsigned int)streamItemCount);
    }
    catch (std::bad_alloc& ba)
    {
        /* failed to allocate memory */
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
        pTransmitOperation = NULL;
    }
    catch (TransferOperationException& tOpEx)
    {
        /* failed to create Transmit Operation */
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(tOpEx);
        pTransmitOperation = NULL;
    }

    /* return the new Transmit Operation, or NULL */
    return pTransmitOperation;
}

/**
 * Performs a Double loopback test with single packets. Single packets of
 * random data are transmitted out of two links simultaneously (using a single
 * transfer operation per link) and received at either end simultaneously 
 * (again using a single transfer operation per link).
 *
 * This test is timed so as to test link speed. Optionally, the data is then
 * compared to check for errors.
 *
 * This method makes use of multiple transfer operation submission.
 */
void StarSystemTest::LoopBackDouble_SinglePacket()
{
    /* declare necessary variables for the test */
    Channel channels[2];
    unsigned long loopCount, errorCount = 0, byteSize;
    int compare;
    char *pTxBuffer[2] = { NULL, NULL };
    TransmitOperation *pTransmitOp[2] = { NULL, NULL };
    ReceiveOperation *pReceiveOp[2] = { NULL, NULL };
    Packet *pSpaceWirePacket[2] = { NULL, NULL };
    TransferOperation *pTransferOperationList[2][2] = { {NULL} };
    TransferOperation::TransferStatus rxStatus, txStatus;
    clock_t start, finish;
    Address *pTransmitAddressPaths[2] = { NULL, NULL };

    /* select the first device and channel to be used */
    if (!ChooseDeviceAndChannel("first", channels[0],
        Channel::DIRECTION_INOUT))
    {
        /* end processing if no valid channel obtained */
        return;
    }

    /* Select the first (optional) path to be added to the front of packets
     * being transmitted */
    pTransmitAddressPaths[0] = GetTransmitPathAddress();

    /* select the second device and channel to be used */
    if (!ChooseDeviceAndChannel("second", channels[1],
        Channel::DIRECTION_INOUT))
    {
        /* jump to end of process if no valid channel obtained */
        goto LoopBackDouble_SinglePacket_Finish;
    }

    /* Select the second (optional) path to be added to the front of packets
     * being transmitted
     */
    pTransmitAddressPaths[1] = GetTransmitPathAddress();

    /* get size of packet to use for test */
    if (!GetPacketSize(&byteSize))
    {
        /* jump to end of process if no valid packet size obtained */
        goto LoopBackDouble_SinglePacket_Finish;
    }

    /* get number of times to run the test */
    if (!GetLoopCount(&loopCount))
    {
        /* jump to end of process if no valid loop count obtained */
        goto LoopBackDouble_SinglePacket_Finish;
    }

    /* check if user would like to compare /  check the validity of
     * the packet data received, to that transmitted
     */
    if (!ChooseCheckData(&compare))
    {
        /* jump to end of process if failed to obtain compare status */
        goto LoopBackDouble_SinglePacket_Finish;
    }

    /* for each channel */
    for (int i = 0; i < 2; i++)
    {
        /* allocate memory for the transmit buffer */
        pTxBuffer[i] = (char *)calloc(1, byteSize);

        /* if failed to allocate memory */
        if (!pTxBuffer[i])
        {
            /* report error and jump to end of process */
            puts("\nERROR: Unable to allocate memory for transmit buffer");
            goto LoopBackDouble_SinglePacket_Finish;
        }

        /* fill transmit buffer with random data */
        FillBufferRandomChar(pTxBuffer[i], byteSize, DATA_TYPE_RANDOM);

        /* create a receive operation to receive 1 packet */
        pReceiveOp[i] = CreateReceiveOperation(1, 
            ReceiveOperation::RECEIVE_PACKETS);

        /* if failed to create receive operation */
        if (pReceiveOp[i] == NULL)
        {
            /* report error, jump to end of process */
            puts("\nERROR: Unable to create receive operation");
            goto LoopBackDouble_SinglePacket_Finish;
        }

        /* create a packet to be transmitted */
        pSpaceWirePacket[i] = CreatePacket(pTransmitAddressPaths[i],
                                           (U8 *)pTxBuffer[i], byteSize,
                                           Packet::EOP_TYPE_EOP);

        /* if failed to create packet */
        if (pSpaceWirePacket[i] == NULL)
        {
            /* report error, jump to end of process */
            puts("\nERROR: Unable to create the packet to be transmitted");
            goto LoopBackDouble_SinglePacket_Finish;
        }

        /* create a transmit operation for the packet */
        pTransmitOp[i] = CreateTransmitOperation(
            (StreamItem **)&pSpaceWirePacket[i], 1);

        /* if failed to create transmit operation */
        if (pTransmitOp[i] == NULL)
        {
            /* report error, jump to end of process */
            puts("\nERROR: Unable to create the transfer operation to be"
                " transmitted");
            goto LoopBackDouble_SinglePacket_Finish;
        }

        /* add transfer operations to list for channel 'i' */
        pTransferOperationList[i][0] = pReceiveOp[i];
        pTransferOperationList[i][1] = pTransmitOp[i];
    }

    /* display header for start of test */
    puts("Starting Single-Packet Double Loopback Test...");

    /* get start time for test */
    start = GET_TIME();

    /* for the number of tests required */
    for (int testNum = 0; testNum < (int)loopCount; testNum++)
    {
        /* for each channel */
        for (int i = 0; i < 2; i++)
        {
            /* if failed to submit the transfer operation list on the channel */
            if (!channels[i].SubmitTransferOperationList(
                pTransferOperationList[i], 2))
            {
                /* report and record error, jump to end of process */
                printf("\nERROR occurred submitting operation list."
                    "  Test %d failed.\n", testNum + 1);
                errorCount++;
                goto LoopBackDouble_SinglePacket_Finish;
            }
        }

        /* for each channel */
        for (int i = 0; i < 2; i++)
        {
            /* wait on the transmit operation completing */
            txStatus = pTransmitOp[i]->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

            /* if transmit operation failed to complete */
            if(txStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
            {
                /* report and record error, jump to end of process */
                printf("\nERROR occurred during transmit.  Test %d failed.\n",
                    testNum + 1);
                errorCount++;
                goto LoopBackDouble_SinglePacket_Finish;
            }
        }

        /* for each channel */
        for (int i = 0; i < 2; i++)
        {
            /* wait on the receive operation completing */
            rxStatus = pReceiveOp[i]->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

            /* if receive operation failed to complete */
            if (rxStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
            {
                /* report and record error, jump to end of process */
                printf("\nERROR occurred during receive.  Test %d failed.\n",
                    testNum + 1);
                errorCount++;
                goto LoopBackDouble_SinglePacket_Finish;
            }
        }

        /* if received packet data has to be compared to that sent */
        if(compare)
        {
            /* compare the received packets to the buffer transmitted */
            errorCount += ComparePackets(pReceiveOp[1], 1, byteSize,
                pTxBuffer[0]);
            errorCount += ComparePackets(pReceiveOp[0], 1, byteSize,
                pTxBuffer[1]);
        }
    }

    /* get end time for test */
    finish = GET_TIME();

    /* display the test results */
    DisplayResults(start, finish, compare, byteSize, 1, loopCount, errorCount,
        "Single-Packet Double Loopback Test");

/* complete processing, clean up resources */
LoopBackDouble_SinglePacket_Finish:

    /* for each channel */
    for (int i = 0; i < 2; i++)
    {
        /* dispose of the transfer operations */
        if (pTransmitOp[i] != NULL)
            delete pTransmitOp[i];
        if (pReceiveOp[i] != NULL)
            delete pReceiveOp[i];

        /* destroy the packet transmitted */
        if (pSpaceWirePacket[i] != NULL)
            delete pSpaceWirePacket[i];

        /* destroy the transmit path address, if any */
        if (pTransmitAddressPaths[i] != NULL)
        {
            delete pTransmitAddressPaths[i];
        }

        /* free the transmit buffer */
        if (pTxBuffer[i])
        {
            free(pTxBuffer[i]);
        }

        /* close the channel */
        channels[i].CloseChannel();
    }
}

/**
 * Performs a Double loopback test with multiple packets. Multiple packets of
 * random data are transmitted out of two links simultaneously (using a single
 * transfer operation per link) and received at either end simultaneously 
 * (again using a single transfer operation per link).
 *
 * This test is timed so as to test link speed. Optionally, the data is then
 * compared to check for errors.
 */
void StarSystemTest::LoopBackDouble_MultiPacket(void)
{
    /* declare necessary variables for test */
    Channel channels[2];
    unsigned long loopCount, errorCount = 0, byteSize, packetCount = 0;
    int compare;
    char *pTxBuffer[2] = { NULL, NULL };
    TransmitOperation *pTransmitOp[2] = { NULL, NULL };
    ReceiveOperation *pReceiveOp[2] = { NULL, NULL };
    TransferOperation *pTransferOperationList[2][2] = { {NULL} };
    Packet **pSpaceWirePackets[2] = { NULL, NULL };
    TransferOperation::TransferStatus rxStatus, txStatus;
    clock_t start, finish;
    Address *pTransmitAddressPaths[2] = { NULL, NULL };

    /* select the first device and channel to be used */
    if (!ChooseDeviceAndChannel("first", channels[0],
        Channel::DIRECTION_INOUT))
    {
        /* end processing if no valid channel obtained */
        return;
    }

    /* select the first (optional) path to be added to the front of packets
     * being transmitted
     */
    pTransmitAddressPaths[0] = GetTransmitPathAddress();

    /* select the second device and channel to be used */
    if (!ChooseDeviceAndChannel("second", channels[1],
        Channel::DIRECTION_INOUT))
    {
        /* jump to end of process if no valid channel obtained */
        goto LoopBackDouble_MultiPacket_Finish;
    }

    /* select the second (optional) path to be added to the front of packets
     * being transmitted
     */
    pTransmitAddressPaths[1] = GetTransmitPathAddress();

    /* get size of packet to use for test */
    if (!GetPacketSize(&byteSize))
    {
        /* jump to end of process if no valid packet size obtained */
        goto LoopBackDouble_MultiPacket_Finish;
    }

    /* get number of packets to send */
    if (!GetPacketCount(&packetCount))
    {
        /* jump to end of process if no valid packet count obtained */
        goto LoopBackDouble_MultiPacket_Finish;
    }

    /* get number of times to peform test */
    if (!GetLoopCount(&loopCount))
    {
        /* jump to end of process if no valid loop count obtained */
        goto LoopBackDouble_MultiPacket_Finish;
    }

    /* check if user wishes to compare the packet data received to that
     * which was originally transmitted
     */
    if (!ChooseCheckData(&compare))
    {
        /* jump to end of process if no valid compare status obtained */
        goto LoopBackDouble_MultiPacket_Finish;
    }

    /* for each channel */
    for (int i = 0; i < 2; i++)
    {
        /* allocate memory for the transmit buffer */
        pTxBuffer[i] = (char *)calloc(1, byteSize * packetCount);

        /* if failed to allocate memory */
        if (!pTxBuffer[i])
        {
            /* report error, jump to end of process */
            puts("\nERROR: Unable to allocate memory for transmit buffer");
            goto LoopBackDouble_MultiPacket_Finish;
        }

        /* fill transmit buffer with random data */
        FillBufferRandomChar(pTxBuffer[i], byteSize * packetCount,
            DATA_TYPE_RANDOM);

        /* create a receive operation to receive the packets */
        pReceiveOp[i] = CreateReceiveOperation((int)packetCount,
                                            ReceiveOperation::RECEIVE_PACKETS);

        /* if failed to create receive op */
        if (pReceiveOp[i] == NULL)
        {
            /* report error, jump to end of process */
            puts("\nERROR: Unable to create receive operation");
            goto LoopBackDouble_MultiPacket_Finish;
        }

        /* create and initialise (to NULL) an array of pointers to
         * packets for channel 'i' for the 'packetCount' specified */
        try
        {
            pSpaceWirePackets[i] = new Packet*[packetCount];
            for (int packetNum = 0; packetNum < (int)packetCount; packetNum++)
            {
                pSpaceWirePackets[i][packetNum] = NULL;
            }
        }
        catch (std::bad_alloc& ba)
        {
            /* memory allocation failure */
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);

            /* ensure array pointer is set to NULL */
            pSpaceWirePackets[i] = NULL;
        }

        /* if failed to allocate memory for the Packet pointers */
        if (pSpaceWirePackets[i] == NULL)
        {
            /* report error, jump to end of processing */
            puts("\nERROR: Unable to allocate memory for transmit stream items");
            goto LoopBackDouble_MultiPacket_Finish;
        }

        /* for each packet */
        for (int packetNum = 0; packetNum < (int)packetCount; packetNum++)
        {
            /* create the packet to be transmitted */
            pSpaceWirePackets[i][packetNum] =
                                CreatePacket(pTransmitAddressPaths[i],
                                     (U8 *)pTxBuffer[i] + byteSize * 
                                     (unsigned long)packetNum,
                                     (unsigned int)byteSize, 
                                     Packet::EOP_TYPE_EOP);

            /* if failed to create packet */
            if (pSpaceWirePackets[i][packetNum] == NULL)
            {
                /* report error, jump to end of processing */
                printf("\nERROR: Unable to create the packet %d to be"
                    " transmitted\n", packetNum);
                goto LoopBackDouble_MultiPacket_Finish;
            }
        }

        /* create the transmit transfer operation for the packets */
        pTransmitOp[i] = CreateTransmitOperation(
                              (StreamItem**)pSpaceWirePackets[i], 
                              (int)packetCount);

        /* if failed to create transmit operation */
        if (pTransmitOp[i] == NULL)
        {
            /* report error, jump to end of processing */
            puts("\nERROR: Unable to create the transfer operation to be"
                " transmitted");
            goto LoopBackDouble_MultiPacket_Finish;
        }

        /* add transfer operations to list for channel 'i' */
        pTransferOperationList[i][0] = pReceiveOp[i];
        pTransferOperationList[i][1] = pTransmitOp[i];
    }

    /* display header for start of test */
    puts("Running Multi-Packet Double Loopback Test...");

    /* get the start time of the test*/
    start = GET_TIME();

    /* for the number of tests to perform */
    for (int testNum = 0; testNum < (int)loopCount; testNum++)
    {
        /* for each channel */
        for (int i = 0; i < 2; i++)
        {
            /* if failed to submit the transfer operation list */
            if (!channels[i].SubmitTransferOperationList(
                                                 pTransferOperationList[i], 2))
            {
                /* report and record error, jump to end of processing */
                printf("\nERROR occurred submitting operation list."
                    "  Test %d failed.\n", testNum + 1);
                errorCount++;
                goto LoopBackDouble_MultiPacket_Finish;
            }
        }

        /* for each channel */
        for (int i = 0; i < 2; i++)
        {
            /* wait on the transmit operation completing */
            txStatus = pTransmitOp[i]->WaitOnTransferOperationCompletion(
                                         TransferOperation::WAIT_INDEFINITELY);

            /* if transmit operation failed to complete */
            if(txStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
            {
                /* report and record error, jump to end of processing */
                printf("\nERROR occurred during transmit.  Test %d failed.\n",
                    testNum + 1);
                errorCount++;
                goto LoopBackDouble_MultiPacket_Finish;
            }
        }

        /* for each channel */
        for (int i = 0; i < 2; i++)
        {
            /* wait on the receive operation completing */
            rxStatus = pReceiveOp[i]->WaitOnTransferOperationCompletion(
                                         TransferOperation::WAIT_INDEFINITELY);

            /* if receive operation failed to complete */
            if (rxStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
            {
                /* report and record error, jump to end of processing */
                printf("\nERROR occurred during receive.  Test %d failed.\n",
                    testNum + 1);
                errorCount++;
                goto LoopBackDouble_MultiPacket_Finish;
            }
        }

        /* if received packets are to be compared to transmitted packets */
        if(compare)
        {
            /* compare the received packets to the buffer transmitted */
            errorCount += ComparePackets(pReceiveOp[1], packetCount,
                byteSize, pTxBuffer[0]);
            errorCount += ComparePackets(pReceiveOp[0], packetCount,
                byteSize, pTxBuffer[1]);
        }
    }

    /* get end time for test */
    finish = GET_TIME();

    /* display the test results */
    DisplayResults(start, finish, compare, byteSize, packetCount, loopCount,
        errorCount, "Multi-Packet Double Loopback Test");

/* clean up resources used during test */
LoopBackDouble_MultiPacket_Finish:

    /* for each channel */
    for (int i = 0; i < 2; i++)
    {
        /* dispose of the transfer operations */
        if (pTransmitOp[i] != NULL)
            delete pTransmitOp[i];
        if (pReceiveOp[i] != NULL)
            delete pReceiveOp[i];

        /* destroy the packets transmitted */
        if (pSpaceWirePackets[i] != NULL)
        {
            /* for each packet created for channel 'i' */
            for (int packetNum = 0; packetNum < (int)packetCount; packetNum++)
            {
                /* if the packet is valid / non-NULL*/
                if (pSpaceWirePackets[i][packetNum] != NULL)
                {
                    /* delete the packet */
                    delete pSpaceWirePackets[i][packetNum];
                }
            }

            /* delete the array which was allocated to contain the packets
             * for channel 'i'
             */
            delete [] pSpaceWirePackets[i];
        }

        /* destroy the transmit path address, if any */
        if (pTransmitAddressPaths[i] != NULL)
        {
            delete pTransmitAddressPaths[i];
        }

        /* free the transmit buffer */
        if (pTxBuffer[i])
        {
            free(pTxBuffer[i]);
        }

        /* close the channel */
        channels[i].CloseChannel();
    }
}



/**
 * Transmits a single packet using one transfer operation.
 */
void StarSystemTest::Transmit_SinglePacket(void)
{
    /* declare necessary variables for the test */
    Channel txChannel;
    char *pTxBuffer = NULL;
    TransmitOperation *pTransmitOp = NULL;
    Packet *pPacket = NULL;
    unsigned long loopCount, byteSize;
    clock_t start, finish;
    TransferOperation::TransferStatus txStatus;
    Address *pAddress = NULL;

    /* select the transmit device and channel to be used */
    if (!ChooseDeviceAndChannel("transmit", txChannel,
        Channel::DIRECTION_OUT))
    {
        return;
    }

    /* get the address to route the packet to */
    pAddress = GetTransmitPathAddress();

    /* if no address provided, or error occurred creating it */
    if (pAddress == NULL)
    {
        /* inform user, in the event that they attempted to provide an
         * address but an error occurred
         */
        puts("No SpaceWire Address provided...");
    }

    /* get the size of packet to send */
    if (!GetPacketSize(&byteSize))
    {
        /* jump to end of process if no valid packet size obtained */
        goto Transmit_SinglePacket_Finish;
    }

    /* get the number of times to run the test */
    if (!GetLoopCount(&loopCount))
    {
        /* jump to end of process if no valid loop count obtained */
        goto Transmit_SinglePacket_Finish;
    }

    /* allocate memory for the transmit buffer */
    pTxBuffer = (char *)calloc(1, byteSize);

    /* if failed to allocate memory */
    if (!pTxBuffer)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to allocate memory for transmit buffer");
        goto Transmit_SinglePacket_Finish;
    }

    /* fill transmit buffer with random data */
    FillBufferRandomChar(pTxBuffer, byteSize, DATA_TYPE_RANDOM);

    /* create the packet to be transmitted */
    pPacket = CreatePacket(pAddress, (U8 *)pTxBuffer, byteSize,
                           Packet::EOP_TYPE_EOP);

    /* if failed to create packet */
    if (pPacket == NULL)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to create the packet to be transmitted");
        goto Transmit_SinglePacket_Finish;
    }

    /* create a transmit transfer operation for the packet */
    pTransmitOp = CreateTransmitOperation((StreamItem **)&pPacket, 1);

    /* if failed to create transmit op */
    if (pTransmitOp == NULL)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to create the transfer operation to be"
            " transmitted");
        goto Transmit_SinglePacket_Finish;
    }

    /* display header for test results */
    puts("Starting Single-Packet Transmit Test...");

    /* get the start time for the test */
    start = GET_TIME();

    /* for the number of tests required */
    for(unsigned long testNum = 0; testNum < loopCount; testNum++)
    {
        /* submit the transmit operation on the channel opened */
        int result = txChannel.SubmitTransferOperation(pTransmitOp);

        /* if transmit operation failed to submit */
        if (result == 0)
        {
            /* report error, jump to end of process */
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1);
            goto Transmit_SinglePacket_Finish;
        }

        /* wait on the transmit operation completing */
        txStatus = pTransmitOp->WaitOnTransferOperationCompletion(
                                         TransferOperation::WAIT_INDEFINITELY);

        /* if transmit failed to complete */
        if(txStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
        {
            /* report error, jump to end of process */
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1);
            goto Transmit_SinglePacket_Finish;
        }
    }

    /* get the end time for the test */
    finish = GET_TIME();

    /* display the test results */
    DisplayResults(start, finish, 0, byteSize, 1, loopCount, 0,
        "Single-Packet Transmit Test");

/* clean up test resources */
Transmit_SinglePacket_Finish:

    /* dispose of the transmit operation */
    if (pTransmitOp != NULL)
    {
        delete pTransmitOp;
    }

    /* destroy the packet transmitted */
    if (pPacket != NULL)
    {
        delete pPacket;
    }

    /* free the transmit buffer */
    if (pTxBuffer != NULL)
    {
        free(pTxBuffer);
    }

    /* free the address created */
    if (pAddress != NULL)
    {
        delete pAddress;
    }

    /* close the channel */
    txChannel.CloseChannel();
}

/**
 * Transmits multiple packets using a single transfer operation.
 */
void StarSystemTest::Transmit_MultiPacket()
{
    /* declare necessary variables for the test */
    Channel txChannel;
    unsigned long loopCount, byteSize, packetCount = 0;
    char *pTxBuffer = NULL;
    StreamItem **pPackets = NULL;
    TransmitOperation *pTransmitOp = NULL;
    TransferOperation::TransferStatus txStatus;
    clock_t start, finish;
    Address *pAddress = NULL;

    /* select the transmit device and channel to be used */
    if (!ChooseDeviceAndChannel("transmit", txChannel,
        Channel::DIRECTION_OUT))
    {
        /* end processing if no valid channel obtained */
        return;
    }

    /* get the address to route the packets to */
    pAddress = GetTransmitPathAddress();

    /* get size of packet to send */
    if (!GetPacketSize(&byteSize))
    {
        /* jump to end of process if no valid packet size obtained */
        goto Transmit_MultiPacket_Finish;
    }

    /* get number of packets to send */
    if (!GetPacketCount(&packetCount))
    {
        /* jump to end of process if no valid packet count obtained */
        goto Transmit_MultiPacket_Finish;
    }

    /* get number of times to perform the test */
    if (!GetLoopCount(&loopCount))
    {
        /* jump to end of process if no valid loop count obtained */
        goto Transmit_MultiPacket_Finish;
    }

    /* allocate memory for the transmit buffer */
    pTxBuffer = (char *)calloc(1, byteSize * packetCount);

    /* if failed to allocate transmit buffer */
    if (!pTxBuffer)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to allocate memory for transmit buffer");
        goto Transmit_MultiPacket_Finish;
    }

    /* fill transmit buffer with random data */
    FillBufferRandomChar(pTxBuffer, byteSize * packetCount, DATA_TYPE_RANDOM);

    /* create an array of pointers to StreamItems (to be filled with
     * Packets later) for the number of packets specified
     */
    pPackets = CreateArrayOfPointersToStreamItems((int)packetCount);

    /* if failed to allocate array of StreamItem pointers */
    if (pPackets == NULL)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to allocate memory for transmit stream items");
        goto Transmit_MultiPacket_Finish;
    }
    else
    {
        /* initialise the pointers in the array to NULL */
        for (unsigned long i=0; i<packetCount; i++)
            pPackets[i] = NULL;
    }

    /* for the number of packets being sent */
    for (unsigned long i = 0; i < packetCount; i++)
    {
        /* create a new packet */
        Packet *pPacket = CreatePacket(pAddress,
                                        (U8 *)pTxBuffer + (byteSize * i),
                                        byteSize, Packet::EOP_TYPE_EOP);

        /* if failed to create packet */
        if (pPacket == NULL)
        {
            /* report error, jump to end of process */
            printf("\nERROR: Unable to create packet %lu to be transmitted", i);
            goto Transmit_MultiPacket_Finish;
        }

        /* otherwise, add packet to array of pointers to StreamItems */
        pPackets[i] = pPacket;
    }

    /* create a transmit operation for the packets */
    pTransmitOp = CreateTransmitOperation(pPackets, (int)packetCount);

    /* if failed to create transmit op */
    if (pTransmitOp == NULL)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to create the transfer operation to be transmitted");
        goto Transmit_MultiPacket_Finish;
    }

    /* display header for test results */
    puts("Starting Multi-Packet Transmit Test...");

    /* get the start time for the test */
    start = GET_TIME();

    /* for the number of tests required */
    for(unsigned long testNum = 0; testNum < loopCount; testNum++)
    {
        /* submit the transmit operation */
        int result = txChannel.SubmitTransferOperation(pTransmitOp);

        /* if transmit op failed to submit */
        if (result == 0)
        {
            /* report error, jump to end of process */
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1);
            goto Transmit_MultiPacket_Finish;
        }

        /* wait on the transmit operation completing */
        txStatus = pTransmitOp->WaitOnTransferOperationCompletion(
                                         TransferOperation::WAIT_INDEFINITELY);

        /* if transmit op failed to complete */
        if(txStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
        {
            /* report error, jump to end of process */
            printf("\nERROR occurred during transmit.  Test %lu failed.\n",
                testNum + 1);
            goto Transmit_MultiPacket_Finish;
        }
    }

    /* get end time for test */
    finish = GET_TIME();

    /* display the test results */
    DisplayResults(start, finish, 0, byteSize, packetCount, loopCount, 0,
        "Multiple-Packet Transmit Test");

/* test finished, clean up test resources */
Transmit_MultiPacket_Finish:

    /* dispose of the transfer operation */
    if (pTransmitOp != NULL)
    {
        delete pTransmitOp;
    }

    /* destroy the packets transmitted */
    if (pPackets != NULL)
    {
        /* for each item in the array of StreamItem pointers */
        for (unsigned long i = 0; i < packetCount; i++)
        {
            /* if the item pointed to is valid (non-NULL) */
            if (pPackets[i] != NULL)
            {
                /* delete the item pointed to */
                delete pPackets[i];
            }
        }

        /* delete the array itself */
        delete [] pPackets;
    }

    /* free the transmit buffer */
    if (pTxBuffer)
    {
        free(pTxBuffer);
    }

    /* free the address */
    if (pAddress != NULL)
    {
        delete pAddress;
    }

    /* close the channel */
    txChannel.CloseChannel();
}



/**
 * Receives a single packet using one transfer operation.
 */
void StarSystemTest::Receive_SinglePacket()
{
    /* declare variables necessary for the test */
    Channel rxChannel;
    ReceiveOperation *pReceiveOp = NULL;
    unsigned long loopCount, rxByteSize = 0;
    clock_t start, finish;
    TransferOperation::TransferStatus rxStatus;

    /* select the receive device and channel to be used */
    if (!ChooseDeviceAndChannel("receive", rxChannel,
        Channel::DIRECTION_IN))
    {
        /* jump to end of process if no valid channel obtained */
        goto Receive_SinglePacket_Finish;
    }

    /* get number of times to perform the test */
    if (!GetLoopCount(&loopCount))
    {
        /* jump to end of process if no valid test count obtained */
        goto Receive_SinglePacket_Finish;
    }

    /* create a receive operation to receive 1 packet */
    pReceiveOp = CreateReceiveOperation(1, ReceiveOperation::RECEIVE_PACKETS);

    /* if failed to create receive operation */
    if (pReceiveOp == NULL)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to create receive operation");
        goto Receive_SinglePacket_Finish;
    }

    /* display header for test results */
    puts("Running Single-Packet Receive Test...");

    /* get the start time of the test (this should be updated when the
     * first packet is received) */
    start = GET_TIME();

    /* for the number of tests required */
    for(unsigned long testNum = 0; testNum < loopCount; testNum++)
    {
        /* submit the receive operation */
        int result = rxChannel.SubmitTransferOperation(pReceiveOp);

        /* if receive op failed to submit */
        if (result == 0)
        {
            /* report error, jump to end of process */
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1);
            goto Receive_SinglePacket_Finish;
        }

        /* wait on the receive operation completing */
        rxStatus = pReceiveOp->WaitOnTransferOperationCompletion(
                                         TransferOperation::WAIT_INDEFINITELY);

        /* if receive op failed to complete */
        if (rxStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
        {
            /* report error, jump to end of process */
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1);
            goto Receive_SinglePacket_Finish;
        }

        /* if this is the first packet received (i.e. the first run through
         * of the test) */
        if (testNum == 0)
        {
            /* make this the start time of the test */
            start = GET_TIME();
        }
        else
        {
            /* otherwise, increment the record of the total lengths of
             * packets received */
            rxByteSize += GetPacketLengths(pReceiveOp, 1);
        }
    }

    /* get the end time for the test */
    finish = GET_TIME();

    /* display the test results */
    DisplayResults(start, finish, 0, rxByteSize, 1, 1, 0,
        "Single-Packet Receive Test");

/* test finished, clean up resources */
Receive_SinglePacket_Finish:

    /* dispose of the receive operation */
    if (pReceiveOp != NULL)
    {
        delete pReceiveOp;
    }

    /* close the channel */
    rxChannel.CloseChannel();
}

/**
 * Get the combined length of received traffic, and check for errors.
 *
 * @param pReceiveOp The completed receive operation containing the packets to
 *                   look at.
 * @param packetCount The number of packets which should be in the received
 *                    operation.
 *
 * @return The length of the received packets.
 *
 */
unsigned long StarSystemTest::GetPacketLengths(ReceiveOperation* pReceiveOp,
                                unsigned long packetCount)
{
    unsigned long rxPacketLength = 0;
    int rxPacketCount, i;

    /* Get the number of stream items received */
    rxPacketCount = (int)pReceiveOp->GetTransferItemCount();

    /* if number of items received isn't as expected */
    if ((unsigned)rxPacketCount != packetCount)
    {
        /* report error */
        printf("\nERROR expected to receive %lu packets but received %d.",
            packetCount, rxPacketCount);
    }

    /* for each transfer item received */
    for (i = 0; i < rxPacketCount; i++)
    {
        /* get the next stream item */
        StreamItem *pRxStreamItem = pReceiveOp->GetTransferItem((unsigned int)i);

        /* if stream item isn't valid or isn't a packet */
        if ((pRxStreamItem == NULL) ||
            (pRxStreamItem->GetStreamItemType() !=
            StreamItem::STREAM_ITEM_TYPE_PACKET))
        {
            /* report  error */
            printf("\nERROR received an unexpected traffic type, or empty "
                   "traffic item in item %d", i);
        }
        else
        {
            /* cast stream item to packet */
            Packet *pPacket = (Packet*)pRxStreamItem;

            /* get packet's length and add to running total */
            rxPacketLength += pPacket->GetPacketLength();
        }

        /* free the stream item */
        delete pRxStreamItem;
    }

    /* return the total packet length */
    return rxPacketLength;
}


/**
 * Receives multiple packets using a single transfer operation.
 */
void StarSystemTest::Receive_MultiPacket()
{
    /* declare variables for the test */
    Channel rxChannel;
    unsigned long loopCount, rxByteSize = 0, packetCount;
    ReceiveOperation *pReceiveOp = NULL;
    TransferOperation::TransferStatus rxStatus;
    clock_t start, finish;

    /* select the receive device and channel to be used */
    if (!ChooseDeviceAndChannel("receive", rxChannel,
        Channel::DIRECTION_IN))
    {
        /* jump to end of process if no valid channel obtained */
        goto Receive_MultiPacket_Finish;
    }

    /* get number of packets to receive */
    if (!GetPacketCount(&packetCount))
    {
        /* jump to end of process if no valid packet count obtained */
        goto Receive_MultiPacket_Finish;
    }

    /* get number of times to perform the test */
    if (!GetLoopCount(&loopCount))
    {
        /* jump to end of process if no valid test count obtained */
        goto Receive_MultiPacket_Finish;
    }

    /* create a receive operation to receive the packets */
    pReceiveOp = CreateReceiveOperation((int)packetCount,
                                        ReceiveOperation::RECEIVE_PACKETS);

    /* if receive op failed to create */
    if (pReceiveOp == NULL)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to create receive operation");
        goto Receive_MultiPacket_Finish;
    }

    /* display header for test results */
    puts("Running Multiple-Packet Receive Test...");

    /* get the start time for the test (this should be updated when the
    * first packets are received) */
    start = GET_TIME();

    /* for the number of tests required */
    for(unsigned long testNum = 0; testNum < loopCount; testNum++)
    {
        /* submit the receive operation on the receive channel */
        int result = rxChannel.SubmitTransferOperation(pReceiveOp);

        /* if receive op failed to submit */
        if (result == 0)
        {
            /* report error, jump to end of process */
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1);
            goto Receive_MultiPacket_Finish;
        }

        /* wait on the receive operation completing */
        rxStatus = pReceiveOp->WaitOnTransferOperationCompletion(
                                         TransferOperation::WAIT_INDEFINITELY);

        /* if receive op failed to complete */
        if (rxStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
        {
            /* report error, jump to end of process */
            printf("\nERROR occurred during receive.  Test %lu failed.\n",
                testNum + 1);
            goto Receive_MultiPacket_Finish;
        }

        /* if this is the first packet(s) received (i.e. the first in the
         * the number of tests required)
         */
        if (testNum == 0)
        {
            /* make this the start time for the test */
            start = GET_TIME();
        }
        else
        {
            /* otherwise, increment the record of the total lengths of
             * packets received
             */
            rxByteSize += GetPacketLengths(pReceiveOp, packetCount);
        }
    }

    /* get the end time for the test */
    finish = GET_TIME();

    /* display the test results */
    DisplayResults(start, finish, 0, rxByteSize, 1, 1, 0,
        "Multi-Packet Receive Test");

/* test finished, clean up resources */
Receive_MultiPacket_Finish:

    /* dispose of the receive operation */
    if (pReceiveOp != NULL)
    {
        delete pReceiveOp;
    }

    /* close the channel */
    rxChannel.CloseChannel();
}



/**
 *  Transmits a file whole, as one packet. Not preceded by a header packet.
 */
void StarSystemTest::TransmitFile_Whole()
{
    /* declare variables required to run test */
    Channel txChannel;
    char *pTxBuffer = NULL;
    TransmitOperation *pTransmitOp = NULL;
    Packet *pPacket = NULL;
    TransferOperation::TransferStatus txStatus;
    Address *pAddress = NULL;
    char sFile[256];
    int fileSize = 0, result;
    size_t sFileLen;

    /* select the transmit device and channel to be used */
    if (!ChooseDeviceAndChannel("transmit", txChannel,
        Channel::DIRECTION_OUT))
    {
        /* end processing if no valid channel obtained */
        return;
    }

    /* get the address to route the packets to */
    pAddress = GetTransmitPathAddress();

    /* display request for the name of the file to read from */
    printf("Enter name of file to transmit: ");

    /* if failed to read in file name */
    if (!fgets(sFile, 256, stdin))
    {
        /* report error, end processing */
        puts("ERROR: Invalid Input");
        return;
    }

    /* strip newline from filename entered, replace with NULL terminator */
    sFileLen = strlen(sFile);
    if (sFile[sFileLen - 1] == '\n')
    {
        sFile[sFileLen - 1] = '\0';
    }

    /* display filename and request confirmation to proceed */
    printf("Filename = \"%s\"\n", sFile);
    printf("Continue with these values ? (Y/N): ");

    /* if user doesn't wish to proceed */
    if (!ConfirmYes())
    {
        /* confirm their intent and jump to end of process */
        puts("File Transfer aborted");
        goto Transmit_File_Whole_Finish;
    }

    /* get the size of file to read from */
    fileSize = SizeOfFile(sFile);

    /* if invalid file size */
    if(fileSize <= 0)
    {
        /* report error, jump to end of process */
        puts("File size invalid, aborting transfer.");
        goto Transmit_File_Whole_Finish;
    }

    /* allocate buffer to read contents of file into */
    pTxBuffer = (char *)malloc((size_t)fileSize);

    /* if failed to allocate memory for buffer */
    if(!pTxBuffer)
    {
        /* report error, jump to end of process */
        puts("ERROR: Could not allocate buffer for file");
        goto Transmit_File_Whole_Finish;
    }

    /* attempt to read the contents of the file into the buffer */
    if(!ReadFromFileIntoBuffer(sFile,pTxBuffer,0,(unsigned int)fileSize))
    {
        /* report error and jump to end of process if failed to read from file */
        puts("ERROR: Could not read file");
        goto Transmit_File_Whole_Finish;
    }

    /* create a packet to contain the data read from the file */
    pPacket = CreatePacket(pAddress, (U8 *)pTxBuffer, (unsigned int)fileSize,
                           Packet::EOP_TYPE_EOP);

    /* if failed to create packet */
    if (pPacket == NULL)
    {
        /* report error, jump to end of process */
        puts("ERROR: Unable to create the packet to be transmitted");
        goto Transmit_File_Whole_Finish;
    }

    /* create a transmit operation for the packet */
    pTransmitOp = CreateTransmitOperation((StreamItem **)&pPacket, 1);

    /* if failed to create transmit op */
    if (pTransmitOp == NULL)
    {
        /* report error, jump to end of process */
        puts("ERROR: Unable to create the transfer operation to be transmitted");
        goto Transmit_File_Whole_Finish;
    }

    /* notify user: file transmit starting */
    puts("Starting File Transmit...");

    /* submit the transmit operation on the transmit channel */
    result = txChannel.SubmitTransferOperation(pTransmitOp);

    /* if transmit op failed to submit */
    if (result == 0)
    {
        /* report error, jump to end of process */
        puts("ERROR occurred during transmit");
        goto Transmit_File_Whole_Finish;
    }

    /* wait on the transmit operation completing */
    txStatus = pTransmitOp->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

    /* if transmit op failed to complete */
    if(txStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        /* report error, jump to end of process */
        puts("ERROR occurred during transmit");
        goto Transmit_File_Whole_Finish;
    }

    /* notify user: transmit complete */
    puts("Complete.");

/* end of file transmit: clean up resources */
Transmit_File_Whole_Finish:

    /* dispose of the transfer operation */
    if (pTransmitOp != NULL)
    {
        delete pTransmitOp;
    }

    /* destroy the packet transmitted */
    if (pPacket != NULL)
    {
        delete pPacket;
    }

    /* free the transmit buffer */
    if (pTxBuffer)
    {
        free(pTxBuffer);
    }

    /* free the address */
    if (pAddress != NULL)
    {
        delete pAddress;
    }

    /* Close the channel */
    txChannel.CloseChannel();
}



/**
 * Gets a yes or no answer from user
 *
 * @return 1 if user input Y, 0 if user input N
 */
int StarSystemTest::ConfirmYes()
{
    /* string to hold user input */
    char response[256] = {0};

    /* length of input */
    size_t responseLen = 0;

    /* until a response is obtained */
    for(;;)
    {
        /* attempt to obtain input from the user if error obtaining input */
        if (!fgets(response, 256, stdin))
        {
            /* report error */
            puts("ERROR: Invalid Input");

            /* return 0 - equivalent to 'N'/No */
            return 0;
        }

        /* strip newline from input, replace with NULL terminator */
        responseLen = strlen(response);
        if (response[responseLen - 1] == '\n')
        {
            response[responseLen - 1] = '\0';
        }

        /* if response is 'y' / yes */
        if(strcmp(response,"y")== 0 ||
            strcmp(response,"Y") == 0)
        {
            /* return a result of 1 (meaning 'y' / yes) */
            return 1;
        }
        /* else if it is 'n' / no */
        else if(strcmp(response,"n") == 0  ||
                strcmp(response,"N") == 0)
        {
            /* return a result of 0 (meaning 'n' / no) */
            return 0;
        }
        else
        {
            /* unrecognised input, ask user again */
            puts("Please enter Y/N");
        }
    }
}

/**
 * Get and return the size, in bytes, of the specified file.
 *
 * @param filePath The name and path of the file to check.
 *
 * @return The size of the file.
 */
int StarSystemTest::SizeOfFile( char filePath[] )
{
    /* declare file pointer */
    FILE *infile;
    int size;

    /* attempt to open file in binary mode */
    infile = fopen(filePath, "rb");

    /* if failed to open file */
    if (infile == NULL)
    {
        /* report error, return zero file size */
        printf("Error: Trouble obtaining file size\n");
        return 0;
    }

    /* move to the end of the file */
    fseek(infile, 0, SEEK_END);

    /* get number of bytes from beginning of file to current position / end */
    size = ftell(infile);

    /* move back to beginning of file */
    fseek(infile, 0, SEEK_SET);

    /* close the file */
    fclose(infile);

    /* return the file's size */
    return size;
}


/**
 * Read a specified number of bytes from a specified file into buffer.
 *
 * @param fname The name and path to the file to read from.
 * @param pBuffer The buffer to read the file contents into
 * @param offset The position in the file to start reading from.
 * @param size The number of bytes to read from the file.
 *
 * @return 1 if read from file successfully, otherwise 0.
 */
int StarSystemTest::ReadFromFileIntoBuffer(char fname[], char *pBuffer,
                                           long offset, unsigned int size)
{
    FILE *infile;
    unsigned int count;

    /* open the file */
    infile = fopen(fname, "rb");

    /* if failed to open file */
    if (infile == NULL)
    {
        /* report error */
        printf("file_read:Trouble opening file\n");

        /* return error code */
        return 0;
    }

    /* move to specified offset / start position in file to read from */
    fseek(infile,offset,SEEK_SET);

    /* attempt to read the specified number of bytes from the file into
     * the buffer provided
     */
    count = (unsigned int)fread(pBuffer, sizeof(char), size, infile);

    /* if there was an error reading, or the correct number of bytes
     * hasn't been read
     */
    if ((ferror(infile)) || (count < size))
    {
        /* report error */
        printf("ERROR trying to read file %s at byte point %ld.\n", fname, offset);

        /* close file */
        fclose(infile);

        /* return error code */
        return 0;
    }

    /* close the file */
    fclose(infile);

    /* return success code */
    return 1;
}

/**
 * Resets a device
 */
void StarSystemTest::ResetDevice()
{
    /* pointer to Device */
    Device *device = NULL;

    /* ask user to select a device */
    device = ChooseDevice("");

    /* if no device selected */
    if (device == NULL)
    {
        /* end processing */
        return;
    }

    /* reset device */
    int result = device->ResetDevice();

    /* if device failed to reset */
    if (result == 0)
    {
        /* display error */
        puts("ERROR: Unable to reset device");
    }

    /* free the device object created */
    delete device;
}

/**
 * Identifies a device by flashing its LEDs (only applicable to Mk2 devices)
 */
void StarSystemTest::IdentifyDevice()
{
    Device *pDevice = NULL;
    Mk2DeviceConfiguration *pMk2DeviceConfiguration = NULL;

    /* get the device to identify */
    pDevice = ChooseDevice("");

    /* if no device selected */
    if (pDevice == NULL)
    {
        /* end processing */
        return;
    }

    /* if the device is a Mk2 device */
    if (DeviceTypeResolver::IsMk2Device(pDevice))
    {
        /* get the device's configuration */
        pMk2DeviceConfiguration =
                    (Mk2DeviceConfiguration*)pDevice->GetDeviceConfiguration();

        /* call identify on the device */
        pMk2DeviceConfiguration->Identify();

        /* free device configuration */
        delete pMk2DeviceConfiguration;
    }
    else
    {
        puts("Not a Mk2 device: unable to identify device");
    }

    /* free the device */
    delete pDevice;
}


/**
 * Receives a file which has been transmitted whole, as one packet without a
 * size header.
 */
void StarSystemTest::ReceiveFile_Whole()
{
    /* declare variables required during the test */
    Channel rxChannel;
    ReceiveOperation *pReceiveOp = NULL;
    TransferOperation::TransferStatus rxStatus;
    char sFile[256];
    char writeStatus;
    StreamItem *pStreamItem = NULL;
    Packet *pReceivedPacket = NULL;
    size_t sFileLen;
    U8 *receivedData = NULL;
    unsigned int receivedDataLen = 0;
    int result;

    /* select the receive device and channel to be used */
    if (!ChooseDeviceAndChannel("receive", rxChannel,
        Channel::DIRECTION_IN))
    {
        /* jump to end of process if no valid channel obtained */
        goto Receive_File_Whole_Finish;
    }

    /* get the name of the file to write the received data into */
    printf("Enter name of file to write data to: ");

    /* if failed to obtain file name */
    if (!fgets(sFile, 256, stdin))
    {
        /* report error and end processing */
        puts("ERROR: Invalid Input");
        return;
    }

    /* replace newline in filename with NULL terminator */
    sFileLen = strlen(sFile);
    if (sFile[sFileLen - 1] == '\n')
    {
        sFile[sFileLen - 1] = '\0';
    }

    /* display file details and ask user to confirm whether to proceed */
    printf("Filename = \"%s\"\n", sFile);
    printf("Continue with these values ? (Y/N): ");

    /* if user doesn't wish to proceed */
    if (!ConfirmYes())
    {
        /* report error, jump to end of process */
        printf("File Transfer aborted\n");
        goto Receive_File_Whole_Finish;
    }

    /* notify user: file receive starting */
    puts("Running File Receive...");

    /* create a receive operation to receive 1 packet */
    pReceiveOp = CreateReceiveOperation(1, ReceiveOperation::RECEIVE_PACKETS);

    /* if receive op failed to create */
    if (pReceiveOp == NULL)
    {
        /* report error, jump to end of process */
        puts("ERROR: Unable to create receive operation");
        goto Receive_File_Whole_Finish;
    }

    /* submit the receive operation on the receive channel */
    result = rxChannel.SubmitTransferOperation(pReceiveOp);

    /* if receive op failed to submit */
    if (result == 0)
    {
        /* report error, jump to end of process */
        puts("ERROR: Could not submit receive operation");
        goto Receive_File_Whole_Finish;
    }

    /* wait on the receive operation completing */
    rxStatus = pReceiveOp->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

    /* if receive op failed to complete */
    if (rxStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        /* report error, jump to end of process */
        printf("ERROR: Error of %d occurred during receive: \n", rxStatus);
        goto Receive_File_Whole_Finish;
    }

    /* get received data, which should be a packet and which will be the
     * first transfer item in the Receive Operation (zero-based index of 0)
     */

    /* get the stream item received */
    pStreamItem = pReceiveOp->GetTransferItem(0);

    /* if valid stream item obtained */
    if (pStreamItem != NULL)
    {
        /* if it is a packet stream item */
        if (pStreamItem->GetStreamItemType() == 
            StreamItem::STREAM_ITEM_TYPE_PACKET)
        {
            /* cast it to a packet */
            pReceivedPacket = (Packet*)pStreamItem;
        }
        else
        {
            /* an unexpected stream item type */

            /* report error */
            puts("Unexpected stream item type received!");

            /* free the stream item */
            delete pStreamItem;
            pStreamItem = NULL;
        }
    }

    /* if failed to obtain expected packet from Receive Operation */
    if (pReceivedPacket == NULL)
    {
        /* report error, jump to end of process */
        printf("ERROR: Failed to retreive packet from Receive operation\n");
        goto Receive_File_Whole_Finish;
    }

    /* get the packet's data, and the length of the data.
     *
     * \note 'GetPacketData()' returns a pointer to newly allocated memory
     * containing the packet's data as an array of bytes. This memory must be
     * freed later by calling the 'DestroyPacketData()' method on the packet.
     */
    receivedData = pReceivedPacket->GetPacketData(&receivedDataLen);

    /* if failed to get the packet's data */
    if (receivedData == NULL)
    {
        /* report error, jump to end of process */
        printf("ERROR: Failed to retreive packet's data\n");
        goto Receive_File_Whole_Finish;
    }

    /* write packet data to file specified */
    writeStatus = (char)WriteBufferToFile(receivedData, (long)receivedDataLen, sFile);

    /* destroy the memory allocated to hold the data retreived from the packet */
    pReceivedPacket->DestroyPacketData(receivedData);

    /* delete / free the packet obtained from the receive operation */
    delete pReceivedPacket;
    pReceivedPacket = NULL;

    /* if write to file failed */
    if (writeStatus == 0)
    {
        /* report error, jump to end of process */
        puts("ERROR: Could not write file");
        goto Receive_File_Whole_Finish;
    }

    /* notify user: test complete */
    puts("Complete.");

/* test finished, clean up */
Receive_File_Whole_Finish:

    /* free packet if required */
    if (pReceivedPacket != NULL)
    {
        delete pReceivedPacket;
    }

    /* dispose of the transfer operation */
    if (pReceiveOp != NULL)
    {
        delete pReceiveOp;
    }

    /* close the channel */
    rxChannel.CloseChannel();
}

/**
 * Write (append) a buffer of data of a specified length to the given file.
 *
 * @param pData The buffer of data to write.
 * @param dataSize The size of the buffer.
 * @param fname The name/path of the file to write to.
 *
 * @return 1 if buffer written to file successfully, otherwise 0.
 */
int StarSystemTest::WriteBufferToFile(unsigned char *pData, long dataSize,
                                      char fname[])
{
    FILE *outfile;
    int returnValue = 1;

    /* open the file to write to (opened in append-binary mode) */
    outfile = fopen(fname, "ab");

    /* if failed to open file */
    if (outfile == NULL)
    {
        /* report error and return error code (0) */
        printf("\nfile_append_chunk: Trouble opening file: %s\n", fname);
        return 0;
    }

    /* write the buffer of data to the file */
    fwrite(pData, (size_t)1, (size_t)dataSize, outfile);

    /* if error writing data to file */
    if(ferror(outfile))
    {
        /* report and record error */
        printf("\nWrite error in file_append_chunk.\n");
        returnValue = 0;
    }

    /* close the file */
    fclose(outfile);

    /* return success / error accordingly */
    return returnValue;
}

/**
 * Transmits a file as multiple packets preceded by a header packet indicating
 * file size.
 */
void StarSystemTest::TransmitFile_Split()
{
    /* declare variables for test */
    Channel txChannel;
    unsigned long chunkSize, dataPacketCount = 0;
    U8 *pTxBuffer = NULL;
    TransmitOperation *pTransmitOp = NULL;
    StreamItem **packets = NULL;
    Packet *pPacket = NULL;
    TransferOperation::TransferStatus txStatus;
    Address *pAddress = NULL;
    char sFile[256], sChunkSize[256];
    int status, result;
    int fileSize = 0;
    U32 fileSizeToSend = 0;
    unsigned int offset = 0;
    size_t sFileLen;
    unsigned int sizeOfFinalPacket;

    /* select the transmit device and channel to be used */
    if (!ChooseDeviceAndChannel("transmit", txChannel,
        Channel::DIRECTION_OUT))
    {
        /* end processing if no valid channel obtained */
        return;
    }

    /* get the address to transmit the packets to */
    pAddress = GetTransmitPathAddress();

    /* request the name of the file to read packet data from */
    printf("Enter name of file to transmit: ");

    /* if failed to read file name */
    if (!fgets(sFile, 256, stdin))
    {
        /* report error, end processing */
        puts("ERROR: Invalid Input");
        return;
    }

    /* remove newline from filename read in, replace with NULL terminator */
    sFileLen = strlen(sFile);
    if (sFile[sFileLen - 1] == '\n')
    {
        sFile[sFileLen - 1] = '\0';
    }

    /* confirm details with user and whether they wish to proceed */
    printf("Filename = \"%s\"\n", sFile);
    printf("Continue with these values ? (Y/N): ");

    /* if user doesn't wish to proceed */
    if (!ConfirmYes())
    {
        /* report error, jump to end of process */
        printf("File Transfer aborted\n");
        goto Transmit_File_Packets_Finish;
    }

    /* get size of file to read data from */
    fileSize = SizeOfFile(sFile);

    /* if invalid file size */
    if (fileSize <=0)
    {
        /* report error, jump to end of process */
        printf("File size invalid, aborting transfer.\n");
        goto Transmit_File_Packets_Finish;
    }

    /* allocate memory to hold data read from the file */
    pTxBuffer = (U8 *)malloc((size_t)fileSize);

    /* if failed to allocate memory */
    if(!pTxBuffer)
    {
        /* report error, jump to end of process */
        puts("ERROR: Could not allocate buffer for file");
        goto Transmit_File_Packets_Finish;
    }

    /* read data from file into buffer allocated */
    result = ReadFromFileIntoBuffer(sFile, (char*)pTxBuffer, 0, 
        (unsigned int)fileSize);

    /* if file read failed */
    if (result == 0)
    {
        /* report error, jump to end of process */
        puts("ERROR: Could not read file");
        goto Transmit_File_Packets_Finish;
    }

    /* get the size of packets to split the data from the file into */
    printf("Enter maximum size of packets to transmit, in bytes: ");
    if (!fgets(sChunkSize, 256, stdin))
    {
        /* report error, jump to end of process */
        puts("ERROR: Invalid input");
        goto Transmit_File_Packets_Finish;
    }
    status = sscanf(sChunkSize, "%lu", &chunkSize);
    if(!status)
    {
        /* report error, jump to end of process */
        puts("ERROR: Invalid input");
        goto Transmit_File_Packets_Finish;
    }

    /* calculate the number of packets to send */
    dataPacketCount = (unsigned long)((unsigned long)fileSize/chunkSize + 
        ((unsigned long)fileSize % chunkSize > 0 ? 1 : 0));

    /* create an array of pointers to StreamItems (to be filled with
     * Packets later) for the number of packets required.
     *
     * NOTE: 1 extra packet is sent as a header.
     */
    packets = CreateArrayOfPointersToStreamItems((int)(dataPacketCount + 1));

    /* if failed to allocate array of StreamItem pointers */
    if (packets == NULL)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to allocate memory for transmit stream items");
        goto Transmit_File_Packets_Finish;
    }
    else
    {
        /* initialise the pointers in the array to NULL */
        for (unsigned long i=0; i<=dataPacketCount; i++)
            packets[i] = NULL;
    }

    /* copy value of file size into an equivalent four byte variable,
     * ensuring that the most significant bit is in the first byte
     */
    CopyNumberToMemory(&fileSizeToSend, (U32)fileSize, sizeof(U32));

    /* create header packet (i.e. first packet to send) containing the
     * file size
     */
    pPacket = CreatePacket(pAddress, (U8*)&fileSizeToSend, sizeof(U32),
                           Packet::EOP_TYPE_EOP);

    /* if failed to create packet */
    if (pPacket == NULL)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to create header packet");
        goto Transmit_File_Packets_Finish;
    }
    else
    {
        /* add packet to head of array of StreamItems */
        packets[0] = pPacket;
    }

    /* for each remaining chunk of file data to send */
    for (unsigned long i = 1; i < dataPacketCount; i++)
    {
        /* create a new packet for the next data chunk */
        pPacket = CreatePacket(pAddress, pTxBuffer + offset,
                               chunkSize, Packet::EOP_TYPE_EOP);

        /* if failed to create packet */
        if (pPacket == NULL)
        {
            /* report error, jump to end of process */
            printf("\nERROR: Unable to create packet %lu to be transmitted", i);
            goto Transmit_File_Packets_Finish;
        }
        else
        {
            /* otherwise, add packet to array of StreamItems */
            packets[i] = pPacket;
        }

        /* increase the offset which forms an index to the start of the next
         * chunk of data to read from in the buffer containing the data read
         * from the file
         */
        offset += chunkSize;
    }

    /* calculate size of final data packet, elaborated below: */
    sizeOfFinalPacket = 0;

    /* if the file size couldn't be split into an exact number of chunks
     * without leaving a remainder
     */
    if(((unsigned long)fileSize % chunkSize > 0 ? 1 : 0))
    {
        /* set size of final packet to be the remainder amount */
        sizeOfFinalPacket = (unsigned long)fileSize % chunkSize;
    }
    else
    {
        /* set size of final packet to be the chunk size */
        sizeOfFinalPacket = chunkSize;
    }

    /* create the final packet */
    pPacket = CreatePacket(pAddress, pTxBuffer + offset,
                           sizeOfFinalPacket, Packet::EOP_TYPE_EOP);

    /* if failed to create final packet */
    if (pPacket == NULL)
    {
        /* report error, jump to end of process */
        printf("\nERROR: Unable to create final packet to be transmitted");
        goto Transmit_File_Packets_Finish;
    }
    else
    {
        /* otherwise, add packet to the end of the array of StreamItems */
        packets[dataPacketCount] = pPacket;
    }

    /* create transmit operation to submit the packets */
    pTransmitOp = CreateTransmitOperation(packets, (int)dataPacketCount+1);

    /* if failed to create transmit op */
    if (pTransmitOp == NULL)
    {
        /* report error, jump to end of process */
        puts("\nERROR: Unable to create the transfer operation to be"
            " transmitted");
        goto Transmit_File_Packets_Finish;
    }

    /* notify user: starting packet send / transmit */
    puts("Starting File Transmit...");

    /* submit the transmit operation on the transmit channel */
    result = txChannel.SubmitTransferOperation(pTransmitOp);

    /* if failed to submit transmit op */
    if (result == 0)
    {
        /* report error, jump to end of process */
        puts("ERROR: Could not submit transmit operation");
        goto Transmit_File_Packets_Finish;
    }

    /* wait on the transmit operation completing */
    txStatus = pTransmitOp->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

    /* if transmit operation failed to complete */
    if(txStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        /* report error, jump to end of process */
        printf("ERROR: Error %d occurred during transmit\n", txStatus);
        goto Transmit_File_Packets_Finish;
    }

    /* notify user: packet send/transmit complete */
    puts("Complete.");

/* end of transmit, clean up resources */
Transmit_File_Packets_Finish:

    /* dispose of the transfer operation */
    if (pTransmitOp != NULL)
    {
        delete pTransmitOp;
    }

    /* destroy the packets transmitted */
    if (packets != NULL)
    {
        /* for each item in the array of StreamItem pointers */
        for (unsigned long i = 0; i <= dataPacketCount; i++)
        {
            /* if the item pointed to is valid (non-NULL) */
            if (packets[i] != NULL)
            {
                /* delete the item pointed to */
                delete packets[i];
            }
        }

        /* delete the array itself */
        delete [] packets;
    }

    /* free the transmit buffer */
    if (pTxBuffer != NULL)
    {
        free(pTxBuffer);
    }

    /* free the SpaceWire Address */
    if (pAddress != NULL)
    {
        delete pAddress;
    }

    /* close the channel */
    txChannel.CloseChannel();
}


/**
 * Copy a numerical value into a buffer, making sure that the MSB is in the
 * first byte of the buffer.
 *
 * @param pBuffer the buffer to copy the number in to
 * @param number the number to copy in to the buffer
 * @param len the number of bytes to be used to represent the number in the
 *            buffer
 */
void StarSystemTest::CopyNumberToMemory(void *pBuffer, U32 number,
                                        unsigned long len)
{
    unsigned long i;
    for (i = 0; i < len; i++)
    {
        ((U8 *)pBuffer)[i] = (U8)((number >> ((len - i - 1) * 8)) & 0xff);
    }
}

/**
 * Receives a file which has been transmitted as multiple packets preceded by
 * a header packet indicating file size
 */
void StarSystemTest::ReceiveFile_Split()
{
    /* declare variables for test */
    Channel rxChannel;
    ReceiveOperation *pReceiveOp = NULL;
    TransferOperation::TransferStatus rxStatus;
    char sFile[256];
    char writeStatus;
    StreamItem *pStreamItem = NULL;
    Packet *pPacket = NULL;
    size_t sFileLen = 0;
    clock_t start, finish;
    U8* receivedData = NULL;
    unsigned int receivedDataLen = 0;
    unsigned int expectedFileSize = 0;
    unsigned int actualFileSize = 0;
    int result;

    /* select the receive device and channel to be used */
    if (!ChooseDeviceAndChannel("receive", rxChannel,
        Channel::DIRECTION_IN))
    {
        /* jump to end of process if no valid channel obtained */
        goto Receive_File_Split_Finish;
    }

    /* request the name of a file to store received packets into */
    printf("Enter name of file to write data to: ");

    /* if failed to read file name */
    if (!fgets(sFile, 256, stdin))
    {
        /* report error, end process */
        puts("ERROR: Invalid input");
        return;
    }

    /* strip newline from filename, replace with NULL terminator */
    sFileLen = strlen(sFile);
    if (sFile[sFileLen - 1] == '\n')
    {
        sFile[sFileLen - 1] = '\0';
    }

    /* confirm details with user and whether they wish to proceed */
    printf("Filename = \"%s\"\n", sFile);
    printf("Continue with these values ? (Y/N): ");

    /* if the user doesn't wish to proceed */
    if (!ConfirmYes())
    {
        /* display confirmation, jump to end of process */
        puts("File Transfer aborted");
        goto Receive_File_Split_Finish;
    }

    /* notify user: starting file receive */
    printf("Running File Receive...");

    /* create a receive operation to receive the Header packet only */
    pReceiveOp = CreateReceiveOperation(1, ReceiveOperation::RECEIVE_PACKETS);

    /* if receive operation failed to create */
    if (pReceiveOp == NULL)
    {
        /* report error, jump to end of process */
        puts("ERROR: Unable to create receive operation");
        goto Receive_File_Split_Finish;
    }

    /* submit the receive operation on the receive channel */
    result = rxChannel.SubmitTransferOperation(pReceiveOp);

    /* if failed to submit receive op */
    if (result == 0)
    {
        /* report error, jump to end of process */
        puts("ERROR: Could not submit receive operation");
        goto Receive_File_Split_Finish;
    }

    /* wait on the receive operation completing */
    rxStatus = pReceiveOp->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

    /* if receive op failed to complete */
    if (rxStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        /* report error, jump to end of process */
        printf("ERROR: Error %d occurred during receive\n", rxStatus);
        goto Receive_File_Split_Finish;
    }

    /* get received data, which should be a packet and which will be the
     * first transfer item in the Receive Operation (zero-based index of 0)
     */

    /* get the stream item received */
    pStreamItem = pReceiveOp->GetTransferItem(0);

    /* if valid stream item obtained */
    if (pStreamItem != NULL)
    {
        /* if it is a packet stream item */
        if (pStreamItem->GetStreamItemType() == 
            StreamItem::STREAM_ITEM_TYPE_PACKET)
        {
            /* cast it to a packet */
            pPacket = (Packet*)pStreamItem;
        }
        else
        {
            /* an unexpected stream item type */

            /* report error */
            puts("Unexpected stream item type received!");

            /* free the stream item */
            delete pStreamItem;
            pStreamItem = NULL;
        }
    }

    /* if failed to obtain header packet from Receive Operation */
    if (pPacket == NULL)
    {
        /* report error, jump to end of process */
        printf("ERROR: Failed to retreive packet from Receive operation\n");
        goto Receive_File_Split_Finish;
    }

    /* get the packet's data, and the length of the data.
     *
     * \note 'GetPacketData()' returns a pointer to newly allocated memory
     * containing the packet's data as an array of bytes. This memory must be
     * freed later by calling the 'DestroyPacketData()' method on the packet.
     */
    receivedData = pPacket->GetPacketData(&receivedDataLen);

    /* if failed to get the packet's data */
    if (receivedData == NULL)
    {
        /* report error, jump to end of process */
        printf("ERROR: Failed to retreive packet's data\n");
        goto Receive_File_Split_Finish;
    }

    /* transform the packet data (which contains a number in the form of
     * an array of bytes) into its numerical equivalent
     */
    CopyNumberFromMemory(&expectedFileSize, receivedData,
                         sizeof(expectedFileSize));

    /* free the packet data obtained */
    pPacket->DestroyPacketData(receivedData);
    receivedData = NULL;

    /* free the packet itself */
    delete pPacket;
    pPacket = NULL;

    /* get the start time for extracting packets containing the file data */
    start = GET_TIME();

    /* read a packet at a time until we get all expected data */

    /* while still packets to processs */
    while(expectedFileSize > actualFileSize)
    {
        /* submit receive operation again, to receive another 1 packet */
        result = rxChannel.SubmitTransferOperation(pReceiveOp);

        /* if receive op failed to submit */
        if (result == 0)
        {
            /* report error, jump to end of process */
            puts("ERROR: Could not submit receive operation");
            goto Receive_File_Split_Finish;
        }

        /* wait for receive op to complete */
        rxStatus = pReceiveOp->WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

        /* if receive op failed to complete */
        if (rxStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
        {
            /* report error, jump to end of process */
            printf("ERROR: Error %d occurred during receive\n", rxStatus);
            goto Receive_File_Split_Finish;
        }

        /* get the next packet from the receive operation */
        /* \note You may wish to perform error checking to determine whether the
         * stream item retreived from the receive operation actually is a packet
         * or not before attempting to cast it to a 'Packet' object as below.
         * Please see handling of the initial header packet earlier.
         */
        pPacket = (Packet*)pReceiveOp->GetTransferItem(0);

        /* if failed to obtain next packet from Receive Operation */
        if (pPacket == NULL)
        {
            /* report error, jump to end of process */
            printf("ERROR: Failed to retreive packet from Receive operation\n");
            goto Receive_File_Split_Finish;
        }

        /* get the packet's data, and the length of the data.*/
        receivedData = pPacket->GetPacketData(&receivedDataLen);

        /* if failed to get the packet's data */
        if (receivedData == NULL)
        {
            /* report error, jump to end of process */
            printf("ERROR: Failed to retreive packet's data\n");
            goto Receive_File_Split_Finish;
        }

        /* append packet data to file specified */
        writeStatus = (char)WriteBufferToFile(receivedData, 
            (long)receivedDataLen, sFile);

        /* free the packet data obtained */
        pPacket->DestroyPacketData(receivedData);
        receivedData = NULL;

        /* free the packet itself */
        delete pPacket;
        pPacket = NULL;

        /* if error occurred writing to file */
        if(writeStatus == 0)
        {
            puts("ERROR: Could not write to file");
            goto Receive_File_Split_Finish;
        }

        /* increment the size of data received thus far */
        actualFileSize += receivedDataLen;
    }

    /* if size of data received isn't as expected */
    if(actualFileSize != expectedFileSize)
    {
        /* report error */
        puts("ERROR: Expected and actual file size do not match");
    }

    /* get end time for process */
    finish = GET_TIME();

    /* display the test results */
    DisplayResults(start, finish, 0, actualFileSize, 1, 1, 0,
        "Receive File from multiple packets");



Receive_File_Split_Finish:

    /* free packet retreived if required (may be required if an error
     * occurred prior to a packet being destroyed)
     */
    if (pPacket != NULL)
    {
        delete pPacket;
    }

    /* dispose of the transfer operation */
    if (pReceiveOp != NULL)
    {
        delete pReceiveOp;
    }

    /* close the channel */
    rxChannel.CloseChannel();
}

/**
 * Copy a numerical value from a buffer, reading the MSB from the first byte
 * of the buffer.
 *
 * @param pNumber a pointer to a variable which will be updated to contain
 *                the number read from the buffer
 * @param pBuffer the buffer to read the number from
 * @param len the number of bytes used to represent the number in the buffer
 */
void StarSystemTest::CopyNumberFromMemory(U32 *pNumber, void *pBuffer, 
    unsigned long len)
{
    unsigned long i;
    *pNumber = 0;
    for (i = 0; i < len; i++)
    {
        *pNumber = (*pNumber << 8) + ((U8 *)pBuffer)[i];
    }
}

/**
 * Get the current clock time.
 */
clock_t StarSystemTest::GET_TIME()
{
#ifdef _WIN32
    return clock();
#else
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return tv.tv_sec * 1000000 + tv.tv_usec;
#endif
}

