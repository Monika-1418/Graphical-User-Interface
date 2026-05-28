/**
 * \file rmap_write_reply_packet.hpp
 *
 * \brief Represents an RMAP write reply packet.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents an RMAP write reply packet.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_RMAP
 */

#ifndef __RMAP_WRITE_REPLY_PACKET_CPP__
#define __RMAP_WRITE_REPLY_PACKET_CPP__

#include "rmap_packet.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmap {

/**
* Represents an RMAP write reply packet.
*/
class RMAPWriteReplyPacket : public RMAPPacket
{
private:
    /**
    * Build an RMAP write reply packet which can be sent to respond to an RMAP
    * write command.
    *
    * @param pInitiatorAddress  A pointer to the SpaceWire path or logical address
    *                           of the device to which the reply should be sent.
    * @param initiatorAddressLength The length of the initiator address.
    * @param targetAddress The SpaceWire logical address of the device that
    *                      sent the command being responded to.
    * @param verifyBeforeWrite Whether or not the command indicated that data
    *                          should be verified before writing
    * @param incrementAddress Whether or not the command indicated that the
    *                         target write address should be incremented when
    *                         writing.
    * @param status The status of the write operation
    * @param transactionIdentifier An identifier for the transaction.
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
    void *BuildWriteReplyPacket(U8 *pInitiatorAddress,
                               unsigned long initiatorAddressLength,
                               U8 targetAddress,
                               char verifyBeforeWrite,
                               char incrementAddress,
                               RMAPStatus status,
                               U16 transactionIdentifier,
                               unsigned long *pRawPacketLength,
                               RMAP_PACKET *pPacketStruct, char alignment)
    {
        /* call corresponding function in the 'c' API */
        return RMAP_BuildWriteReplyPacket(pInitiatorAddress,
                                     initiatorAddressLength, targetAddress,
                                     verifyBeforeWrite, incrementAddress,
                                     (RMAP_STATUS)status,
                                     transactionIdentifier,
                                     pRawPacketLength, pPacketStruct,
                                     alignment);
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPWriteReplyPacket() : RMAPPacket()
    {
        /* call base class constructor to initialise */
    }

    /**
    * Overloaded constructor. Automatically attempt to create/build an RMAP
    * write reply packet using the values provided.
    *
    * @param pInitiatorAddress  A pointer to the SpaceWire path or logical address
    *                           of the device to which the reply should be sent.
    * @param initiatorAddressLength The length of the initiator address.
    * @param targetAddress The SpaceWire logical address of the device that
    *                      sent the command being responded to.
    * @param verifyBeforeWrite Whether or not the command indicated that data
    *                          should be verified before writing
    * @param incrementAddress Whether or not the command indicated that the
    *                         target write address should be incremented when
    *                         writing.
    * @param status The status of the write operation
    * @param transactionIdentifier An identifier for the transaction.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * \throws RMAPPacketException if the RMAP Packet fails to create.
    *
    */
    RMAPWriteReplyPacket(U8 *pInitiatorAddress,
                        unsigned long initiatorAddressLength,
                        U8 targetAddress,
                        char verifyBeforeWrite, char incrementAddress,
                        RMAPStatus status,
                        U16 transactionIdentifier,
                        char alignment)
    {
        /* initialise object member variables to default values */
        InitialiseToDefaultState();

        /* attempt to build an RMAP write reply packet using the
         * values provided */
        void *pPacket = BuildWriteReplyPacket(pInitiatorAddress,
                                           initiatorAddressLength, targetAddress,
                                           verifyBeforeWrite, incrementAddress,
                                           status, transactionIdentifier,
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
    * Create an RMAP write reply packet using the values provided.
    *
    * \note If a packet already exists it will be disposed of and a new packet
    *       created with the values provided.
    *
    * @param pInitiatorAddress  A pointer to the SpaceWire path or logical address
    *                           of the device to which the reply should be sent.
    * @param initiatorAddressLength The length of the initiator address.
    * @param targetAddress The SpaceWire logical address of the device that
    *                      sent the command being responded to.
    * @param verifyBeforeWrite Whether or not the command indicated that data
    *                          should be verified before writing
    * @param incrementAddress Whether or not the command indicated that the
    *                         target write address should be incremented when
    *                         writing.
    * @param status The status of the write operation
    * @param transactionIdentifier An identifier for the transaction.
    * @param alignment The word size used by the device sending the packet,
    *                   normally 1. See Byte Alignment for more information.
    *
    * @return 1 if write register packet created successfully, otherwise 0.
    *
    */
    int CreateWriteReplyPacket(U8 *pInitiatorAddress,
                        unsigned long initiatorAddressLength,
                        U8 targetAddress,
                        char verifyBeforeWrite, char incrementAddress,
                        RMAPStatus status,
                        U16 transactionIdentifier,
                        char alignment)
    {
        /* free any existing packet held by this object */
        CleanUp();

        /* attempt to build an RMAP write reply packet using the
         * values provided */
        void *pPacket = BuildWriteReplyPacket(pInitiatorAddress,
                                           initiatorAddressLength, targetAddress,
                                           verifyBeforeWrite, incrementAddress,
                                           status, transactionIdentifier,
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
    virtual ~RMAPWriteReplyPacket()
    {
        /* Allow base class destructor (which will be called after this)
         * to clean up as necessary */
    }
};

/* end namespace tags */
} /* rmap */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

