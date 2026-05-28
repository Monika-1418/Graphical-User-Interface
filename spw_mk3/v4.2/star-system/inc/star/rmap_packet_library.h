/**
 * \file rmap_packet_library.h
 *
 * \brief Declarations of the functions provided by the STAR-Dundee RMAP Packet
 *        Library.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the declarations of the functions provided by the
 * STAR-Dundee RMAP Packet Library, along with constants and types used by the
 * library.  The RMAP Packet Library provides functions for building and
 * interpreting RMAP packets.
 *
 * <b>IMPORTANT NOTE:</b>
 * \note            If you are experiencing compilation errors indicating that
 *                  U8 is already defined, for example, please add the following
 *                  line to your code prior to including this file:\n
 *                      <code>\#define NO_STAR_TYPES</code>\n
 *                  Alternatively you can compile your code with a flag of
 *                  <code>-DNO_STAR_TYPES</code>.\n
 *                  (Copied from star_dundee_types.h)
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */



#ifndef RMAP_PACKET_LIBRARY_H
#define RMAP_PACKET_LIBRARY_H



#include "star-dundee_types.h"



#ifdef __cplusplus
extern "C" {
#endif


/**
 * \def RMAPPACKETLIBRARY_CC
 * \ingroup RMAPPacketLibrary
 * The calling convention used by functions exported by the RMAP Packet Library.
 *
 * \versionAdded \ref changes_v0_8
 */
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #if defined(_WIN64) || defined(__x86_64__)
        #define RMAPPACKETLIBRARY_CC
    #else
        #define RMAPPACKETLIBRARY_CC __stdcall
    #endif
#else
    #define RMAPPACKETLIBRARY_CC
#endif


/**
 * \ingroup RMAPPacketLibrary
 * Possible values for the status of RMAP operations.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /** The status value indicating success. */
    RMAP_SUCCESS								= 0x00,

    /**
     * The status value indicating that the detected error does not fit into the
     * other error cases.
     */
    RMAP_GENERAL_ERROR							= 0x01,

    /**
     * The status value indicating the packet type is reserved or the command is
     * not used by the RMAP protocol.
     */
    RMAP_UNUSED_PACKET_TYPE_OR_COMMAND_CODE		= 0x02,

    /**
     * The status value indicating the key did not match that expected by the
     * target user application.
     */
    RMAP_INVALID_KEY							= 0x03,

    /** The status value indicating there was an error in the data CRC. */
    RMAP_INVALID_DATA_CRC						= 0x04,

    /**
     * The status value indicating an EOP was detected before the end of the
     * data.
     */
    RMAP_EARLY_EOP								= 0x05,

    /** The status value indicating there was more data than was expected. */
    RMAP_TOO_MUCH_DATA							= 0x06,

    /**
     * The status value indicating that an EEP was encountered in the packet
     * after the header.
     */
    RMAP_EEP									= 0x07,

    /**
     * The status value indicating that verify before write was enabled in the
     * command but not enough buffer space was available to receive the full
     * command.
     */
    RMAP_VERIFY_BUFFER_OVERRUN					= 0x09,

    /**
     * The status value indicating the target user application did not authorise
     * the requested operation.
     */
    RMAP_COMMAND_NOT_IMPLEMENTED_OR_AUTHORISED	= 0x0a,

    /**
     * The status value indicating the amount of data in a read/modify/write
     * command is invalid.
     */
    RMAP_RMW_DATA_LENGTH_ERROR					= 0x0b,

    /**
     * The status value indicating the target logical address was not the value
     * expected by the target.
     */
    RMAP_INVALID_TARGET_LOGICAL_ADDRESS			= 0x0c,

    /**
     * The status value indicating that an invalid status value was encountered,
     * or the status could not be determined.  Note that this is not a standard
     * RMAP error.
     */
    RMAP_INVALID_STATUS							= 0xff

} RMAP_STATUS;



/**
 * \ingroup RMAPPacketLibrary
 * The value used in the protocol identifier field of all RMAP packets.
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_PROTOCOL_IDENTIFIER	1

/**
 * \ingroup RMAPPacketLibrary
 * The value used in the protocol identifier field of all SpaceWire-PnP packets.
 */
#define PNP_PROTOCOL_IDENTIFIER     3

