/**
 * \file rmap_examples.cpp
 *
 * \brief Provides implementations of classes which provide examples of using
 *        objects and functions in the RMAP packet library.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides implementations of classes which provide examples of using objects
 * and functions in the RMAP packet library.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "rmap_examples.h"
#include "packet.hpp"
#include "transmit_operation.hpp"
#include "receive_operation.hpp"
#include "cpp_api_macros.hpp"
#include "device_selecter.h"

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::transferoperations;
using namespace stardundee::com::starsystem::channels;

/**
 * Provides an example of using the CRC functions in the RMAP
 * packet library.
 */
void RMAPExamples::PerformCrcExample()
{
    U8 pBuffer[BUFFER_LEN], crc;
    int i;

    /* display test header */
    puts("\n#####\nRMAP - CRC Example\n");

    /* fill data buffer */
    for (i=0; i<BUFFER_LEN; i++)
    {
        pBuffer[i] = (U8)i;
    }

    /* calculate a CRC for the header part of the buffer */
    crc = RMAPPacketLibrary::CalculateCRC(pBuffer, HEADER_LEN);

    /* display the header CRC */
    printf("The CRC of the first %d bytes of the buffer is 0x%x\n",
            HEADER_LEN, crc);

    /* calculate a CRC for the entire buffer, using the CRC already
     * calculated as a seed
     */
    crc = RMAPPacketLibrary::CalculateCRCWithSeed(pBuffer + HEADER_LEN,
                                                  BUFFER_LEN - HEADER_LEN,
                                                  crc);

    /* display the CRC for the entire buffer */
    printf("The CRC of the full buffer is 0x%x\n", crc);

    /* check the CRC is correct for the buffer */
    printf("The CRC of the buffer is %svalid\n",
    RMAPPacketLibrary::IsCRCValid(pBuffer, BUFFER_LEN, crc) ? "" : "not ");
}

/**
 * Display the type of a packet based upon the given type value.
 *
 * @param type The packet's type
 */
void RMAPExamples::display_packet_type(RMAPPacket::PacketType type)
{
    switch (type)
    {
        case RMAPPacket::RMAP_PACKET_TYPE_WRITE_COMMAND:
            puts("The packet is an RMAP write command.");
            break;

        case RMAPPacket::RMAP_PACKET_TYPE_WRITE_REPLY:
            puts("The packet is an RMAP write reply.");
            break;

        case RMAPPacket::RMAP_PACKET_TYPE_READ_COMMAND:
            puts("The packet is an RMAP read command.");
            break;

        case RMAPPacket::RMAP_PACKET_TYPE_READ_REPLY:
            puts("The packet is an RMAP read reply.");
            break;

        case RMAPPacket::RMAP_PACKET_TYPE_READ_MODIFY_WRITE_COMMAND:
            puts("The packet is an RMAP read/modify/write command.");
            break;

        case RMAPPacket::RMAP_PACKET_TYPE_READ_MODIFY_WRITE_REPLY:
            puts("The packet is an RMAP read/modify/write reply.");
            break;

        case RMAPPacket::RMAP_PACKET_TYPE_INVALID:
            puts("The packet is of an invalid RMAP packet type.");
            break;

        default:
            puts("The packet is of an unexpected type.");
    }
}

/**
 * Display the status of a packet from the given status value.
 *
 * @param status The packet's status
 */
void RMAPExamples::display_packet_status(RMAPPacket::RMAPStatus status)
{
    switch (status)
    {
        case RMAPPacket::RMAP_STATUS_SUCCESS:
            puts("The status indicates the command completed successfully.");
            break;
        case RMAPPacket::RMAP_STATUS_GENERAL_ERROR:
            puts("The status indicates a general error that does not fit into"
                 " the other error cases.");
            break;
        case RMAPPacket::RMAP_STATUS_UNUSED_PACKET_TYPE_OR_COMMAND_CODE:
            puts("The status indicates the packet type or command is an"
                 " unexpected value.");
            break;
        case RMAPPacket::RMAP_STATUS_INVALID_KEY:
            puts("The status indicates the key did not match that expected by"
                 " the target user application.");
            break;
        case RMAPPacket::RMAP_STATUS_INVALID_DATA_CRC:
            puts("The status indicates an invalid data CRC.");
            break;
        case RMAPPacket::RMAP_STATUS_EARLY_EOP:
            puts("The status indicates an EOP was detected before the end of"
                 " the data.");
            break;
        case RMAPPacket::RMAP_STATUS_TOO_MUCH_DATA:
            puts("The status indicates there was more data than was expected.");
            break;
        case RMAPPacket::RMAP_STATUS_EEP:
            puts("The status indicates an EEP was  encountered in the packet"
                 " after the header.");
            break;
        case RMAPPacket::RMAP_STATUS_VERIFY_BUFFER_OVERRUN:
            puts("The status indicates verify before write was enabled in the"
                 " command but not enough buffer space was available to receive"
                 " the full command.");
            break;
        case RMAPPacket::RMAP_STATUS_COMMAND_NOT_IMPLEMENTED_OR_AUTHORISED:
            puts("The status indicates the target user application did not"
                 " authorise the requested operation.");
            break;
        case RMAPPacket::RMAP_STATUS_RMW_DATA_LENGTH_ERROR:
            puts("The status indicates the amount of data in a"
                 " read/modify/write command is invalid.");
            break;
        case RMAPPacket::RMAP_STATUS_INVALID_TARGET_LOGICAL_ADDRESS:
            puts("The status indicates the target logical address was not the"
                 " value expected by the target.");
            break;
        default:
            puts("The status is an unexpected value.");
    }
}

/**
 * Check the fields of the given packet to ensure that they are valid and
 * as expected, reporting their values.
 *
 * @param pPacketToCheck A pointer to an RMAP packet object, representing the
 *                       packet to be checked.
 */
