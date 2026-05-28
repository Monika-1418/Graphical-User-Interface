/**
 * \file receive_operation.hpp
 *
 * \brief Represents a receive operation, used to receive packets
 *        or other stream items on an opened channel.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a receive operation, used to receive packets or other
 * stream items on an opened channel.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_TRANSFEROPS
 */

#ifndef __STAR_RECEIVE_OPERATION_CPP__
#define __STAR_RECEIVE_OPERATION_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"
#include "stream_item_factory.hpp"
#include "transfer_operation_exception.hpp"
#include "transfer_operation.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace transferoperations {

/**
* Represents a receive operation, used to receive packets or other
* stream items on an opened channel
*/
class ReceiveOperation : public TransferOperation
{
public:
    /** the types of stream items to receive */
    enum ReceiveMask
    {
        /** spacewire packets */
        RECEIVE_PACKETS = STAR_RECEIVE_PACKETS,
        /** data chunks */
        RECEIVE_DATACHUNKS = STAR_RECEIVE_CHUNKS,
        /** time-codes */
        RECEIVE_TIMECODES = STAR_RECEIVE_TIMECODES,
        /** flow control characters (not currently supported) */
        RECEIVE_FCTS = STAR_RECEIVE_FCTS,
        /** null characters (not currently supported) */
        RECEIVE_NULLS = STAR_RECEIVE_NULL,
        /** link state events */
        RECEIVE_LINK_STATE_EVENTS = STAR_RECEIVE_LINK_STATE_EVENTS,
        /** link speed events */
        RECEIVE_LINK_SPEED_EVENTS = STAR_RECEIVE_LINK_SPEED_EVENTS
    };

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    ReceiveOperation()
    {
        /* set empty transfer operation */
        pTransferOp = NULL;
    }

    /**
    * Overloaded constructor to automatically create a receive operation using
    * receive parameters provided, which can then be submitted later.
    *
    * @param itemCount The maximum number of stream items to receive. This can
    *                  be -1 to receive an unlimited number of items. The size
    *                  of an individual stream item is not limited
    * \note If a receive operation receiving an unlimited number of items is not
    *       cancelled by the user, then the operation will eventually fail to
    *       allocate new memory for new incoming data, failing the operation.
    * @param mask A bitmask with flags set for the type of traffic one wishes to
    *             receive.
    * \note It is not possible to receive whole packets at the same time as link
    *       control tokens / data chunks.
    *
    * \throws TransferOperationException If the receive operation fails
    *                                         to create.
    */
    ReceiveOperation(int itemCount, int mask)
    {
        /* set empty transfer operation */
        pTransferOp = NULL;

        /* create the receive operation */
        int result = CreateRxOperation(itemCount, mask);

        /* if error creating receive operation */
        if (result == 0)
        {
            /* throw exception */
            throw TransferOperationException();
        }
    }

    /**
    * Destructor. Automatically dispose of the receive operation if required,
    * if it hasn't already been manually disposed of by the user (by calling
    * DisposeTransferOperation()).
    */
    virtual ~ReceiveOperation()
    {
        /* Allow base class destructor (which will be called after this)
         * to clean up as necessary */
    }

