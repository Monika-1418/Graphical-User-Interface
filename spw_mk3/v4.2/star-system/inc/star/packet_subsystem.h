/**
 * \file packet_subsystem.h
 *
 * \brief Definitions of the functions provided by the PCI Mk2 packet subsystem API.
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

#ifndef SPW_PCI_MK_2_PACKET_SUBSYSTEM_H
#define SPW_PCI_MK_2_PACKET_SUBSYSTEM_H


#include "star-api.h"

/** The name of the module, used for logging purposes. */
#define PKT_SUBSYS_MODULE_NAME      "PACKET_SUBSYSTEM"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef NO_STAR_TYPES

    #include <limits.h>

    /* Define U64 */
    #ifndef U64
        #if ((ULLONG_MAX == 0xffffffffffffffffULL) || (ULONG_LONG_MAX == 0xffffffffffffffffULL))
            typedef unsigned long long  U64;
        #else
            #error "No valid definition of U64 available"
        #endif
    #endif

#endif  /* NO_STAR_TYPES */


/**
 * \ingroup packet_subsystem
 *
 *  Statistics that can be obtained from the PCI Mk2 device packet sink subsystem.
 *  Statistics are updated once per second.
 *
 * \versionAdded \ref changes_v1_7
 */
typedef struct {
    /** Number of data characters received since the PCI Mk2 was last reset. */
    U64 dataCharactersReceived;
    /** Number of EOP characters received since the PCI Mk2 was last reset. */
    U64 eopCharactersReceived;
    /** Number of EEP characters received since the PCI Mk2 was last reset. */
    U64 eepCharactersReceived;
    /** Number of data characters received in the last sampling period.
        The sampling period is one second in duration. */
    U32 dataCharacterRate;
    /** Number of EOP characters received in the last sampling period.
        The sampling period is one second in duration. */
    U32 eopCharacterRate;
    /** Number of EEP characters received in the last sampling period.
        The sampling period is one second in duration. */
    U32 eepCharacterRate;
} PKT_SUBSYS_STATISTICS;

/**
 * \ingroup packet_subsystem
 *
 * Available patterns to use with PKT_SUBSYS_fillBufferWithPattern()
 *
 * \versionAdded \ref changes_v1_7
 */
typedef enum {
    PKT_SUBSYS_PATTERN_ALL_ZEROES,
    PKT_SUBSYS_PATTERN_ALL_ONES,
    PKT_SUBSYS_PATTERN_ALTERNATING_BITS,
    PKT_SUBSYS_PATTERN_INCREMENTING_BYTES,
    PKT_SUBSYS_PATTERN_DECREMENTING_BYTES,
    PKT_SUBSYS_PATTERN_WALKING_ONES,
    PKT_SUBSYS_PATTERN_WALKING_ZEROES
}PKT_SUBSYS_PATTERN;

/**
 * \ingroup packet_subsystem
 *
 * The function type used by the statistics loop function, which is called when
 * a monitored subsystem's statistics are updated.
 *
 * @param   deviceId        Identifier for the device statistics were obtained from.
 * @param   subsystem       Subsystem number (indexed from 1) that statistics were obtained from.
 * @param   statistics      Newly gathered statisics
 *
 * \versionAdded \ref changes_v1_7
 */
typedef void (STAR_API_CC *PKT_SUBSYS_StatisticsFunc)(
    STAR_DEVICE_ID deviceIdentifier,
    unsigned int subsystem,
    PKT_SUBSYS_STATISTICS statistics);

/**
 * \ingroup packet_subsystem
 * Type of identifier used to identify a statistics polling thread. Used when
 * manually stopping a statistics polling thread.
 *
 * \versionAdded \ref changes_v1_7
 */
typedef U32 STATISTICS_LOOP_ID;