/**
 * \ingroup RMAPPacketLibrary
 * A mask for the reserved bit of the Instruction field.
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_RESERVED_BIT				0x80

/**
 * \ingroup RMAPPacketLibrary
 * A packet is a command if the bit specified by this mask is set in the
 * Instruction field.
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_COMMAND_BIT				0x40

/**
 * \ingroup RMAPPacketLibrary
 * A packet is a write operation if the bit specified by this mask is set in the
 * Instruction field.
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_WRITE_OPERATION_BIT		0x20

/**
 * \ingroup RMAPPacketLibrary
 * An operation verifies before writing if the bit specified by this mask is set
 * in the Instruction field.
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_VERIFY_BEFORE_WRITE_BIT	0x10

/**
 * \ingroup RMAPPacketLibrary
 * An operation is acknowledged if the bit specified by this mask is set in the
 * Instruction field.
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_REPLY_BIT					0x08

/**
 * \ingroup RMAPPacketLibrary
 * A read or write address is incremented if the bit specified by this mask is
 * set in the Instruction field.
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_INCREMENT_ADDRESS_BIT		0x04

/**
 * \ingroup RMAPPacketLibrary
 * A mask for the bits in the Instruction field containing the length of the
 * reply address.
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_REPLY_ADDRESS_LENGTH_BITS	0x03

/**
 * \ingroup RMAPPacketLibrary
 * Possible values for the packet type.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef enum
{
    /** The write command packet type. */
    RMAP_WRITE_COMMAND				= (RMAP_COMMAND_BIT | RMAP_WRITE_OPERATION_BIT),

    /** The write reply packet type. */
    RMAP_WRITE_REPLY				= (RMAP_WRITE_OPERATION_BIT),

    /** The read command packet type. */
    RMAP_READ_COMMAND				= (RMAP_COMMAND_BIT),

    /** The read reply packet type. */
    RMAP_READ_REPLY					= (0),

    /** The read/modify/write command packet type. */
    RMAP_READ_MODIFY_WRITE_COMMAND	= (RMAP_COMMAND_BIT | RMAP_VERIFY_BEFORE_WRITE_BIT),

    /** The read/modify/write reply packet type. */
    RMAP_READ_MODIFY_WRITE_REPLY	= (RMAP_VERIFY_BEFORE_WRITE_BIT),

    /** The packet type used when a valid packet type cannot be determined. */
    RMAP_INVALID_PACKET_TYPE		= (0xff)
} RMAP_PACKET_TYPE;



/**
 * \ingroup RMAPPacketLibrary
 * A structure used to describe an RMAP packet.  This structure should not be
 * accessed directly, but should be populated with values using one of the
 * RMAP_Build* or RMAP_Fill* functions.  The fields should then be accessed
 * using the RMAP_Get* functions.
 *
 * \versionAdded \ref changes_v0_8
 */
