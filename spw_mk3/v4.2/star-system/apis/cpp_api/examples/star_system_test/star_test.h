/**
 * \file star_test.h
 *
 * \brief Declarations for STAR-System Test program.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Contains the declarations used by the Star-System Test program.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "star-dundee_types.h"
#include "star_system.hpp"
#include "version_information.hpp"
#include "receive_operation.hpp"
#include "transmit_operation.hpp"

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::transferoperations;

#ifndef STAR_TEST_H
#define STAR_TEST_H

/* Macros used to time operations, specific to individual operating systems */
#ifdef _WIN32
    #include <time.h>
    #define TIME_DIVIDER    CLOCKS_PER_SEC
#else
    #include <sys/time.h>
    #define TIME_DIVIDER    1000000
#endif

/* version of Star System Test Program */
#define VERSION_INFO "STAR-System Test - C++ - v1.0"

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
#define MENU_EXIT                           0

/* data types required for 'Fill buffer' operations */
#define DATA_TYPE_0             0
#define DATA_TYPE_1             1
#define DATA_TYPE_RANDOM        2
#define DATA_TYPE_COUNT         3
#define DATA_TYPE_NOT_COUNT     4

/**
 * Contains a range of functions to allow testing of various aspects of the
 * STAR-System API. In particular, sending and receiving of packets.
 */
class StarSystemTest
{
public:

    /**
     * Run the main test program, providing options for the user to select
     * from to test different aspects of the STAR-System API.
     */
    void RunTestProgram();

    /**
     * Display the main options menu for the Star System Test Program.
     */
    void DisplayMenu();

    /**
     * Tests the SpaceWire device using a loop back test.  Packets of random 
     * data are transmitted on one link and received on a second link.
     *
     * This test is timed so as to test link speed. Optionally, the data is 
     * then compared to check for errors.
     */
    void LoopBack_SinglePacket(void);

    /**
     * Prompts the user to choose which device and channel to use for the test,
     * if multiple devices and/or channels are available.
     *
     * @param descriptionStr the description of what the device is to be used 
     *                       for (e.g. transmit or receive)
     * @param selectedChannel A reference to a Channel object which will be set
     *                        with information for the selected, opened channel.
     * @param direction the direction in which traffic is to be sent over the
     *                  channel
     *
     * @return whether a device and a channel was successfully selected
     */
    int ChooseDeviceAndChannel(const char *descriptionStr,
                               Channel& selectedChannel,
                               Channel::ChannelDirection direction);

    /**
     * Prompts the user to choose which device to use for the test if more than
     * one device is available.
     *
     * @param descriptionStr the description of what the device is to be used 
     *                       for (e.g. transmit or receive)
     *
     * @return A pointer to a Device object representing the selected
     *          device, or NULL if no devices are available or an error 
     *          occurred during processing.
     *
     * \note The object returned must be freed when no longer required by using
     *       the C++ delete operator.
     */
    Device* ChooseDevice(const char *descriptionStr);

    /**
    * Prompts the user to choose which channel on a specified device to use for
    * the test if more than one channel is available.
    *
    * @param descriptionStr the description of what the device is to be used 
    *                       for (e.g. transmit or receive)
    * @param device A Device from which the channel will be selected.
    *
    * @return the number of the channel to be used for the test, or 0 if there
    *         was an error or no channels are present on the device
    */
    unsigned char ChooseChannel(const char *descriptionStr, Device* device);

    /**
     * Prompts the user to choose whether to check the received data.
     *
     * @param pCompare a pointer to a variable which will be updated to 
     *                 indicate whether the data should be compared
     *
     * @return whether the user selected a valid option
     */
    int ChooseCheckData(int *pCompare);

    /**
     * Prompts the user to specify the packet size to be used for a test.
     *
     * @param pPacketSize a pointer to a variable which will be updated to
     *                    contain the packet size specified
     *
     * @return whether the user selected a valid packet size
     */
    int GetPacketSize(unsigned long *pPacketSize);