void RMAPExamples::CheckPacket(RMAPPacket *pPacketToCheck)
{
    RMAPPacket::PacketType type;
    RMAPPacket::RMAPStatus status;
    unsigned long addressLen, i;
    U8 *pAddress, *pData, *pMask;
    char enabled;
    U8 key, crc, maskLen, extendedMemoryAddress;
    U16 transactionID;
    U32 memoryAddress, dataLen;
    char checkPacketLength;

    /* display test header */
    puts("\n#####\nRMAP - Check Packet Test\n");

    /* check the format of the packet, making sure it's not longer than
     * expected
     */
    checkPacketLength = 1; /* 1=check length; 0=don't check length */
    status = pPacketToCheck->CheckPacketValid(checkPacketLength);
    printf("The packet is %sa valid RMAP packet.\n",
        (status == RMAPPacket::RMAP_STATUS_SUCCESS) ? "" : "not ");

    /* get and display the packet's type */
    type = pPacketToCheck->GetPacketType();
    display_packet_type(type);

    /* get the target address of the packet */
    pAddress = pPacketToCheck->GetTargetAddress(&addressLen);

    /* if target address not valid */
    if ((!pAddress) || (addressLen == 0))
    {
        puts("No valid target address is present in the packet.");
    }
    else
    {
        /* display the target address of the packet */
        printf("The packet has a target address of:");
        for (i = 0; i < addressLen; i++)
        {
            printf(" %x", pAddress[i]);
        }
        puts("");
    }

    /* determine if verify before write is enabled */
    enabled = pPacketToCheck->GetVerifyBeforeWrite();
    printf("Verify before write is %senabled for the packet.\n",
        enabled ? "" : "not ");

    /* determine if acknowledgement is enabled */
    enabled = pPacketToCheck->GetPerformAcknowledgement();
    printf("Acknowledgement is %senabled for the packet.\n",
        enabled ? "" : "not ");

    /* determine if incrementing addresses is enabled */
    enabled = pPacketToCheck->GetIncrementAddress();
    printf("Incrementing addresses is %senabled for the packet.\n",
        enabled ? "" : "not ");

    /* get and display the packet's key */
    key = pPacketToCheck->GetKey();
    printf("The value of the key is 0x%x.\n", key);

    /* get the transaction identifier of the packet */
    transactionID = pPacketToCheck->GetTransactionID();
    printf("The value of the transaction identifier is %d.\n", transactionID);

    /* get the reply address of the packet */
    pAddress = pPacketToCheck->GetReplyAddress(&addressLen);

    /* if no reply address */
    if ((!pAddress) || (addressLen == 0))
    {
        puts("No valid reply address is present in the packet.");
    }
    else
    {
        /* display the reply address of the packet */
        printf("The packet has a reply address of:");
        for (i = 0; i < addressLen; i++)
        {
            printf(" %x", pAddress[i]);
        }
        puts("");
    }

    /* get the memory address and extended memory address of the packet */
    memoryAddress = pPacketToCheck->GetAddress(&extendedMemoryAddress);

    /* display the memory address and extended memory address of the packet */
    printf("The value of the memory address is 0x%x ", memoryAddress);
    printf("and the extended address is 0x%x.\n", extendedMemoryAddress);

    /* get the data in the packet */
    pData = pPacketToCheck->GetData(&dataLen);

    /* if no data */
    if ((!pData) || (dataLen == 0))
    {
        puts("No valid data is present in the packet.");
    }
    else
    {
        /* display the data in the packet */
        printf("The packet has the following data:");
        for (i = 0; i < dataLen; i++)
        {
            printf(" %x", pData[i]);
        }
        puts("");
    }

    /* display the status of the packet */
    status = pPacketToCheck->GetStatus();
    display_packet_status(status);

    /* display the header CRC of the packet */
    crc = pPacketToCheck->GetHeaderCRC();
    printf("The value of the header CRC is 0x%x.\n", crc);

    /* display the data CRC of the packet */
    crc = pPacketToCheck->GetDataCRC();
    printf("The value of the data CRC is 0x%x.\n", crc);

    /* get the mask in the packet */
    pMask = pPacketToCheck->GetMask(&maskLen);

    /* if no mask */
    if ((!pMask) || (maskLen == 0))
    {
        puts("No valid mask is present in the packet.");
    }
    else
    {
        /* display the mask in the packet */
        printf("The packet has the following mask:");
        for (i = 0; i < maskLen; i++)
        {
            printf(" %x", pMask[i]);
        }
        puts("");
    }
}

/**
 * Provides an example of how to check that the format of a received RMAP
 * packet is correct, and then to access the fields in the packet.
 *
 * The example creates an RMAP write command packet, to write to a 4-byte
 * register (a 'write register' packet).
 */
void RMAPExamples::PerformCheckPacketExample()
{
    /* define fields required to build a write register packet */
    RMAPWriteCommandPacket writeRegisterPacket;
    U8 pTargetAddress[] = {0, 254};
    unsigned long targetAddressLength = 2;
    U8 pReplyAddress[] = {254};
    unsigned long replyAddressLength = 1;
    char verifyBeforeWrite = 1;
    char acknowledge = 1;
    char incrementAddress = 0;
    U8 key = 0x20;
    U16 transactionIdentifier = 0;
    U32 writeAddress = 0x106;
    U8 extendedWriteAddress = 0;
    U32 registerValue = 0x12345678;
    char alignment = 1;

    /* create an RMAP write command packet to write to a 4-byte register */
    if (!writeRegisterPacket.CreateWriteRegisterPacket(pTargetAddress,
                                                    targetAddressLength,
                                                    pReplyAddress,
                                                    replyAddressLength,
                                                    verifyBeforeWrite,
                                                    acknowledge,
                                                    incrementAddress,
                                                    key,
                                                    transactionIdentifier,
                                                    writeAddress,
                                                    extendedWriteAddress,
                                                    registerValue,
                                                    alignment))
    {
        /* report error */
        puts("Error: unable to build RMAP write command packet. An error may"
                " have occurred or one of the fields is invalid. Ending test.");

        /* end processing */
        return;
    }

    /* having created a packet, check the fields of the packet to ensure
     * that they are valid and as expected
     */
    CheckPacket(&writeRegisterPacket);

    /* dispose of packet when no longer required */
    /* \note The packet object destructor will automatically dispose
     *       of the packet if required, if it isn't manually disposed of.
     */
    writeRegisterPacket.Dispose();
}

