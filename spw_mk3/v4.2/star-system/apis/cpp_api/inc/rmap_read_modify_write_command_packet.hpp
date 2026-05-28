/**
 * \file rmap_read_modify_write_command_packet.hpp
 *
 * \brief Represents an RMAP read-modify-write command packet.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents an RMAP read-modify-write command packet.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_RMAP
 */

#ifndef __RMAP_READ_MODIFY_WRITE_COMMAND_PACKET_CPP__
#define __RMAP_READ_MODIFY_WRITE_COMMAND_PACKET_CPP__

#include "rmap_packet.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmap {

/**
* Represents an RMAP read-modify-write command packet.
*/
class RMAPReadModifyWriteCommandPacket : public RMAPPacket
{
private:
    /**
    * Build an RMAP read-modify-write command packet which can be sent to
    * perform an RMAP read-modify-write command.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be written to.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readModifyWriteAddress The memory address at the destination to
    *                               to read from and write to.
    * @param extendedReadModifyWriteAddress The extended memory address at the
    *                                       destination to read from and write to.
    * @param dataAndMaskLength The combined length in bytes of the data and mask
    *                          fields, this should be an even number between 0
    *                          and 8.
    * @param pData The data to be written.
    * @param pMask The mask to be applied to the data.
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
    *                   normally 1. See Byte Alignment for more information
    *
    * @return The packet generated, or NULL if there was an error, for example
    *         if one of the fields is invalid.
    */
    void *BuildReadModifyWriteCommandPacket(U8 *pTargetAddress,
                                 unsigned long targetAddressLength,
                                 U8 *pReplyAddress,
                                 unsigned long replyAddressLength,
                                 U8 key, U16 transactionIdentifier,
                                 U32 readModifyWriteAddress,
                                 U8 extendedReadModifyWriteAddress,
                                 U8 dataAndMaskLength, U8 *pData, U8 *pMask,
                                 unsigned long *pRawPacketLength,
                                 RMAP_PACKET *pPacketStruct, char alignment)
    {
        /* call corresponding function in the 'c' API */
        return RMAP_BuildReadModifyWriteCommandPacket(pTargetAddress,
                                           targetAddressLength, pReplyAddress,
                                           replyAddressLength, key,
                                           transactionIdentifier,
                                           readModifyWriteAddress,
                                           extendedReadModifyWriteAddress,
                                           dataAndMaskLength, pData, pMask,
                                           pRawPacketLength, pPacketStruct,
                                           alignment);
    }

    /**
    * Build an RMAP read-modify-write command packet which can be sent to
    * perform an RMAP read-modify-write command on 4-byte register.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be written to.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readModifyWriteAddress The memory address at the destination to
    *                               to read from and write to.
    * @param extendedReadModifyWriteAddress The extended memory address at the
    *                                       destination to read from and write to.
    * @param registerValue The value to be written to the register.
    * @param mask The mask to be applied to the data.
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
    void *BuildReadModifyWriteRegisterPacket(U8 *pTargetAddress,
                                 unsigned long targetAddressLength,
                                 U8 *pReplyAddress,
                                 unsigned long replyAddressLength,
                                 U8 key, U16 transactionIdentifier,
                                 U32 readModifyWriteAddress,
                                 U8 extendedReadModifyWriteAddress,
                                 U32 registerValue, U32 mask,
                                 unsigned long *pRawPacketLength,
                                 RMAP_PACKET *pPacketStruct, char alignment)
    {
        /* call corresponding function in the 'c' API */
        return RMAP_BuildReadModifyWriteRegisterPacket(pTargetAddress,
                                           targetAddressLength, pReplyAddress,
                                           replyAddressLength, key,
                                           transactionIdentifier,
                                           readModifyWriteAddress,
                                           extendedReadModifyWriteAddress,
                                           registerValue, mask,
                                           pRawPacketLength, pPacketStruct,
                                           alignment);
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPReadModifyWriteCommandPacket() : RMAPPacket()
    {
        /* call base class constructor to initialise */
    }