    /**
     * Prompts the user to specify the number of times to run a test.
     *
     * @param pLoopCount a pointer to a variable which will be updated to 
     *                   contain the specified value for the number of times 
     *                   the test should be run
     *
     * @return whether the user selected a valid number of loops
     */
    int GetLoopCount(unsigned long *pLoopCount);

    /**
     * Prompts the user to specify the number of packets to transmit and 
     * receive in a test.
     *
     * @param pPacketCount a pointer to a variable which will be updated to 
     *                     contain the specified value for the number of 
     *                     packets to transmit and receive in the test
     *
     * @return whether the user selected a valid number of packets
     */
    int GetPacketCount(unsigned long *pPacketCount);



    /**
     * Fills a buffer with different random data values.
     *
     * @param pBuffer A pointer to the buffer to fill.
     * @param size The length of the buffer.
     * @param dataType The type of data to fill the buffer with, e.g. zeros,
     *                 ones, random chars, etc.
     */
    void FillBufferRandomChar(char *pBuffer, unsigned int size, int dataType);

    /**
     * Generates a 32-bit random number.
     *
     * @return A random, 32-bit number.
     */
    unsigned int random32(void);

    /**
     * Generates a 16-bit random number.
     *
     * @return A random, 16-bit number.
     */
    unsigned int random16(void);

    /**
     * Create and return a pointer to a new ReceiveOperation.
     *
     * @param itemCount The maximum number of stream items to receive. This can
     *                  be -1 to receive an unlimited number of items. The size
     *                  of an individual stream item is not limited
     * \note If a receive operation receiving an unlimited number of items is 
     *       not cancelled by the user, then the operation will eventually fail 
     *       to allocate new memory for new incoming data, failing the 
     *       operation.
     * @param mask A bitmask with flags set for the type of traffic one wishes 
     *             to receive.
     *
     * @return A pointer to a new ReceiveOperation object, or NULL.
     */
    ReceiveOperation *CreateReceiveOperation(int itemCount,
                                          ReceiveOperation::ReceiveMask mask);


    /**
     * Create and return a pointer to a new Packet.
     *
     * @param[in] pAddress Optional pointer to a SpaceWire address.
     * @param[in] pData Optional pointer to data buffer.
     * \note The contents of this buffer are copied into data structures 
     *       managed by the API. It is safe to dispose of this buffer after 
     *       this function completes.
     * @param dataLen The length of the data buffer.
     * @param eopType End of packet marker type for the packet (may be none)
     *
     * @return A pointer to a new Packet object, or NULL.
     */
    Packet *CreatePacket(Address *pAddress,
                         unsigned char *pData,
                         unsigned int dataLen,
                         Packet::EopType eopType);

    /**
     * Create and return a pointer to a new TransmitOperation.
     *
     * @param ppStreamItems An array of pointers to StreamItem objects to be 
     *                      sent.
     * \note It is safe to free this array after the function is completed.
     * @param streamItemCount The number of StreamItems in the array.
     *
     * @return A pointer to a new TransmitOperation object, or NULL.
     */
    TransmitOperation *CreateTransmitOperation(StreamItem** ppStreamItems,
                                               int streamItemCount);



    /**
     * Compare the contents of received traffic against a previously created 
     * buffer.
     *
     * @param pReceiveOp The completed receive operation containing the packets 
     *                   to look at.
     * @param packetCount The number of packets which should be in the received
     *                    operation.
     * @param packetSize The size of each packet which should be received.
     * @param pBuffer The buffer that the received packets should be compared
     *                against.
     *
     * @return The number of errors in the received packets.
     */
    unsigned long ComparePackets(ReceiveOperation* pReceiveOp,
                                 unsigned long packetCount,
                                 unsigned long packetSize,
                                 char *pBuffer);