typedef struct
{
    /** The type of the RMAP packet. */
    RMAP_PACKET_TYPE	packetType;

    /**
     * A pointer to the target address in the packet, or NULL if the target
     * address could not be identified.
     */
    U8					*pTargetAddress;

    /** The length of the target address in the packet. */
    unsigned long		targetAddressLength;

    /**
     * A pointer to the reply address in the packet, or NULL if the reply
     * address could not be identified.
     */
    U8					*pReplyAddress;

    /** The length of the reply address in the packet. */
    unsigned long		replyAddressLength;

    /**
     * A pointer to the protocol identifier byte in the packet, or NULL if the
     * field could not be identified.
     */
    U8					*pProtocolIdentifier;

    /**
     * A pointer to the instruction byte in the packet, or NULL if the field
     * could not be identified.
     */
    U8					*pInstruction;

    /**
     * Whether verify before write is enabled in the instruction byte in the
     * packet.
     */
    char				verifyBeforeWrite;

    /**
     * Whether acknowledgement is enabled in the instruction byte in the packet.
     */
    char				acknowledge;

    /**
     * Whether incrementing of memory addresses is enabled in the instruction
     * byte in the packet.
     */
    char				incrementAddress;

    /** A pointer to the key byte in the packet, if present, otherwise NULL. */
    U8					*pKey;

    /**
     * A pointer to the two byte transaction identifier field in the packet, or
     * NULL if the field could not be identified.
     */
    U16					*pTransactionIdentifier;

    /**
     * A pointer to the four byte read or write memory address field in the
     * packet, if present, otherwise NULL.
     */
    U32					*pReadWriteAddress;

    /**
     * A pointer to the extended read or write memory address byte in the
     * packet, if present, otherwise NULL.
     */
    U8					*pExtendedReadWriteAddress;

    /**
     * A pointer to the status byte in the packet, if present, otherwise NULL.
     */
    U8					*pStatus;

    /**
     * A pointer to the header in the packet, starting from the last byte in the
     * address at the start of the packet.
     */
    U8					*pHeader;

    /** The length of the header. */
    unsigned long		headerLength;

    /**
     * A pointer to the header CRC byte in the packet, or NULL if the field
     * could not be identified.
     */
    U8					*pHeaderCRC;

    /**
     * A pointer to the data field in the packet, if present, otherwise NULL.
     */
    U8					*pData;

    /**
     * A pointer to the three byte data length field in the packet, if present,
     * otherwise NULL.
     */
    void				*pDataLength;

    /**
     * The length of the data field in the packet, if present, otherwise 0.  If
     * the packet is read/modify/write command packet, then this value will be
     * different from the data length field in the packet, which is the total
     * length of both the data and mask fields.
     */
    U32					dataLength;

    /**
     * A pointer to the data CRC byte in the packet, if present, otherwise NULL.
     */
    U8					*pDataCRC;

    /**
     * A pointer to the mask field in the packet, if present, otherwise NULL.
     */
    U8					*pMask;

    /**
     * The length of the mask field in the packet, if present, otherwise 0.
     * This is half of the data length field in a read/modify write command
     * packet.
     */
    U8					maskLength;

    /** A pointer to the raw packet. */
    U8					*pRawPacket;

    /** The length of the raw packet. */
    unsigned long		rawPacketLength;

} RMAP_PACKET;



/* Declarations of the functions provided by the RMAP Packet Library */

U32
RMAPPACKETLIBRARY_CC
RMAP_GetVersion(
    void
    );

U8
RMAPPACKETLIBRARY_CC
RMAP_CalculateCRC(
    void			*pBuffer,
    unsigned long	len
    );

U8
RMAPPACKETLIBRARY_CC
RMAP_CalculateCRCWithSeed(
    void			*pBuffer,
    unsigned long	len,
    U8				crc
    );

char
RMAPPACKETLIBRARY_CC
RMAP_IsCRCValid(
    void			*pBuffer,
    unsigned long	len,
    U8				crc
    );

RMAP_STATUS
RMAPPACKETLIBRARY_CC
RMAP_CheckPacketValid(
    void			*pRawPacket,
    unsigned long	packetLength,
    RMAP_PACKET		*pPacketStruct,
    char			checkPacketTooLong
    );

RMAP_STATUS
RMAPPACKETLIBRARY_CC
RMAP_CheckPacketValidIgnoreProtocol(
    void			*pRawPacket,
    unsigned long	packetLength,
    RMAP_PACKET		*pPacketStruct,
    char			checkPacketTooLong
    );

RMAP_PACKET_TYPE
RMAPPACKETLIBRARY_CC
RMAP_GetPacketType(
    RMAP_PACKET	*pPacketStruct
    );

U8 *
RMAPPACKETLIBRARY_CC
RMAP_GetTargetAddress(
    RMAP_PACKET	*pPacketStruct,
    unsigned long	*pTargetAddressLength
    );

char
RMAPPACKETLIBRARY_CC
RMAP_GetVerifyBeforeWrite(
    RMAP_PACKET	*pPacketStruct
    );

char
RMAPPACKETLIBRARY_CC
RMAP_GetPerformAcknowledgement(
    RMAP_PACKET	*pPacketStruct
    );

char
RMAPPACKETLIBRARY_CC
RMAP_GetIncrementAddress(
    RMAP_PACKET	*pPacketStruct
    );

U8
RMAPPACKETLIBRARY_CC
RMAP_GetKey(
    RMAP_PACKET	*pPacketStruct
    );

