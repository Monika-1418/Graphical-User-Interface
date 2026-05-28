/**
 * \file rmap_packet_library.hpp
 *
 * \brief Provides a general interface to the RMAP packet library.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides a general interface to the RMAP packet library. The RMAP packet
 * library provides functions which can be called to create RMAP packets, to
 * check the format of RMAP packets and to determine the values of fields in
 * RMAP packets. It does not send or receive packets using a SpaceWire device;
 * this work must be done by the programmer as this is application and device
 * specific.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_RMAP
 */

#ifndef __RMAP_PACKET_LIBRARY_CPP__
#define __RMAP_PACKET_LIBRARY_CPP__

#include "rmap_version.hpp"
#include "rmap_read_command_packet.hpp"
#include "rmap_read_modify_write_command_packet.hpp"
#include "rmap_read_modify_write_reply_packet.hpp"
#include "rmap_read_reply_packet.hpp"
#include "rmap_write_command_packet.hpp"
#include "rmap_write_reply_packet.hpp"
#include "cpp_api_macros.hpp"
#include <stdlib.h>
#include "packet.hpp"

namespace stardundee
{
namespace com
{
namespace starsystem
{
/**
 * Classes for building RMAP packets.
 **/
namespace rmap {

/**
* Provides a general interface to the RMAP packet library. The RMAP packet
* library provides functions which can be called to create RMAP packets, to
* check the format of RMAP packets and to determine the values of fields in
* RMAP packets. It does not send or receive packets using a SpaceWire device;
* this work must be done by the programmer as this is application and device
* specific.
*/
class RMAPPacketLibrary
{
private:

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPPacketLibrary()
    {
    }

    /**
    * Copy constructor - to allow copies of RMAPPacketLibrary objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    RMAPPacketLibrary(const RMAPPacketLibrary& libObjToCopyFrom)
    {
        /* copy contents of object being copied into this object */

        /* nothing to copy */
        /* suppress compiler warning for unreferenced input parameter */
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(libObjToCopyFrom);
    }

