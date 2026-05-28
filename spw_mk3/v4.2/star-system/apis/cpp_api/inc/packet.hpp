/**
 * \file packet.hpp
 *
 * \brief Represents a packet which can be sent over a SpaceWire link.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a packet which can be sent over a SpaceWire link.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __STAR_SPACEWIRE_PACKET_CPP__
#define __STAR_SPACEWIRE_PACKET_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"
#include "address.hpp"
#include "stream_item.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {

/**
* A SpaceWire packet contains the data which is to be sent through a SpaceWire
* network and may include the address to which the packet should be routed, the
* data being transferred, and end of packet and/or control codes.
*
* \note If a Packet originates from a \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       (e.g.
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItem() GetTransferItem() \endlink
*       or
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItemList() GetTransferItemList()\endlink)
*       then the contained data may be disposed of by the underlying C API or
*       when the \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       is destroyed. If the received data is being queued for later processing
*       then it is recommended to queue the contained data rather than the
*       Packet.
*/
class Packet : public StreamItem
{
public:
    /** end of packet marker types */
    enum EopType
    {
        /** Error occurred determining EOP type */
        EOP_TYPE_INVALID = STAR_EOP_TYPE_INVALID,
        /** End of packet marker */
        EOP_TYPE_EOP = STAR_EOP_TYPE_EOP,
        /** Error End of packet marker */
        EOP_TYPE_EEP = STAR_EOP_TYPE_EEP,
        /** No end of packet marker present */
        EOP_TYPE_NONE = STAR_EOP_TYPE_NONE
    };

private:
    /* A STAR_STREAM_ITEM which represents the contained data for a SpaceWire
     * packet.
     *
     * The STAR-System API uses a generic data structure to represent data types
     * such as packets: STAR_STREAM_ITEM. This data structure has a field within
     * which identifies the type of item it represents (whether it is a packet,
     * a timecode, data chunk, error in data or event) and contains the data
     * associated for that item. Access to this contained data, on the basis
     * that it is a packet, is managed by this class
     */
    STAR_STREAM_ITEM *pPacketStreamItem;