/**
 * \ingroup packet_subsystem
 *
 * Convenience function that fills a user provided buffer with a known pattern.
 *
 * @param   pattern         Pattern to generate.
 * @param   bufferSize      Size in bytes of user provided buffer.
 * @param   pBuffer         Pointer to user allocated buffer.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
void STAR_API_CC PKT_SUBSYS_fillBufferWithPattern(
                                    PKT_SUBSYS_PATTERN pattern,
                                    U16 bufferSize,
                                    _Out_bytecap_(bufferSize) void* pBuffer);

/**
 * \ingroup packet_subsystem
 *
 * Writes a buffer to the PCI Mk2's dual port memory.
 *
 * \note The dual port memory is made up of 64K 32 bit words. Therefore the valid address range
 *       is 0 through 65535. Attempts to write to addresses beyond this range will fail.
 *
 * @param     deviceId      Identifier for the device to write to.
 * @param     address       Word address to write to in the device's dual port memory.
 *                          Address values 0 through 65536 are valid.
 * @param     writeSize     Length in bytes of user provided buffer to write to device memory.
 *                          This must be a multiple of 4 (only entire words may be written)
 * @param[in] pBuffer       Pointer to user allocated buffer.
 *
 * @return  0 if the memory was succesfully written to, else 1.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_writeMemory(
                                    STAR_DEVICE_ID deviceId,
                                    U16 address,
                                    U16 writeSize,
                                    _In_bytecount_(writeSize) void* pBuffer);

/**
 * \ingroup packet_subsystem
 *
 * Reads from the PCI Mk2's dual port memory to a user allocated buffer.
 *
 * \note The dual port memory is made up of 64K 32 bit words. Therefore the valid address range
 *       is 0 through 65535. Attempts to read from addresses beyond this range will fail.
 *
 * @param        deviceId       Identifier for the device to read from.
 * @param        address        Word address to read from in the device's dual port memory.
 *                              Address values 0 through 65536 are valid.
 * @param        readSize       Size in bytes to read from the device memory into user provided buffer.
 *                              This must be a multiple of 4 (only entire words may be read)
 * @param[in,out] pBuffer       Pointer to user allocated buffer.
 *
 * @return  0 if the memory was succesfully read from to, else 1.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_readMemory(
                                    STAR_DEVICE_ID deviceId,
                                    U16 address,
                                    U16 readSize,
                                    _Inout_bytecap_(readSize) void* pBuffer);
/**
 * \ingroup packet_subsystem
 *
 * Sets up the format of the packet to be inserted by the packet generator into
 * the router.
 *
 * @param   deviceId        Identifier for the device to set up the packet generator subsystem for.
 * @param   subsystem       Subsystem number to configure (indexed from 1)
 * @param   headerPointer   Starting word address of the packet header in the device's memory.
 * @param   headerLength    Length of the packet header in bytes.
 * @param   bodyPointer     Starting word address of the area in device memory to be used for the packet body values.
 * @param   bodyLength      Number of consecutive words in memory the packet generator will use to build packets.
 * @param   packetLength    Desired length of entire packet.
 * @param   eop             Type of end of packet identifier to be used. Valid values are EOP or EEP.
 * @param   gap             Number of clock cycles between the end of one packet and the start of the next.
 *
 * @return  1 if the packet format was successfully set.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_setPacketGeneratorFormat(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    U16 headerPointer,
                                    U16 headerLength,
                                    U16 bodyPointer,
                                    U16 bodyLength,
                                    U32 packetLength,
                                    STAR_EOP_TYPE eop,
                                    U16 gap);

/**
 * \ingroup packet_subsystem
 *
 * Sets up the packet generation subsystem blocking parameters. When blocked,
 * no packet data will be inserted into the router by the packet generator.
 *
 * @param   deviceId        Identifier for the device to set up the packet generator subsystem for.
 * @param   subsystem       Subsystem number to configure (indexed from 1)
 * @param   blockFrequency  Number of clock cycles between the end of a blocking operation and the start
 *                          of the next.
 * @param   blockDuration   Number of clock cycles which blocking will occur over.
 *
 * @return  1 if packet generation was stopped.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_setGeneratorBlockingParameters(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    U16 blockFrequency,
                                    U16 blockDuration);


/**
 * \ingroup packet_subsystem
 *
 * Starts the packet generator inserting packets into the router.
 *
 * @param   deviceId        Identifier for the device to set up the packet generator subsystem for.
 * @param   subsystem       Subsystem number to configure (indexed from 1)
 * @param   sequenceLength  Number of packets that will be inserted before generation stops. Specify a
 *                          sequence length of zero to enable continuous transmission.
 *
 * @return  1 if packet generation was started.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_startPacketGeneration(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    U16 sequenceLength);

/**
 * \ingroup packet_subsystem
 *
 * Blocks the current thread until packet generation has stopped.
 *
 * @param   deviceId        Identifier for the device with the packet generator subsystem to wait for.
 * @param   subsystem       Subsystem number to wait on (indexed from 1)
 * @param   timeout         Timeout period in millisecods. Provide a value of 0 to wait indefinitely.
 *
 * @return  0 if error occurred communicating with device, 1 if packet generation completed or -1 if timeout occurred.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_waitForPacketGenerationSequenceComplete(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    unsigned int timeout);



/**
 * \ingroup packet_subsystem
 *
 *  Stops the packet generator inserting packets into the router.
 *
 * @param   deviceId        Identifier for the device to set up the packet generator subsystem for.
 * @param   subsystem       Subsystem number to configure (indexed from 1)
 *
 * @return  1 if packet generation was stopped.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_stopPacketGeneration(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem);




/**
 * \ingroup packet_subsystem
 *
 * Sets up the circular buffer for the packet sink subsystem.
 *
 * @param   deviceId        Identifier for the device to set up the packet sink subsystem for.
 * @param   subsystem       Subsystem number to configure (indexed from 1)
 * @param   memoryPointer   Starting word address for the circular buffer used to record packets received by the sink.
 * @param   memoryLength    Length of the circular buffer in words.
 *
 * @return  1 if the packet sink parameters were set.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_setPacketSinkParameters(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    U16 memoryPointer,
                                    U16 memoryLength);


/**
 * \ingroup packet_subsystem
 *
 * Sets up the packet sink subsystem blocking parameters. When blocked,
 * no packet data will be read from router into packet sink memory.
 *
 * @param   deviceId        Identifier for the device to set up the packet generator subsystem for.
 * @param   subsystem       Subsystem number to configure (indexed from 1)
 * @param   blockFrequency  Number of clock cycles between the end of a blocking operation and the start
 *                          of the next.
 * @param   blockDuration   Number of clock cycles which blocking will occur over.
 *
 * @return  1 if packet sink blocking parameters were set.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_setSinkBlockingParameters(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    U16 blockFrequency,
                                    U16 blockDuration);

/**
 * \ingroup packet_subsystem
 *
 * Starts the packet sink reading packets into memory.
 *
 * @param   deviceId        Identifier for the device to start the packet sink on.
 * @param   subsystem       Subsystem number to configure (indexed from 1)
 *
 * @return  1 if packet sink was started.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_startSink(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem);


/**
 * \ingroup packet_subsystem
 *
 * Stops the packet sink reading packets into memory.
 *
 * @param   deviceId        Identifier for the device to stop the packet sink on.
 * @param   subsystem       Subsystem number to configure (indexed from 1)
 *
 * @return  1 if packet sink was stopped.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_stopSink(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem);

/**
 * \ingroup packet_subsystem
 *
 * Gets the address for the start and end of valid data in the circular buffer in device memory.
 * Note that if the endPointer is less than the startPointer then the end pointer
 * has wrapped round since the sink was last started.
 *
 * @param       deviceId          Identifier for the device to get the memory location for.
 * @param       subsystem         Subsystem number to get information from (indexed from 1).
 * @param[out]  startPointer      32-bit word address of start of acquired data in the circular buffer.
 * @param[out]  endPointer        32-bit word address of end of acquired data in the circular buffer.
 *
 * @return  1 if packet sink was stopped.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_getSinkDataPointers(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    U16* startPointer,
                                    U16* endPointer);

/**
 * \ingroup packet_subsystem
 *
 * Starts a packet checker running on a packet sink. If a character mismatch is detected
 * the packet sink is disabled after a specified number of data characters are received.
 * Note that a packet sink must be running to enable checking of incoming packets.
 *
 * @param   deviceId             Identifier for the device to start the packet checking on.
 * @param   subsystem            Subsystem number to configure (indexed from 1).
 * @param   disableSinkOnError   Whether to disable the packet sink if a mismatched character is
 *                               received (1) or not (0).
 * @param   disableSinkdelay     The number of bytes to be received after a character mismatch
 *                               is detected and before the packet sink is disabled.
 *
 * @return  1 if packet checking was started.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_startPacketChecking(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    int disableSinkOnError,
                                    U16 disableSinkdelay);

/**
 * \ingroup packet_subsystem
 *
 * Stops the packet checker.
 *
 * @param   deviceId        Identifier for the device packet checking is started on.
 * @param   subsystem       Subsystem number to configure (indexed from 1).
 *
 * @return  1 if packet checking was stopped
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_stopPacketChecking(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem);

/**
 * \ingroup packet_subsystem
 *
 * Blocks the current thread until either a character mismatch is detected and the packet
 * checker is automatically stopped, or the user manually stops the packet checker.
 *
 * @param   deviceId        Identifier for the device packet checking is started on.
 * @param   subsystem       Subsystem number to configure (indexed from 1).
 *
 * @return  1 if packet sink was stopped, or 0 if an error occurred.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_waitForPacketCheckingError(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem);

/**
 * \ingroup packet_subsystem
 *
 * Cancels all waits started by PKT_SUBSYS_waitForPacketCheckingError()
 * for packet checker errors on a given device and subsystem.
 *
 * @param   deviceId        Identifier for the device packet checking is started on.
 * @param   subsystem       Subsystem number to configure (indexed from 1).
 *
 * @return  1 if packet sink was stopped, or 0 if an error occurred.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_cancelWaitsForPacketCheckingError(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem);

/**
 * \ingroup packet_subsystem
 *
 * Sets up the packet checker to check packets match the format of packets provided by user
 * entered parameters.
 *
 * @param   deviceId        Identifier for the device to set up the packet checking subsystem for.
 * @param   subsystem       Subsystem number to configure (indexed from 1)
 * @param   headerPointer   Address of the expected packet header in the device's memory.
 * @param   headerLength    Length of the expected packet header in bytes.
 * @param   bodyPointer     Starting address of the area in device memory to be used for the expected packet body values.
 * @param   bodyLength      Expected Length of the packet body in bytes.
 * @param   packetLength    Expected length of entire packet.
 * @param   eop             Type of end of packet identifier to be used. Valid values are EOP or EEP.
 *
 * @return  1 if the packet format was successfully set.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_setPacketCheckingFormat(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    U16 headerPointer,
                                    U16 headerLength,
                                    U16 bodyPointer,
                                    U16 bodyLength,
                                    U32 packetLength,
                                    STAR_EOP_TYPE eop);

/**
 * \ingroup packet_subsystem
 *
 * Gets the number of mismatched data characters observed on the SpaceWire interface.
 *
 * @param       deviceId        Identifier for the device to start the packet checking on.
 * @param       subsystem       Subsystem number to configure (indexed from 1)
 * @param[out]  errorCount      Count of mismatched characters.
 *
 * @return  1 if packet sink was started.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_getPacketCheckingMismatchCount(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    U64* errorCount);


/**
 * \ingroup packet_subsystem
 *
 * Gets statistics for a given subsystem. Note that a sink must
 * be started for statistics to be updated.
 *
 * @param       deviceId        Identifier for the device to stop the packet sink on.
 * @param       subsystem       Subsystem number to configure (indexed from 1).
 * @param[out]  pStatistics     Pointer to a user allocated PKT_SUBSYS_STATISTICS structure.
 *                              that will be updated with the current values of the statistics
 *                              registers.
 *
 * @return  1 if packet sink was stopped.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
int STAR_API_CC PKT_SUBSYS_getStatistics(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    PKT_SUBSYS_STATISTICS* pStatistics);

/**
 * \ingroup packet_subsystem
 *
 * Starts a thread that polls statistics on a given subsystem, and calls a given
 * function with the updated statistics when they are retrieved. If an error occurs
 * obtaining the statistics, the thread terminates. The thread will keep running until stopped with
 * PKT_SUBSYS_stopGetStatisticsLoop() even if the packet sink is disabled (though no statistics are
 * updated when the packet sink is disabled).
 *
 * \note If packet checking is enabled and a character mismatch occurs, the packet
 *       sink is disabled. This causes statistics to stop being gathered until the sink
 *       is restarted.
 *
 * @param   deviceId            Identifier for the device to stop the packet sink on.
 * @param   subsystem           Subsystem number to configure (indexed from 1)
 * @param   statisticsHandler   Pointer to function that will be called when updated statistics are
 *                              obtained.
 *
 * @return Identifier for the loop that can be used to stop it.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
STATISTICS_LOOP_ID STAR_API_CC PKT_SUBSYS_startGetStatisticsLoop(
                                    STAR_DEVICE_ID deviceId,
                                    unsigned int subsystem,
                                    PKT_SUBSYS_StatisticsFunc statisticsHandler);

/**
 * \ingroup packet_subsystem
 *
 * Stops a statistics gathering loop set up by PKT_SUBSYS_startGetStatisticsLoop().
 *
 * @param       loopId      Identifier for the loop to stop.
 *
 * \versionAdded \ref changes_v1_7
 *
 * \devicesSupported \ref PCIMk2
 */
void  STAR_API_CC PKT_SUBSYS_stopGetStatisticsLoop(STATISTICS_LOOP_ID loopId);

#ifdef __cplusplus
}
#endif



#endif  /* SPW_PCI_MK_2_PACKET_SUBSYSTEM_H */
