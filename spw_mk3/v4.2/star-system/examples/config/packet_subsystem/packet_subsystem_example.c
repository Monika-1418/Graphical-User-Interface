/**
 * \file packet_subsystem_example.c
 *
 * \brief Example showing usage of the PCI Mk2 Packet
 *        subsystem API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */

#include "packet_subsystem.h"
#include "cfg_api_generic.h"
#include "ui.h"
#include <stdlib.h>
#include <stdio.h>

#define VERSION_INFO "PCI Mk2 Packet Subsystem Test Application"

/* Currently the PCI Mk2 only contains a single packet subsystem on port 8*/
#define PACKET_SUBSYSTEM_NUMBER 1
#define SUBSYTEM_PORT_START 7

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#if !defined(UNREFERENCED_PARAMETER)
    #define UNREFERENCED_PARAMETER(a)   ((void)(a))
#endif


void printVersionInformation()
{
    puts(VERSION_INFO);
    puts("Copyright (C) 2012 STAR-Dundee Ltd.");
    puts("http://www.star-dundee.com/ \n");
}

int runTest(STAR_DEVICE_ID generatorDevice,
            STAR_DEVICE_ID checkerDevice)
{
    /* Buffer that will be filled with bytes to be used for generated packet headers */
    U8 *pHeaderBuffer;

    /* Number of header bytes to use */
    U16 headerBytes = 2;
    /* Determine how many words header bytes occupy */
    U16 headerWords = (headerBytes + 3) / 4;

    /* Sizes for expected headerdata used for checker*/
    U16 expectedHeaderWords = 0;

    /* Buffer that will be filled with bytes to be used for generated packet bodies */
    U8 *pBodyBuffer;

    /* Number of 4 byte words to use when generating packet bodies */
    U16 bodyWords = 12;

    /* Size of packets to be generated. Generated packets will be made up of the specified
       header  bytes, followed by the body bytes specified repeating as necessary to make
       the packet up to the length specified.*/
    U16 packetLen = 100 + headerBytes;

    /* Size of expected packets received by the packet checker. Header deletion by the router
       will strip the path address. */
    U16 expectedPacketLen = packetLen - headerBytes;

    /* Here we decide on the the physical location in the device dual port memory to use
       for the packet generator and checker information, and for the sink's receive buffer.
       These locations and the order we specify them are arbitrary. */

    /* We decide to put the header data at address 0 */
    U16 memHeaderGenStart = 0;

    /* We put the body data after the end of the header data */
    U16 memBodyGenStart = memHeaderGenStart + headerWords;

    /* Put the checker memory after the end of the generator data */
    U16 memHeaderCheckerStart = memBodyGenStart + bodyWords;
    U16 memBodyCheckerStart = memHeaderCheckerStart + expectedHeaderWords;

    /* Put the sink buffer after the checker memory */
    U16 memSinkStart = memBodyCheckerStart + bodyWords;

    U16 sinkLenWords;

    /* Number of bytes to wait before turning off sink after detecting a character mismatch */
    U16 sinkDelayOnMismatch = 16;

     /* Count of byte mismatches caught by the packet checker */
    U64 mismatchCount = 0;

    /* Circular buffer pointers used when reading data back from the packet sink */
    U16 startPointer = 0 , endPointer = 0;

    /* Buffer that will be filled  with data read back from the packet sink*/
    U8* pReadBackBuffer;

    /* Values used when displaying results*/
    unsigned int offset= 0, address = 0;

    /* Create local buffers which we will copy to the devices on board memory */
    pHeaderBuffer = (U8*)malloc(headerWords * 4);
    if(!pHeaderBuffer)
    {
        return 0;
    }
    pBodyBuffer   = (U8*)malloc(bodyWords * 4);
    if(!pBodyBuffer)
    {
        free(pHeaderBuffer);
        return 0;
    }

    printVersionInformation();

    /* In this example (with a cable looped back from port 1 to port 3) we want the packet
       to have a path address of [1, 8]. 1 to send packet out of port one, 8 to receive the
       packet back at the packet subsytstem.*/
    *pHeaderBuffer       = 1;
    *(pHeaderBuffer + 1) = SUBSYTEM_PORT_START + PACKET_SUBSYSTEM_NUMBER;

    /* Fill the packet body with a repeating pattern*/
    PKT_SUBSYS_fillBufferWithPattern(PKT_SUBSYS_PATTERN_ALTERNATING_BITS, bodyWords * 4, pBodyBuffer);

    /* Write packet header and body to device memory */
    PKT_SUBSYS_writeMemory(generatorDevice, memHeaderGenStart, headerWords * 4, pHeaderBuffer);
    PKT_SUBSYS_writeMemory(generatorDevice, memBodyGenStart, bodyWords * 4, pBodyBuffer);
    free(pHeaderBuffer);
    pHeaderBuffer = NULL;

    /* Write expected packet body to checker device memory. As we don't expect the packet body to change
       we can just use the same buffer as before.
       Note that header deletion will strip the path address, so we will not expect to see a header*/
    PKT_SUBSYS_writeMemory(checkerDevice, memBodyCheckerStart, bodyWords * 4, pBodyBuffer);
    free(pBodyBuffer);
    pBodyBuffer = NULL;

    /* Instruct subsystem A packet generator to use supplied buffers */
    PKT_SUBSYS_setPacketGeneratorFormat(generatorDevice,
        PACKET_SUBSYSTEM_NUMBER,
        memHeaderGenStart,
        headerBytes,
        memBodyGenStart,
        bodyWords,
        packetLen,
        STAR_EOP_TYPE_EOP,
        0);

    /* Set up a packet sink on subsytem B to receive generated packets:
       We set up circular buffer with room for 20 packets,
       ie sink size in words  = ((20 packets * (packetLen in bytes + 2 bytes for  end of packet marker) + 3) / 4
       Note that 3 is added to ensure there are enough words if (packet length + EOP) is not a multiple of 4
     */

    sinkLenWords = ((20 * (expectedPacketLen+2)) + 3) / 4;
    PKT_SUBSYS_setPacketSinkParameters(checkerDevice, PACKET_SUBSYSTEM_NUMBER, memSinkStart, sinkLenWords);

    /* Set up the packet checker on subsytem B to check the received generated packets
       Here we just re-use the buffers already written to device to generate packets from.
       As we do not expect a packet header, we set its length to 0.*/
    PKT_SUBSYS_setPacketCheckingFormat(checkerDevice,
        PACKET_SUBSYSTEM_NUMBER,
        memHeaderCheckerStart,
        0,
        memBodyCheckerStart,
        bodyWords,
        expectedPacketLen,
        STAR_EOP_TYPE_EOP);


    /* Initiate packet generation and sinking run */
    PKT_SUBSYS_startSink(checkerDevice, PACKET_SUBSYSTEM_NUMBER);
    /* Start the packet checker, disable the the packet sink after a short delay on receiving a mismatched character */
    PKT_SUBSYS_startPacketChecking(checkerDevice, PACKET_SUBSYSTEM_NUMBER, 1, sinkDelayOnMismatch);
    /* Kick off packet generation of 10 packets */
    PKT_SUBSYS_startPacketGeneration(generatorDevice, PACKET_SUBSYSTEM_NUMBER, 10);
    /* Wait for packet generation to complete */
    PKT_SUBSYS_waitForPacketGenerationSequenceComplete(generatorDevice, PACKET_SUBSYSTEM_NUMBER, 0);
    /* Get a count of the number of received bytes that did not match the expected pattern */
    PKT_SUBSYS_getPacketCheckingMismatchCount(checkerDevice, PACKET_SUBSYSTEM_NUMBER, &mismatchCount);
    /* Stop packet checking*/
    PKT_SUBSYS_stopPacketChecking(checkerDevice, PACKET_SUBSYSTEM_NUMBER);
    PKT_SUBSYS_stopSink(checkerDevice, PACKET_SUBSYSTEM_NUMBER);


    printf("Mismatch count: %llu\n", mismatchCount);

    /* Display sink contents if we received a mismatched character */
    if(mismatchCount)
    {
        /* Read back the packet sink data from the device */
        PKT_SUBSYS_getSinkDataPointers(checkerDevice, PACKET_SUBSYSTEM_NUMBER, &startPointer, &endPointer);
        if(endPointer < startPointer)
        {
            /* wraparound */
            pReadBackBuffer = (U8*)calloc(sinkLenWords, sizeof(U32));
            if(!pReadBackBuffer)
            {
                return 0;
            }

            PKT_SUBSYS_readMemory(checkerDevice, startPointer, ((memSinkStart+sinkLenWords) - startPointer) * sizeof(U32), pReadBackBuffer);
            PKT_SUBSYS_readMemory(checkerDevice, memSinkStart, (endPointer - memSinkStart) * sizeof(U32), pReadBackBuffer + ((memSinkStart+sinkLenWords) - endPointer) * sizeof(U32));
        }
        else
        {
            /* no wraparound */
            sinkLenWords = endPointer - startPointer;
            pReadBackBuffer = (U8*)calloc(sinkLenWords, sizeof(U32));
            if(!pReadBackBuffer)
            {
                return 0;
            }
            PKT_SUBSYS_readMemory(checkerDevice, startPointer, (endPointer - startPointer) * sizeof(U32), pReadBackBuffer);
        }


        /* Display all data received after the packet sink was stopped automatically on
           character mismatch. Note that if the packet sink was stopped manually before this
           delay was reached, some displayed data will be from before the first mismatch occurred. */

        if((sinkDelayOnMismatch+3)/4 >= sinkLenWords)
        {
            /* sink disable delay is larger than sink; display whole sink */
            offset = address = 0;
        }
        else
        {
            /* display only last (sink delay words + mismatch word) of sink */
            offset  = (sinkLenWords * 4 - sinkDelayOnMismatch) - 4;
            address = (sinkLenWords - (sinkDelayOnMismatch+3)/4) - 1;
        }

        printf("Packet Sink contents (Last %d words):\n", MIN(sinkLenWords, ((sinkDelayOnMismatch+3)/4) + 1));

        for(; offset <= sinkLenWords * sizeof(U32) - 4; offset += 4)
        {
            printf(" %-5d |  %02x %02x %02x %02x\n", address++,
                ((U8 *)pReadBackBuffer)[offset], ((U8 *)pReadBackBuffer)[offset + 1],
                ((U8 *)pReadBackBuffer)[offset + 2], ((U8 *)pReadBackBuffer)[offset + 3]);
        }

        free(pReadBackBuffer);
        pReadBackBuffer = NULL;

    }

    return 0;
}