/**
 * Perform example calls to build read command packets and to
 * determine the length of these packets.
 */
void RMAPExamples::PerformReadCommandPacketExample()
{
    /* declare variables required for example */
    RMAPReadCommandPacket readCommandPacket;
    RMAPReadCommandPacket readRegisterPacket;
    U8 pTarget[] = {0, 254};
    unsigned long targetAddressLen = 2;
    U8 pReply[] = {254};
    unsigned long replyAddressLen = 1;
    char incrementAddress = 0;
    U8 key = 0x20;
    U16 transactionID = 0;
    U32 readAddress = 0x106;
    U8 extendedReadAddress = 0;
    U32 dataLength = 4;
    char alignment = 1;

    /* display test header */
    puts("\n#####\nRMAP - Read Command Packet Example\n");

    /* build an RMAP read command packet to read 4-bytes */
    if (!readCommandPacket.CreateReadCommandPacket(pTarget,
                                                        targetAddressLen,
                                                        pReply,
                                                        replyAddressLen,
                                                        incrementAddress,
                                                        key, transactionID,
                                                        readAddress,
                                                        extendedReadAddress,
                                                        dataLength,
                                                        alignment))
    {
        /* report error */
        puts("Couldn't build the read command packet");

        /* end processing */
        return;
    }

    /* create an RMAP read command packet to read from a 4-byte register */
    if (!readRegisterPacket.CreateReadRegisterPacket(pTarget,
                                                        targetAddressLen,
                                                        pReply,
                                                        replyAddressLen,
                                                        incrementAddress,
                                                        key, transactionID,
                                                        readAddress,
                                                        extendedReadAddress,
                                                        alignment))
    {
        /* report error */
        puts("Couldn't build the read register packet");

        /* end processing */
        return;
    }

    /* check the lengths are all the same */
    if (readCommandPacket.GetPacketLength() !=
        readRegisterPacket.GetPacketLength())
    {
        puts("The packet lengths are different:");
        printf("\tLength of Read Command packet: %lu\n",
                readCommandPacket.GetPacketLength());
        printf("\tLength of Read Register packet: %lu\n",
                readRegisterPacket.GetPacketLength());
    }
    else
    {
        puts("The packet lengths are the same.");

        /* check the packets are identical, despite being built differently */
        if (PacketsAreIdentical(&readCommandPacket, &readRegisterPacket))
        {
            puts("The packets are identical.");
        }
        else
        {
            puts("The read command packet is different to the read"
                 " register packet.");
        }
    }

    /* dispose of packets when no longer required */
    /* \note The packets will be automatically disposed of if required in the
     *       object destructors, if they aren't manually disposed of.
     */
    readCommandPacket.Dispose();
    readRegisterPacket.Dispose();
}

/**
 * Compare the contents of two RMAPPackets to determine if they are identical
 * or not.
 *
 * @param pPacket1 The first packet to compare.
 * @param pPacket2 The second packet to compare.
 *
 * @return 1 if the packets are identical, otherwise 0.
 */
char RMAPExamples::PacketsAreIdentical(RMAPPacket *pPacket1,
                                       RMAPPacket *pPacket2)
{
    void *pPacket1Data, *pPacket2Data;
    unsigned long packet1DataLen;

    /* get a pointer to the first packet data, and its length */
    pPacket1Data = pPacket1->GetPacketData();
    packet1DataLen = pPacket1->GetPacketLength();

    /* get a pointer to the second packet data */
    pPacket2Data = pPacket2->GetPacketData();

    /* if data in packet 1 is identifical to that in packet 2 */
    if (memcmp(pPacket1Data, pPacket2Data, packet1DataLen) == 0)
    {
        /* return 1, packets are identical */
        return 1;
    }
    else
    {
        /* return 0, packets not identical */
        return 0;
    }
}

/**
 * Perform example calls to build read reply packets and to
 * determine the length of these packets.
 */
void RMAPExamples::PerformReadReplyPacketExample()
{
    /* define variables necessary for test */
    RMAPReadReplyPacket readReplyPacket;
    U8 pInitiatorAddress[] = {254}; /* the address to reply to */
    unsigned long initiatorAddressLen = 1;
    U8 targetAddress = 254;
    char incrementAddress = 0;
    RMAPPacket::RMAPStatus status = RMAPPacket::RMAP_STATUS_SUCCESS; /* the status of the read operation */
    U16 transactionID = 0;
    U8 pData[] = {0x12, 0x34, 0x56, 0x78};
    U32 dataLen = 4;
    char alignment = 1;

    /* display test header */
    puts("\n#####\nRMAP - Read Reply Packet Example\n");

    /* build an RMAP read reply packet */
    if (!readReplyPacket.CreateReadReplyPacket(pInitiatorAddress,
                                                    initiatorAddressLen,
                                                    targetAddress,
                                                    incrementAddress,
                                                    status,
                                                    transactionID,
                                                    pData,
                                                    dataLen,
                                                    alignment))
    {
        /* report error */
        puts("Couldn't build the read reply packet");

        /* end processing */
        return;
    }

    /* use / process packet */
    /* <ommitted> */

    /* dispose of packet when no longer required */
    /* \note The ReadReplyPacket object destructor will automatically dispose
     *       of the packet if required, if it isn't manually disposed of.
     */
    readReplyPacket.Dispose();
}

/**
 * Perform example calls to build read/modify/write command packets and to
 * determine the length of these packets.
 */