    /**
    * Creates a new packet from user provided data.
    *
    * @param[in] address Optional pointer to a STAR_SPACEWIRE_ADDRESS.
    * \note It is safe to dispose of this buffer after this function completes.
    * @param[in] data Optional pointer to data buffer.
    * \note The contents of this buffer are copied into data structures managed
    *       by the API. It is safe to dispose of this buffer after this function
    *       completes.
    * @param dataLen The length of the data buffer.
    * @param eopType End of packet marker type for the packet (may be none)
    *
    * @return 1 if packet created successfully, otherwise 0
    */
    _Check_return_ int CreatePacket(_In_opt_ STAR_SPACEWIRE_ADDRESS* address,
                                    _In_opt_count_(dataLen) unsigned char* data,
                                    unsigned int dataLen,
                                    EopType eopType)
    {
        /* if there is already a previously created packet */
        if (pPacketStreamItem != NULL)
        {
            /* destroy the existing packet */
            DestroyPacket();
        }

        /* call 'c' API to create a spacewire packet / stream item */
        pPacketStreamItem = STAR_createPacket(address, data, dataLen,
                                              (STAR_EOP_TYPE)eopType);

        /* if packet stream item created successfully */
        if (pPacketStreamItem != NULL)
        {
            /* return success */
            return 1;
        }
        else
        {
            /* return error code */
            return 0;
        }

    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    Packet() : pPacketStreamItem(NULL)
    {
    }

    /**
    * Constructor - use given packet information to automatically create
    * a packet stream item.
    *
    * @param[in] address Optional pointer to a SpaceWire address.
    * @param[in] data Optional pointer to data buffer.
    * \note The contents of this buffer are copied into data structures managed
    *       by the API. It is safe to dispose of this buffer after this function
    *       completes.
    * @param dataLen The length of the data buffer.
    * @param eopType End of packet marker type for the packet (may be none)
    *
    * \throws PacketException If the Packet fails to
    *                                       create.
    */
    Packet(_In_opt_ Address* address,
                     _In_opt_count_(dataLen) unsigned char* data,
                     unsigned int dataLen,
                     EopType eopType) : pPacketStreamItem(NULL)
    {
        /* create the packet */
        int result = CreatePacket(address, data, dataLen, eopType);

        /* if error creating packet */
        if (result == 0)
        {
            /* throw exception */
            throw PacketException();
        }
    }

    /**
    * Constructor - use given packet information to automatically create
    * a packet stream item.
    *
    * @param[in] path Optional pointer to spacewire address path.
    * \note The contents of this buffer are copied into data structures managed
    *       by the API. It is safe to dispose of this buffer after this function
    *       completes.
    * @param pathLen The length of the address path.
    * @param[in] data Optional pointer to data buffer.
    * \note The contents of this buffer are copied into data structures managed
    *       by the API. It is safe to dispose of this buffer after this function
    *       completes.
    * @param dataLen The length of the data buffer.
    * @param eopType End of packet marker type for the packet (may be none)
    *
    * \throws PacketException If the Packet fails to
    *                                       create.
    */
    Packet(_In_opt_count_(pathLen) unsigned char* path,
                     U16 pathLen,
                     _In_opt_count_(dataLen) unsigned char* data,
                     unsigned int dataLen,
                     EopType eopType) : pPacketStreamItem(NULL)
    {
        /* create the packet */
        int result = CreatePacket(path, pathLen, data, dataLen, eopType);

        /* if error creating packet */
        if (result == 0)
        {
            /* throw exception */
            throw PacketException();
        }
    }

    /**
    * Overloaded constructor. Initialize object with a pointer to an already
    * created stream item.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    *
    * @param[in] streamItem The stream item to initialize the object with.
    * @param[in] shouldBeDestroyed Whether or not the underlying stream item
    *                              should be destroyed when the packet is freed.
    *
    * \note Because a pointer to an already existing stream item is provided,
    *       the memory associated with the stream item will not be automatically
    *       freed by this object when it goes out of scope or is destroyed. Only
    *       the pointer reference will be removed.
    *
    */
    Packet(_In_opt_ STAR_STREAM_ITEM* streamItem,
        _In_opt_ bool shouldBeDestroyed = false) : pPacketStreamItem(NULL)
    {
        /* if a valid stream item is provided */
        if ((streamItem != NULL) &&
               (streamItem->itemType == STAR_STREAM_ITEM_TYPE_SPACEWIRE_PACKET))
        {
            /* proceed to store reference to stream item */

            /* set packet stream item to that provided */
            pPacketStreamItem = streamItem;

            /* record that the object has been initialised to hold a reference
             * to an already existing, externally created STAR_STREAM_ITEM.
             */
            holdsReferenceToExternallyCreatedStreamItem = true;

            /* however, if it has been explicitely stated that the stream item
             * should be destroyed then we want to treat it as if it was
             * created internally and therefore will be destroyed. */
            if(shouldBeDestroyed)
            {
                holdsReferenceToExternallyCreatedStreamItem = false;
            }
        }
        else
        {
            /* throw exception (can't return error code from constructor) */
            throw PacketException();
        }
    }

    /**
    * Destructor. Automatically destroys the packet if required, if it
    * hasn't been manually destroyed by the user (by calling DestroyPacket()).
    */
    virtual ~Packet()
    {
        /* if spacewire packet / stream item not destroyed */
        if (pPacketStreamItem != NULL)
        {
            /* automatically destroy the packet */
            DestroyPacket();
        }
    }

    /**
    * Copy constructor - invoked when a copy of a Packet object is
    * made, e.g. such as through being a return value from a function or when
    * being passed into a function as a 'value' parameter.
    *
    * \note This will result in a duplicate packet being made from that which
    *       is to be copied from, including the packet data.
    *
    * @param packet The Packet object from which a copy is being made
    *
    * \throws PacketException If fail to create a new Packet which is a
    *         duplicate of that being copied.
    */
    Packet(const Packet& packet) : StreamItem()
    {
        /* if the object being copied contains an initialised packet */
        if (packet.pPacketStreamItem != NULL)
        {
            /* Perform deep copy of the packet. The default strategy for
             * achieving this is to initiate another call to the STAR-System
             * API, to create a new packet using the same data as the object
             * being copied.
             */

            /* record result of creating a new packet */
            int result = 0;

            /* collate various data items from existing packet */

            /* get copy of data from existing packet, and its length */
            unsigned int dataLen = 0;
            unsigned char* data = packet.GetPacketData(&dataLen);

            /* get the packet's EOP type */
            EopType eopType = packet.GetPacketEOP();

            /* get the address of the packet, if any */
            STAR_SPACEWIRE_ADDRESS *pAddress =
              ((STAR_SPACEWIRE_PACKET*)packet.pPacketStreamItem->item)->address;

            /* attempt to create a new packet using the data gathered */
            result = CreatePacket(pAddress, data, dataLen, eopType);

            /* Free the copy of the data which was obtained from the original
             * packet. It was only required temporarily to initialise the new
             * packet being created.
             */
            DestroyPacketData(data);

            /* if error creating packet */
            if (result == 0)
            {
                /* throw exception (can't return error code from constructor) */
                throw PacketException();
            }
        }
        else
        {
            /* perform shallow copy of object data */
            this->pPacketStreamItem = NULL;
        }
    }

    /**
    * Overloaded assignment operator - to allow instances of Packets
    * to have their contents assigned to each other.
    *
    * \note This will result in a duplicate packet being made from that which
    *       is assigned, including the packet data. Any existing packet held
    *       will be destroyed if required and a new packet created with the
    *       values assigned.
    *
    * \throws PacketException If fail to create a new Packet which is a
    *         duplicate of that being assigned.
    */
    Packet& operator=(const Packet& packet)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &packet)
            return *this;