void STAR_API_CC displayStats(
    STAR_DEVICE_ID deviceIdentifier,
    unsigned int subsystem,
    PKT_SUBSYS_STATISTICS statistics)
{
    UNREFERENCED_PARAMETER(deviceIdentifier);
    UNREFERENCED_PARAMETER(subsystem);

    printf("Data Character Rate: %u/s \n", statistics.dataCharacterRate);
    printf("Data Characters Received: %llu \n", statistics.dataCharactersReceived);
    printf("EEP Character Rate: %u/s \n", statistics.eepCharacterRate);
    printf("EEP Characters Received: %llu \n", statistics.eepCharactersReceived);
    printf("EOP Character Rate: %u/s \n", statistics.eopCharacterRate);
    printf("EOP Characters Received: %llu \n", statistics.eopCharactersReceived);
    puts("Press enter to stop");
}

void showStatistics(STAR_DEVICE_ID deviceId)
{
    char string [256];
    STATISTICS_LOOP_ID id;

    /* Enable packet sink */
    PKT_SUBSYS_startSink(deviceId, PACKET_SUBSYSTEM_NUMBER);

    puts("Ensure the device is not running in interface mode.");
    puts("Press enter to start:");
    fgets(string, 256, stdin);

    /* Begin polling statistics */
    id = PKT_SUBSYS_startGetStatisticsLoop(deviceId, PACKET_SUBSYSTEM_NUMBER, displayStats);
    if(!id)
    {
        puts("Could not start statistics loop");
        return;
    }
    puts("Press enter to stop:");
    fgets(string, 256, stdin);


    /* End loop, disable packet sink again */
    PKT_SUBSYS_stopGetStatisticsLoop(id);
    PKT_SUBSYS_stopSink(deviceId, PACKET_SUBSYSTEM_NUMBER);
}

