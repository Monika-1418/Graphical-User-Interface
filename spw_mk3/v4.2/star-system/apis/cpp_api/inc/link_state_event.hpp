/**
 * \file link_state_event.hpp
 *
 * \brief Holds information for a link state event.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information for a link state event.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __STAR_LINK_STATE_EVENT_CPP__
#define __STAR_LINK_STATE_EVENT_CPP__

#include "star-api.h"
#include "stream_item_exceptions.hpp"
#include "stream_item.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {

/**
* Holds information for a link state event.
*
* \note If a LinkStateEvent originates from a \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       (e.g.
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItem() GetTransferItem() \endlink
*       or
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItemList() GetTransferItemList()\endlink)
*       then the contained data may be disposed of by the underlying C API or
*       when the \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       is destroyed. If the received data is being queued for later processing
*       then it is recommended to queue the contained data rather than the
*       LinkStateEvent.
*/
class LinkStateEvent : public StreamItem
{
private:
    U8 count; /* The number of times that the event(s) have occurred */
    unsigned int disconnectError; /* Disconnect error. */
    unsigned int escapeError; /* Escape error. */
    unsigned int linkRunning; /* Link running. */
    unsigned int parityError; /* Parity error. */
    unsigned int receiveCreditError; /* Receive credit error. */
    unsigned int transmitCreditError; /* Transmit credit error. */

    /* A STAR_STREAM_ITEM which represents the contained data for a link state
     * event.
     *
     * The STAR-System API uses a generic data structure to represent data types
     * such as link speed events: STAR_STREAM_ITEM. This data structure has a
     * field within which identifies the type of item it represents (whether it
     * is a packet, a time-code, data chunk, error in data or event) and
     * contains the data associated for that item. Access to this contained
     * data, on the basis that it is a Link State Event, is managed by this
     * class.
     */
    STAR_STREAM_ITEM *pLinkStateEventStreamItem;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    LinkStateEvent() : count(0), disconnectError(0), escapeError(0),
                       linkRunning(0), parityError(0), receiveCreditError(0),
                       transmitCreditError(0), pLinkStateEventStreamItem(NULL)
    {
    }