    /**
     * Buffer compare function: Used instead of memcmp() for debugging.
     *
     * @param pBuffer1 Pointer to buffer1, to be compared with buffer2.
     * @param pBuffer2 Pointer to buffer2, to be compared with buffer1.
     * @param size The size or extent of the buffers to compare.
     *
     * @return The number of errors found during the compare.
     */
    unsigned long BufferCompareChar(char *pBuffer1, char *pBuffer2,
                                    unsigned long size);



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
    void DisplayResults(clock_t start, clock_t finish, int compared,
        unsigned long byteSize, unsigned long packetCount, unsigned long loopCount,
        unsigned long errorCount, const char *descriptionStr);

    /**
     * Prompts the user to specify the path address to be added to the front of 
     * the packets to be transmitted for a test.
     *
     * @return the SpaceWire address entered, or NULL if there was an error
     */
    Address* GetTransmitPathAddress();

    /**
     * Transmits multiple packets using a single transfer operation.
     */
    void Transmit_MultiPacket();

    /**
     * Receives a single packet using one transfer operation.
     */
    void Receive_SinglePacket();

    /**
     * Receives multiple packets using a single transfer operation.
     */
    void Receive_MultiPacket();

    /**
     * Gets a yes or no answer from user
     *
     * @return 1 if user input Y, 0 if user input N
     */
    int ConfirmYes();

    /**
     *  Transmits a file whole, as one packet. Not preceded by a header packet.
     */
    void TransmitFile_Whole();

    /**
     * Get and return the size, in bytes, of the specified file.
     *
     * @param filePath The name and path of the file to check.
     *
     * @return The size of the file.
     */
    int SizeOfFile(char filePath[]);

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
    int ReadFromFileIntoBuffer(char fname[], char *pBuffer, long offset,
                                                      unsigned int size);

    /**
     * Receives a file which has been transmitted whole, as one packet without 
     * a size header.
     */
    void ReceiveFile_Whole();

    /**
     * Return a pointer to the index'th transfer item in the given Receive
     * operation as a Packet.
     *
     * @param receiveOp The Receive Operation to query.
     * @param index The zero-based index of the transfer item to obtain from
     *              the receive operation.
     *
     * @return A pointer to a new Packet which represents the transfer
     *         item requested or NULL if the item cannot be retreived and/or
     *         if the item at the specified index isn't a packet stream item.
     *
     * \note This function returns a pointer to a new Packet object which
     *       has been allocated in memory to hold a reference to the index'th 
     *       item within the Receive Operation. The memory for the Packet 
     *       object must be freed when no longer required either by using the 
     *       C++ delete operator or by calling the 'FreeTransferItem()' 
     *       function on the Receive Operation object.
     */
    /*Packet *GetPacketFromReceiveOp(ReceiveOperation *receiveOp,
                                            int index);*/

    /**
     * Write (append) a buffer of data of a specified length to the given file.
     *
     * @param pData The buffer of data to write.
     * @param dataSize The size of the buffer.
     * @param fname The name/path of the file to write to.
     *
     * @return 1 if buffer written to file successfully, otherwise 0.
     */
    int WriteBufferToFile(unsigned char *pData, long dataSize, char fname[]);

    /**
     * Transmits a file as multiple packets preceded by a header packet 
     * indicating file size.
     */
    void TransmitFile_Split();

    /**
     * Copy a numerical value into a buffer, making sure that the MSB is in the
     * first byte of the buffer.
     *
     * @param pBuffer the buffer to copy the number in to
     * @param number the number to copy in to the buffer
     * @param len the number of bytes to be used to represent the number in the
     *            buffer
     */
    void CopyNumberToMemory(void *pBuffer, U32 number, unsigned long len);

    /**
     * Receives a file which has been transmitted as multiple packets preceded by
     * a header packet indicating file size
     */
    void ReceiveFile_Split();

    /**
     * Copy a numerical value from a buffer, reading the MSB from the first 
     * byte of the buffer.
     *
     * @param pNumber a pointer to a variable which will be updated to contain 
     *                the number read from the buffer
     * @param pBuffer the buffer to read the number from
     * @param len the number of bytes used to represent the number in the buffer
     */
    void CopyNumberFromMemory(U32 *pNumber, void *pBuffer,
        unsigned long len);