U8 *
RMAPPACKETLIBRARY_CC
RMAP_GetReplyAddress(
    RMAP_PACKET		*pPacketStruct,
    unsigned long	*pReplyAddressLength
    );

U16
RMAPPACKETLIBRARY_CC
RMAP_GetTransactionID(
    RMAP_PACKET	*pPacketStruct
    );

U32
RMAPPACKETLIBRARY_CC
RMAP_GetAddress(
    RMAP_PACKET	*pPacketStruct,
    U8			*pExtendedAddress
    );

U8 *
RMAPPACKETLIBRARY_CC
RMAP_GetData(
    RMAP_PACKET	*pPacketStruct,
    U32			*pDataLength
    );

U32
RMAPPACKETLIBRARY_CC
RMAP_GetReadLength(
    RMAP_PACKET* pPacketStruct
    );

RMAP_STATUS
RMAPPACKETLIBRARY_CC
RMAP_GetStatus(
    RMAP_PACKET	*pPacketStruct
    );

U8
RMAPPACKETLIBRARY_CC
RMAP_GetHeaderCRC(
    RMAP_PACKET	*pPacketStruct
    );

U8
RMAPPACKETLIBRARY_CC
RMAP_GetDataCRC(
    RMAP_PACKET	*pPacketStruct
    );

U8 *
RMAPPACKETLIBRARY_CC
RMAP_GetMask(
    RMAP_PACKET	*pPacketStruct,
    U8			*pMaskLength
    );

unsigned long
RMAPPACKETLIBRARY_CC
RMAP_CalculateWriteCommandPacketLength(
    unsigned long	targetAddressLength,
    unsigned long	replyAddressLength,
    U32				dataLength,
    char			alignment
    );

char
RMAPPACKETLIBRARY_CC
RMAP_FillWriteCommandPacket(
    U8				*pTargetAddress,
    unsigned long	targetAddressLength,
    U8				*pReplyAddress,
    unsigned long	replyAddressLength,
    char			verifyBeforeWrite,
    char			acknowledge,
    char			incrementAddress,
    U8				key,
    U16				transactionIdentifier,
    U32				writeAddress,
    U8				extendedWriteAddress,
    U8				*pData,
    U32				dataLength,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment,
    U8				*pRawPacket,
    unsigned long	rawPacketLength
    );

void *
RMAPPACKETLIBRARY_CC
RMAP_BuildWriteCommandPacket(
    U8				*pTargetAddress,
    unsigned long	targetAddressLength,
    U8				*pReplyAddress,
    unsigned long	replyAddressLength,
    char			verifyBeforeWrite,
    char			acknowledge,
    char			incrementAddress,
    U8				key,
    U16				transactionIdentifier,
    U32				writeAddress,
    U8				extendedWriteAddress,
    U8				*pData,
    U32				dataLength,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment
    );

void *
RMAPPACKETLIBRARY_CC
RMAP_BuildWriteRegisterPacket(
    U8				*pTargetAddress,
    unsigned long	targetAddressLength,
    U8				*pReplyAddress,
    unsigned long	replyAddressLength,
    char			verifyBeforeWrite,
    char			acknowledge,
    char			incrementAddress,
    U8				key,
    U16				transactionIdentifier,
    U32				writeAddress,
    U8				extendedWriteAddress,
    U32				registerValue,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment
    );

unsigned long
RMAPPACKETLIBRARY_CC
RMAP_CalculateWriteReplyPacketLength(
    unsigned long	initiatorAddressLength,
    char			alignment
    );

char
RMAPPACKETLIBRARY_CC
RMAP_FillWriteReplyPacket(
    U8				*pInitiatorAddress,
    unsigned long	initiatorAddressLength,
    U8				targetAddress,
    char			verifyBeforeWrite,
    char			incrementAddress,
    RMAP_STATUS		status,
    U16				transactionIdentifier,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment,
    U8				*pRawPacket,
    unsigned long	rawPacketLength
    );

void *
RMAPPACKETLIBRARY_CC
RMAP_BuildWriteReplyPacket(
    U8				*pInitiatorAddress,
    unsigned long	initiatorAddressLength,
    U8				targetAddress,
    char			verifyBeforeWrite,
    char			incrementAddress,
    RMAP_STATUS		status,
    U16				transactionIdentifier,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment
    );