void RMAPExamples::PerformReadModifyWriteCommandPacketExample()
{
    RMAPReadModifyWriteCommandPacket readModifyWriteCommandPacket;
    U8 pTarget[] = {0, 254};
    unsigned long targetAddressLen = 2;
    U8 pReply[] = {254};
    unsigned long replyAddressLen = 1;
    U8 pData[] = {0x12, 0x34, 0x56, 0x78};
    U8 pMask[] = {0x0f, 0x0f, 0x0f, 0x0f};
    U8 dataAndMaskLen = 8;
    char alignment = 1;
    U8 key = 0x20;
    U16 transactionID = 0;
    U32 readModifyWriteAddress = 0x106;
    U8 extendedReadModifyWriteAddress = 0;
    RMAPReadModifyWriteCommandPacket readModifyWriteRegisterPacket;
    U32 registerValue = 0x12345678;
    U32 mask = 0x0f0f0f0f;

    /* display test header */
    puts("\n#####\nRMAP - Read/Modify/Write Packet Example\n");

    /* build an RMAP read/modify/write command packet to write 4-bytes */
    if (!readModifyWriteCommandPacket.CreateReadModifyWriteCommandPacket(
                                                pTarget,
                                                targetAddressLen,
                                                pReply,
                                                replyAddressLen,
                                                key,
                                                transactionID,
                                                readModifyWriteAddress,
                                                extendedReadModifyWriteAddress,
                                                dataAndMaskLen,
                                                pData,
                                                pMask,
                                                alignment))
    {
        /* report error */
        puts("Couldn't build the read/modify/write command packet");

        /* end processing */
        return;
    }

    /* create an RMAP read/modify/write command packet to write to a 4-byte
     * register
     */
    if (!readModifyWriteRegisterPacket.CreateReadModifyWriteRegisterPacket(
                                                        pTarget,
                                                        targetAddressLen,
                                                        pReply,
                                                        replyAddressLen,
                                                        key,
                                                        transactionID,
                                                        readModifyWriteAddress,
                                                        extendedReadModifyWriteAddress,
                                                        registerValue,
                                                        mask,
                                                        alignment))
    {
        /* report error */
        puts("Couldn't build the read/modify/write register packet");

        /* end processing */
        return;
    }

    /* check the lengths are all the same */
    if ((readModifyWriteCommandPacket.GetPacketLength() !=
            readModifyWriteRegisterPacket.GetPacketLength()))
    {
        puts("The lengths are different:");
        printf("\tLength of read/modify/write command packet: %lu\n",
                readModifyWriteCommandPacket.GetPacketLength());
        printf("\tLength of read/modify/write register packet: %lu\n",
                readModifyWriteRegisterPacket.GetPacketLength());
    }
    else
    {
        puts("The packet lengths are all the same.");

        /* check the packets are identical, despite being built differently */
        if (!PacketsAreIdentical(&readModifyWriteRegisterPacket,
                                 &readModifyWriteCommandPacket))
        {
            puts("The packets are different.");
        }
        else
        {
            puts("The packets are identical.");
        }
    }

    /* dispose of packets when no longer required */
    /* \note The packets will be automatically disposed of if required in the
     *       object destructors, if they aren't manually disposed of.
     */
    readModifyWriteCommandPacket.Dispose();
    readModifyWriteRegisterPacket.Dispose();
}

/**
 * Perform example calls to build read/modify/write reply packets and to
 * determine the length of these packets.
 */
void RMAPExamples::PerformReadModifyWriteReplyPacketExample()
{
    /* define variables necessary for test */
    U8 pInitiatorAddress[] = {254};
    unsigned long initiatorAddressLen = 1;
    U8 targetAddress = 254;
    RMAPPacket::RMAPStatus replyStatus = RMAPPacket::RMAP_STATUS_SUCCESS;
    U16 transactionID = 0;
    unsigned long dataLen = 4;
    U8 pData[] = {0x12, 0x34, 0x56, 0x78};
    char alignment = 1;
    RMAPReadModifyWriteReplyPacket readModifyWriteReplyPacket;

    /* display test header */
    puts("\n#####\nRMAP - Read/Modify/Write Reply Packet Example\n");

    /* build an RMAP read/modify/write reply packet */
    if (!readModifyWriteReplyPacket.CreateReadModifyWriteReplyPacket(
                                                pInitiatorAddress,
                                                initiatorAddressLen,
                                                targetAddress, replyStatus,
                                                transactionID, dataLen,
                                                pData, alignment))
    {
        /* report error */
        puts("Couldn't build the read/modify/write reply packet");

        /* end processing */
        return;
    }

    /* perform processing of the packet */
    /* e.g. check / display its length */
    printf("\tLength of packet: %lu\n",
            readModifyWriteReplyPacket.GetPacketLength());

    /* dispose of packet when no longer required */
    /* \note The packet object destructor will automatically dispose
     *       of the packet if required, if it isn't manually disposed of.
     */
    readModifyWriteReplyPacket.Dispose();
}

/**
 * Perform example calls to build write command packets and to
 * determine the length of these packets.
 */
