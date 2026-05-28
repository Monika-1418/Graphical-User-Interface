/**
 * \file timestamp_event.hpp
 *
 * \brief Holds information for a timestamp event.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information for a timestamp event.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __TIMESTAMP_EVENT_CPP__
#define __TIMESTAMP_EVENT_CPP__

#include "star-api.h"
#include "stream_item_exceptions.hpp"
#include "stream_item.hpp"
#include "timestamp_raw_counters.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {

/**
* Holds information for a timestamp event.
*
* \note If a TimestampEvent originates from a \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       (e.g.
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItem() GetTransferItem() \endlink
*       or
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItemList() GetTransferItemList()\endlink)
*       then the contained data may be disposed of by the underlying C API or
*       when the \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       is destroyed. If the received data is being queued for later processing
*       then it is recommended to queue the contained data rather than the
*       TimestampEvent.
*/
class TimestampEvent : public StreamItem
{
private:
    /** the timestamp direction */
    STAR_TIMESTAMP_DIRECTION direction;

    /** whether receive or transmit timestamp */
    STAR_TIMESTAMP_TYPE type;

    /** the raw counter values associated with the timestamp event */
    TimestampRawCounters *pTimestampRawCounters;

    /* A STAR_STREAM_ITEM which represents the contained data for a timestamp
     * event.
     *
     * The STAR-System API uses a generic data structure to represent data types
     * such as timestamp events: STAR_STREAM_ITEM. This data structure has a
     * field within which identifies the type of item it represents (whether it
     * is a packet, a time-code, data chunk, error in data or event) and
     * contains the data associated for that item. Access to this contained
     * data, on the basis that it is a Timestamp Event, is managed by this
     * class.
     */
    STAR_STREAM_ITEM *pTimestampEventStreamItem;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    TimestampEvent() : pTimestampRawCounters(NULL), 
        pTimestampEventStreamItem(NULL)
    {
    }

    /**
    * Overloaded constructor. Initialize object using a pointer to a stream
    * item which is assumed to contain a TIMESTAMP_EVENT struct.
    *
    * @param[in] streamItem The stream item which contains the data for the
    *                       timestamp event.
    * @param[in] shouldBeDestroyed Whether or not the underlying stream item
    *                              should be destroyed when the timestamp
    *                              event is freed.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    *
    * \throws StreamItemException If the timestamp event fails to create due
    *                             to an error in retreiving the data from the
    *                             stream item provided.
    *
    */
    TimestampEvent(_In_opt_ STAR_STREAM_ITEM* streamItem,
        _In_opt_ bool shouldBeDestroyed = false) : pTimestampRawCounters(NULL)
    {
        /* if a valid stream item is provided */
        if ((streamItem != NULL) &&
              (streamItem->itemType == STAR_STREAM_ITEM_TYPE_TIMESTAMP_EVENT))
        {
            U32 startSyncPulseCount;
            U32 startClockCycleCount;
            U32 startTotalCycleCount;
            U32 endSyncPulseCount;
            U32 endClockCycleCount;
            U32 endTotalCycleCount;
            U32 clockFrequency;

            /* retrieve data pertaining to the timestamp event */
            direction = STAR_getTimestampEventDirection(
                (STAR_TIMESTAMP_EVENT *)(streamItem->item));
            type = STAR_getTimestampEventType(
                (STAR_TIMESTAMP_EVENT *)(streamItem->item));
            STAR_getTimestampEventRawCounters(
                (STAR_TIMESTAMP_EVENT *)(streamItem->item),
                &startSyncPulseCount, &startClockCycleCount,
                &startTotalCycleCount, &endSyncPulseCount,
                &endClockCycleCount, &endTotalCycleCount,
                &clockFrequency);

            /* initialise raw counters instance */
            pTimestampRawCounters = new TimestampRawCounters(
                startSyncPulseCount, startClockCycleCount, 
                startTotalCycleCount, endSyncPulseCount, endClockCycleCount, 
                endTotalCycleCount, clockFrequency);

            /* proceed to store reference to stream item */

            /* set timestamp event stream item to that provided */
            pTimestampEventStreamItem = streamItem;

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
            throw StreamItemException();
        }
    }

    /**
    * Destructor - free any resources no longer required
    */
    ~TimestampEvent()
    {
        /* if timestamp event stream item not destroyed */
        if (pTimestampEventStreamItem != NULL)
        {
            /* automatically destroy the timestamp event stream item */
            DestroyTimestampEvent();
        }
    }

    /**
    * Free any resources held by the TimestampEvent object, e.g. pointers
    * to memory resources, etc.
    */
    void DestroyTimestampEvent()
    {
        /* if valid timestamp event stream item to destroy */
        if (pTimestampEventStreamItem != NULL)
        {
            /* free memory that was allocated for the raw counters */
            if (pTimestampRawCounters != NULL)
            {
                delete pTimestampRawCounters;
            }

            /* if the timestamp event which is held by this object was created 
             * during an earlier create operation (i.e. it isn't just a 
             * reference to an externally created and provided stream item).
             */
            if (holdsReferenceToExternallyCreatedStreamItem == false)
            {
                /* call 'c' API to destroy the timestamp event stream item */
                STAR_destroyStreamItem(pTimestampEventStreamItem);
            }
            else
            {
                /* Memory pointed to for the timestamp event won't be freed since it
                 * was created externally to this object. Instead, the pointer
                 * will be set to NULL, to detach it from the packet pointed
                 * to. This occurs later below.
                 */

                /* The flag which indicates that the memory pointed to was
                 * created externally now has to be reset */
                holdsReferenceToExternallyCreatedStreamItem = false;
            }

            /* ensure timestamp event stream item is set as empty */
            pTimestampEventStreamItem = NULL;
        }
    }