        /* delete contents of current packet if any, they are about to
         * replaced */
        DestroyPacket();

        /* if the object being assigned contains an initialised packet */
        if (packet.pPacketStreamItem != NULL)
        {
            /* Perform deep copy of the packet. The default strategy for
             * achieving this is to initiate another call to the STAR-System
             * API, to create a new packet using the same data as the object
             * being copied.
             */

            /* record result of creating a new packet */
            int result = 0;

            /* collate various data items from packet whose values are being
             * assigned to 'this'*/

            /* get copy of data from existing packet, and its length */
            unsigned int dataLen = 0;
            unsigned char* data = packet.GetPacketData(&dataLen);

            /* get the packet's EOP type */
            EopType eopType = packet.GetPacketEOP();

            /* get the address of the packet, if any */
            STAR_SPACEWIRE_ADDRESS *pAddress =
              ((STAR_SPACEWIRE_PACKET*)packet.pPacketStreamItem->item)->address;

            /* attempt to create a new packet using the data gathered */
            result = CreatePacket(pAddress, data, dataLen, eopType);

            /* Free the copy of the data which was obtained from the original
             * packet. It was only required temporarily to initialise the new
             * packet being created.
             */
            DestroyPacketData(data);

            /* if error creating packet */
            if (result == 0)
            {
                /* throw exception (can't return error code from constructor) */
                throw PacketException();
            }
        }
        else
        {
            /* perform shallow copy of object data */
            this->pPacketStreamItem = NULL;
        }

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Creates a new packet from user provided data.
    *
    * \note Any existing packet held will be destroyed if required and a new
    *       packet created with the values specified.
    *
    * @param[in] address Optional pointer to a SpaceWire address.
    * @param[in] data Optional pointer to data buffer.
    * \note The contents of this buffer are copied into data structures managed
    *       by the API. It is safe to dispose of this buffer after this function
    *       completes.
    * @param dataLen The length of the data buffer.
    * @param eopType End of packet marker type for the packet (may be none)
    *
    * @return 1 if packet created successfully, otherwise 0
    */
    _Check_return_ int CreatePacket(_In_opt_ Address* address,
                     _In_opt_count_(dataLen) unsigned char* data,
                     unsigned int dataLen,
                     EopType eopType)
    {
        /* if there is already a previously created packet */
        if (pPacketStreamItem != NULL)
        {
            /* destroy the existing packet */
            DestroyPacket();
        }

        /* call 'c' API to create a spacewire packet / stream item */
        STAR_SPACEWIRE_ADDRESS* pAddress = NULL;
        if (address != NULL)
            pAddress = address->GetAs_STAR_SPACEWIRE_ADDRESS();
        pPacketStreamItem = STAR_createPacket(pAddress, data, dataLen,
                                              (STAR_EOP_TYPE)eopType);

        /* if packet stream item created successfully */
        if (pPacketStreamItem != NULL)
        {
            /* return success */
            return 1;
        }
        else
        {
            /* return error code */
            return 0;
        }
    }