void RMAPExamples::PerformWriteCommandPacketExample()
{
    /* define variables required for test */
    U8 pTargetAddress[] = {0, 254};
    unsigned long targetAddressLen = 2;
    U8 pReplyAddress[] = {254};
    unsigned long replyAddressLen = 1;
    char acknowledge = 1;
    char verifyBeforeWrite = 1;
    char incrementAddress = 0;
    U8 key = 0x20;
    U16 transactionID = 0;
    U32 writeAddress = 0x106;
    U8 extendedWriteAddress = 0;
    U8 pData[] = {0x12, 0x34, 0x56, 0x78};
    U32 dataLen = 4;
    char alignment = 1;
    RMAPWriteCommandPacket writeCommandPacket;
    RMAPWriteCommandPacket writeRegisterPacket;
    U32 registerValue = 0x12345678;

    /* display test header */
    puts("\n#####\nRMAP - Write Command Packet Example\n");

    /* build an RMAP write command packet to write 4-bytes */
    if (!writeCommandPacket.CreateWriteCommandPacket(pTargetAddress,
                                                        targetAddressLen,
                                                        pReplyAddress,
                                                        replyAddressLen,
                                                        verifyBeforeWrite,
                                                        acknowledge,
                                                        incrementAddress,
                                                        key,
                                                        transactionID,
                                                        writeAddress,
                                                        extendedWriteAddress,
                                                        pData,
                                                        dataLen,
                                                        alignment))
    {
        /* report error */
        puts("Couldn't build the write command packet");

        /* end processing */
        return;
    }

    /* create an RMAP write command packet to write to a 4-byte register */
    if (!writeRegisterPacket.CreateWriteRegisterPacket(pTargetAddress,
                                                        targetAddressLen,
                                                        pReplyAddress,
                                                        replyAddressLen,
                                                        verifyBeforeWrite,
                                                        acknowledge,
                                                        incrementAddress,
                                                        key,
                                                        transactionID,
                                                        writeAddress,
                                                        extendedWriteAddress,
                                                        registerValue,
                                                        alignment))
    {
        /* report error */
        puts("Couldn't build the write register packet");

        /* end processing */
        return;
    }

    /* check the two packet lengths are the same */
    if (writeCommandPacket.GetPacketLength() !=
            writeRegisterPacket.GetPacketLength())
    {
        puts("The lengths are different:");
        printf("\tLength of write command packet: %lu\n",
                writeCommandPacket.GetPacketLength());
        printf("\tLength of write register packet: %lu\n",
                writeRegisterPacket.GetPacketLength());
    }
    else
    {
        puts("The packet lengths are the same.");

        /* check the packets are identical, despite being built differently */
        if (!PacketsAreIdentical(&writeCommandPacket, &writeRegisterPacket))
        {
            puts("The write command packet is different to the write register"
                 " packet.");
        }
        else
        {
            puts("The packets are identical.");
        }
    }

    /* dispose of packets when no longer required */
    /* \note The packets will be automatically disposed of if required in the
     *       object destructors, if they aren't manually disposed of.
     */
    writeCommandPacket.Dispose();
    writeRegisterPacket.Dispose();
}

/**
 * Perform example calls to build write reply packets and to
 * determine the length of these packets.
 */
void RMAPExamples::PerformWriteReplyPacketExample()
{
    /* define variables required for test */
    U8 pInitiatorAddress[] = {254};
    unsigned long initiatorAddressLen = 1;
    U8 targetAddress = 254;
    char verifyBeforeWrite = 1;
    char incrementAddress = 0;
    RMAPPacket::RMAPStatus writeStatus = RMAPPacket::RMAP_STATUS_SUCCESS;
    U16 transactionID = 0;
    char alignment = 1;
    RMAPWriteReplyPacket writeReplyPacket;

    /* display test header */
    puts("\n#####\nRMAP - Write Reply Packet Example\n");

    /* build an RMAP write reply packet */
    if (!writeReplyPacket.CreateWriteReplyPacket(pInitiatorAddress,
                                                        initiatorAddressLen,
                                                        targetAddress,
                                                        verifyBeforeWrite,
                                                        incrementAddress,
                                                        writeStatus,
                                                        transactionID,
                                                        alignment))
    {
        /* report error */
        puts("Couldn't build the write reply packet");

        /* end processing */
        return;
    }

    /* perform necessary processing with packet */

    /* e.g. display the packet's length */
    printf("\tLength of write reply packet object: %lu\n",
            writeReplyPacket.GetPacketLength());

    /* dispose of packet when no longer required */
    /* \note The packet object destructor will automatically dispose
     *       of the packet if required, if it isn't manually disposed of.
     */
    writeReplyPacket.Dispose();
}

/**
 * Perform example call to obtain the version information of the RMAP Packet
 * Library and to access the fields in the version information.
 */
void RMAPExamples::PerformVersionExample()
{
    /* declare variables required for test */
    RMAPVersion version;
    U16 edit;
    U8 patch;

    /* display test header */
    puts("\n#####\nRMAP - Version Example\n");

    /* get the RMAP version */
    version = RMAPPacketLibrary::GetVersion();

    /* display version info */
    printf("RMAP Packet Library v%d.%02d", version.GetMajor(),
                                           version.GetMinor());
    edit = version.GetEdit();
    if (edit)
    {
        printf(" edit %d", edit);
    }
    patch = version.GetPatch();
    if (patch)
    {
        printf(" patch level %d", patch);
    }
    puts("");
}

/**
 * Provides an example of using an RMAP write command to write a
 * value to the general purpose / user register on a device. The example
 * includes creating a packet stream item to contain the RMAP packet and then
 * transmitting it on an opened channel on a device.
 */