    /**
    * Copy constructor - to allow copies of timestamp event objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    */
    TimestampEvent(const TimestampEvent& timestampEventObj) : StreamItem()
    {
        /* copy timestamp data of object being copied into 'this' */
        direction = timestampEventObj.direction;
        type = timestampEventObj.type;
        pTimestampRawCounters = timestampEventObj.pTimestampRawCounters;

        /* copy timestamp event value of object being copied into 'this' */
        pTimestampEventStreamItem = timestampEventObj.pTimestampEventStreamItem;
    }

    /**
    * Overloaded assignment operator - to allow instances of timestamp event
    * objects to have their contents assigned to each other.
    */
    TimestampEvent& operator=(const TimestampEvent& timestampEventObj)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &timestampEventObj)
            return *this;

        /* copy timestamp data of object being copied into 'this' */
        direction = timestampEventObj.direction;
        type = timestampEventObj.type;
        pTimestampRawCounters = timestampEventObj.pTimestampRawCounters;

        /* copy timestamp event value of object being copied into 'this' */
        pTimestampEventStreamItem = timestampEventObj.pTimestampEventStreamItem;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Returns the type of timestamp that the stream item represents.
    *
    * @return timestamp type.
    */
    STAR_TIMESTAMP_TYPE GetType()
    {
        return type;
    }

    /**
    * Returns the timestamp direction, i.e. whether it is for data being 
    * transmitted or received.
    *
    * @return timestamp direction.
    */
    STAR_TIMESTAMP_DIRECTION GetDirection()
    {
        return direction;
    }

    /**
    * Returns the start value in seconds and remainder parts based on the given 
    * synchronisation pulse frequency.
    *
    * @param syncPulseFrequency the input synchronisation pulse frequency.
    * @param[out] pSeconds the number of whole seconds of the timestamp.
    * @param[out] pRemainder the remainder in nanoseconds.
    *
    * @return 1 if the operation was successful, 0 otherwise.
    */
    int GetStartValue(U32 syncPulseFrequency, _Out_ U32 *pSeconds, 
        _Out_ U32 *pRemainder)
    {
        /* ensure the output parameters are not null */
        if (pSeconds == NULL || pRemainder == NULL)
        {
            return 0;
        }

        U32 seconds;
        U32 remainder;

        /* get timestamp start value in seconds and remainder parts */
        STAR_getTimestampEventStartValue(
            (STAR_TIMESTAMP_EVENT *)pTimestampEventStreamItem->item, 
            syncPulseFrequency, &seconds, &remainder);

        /* set the timestamp value components */
        *pSeconds = seconds;
        *pRemainder = remainder;

        /* return 1 for success */
        return 1;
    }

    /**
    * Returns the end value in seconds and remainder parts based on the given 
    * synchronisation pulse frequency.
    *
    * @param syncPulseFrequency the input synchronisation pulse frequency.
    * @param[out] pSeconds the number of whole seconds of the timestamp.
    * @param[out] pRemainder the remainder in nanoseconds.
    *
    * @return 1 if the operation was successful, 0 otherwise.
    */
    int GetEndValue(U32 syncPulseFrequency, _Out_ U32 *pSeconds, 
        _Out_ U32 *pRemainder)
    {
        /* ensure the output parameters are not null */
        if (pSeconds == NULL || pRemainder == NULL)
        {
            return 0;
        }

        U32 seconds;
        U32 remainder;

        /* get timestamp end value in seconds and remainder parts */
        STAR_getTimestampEventEndValue(
            (STAR_TIMESTAMP_EVENT *)pTimestampEventStreamItem->item, 
            syncPulseFrequency, &seconds, &remainder);

        /* set the timestamp value components */
        *pSeconds = seconds;
        *pRemainder = remainder;

        /* return 1 for success */
        return 1;
    }

    /**
    * Returns the TimestampRawCounters instance associated with this event.
    *
    * @return instance of TimestampRawCounters containing the raw timestamp 
    *         counter values.
    */
    TimestampRawCounters * GetRawCounters()
    {
        /* return 'timestamp raw counters' instance */
        return pTimestampRawCounters;
    }

    /**
    * Gets a pointer to the STAR_STREAM_ITEM which this object forms a wrapper
    * around.
    *
    * @return A pointer to a STAR_STREAM_ITEM, or NULL if no stream item exists.
    *
    */
    STAR_STREAM_ITEM *GetStreamItem()
    {
        /* no STAR_STREAM_ITEM struct is held by this object, return NULL */
        return NULL;
    }

    /**
    * Gets the stream item type associated with the object.
    *
    * @return The stream item type associated with the object.
    *
    */
    StreamItemType GetStreamItemType()
    {
        /* return 'timestamp event' type */
        return STREAM_ITEM_TYPE_TIMESTAMP_EVENT;
    }

};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