unsigned long
RMAPPACKETLIBRARY_CC
RMAP_CalculateReadCommandPacketLength(
    unsigned long	targetAddressLength,
    unsigned long	replyAddressLength,
    char			alignment
    );

char
RMAPPACKETLIBRARY_CC
RMAP_FillReadCommandPacket(
    U8				*pTargetAddress,
    unsigned long	targetAddressLength,
    U8				*pReplyAddress,
    unsigned long	replyAddressLength,
    char			incrementAddress,
    U8				key,
    U16				transactionIdentifier,
    U32				readAddress,
    U8				extendedReadAddress,
    U32				dataLength,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment,
    U8				*pRawPacket,
    unsigned long	rawPacketLength
    );

void *
RMAPPACKETLIBRARY_CC
RMAP_BuildReadCommandPacket(
    U8				*pTargetAddress,
    unsigned long	targetAddressLength,
    U8				*pReplyAddress,
    unsigned long	replyAddressLength,
    char			incrementAddress,
    U8				key,
    U16				transactionIdentifier,
    U32				readAddress,
    U8				extendedReadAddress,
    U32				dataLength,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment
    );

void *
RMAPPACKETLIBRARY_CC
RMAP_BuildReadRegisterPacket(
    U8				*pTargetAddress,
    unsigned long	targetAddressLength,
    U8				*pReplyAddress,
    unsigned long	replyAddressLength,
    char			incrementAddress,
    U8				key,
    U16				transactionIdentifier,
    U32				readAddress,
    U8				extendedReadAddress,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment
    );

unsigned long
RMAPPACKETLIBRARY_CC
RMAP_CalculateReadReplyPacketLength(
    unsigned long	initiatorAddressLength,
    U32				dataLength,
    char			alignment
    );

char
RMAPPACKETLIBRARY_CC
RMAP_FillReadReplyPacket(
    U8				*pInitiatorAddress,
    unsigned long	initiatorAddressLength,
    U8				targetAddress,
    char			incrementAddress,
    RMAP_STATUS		status,
    U16				transactionIdentifier,
    U8				*pData,
    U32				dataLength,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment,
    U8				*pRawPacket,
    unsigned long	rawPacketLength
    );

void *
RMAPPACKETLIBRARY_CC
RMAP_BuildReadReplyPacket(
    U8				*pInitiatorAddress,
    unsigned long	initiatorAddressLength,
    U8				targetAddress,
    char			incrementAddress,
    RMAP_STATUS		status,
    U16				transactionIdentifier,
    U8				*pData,
    U32				dataLength,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment
    );

unsigned long
RMAPPACKETLIBRARY_CC
RMAP_CalculateReadModifyWriteCommandPacketLength(
    unsigned long	targetAddressLength,
    unsigned long	replyAddressLength,
    U32				dataAndMaskLength,
    char			alignment
    );

char
RMAPPACKETLIBRARY_CC
RMAP_FillReadModifyWriteCommandPacket(
    U8				*pTargetAddress,
    unsigned long	targetAddressLength,
    U8				*pReplyAddress,
    unsigned long	replyAddressLength,
    U8				key,
    U16				transactionIdentifier,
    U32				readModifyWriteAddress,
    U8				extendedReadModifyWriteAddress,
    U8				dataAndMaskLength,
    U8				*pData,
    U8				*pMask,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment,
    U8				*pRawPacket,
    unsigned long	rawPacketLength
    );

void *
RMAPPACKETLIBRARY_CC
RMAP_BuildReadModifyWriteCommandPacket(
    U8				*pTargetAddress,
    unsigned long	targetAddressLength,
    U8				*pReplyAddress,
    unsigned long	replyAddressLength,
    U8				key,
    U16				transactionIdentifier,
    U32				readModifyWriteAddress,
    U8				extendedReadModifyWriteAddress,
    U8				dataAndMaskLength,
    U8				*pData,
    U8				*mask,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment
    );

void *
RMAPPACKETLIBRARY_CC
RMAP_BuildReadModifyWriteRegisterPacket(
    U8				*pTargetAddress,
    unsigned long	targetAddressLength,
    U8				*pReplyAddress,
    unsigned long	replyAddressLength,
    U8				key,
    U16				transactionIdentifier,
    U32				readModifyWriteAddress,
    U8				extendedReadModifyWriteAddress,
    U32				registerValue,
    U32				mask,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment
    );

