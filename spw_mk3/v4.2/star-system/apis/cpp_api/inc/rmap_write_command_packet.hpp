/**
 * \file rmap_write_command_packet.hpp
 *
 * \brief Represents an RMAP write command packet.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents an RMAP write command packet.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_RMAP
 */

#ifndef __RMAP_WRITE_COMMAND_PACKET_CPP__
#define __RMAP_WRITE_COMMAND_PACKET_CPP__

#include "rmap_packet.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmap {

/**
* Represents an RMAP write command packet.
*/
class RMAPWriteCommandPacket : public RMAPPacket
{
private:
    /**
    * Build an RMAP write command packet which can be sent to perform an RMAP
    * write command.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be written to.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param verifyBeforeWrite Whether or not the data should be verified before
    *                          writing
    * @param acknowledge Whether or not the command should be acknowledged.
    * @param incrementAddress Whether or not the target write address should be
    *                         incremented when writing.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param writeAddress The memory address at the destination to write to.
    * @param extendedWriteAddress The extended memory address at the destination
    *                            to write to.
    * @param pData The data to be written.
    * @param dataLength The length of data, which is a 24-bit number and
    *                   so should be at most 0xffffff.
    * @param pRawPacketLength A pointer to a previously allocated variable which
    *                         will be updated to contain the length of the
    *                         packet returned
    * @param pPacketStruct An optional structure which will be updated to
    *                      contain details of the fields in the packet. This
    *                      structure should not be accessed directly, but by
    *                      using other functions provided by the API.
    *                      Alternatively the parameter can be left as NULL if
    *                      the properties of the packet are not of interest.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * @return The packet generated, or NULL if there was an error, for example
    *         if one of the fields is invalid.
    */
    void *BuildWriteCommandPacket(U8 *pTargetAddress,
                                 unsigned long targetAddressLength,
                                 U8 *pReplyAddress,
                                 unsigned long replyAddressLength,
                                 char verifyBeforeWrite,
                                 char acknowledge,
                                 char incrementAddress, U8 key,
                                 U16 transactionIdentifier, U32 writeAddress,
                                 U8 extendedWriteAddress, U8 *pData,
                                 U32 dataLength, unsigned long *pRawPacketLength,
                                 RMAP_PACKET *pPacketStruct, char alignment)
    {
        /* call corresponding function in the 'c' API */
        return RMAP_BuildWriteCommandPacket(pTargetAddress, targetAddressLength,
                                           pReplyAddress, replyAddressLength,
                                           verifyBeforeWrite, acknowledge,
                                           incrementAddress, key,
                                           transactionIdentifier, writeAddress,
                                           extendedWriteAddress, pData,
                                           dataLength, pRawPacketLength,
                                           pPacketStruct, alignment);
    }