void RMAPExamples::WriteToUserRegister(Device** ppDevice)
{
    /* define variables required for test */
    U8 pTargetAddress[] = {0, 254};
    unsigned long targetAddressLen = 2;
    U8 pReplyAddress[] = {254};
    unsigned long replyAddressLen = 1;
    char acknowledge = 1;
    char verifyBeforeWrite = 1;
    char incrementAddress = 0;
    U8 key = 0x20;
    U16 transactionID = 0x1234;
    U32 writeAddress = 0x106; /* address of general purpose register on Brick */
    U8 extendedWriteAddress = 0;
    U32 registerValue = 0x89abcdef;
    char alignment = 1;
    RMAPWriteCommandPacket writeRegisterPacket;
    U32 deviceCount = 0;
    DeviceList deviceList;
    STAR_CHANNEL_MASK channelsOnDevice = 0;
    const char channelNumber = 0;
    Channel channel;
    Packet spaceWirePacket;
    TransmitOperation transmitOp;
    ReceiveOperation receiveOp;
    TransferOperation::TransferStatus transmitStatus;
    TransferOperation::TransferStatus receiveStatus;

    /* get pointer to packet required for TransmitOperation */
    Packet *pSpaceWirePacket = &spaceWirePacket;

    /* display test header */
    puts("\n#####\nRMAP - Writing 4 byte value to User register on Brick \n");

    /* get a list of all devices present */
    *ppDevice = DeviceSelector::ChooseDevice();
    if (*ppDevice == NULL)
    {
        puts("No device selected");
        return;
    }

    /* create an RMAP write command packet to write to a 4-byte register */
    if (!writeRegisterPacket.CreateWriteRegisterPacket(pTargetAddress,
                                                        targetAddressLen,
                                                        pReplyAddress,
                                                        replyAddressLen,
                                                        verifyBeforeWrite,
                                                        acknowledge,
                                                        incrementAddress,
                                                        key,
                                                        transactionID,
                                                        writeAddress,
                                                        extendedWriteAddress,
                                                        registerValue,
                                                        alignment))
    {
        /* report error */
        puts("Couldn't build the write register packet");

        /* end processing */
        return;
    }

    /* get the channels available on the device */
    puts("Obtaining channels on device...");
    channelsOnDevice = (*ppDevice)->GetDeviceChannels();

    /* if the required channel doesn't exist on the device */
    if (!(channelsOnDevice & (1 << channelNumber)))
    {
        /* report that the required channel doesn't exist */
        printf("Sorry, the selected device doesn't have a channel %d.\n",
                channelNumber);

        /* end processing  */
        goto cleanup_nochannel;
    }

    /* open the selected channel on the device, for bidirectional use */
    puts("Opening channel on device...");
    channel = (*ppDevice)->OpenChannel(Channel::DIRECTION_INOUT,
                                   channelNumber, 1);

    /* if unable to open channel, i.e. no valid channel obtained */
    if (!channel.IsOpen())
    {
        /* report unable to open the channel */
        puts("Sorry, unable to open the specified channel on the device.");

        /* end processing  */
        goto cleanup_nochannel;
    }

    /* create a SpaceWire packet stream item for the RMAP write command to
     * be transmitted, terminated with an EOP. */
    if (!spaceWirePacket.CreatePacket((Address*)NULL,
                        (unsigned char*)writeRegisterPacket.GetPacketData(),
                        writeRegisterPacket.GetPacketLength(),
                        Packet::EOP_TYPE_EOP))
    {
        /* report error */
        puts("Failed to create SpaceWire packet stream item for the RMAP "
                "read command.");

        /* end processing */
        goto cleanup_buildSpaceWirePacketFailed;
    }

    /* create a transmit operation to transmit the SpaceWire packet */
    puts("Creating transmit operation to transmit the SpaceWire packet...");
    if (!transmitOp.CreateTxOperation((StreamItem **)&pSpaceWirePacket, 1))
    {
        /* report error */
        puts("Failed to create transmit operation for RMAP write command.");

        /* end processing */
        goto cleanup_createTransmitOpFailed;
    }

    /* create a receive operation for receiving one packet, the RMAP reply */
    puts("Creating receive operation to receive the RMAP reply...");
    if (!receiveOp.CreateRxOperation(1, ReceiveOperation::RECEIVE_PACKETS))
    {
        /* report error */
        puts("Failed to create receive operation for RMAP reply.");

        /* end processing */
        goto cleanup_createReceiveOpFailed;
    }

    /* start receiving a packet on the channel */
    puts("Initiating receive for RMAP reply packet...");
    channel.SubmitTransferOperation(&receiveOp);

    /* start transmitting the packet on the channel opened on the device */
    puts("Initiating transmit of RMAP packet...");
    channel.SubmitTransferOperation(&transmitOp);

    /* wait on the transmit completing, indefinitely (-1)  */
    transmitStatus = transmitOp.WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

    /* if transmit didn't complete */
    if (transmitStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        printf("Could not transmit packet, error of %d.\n", transmitStatus);
    }

    /* wait indefinitely on the reply packet being received */
    puts("Waiting indefinitely to receive the RMAP reply packet...");
    receiveStatus = receiveOp.WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

    /* if receive didn't complete */
    if (receiveStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        printf("Could not receive packet, error of %d.\n", receiveStatus);
    }

    /* dispose of the receive op created to receive the RMAP read command */
    receiveOp.DisposeTransferOperation();

cleanup_createReceiveOpFailed:
    /* dispose of the transmit op created to transmit the RMAP read command */
    transmitOp.DisposeTransferOperation();

cleanup_createTransmitOpFailed:
    /* destroy the SpaceWire packet created to transmit the RMAP write command */
    spaceWirePacket.DestroyPacket();

cleanup_buildSpaceWirePacketFailed:
    /* close the channel opened on the device */
    channel.CloseChannel();

cleanup_nochannel:
    /* free list of devices obtained from the STAR API */
    /* \note This is not required, the DeviceList object destructor will
     * free the contents of the list if they haven't been disposed of
     * already. */
    deviceList.Empty();

cleanup_nodevices:
    /* dispose of RMAP write register packet */
    /* \note The packet object destructor will automatically dispose
     *       of the packet if required, if it isn't manually disposed of.
     */
    writeRegisterPacket.Dispose();
}

/**
 * Provides an example of using the C++ version of the RMAP Packet Library API
 * to transmit an RMAP read command packet to read the value of the general
 * purpose / user register on a device and then receive an RMAP ready reply
 * packet which contains the data requested. The example includes the following:
 * (i) creating a packet stream item to contain the RMAP read command packet 
 * and transmitting it on an opened channel on a device; (ii) receiving a 
 * packet stream item containing the RMAP read reply and extracting the data it
 * contains.
 */
