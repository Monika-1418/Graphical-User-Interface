/**
 * \file data_chunk.hpp
 *
 * \brief Represents a chunk of contiguous SpaceWire data within a single
 *        packet.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a chunk of contiguous SpaceWire data within a single packet.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __STAR_DATA_CHUNK_CPP__
#define __STAR_DATA_CHUNK_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"
#include "stream_item_exceptions.hpp"
#include "stream_item.hpp"

namespace stardundee
{
namespace com
{
namespace starsystem
{
/**
 * Classes to represent the stream items which can be transmitted
 * and received over SpaceWire links such as Packets, DataChunks,
 * and Time-codes.
 **/
namespace streamitems {

/**
* Represents a chunk of contiguous SpaceWire data, within a single packet.
*
* \note If a DataChunk originates from a \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       (e.g.
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItem() GetTransferItem() \endlink
*       or
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItemList() GetTransferItemList()\endlink)
*       then the contained data may be disposed of by the underlying C API or
*       when the \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       is destroyed. If the received data is being queued for later processing
*       then it is recommended to queue the contained data rather than the
*       DataChunk.
*/
class DataChunk : public StreamItem
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
    /* A STAR_STREAM_ITEM which represents the contained data for a Data Chunk.
     *
     * The STAR-System API uses a generic data structure to represent data types
     * such as data chunks: STAR_STREAM_ITEM. This data structure has a field
     * within which identifies the type of item it represents (whether it is a
     * packet, a timecode, data chunk, error in data or event) and contains the
     * data associated for that item. Access to this contained data, on the
     * basis that it is a Data Chunk, is managed by this class
     */
    STAR_STREAM_ITEM *pDataChunk;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    DataChunk() : pDataChunk(NULL)
    {
    }

