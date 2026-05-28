/**
 * \file rmap_packet.hpp
 *
 * \brief Parent class for RMAP packet objects.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Parent class for RMAP packet objects. Defines general properties and methods
 * relevant to any type of RMAP packet.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_RMAP
 */

#ifndef __RMAP_PACKET_CPP__
#define __RMAP_PACKET_CPP__

#include "star-api.h"
#include "rmap_packet_library.h"
#include "rmap_packet_exception.hpp"
#include "cpp_api_macros.hpp"
#include <stdlib.h>
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmap {

/**
* Parent class for RMAP packet objects. Defines general properties and methods
* relevant to any type of RMAP packet.
*/
class RMAPPacket
{
public:
    /** possible status values for RMAP operations */
    enum RMAPStatus
    {
        /** The status value indicating success */
        RMAP_STATUS_SUCCESS = RMAP_SUCCESS,
        /** The status value indicating that the detected error does not fit
          * into the other error cases. */
        RMAP_STATUS_GENERAL_ERROR = RMAP_GENERAL_ERROR,
        /** The status value indicating the packet type is reserved or the
          * command is not used by the RMAP protocol */
        RMAP_STATUS_UNUSED_PACKET_TYPE_OR_COMMAND_CODE =
                                        RMAP_UNUSED_PACKET_TYPE_OR_COMMAND_CODE,
        /** The status value indicating the key did not match that expected by
          * the target user application */
        RMAP_STATUS_INVALID_KEY = RMAP_INVALID_KEY,
        /** The status value indicating there was an error in the data CRC */
        RMAP_STATUS_INVALID_DATA_CRC = RMAP_INVALID_DATA_CRC,
        /** The status value indicating an EOP was detected before the end of
          * the data */
        RMAP_STATUS_EARLY_EOP = RMAP_EARLY_EOP,
        /** The status value indicating there was more data than was expected */
        RMAP_STATUS_TOO_MUCH_DATA = RMAP_TOO_MUCH_DATA,
        /** The status value indicating that an EEP was encountered in the
          * packet after the header. */
        RMAP_STATUS_EEP = RMAP_EEP,
        /** The status value indicating that verify before write was enabled in
          * the command but not enough buffer space was available to receive
          * the full command */
        RMAP_STATUS_VERIFY_BUFFER_OVERRUN = RMAP_VERIFY_BUFFER_OVERRUN,
        /** The status value indicating the target user application did not
          * authorise the requested operation */
        RMAP_STATUS_COMMAND_NOT_IMPLEMENTED_OR_AUTHORISED =
                                    RMAP_COMMAND_NOT_IMPLEMENTED_OR_AUTHORISED,
        /** The status value indicating the amount of data in a
          * read/modify/write command is invalid */
        RMAP_STATUS_RMW_DATA_LENGTH_ERROR = RMAP_RMW_DATA_LENGTH_ERROR,
        /** The status value indicating the target logical address was not the
          * value expected by the target */
        RMAP_STATUS_INVALID_TARGET_LOGICAL_ADDRESS =
                                            RMAP_INVALID_TARGET_LOGICAL_ADDRESS,
        /** The status value indicating that an invalid status value was
          * encountered, or the status could not be determined. Note that this
          * is not a standard RMAP error */
        RMAP_STATUS_INVALID = RMAP_INVALID_STATUS
    };

    /** types of RMAP packets */
    enum PacketType
    {
        /** a write command packet */
        RMAP_PACKET_TYPE_WRITE_COMMAND = RMAP_WRITE_COMMAND,
        /** a write reply packet */
        RMAP_PACKET_TYPE_WRITE_REPLY = RMAP_WRITE_REPLY,
        /** a read command packet */
        RMAP_PACKET_TYPE_READ_COMMAND = RMAP_READ_COMMAND,
        /** a read reply packet */
        RMAP_PACKET_TYPE_READ_REPLY = RMAP_READ_REPLY,
        /** a read-modify-write command packet */
        RMAP_PACKET_TYPE_READ_MODIFY_WRITE_COMMAND =
                                                RMAP_READ_MODIFY_WRITE_COMMAND,
        /** a read-modify-write reply packet */
        RMAP_PACKET_TYPE_READ_MODIFY_WRITE_REPLY =
                                                 RMAP_READ_MODIFY_WRITE_REPLY,
        /** packet type could not be determined */
        RMAP_PACKET_TYPE_INVALID = RMAP_INVALID_PACKET_TYPE
    };

protected:
    /** the raw data for an RMAP packet created using a Build command */
    void *pRMAPPacket;

    /** the length of the raw packet data created using a Build command  */
    unsigned long packetLen;

    /** record whether the memory allocated to hold the raw packet data was
     * performed internally by this object, rather than using functions on the
     * STAR API */
    bool rawPacketDataAllocatedInternally;

    /** RMAP packet struct associated with the raw packet created using a
      * Build command */
    RMAP_PACKET rmapPacketStruct;

    /** RMAP packet struct associated with the raw packet created using a
      * Build command */
    RMAP_PACKET *pRmapPacketStruct;

    /** record whether the data for a received packet is being held, rather than
     * the data for a packet which was created using a Build command */
    bool holdsReceivedPacketData;

    /** a pointer to the raw data of an existing / received created packet */
    void *pReceivedRMAPPacket;

    /** the length of the raw packet data for a received packet  */
    unsigned long receivedPacketLen;

    /** a pointer to an RMAP packet struct which corresponds to the raw data for
     * a received RMAP packet */
    RMAP_PACKET *pReceivedRmapPacketStruct;

    /**
    * Initialise this object to it's default state.
    */
    void InitialiseToDefaultState()
    {
        /* fields related to a packet created using a Build command */
        pRMAPPacket = NULL;
        packetLen = 0;
        rawPacketDataAllocatedInternally = false;
        pRmapPacketStruct = NULL;

        /* fields related to a received packet */
        holdsReceivedPacketData = false;
        pReceivedRMAPPacket = NULL;
        receivedPacketLen = 0;
        pReceivedRmapPacketStruct = NULL;
    }

    /**
    * Perform any clean-up necessary on this object, e.g. free memory allocated
    * etc.
    */
    void CleanUp()
    {
        /* if an RMAP packet exists which was created using a Build command */
        if (pRMAPPacket != NULL)
        {
            /* if the memory for the raw packet data was allocated internally,
             * rather than using the STAR API */
            if (rawPacketDataAllocatedInternally)
            {
                /* free it 'manually' */
                free(pRMAPPacket);
            }
            else
            {
                /* free it using the STAR API*/
                RMAP_FreeBuffer(pRMAPPacket);
            }

            /* set it to NULL */
            pRMAPPacket = NULL;
        }

        /* if the data for a received RMAP packet is being held */
        if (pReceivedRMAPPacket != NULL)
        {
            /* free it */
            free(pReceivedRMAPPacket);

            /* set it to NULL */
            pReceivedRMAPPacket = NULL;
        }

        /* if a pointer to a received RMAP packet struct is being held */
        if (pReceivedRmapPacketStruct != NULL)
        {
            /* free it */
            delete pReceivedRmapPacketStruct;

            /* set it to NULL */
            pReceivedRmapPacketStruct = NULL;
        }

        /* reset other members */
        packetLen = 0;
        receivedPacketLen = 0;
        holdsReceivedPacketData = false;
        rawPacketDataAllocatedInternally = false;
        pRmapPacketStruct = NULL;
    }

    /**
    * Determine whether the given packet type corresponds to a 'reply' packet.
    *
    * @param type The packet type to check
    *
    * @returns 1 if it is a reply packet type, otherwise 0
    */
    char IsReplyPacket(PacketType type)
    {
        /* assume packet isn't a 'reply' type, until proved otherwise */
        char isReplyType = 0;

        switch (type)
        {
            case RMAP_WRITE_REPLY:
            case RMAP_READ_REPLY:
            case RMAP_READ_MODIFY_WRITE_REPLY:
                /* a reply packet type */
                isReplyType = 1;
                break;

            default:
                /* packet is not a reply type or is invalid type */
                break;
        }

        /* return whether it is a reply packet or not */
        return isReplyType;
    }


public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPPacket()
    {
        InitialiseToDefaultState();
    }

    /**
    * Copy constructor - to allow copies of RMAPPacket objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    * \note During the copy process a new RMAP packet will be created which
    *       contains a replica of the data of the RMAP packet being copied
    *       resulting in two separate but identical packet structures.
    *
    * \throws RMAPPacketException Occurs if there is a failure to allocate
    *                             memory required when making a copy of an
    *                             existing RMAP packet object.
    *
    */
    RMAPPacket(const RMAPPacket& packetObjToCopyFrom)
    {
        /* initialise object, prior to copying values to it */
        InitialiseToDefaultState();

        /* if the packet being copied holds data for a received packet */
        if (packetObjToCopyFrom.HoldsDataForAReceivedPacket())
        {
            /* get a pointer to the received raw packet data */
            void *pReceivedRawData =
                            packetObjToCopyFrom.GetReceivedPacketRawDataPtr();

            /* get the length of the raw data */
            unsigned long receivedRawDataLen =
                            packetObjToCopyFrom.GetReceivedPacketRawDataLen();

            /* if valid packet data exists */
            if (pReceivedRawData != NULL)
            {
                /* allocate memory to hold packet data being copied */
                pReceivedRMAPPacket = malloc(receivedRawDataLen);

                /* if failed to allocate memory */
                if (pReceivedRMAPPacket == NULL)
                {
                    /* throw exception, can't proceed to construct object */
                    throw RMAPPacketException();
                }

                /* create an RMAP_PACKET to associate with the received
                 * packet data */
                try
                {
                    pReceivedRmapPacketStruct = new RMAP_PACKET;
                }
                catch(std::bad_alloc& ba)
                {
                    _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                    /* memory allocation error */

                    /* free memory created for raw packet data */
                    free(pReceivedRMAPPacket);
                    pReceivedRMAPPacket = NULL;

                    /* ensure unallocated RMAP_PACKET is null */
                    pReceivedRmapPacketStruct = NULL;


                    /* throw exception, can't proceed to construct object */
                    throw RMAPPacketException();
                }

                /* copy raw packet data */
                memcpy(pReceivedRMAPPacket, pReceivedRawData,
                                            receivedRawDataLen);

                /* store the length of the data */
                receivedPacketLen = receivedRawDataLen;

                /* check that the contents of the packet are valid and, in doing
                 * so, populate the contents of the RMAP_PACKET struct held by
                 * this object to correspond to the raw packet data copied */
                RMAP_STATUS status = RMAP_CheckPacketValid(pReceivedRMAPPacket,
                                                    receivedRawDataLen,
                                                    pReceivedRmapPacketStruct,
                                                    0);

                /* if packet validation failed */
                if (status != RMAP_SUCCESS)
                {
                    /* the final packet creation step has failed */

                    /* free raw packet data allocated */
                    free(pReceivedRMAPPacket);
                    pReceivedRMAPPacket = NULL;
                    receivedRawDataLen = 0;
                    delete pReceivedRmapPacketStruct;
                    pReceivedRmapPacketStruct = NULL;

                    /* throw exception, can't proceed to construct object */
                    throw RMAPPacketException();
                }

                /* otherwise, packet creation steps completed */

                /* indicate that this object holds the data for a received
                 * packet */
                holdsReceivedPacketData = true;
            }
            else
            {
                /* no valid packet to copy from,
                 * leave this object uninitialised */
            }
        }
        else
        {
            /* packet being copied contains data for a packet created using
             * a Build command */

            /* get a pointer to the raw packet data */
            void *pRawData = packetObjToCopyFrom.GetRawDataPtr();

            /* get the length of the raw data */
            unsigned long rawDataLen = packetObjToCopyFrom.GetRawDataLen();

            /* if valid packet data exists */
            if (pRawData != NULL)
            {
                /* allocate memory to hold packet data being copied */
                pRMAPPacket = malloc(rawDataLen);

                /* if failed to allocate memory */
                if (pRMAPPacket == NULL)
                {
                    /* throw exception, can't proceed to construct object */
                    throw RMAPPacketException();
                }

                /* copy raw packet data */
                memcpy(pRMAPPacket, pRawData, rawDataLen);

                /* store the length of the data */
                packetLen = rawDataLen;

                /* get a pointer to the RMAP_PACKET struct held in the object
                 * being copied from */
                RMAP_PACKET *pRMAPPacketStructInObjectBeingCopied;
                pRMAPPacketStructInObjectBeingCopied =
                                    packetObjToCopyFrom.GetRMAP_PACKETStruct();

                /* get the type of packet being copied */
                RMAP_PACKET_TYPE packetType =
                    RMAP_GetPacketType(pRMAPPacketStructInObjectBeingCopied);

                /* get the length of the destination address held in the
                 * packet.
                 *
                 * NOTE: for 'reply' packets, this will be the reply address.
                 * For other packets it will be the target address.
                 */
                unsigned long addressLength;

                /* if packet has a 'reply' packet type */
                if (IsReplyPacket((PacketType)packetType))
                {
                    /* get length of reply address */
                    RMAP_GetReplyAddress(pRMAPPacketStructInObjectBeingCopied,
                                         &addressLength);
                }
                else
                {
                    /* get length of target address */
                    RMAP_GetTargetAddress(pRMAPPacketStructInObjectBeingCopied,
                                          &addressLength);
                }

                /* calculate an address offset which is 1 less than the length
                 * of the destination address.
                 *
                 * Rationale: a call will be made to RMAP_CheckPacketValid.
                 * This method expects to be passed data for a packet which has
                 * an address of length of at most 1. This is because it is
                 * typically used to extract data for a received packet which
                 * has already reached its destination and where, therefore, the
                 * original address has now been stripped from it due to the
                 * routing process.
                 *
                 * If the RMAP_CheckPacketValid method is called on a packet
                 * before it is transmitted, the address that it contains may be
                 * longer than 1 in length. In which case, the starting point of
                 * the data provided to the RMAP_CheckPacketValid method has to
                 * be adjusted to skip past the leading address bytes.
                 *
                 * An address offset will be calculated such that it is one less
                 * that the length of the packet's address. The offset will be
                 * used to adjust the starting point in the raw packet data
                 * passed to RMAP_CheckPacketValid. If the packet's address
                 * length is 1 (because it is a received packet), the offset
                 * calculated will be zero, i.e. no offset is required.
                 */
                unsigned long addressOffset = addressLength - 1;

                /* check that the contents of the packet are valid and, in doing
                 * so, populate the contents of the RMAP_PACKET struct held by
                 * this object to correspond to the raw packet data copied.
                 *
                 * The starting point of the raw packet data is adjusted by the
                 * offset calculated previously, in cases where the packet
                 * doesn't represent a received packet (and where the address
                 * may be longer than the expected 1 byte)
                 */
                RMAP_STATUS status = RMAP_CheckPacketValid(
                                            (U8*)pRMAPPacket + addressOffset,
                                            packetLen - addressOffset,
                                            &rmapPacketStruct, 0);

                /* if packet validation failed */
                if (status != RMAP_SUCCESS)
                {
                    /* the final packet creation step has failed */

                    /* free raw packet data allocated */
                    free(pRMAPPacket);
                    pRMAPPacket = NULL;
                    packetLen = 0;

                    /* throw exception, can't proceed to construct object */
                    throw RMAPPacketException();
                }

                /* otherwise, packet creation steps completed */

                /* indicate that the raw packet data held by this object was
                 * allocated internally by this object, rather than using
                 * functions in the STAR API */
                rawPacketDataAllocatedInternally = true;

                /* store pointer reference to RMAP_PACKET struct associated with
                 * the newly copied packet */
                pRmapPacketStruct = &rmapPacketStruct;
            }
            else
            {
                /* no valid packet to copy from,
                 * leave this object uninitialised */
            }
        }
    }

    /**
    * Overloaded assignment operator - to allow instances of RMAPPacket
    * objects to have their contents assigned to each other.
    *
    * \note During the copy process a new RMAP packet will be created which
    *       contains a replica of the data of the RMAP packet being assigned
    *       resulting in two separate but identical packet structures.
    *
    * \throws RMAPPacketException Occurs if there is a failure to allocate
    *                             memory required when making a copy of an
    *                             existing RMAP packet object.
    *
    */
    RMAPPacket& operator=(const RMAPPacket& packetObjBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &packetObjBeingAssigned)
            return *this;

        /* clear/free existing contents of this packet, it is being assigned
         * new values */
        CleanUp();

        /* copy values from the packet object being assigned to this */

        /* if the packet being copied holds data for a received packet */
        if (packetObjBeingAssigned.HoldsDataForAReceivedPacket())
        {
            /* get a pointer to the received raw packet data */
            void *pReceivedRawData =
                        packetObjBeingAssigned.GetReceivedPacketRawDataPtr();

            /* get the length of the raw data */
            unsigned long receivedRawDataLen =
                        packetObjBeingAssigned.GetReceivedPacketRawDataLen();

            /* if valid packet data exists */
            if (pReceivedRawData != NULL)
            {
                /* allocate memory to hold packet data being copied */
                pReceivedRMAPPacket = malloc(receivedRawDataLen);

                /* if failed to allocate memory */
                if (pReceivedRMAPPacket == NULL)
                {
                    /* throw exception, can't proceed to construct object */
                    throw RMAPPacketException();
                }

                /* create an RMAP_PACKET to associate with the received packet data */
                try
                {
                    pReceivedRmapPacketStruct = new RMAP_PACKET;
                }
                catch(std::bad_alloc& ba)
                {
                    _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                    /* memory allocation error */

                    /* free memory created for raw packet data */
                    free(pReceivedRMAPPacket);
                    pReceivedRMAPPacket = NULL;

                    /* ensure unallocated RMAP_PACKET is null */
                    pReceivedRmapPacketStruct = NULL;


                    /* throw exception, can't proceed to construct object */
                    throw RMAPPacketException();
                }

                /* copy raw packet data */
                memcpy(pReceivedRMAPPacket, pReceivedRawData,
                                            receivedRawDataLen);

                /* store the length of the data */
                receivedPacketLen = receivedRawDataLen;

                /* check that the contents of the packet are valid and, in doing
                 * so, populate the contents of the RMAP_PACKET struct held by
                 * this object to correspond to the raw packet data copied */
                RMAP_STATUS status = RMAP_CheckPacketValid(pReceivedRMAPPacket,
                                                    receivedRawDataLen,
                                                    pReceivedRmapPacketStruct,
                                                    0);

                /* if packet validation failed */
                if (status != RMAP_SUCCESS)
                {
                    /* the final packet creation step has failed */

                    /* free raw packet data allocated */
                    free(pReceivedRMAPPacket);
                    pReceivedRMAPPacket = NULL;
                    receivedRawDataLen = 0;
                    delete pReceivedRmapPacketStruct;
                    pReceivedRmapPacketStruct = NULL;

                    /* throw exception, can't proceed to construct object */
                    throw RMAPPacketException();
                }

                /* otherwise, packet creation steps completed */

                /* indicate that this object holds the data for a received
                 * packet */
                holdsReceivedPacketData = true;
            }
            else
            {
                /* no valid packet to copy from,
                 * leave this object uninitialised */
            }
        }
        else
        {
            /* packet being copied contains data for a packet created using
             * a Build command */

            /* get a pointer to the raw packet data */
            void *pRawData = packetObjBeingAssigned.GetRawDataPtr();

            /* get the length of the raw data */
            unsigned long rawDataLen = packetObjBeingAssigned.GetRawDataLen();

            /* if valid packet data exists */
            if (pRawData != NULL)
            {
                /* allocate memory to hold packet data being copied */
                pRMAPPacket = malloc(rawDataLen);

                /* if failed to allocate memory */
                if (pRMAPPacket == NULL)
                {
                    /* throw exception, can't proceed to construct object */
                    throw RMAPPacketException();
                }

                /* copy raw packet data */
                memcpy(pRMAPPacket, pRawData, rawDataLen);

                /* store the length of the data */
                packetLen = rawDataLen;

                /* get a pointer to the RMAP_PACKET struct held in the object
                 * being copied from */
                RMAP_PACKET *pRMAPPacketStructInObjectBeingAssigned;
                pRMAPPacketStructInObjectBeingAssigned =
                                packetObjBeingAssigned.GetRMAP_PACKETStruct();

                /* get the type of packet being copied */
                RMAP_PACKET_TYPE packetType =
                    RMAP_GetPacketType(pRMAPPacketStructInObjectBeingAssigned);

                /* get the length of the destination address held in the
                 * packet.
                 *
                 * NOTE: for 'reply' packets, this will be the reply address.
                 * For other packets it will be the target address.
                 */
                unsigned long addressLength;

                /* if packet has a 'reply' packet type */
                if (IsReplyPacket((PacketType)packetType))
                {
                    /* get length of reply address */
                    RMAP_GetReplyAddress(pRMAPPacketStructInObjectBeingAssigned,
                                         &addressLength);
                }
                else
                {
                    /* get length of target address */
                    RMAP_GetTargetAddress(
                                        pRMAPPacketStructInObjectBeingAssigned,
                                        &addressLength);
                }

                /* calculate an address offset which is 1 less than the length
                 * of the destination address.
                 *
                 * Rationale: a call will be made to RMAP_CheckPacketValid.
                 * This method expects to be passed data for a packet which has
                 * an address of length of at most 1. This is because it is
                 * typically used to extract data for a received packet which
                 * has already reached its destination and where, therefore, the
                 * original address has now been stripped from it due to the
                 * routing process.
                 *
                 * If the RMAP_CheckPacketValid method is called on a packet
                 * before it is transmitted, the address that it contains may be
                 * longer than 1 in length. In which case, the starting point of
                 * the data provided to the RMAP_CheckPacketValid method has to
                 * be adjusted to skip past the leading address bytes.
                 *
                 * An address offset will be calculated such that it is one less
                 * that the length of the packet's address. The offset will be
                 * used to adjust the starting point in the raw packet data
                 * passed to RMAP_CheckPacketValid. If the packet's address
                 * length is 1 (because it is a received packet), the offset
                 * calculated will be zero, i.e. no offset is required.
                 */
                unsigned long addressOffset = addressLength - 1;

                /* check that the contents of the packet are valid and, in doing
                 * so, populate the contents of the RMAP_PACKET struct held by
                 * this object to correspond to the raw packet data copied.
                 *
                 * The starting point of the raw packet data is adjusted by the
                 * offset calculated previously, in cases where the packet
                 * doesn't represent a received packet (and where the address
                 * may be longer than the expected 1 byte)
                 */
                RMAP_STATUS status = RMAP_CheckPacketValid(
                                            (U8*)pRMAPPacket + addressOffset,
                                            packetLen - addressOffset,
                                            &rmapPacketStruct, 0);

                /* if packet validation failed */
                if (status != RMAP_SUCCESS)
                {
                    /* the final packet creation step has failed */

                    /* free raw packet data allocated */
                    free(pRMAPPacket);
                    pRMAPPacket = NULL;
                    packetLen = 0;

                    /* throw exception, can't proceed to construct object */
                    throw RMAPPacketException();
                }

                /* otherwise, packet creation steps completed */

                /* indicate that the raw packet data held by this object was
                 * allocated internally by this object, rather than using
                 * functions in the STAR API */
                rawPacketDataAllocatedInternally = true;

                /* store pointer reference to RMAP_PACKET struct associated with
                 * the newly copied packet */
                pRmapPacketStruct = &rmapPacketStruct;
            }
            else
            {
                /* no valid packet to copy from,
                 * leave this object uninitialised */
            }
        }

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Automatically dispose of the packet and its data if required,
    * if it hasn't been manually disposed of by the user (by calling the
    * Dispose() function).
    */
    virtual ~RMAPPacket()
    {
        CleanUp();
    }

    /**
    * Free / dispose of the packet and its data.
    */
    void Dispose()
    {
        CleanUp();
    }


    /**
    * Check that the packet is in the correct format for an RMAP packet.
    *
    * @param checkPacketTooLong Specifies whether to check if the packet is
    *                           longer than it should be based on the values
    *                           in the RMAP fields. This should be set to 0 if
    *                           the packet was received using a device which
    *                           can only receive multiples of 4 bytes, for
    *                           example.
    *
    * @return An RMAP status code indicating if the packet contained any errors,
    *         for example if any fields have invalid values, or the packet is
    *         too short or long.
    *
    */
    RMAPStatus CheckPacketValid(char checkPacketTooLong)
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* call method on packet to obtain information */
            return (RMAPStatus)RMAP_CheckPacketValid(pReceivedRMAPPacket,
                                                     receivedPacketLen,
                                                     NULL, checkPacketTooLong);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* get the packet's type  */
            RMAP_PACKET_TYPE packetType = (RMAP_PACKET_TYPE)GetPacketType();

            /* get the length of the destination address held in the
            * packet.
            *
            * NOTE: for 'reply' packets, this will be the reply address.
            * For other packets it will be the target address.
            */
            unsigned long addressLength;

            /* if packet has a 'reply' packet type */
            if (IsReplyPacket((PacketType)packetType))
            {
                /* get length of reply address */
                GetReplyAddress(&addressLength);
            }
            else
            {
                /* get length of target address */
                GetTargetAddress(&addressLength);
            }

            /* calculate an address offset which is 1 less than the length
            * of the destination address.
            *
            * Rationale: a call will be made to RMAP_CheckPacketValid.
            * This method expects to be passed data for a packet which has
            * an address of length of at most 1. This is because it is
            * typically used to extract data for a received packet which
            * has already reached its destination and where, therefore, the
            * original address has now been stripped from it due to the
            * routing process.
            *
            * If the RMAP_CheckPacketValid method is called on a packet
            * before it is transmitted, the address that it contains may be
            * longer than 1 in length. In which case, the starting point of
            * the data provided to the RMAP_CheckPacketValid method has to
            * be adjusted to skip past the leading address bytes.
            *
            * An address offset will be calculated such that it is one less
            * that the length of the packet's address. If the packet's
            * address length is 1 (because it is a received packet), the
            * offset calculated will be zero, i.e. no offset is required.
            */
            unsigned long addressOffset = addressLength - 1;

            /* call method on packet to obtain information, adjusting the
                * start of the packet's data with the offset */
            return (RMAPStatus)RMAP_CheckPacketValid(
                                            (U8*)pRMAPPacket + addressOffset,
                                            packetLen - addressOffset, NULL,
                                            checkPacketTooLong);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return invalid status */
        return (RMAPStatus)RMAP_INVALID_STATUS;
    }

    /**
    * Get the type of the packet (whether it is a read, write or
    * read/modify/write command or reply).
    *
    * @return The type of the packet, or RMAP_INVALID_PACKET_TYPE if the packet
    *         is invalid.
    *
    */
    PacketType GetPacketType()
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return the received packet's type */
            return (PacketType)RMAP_GetPacketType(pReceivedRmapPacketStruct);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return the pre-built packet's type */
            return (PacketType)RMAP_GetPacketType(pRmapPacketStruct);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return invalid type */
        return (PacketType)RMAP_INVALID_PACKET_TYPE;
    }

    /**
    * Get the target address of the packet.
    *
    * @param pTargetAddressLength A pointer to a user provided variable
    *        which will be updated to contain the length of the target address.
    *        This can be NULL if the length is not of interest.
    *
    * @return A pointer to the target address in the packet, or NULL if the
    *         packet is invalid.
    *
    */
    U8 *GetTargetAddress(unsigned long *pTargetAddressLength)
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return the received packet's target address */
            return RMAP_GetTargetAddress(pReceivedRmapPacketStruct,
                                         pTargetAddressLength);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return the pre-built packet's target address */
            return RMAP_GetTargetAddress(pRmapPacketStruct,
                                         pTargetAddressLength);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return NULL / no address */
        return NULL;
    }

    /**
    * Get whether the packet has verify before write enabled, to check any data
    * before writing it to memory.
    *
    * @return Whether verify before write is enabled in the packet.
    *
    */
    char GetVerifyBeforeWrite()
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetVerifyBeforeWrite(pReceivedRmapPacketStruct);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for th pre-built packet */
            return RMAP_GetVerifyBeforeWrite(pRmapPacketStruct);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return 0 */
        return 0;
    }

    /**
    * Get whether the packet has acknowledgement enabled, to acknowledge the
    * command with a reply packet.
    *
    * @return Whether acknowledgement is enabled in the packet.
    *
    */
    char GetPerformAcknowledgement()
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetPerformAcknowledgement(pReceivedRmapPacketStruct);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for th pre-built packet */
            return RMAP_GetPerformAcknowledgement(pRmapPacketStruct);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return 0 */
        return 0;
    }

    /**
    * Get whether the packet has incrementing of memory addresses enabled, to
    * read from or write to sequential memory.
    *
    * @return Whether incrementing addresses is enabled in the packet.
    */
    char GetIncrementAddress()
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetIncrementAddress(pReceivedRmapPacketStruct);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for th pre-built packet */
            return RMAP_GetIncrementAddress(pRmapPacketStruct);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return 0 */
        return 0;
    }

    /**
    * Get the key field in the packet.
    *
    * @return The value of the key field in the packet
    */
    U8 GetKey()
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetKey(pReceivedRmapPacketStruct);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for th pre-built packet */
            return RMAP_GetKey(pRmapPacketStruct);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return 0 */
        return 0;
    }

    /**
    * Get the reply address of the packet.
    *
    * \note The reply address may contain 0 byte padding if the packet is a
    *       command.
    *
    * @param pReplyAddressLength A pointer to a previously allocated variable
    *                            which will be updated to contain the length
    *                            of the reply address. This can be NULL if the
    *                            length is not of interest.
    *
    * @return A pointer to the reply address in the packet, or NULL if the
    *         packet is invalid.
    */
    U8 *GetReplyAddress(unsigned long *pReplyAddressLength)
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetReplyAddress(pReceivedRmapPacketStruct,
                                        pReplyAddressLength);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for th pre-built packet */
            return RMAP_GetReplyAddress(pRmapPacketStruct, pReplyAddressLength);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return NULL / no address */
        return NULL;
    }

    /**
    * Get the transaction identifier in the packet. The transaction identifier
    * is used to associate a reply with the command that caused the reply.
    *
    * @return The value of the transaction identifier field in the packet.
    */
    U16 GetTransactionID()
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetTransactionID(pReceivedRmapPacketStruct);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for th pre-built packet */
            return RMAP_GetTransactionID(pRmapPacketStruct);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return 0 */
        return 0;
    }

    /**
    * Get the memory address and extended memory address of the packet. The
    * memory address is the address to be read from or written to at the
    * target.
    *
    * @param pExtendedAddress A pointer to a previously allocated variable which
    *                         will be updated to contain the extended address.
    *                         This can be NULL if the extended address is not of
    *                         interest.
    *
    * @return The memory address in the packet, or 0 if the packet is invalid or
    *         is of a type which does not contain a memory address.
    */
    U32 GetAddress(U8 *pExtendedAddress)
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetAddress(pReceivedRmapPacketStruct, pExtendedAddress);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for th pre-built packet */
            return RMAP_GetAddress(pRmapPacketStruct, pExtendedAddress);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return 0 */
        return 0;
    }

    /**
    * Get the data in the packet. The data bytes are bytes to be written or the
    * bytes that have been read.
    *
    * @param pDataLength A pointer to a previously allocated variable which will
    *                    be updated to contain the length of the data field. This
    *                    can be NULL if the length is not of interest.
    *
    * @return A pointer to the data in the packet, or NULL if the packet is
    *         invalid or is of a type which does not contain data.
    */
    U8 *GetData(U32 *pDataLength)
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetData(pReceivedRmapPacketStruct, pDataLength);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for the pre-built packet */
            return RMAP_GetData(pRmapPacketStruct, pDataLength);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return NULL / no data */
        return NULL;
    }

    /**
    * Gets the data length property of a read command packet.
    *
    * @return The data length of the read command packet.
    */
    U32 GetReadLength()
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetReadLength(pReceivedRmapPacketStruct);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for the pre-built packet */
            return RMAP_GetReadLength(pRmapPacketStruct);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return 0 */
        return 0;
    }

    /**
    * Get the value of the status field in the packet.
    *
    * @return The value of the status field in the packet, if present.
    */
    RMAPStatus GetStatus()
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return (RMAPStatus)RMAP_GetStatus(pReceivedRmapPacketStruct);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for the pre-built packet */
            return (RMAPStatus)RMAP_GetStatus(pRmapPacketStruct);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return invalid status */
        return (RMAPStatus)RMAP_INVALID_STATUS;
    }

    /**
    * Get the value of the header CRC field in the packet.
    *
    * The header CRC contains an 8-bit CRC covering each byte in the header.
    *
    * @return The value of the header CRC field in the packet.
    */
    U8 GetHeaderCRC()
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetHeaderCRC(pReceivedRmapPacketStruct);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for the pre-built packet */
            return RMAP_GetHeaderCRC(pRmapPacketStruct);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return 0 */
        return 0;
    }

    /**
    * Get the value of the data CRC field in the packet.
    *
    * The data CRC field is present in packet types with data fields and
    * contains an 8-bit CRC covering each byte in the data and mask (in
    * read/write/modify commands).
    *
    * @return The value of the data CRC field in the packet, if present.
    */
    U8 GetDataCRC()
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetDataCRC(pReceivedRmapPacketStruct);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for the pre-built packet */
            return RMAP_GetDataCRC(pRmapPacketStruct);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return 0 */
        return 0;
    }

    /**
    * Get the mask in the packet.
    *
    * The mask bytes are present in read/modify/write commands and defines how
    * the data written to memory is formed in an application dependent manner.
    *
    * @param pMaskLength A pointer to a previously allocated variable which will
    *                    be updated to contain the length of the mask field. This
    *                    can be NULL if the length is not of interest.
    *
    * @return A pointer to the mask in the packet, or NULL if the packet is
    *         invalid or is not a read/modify/write command.
    */
    U8 *GetMask(U8 *pMaskLength)
    {
        /* if data for a received packet is available */
        if ((HoldsDataForAReceivedPacket()) &&
            (pReceivedRmapPacketStruct != NULL))
        {
            /* return information requested for the received packet */
            return RMAP_GetMask(pReceivedRmapPacketStruct, pMaskLength);
        }

        /* if data for a pre-built packet is available */
        if (pRmapPacketStruct != NULL)
        {
            /* return information requested for the pre-built packet */
            return RMAP_GetMask(pRmapPacketStruct, pMaskLength);
        }

        /* otherwise, attempt to access a packet which hasn't been created
         * yet, return NULL */
        return NULL;
    }

    /**
    * Set the raw data for a received RMAP packet to be held by this object.
    *
    * \note This is for internal use of the C++ API only.
    *
    * @param rawPacketData A pointer to the packet data.
    * @param packetLen The length of the data.
    *
    */
    void SetRawDataForReceivedPacket(void *rawPacketData, unsigned long packetLen)
    {
        /* if data for a received packet is already held */
        if (pReceivedRMAPPacket != NULL)
        {
            /* free existing data */
            free(pReceivedRMAPPacket);
            pReceivedRMAPPacket = NULL;
        }

        /* store pointer to data provided, and the length of the data */
        pReceivedRMAPPacket = rawPacketData;
        receivedPacketLen = packetLen;

        /* indicate that the data for a received packet is being held */
        holdsReceivedPacketData = true;
    }

    /**
    * Set the RMAP_PACKET struct corresponding to a received RMAP packet.
    *
    * \note This is for internal use of the C++ API only.
    *
    * @param pRmapPacket A pointer to an RMAP_PACKET
    */
    void SetRMAP_PACKETForReceivedPacket(RMAP_PACKET *pRmapPacket)
    {
        /* if data for a received packet is already held */
        if (pReceivedRmapPacketStruct != NULL)
        {
            /* free existing data */
            delete pReceivedRmapPacketStruct;
            pReceivedRmapPacketStruct = NULL;
        }

        /* store pointer to data provided, and the length of the data */
        pReceivedRmapPacketStruct = pRmapPacket;

        /* indicate that the data for a received packet is being held */
        holdsReceivedPacketData = true;
    }

    /**
    * Return whether this object holds the data for a received packet (rather
    * than the data for a packet which was created using a Build command).
    *
    * \note This is for internal use of the C++ API only.
    *
    * @return True if the packet holds data for a received packet, otherwise
    *         False.
    */
    bool HoldsDataForAReceivedPacket() const
    {
        return holdsReceivedPacketData;
    }

    /**
    * Obtains a pointer to the raw data for the packet.
    *
    * \note This is for internal use of the C++ API only.
    *
    * @return A pointer to the raw data for the packet, or NULL if the packet
    *         hasn't been created or initialised yet.
    */
    void* GetRawDataPtr() const
    {
        return pRMAPPacket;
    }

    /**
    * Get the length of the packet's raw data.
    *
    * \note This is for internal use of the C++ API only.
    *
    * @return The length of the packet's raw data, or 0 if the packet
    *         hasn't been created or initialised yet.
    */
    unsigned long GetRawDataLen() const
    {
        return packetLen;
    }

    /**
    * Obtains a pointer to the raw data for a received packet.
    *
    * \note This is for internal use of the C++ API only.
    *
    * \note A received packet is one which has been received, for example,
    *       through a receive operation. This is rather than a packet
    *       which has been created using the 'Build' command, which may then be
    *       the basis of a transmit operation.
    *
    * @return A pointer to the raw data for a received packet, or NULL if the
    *         there is no received packet data.
    */
    void* GetReceivedPacketRawDataPtr() const
    {
        return pReceivedRMAPPacket;
    }

    /**
    * Get the length of a received packet's raw data.
    *
    * \note This is for internal use of the C++ API only.
    *
    * \note A received packet is one which has been received, for example,
    *       through a receive operation. This is rather than a packet
    *       which has been created using the 'Build' command, which may then be
    *       the basis of a transmit operation.
    *
    * @return The length of the received packet's raw data, or 0 if the packet
    *         hasn't been created or initialised yet.
    */
    unsigned long GetReceivedPacketRawDataLen() const
    {
        return receivedPacketLen;
    }

    /**
    * Obtains a pointer to the RMAP_PACKET struct for a held for a packet.
    *
    * \note This is for internal use of the C++ API only.
    *
    * @return A pointer to the RMAP_PACKET struct.
    */
    RMAP_PACKET* GetRMAP_PACKETStruct() const
    {
        return pRmapPacketStruct;
    }

    /**
    * Get a pointer to the RMAP_PACKET struct held for a received packet.
    *
    * \note This is for internal use of the C++ API only.
    *
    * \note A received packet is one which has been received, for example,
    *       through a receive operation. This is rather than a packet
    *       which has been created using the 'Build' command, which may then be
    *       the basis of a transmit operation.
    *
    * @return A pointer to the RMAP_PACKET struct for a received packet, or NULL
    *         if the packet hasn't been initialised yet.
    */
    RMAP_PACKET* GetRMAP_PACKETStructForReceivedPacket() const
    {
        return pReceivedRmapPacketStruct;
    }

    /**
    * Get the length of the packet held by this object.
    *
    * @returns The packet length, or zero if no packet exists yet.
    */
    unsigned long GetPacketLength()
    {
        /* if the data for a received packet is being held */
        if (holdsReceivedPacketData)
        {
            /* return the length of the received packet */
            return receivedPacketLen;
        }
        else
        {
            /* return the length of a non-received packet, i.e. a packet which
             * was constructed using a Build*Packet command */
            return packetLen;
        }
    }

    /**
    * Get a pointer to the packet's data.
    *
    * @returns A pointer to a buffer containing the packet's data, or NULL if
    * no packet exists yet.
    */
    void* GetPacketData()
    {
        /* if the data for a received packet is being held */
        if (holdsReceivedPacketData)
        {
            /* return the received packet data */
            return pReceivedRMAPPacket;
        }
        else
        {
            /* return the data for a non-received packet, i.e. a packet which
             * was constructed using a Build*Packet command */
            return pRMAPPacket;
        }
    }
};

/* end namespace tags */
} /* rmap */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