    /**
    * Creates a new packet from user provided data.
    *
    * \note Any existing packet held will be destroyed if required and a new
    *       packet created with the values specified.
    *
    * @param[in] path Optional pointer to spacewire address path.
    * \note The contents of this buffer are copied into data structures managed
    *       by the API. It is safe to dispose of this buffer after this function
    *       completes.
    * @param pathLen The length of the address path.
    * @param[in] data Optional pointer to data buffer.
    * \note The contents of this buffer are copied into data structures managed
    *       by the API. It is safe to dispose of this buffer after this function
    *       completes.
    * @param dataLen The length of the data buffer.
    * @param eopType End of packet marker type for the packet (may be none)
    *
    * @return 1 if packet created successfully, otherwise 0
    */
    _Check_return_ int CreatePacket(_In_opt_count_(pathLen) unsigned char* path,
                                    U16 pathLen,
                                    _In_opt_count_(dataLen) unsigned char* data,
                                    unsigned int dataLen,
                                    EopType eopType)
    {
        /* if there is already a previously created packet */
        if (pPacketStreamItem != NULL)
        {
            /* destroy the existing packet */
            DestroyPacket();
        }

        /* define a STAR_SPACEWIRE_ADDRESS pointer, required later */
        STAR_SPACEWIRE_ADDRESS *pAddress = NULL;

        /* if a spacewire address path has been provided */
        if (path != NULL)
        {
            /* create a new STAR_SPACEWIRE_ADDRESS */
            pAddress = STAR_createAddress(path, pathLen);

            /* if address failed to create */
            if (pAddress == NULL)
            {
                /* return error */
                return 0;
            }
        }

        /* call 'c' API to create a spacewire packet / stream item */
        pPacketStreamItem = STAR_createPacket(pAddress, data, dataLen,
                                              (STAR_EOP_TYPE)eopType);

        /* if a spacewire address was created */
        if (pAddress != NULL)
        {
            /* destroy the address, it is no longer required */
            STAR_destroyAddress(pAddress);
        }

        /* if packet stream item created successfully */
        if (pPacketStreamItem != NULL)
        {
            /* return success */
            return 1;
        }
        else
        {
            /* return error code */
            return 0;
        }

    }