    /**
    * Creates a receive operation using receive parameters provided, which can
    * then be submitted later.
    *
    * \note Any existing receive operation currently held by this object will be
    *       disposed of and replaced with a new receive operation using the
    *       values specified.
    *
    * @param itemCount The maximum number of stream items to receive. This can
    *                  be -1 to receive an unlimited number of items. The size
    *                  of an individual stream item is not limited
    * \note If a receive operation receiving an unlimited number of items is not
    *       cancelled by the user, then the operation will eventually fail to
    *       allocate new memory for new incoming data, failing the operation.
    * @param mask A bitmask with flags set for the type of traffic one wishes to
    *             receive.
    * \note It is not possible to receive whole packets at the same time as link
    *       control tokens / data chunks.
    *
    * @return 1 if receive operation created successfully, otherwise 0.
    *
    */
    int CreateRxOperation(int itemCount, int mask)
    {
        /* if there is already a transfer operation in place */
        if (pTransferOp != NULL)
        {
            /* dispose of the existing transfer operation */
            DisposeTransferOperation();
        }

        /* call 'c' API to create a receive operation */
        pTransferOp = STAR_createRxOperation(itemCount, (STAR_RECEIVE_MASK)mask);

        /* if receive operation created successfully */
        if (pTransferOp != NULL)
        {
            /* create reference count for transfer op */
            pTransferOpReferenceCountandStatus = new TransferOpReferenceCount();
            pTransferOpReferenceCountandStatus->IncrementReferenceCount();

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
    * Gets the current count of stream items received by the operation.
    *
    * @return Count of received stream items. NOTE: a value of zero may be
    *         returned if no transfer operation exists (e.g. it hasn't been
    *         created yet).
    */
    unsigned int GetTransferItemCount()
    {
        /* if a transfer operation exists */
        if (pTransferOp != NULL)
        {
            /* get number of stream items received by it */
            return STAR_getTransferItemCount(pTransferOp);
        }
        else
        {
            /* no transfer operation */
            return 0;
        }
    }

    /**
    * Gets the stream item at a given zero-based index of the receive operation.
    *
    * @param index The index of the item to obtain.
    *
    * @return A pointer to a \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *         object for the stream item at the given index of the receive
    *         operation, or NULL if an error occurred or failed to allocate
    *         memory for stream item object.
    *
    * \note The \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       object returned by this function must be freed when no longer
    *       required by calling the C++ delete operator. Although the
    *       \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       object may still exist in memory, its contained state may be
    *       invalid if the originating ReceiveOperation has already been
    *       disposed of or if it has been freed by the underlying C API. If the
    *       received data is being queued for later processing then it is
    *       recommended to queue the contained data rather than the \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink.
    *
    * \note A hierarchy of \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       objects exists to represent the different types of stream items
    *       which can be received, e.g. \link stardundee::com::starsystem::streamitems::Packet Packet \endlink,
    *       \link stardundee::com::starsystem::streamitems::DataChunk DataChunk\endlink,
    *       etc. Whereas the parent class (\link stardundee::com::starsystem::streamitems::StreamItem StreamItem\endlink)
    *       will provide access to a small number of general properties for any
    *       stream item (such as obtaining the stream item type with \link stardundee::com::starsystem::streamitems::StreamItem::GetStreamItemType() GetStreamItemType()\endlink),
    *       it is necessary to cast the \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       object returned from this function to the relevant child class in
    *       order to access the expected properties of the stream item, e.g.
    *       cast it to a \link stardundee::com::starsystem::streamitems::Packet Packet \endlink
    *       object to get the packet data and EOP type, etc.
    */
    _Check_return_ stardundee::com::starsystem::streamitems::StreamItem
    *GetTransferItem(unsigned int index)
    {
        /* if a transfer operation exists */
        if (pTransferOp != NULL)
        {
            /* check the given index is within acceptable bounds */
            unsigned int itemCount = GetTransferItemCount();
            if (index >= itemCount)
            {
                /* invalid index, return error / NULL */
                return NULL;
            }

            /* get the stream item at the specified index */
            STAR_STREAM_ITEM* streamItem = NULL;
            streamItem = STAR_getTransferItem(pTransferOp, index);

            /* if a valid stream item returned */
            if (streamItem != NULL)
            {
                /* create a corresponding StreamItem object for it */
                stardundee::com::starsystem::streamitems::StreamItem
                *newStreamItem =
                stardundee::com::starsystem::streamitems::StreamItemFactory::GetStreamItemFor(streamItem);

                /* if a valid StreamItem object returned */
                if (newStreamItem != NULL)
                {
                    /* return it */
                    return newStreamItem;
                }
            }
        }

        /* else, if no transfer operation exists, or errors encountered in
         * getting the specified transfer operation, return an empty / null
         * stream item
         */
        return NULL;
    }

    /**
    * Get the entire list of transfer items associated with the receive operation.
    *
    * @param[out] count The number of items obtained.
    *
    * @return An array of pointers to StreamItem objects with the transfer items
    *         associated with the receive operation, or NULL if no transfer items
    *         found or if an error occurs during processing.
    *
    * \note The array of pointers to \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       objects returned by this function must be freed when no longer
    *       required by using the C++ delete operator or by calling
    *       FreeTransferItemList(). Although the
    *       \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       object may still exist in memory, its contained state
    *       may be invalid if the originating ReceiveOperation has already been
    *       disposed of or if it has been freed by the underlying C API. If the
    *       received data is being queued for later processing then it is
    *       recommended to queue the contained data rather than the \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink.
    *
    * \note A hierarchy of \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       objects exists to represent the different types of stream items
    *       which can be received, e.g. \link stardundee::com::starsystem::streamitems::Packet Packet \endlink,
    *       \link stardundee::com::starsystem::streamitems::DataChunk DataChunk\endlink,
    *       etc. Whereas the parent class (\link stardundee::com::starsystem::streamitems::StreamItem StreamItem\endlink)
    *       will provide access to a small number of general properties for any
    *       stream item (such as obtaining the stream item type with \link stardundee::com::starsystem::streamitems::StreamItem::GetStreamItemType() GetStreamItemType()\endlink),
    *       it is necessary to cast the \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       objects contained in the list returned from this function to the
    *       relevant child class in order to access the expected properties of
    *       the stream item, e.g. cast it to a \link stardundee::com::starsystem::streamitems::Packet Packet \endlink
    *       object to get the packet data and EOP type, etc.
    */
    stardundee::com::starsystem::streamitems::StreamItem**
                    GetTransferItemList(_Out_ unsigned int* count)
    {
        /* if a transfer operation exists */
        if (pTransferOp != NULL)
        {
            /* call 'c' API to get array of pointers to STAR_STREAM_ITEMs which
             * are associated with the receive.
             */
            unsigned int itemCount = 0;
            STAR_STREAM_ITEM** streamItems = NULL;
            streamItems = STAR_getTransferItemList(pTransferOp, &itemCount);

            /* if no stream items available */
            if ((streamItems == NULL) || (itemCount == 0))
            {
                /* return no items */
                return NULL;
            }

            /* try to create a corresponding array of pointers to StreamItem
             * objects.
             */
            stardundee::com::starsystem::streamitems::StreamItem
            **streamItemObjArray = NULL;
            try
            {
                streamItemObjArray = new stardundee::com::starsystem::streamitems::StreamItem*[itemCount];
            }
            catch(std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                streamItemObjArray = NULL;
            }

            /* if failed to create array */
            if (streamItemObjArray == NULL)
            {
                /* return, no items */
                return NULL;
            }

            /* for each stream item in the receive */
            for (unsigned int i=0; i<itemCount; i++)
            {
                /* create a corresponding StreamItem object to 'wrap' the
                 * stream item.
                 */
                stardundee::com::starsystem::streamitems::StreamItem
                *newStreamObj =
                stardundee::com::starsystem::streamitems::StreamItemFactory::GetStreamItemFor(streamItems[i], true);

                /* add this to the array of StreamItem objects being populated */
                streamItemObjArray[i] = newStreamObj;
            }

            /* record count of items found in the receive */
            *count = itemCount;

            /* destroy the transfer item list but not the contained items */
            STAR_destroyTransferItemList(streamItems, itemCount, 0);

            /* return array of pointers to StreamItem objects */
            return streamItemObjArray;
        }
        else
        {
            /* no transfer operation to query, so no transfer items to return */
            return NULL;
        }
    }

    /**
    * Free the memory associated with an array of pointers to StreamItem objects
    * previously obtained from a call to GetTransferItemList().
    *
    * @param streamItems An array of pointers to StreamItem objects to free.
    * @param count The number of elements in the array.
    *
    */
    void FreeTransferItemList(
           stardundee::com::starsystem::streamitems::StreamItem** streamItems,
           unsigned int count)
    {
        /* if valid stream item list */
        if (streamItems != NULL)
        {
            /* for each StreamItem object in the list */
            for (unsigned int i=0; i<count; i++)
            {
                /* if valid list element */
                if (streamItems[i] != NULL)
                {
                    /* free the memory it occupies */
                    delete streamItems[i];
                }
            }

            /* delete the list itself */
            delete [] streamItems;
        }
    }
};

/* end namespace tags */
} /* transferops */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