    /**
    * Overloaded constructor. Initialize object using a pointer to a stream
    * item which is assumed to contain a STAR_LINK_STATE_EVENT struct.
    *
    * @param[in] streamItem The stream item which contains the data for the
    *                       link state event.
    * @param[in] shouldBeDestroyed Whether or not the underlying stream item
    *                              should be destroyed when the link state
    *                              event is freed.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    *
    * \throws StreamItemException If the link state event fails to create due
    *                             to an error in retreiving the data from the
    *                             stream item provided.
    *
    */
    LinkStateEvent(_In_opt_ STAR_STREAM_ITEM* streamItem,
        _In_opt_ bool shouldBeDestroyed = false) : count(0),
        disconnectError(0), escapeError(0), linkRunning(0), parityError(0),
        receiveCreditError(0), transmitCreditError(0)
    {
        /* if a valid stream item is provided */
        if ((streamItem != NULL) &&
              (streamItem->itemType == STAR_STREAM_ITEM_TYPE_LINK_STATE_EVENT))
        {
            /* retrieve data pertaining to the link state event */
            count = STAR_getLinkStateEventCount(
                                   (STAR_LINK_STATE_EVENT*)(streamItem->item));
            disconnectError = (unsigned int)STAR_isLinkStateEventDisconnectError(
                                   (STAR_LINK_STATE_EVENT*)(streamItem->item));
            escapeError = (unsigned int)STAR_isLinkStateEventEscapeError(
                                   (STAR_LINK_STATE_EVENT*)(streamItem->item));
            linkRunning = (unsigned int)STAR_isLinkStateEventLinkRunning(
                                   (STAR_LINK_STATE_EVENT*)(streamItem->item));
            parityError = (unsigned int)STAR_isLinkStateEventParityError(
                                   (STAR_LINK_STATE_EVENT*)(streamItem->item));
            receiveCreditError = (unsigned int)STAR_isLinkStateEventReceiveCreditError(
                                   (STAR_LINK_STATE_EVENT*)(streamItem->item));
            transmitCreditError = (unsigned int)STAR_isLinkStateEventTransmitCreditError(
                                   (STAR_LINK_STATE_EVENT*)(streamItem->item));

            /* proceed to store reference to stream item */

            /* set link state event stream item to that provided */
            pLinkStateEventStreamItem = streamItem;

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
    ~LinkStateEvent()
    {
        /* if link state event stream item not destroyed */
        if (pLinkStateEventStreamItem != NULL)
        {
            /* automatically destroy the link state event stream item */
            DestroyLinkStateEvent();
        }
    }

    /**
    * Free any resources held by the LinkStateEvent object, e.g. pointers
    * to memory resources, etc.
    */
    void DestroyLinkStateEvent()
    {
        /* if valid link state event stream item to destroy */
        if (pLinkStateEventStreamItem != NULL)
        {
            /* if the link state event which is held by this object was created
             * during an earlier create operation (i.e. it isn't just a
             * reference to an externally created and provided stream item).
             */
            if (holdsReferenceToExternallyCreatedStreamItem == false)
            {
                /* call 'c' API to destroy the link state event stream item */
                STAR_destroyStreamItem(pLinkStateEventStreamItem);
            }
            else
            {
                /* Memory pointed to for the link state event won't be freed
                 * since it was created externally to this object. Instead, the
                 * pointer will be set to NULL, to detach it from the packet
                 * pointed to. This occurs later below.
                 */

                /* The flag which indicates that the memory pointed to was
                 * created externally now has to be reset */
                holdsReferenceToExternallyCreatedStreamItem = false;
            }

            /* ensure link state event stream item is set as empty */
            pLinkStateEventStreamItem = NULL;
        }
    }

    /**
    * Copy constructor - to allow copies of link state event objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    */
    LinkStateEvent(const LinkStateEvent& linkStateEventObj) : StreamItem()
    {
        /* copy values from link state event being copied into 'this' */
        count = linkStateEventObj.count;
        disconnectError = linkStateEventObj.disconnectError;
        escapeError = linkStateEventObj.escapeError;
        linkRunning = linkStateEventObj.linkRunning;
        parityError = linkStateEventObj.parityError;
        receiveCreditError = linkStateEventObj.receiveCreditError;
        transmitCreditError = linkStateEventObj.transmitCreditError;

        /* copy link state event value of object being copied into 'this' */
        pLinkStateEventStreamItem = linkStateEventObj.pLinkStateEventStreamItem;
    }

    /**
    * Overloaded assignment operator - to allow instances of link state event
    * objects to have their contents assigned to each other.
    */
    LinkStateEvent& operator=(const LinkStateEvent& linkStateEventObj)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &linkStateEventObj)

        /* copy values from link state event being copied into 'this' */
        count = linkStateEventObj.count;
        disconnectError = linkStateEventObj.disconnectError;
        escapeError = linkStateEventObj.escapeError;
        linkRunning = linkStateEventObj.linkRunning;
        parityError = linkStateEventObj.parityError;
        receiveCreditError = linkStateEventObj.receiveCreditError;
        transmitCreditError = linkStateEventObj.transmitCreditError;

        /* copy link state event value of object being copied into 'this' */
        pLinkStateEventStreamItem = linkStateEventObj.pLinkStateEventStreamItem;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Gets the count of a given link state event, indicating the number of
    * times the specified event has occurred.
    *
    * @return the link state event's count.
    *
    */
    int GetLinkStateEventCount()
    {
        return count;
    }

    /**
    * Gets whether the link state event includes a disconnect error.
    *
    * @return whether the link state event includes a disconnect error
    */
    int IsDisconnectError()
    {
        return (int)disconnectError;
    }

    /**
    * Gets whether the link state event includes an escape error.
    *
    * @return whether the link state event includes an escape error
    */
    int IsEscapeError()
    {
        return (int)escapeError;
    }

    /**
    * Gets whether the link state event includes a link running state change.
    *
    * @return whether the link state event includes a link running state change
    */
    int IsLinkRunning()
    {
        return (int)linkRunning;
    }

    /**
    * Gets whether the link state event includes a parity error.
    *
    * @return whether the link state event includes a parity error
    */
    int IsParityError()
    {
        return (int)parityError;
    }

    /**
    * Gets whether the link state event includes a receive credit error.
    *
    * @return whether the link state event includes a receive credit error
    */
    int IsReceiveCreditError()
    {
        return (int)receiveCreditError;
    }

    /**
    * Gets whether the link state event includes a transmit credit error.
    *
    * @return whether the link state event includes a transmit credit error
    */
    int IsTransmitCreditError()
    {
        return (int)transmitCreditError;
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
        /* return 'link state event' type */
        return STREAM_ITEM_TYPE_LINK_STATE_EVENT;
    }

};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