void RMAPExamples::ReadFromUserRegister(Device *pDevice)
{
    /* define variables required for test */
    U32 deviceCount = 0;
    DeviceList deviceList;
    STAR_CHANNEL_MASK channelsOnDevice = 0;
    const char channelNumber = 0;
    Channel channel;
    RMAPReadCommandPacket readCommandPacket;
    U8 targetAddress[] = { 0, 254 };
    const unsigned long targetAddressLen = sizeof(targetAddress);
    U8 replyAddress[] = { 254 };
    const unsigned long replyAddressLen = sizeof(replyAddress);
    const char alignment = 1, incrementAddress = 1;
    const U16 transactionIdentifier = 0x1235;
    const U32 readAddress = 0x106, readLength = 4;
    const U8 key = 0x20, extendedReadAddress = 0;
    Packet transmitPacket;
    TransmitOperation transmitOp;
    ReceiveOperation receiveOp;
    TransferOperation::TransferStatus receiveStatus;
    StreamItem *pReceivedStreamItem = NULL;
    Packet *pReceivedSpaceWirePacket = NULL;
    RMAPPacket *pReceivedRMAPPacket = NULL;
    RMAPPacket::RMAPStatus receivedRMAPPacketStatus;
    U8* pRMAPData = NULL;
    U32 rmapDataLen, i;

    /* get pointer to packet required for transmit operation */
    Packet *pTransmitPacket = &transmitPacket;

    /* display program header */
    puts("\n### RMAP Initiator Sample ###");

    /* get the channels available on the device */
    puts("Obtaining channels on device...");
    channelsOnDevice = pDevice->GetDeviceChannels();

    /* if the required channel doesn't exist on the device */
    if (!(channelsOnDevice & (1 << channelNumber)))
    {
        /* report that the required channel doesn't exist */
        printf("Sorry, the selected device doesn't have a channel %d.\n",
                channelNumber);

        /* end processing  */
        goto channelFailed;
    }

    /* open the selected channel on the device, for bidirectional use */
    puts("Opening channel on device...");
    channel = pDevice->OpenChannel(Channel::DIRECTION_INOUT,
                                   channelNumber, 1);

    /* if unable to open channel, i.e. no valid channel obtained */
    if (!channel.IsOpen())
    {
        /* report unable to open the channel */
        puts("Sorry, unable to open the specified channel on the device.");

        /* end processing  */
        goto channelFailed;
    }

    /* build an RMAP read command packet to read 4 bytes */
    puts("Building RMAP read command packet...");
    if (!readCommandPacket.CreateReadCommandPacket(targetAddress,
                                                        targetAddressLen,
                                                        replyAddress,
                                                        replyAddressLen,
                                                        incrementAddress,
                                                        key,
                                                        transactionIdentifier,
                                                        readAddress,
                                                        extendedReadAddress,
                                                        readLength,
                                                        alignment))
    {
        /* report error */
        puts("Couldn't build the read command packet");

        /* end processing */
        goto buildPacketFailed;
    }

    /* create a packet to transmit the RMAP read command, terminated with an
     * EOP
     */
    puts("Building SpaceWire packet stream item for RMAP read command...");
    if (!transmitPacket.CreatePacket((Address*)NULL,
                        (unsigned char*)readCommandPacket.GetPacketData(),
                        readCommandPacket.GetPacketLength(),
                        Packet::EOP_TYPE_EOP))
    {
        /* report error */
        puts("Failed to create SpaceWire packet stream item for the RMAP "
                "read command.");

        /* end processing */
        goto buildSpaceWirePacketFailed;

    }

    /* create a transmit operation to transmit the SpaceWire packet */
    puts("Creating transmit operation to transmit the SpaceWire packet...");
    if (!transmitOp.CreateTxOperation((StreamItem **)&pTransmitPacket, 1))
    {
        /* report error */
        puts("Failed to create transmit operation for RMAP read command.");

        /* end processing */
        goto createTransmitOpFailed;
    }

    /* create a receive operation for receiving one packet, the RMAP reply */
    puts("Creating receive operation to receive the RMAP reply...");
    if (!receiveOp.CreateRxOperation(1, ReceiveOperation::RECEIVE_PACKETS))
    {
        /* report error */
        puts("Failed to create receive operation for RMAP reply.");

        /* end processing */
        goto createReceiveOpFailed;
    }

    /* start receiving a packet on the channel */
    puts("Initiating receive for RMAP reply packet...");
    channel.SubmitTransferOperation(&receiveOp);

    /* start transmitting the packet on the channel opened on the device */
    puts("Initiating transmit of RMAP packet...");
    channel.SubmitTransferOperation(&transmitOp);

    /* wait indefinitely on the reply packet being received */
    puts("Waiting indefinitely to receive the RMAP reply packet...");
    receiveStatus = receiveOp.WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

    /* if failed to receive the reply packet */
    if (receiveStatus != TransferOperation::TRANSFER_STATUS_COMPLETE)
    {
        /* report error */
        printf("Error: could not receive packet. Error of %d.\n",
                receiveStatus);

        /* end processing */
        goto receiveFailed;
    }

    /* get the stream item received */
    puts("Getting stream item received...");
    pReceivedStreamItem = receiveOp.GetTransferItem(0);

    /* if failed to receive stream item */
    if (!pReceivedStreamItem)
    {
        /* report error */
        puts("Unable to obtain the stream item received");

        /* end processing */
        goto receiveFailed;
    }

    /* the stream item received is expected to be a SpaceWire packet.
     * Therefore, cast the object from being a StreamItem object (parent
     * class) to the more specific form of a Packet (child class
     * of StreamItem), in order to obtain the packet's data.
     *
     * For more robust error checking, one can call the GetStreamItemType()
     * method on the received StreamItem object, and determine whether it is
     * of type StreamItemType::PACKET, prior to casting.
     */
    pReceivedSpaceWirePacket = (Packet*)pReceivedStreamItem;

    /* the received packet is expected to contain an RMAP read reply with
     * the data requested by the RMAP read command. Extract the RMAP packet
     * from the received packet.
     */
    puts("Extracting RMAP packet from the received packet stream item...");
    pReceivedRMAPPacket = RMAPPacketLibrary::GetRMAPPacketFromStreamItem(
                                                pReceivedSpaceWirePacket,
                                                1, &receivedRMAPPacketStatus);

    /* if failed to obtain valid RMAP packet from the received packet data */
    if (!pReceivedRMAPPacket)
    {
        /* report error */
        puts("Unable to extract a valid RMAP packet from the received "
                " packet data.");
        printf("RMAP status: %d\n", receivedRMAPPacketStatus);

        /* end processing */
        goto rmapPacketFromReceivedStreamItemFailed;
    }

    /* if the RMAP packet obtained isn't a read reply packet, as expected */
    if (pReceivedRMAPPacket->GetPacketType() !=
                                    RMAPPacket::RMAP_PACKET_TYPE_READ_REPLY)
    {
        /* report error */
        puts("The packet received is not an RMAP read reply packet.");
        switch (pReceivedRMAPPacket->GetPacketType())
        {
        case RMAPPacket::RMAP_PACKET_TYPE_INVALID:
            puts("Packet type: INVALID");
            break;
        case RMAPPacket::RMAP_PACKET_TYPE_READ_COMMAND:
            puts("Packet type: READ COMMAND");
            break;
        case RMAPPacket::RMAP_PACKET_TYPE_READ_MODIFY_WRITE_COMMAND:
            puts("Packet type: R-M-W COMMAND");
            break;
        case RMAPPacket::RMAP_PACKET_TYPE_READ_MODIFY_WRITE_REPLY:
            puts("Packet type: R-M-W REPLY");
            break;
        case RMAPPacket::RMAP_PACKET_TYPE_READ_REPLY:
            puts("Packet type: READ REPLY");
            break;
        case RMAPPacket::RMAP_PACKET_TYPE_WRITE_COMMAND:
            puts("Packet type: WRITE COMMAND");
            break;
        case RMAPPacket::RMAP_PACKET_TYPE_WRITE_REPLY:
            puts("Packet type: WRITE REPLY");
            break;
        default:
            puts("Packet type: UNKNOWN");
            break;
        }

        /* end processing */
        goto rmapFailed;
    }

    /* check the status in the reply packet is a success */
    receivedRMAPPacketStatus = pReceivedRMAPPacket->GetStatus();
    if (receivedRMAPPacketStatus != RMAPPacket::RMAP_STATUS_SUCCESS)
    {
        puts("The reply packet contains an error status");

        /* end processing */
        goto rmapFailed;
    }

    /* if the RMAP reply packet's transaction identifier isn't the same as
     * that provided in the originally transmitted read command */
    if (transactionIdentifier != pReceivedRMAPPacket->GetTransactionID())
    {
        /* report error */
        puts("The packet received has an unexpected transaction identifier");
        printf("Expected a value of 0x%x, received a value of 0x%x\n",
            transactionIdentifier, pReceivedRMAPPacket->GetTransactionID());

        /* end processing */
        goto rmapFailed;
    }

    /* get a pointer to the data held within the read reply packet */
    puts("Getting the data from the RMAP read reply packet...");
    pRMAPData = pReceivedRMAPPacket->GetData(&rmapDataLen);

    /* if no data retrieved */
    if (!pRMAPData)
    {
        /* report error */
        puts("Unable to get the data in the RMAP read reply packet.");

        /* end processing */
        goto rmapFailed;
    }

    /* if the length of the data in read reply isn't as expected */
    if (rmapDataLen != readLength)
    {
        /* report error */
        puts("Unexpected data length in the RMAP read reply packet.");
        printf("Expected a length of %d, received a length of %d\n",
                readLength, rmapDataLen);

        /* end processing */
        goto rmapFailed;
    }

    /* display the data in the reply */
    puts("Data read:");
    for (i =0; i< rmapDataLen; i++)
    {
        printf(" %x", pRMAPData[i]);
    }
    puts(".");


rmapFailed:
    /* destroy the RMAP packet extracted from the received packet data */
    delete pReceivedRMAPPacket;

rmapPacketFromReceivedStreamItemFailed:
    /* delete the stream item received */
    delete pReceivedStreamItem;

receiveFailed:
    /* dispose of the receive op created to receive the RMAP reply */
    /* \note This isn't required as the receive operation object will
     * automatically dispose of the operation it holds in its destructor if
     * required when it is deleted or goes out of scope. However, you may wish
     * to manually dispose of the operation at a specified time during your
     * processing stages, to have control over when you wish to free the memory
     * it occupies.
     */
    receiveOp.DisposeTransferOperation();

createReceiveOpFailed:
    /* dispose of the transmit op created to transmit the RMAP read command */
    /* \note This isn't required as the transmit operation object will
     * automatically dispose of the operation it holds in its destructor if
     * required when it is deleted or goes out of scope. However, you may wish
     * to manually dispose of the operation at a specified time during your
     * processing stages, to have control over when you wish to free the memory
     * it occupies.
     */
    transmitOp.DisposeTransferOperation();

createTransmitOpFailed:
    /* destroy the SpaceWire packet created to transmit the RMAP read command */
    /* \note This isn't required as the packet object will automatically
     * destroy the packet it contains in its destructor if required when it is
     * deleted or goes out of scope. However, you may wish to manually destroy
     * the packet at a specified time during your processing stages,  to have
     * control over when you wish to free the memory it occupies.
     */
    transmitPacket.DestroyPacket();

buildSpaceWirePacketFailed:
    /* dispose of the RMAP read command packet */
    /* \note This isn't required as the packet object will automatically
     * dispose of the packet it contains in its destructor if required when it
     * is deleted or goes out of scope. However, you may wish to manually
     * dispose of the packet at a specified time during your processing stages,
     * to have control over when you wish to free the memory it occupies.
     */
    readCommandPacket.Dispose();

buildPacketFailed:
    /* close the channel opened on the device */
    channel.CloseChannel();

channelFailed:
    /* free list of devices obtained from the STAR API */
    deviceList.Empty();
}