unsigned long
RMAPPACKETLIBRARY_CC
RMAP_CalculateReadModifyWriteReplyPacketLength(
    unsigned long	initiatorAddressLength,
    U32				dataLength,
    char			alignment
    );

char
RMAPPACKETLIBRARY_CC
RMAP_FillReadModifyWriteReplyPacket(
    U8				*pInitiatorAddress,
    unsigned long	initiatorAddressLength,
    U8				targetAddress,
    RMAP_STATUS		status,
    U16				transactionIdentifier,
    unsigned long	dataLength,
    U8				*pData,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment,
    U8				*pRawPacket,
    unsigned long	rawPacketLength
    );

void *
RMAPPACKETLIBRARY_CC
RMAP_BuildReadModifyWriteReplyPacket(
    U8				*pInitiatorAddress,
    unsigned long	initiatorAddressLength,
    U8				targetAddress,
    RMAP_STATUS		status,
    U16				transactionIdentifier,
    unsigned long	dataLength,
    U8				*pData,
    unsigned long	*pRawPacketLength,
    RMAP_PACKET		*pPacketStruct,
    char			alignment
    );

void
RMAPPACKETLIBRARY_CC
RMAP_FreeBuffer(
    void	*pBuffer
    );

U8
RMAPPACKETLIBRARY_CC
RMAP_GetProtocolIdentifier(
    RMAP_PACKET	*pPacketStruct
    );

void
RMAPPACKETLIBRARY_CC
RMAP_SetProtocolIdentifier(
    RMAP_PACKET	*pPacketStruct,
    U8          protocolIdentifier
    );


/* Macros for accessing the version */

/**
 * \ingroup RMAPPacketLibrary
 * Return the major version number from the specified version information.  The
 * version information can be obtained from a call to
 * {@link #RMAP_GetVersion() RMAP_GetVersion}, which returns a value with the
 * major version number in the most significant 8 bits, bits 24 to 31.
 *
 * @param versionInfo the version information returned by a call to 
 *                    {@link #RMAP_GetVersion() RMAP_GetVersion}
 *
 * @return the major version number
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_GET_VERSION_MAJOR(versionInfo)	(((version) & 0xff000000) >> 24)

/**
 * \ingroup RMAPPacketLibrary
 * Return the minor version number from the specified version information.  The
 * version information can be obtained from a call to
 * {@link #RMAP_GetVersion() RMAP_GetVersion}, which returns a value with the
 * minor version number in bits 16 to 23.
 *
 * @param versionInfo the version information returned by a call to 
 *                    {@link #RMAP_GetVersion() RMAP_GetVersion}
 *
 * @return the minor version number
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_GET_VERSION_MINOR(versionInfo)	(((version) & 0x00ff0000) >> 16)

/**
 * \ingroup RMAPPacketLibrary
 * Return the edit number from the specified version information.  The version
 * information can be obtained from a call to
 * {@link #RMAP_GetVersion() RMAP_GetVersion}, which returns a value with the
 * edit number in bits 6 to 15.
 *
 * @param versionInfo the version information returned by a call to 
 *                    {@link #RMAP_GetVersion() RMAP_GetVersion}
 *
 * @return the edit number
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_GET_VERSION_EDIT(versionInfo)	(((version) & 0x0000ffc0) >> 6)

/**
 * \ingroup RMAPPacketLibrary
 * Return the patch level from the specified version information.  The version
 * information can be obtained from a call to
 * {@link #RMAP_GetVersion() RMAP_GetVersion}, which returns a value with the
 * patch level in bits 0 to 5.  The patch level should be 0 in a release version
 * of the RMAP Packet Library.
 *
 * @param versionInfo the version information returned by a call to 
 *                    {@link #RMAP_GetVersion() RMAP_GetVersion}
 *
 * @return the patch level
 *
 * \versionAdded \ref changes_v0_8
 */
#define RMAP_GET_VERSION_PATCH(versionInfo)	((version) & 0x0000003f)



#ifdef __cplusplus
}
#endif



#endif	/* RMAP_PACKET_LIBRARY_H */