    /**
    * Free any resources held by the Packet object, e.g. pointers
    * to memory resources, etc.
    */
    void DestroyPacket()
    {
        /* if valid packet stream item to destroy */
        if (pPacketStreamItem != NULL)
        {
            /* if the packet which is held by this object was created during
             * an earlier create operation (i.e. it isn't just a reference to an
             * externally created and provided stream item).
             */
            if (holdsReferenceToExternallyCreatedStreamItem == false)
            {
                /* call 'c' API to destroy the packet stream item */
                STAR_destroyStreamItem(pPacketStreamItem);
            }
            else
            {
                /* Memory pointed to for the packet won't be freed since it
                 * was created externally to this object. Instead, the pointer
                 * will be set to NULL, to detach it from the packet pointed
                 * to. This occurs later below.
                 */

                /* The flag which indicates that the memory pointed to was
                 * created externally now has to be reset */
                holdsReferenceToExternallyCreatedStreamItem = false;
            }

            /* ensure packet stream item is set as empty */
            pPacketStreamItem = NULL;
        }
    }

    /**
    * Gets the end of packet marker type for the packet.
    *
    * @return The EOP type of the packet or EOP_TYPE_INVALID if the
    *         packet has not been created yet, therefore its end of packet
    *         marker type cannot be determined.
    */
    EopType GetPacketEOP() const
    {
        /* if packet available */
        if (pPacketStreamItem != NULL)
        {
            /* call c API to obtain packet's EOP type */
            return (EopType)STAR_getPacketEOP(
                   (STAR_SPACEWIRE_PACKET*)pPacketStreamItem->item);
        }
        else
        {
            /* return invalid EOP type */
            return EOP_TYPE_INVALID;
        }
    }


    /**
    * Sets the end of packet marker type for the packet.
    *
    * @param eop End of packer marker type to set.
    *
    * @return 1 if EOP was successfully set, otherwise 0.
    *
    * \note 0 may also be returned if the packet has not been created yet,
    *         therefore its end of packet marker type cannot be set.
    */
    int SetPacketEOP(EopType eop)
    {
        /* if packet available */
        if (pPacketStreamItem != NULL)
        {
            /* call c API to set packet's EOP type */
            return STAR_setPacketEOP((STAR_SPACEWIRE_PACKET*)pPacketStreamItem->item,
                                     (STAR_EOP_TYPE)eop);
        }
        else
        {
            /* return 0 to indicate eop type set unsuccessful */
            return 0;
        }
    }

    /**
    * Gets the length of the packet.
    *
    * @return The length in bytes of the packet (inclusive of both data and
    *         address components) or 0 to indicate that no data is present OR
    *         to indicate that the packet has not been created yet, therefore
    *         its length cannot be determined.
    */
    unsigned int GetPacketLength()
    {
        /* if packet available */
        if (pPacketStreamItem != NULL)
        {
            /* call c API to get packet's length */
            return STAR_getPacketLength(
                              (STAR_SPACEWIRE_PACKET*)pPacketStreamItem->item);
        }
        else
        {
            /* return 0 to indicate no packet present */
            return 0;
        }
    }

    /**
    * Get the packet's data as an array of bytes.
    *
    * @param[out] dataLength The number of items in the packet data array
    *
    * @return Array of bytes containing packet data, or NULL
    *
    * \note This function creates a new buffer in memory to contain the packet's
    *       data as an array. This buffer must be freed with a call to
    *       DestroyPacketData when it is no longer required.
    */
    _Ret_opt_bytecap_x_(*dataLength) unsigned char*
    GetPacketData(_Out_ unsigned int* dataLength) const
    {
        /* if packet available */
        if (pPacketStreamItem != NULL)
        {
            /* call c API to get the packet data */
            return STAR_getPacketData(
                               (STAR_SPACEWIRE_PACKET*)pPacketStreamItem->item,
                               dataLength);
        }
        else
        {
            /* return NULL / no data */
            *dataLength = 0;
            return NULL;
        }
    }

    /**
    * Frees an array of packet data previously created by a call to
    * GetPacketData().
    *
    * DestroyPacketData() is a static method and may be called directly to
    * destroy packet data when the originating packet has been disposed, or is
    * not available.
    *
    * @param pData The array of packet data to free.
    */
    static void DestroyPacketData(_Post_ptr_invalid_ unsigned char* pData)
    {
        /* call c API to free the packet data */
        STAR_destroyPacketData(pData);
    }

