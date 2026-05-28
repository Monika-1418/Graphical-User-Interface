/**
 * \file stream_item.hpp
 *
 * \brief Base class for stream items. Stream items are abstract representations
 *        of the data which are typically transmitted and received over a
 *        SpaceWire link, e.g. packets, time-codes, link control tokens.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Base class for stream items. Stream items are abstract representations of the
 * data which are typically transmitted and received over a SpaceWire link,
 * e.g. packets, time-codes, link control tokens.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __STAR_STREAM_ITEMS_BASE_CPP__
#define __STAR_STREAM_ITEMS_BASE_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {

/**
* Base class for stream items. Stream items are abstract representations of the
* data which are typically transmitted and received over a SpaceWire link,
* e.g. packets, time-codes, link control tokens.
*
* \note If a StreamItem originates from a \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       (e.g.
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItem() GetTransferItem() \endlink
*       or
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItemList() GetTransferItemList()\endlink)
*       then the contained data may be disposed of by the underlying C API or
*       when the \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       is destroyed. If the received data is being queued for later processing
*       then it is recommended to queue the contained data rather than the
*       StreamItem.
*/
class StreamItem
{
public:
    /** the types of stream item that can be transmitted and/or received over
     * a SpaceWire link */
    enum StreamItemType
    {
        /** a SpaceWire packet */
        STREAM_ITEM_TYPE_PACKET = STAR_STREAM_ITEM_TYPE_SPACEWIRE_PACKET,
        /** a time-code */
        STREAM_ITEM_TYPE_TIMECODE = STAR_STREAM_ITEM_TYPE_TIMECODE,
        /** a link state event */
        STREAM_ITEM_TYPE_LINK_STATE_EVENT = STAR_STREAM_ITEM_TYPE_LINK_STATE_EVENT,
        /** a datachunk */
        STREAM_ITEM_TYPE_DATACHUNK = STAR_STREAM_ITEM_TYPE_DATA_CHUNK,
        /** a link speed event */
        STREAM_ITEM_TYPE_LINK_SPEED_EVENT = STAR_STREAM_ITEM_TYPE_LINK_SPEED_EVENT,
        /** an error in data */
        STREAM_ITEM_TYPE_ERROR_INJECT = STAR_STREAM_ITEM_TYPE_ERROR_INJECT,
        /** a timestamp event */
        STREAM_ITEM_TYPE_TIMESTAMP_EVENT = STAR_STREAM_ITEM_TYPE_TIMESTAMP_EVENT
    };

protected:
    /** record whether the stream item object has been initialized to an
     * externally created stream item (i.e. rather than having created the
     * stream item structure itself via a call to the STAR-System API) */
    bool holdsReferenceToExternallyCreatedStreamItem;

public:
    /**
    * Default constructor. Perform object initialization
    */
    StreamItem() : holdsReferenceToExternallyCreatedStreamItem(false)
    {
    }

    /**
    * Destructor - to clean up. This is declared as virtual to allow correct,
    * polymorphic deletion of instances of child classes.
    */
    virtual ~StreamItem()
    {
    }

    /**
    * Gets a pointer to the STAR_STREAM_ITEM which this object forms a wrapper
    * around.
    *
    * @return A pointer to a STAR_STREAM_ITEM, or NULL if no stream item exists.
    *
    * \note This is a pure virtual function. Child classes are expected to
    *       override this function and provide their own implementation based
    *       upon the specific form of STAR_STREAM_ITEM that they represent.
    */
    virtual STAR_STREAM_ITEM *GetStreamItem() = 0;

    /**
    * Gets the stream item type associated with the object.
    *
    * @return The stream item type associated with the object.
    *
    * \note This is a pure virtual function. Child classes are expected to
    *       override this function and provide their own implementation based
    *       upon the specific form of stream item that they represent.
    */
    virtual StreamItemType GetStreamItemType() = 0;
};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

