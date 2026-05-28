/**
 * \file time_code.hpp
 *
 * \brief Holds information for a time-code which is to be transmitted (or has
 *        been received) over a SpaceWire link.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information for a time-code which is to be transmitted (or has been
 * received) over a SpaceWire link.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __STAR_TIMECODE_CPP__
#define __STAR_TIMECODE_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"
#include "stream_item_exceptions.hpp"
#include "stream_item.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {

/**
* Represents a SpaceWire time-code which is to be transmitted (or has been
* received) over a SpaceWire link
*
* \note If a TimeCode originates from a \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       (e.g.
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItem() GetTransferItem() \endlink
*       or
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItemList() GetTransferItemList()\endlink)
*       then the contained data may be disposed of by the underlying C API or
*       when the \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
*       is destroyed. If the received data is being queued for later processing
*       then it is recommended to queue the contained data rather than the
*       TimeCode.
*/
class TimeCode : public StreamItem
{
private:
    /* A STAR_STREAM_ITEM which represents the contained data for a time-code.
     *
     * The STAR-System API uses a generic data structure to represent data types
     * such as time-codes: STAR_STREAM_ITEM. This data structure has a field
     * within which identifies the type of item it represents (whether it is a
     * packet, a time-code, data chunk, error in data or event) and contains
     * the data associated for that item. Access to this contained data, on the
     * basis that it is a time-code, is managed by this class
     */
    STAR_STREAM_ITEM *pTimeCode;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    TimeCode() : pTimeCode(NULL)
    {
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
    *                              should be destroyed when the time-code is
    *                              freed.
    *
    */
    TimeCode(_In_opt_ STAR_STREAM_ITEM* streamItem,
        _In_opt_ bool shouldBeDestroyed = false) : pTimeCode(NULL)
    {
        /* if a valid stream item is provided */
        if ((streamItem != NULL) &&
                      (streamItem->itemType == STAR_STREAM_ITEM_TYPE_TIMECODE))
        {
            /* proceed to store reference to stream item */

            /* set time-code stream item to that provided */
            pTimeCode = streamItem;

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
            throw TimeCodeException();
        }
    }

    /**
    * Constructor - create a time-code and initialise it using the value
    * provided.
    *
    * @param value time-code value.
    *
    * \throws STAR_TimeCodeException If the time-code fails to create.
    */
    explicit TimeCode(U8 value) : pTimeCode(NULL)
    {
        /* create the time-code */
        int result = CreateTimeCode(value);

        /* if error creating time-code */
        if (result == 0)
        {
            /* throw exception */
            throw TimeCodeException();
        }
    }

    /**
    * Destructor. Automatically destroys the time-code if required, if it
    * hasn't been manually destroyed by the user (by calling the
    * DestroyTimeCode() function).
    */
    virtual ~TimeCode()
    {
        /* if time-code stream item not destroyed */
        if (pTimeCode != NULL)
        {
            /* automatically destroy the time-code */
            DestroyTimeCode();
        }
    }

    /**
    * Copy constructor - to allow copies of time-code objects to be made when
    * being passed by value in functions or returned by value from functions.
    *
    * \throws TimeCodeException If the time-code fails to be assigned / copied.
    */
    TimeCode(const TimeCode& timeCode) : StreamItem()
    {
        /* if the object being copied contains an initialised time-code struct */
        if (timeCode.pTimeCode != NULL)
        {
            /* Perform deep copy of time-code data. The default strategy for
             * achieving this is to initiate another call to the STAR-System
             * API, to create a time-code using the same data as the object
             * being copied.
             */
            int result = CreateTimeCode(timeCode.GetTimeCodeValue());

            /* if error creating time-code */
            if (result == 0)
            {
                /* throw exception (can't return error code from constructor) */
                throw TimeCodeException();
            }

            /* ensure contents of new time-code structure are exactly the same
             * as the source being copied from */
            ((STAR_TIMECODE*)pTimeCode->item)->numSinceLastTx =
                ((STAR_TIMECODE*)timeCode.pTimeCode->item)->numSinceLastTx;
        }
        else
        {
            /* perform shallow copy of object data */
            this->pTimeCode = NULL;
        }
    }

    /**
    * Overloaded assignment operator - to allow instances of time-codes to have
    * their contents assigned to each other.
    *
    * \throws TimeCodeException If the time-code fails to be assigned / copied.
    */
    TimeCode& operator=(const TimeCode& timeCode)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &timeCode)
            return *this;

