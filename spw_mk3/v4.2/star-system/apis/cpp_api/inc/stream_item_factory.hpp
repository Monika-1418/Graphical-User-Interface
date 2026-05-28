/**
 * \file stream_item_factory.hpp
 *
 * \brief Factory, for creating instances of StreamItem objects.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Factory, to create instances of StreamItem objects.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __STAR_STREAM_ITEMS_FACTORY_CPP__
#define __STAR_STREAM_ITEMS_FACTORY_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"
#include "stream_item.hpp"
#include "packet.hpp"
#include "data_chunk.hpp"
#include "time_code.hpp"
#include "link_speed_event.hpp"
#include "link_state_event.hpp"
#include "error_in_data.hpp"
#include "timestamp_event.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {

/**
* Responsible for creating instances of StreamItem objects.
*
* \note This class is mainly required for internal processing within the C++
*       API and is not typically required for external use.
*/
class StreamItemFactory
{
public:
    /**
    * Create and return an instance of a StreamItem object corresponding to the
    * given STAR_STREAM_ITEM.
    *
    * \note This function is mainly required for internal processing within the
    *       C++ API and is not typically required for external use.
    *
    * @param[in] pStreamItem A pointer to a STAR_STREAM_ITEM, used to determine
    *                        the type of StreamItem object to create.
    * @param[in] shouldBeDestroyed A flag that indicates whether the underlying
    *                              stream item should be destroyed when the
    *                              returned StreamItem object is freed.
    *
    * @return A pointer to a new StreamItem object, or NULL.
    *
    */
    static _Check_return_ StreamItem*
    GetStreamItemFor(_In_opt_ STAR_STREAM_ITEM* pStreamItem,
    _In_opt_ bool shouldBeDestroyed = false)
    {
        /* if no valid stream item provided */
        if (pStreamItem == NULL)
        {
            /* can't create useful instance */
            return NULL;
        }

        /* get the stream item's type */
        STAR_STREAM_ITEM_TYPE type = pStreamItem->itemType;

        /* declare pointer to StreamItem object to be created */
        StreamItem *pNewStreamItem = NULL;

        /* create an instance of the appropriate stream item required */
        try
        {
            /* if it is a packet stream item */
            if (type == STAR_STREAM_ITEM_TYPE_SPACEWIRE_PACKET)
            {
                /* create a new Packet object. */
                pNewStreamItem = new Packet(pStreamItem, shouldBeDestroyed);
            }
            /* else, if it is a data chunk stream item */
            else if (type == STAR_STREAM_ITEM_TYPE_DATA_CHUNK)
            {
                /* create a new DataChunk object. */
                pNewStreamItem = new DataChunk(pStreamItem, shouldBeDestroyed);
            }
            /* else, if it is a time-code stream item */
            else if (type == STAR_STREAM_ITEM_TYPE_TIMECODE)
            {
                /* create a new TimeCode object. */
                pNewStreamItem = new TimeCode(pStreamItem, shouldBeDestroyed);
            }
            /* else, if it is a link speed event stream item */
            else if (type == STAR_STREAM_ITEM_TYPE_LINK_SPEED_EVENT)
            {
                /* create a new LinkSpeedEvent object. */
                pNewStreamItem = new LinkSpeedEvent(pStreamItem,
                    shouldBeDestroyed);
            }
            /* else, if it is a link state event stream item */
            else if (type == STAR_STREAM_ITEM_TYPE_LINK_STATE_EVENT)
            {
                /* create a new LinkStateEvent object. */
                pNewStreamItem = new LinkStateEvent(pStreamItem,
                    shouldBeDestroyed);
            }
            /* else, if it is an error in data stream item */
            else if (type == STAR_STREAM_ITEM_TYPE_ERROR_INJECT)
            {
                /* create a new ErrorInData object. */
                pNewStreamItem = new ErrorInData(pStreamItem,
                    shouldBeDestroyed);
            }
            /* else, if it is a timestamp event stream item */
            else if (type == STAR_STREAM_ITEM_TYPE_TIMESTAMP_EVENT)
            {
                /* create a new TimestampEvent object */
                pNewStreamItem = new TimestampEvent(pStreamItem,
                    shouldBeDestroyed);
            }
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* error allocating memory */
            pNewStreamItem = NULL;

        }

        /* return the stream item object created, may be NULL */
        return pNewStreamItem;
    }
};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

