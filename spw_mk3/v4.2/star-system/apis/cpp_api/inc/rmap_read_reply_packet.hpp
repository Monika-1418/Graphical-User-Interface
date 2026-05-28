/**
 * \file rmap_read_reply_packet.hpp
 *
 * \brief Represents an RMAP read reply packet.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents an RMAP read reply packet.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_RMAP
 */

#ifndef __RMAP_READ_REPLY_PACKET_CPP__
#define __RMAP_READ_REPLY_PACKET_CPP__

#include "rmap_packet.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmap {

/**
* Represents an RMAP read reply packet.
*/
class RMAPReadReplyPacket : public RMAPPacket
{
private:
    /**
    * Build an RMAP read reply packet which can be sent to respond to an RMAP
    * read command.
    *
    * @param pInitiatorAddress  A pointer to the SpaceWire path or logical address
    *                           of the device to which the reply should be sent.
    * @param initiatorAddressLength The length of the initiator address.
    * @param targetAddress The SpaceWire logical address of the device that
    *                      sent the command being responded to.
    * @param incrementAddress Whether or not the command indicated that the
    *                         target read address should be incremented when
    *                         reading.
    * @param status The status of the read operation
    * @param transactionIdentifier An identifier for the transaction.
    * @param pData The data read.
    * @param dataLength The length of the data, which is a 24-bit number and so
    *                   should be at most 0xffffff.
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
    void *BuildReadReplyPacket(U8 *pInitiatorAddress,
                               unsigned long initiatorAddressLength,
                               U8 targetAddress,
                               char incrementAddress,
                               RMAPStatus status,
                               U16 transactionIdentifier,
                               U8 *pData,
                               U32 dataLength,
                               unsigned long *pRawPacketLength,
                               RMAP_PACKET *pPacketStruct, char alignment)
    {
        /* call corresponding function in the 'c' API */
        return RMAP_BuildReadReplyPacket(pInitiatorAddress,
                                     initiatorAddressLength, targetAddress,
                                     incrementAddress,
                                     (RMAP_STATUS)status,
                                     transactionIdentifier,
                                     pData, dataLength,
                                     pRawPacketLength, pPacketStruct,
                                     alignment);
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPReadReplyPacket() : RMAPPacket()
    {
        /* call base class constructor to initialise */
    }

    /**
    * Overloaded constructor. Automatically attempt to create/build an RMAP
    * read reply packet using the values provided.
    *
    * @param pInitiatorAddress  A pointer to the SpaceWire path or logical address
    *                           of the device to which the reply should be sent.
    * @param initiatorAddressLength The length of the initiator address.
    * @param targetAddress The SpaceWire logical address of the device that
    *                      sent the command being responded to.
    * @param incrementAddress Whether or not the command indicated that the
    *                         target read address should be incremented when
    *                         reading.
    * @param status The status of the read operation
    * @param transactionIdentifier An identifier for the transaction.
    * @param pData The data read.
    * @param dataLength The length of the data, which is a 24-bit number and so
    *                   should be at most 0xffffff.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * \throws RMAPPacketException if the RMAP Packet fails to create.
    *
    */
    RMAPReadReplyPacket(U8 *pInitiatorAddress,
                        unsigned long initiatorAddressLength,
                        U8 targetAddress,
                        char incrementAddress,
                        RMAPStatus status,
                        U16 transactionIdentifier,
                        U8 *pData,
                        U32 dataLength,
                        char alignment)
    {
        /* initialise object member variables to default values */
        InitialiseToDefaultState();

        /* attempt to build an RMAP read reply packet using the
         * values provided */
        void *pPacket = BuildReadReplyPacket(pInitiatorAddress,
                                           initiatorAddressLength, targetAddress,
                                           incrementAddress,
                                           status, transactionIdentifier,
                                           pData, dataLength,
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
    * Create an RMAP read reply packet using the values provided.
    *
    * \note If a packet already exists it will be disposed of and a new packet
    *       created with the values provided.
    *
    * @param pInitiatorAddress  A pointer to the SpaceWire path or logical
    *                           address of the device to which the reply should
    *                           be sent.
    * @param initiatorAddressLength The length of the initiator address.
    * @param targetAddress The SpaceWire logical address of the device that
    *                      sent the command being responded to.
    * @param incrementAddress Whether or not the command indicated that the
    *                         target read address should be incremented when
    *                         reading.
    * @param status The status of the read operation
    * @param transactionIdentifier An identifier for the transaction.
    * @param pData The data read.
    * @param dataLength The length of the data, which is a 24-bit number and so
    *                   should be at most 0xffffff.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * @return 1 if write register packet created successfully, otherwise 0.
    *
    */
    int CreateReadReplyPacket(U8 *pInitiatorAddress,
                        unsigned long initiatorAddressLength,
                        U8 targetAddress,
                        char incrementAddress,
                        RMAPStatus status,
                        U16 transactionIdentifier,
                        U8 *pData,
                        U32 dataLength,
                        char alignment)
    {
        /* free any existing packet held by this object */
        CleanUp();

        /* attempt to build an RMAP read reply packet using the
         * values provided */
        void *pPacket = BuildReadReplyPacket(pInitiatorAddress,
                                           initiatorAddressLength,
                                           targetAddress,
                                           incrementAddress,
                                           status,
                                           transactionIdentifier,
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
    * Destructor. Automatically dispose of the packet and its data if required,
    * if it hasn't been manually disposed of by the user (by calling the
    * Dispose() function).
    */
    virtual ~RMAPReadReplyPacket()
    {
        /* Allow base class destructor (which will be called after this)
         * to clean up as necessary */
    }
};

/* end namespace tags */
} /* rmap */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

