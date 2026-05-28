/**
 * \file rmap_read_command_packet.hpp
 *
 * \brief Represents an RMAP read command packet.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents an RMAP read command packet.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_RMAP
 */

#ifndef __RMAP_READ_COMMAND_PACKET_CPP__
#define __RMAP_READ_COMMAND_PACKET_CPP__

#include "rmap_packet.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmap {

/**
* Represents an RMAP read command packet.
*/
class RMAPReadCommandPacket : public RMAPPacket
{
private:
    /**
    * Build an RMAP read command packet which can be sent to perform an RMAP
    * read command.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be read from.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param incrementAddress Whether or not the target read address should be
    *                         incremented when reading.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readAddress The memory address at the destination to read from.
    * @param extendedReadAddress The extended memory address at the destination
    *                            to read from.
    * @param dataLength The length of data to read, which is a 24-bit number and
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
    void *BuildReadCommandPacket(U8 *pTargetAddress,
                                 unsigned long targetAddressLength,
                                 U8 *pReplyAddress,
                                 unsigned long replyAddressLength,
                                 char incrementAddress, U8 key,
                                 U16 transactionIdentifier, U32 readAddress,
                                 U8 extendedReadAddress, U32 dataLength,
                                 unsigned long *pRawPacketLength,
                                 RMAP_PACKET *pPacketStruct, char alignment)
    {
        /* call corresponding function in the 'c' API */
        return RMAP_BuildReadCommandPacket(pTargetAddress, targetAddressLength,
                                           pReplyAddress, replyAddressLength,
                                           incrementAddress, key,
                                           transactionIdentifier, readAddress,
                                           extendedReadAddress, dataLength,
                                           pRawPacketLength, pPacketStruct,
                                           alignment);
    }

    /**
    * Build an RMAP read command packet which can be sent to perform an RMAP
    * read command on a 4-byte register.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be read from.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param incrementAddress Whether or not the target read address should be
    *                         incremented when reading.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readAddress The memory address at the destination to read from.
    * @param extendedReadAddress The extended memory address at the destination
    *                            to read from.
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
    void *BuildReadRegisterPacket(U8 *pTargetAddress,
                                 unsigned long targetAddressLength,
                                 U8 *pReplyAddress,
                                 unsigned long replyAddressLength,
                                 char incrementAddress, U8 key,
                                 U16 transactionIdentifier, U32 readAddress,
                                 U8 extendedReadAddress,
                                 unsigned long *pRawPacketLength,
                                 RMAP_PACKET *pPacketStruct, char alignment)
    {
        /* call corresponding function in the 'c' API */
        return RMAP_BuildReadRegisterPacket(pTargetAddress, targetAddressLength,
                                           pReplyAddress, replyAddressLength,
                                           incrementAddress, key,
                                           transactionIdentifier, readAddress,
                                           extendedReadAddress, pRawPacketLength,
                                           pPacketStruct, alignment);
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPReadCommandPacket() : RMAPPacket()
    {
        /* call base class constructor to initialise */
    }

    /**
    * Overloaded constructor. Automatically attempt to create/build an RMAP
    * read command packet using the values provided.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be read from.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param incrementAddress Whether or not the target read address should be
    *                         incremented when reading.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readAddress The memory address at the destination to read from.
    * @param extendedReadAddress The extended memory address at the destination
    *                            to read from.
    * @param dataLength The length of data to read, which is a 24-bit number and
    *                   so should be at most 0xffffff.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * \throws RMAPPacketException if the RMAP Packet fails to create.
    *
    */
    RMAPReadCommandPacket(U8 *pTargetAddress, unsigned long targetAddressLength,
                          U8 *pReplyAddress, unsigned long replyAddressLength,
                          char incrementAddress, U8 key,
                          U16 transactionIdentifier, U32 readAddress,
                          U8 extendedReadAddress, U32 dataLength, char alignment)
    {
        /* initialise object member variables to default values */
        InitialiseToDefaultState();

        /* attempt to build an RMAP read command packet using the values provided */
        void *pPacket = BuildReadCommandPacket(pTargetAddress,
                                               targetAddressLength, pReplyAddress,
                                               replyAddressLength,
                                               incrementAddress, key,
                                               transactionIdentifier, readAddress,
                                               extendedReadAddress, dataLength,
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
    * read command packet which can be sent to perform an RMAP read command on
    * a 4-byte register.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be read from.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param incrementAddress Whether or not the target read address should be
    *                         incremented when reading.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readAddress The memory address at the destination to read from.
    * @param extendedReadAddress The extended memory address at the destination
    *                            to read from.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * \throws RMAPPacketException if the RMAP Packet fails to create.
    *
    */
    RMAPReadCommandPacket(U8 *pTargetAddress, unsigned long targetAddressLength,
                          U8 *pReplyAddress, unsigned long replyAddressLength,
                          char incrementAddress, U8 key,
                          U16 transactionIdentifier, U32 readAddress,
                          U8 extendedReadAddress, char alignment)
    {
        /* initialise object member variables to default values */
        InitialiseToDefaultState();

        /* attempt to build an RMAP read register packet using the values provided */
        void *pPacket = BuildReadRegisterPacket(pTargetAddress,
                                               targetAddressLength, pReplyAddress,
                                               replyAddressLength,
                                               incrementAddress, key,
                                               transactionIdentifier, readAddress,
                                               extendedReadAddress, &packetLen,
                                               &rmapPacketStruct, alignment);

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
    * Create an RMAP read command packet using the values provided.
    *
    * \note If a packet already exists it will be disposed of and a new packet
    *       created with the values provided.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be read from.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param incrementAddress Whether or not the target read address should be
    *                         incremented when reading.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readAddress The memory address at the destination to read from.
    * @param extendedReadAddress The extended memory address at the destination
    *                            to read from.
    * @param dataLength The length of data to read, which is a 24-bit number and
    *                   so should be at most 0xffffff.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * @return 1 if read command created successfully, otherwise 0.
    *
    */
    int CreateReadCommandPacket(U8 *pTargetAddress,
                          unsigned long targetAddressLength,
                          U8 *pReplyAddress, unsigned long replyAddressLength,
                          char incrementAddress, U8 key,
                          U16 transactionIdentifier, U32 readAddress,
                          U8 extendedReadAddress, U32 dataLength,
                          char alignment)
    {
        /* free any existing packet held by this object */
        CleanUp();

        /* attempt to build an RMAP read command packet using the values
         * provided */
        void *pPacket = BuildReadCommandPacket(pTargetAddress,
                                               targetAddressLength,
                                               pReplyAddress,
                                               replyAddressLength,
                                               incrementAddress, key,
                                               transactionIdentifier,
                                               readAddress,
                                               extendedReadAddress,
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
    * Create an RMAP read command packet which can be sent to perform an RMAP
    * read command on a 4-byte register.
    *
    * \note If a packet already exists it will be disposed of and a new packet
    *       created with the values provided.
    *
    * @param pTargetAddress A pointer to the SpaceWire path or logical address
    *                       of the device to be read from.
    * @param targetAddressLength The length of the target address.
    * @param pReplyAddress A pointer to the SpaceWire path or logical address
    *                      that will be used to send any responses back to the
    *                      device from which the command will be sent from.
    * @param replyAddressLength The length of the reply address.
    * @param incrementAddress Whether or not the target read address should be
    *                         incremented when reading.
    * @param key The key expected by the destination device.
    * @param transactionIdentifier An identifier for the transaction.
    * @param readAddress The memory address at the destination to read from.
    * @param extendedReadAddress The extended memory address at the destination
    *                            to read from.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * @return 1 if RMAP packet created successfully, otherwise 0.
    *
    */
    int CreateReadRegisterPacket(U8 *pTargetAddress,
                          unsigned long targetAddressLength,
                          U8 *pReplyAddress, unsigned long replyAddressLength,
                          char incrementAddress, U8 key,
                          U16 transactionIdentifier, U32 readAddress,
                          U8 extendedReadAddress, char alignment)
    {
         /* free any existing packet held by this object */
        CleanUp();

        /* attempt to build an RMAP read register packet using the values
         * provided */
        void *pPacket = BuildReadRegisterPacket(pTargetAddress,
                                               targetAddressLength,
                                               pReplyAddress,
                                               replyAddressLength,
                                               incrementAddress, key,
                                               transactionIdentifier,
                                               readAddress,
                                               extendedReadAddress,
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
    virtual ~RMAPReadCommandPacket()
    {
        /* Allow base class destructor (which will be called after this)
         * to clean up as necessary */
    }
};

/* end namespace tags */
} /* rmap */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