        /* delete contents of current time-code if any, they are about to replaced */
        DestroyTimeCode();

        /* copy contents of time-code being copied into 'this' */
        if (timeCode.pTimeCode != NULL)
        {
            /* Perform deep copy of time-code data. The default strategy for
             * achieving this is to initiate another call to the STAR-System
             * API, to create a time-code using the same data as the object
             * being copied.
             */
            int result = CreateTimeCode(timeCode.GetTimeCodeValue());

            /* if error creating time-code */
            if (result == 0)
            {
                /* throw exception (can't return error code from constructor) */
                throw TimeCodeException();
            }

            /* ensure contents of new time-code structure are exactly the same
             * as the source being copied from */
            ((STAR_TIMECODE*)pTimeCode->item)->numSinceLastTx =
                ((STAR_TIMECODE*)timeCode.pTimeCode->item)->numSinceLastTx;
        }
        else
        {
            /* perform shallow copy of object data */
            this->pTimeCode = NULL;
        }

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Creates a time-code using the time-code value provided.
    *
    * @param value time-code value.
    *
    * @return 1 if time-code created successfully, otherwise 0
    */
    _Check_return_ int CreateTimeCode(U8 value)
    {
        /* if there is already a previously created time-code */
        if (pTimeCode != NULL)
        {
            /* destroy the existing time-code */
            DestroyTimeCode();
        }

        /* call 'c' API to create a time-code stream item */
        pTimeCode = STAR_createTimeCode(value);

        /* if time-code stream item created successfully */
        if (pTimeCode != NULL)
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
    * Free any resources held by the time-code object, e.g. pointers
    * to memory resources, etc.
    */
    void DestroyTimeCode()
    {
        /* if valid time-code stream item to destroy */
        if (pTimeCode != NULL)
        {
            /* if the time-code which is held by this object was created during
             * an earlier create operation (i.e. it isn't just a reference to an
             * externally created and provided stream item).
             */
            if (holdsReferenceToExternallyCreatedStreamItem == false)
            {
                /* call 'c' API to destroy the time-code stream item */
                STAR_destroyStreamItem(pTimeCode);
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

            /* ensure time-code stream item is set as empty */
            pTimeCode = NULL;
        }
    }


    /**
    * Gets the value of this time-code.
    *
    * @return The time-code value or 0 if no time-code exists (i.e. it hasn't
    *         been created yet).
    */
    U8 GetTimeCodeValue() const
    {
        /* if time-code available */
        if (pTimeCode != NULL)
        {
            /* call c API to get the time-code's value */
            return STAR_getTimeCodeValue((STAR_TIMECODE*)pTimeCode->item);
        }
        else
        {
            /* no valid time-code yet */
            return 0;
        }
    }

    /**
    * Sets the value of this time-code.
    *
    * @param value The new time-code value.
    *
    * @return 1 if the request to set the time-code has been made, or 0 if no
    *         time-code exists (i.e. it hasn't been created yet).
    *
    */
    int SetTimeCodeValue(U8 value)
    {
        /* if time-code available */
        if (pTimeCode != NULL)
        {
            /* call c API to set the time-code's value */
            STAR_setTimeCodeValue((STAR_TIMECODE*)pTimeCode->item, value);

            /* return 1, to indicate request made to set time-code value */
            return 1;
        }
        else
        {
            /* no valid time-code yet */
            return 0;
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
        /* return pointer to time-code stream item */
        return this->pTimeCode;
    }

    /**
    * Gets the stream item type associated with the object.
    *
    * @return The stream item type associated with the object.
    *
    */
    StreamItemType GetStreamItemType()
    {
        /* return 'time-code' type */
        return STREAM_ITEM_TYPE_TIMECODE;
    }
};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