    /**
    * Constructor - create a data chunk and initialise it using the buffer
    * provided.
    *
    * @param[in] data Pointer to start of chunk data.
    * \note The contents of this buffer are copied into data structures
    *       managed by the API. It is safe to dispose of this buffer after this
    *       function completes.
    *
    * @param dataLen The length of the chunk.
    * @param isStart Specifies whether this data chunk represents the start of
    *                a packet (1) or not (0).
    * @param eopType Specifies the end of packet marker type for the data chunk
    *                (may be none).
    *
    * \throws DataChunkException If the Data Chunk fails to create.
    */
    DataChunk(_In_opt_count_(dataLen) unsigned char* data, unsigned int dataLen,
                                          int isStart, EopType eopType)
                                          : pDataChunk(NULL)
    {
        /* create the data chunk */
        int result = CreateDataChunk(data, dataLen, isStart, eopType);

        /* if error creating data chunk */
        if (result == 0)
        {
            /* throw exception */
            throw DataChunkException();
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
    *                              should be destroyed when the data chunk is
    *                              freed.
    *
    * \note Because a pointer to an already existing stream item is provided,
    *       the memory associated with the stream item will not be automatically
    *       freed by this object when it goes out of scope or is destroyed. Only
    *       the pointer reference will be removed.
    *
    */
    DataChunk(_In_opt_ STAR_STREAM_ITEM* streamItem,
        _In_opt_ bool shouldBeDestroyed = false) : pDataChunk(NULL)
    {
        /* if a valid stream item is provided */
        if ((streamItem != NULL) &&
                     (streamItem->itemType == STAR_STREAM_ITEM_TYPE_DATA_CHUNK))
        {
            /* proceed to store reference to stream item */

            /* set data chunk stream item to that provided */
            pDataChunk = streamItem;

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
            throw DataChunkException();
        }
    }

    /**
    * Destructor. Automatically destroys the data chunk if required, if it
    * hasn't been manually destroyed by the user (by calling
    * DestroyDataChunk()).
    */
    virtual ~DataChunk()
    {
        /* if spacewire data chunk / stream item not destroyed */
        if (pDataChunk != NULL)
        {
            /* automatically destroy the data chunk */
            DestroyDataChunk();
        }
    }

    /**
    * Copy constructor - to allow copies of DataChunk objects to be made when
    * being passed by value in functions or returned by value from functions.
    *
    * A new DataChunk will be created which is a duplicate of that being copied,
    * this includes making a duplicate copy of the data content of the data
    * chunk being copied from.
    *
    * \throws DataChunkException If the DataChunk fails to be assigned / copied.
    */
    DataChunk(const DataChunk& dataChunk) : StreamItem()
    {
        /* if the object being copied contains an initialised data chunk */
        if (dataChunk.pDataChunk != NULL)
        {
            /* Perform deep copy of the data chunk. The default strategy for
             * achieving this is to initiate another call to the STAR-System
             * API, to create a new data chunk using the same data as the object
             * being copied.
             */

            /* record result of creating new data chunk */
            int result = 0;

            /* get data from existing data chunk */
            unsigned int dataLen = 0;
            unsigned char* data = dataChunk.GetChunkData(&dataLen);

            /* if data retreived successfully from existing data chunk */
            if (data != NULL)
            {
                /* attempt to create new data chunk to populate 'this' object */
                result = CreateDataChunk(data, dataLen,
                                         dataChunk.IsStartOfPacket(),
                                         dataChunk.GetChunkEOP());
            }

            /* if error creating data chunk, or if no data retreived from
             * existing data chunk*/
            if (result == 0)
            {
                /* throw exception (can't return error code from constructor) */
                throw DataChunkException();
            }
        }
        else
        {
            /* perform shallow copy of object data */
            this->pDataChunk = NULL;
        }
    }

    /**
    * Overloaded assignment operator - to allow instances of DataChunk objects
    * to have their contents assigned to each other.
    *
    * A new DataChunk will be created which is a duplicate of that being
    * assigned, this includes making a duplicate copy of the data content of
    * the data chunk being assigned.
    *
    * \throws DataChunkException If the Data Chunk fails to be assigned / copied.
    */
    DataChunk& operator=(const DataChunk& dataChunk)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &dataChunk)
            return *this;

        /* delete contents of current data chunk if any, they are about to
         * replaced */
        DestroyDataChunk();

        /* if the object being copied contains an initialised data chunk */
        if (dataChunk.pDataChunk != NULL)
        {
            /* Perform deep copy of the data chunk. The default strategy for
             * achieving this is to initiate another call to the STAR-System
             * API, to create a new data chunk using the same data as the object
             * being copied.
             */

            /* record result of creating new data chunk */
            int result = 0;

            /* get data from existing data chunk */
            unsigned int dataLen = 0;
            unsigned char* data = dataChunk.GetChunkData(&dataLen);

            /* if data retreived successfully from existing data chunk */
            if (data != NULL)
            {
                /* attempt to create new data chunk to populate 'this' object */
                result = CreateDataChunk(data, dataLen,
                                         dataChunk.IsStartOfPacket(),
                                         dataChunk.GetChunkEOP());
            }

            /* if error creating data chunk, or if no data retreived from
             * existing data chunk*/
            if (result == 0)
            {
                /* throw exception (can't return error code from constructor) */
                throw DataChunkException();
            }
        }
        else
        {
            /* perform shallow copy of object data */
            this->pDataChunk = NULL;
        }

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Creates a DataChunk, used to refer to part of a buffer.
    *
    * @param[in] data Pointer to start of chunk data.
    * \note The contents of this buffer are copied into data structures
    *       managed by the API. It is safe to dispose of this buffer after this
    *       function completes.
    *
    * @param dataLen The length of the chunk.
    * @param isStart Specifies whether this data chunk represents the start of
    *                a packet (1) or not (0).
    * @param eopType Specifies the end of packet marker type for the data chunk
    *                (may be none).
    *
    * @return 1 if data chunk created successfully, otherwise 0
    */
    _Check_return_ int CreateDataChunk(_In_opt_count_(dataLen) unsigned char* data,
                                        unsigned int dataLen,
                                        int isStart,
                                        EopType eopType)
    {
        /* if there is already a previously created data chunk */
        if (pDataChunk != NULL)
        {
            /* destroy the existing data chunk */
            DestroyDataChunk();
        }

        /* call 'c' API to create a data chunk stream item */
        pDataChunk = STAR_createDataChunk(data, dataLen, isStart,
                                          (STAR_EOP_TYPE)eopType);

        /* if data chunk stream item created successfully */
        if (pDataChunk != NULL)
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
    * Free any resources held by the DataChunk object, e.g. pointers
    * to memory resources, etc.
    */
    void DestroyDataChunk()
    {
        /* if valid data chunk stream item to destroy */
        if (pDataChunk != NULL)
        {
            /* if the data chunk which is held by this object was created during
             * an earlier create operation (i.e. it isn't just a reference to an
             * externally created and provided stream item).
             */
            if (holdsReferenceToExternallyCreatedStreamItem == false)
            {
                /* call 'c' API to destroy the data chunk stream item */
                STAR_destroyStreamItem(pDataChunk);
            }
            else
            {
                /* Memory pointed to for the data chunk won't be freed since it
                 * was created externally to this object. Instead, the pointer
                 * will be set to NULL, to detach it from the data chunk pointed
                 * to. This occurs later below.
                 */

                /* The flag which indicates that the memory pointed to was
                 * created externally now has to be reset */
                holdsReferenceToExternallyCreatedStreamItem = false;
            }

            /* ensure data chunk stream item is set as empty */
            pDataChunk = NULL;
        }
    }

    /**
    * Gets a pointer to the chunk's data.
    *
    * @param[out] length The length of the data chunk
    *
    * @return Pointer to the chunk's data, or NULL if no data is present or if
    *         the data chunk has not been created yet, therefore contains no data.
    *
    * \note A pointer to the chunk's data is returned, NOT a copy of the data.
    *       Therefore, the data should NOT be freed and it will to cease to
    *       exist if the data chunk is destroyed.
    */
    _Check_return_ unsigned char* GetChunkData(_Out_ unsigned int* length) const
    {
        /* if data chunk available */
        if (pDataChunk != NULL)
        {
            /* call c API to get the chunk data */
            return STAR_getChunkData((STAR_DATA_CHUNK*)pDataChunk->item, length);
        }
        else
        {
            /* return NULL / no data */
            *length = 0;
            return NULL;
        }
    }

    /**
    * Get the length of the chunk's data.
    *
    * @return Length of the chunk's data or zero if no data is present or if
    *         the data chunk has not been created yet.
    */
    unsigned int GetChunkDataLength() const
    {
        /* if data chunk available */
        if (pDataChunk != NULL)
        {
            /* call c API to get the length of the chunk's data */
            return STAR_getChunkDataLength((STAR_DATA_CHUNK*)pDataChunk->item);
        }
        else
        {
            /* return zero / no data */
            return 0;
        }
    }

    /**
    * Get the end of packet marker type for the data chunk.
    *
    * @return The EOP type of the data chunk or EOP_TYPE_INVALID if the
    *         the data chunk is invalid or does not exist yet.
    */
    EopType GetChunkEOP() const
    {
        /* if data chunk available */
        if (pDataChunk != NULL)
        {
            /* call c API to get the chunk's EOP type */
            return (EopType)STAR_getChunkEop(
                                    (STAR_DATA_CHUNK*)pDataChunk->item);
        }
        else
        {
            /* no valid data chunk yet */
            return EOP_TYPE_INVALID;
        }
    }

    /**
    * Determines whether the data chunk is at the start of a packet.
    *
    * @return Whether the data chunk is at the start of a packet (1) or not (0),
    *         or -1 if the data chunk is invalid or does not exist yet.
    */
    int IsStartOfPacket() const
    {
        /* if data chunk available */
        if (pDataChunk != NULL)
        {
            /* call c API to get whether the chunk is SOP or not */
            return STAR_getChunkSop((STAR_DATA_CHUNK*)pDataChunk->item);
        }
        else
        {
            /* no valid data chunk yet */
            return -1;
        }
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
        /* return pointer to data chunk stream item */
        return this->pDataChunk;
    }

    /**
    * Gets the stream item type associated with the object.
    *
    * @return The stream item type associated with the object.
    *
    */
    StreamItemType GetStreamItemType()
    {
        /* return 'data chunk' type */
        return STREAM_ITEM_TYPE_DATACHUNK;
    }

};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