    /**
    * Overloaded constructor. Automatically attempt to create/build an RMAP
    * read-modify-write command packet using the values provided.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be written to.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readModifyWriteAddress The memory address at the destination to
    *                               to read from and write to.
    * @param extendedReadModifyWriteAddress The extended memory address at the
    *                                       destination to read from and write to.
    * @param dataAndMaskLength The combined length in bytes of the data and mask
    *                          fields, this should be an even number between 0
    *                          and 8.
    * @param pData The data to be written.
    * @param pMask The mask to be applied to the data.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * \throws RMAPPacketException if the RMAP Packet fails to create.
    *
    */
    RMAPReadModifyWriteCommandPacket(U8 *pTargetAddress,
                          unsigned long targetAddressLength, U8 *pReplyAddress,
                          unsigned long replyAddressLength,
                          U8 key, U16 transactionIdentifier,
                          U32 readModifyWriteAddress,
                          U8 extendedReadModifyWriteAddress,
                          U8 dataAndMaskLength, U8 *pData, U8 *pMask,
                          char alignment)
    {
        /* initialise object member variables to default values */
        InitialiseToDefaultState();

        /* attempt to build an RMAP read-modify-write command packet using the
         * values provided */
        void *pPacket = BuildReadModifyWriteCommandPacket(pTargetAddress,
                                               targetAddressLength, pReplyAddress,
                                               replyAddressLength,
                                               key, transactionIdentifier,
                                               readModifyWriteAddress,
                                               extendedReadModifyWriteAddress,
                                               dataAndMaskLength, pData, pMask,
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
    * Overloaded constructor. Automatically attempt to create/build an RMAP
    * read-modify-write command packet which can be sent to perform an RMAP
    * read-modify-write command on 4-byte register.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be written to.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readModifyWriteAddress The memory address at the destination to
    *                               to read from and write to.
    * @param extendedReadModifyWriteAddress The extended memory address at the
    *                                       destination to read from and write to.
    * @param registerValue The value to be written to the register.
    * @param mask The mask to be applied to the data.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * \throws RMAPPacketException if the RMAP Packet fails to create.
    *
    */
    RMAPReadModifyWriteCommandPacket(U8 *pTargetAddress,
                          unsigned long targetAddressLength, U8 *pReplyAddress,
                          unsigned long replyAddressLength,
                          U8 key, U16 transactionIdentifier,
                          U32 readModifyWriteAddress,
                          U8 extendedReadModifyWriteAddress,
                          U32 registerValue, U32 mask, char alignment)
    {
        /* initialise object member variables to default values */
        InitialiseToDefaultState();

        /* attempt to build an RMAP read-modify-write register packet using the
         * values provided */
        void *pPacket = BuildReadModifyWriteRegisterPacket(pTargetAddress,
                                               targetAddressLength, pReplyAddress,
                                               replyAddressLength,
                                               key, transactionIdentifier,
                                               readModifyWriteAddress,
                                               extendedReadModifyWriteAddress,
                                               registerValue, mask,
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
    * Create an RMAP read-modify-write command packet using the values provided.
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
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readModifyWriteAddress The memory address at the destination to
    *                               to read from and write to.
    * @param extendedReadModifyWriteAddress The extended memory address at the
    *                                       destination to read from and write
    *                                       to.
    * @param dataAndMaskLength The combined length in bytes of the data and mask
    *                          fields, this should be an even number between 0
    *                          and 8.
    * @param pData The data to be written.
    * @param pMask The mask to be applied to the data.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * @return 1 if write register packet created successfully, otherwise 0.
    *
    */
    int CreateReadModifyWriteCommandPacket(U8 *pTargetAddress,
                          unsigned long targetAddressLength, U8 *pReplyAddress,
                          unsigned long replyAddressLength,
                          U8 key, U16 transactionIdentifier,
                          U32 readModifyWriteAddress,
                          U8 extendedReadModifyWriteAddress,
                          U8 dataAndMaskLength, U8 *pData, U8 *pMask,
                          char alignment)
    {
        /* free any existing packet held by this object */
        CleanUp();

        /* attempt to build an RMAP read-modify-write command packet using the
         * values provided */
        void *pPacket = BuildReadModifyWriteCommandPacket(pTargetAddress,
                                               targetAddressLength,
                                               pReplyAddress,
                                               replyAddressLength,
                                               key, transactionIdentifier,
                                               readModifyWriteAddress,
                                               extendedReadModifyWriteAddress,
                                               dataAndMaskLength, pData, pMask,
                                               &packetLen, &rmapPacketStruct,
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
    * Create an RMAP read-modify-write command packet which can be sent to
    * perform an RMAP read-modify-write command on 4-byte register.
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
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readModifyWriteAddress The memory address at the destination to
    *                               to read from and write to.
    * @param extendedReadModifyWriteAddress The extended memory address at the
    *                                       destination to read from and write to.
    * @param registerValue The value to be written to the register.
    * @param mask The mask to be applied to the data.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * @return 1 if write register packet created successfully, otherwise 0.
    *
    */
    int CreateReadModifyWriteRegisterPacket(U8 *pTargetAddress,
                          unsigned long targetAddressLength, U8 *pReplyAddress,
                          unsigned long replyAddressLength,
                          U8 key, U16 transactionIdentifier,
                          U32 readModifyWriteAddress,
                          U8 extendedReadModifyWriteAddress,
                          U32 registerValue, U32 mask, char alignment)
    {
        /* free any existing packet held by this object */
        CleanUp();

        /* attempt to build an RMAP read-modify-write register packet using the
         * values provided */
        void *pPacket = BuildReadModifyWriteRegisterPacket(pTargetAddress,
                                               targetAddressLength, pReplyAddress,
                                               replyAddressLength,
                                               key, transactionIdentifier,
                                               readModifyWriteAddress,
                                               extendedReadModifyWriteAddress,
                                               registerValue, mask,
                                               &packetLen, &rmapPacketStruct,
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
    virtual ~RMAPReadModifyWriteCommandPacket()
    {
        /* Allow base class destructor (which will be called after this)
         * to clean up as necessary */
    }
};

/* end namespace tags */
} /* rmap */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

