/**
 * \file link_speed_event.hpp
 *
 * \brief Holds information for a link speed event.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information for a link speed event.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __STAR_LINK_SPEED_EVENT_CPP__
#define __STAR_LINK_SPEED_EVENT_CPP__

#include "star-api.h"
#include "stream_item_exceptions.hpp"
#include "stream_item.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {

/**
* Holds information for a link speed event.
*
* \note If a LinkSpeedEvent originates from a \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       (e.g.
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItem() GetTransferItem() \endlink
*       or
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItemList() GetTransferItemList()\endlink)
*       then the contained data may be disposed of by the underlying C API or
*       when the \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       is destroyed. If the received data is being queued for later processing
*       then it is recommended to queue the contained data rather than the
*       LinkSpeedEvent.
*/
class LinkSpeedEvent : public StreamItem
{
private:
    U32 linkSpeed; /* the new link speed represented in bit/s.
                    * Note that this value is approximate. */

    /* A STAR_STREAM_ITEM which represents the contained data for a link speed
     * event.
     *
     * The STAR-System API uses a generic data structure to represent data types
     * such as link speed events: STAR_STREAM_ITEM. This data structure has a
     * field within which identifies the type of item it represents (whether it
     * is a packet, a time-code, data chunk, error in data or event) and
     * contains the data associated for that item. Access to this contained
     * data, on the basis that it is a Link Speed Event, is managed by this
     * class.
     */
    STAR_STREAM_ITEM *pLinkSpeedEventStreamItem;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    LinkSpeedEvent() : linkSpeed(0), pLinkSpeedEventStreamItem(NULL)
    {
    }

    /**
    * Overloaded constructor. Initialize object using a pointer to a stream
    * item which is assumed to contain a STAR_LINK_SPEED_EVENT struct.
    *
    * @param[in] streamItem The stream item which contains the data for the
    *                       link speed event.
    * @param[in] shouldBeDestroyed Whether or not the underlying stream item
    *                              should be destroyed when the link speed
    *                              event is freed.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    *
    * \throws StreamItemException If the link speed event fails to create due
    *                             to an error in retreiving the data from the
    *                             stream item provided.
    *
    */
    LinkSpeedEvent(_In_opt_ STAR_STREAM_ITEM* streamItem,
        _In_opt_ bool shouldBeDestroyed = false) : linkSpeed(0)
    {
        /* if a valid stream item is provided */
        if ((streamItem != NULL) &&
              (streamItem->itemType == STAR_STREAM_ITEM_TYPE_LINK_SPEED_EVENT))
        {
            /* retrieve data pertaining to the link speed event */
            linkSpeed = STAR_getLinkSpeedEventSpeed(
                                   (STAR_LINK_SPEED_EVENT*)(streamItem->item));

            /* proceed to store reference to stream item */

            /* set link speed event stream item to that provided */
            pLinkSpeedEventStreamItem = streamItem;

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
    ~LinkSpeedEvent()
    {
        /* if link speed event stream item not destroyed */
        if (pLinkSpeedEventStreamItem != NULL)
        {
            /* automatically destroy the link speed event stream item */
            DestroyLinkSpeedEvent();
        }
    }

    /**
    * Free any resources held by the LinkSpeedEvent object, e.g. pointers
    * to memory resources, etc.
    */
    void DestroyLinkSpeedEvent()
    {
        /* if valid link speed event stream item to destroy */
        if (pLinkSpeedEventStreamItem != NULL)
        {
            /* if the link speed event which is held by this object was created
             * during an earlier create operation (i.e. it isn't just a
             * reference to an externally created and provided stream item).
             */
            if (holdsReferenceToExternallyCreatedStreamItem == false)
            {
                /* call 'c' API to destroy the link speed event stream item */
                STAR_destroyStreamItem(pLinkSpeedEventStreamItem);
            }
            else
            {
                /* Memory pointed to for the link speed event won't be freed
                 * since it was created externally to this object. Instead, the
                 * pointer will be set to NULL, to detach it from the packet
                 * pointed to. This occurs later below.
                 */

                /* The flag which indicates that the memory pointed to was
                 * created externally now has to be reset */
                holdsReferenceToExternallyCreatedStreamItem = false;
            }

            /* ensure link speed event stream item is set as empty */
            pLinkSpeedEventStreamItem = NULL;
        }
    }

    /**
    * Copy constructor - to allow copies of link speed event objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    */
    LinkSpeedEvent(const LinkSpeedEvent& linkSpeedEventObj) : StreamItem()
    {
        /* copy link speed value of object being copied into 'this' */
        linkSpeed = linkSpeedEventObj.linkSpeed;

        /* copy link speed event value of object being copied into 'this' */
        pLinkSpeedEventStreamItem = linkSpeedEventObj.pLinkSpeedEventStreamItem;
    }

    /**
    * Overloaded assignment operator - to allow instances of link speed event
    * objects to have their contents assigned to each other.
    */
    LinkSpeedEvent& operator=(const LinkSpeedEvent& linkSpeedEventObj)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &linkSpeedEventObj)
            return *this;

        /* copy link speed value of object being copied into 'this' */
        linkSpeed = linkSpeedEventObj.linkSpeed;

        /* copy link speed event value of object being copied into 'this' */
        pLinkSpeedEventStreamItem = linkSpeedEventObj.pLinkSpeedEventStreamItem;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Get the link speed contained in the link speed event.
    *
    * @return The link speed from the link speed event.
    *
    */
    U32 GetLinkSpeed()
    {
        /* return the link speed held */
        return linkSpeed;
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
        /* return 'link speed event' type */
        return STREAM_ITEM_TYPE_LINK_SPEED_EVENT;
    }

};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