    /**
    * Sets the address of the packet.
    *
    * @param address The new address for the packet.
    *
    * @return 1 if the new address was successfully set, otherwise 0 if
    *         unsuccessful OR if no packet exists to set the address on.
    *
    */
    int SetPacketAddress(_In_ Address* address)
    {
        /* if packet available */
        if (pPacketStreamItem != NULL)
        {
            /* obtain reference to the SpaceWire address */
            STAR_SPACEWIRE_ADDRESS* pAddress = NULL;
            if (address != NULL)
                pAddress = address->GetAs_STAR_SPACEWIRE_ADDRESS();

            /* call c API to set packet's address */
            return STAR_setPacketAddress(
                              (STAR_SPACEWIRE_PACKET*)pPacketStreamItem->item,
                              pAddress);
        }
        else
        {
            /* return 0 to indicate address set unsuccessful */
            return 0;
        }
    }

    /**
    * Set the data contents for this packet to that provided.
    *
    * \note This function is temporarily unavailable within STAR-System,
    *       pending a bug fix.
    */
    void SetPacketData(_In_count_(dataLen) unsigned char* data,
                       unsigned int dataLen)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(data);
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(dataLen);

        /* \note This method has not been implemented yet within STAR-System */
        throw PacketException();

    }

    /**
    * Gets a pointer to the STAR_STREAM_ITEM which this object forms a wrapper
    * around.
    *
    * \note It is not usually necessary to use this function, it is
    *       used to support internal processing within the C++ API.
    *
    * @return A pointer to a STAR_STREAM_ITEM, or NULL if no stream item exists.
    *
    */
    STAR_STREAM_ITEM *GetStreamItem()
    {
        /* return pointer to packet stream item */
        return this->pPacketStreamItem;
    }

    /**
    * Gets the stream item type associated with the object.
    *
    * @return The stream item type associated with the object.
    *
    */
    StreamItemType GetStreamItemType()
    {
        /* return 'packet' type */
        return STREAM_ITEM_TYPE_PACKET;
    }

    /**
    * Gets the packet's address, if any.
    *
    * \note Received packets will not have an address because the address
    *       information is removed during the routing process. Therefore, this
    *       function is only useful for accessing the address section of a
    *       packet which has been created by the user, prior to it being sent.
    *
    * @return A pointer to a new Address object containing the packet's
    *         address, or NULL if no address found.
    *
    * \note The Address object returned must be freed when no longer
    *       required by using the C++ delete operator.
    *
    */
    _Check_return_ Address* GetPacketAddress()
    {
        /* if packet available */
        if (pPacketStreamItem != NULL)
        {
            /* call c API to get the packet's address */
            STAR_SPACEWIRE_ADDRESS *pAddress = NULL;
            pAddress = STAR_getPacketAddress(
                             (STAR_SPACEWIRE_PACKET*)pPacketStreamItem->item);

            /* if valid address obtained */
            if (pAddress != NULL)
            {
                /* try to create a new SpaceWire address object for the
                 * address */
                Address *pSpaceWireAddress = NULL;
                try
                {
                    pSpaceWireAddress = new Address(pAddress);
                }
                catch (std::bad_alloc& ba)
                {
                    _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                    /* memory allocation failure */
                    pSpaceWireAddress = NULL;
                }

                /* if Address created successfully */
                if (pSpaceWireAddress != NULL)
                {
                    /* return the address */
                    return pSpaceWireAddress;
                }
                else
                {
                    /* destroy the STAR_SPACEWIRE_ADDRESS obtained from the
                     * 'c' API */
                    STAR_destroyAddress(pAddress);

                    /* return NULL to indicate no packet address obtained */
                    return NULL;
                }
            }
            else
            {
                /* return NULL to indicate no packet address available */
                return NULL;
            }
        }
        else
        {
            /* return NULL to indicate no packet address available */
            return NULL;
        }
    }

};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