/**
 * Displays usage for command line arguments.
 *
 * @param[in] argv   Array of arguments passed to the program
 * @param     error  Whether to send usage info to stderr (1) or stdout (0)
 */
void usage(char *argv[], int error)
{
    puts("");

    if(error)
         fprintf(stderr, "usage: %s [-v][-s][-help]\n", argv[0]);
    else
         fprintf(stdout, "usage: %s [-v][-s][-help]\n", argv[0]);
}

/**
 * Displays program description and describes optional
 * command line arguments.
 */
void showHelp(void)
{
    puts("");
    puts("Description:");
    puts("  A program to demonstrate usage of the PCI Mk2");
    puts("  packet subsystem API. Sets up a packet generator");
    puts("  and checker, starts them, then displays results.");
    puts("");
    puts("  Note that interface mode will be disabled on the");
    puts("  the device(s) chosen.");
    puts("");
    puts("Optional Switches:");
    puts("");
    puts("  -v       Displays version information and exits.");
    puts("");
    puts("  -help    Displays this help message and exits.");
    puts("");
    puts("  -s       Polls and displays device statistics until");
    puts("           user hits enter.");
    puts("");
    puts(" When called without arguments, the program sets up a packet");
    puts(" generator a chosen device to send 10 102 byte packets,");
    puts(" addressed to the packet checking subsystem on a second chosen");
    puts(" device. The packets are built from 2 address bytes");
    puts(" (1, then subsystem port) followed by 100 bytes of a repeating");
    puts(" alternating bits pattern.");
}