    /**
    * Build an RMAP write command packet which can be sent to perform an RMAP
    * write command on a 4-byte register.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be written to.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param verifyBeforeWrite Whether or not the data should be verified before
    *                          writing
    * @param acknowledge Whether or not the command should be acknowledged.
    * @param incrementAddress Whether or not the target write address should be
    *                         incremented when writing.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param writeAddress The memory address at the destination to write to.
    * @param extendedWriteAddress The extended memory address at the destination
    *                            to write to.
    * @param registerValue The value to be written to the register
    * @param pRawPacketLength A pointer to a previously allocated variable which
    *                         will be updated to contain the length of the
    *                         packet returned
    * @param pPacketStruct An optional structure which will be updated to
    *                      contain details of the fields in the packet. This
    *                      structure should not be accessed directly, but by
    *                      using other functions provided by the API.
    *                      Alternatively the parameter can be left as NULL if
    *                      the properties of the packet are not of interest.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * @return The packet generated, or NULL if there was an error, for example
    *         if one of the fields is invalid.
    */
    void *BuildWriteRegisterPacket(U8 *pTargetAddress,
                                 unsigned long targetAddressLength,
                                 U8 *pReplyAddress,
                                 unsigned long replyAddressLength,
                                 char verifyBeforeWrite,
                                 char acknowledge,
                                 char incrementAddress, U8 key,
                                 U16 transactionIdentifier, U32 writeAddress,
                                 U8 extendedWriteAddress, U32 registerValue,
                                 unsigned long *pRawPacketLength,
                                 RMAP_PACKET *pPacketStruct, char alignment)
    {
        /* call corresponding function in the 'c' API */
        return RMAP_BuildWriteRegisterPacket(pTargetAddress, targetAddressLength,
                                           pReplyAddress, replyAddressLength,
                                           verifyBeforeWrite, acknowledge,
                                           incrementAddress, key,
                                           transactionIdentifier, writeAddress,
                                           extendedWriteAddress, registerValue,
                                           pRawPacketLength, pPacketStruct,
                                           alignment);
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPWriteCommandPacket() : RMAPPacket()
    {
        /* call base class constructor to initialise */
    }

    /**
    * Overloaded constructor. Automatically attempt to create/build an RMAP
    * write command packet using the values provided.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be written to.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param verifyBeforeWrite Whether or not the data should be verified before
    *                          writing
    * @param acknowledge Whether or not the command should be acknowledged.
    * @param incrementAddress Whether or not the target write address should be
    *                         incremented when writing.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param writeAddress The memory address at the destination to write to.
    * @param extendedWriteAddress The extended memory address at the destination
    *                            to write to.
    * @param pData The data to be written.
    * @param dataLength The length of data, which is a 24-bit number and
    *                   so should be at most 0xffffff.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * \throws RMAPPacketException if the RMAP Packet fails to create.
    *
    */
    RMAPWriteCommandPacket(U8 *pTargetAddress, unsigned long targetAddressLength,
                          U8 *pReplyAddress, unsigned long replyAddressLength,
                          char verifyBeforeWrite, char acknowledge,
                          char incrementAddress, U8 key,
                          U16 transactionIdentifier, U32 writeAddress,
                          U8 extendedWriteAddress, U8 *pData, U32 dataLength,
                          char alignment)
    {
        /* initialise object member variables to default values */
        InitialiseToDefaultState();

        /* attempt to build an RMAP write command packet using the values provided */
        void *pPacket = BuildWriteCommandPacket(pTargetAddress,
                                               targetAddressLength,
                                               pReplyAddress,
                                               replyAddressLength,
                                               verifyBeforeWrite, acknowledge,
                                               incrementAddress, key,
                                               transactionIdentifier,
                                               writeAddress,
                                               extendedWriteAddress, pData,
                                               dataLength, &packetLen,
                                               &rmapPacketStruct,
                                               alignment);

        /* if failed to build packet */
        if (pPacket == NULL)
        {
            /* throw exception */
            throw RMAPPacketException();
        }
        else
        {
            /* store reference to the packet obtained */
            pRMAPPacket = pPacket;

            /* store pointer to RMAP_PACKET struct which has been initialised
             * with the contents of the packet.
             *
             * NOTE: the pointer is required to assist processing of copy
             * constructors.
             */
            pRmapPacketStruct = &rmapPacketStruct;
        }
    }

    /**
    * Overloaded constructor. Automatically attempt to create/build an RMAP
    * write RMAP write command packet which can be sent to perform an RMAP
    * write command on a 4-byte register.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be written to.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param verifyBeforeWrite Whether or not the data should be verified before
    *                          writing
    * @param acknowledge Whether or not the command should be acknowledged.
    * @param incrementAddress Whether or not the target write address should be
    *                         incremented when writing.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param writeAddress The memory address at the destination to write to.
    * @param extendedWriteAddress The extended memory address at the destination
    *                            to write to.
    * @param registerValue The value to be written to the register.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * \throws RMAPPacketException if the RMAP Packet fails to create.
    *
    */
    RMAPWriteCommandPacket(U8 *pTargetAddress, unsigned long targetAddressLength,
                          U8 *pReplyAddress, unsigned long replyAddressLength,
                          char verifyBeforeWrite, char acknowledge,
                          char incrementAddress, U8 key,
                          U16 transactionIdentifier, U32 writeAddress,
                          U8 extendedWriteAddress, U32 registerValue,
                          char alignment)
    {
        /* initialise object member variables to default values */
        InitialiseToDefaultState();

        /* attempt to build an RMAP write register packet using the values provided */
        void *pPacket = BuildWriteRegisterPacket(pTargetAddress,
                                               targetAddressLength, pReplyAddress,
                                               replyAddressLength,
                                               verifyBeforeWrite, acknowledge,
                                               incrementAddress, key,
                                               transactionIdentifier, writeAddress,
                                               extendedWriteAddress, registerValue,
                                               &packetLen, &rmapPacketStruct,
                                               alignment);

        /* if failed to build packet */
        if (pPacket == NULL)
        {
            /* throw exception */
            throw RMAPPacketException();
        }
        else
        {
            /* store reference to the packet obtained */
            pRMAPPacket = pPacket;

            /* store pointer to RMAP_PACKET struct which has been initialised
             * with the contents of the packet.
             *
             * NOTE: the pointer is required to assist processing of copy
             * constructors.
             */
            pRmapPacketStruct = &rmapPacketStruct;
        }
    }

    /**
    * Create an RMAP write command packet using the values provided.
    *
    * \note If a packet already exists it will be disposed of and a new packet
    *       created with the values provided.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be written to.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param verifyBeforeWrite Whether or not the data should be verified before
    *                          writing
    * @param acknowledge Whether or not the command should be acknowledged.
    * @param incrementAddress Whether or not the target write address should be
    *                         incremented when writing.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param writeAddress The memory address at the destination to write to.
    * @param extendedWriteAddress The extended memory address at the destination
    *                            to write to.
    * @param pData The data to be written.
    * @param dataLength The length of data, which is a 24-bit number and
    *                   so should be at most 0xffffff.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * @return 1 if write command packet created successfully, otherwise 0.
    *
    */
    int CreateWriteCommandPacket(U8 *pTargetAddress,
                          unsigned long targetAddressLength,
                          U8 *pReplyAddress, unsigned long replyAddressLength,
                          char verifyBeforeWrite, char acknowledge,
                          char incrementAddress, U8 key,
                          U16 transactionIdentifier, U32 writeAddress,
                          U8 extendedWriteAddress, U8 *pData, U32 dataLength,
                          char alignment)
    {
        /* free any existing packet held by this object */
        CleanUp();

        /* attempt to build an RMAP write command packet using the values
         * provided */
        void *pPacket = BuildWriteCommandPacket(pTargetAddress,
                                               targetAddressLength,
                                               pReplyAddress,
                                               replyAddressLength,
                                               verifyBeforeWrite,
                                               acknowledge,
                                               incrementAddress, key,
                                               transactionIdentifier,
                                               writeAddress,
                                               extendedWriteAddress,
                                               pData,
                                               dataLength,
                                               &packetLen,
                                               &rmapPacketStruct,
                                               alignment);

        /* if failed to build packet */
        if (pPacket == NULL)
        {
            /* return fail code */
            return 0;
        }
        else
        {
            /* store reference to the packet obtained */
            pRMAPPacket = pPacket;

            /* store pointer to RMAP_PACKET struct which has been initialised
             * with the contents of the packet.
             *
             * NOTE: the pointer is required to assist processing of copy
             * constructors.
             */
            pRmapPacketStruct = &rmapPacketStruct;

            /* return success */
            return 1;
        }
    }

    /**
    * Create an RMAP write RMAP write command packet which can be sent to
    * perform an RMAP write command on a 4-byte register.
    *
    * \note If a packet already exists it will be disposed of and a new packet
    *       created with the values provided.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be written to.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param verifyBeforeWrite Whether or not the data should be verified before
    *                          writing
    * @param acknowledge Whether or not the command should be acknowledged.
    * @param incrementAddress Whether or not the target write address should be
    *                         incremented when writing.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param writeAddress The memory address at the destination to write to.
    * @param extendedWriteAddress The extended memory address at the destination
    *                            to write to.
    * @param registerValue The value to be written to the register.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * @return 1 if write register packet created successfully, otherwise 0.
    *
    */
    int CreateWriteRegisterPacket(U8 *pTargetAddress,
                          unsigned long targetAddressLength,
                          U8 *pReplyAddress, unsigned long replyAddressLength,
                          char verifyBeforeWrite, char acknowledge,
                          char incrementAddress, U8 key,
                          U16 transactionIdentifier, U32 writeAddress,
                          U8 extendedWriteAddress, U32 registerValue,
                          char alignment)
    {
        /* free any existing packet held by this object */
        CleanUp();

        /* attempt to build an RMAP write register packet using the values
         * provided */
        void *pPacket = BuildWriteRegisterPacket(pTargetAddress,
                                               targetAddressLength,
                                               pReplyAddress,
                                               replyAddressLength,
                                               verifyBeforeWrite,
                                               acknowledge,
                                               incrementAddress, key,
                                               transactionIdentifier,
                                               writeAddress,
                                               extendedWriteAddress,
                                               registerValue,
                                               &packetLen,
                                               &rmapPacketStruct,
                                               alignment);

        /* if failed to build packet */
        if (pPacket == NULL)
        {
            /* return fail code */
            return 0;
        }
        else
        {
            /* store reference to the packet obtained */
            pRMAPPacket = pPacket;

            /* store pointer to RMAP_PACKET struct which has been initialised
             * with the contents of the packet.
             *
             * NOTE: the pointer is required to assist processing of copy
             * constructors.
             */
            pRmapPacketStruct = &rmapPacketStruct;

            /* return success */
            return 1;
        }
    }

    /**
    * Destructor. Automatically dispose of the packet and its data if required,
    * if it hasn't been manually disposed of by the user (by calling the
    * Dispose() function).
    */
    virtual ~RMAPWriteCommandPacket()
    {
        /* Allow base class destructor (which will be called after this)
         * to clean up as necessary */
    }
};

/* end namespace tags */
} /* rmap */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