    /**
    * Overloaded assignment operator - to allow instances of RMAPPacketLibrary
    * objects to have their contents assigned to each other.
    *
    */
    RMAPPacketLibrary& operator=(const RMAPPacketLibrary& libObjBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &libObjBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~RMAPPacketLibrary()
    {
        /* no resources to free at present */
    }

    /**
    * Get the current version information for the RMAP packet library.
    *
    * @return An RMAPVersion object which contains the version information and
    *         methods to determine the major, minor, edit and patch for the
    *         version.
    *
    */
    static RMAPVersion GetVersion()
    {
        /* call 'c' API to get the version */
        U32 version;
        version = RMAP_GetVersion();

        /* return the version information as an RMAPVersion object */
        return RMAPVersion(version);
    }

    /**
    * Calculate an 8-bit CRC for the given buffer.
    *
    * @param pBuffer The buffer to calculate the CRC for.
    * @param len The length of the buffer.
    *
    * @return An 8-bit CRC for the specified buffer.
    *
    */
    static U8 CalculateCRC(void *pBuffer, unsigned long len)
    {
        /* call 'c' API */
        return RMAP_CalculateCRC(pBuffer, len);
    }

    /**
    * Calculate an 8-bit CRC for the given buffer, starting with a seed CRC.
    *
    * @param pBuffer The buffer to calculate the CRC for.
    * @param len The length of the buffer.
    * @param crc The seed CRC to be used when calculating the CRC.
    *
    * @return An 8-bit CRC for the specified buffer.
    *
    */
    static U8 CalculateCRCWithSeed(void *pBuffer, unsigned long len, U8 crc)
    {
        /* call 'c' API */
        return RMAP_CalculateCRCWithSeed(pBuffer, len, crc);
    }

    /**
    * Determines if the specified 8-bit CRC is valid for the given buffer.
    *
    * @param pBuffer The buffer to check the CRC for.
    * @param len The length of the buffer.
    * @param crc The CRC to check against.
    *
    * @return 1 if the CRC is correct for the buffer, else 0.
    *
    */
    static char IsCRCValid(void *pBuffer, unsigned long len, U8 crc)
    {
        /* call 'c' API */
        return RMAP_IsCRCValid(pBuffer, len, crc);
    }

    /**
    * Extract an RMAP packet from the data of a received Packet stream item,
    * and return it.
    *
    * @param pPacket A \link stardundee::com::starsystem::streamitems::Packet Packet \endlink
    *                stream item which has been received as part of a
    *                \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
    *                and which is expected to contain a valid RMAP packet
    *                within its data.
    *
    * \note The packet provided can be freed when no longer required after
    *       calling this function.
    *
    * @param checkPacketTooLong Specifies whether to check if the RMAP packet
    *                           which is being extracted from the given stream
    *                           item is longer than it should be when being
    *                           validated. Usually this will be set to
    *                           1 and corresponds to an alignment value of 1
    *                           being specified when the received packet was
    *                           being constructed in the first instance (the
    *                           alignment specifies the word size used by the
    *                           device sending the packet). This should be
    *                           set to 0 if the packet was received from a
    *                           device where a different alignment has been
    *                           specified, otherwise the packet will fail
    *                           validation due to being longer than expected.
    * @param statusInfo A pointer to a previously allocated variable whose value
    *                   will be updated to indicate the status of the received
    *                   RMAP packet extracted from the stream item provided.
    *                   This value can be used to determine any errors which
    *                   may have occurred when extracting the packet data.
    *
    * @return An instance of a new RMAP packet object, or NULL if failed
    *         to create the packet (e.g. due to a memory allocation failure), or
    *         if a valid RMAP packet could not be extracted from the data of the
    *         given stream item, or if there was an error in the status of the
    *         RMAP packet, e.g. the packet's fields contained invalid values or
    *         the packet was too short or too long. In the latter case, the
    *         value of the statusInfo parameter provided to this function can
    *         be used to determine the the reason for failure.
    *
    * \note The RMAPPacket object returned must be freed when no longer required
    *       by using the C++ delete operator.
    */
    static RMAPPacket *GetRMAPPacketFromStreamItem(
                    stardundee::com::starsystem::streamitems::Packet *pPacket,
                    char checkPacketTooLong, RMAPPacket::RMAPStatus* statusInfo)
    {
        unsigned char *pPacketData;
        unsigned int packetDataLen;
        RMAPPacket *pRMAPPacket;

        /* set the status of the RMAP packet to be unknown / invalid */
        *statusInfo = (RMAPPacket::RMAPStatus)RMAP_INVALID_STATUS;

        /* if a valid packet stream item hasn't been provided */
        if (!pPacket)
        {
            /* return NULL - no packet to get */
            return NULL;
        }

        /* get the data from the stream item provided */
        pPacketData = pPacket->GetPacketData(&packetDataLen);

        /* if no data to work with */
        if (!pPacketData)
        {
            /* return NULL */
            return NULL;
        }

        /* attempt to extract an RMAP packet from the received stream item's
         * data */
        pRMAPPacket = RMAPPacketLibrary::GetRMAPPacketFromBuffer(pPacketData,
                                         packetDataLen, checkPacketTooLong,
                                         statusInfo);

        /* destroy the data obtained from the stream item provided */
        pPacket->DestroyPacketData(pPacketData);

        /* return the RMAP packet extracted */
        /* \note This may be NULL if failed to extract a valid RMAP packet
         *       from the data in the received stream item
         */
        return pRMAPPacket;
    }

    /**
    * Extract an RMAP packet from a given data buffer, and return it.
    *
    * @param pBuffer A buffer which contains data that has been received as part
    *                of a \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
    *                and which is expected to contain a valid RMAP packet.
    *
    * \note The data buffer provided can be freed when no longer required after
    *       calling this function.
    *
    * @param bufferLen The length of the data buffer provided.
    * @param checkPacketTooLong Specifies whether to check if the RMAP packet
    *                           which is being extracted from the given data
    *                           buffer is longer than it should be when being
    *                           validated. Usually this will be set to
    *                           1 and corresponds to an alignment value of 1
    *                           being specified when the received packet was
    *                           being constructed in the first instance (the
    *                           alignment specifies the word size used by the
    *                           device sending the packet). This should be
    *                           set to 0 if the packet was received from a
    *                           device where a different alignment has been
    *                           specified, otherwise the packet will fail
    *                           validation due to being longer than expected.
    * @param statusInfo A pointer to a previously allocated variable whose value
    *                   will be updated to indicate the status of the received
    *                   RMAP packet extracted from the data buffer provided.
    *                   This value can be used to determine any errors which
    *                   may have occurred when extracting the packet data.
    *
    * @return An instance of a new RMAP packet object, or NULL if failed
    *         to create packet object (e.g. due to a memory allocation failure)
    *         or if there was an error in the status of the RMAP packet, e.g.
    *         the packet's fields contained invalid values or the packet was too
    *         short or too long. In the latter case, the value of the statusInfo
    *         parameter provided to this function can be used to determine the
    *         the reason for failure.
    *
    * \note The RMAPPacket object returned must be freed when no longer required
    *       by using the C++ delete operator.
    */
    static RMAPPacket *GetRMAPPacketFromBuffer(void *pBuffer,
                                         unsigned long bufferLen,
                                         char checkPacketTooLong,
                                         RMAPPacket::RMAPStatus* statusInfo)
    {
        /* set the status of the RMAP packet to be unknown / invalid */
        *statusInfo = (RMAPPacket::RMAPStatus)RMAP_INVALID_STATUS;

        /* if a valid data buffer hasn't been provided */
        if ((!pBuffer) || (!bufferLen))
        {
            /* return NULL */
            return NULL;
        }

        /* allocate memory to hold a copy of the data buffer provided */
        /* a copy of the data is required so it can be provided to and held
         * within the RMAP packet object which this function will eventually
         * return */
        void *copyOfRawPacketData = malloc(bufferLen);

        /* if failed to allocate memory */
        if (copyOfRawPacketData == NULL)
        {
            /* end processing, return error / NULL */
            return NULL;
        }

        /* create an RMAP_PACKET struct which will be populated with the
         * contents of the raw packet data */
        RMAP_PACKET *pRmapPacketStruct;
        try
        {
            pRmapPacketStruct = new RMAP_PACKET;
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* memory allocation error */

            /* free resources created up to this point */
            free(copyOfRawPacketData);

            /* end processing, return error / NULL */
            return NULL;
        }

        /* copy the contents of data buffer into the copy buffer */
        memcpy(copyOfRawPacketData, pBuffer, bufferLen);

        /* call Star System API function to check that the packet data is
         * valid and to populate the RMAP_PACKET struct with the packet data.
         */
        RMAP_STATUS status = RMAP_CheckPacketValid(copyOfRawPacketData,
                                                   bufferLen,
                                                   pRmapPacketStruct,
                                                   checkPacketTooLong);

        /* store the packet status, which may include the reason for error
         * if applicable */
        *statusInfo = (RMAPPacket::RMAPStatus)status;

        /* if a valid RMAP packet has been identified */
        if (status == RMAP_SUCCESS || status == RMAP_INVALID_DATA_CRC ||
            status == RMAP_EARLY_EOP)
        {
            /* get the packet's type */
            RMAP_PACKET_TYPE packetType = RMAP_GetPacketType(pRmapPacketStruct);

            /* create an instance of an RMAP packet object corresponding to the
             * type of packet received */
            RMAPPacket *pNewPacketObj = NULL;
            try
            {
                /* check the packet type */
                switch (packetType)
                {
                    case RMAP_WRITE_COMMAND:
                        /* create a write command packet */
                        pNewPacketObj = new RMAPWriteCommandPacket();
                        break;

                    case RMAP_WRITE_REPLY:
                        /* create a write reply packet */
                        pNewPacketObj = new RMAPWriteReplyPacket();
                        break;

                    case RMAP_READ_COMMAND:
                        /* create a read command packet */
                        pNewPacketObj = new RMAPReadCommandPacket();
                        break;

                    case RMAP_READ_REPLY:
                        /* create a read reply packet */
                        pNewPacketObj = new RMAPReadReplyPacket();
                        break;

                    case RMAP_READ_MODIFY_WRITE_COMMAND:
                        /* create a read-modify-write command packet */
                        pNewPacketObj = new RMAPReadModifyWriteCommandPacket();
                        break;

                    case RMAP_READ_MODIFY_WRITE_REPLY:
                        /* create a read-modify-write reply packet */
                        pNewPacketObj = new RMAPReadModifyWriteReplyPacket();
                        break;

                    case RMAP_INVALID_PACKET_TYPE:
                        /* unknown packet type - end processing */

                        /* free any resources created */
                        delete pRmapPacketStruct;
                        free(copyOfRawPacketData);

                        /* return error / NULL */
                        return NULL;
                        break;

                    default:
                        /* unknown packet type - end processing */

                        /* free any resources created */
                        delete pRmapPacketStruct;
                        free(copyOfRawPacketData);

                        /* return error / NULL */
                        return NULL;
                        break;
                }
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory allocation failure when attempting to create instance
                 * of RMAP packet object */

                /* free any resources created */
                delete pRmapPacketStruct;
                free(copyOfRawPacketData);

                /* return error / NULL */
                return NULL;
            }

            /* provide the newly created packet object with references to the
             * data it requires, e.g. the raw packet data and RMAP_PACKET */
            pNewPacketObj->SetRawDataForReceivedPacket(copyOfRawPacketData,
                                                       bufferLen);
            pNewPacketObj->SetRMAP_PACKETForReceivedPacket(pRmapPacketStruct);

            /* return the new packet object */
            return pNewPacketObj;
        }
        else
        {
            /* error in processing packet */

            /* a null packet will be returned and the reason for the error
             * is stored in the 'statusInfo' variable provided to the function
             */

            /* free any resources created */
            delete pRmapPacketStruct;
            free(copyOfRawPacketData);

            /* return error / NULL */
            return NULL;
        }
    }
};

/* end namespace tags */
} /* rmap */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