    /**
     * Resets a device
     */
    void ResetDevice();

    /**
     * Identifies a device by flashing its LEDs
     */
    void IdentifyDevice();

    /**
     * Displays usage for command line arguments.
     *
     * @param[in] argv   Array of arguments passed to the program
     * @param     error  Whether to send usage info to stderr (true) or
     *                   stdout (false)
     */
    void DisplayUsage(_In_ char *argv[], bool error);

    /**
     * Process an optional command line argument which has been
     * provided to the program.
     *
     * @param[in] argv Array of arguments passed to the program
     *
     * @return 0 if argument processed normally, otherwise 1.
     */
    int ProcessCommandLineArgument(_In_ char *argv[]);

    /**
     * Displays version information for the program.
     *
     * @param[in] argv   Array of arguments passed to the program
     *
     */
    void DisplayVersion();

    /**
     * Displays help for the program.
     *
     * @param[in] argv   Array of arguments passed to the program
     */
    void DisplayHelp(_In_ char *argv[]);

    /**
     * Display version information for all the STAR-System API modules and
     * drivers currently in use, and the properties of all the currently
     * connected devices.
     */
    void DisplayAPIVersionAndDeviceInformation();

    /**
     * Display version information for all the STAR-System API modules and
     * drivers currently in use.
     */
    void DisplayAPIVersionAndModules();

    /**
     * Display version information and properties for all the currently
     * connected STAR devices.
     */
    void DisplayDeviceProperties();

    /**
     * Display information for all available STAR drivers.
     */
    void DisplayDriverProperties();

    /**
     * Runs the StarSystemTester program, using the command line arguments
     * provided.
     *
     * @param argc The number of arguments passed to the program.
     * @param argv An array of command line arguments passed to the program.
     *
     * @return 0 if program completed normally, otherwise 1.
     */
    int Run(int argc, char *argv[]);

    /**
     * Performs a loopback test with multiple packets. Multiple packets of 
     * random data are transmitted out of one link (using a single transfer 
     * operation) and received at a second link (again using a single transfer 
     * operation).
     *
     * This test is timed so as to test link speed. Optionally, the data is 
     * then compared to check for errors.
     */
    void LoopBack_MultiPacket();

    /**
     * Create and return an array of pointers to stream items, of the specified
     * length.
     *
     * @param len The length of the array to create.
     *
     * @return An array of pointers to StreamItems, or NULL.
     */
    StreamItem** CreateArrayOfPointersToStreamItems(int len);

    /**
     * Performs a Double loopback test with single packets.  Single packets of
     * random data are transmitted out of two links simultaneously (using a 
     * single transfer operation per link) and received at either end 
     * simultaneously (again using a single transfer operation per link).
     *
     * This test is timed so as to test link speed.  Optionally, the data is 
     * then compared to check for errors.
     *
     * This method makes use of multiple transfer operation submission.
     */
    void LoopBackDouble_SinglePacket();

    /**
     * Performs a Double loopback test with multiple packets. Multiple packets 
     * of random data are transmitted out of two links simultaneously (using a 
     * single transfer operation per link) and received at either end 
     * simultaneously (again using a single transfer operation per link).
     *
     * This test is timed so as to test link speed.  Optionally, the data is 
     * then compared to check for errors.
     */
    void LoopBackDouble_MultiPacket();

    /**
     * Transmits a single packet using one transfer operation.
     */
    void Transmit_SinglePacket();

    /**
     * Get the combined length of received traffic, and check for errors.
     *
     * @param pReceiveOp The completed receive operation containing the packets 
     *                   to look at.
     * @param packetCount The number of packets which should be in the received
     *                    operation.
     *
     * @return The length of the received packets.
     */
    unsigned long GetPacketLengths(ReceiveOperation* pReceiveOp,
                                   unsigned long packetCount);

    /**
     * Get the current clock time.
     */
    clock_t GET_TIME();
};

#endif /* STAR_TEST_H */