/*Set up a packet generator on subsystem one to send packets to a sink on subsystem 2*/
int __cdecl main(int argc, char* argv[])
{
    int i;
    int version = 0;  /* Value for the "-v" optional argument. */
    int help = 0;     /* Value for the "-help" optional argument. */
    int statistics = 0;     /* Value for the "-s" optional argument. */
    STAR_DEVICE_ID deviceA, deviceB;

    STAR_DEVICE_TYPE aDeviceTypes[1];
    aDeviceTypes[0] = STAR_DEVICE_PCI_MK2;

    /* If no args provided, run with default settings */
    if (argc <= 1)
    {
        puts("Select device to run packet generator on:");
        deviceA = STAR_UI_chooseDevice(aDeviceTypes,1);

        puts("Select device to run packet checker on:");
        deviceB = STAR_UI_chooseDevice(aDeviceTypes,1);

        if (!deviceA || !deviceB)
        {
            return 1;
        }

        CFG_disableInterfaceMode(deviceB);

        runTest(deviceA, deviceB);
        return 0;
    }

    /* Currently we only expect 1 optional argument */
    if(argc > 2)
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
        else if(strcmp(argv[i], "-help") == 0)
        {
            help = 1;
        }
        else if(strcmp(argv[i], "-s") == 0)
        {
            statistics = 1;
        }
    }

    if(help)
    {
        usage(argv, 0);
        showHelp();
    }
    else if(version)
    {
        printVersionInformation();
    }
    else if (statistics)
    {
        puts("Select device:");
        //deviceA = chooseDevice();
        deviceA = STAR_UI_chooseDevice(aDeviceTypes,1);

        CFG_disableInterfaceMode(deviceA);

        if(!deviceA)
        {
           return 1;
        }

        showStatistics(deviceA);
    }
    else
    {
        usage(argv, 1);
        return 1;
    }

    return 0;
}
