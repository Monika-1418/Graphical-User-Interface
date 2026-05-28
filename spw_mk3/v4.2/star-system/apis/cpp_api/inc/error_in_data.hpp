/**
 * \file error_in_data.hpp
 *
 * \brief Represents an error which can be injected into a packet.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a type of error which can be injected into data in a packet.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __STAR_SPACEWIRE_ERROR_IN_DATA_CPP__
#define __STAR_SPACEWIRE_ERROR_IN_DATA_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"
#include "stream_item.hpp"
#include "cpp_api_macros.hpp"
#include "stream_item_exceptions.hpp"
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {

/**
* An error in data instance is used to indicate an error to be injected into
* packet data and cause an error to occur on the following data character.
*
* \note If a ErrorInData originates from a \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink 
*       (e.g. 
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItem() GetTransferItem() \endlink 
*       or 
*       \link stardundee::com::starsystem::transferoperations::ReceiveOperation::GetTransferItemList() GetTransferItemList()\endlink) 
*       then the contained data may be disposed of by the underlying C API or 
*       when the \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink 
*       is destroyed. If the received data is being queued for later processing 
*       then it is recommended to queue the contained data rather than the 
*       ErrorInData.
*/
class ErrorInData : public StreamItem
{
private:
    /* A STAR_STREAM_ITEM which represents the contained data for an error in
     * data instance.
     *
     * The STAR-System API uses a generic data structure to represent data types
     * such as packets: STAR_STREAM_ITEM. This data structure has a field within
     * which identifies the type of item it represents (whether it is a packet,
     * a timecode, data chunk, data chunk, error in data or event) and contains
     * the data associated for that item.
     * Access to this contained data, on the basis that it is an error in data,
     * is managed by this class
     */
    STAR_STREAM_ITEM *pErrorInDataStreamItem;

    /* the type of error that is represented */
    STAR_ERROR_IN_DATA_TYPE errorType;

    /**
    * Creates a new error in data from user provided data.
    *
    * @param[in] errorType The type of error that is represented.
    *
    * @return 1 if error in data created successfully, otherwise 0
    */
    _Check_return_ int CreateErrorInData(_In_ STAR_ERROR_IN_DATA_TYPE errorType)
    {
        /* if there is already a previously created error in data */
        if (pErrorInDataStreamItem != NULL)
        {
            /* destroy the existing error in data */
            DestroyErrorInData();
        }

        /* call 'c' API to create an error in data / stream item */
        pErrorInDataStreamItem = STAR_createErrorInData(errorType);

        /* store local error type */
        this->errorType = errorType;

        /* if error in data stream item created successfully */
        if (pErrorInDataStreamItem != NULL)
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
    ErrorInData() : pErrorInDataStreamItem(NULL)
    {
    }

    /**
    * Constructor - use given error in data information to automatically create
    * an error in data stream item.
    *
    * @param[in] errorType The type of error that is represented.
    *
    * \throws ErrorInDataException If the ErrorInData fails to create.
    */
    explicit ErrorInData(
        _In_ STAR_ERROR_IN_DATA_TYPE errorType) : pErrorInDataStreamItem(NULL)
    {
        /* create the error in data */
        int result = CreateErrorInData(errorType);

        /* if error creating error in data */
        if (result == 0)
        {
            /* throw exception */
            throw ErrorInDataException();
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
    *                              should be destroyed when the error in data
    *                              is freed.
    *
    * \note Because a pointer to an already existing stream item is provided,
    *       the memory associated with the stream item will not be automatically
    *       freed by this object when it goes out of scope or is destroyed. Only
    *       the pointer reference will be removed.
    */
    ErrorInData(
        _In_ STAR_STREAM_ITEM* streamItem,
        _In_opt_ bool shouldBeDestroyed = false) : pErrorInDataStreamItem(NULL)
    {
        /* if a valid stream item is provided */
        if ((streamItem != NULL) &&
               (streamItem->itemType == STAR_STREAM_ITEM_TYPE_ERROR_INJECT))
        {
            /* proceed to store reference to stream item */

            /* set error in data stream item to that provided */
            pErrorInDataStreamItem = streamItem;

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
            throw ErrorInDataException();
        }
    }

    /**
    * Destructor. This will automatically destroy / dispose of the ErrorInData
    * if required, if it hasn't already been destroyed by the user (by calling
    * DestroyErrorInData()).
    */
    ~ErrorInData()
    {
        /* if error in data not destroyed */
        if(pErrorInDataStreamItem != NULL)
        {
            /* automatically destroy the error in data */
            DestroyErrorInData();
        }
    }

    /**
    * Copy constructor - invoked when a copy of an ErrorInData object is
    * made, e.g. such as through being a return value from a function or when
    * being passed into a function as a 'value' parameter.
    *
    * A new ErrorInData will be created which is a duplicate of the ErrorInData
    * being copied from.
    *
    * @param errorInData The ErrorInData object which is being copied
    *
    * \throws ErrorInDataException If the ErrorInData fails to be copied.
    */
    ErrorInData(const ErrorInData& errorInData) : StreamItem()
    {
        /* if the object being copied contains an initialised error struct */
        if (errorInData.pErrorInDataStreamItem != NULL)
        {
            /* Perform deep copy of error in data. The default strategy for
             * achieving this is to initiate another call to the STAR-System
             * API, to create an error in data using the same data as the
             * object being copied.
             */
            int result = CreateErrorInData(errorInData.GetErrorType());

            /* if error creating address */
            if (result == 0)
            {
                /* throw exception (can't return error code from constructor) */
                throw ErrorInDataException();
            }
        }
        else
        {
            /* perform shallow copy of object data */
            this->pErrorInDataStreamItem = NULL;
        }
    }

    /**
    * Overloaded assignment operator - invoked when an attempt is made to assign
    * the contents of one ErrorInData object into another.
    *
    * Any existing ErrorInData will be destroyed and a new ErrorInData created
    * which is a duplicate of the ErrorInData being assigned.
    *
    * @param errorInData The ErrorInData object whose contents are being
    *                    assigned to 'this' object.
    *
    * \throws ErrorInDataException If ErrorInData fails to be assigned / copied.
    */
    ErrorInData& operator=(const ErrorInData& errorInData)
    {
        /* check for self-assignment, no point in copying one's self */
        if(this == &errorInData)
        {
            return *this;
        }

        /* delete contents of current error if any, they are being replaced */
        DestroyErrorInData();

        /* copy contents of error in data being copied into 'this' */
        if (errorInData.pErrorInDataStreamItem != NULL)
        {
            /* Perform deep copy of address data. The default strategy for
             * achieving this is to initiate another call to the STAR-System
             * API, to create an address using the same data as the object
             * being copied.
             */
            int result = CreateErrorInData(errorInData.GetErrorType());

            /* if error creating error in data */
            if (result == 0)
            {
                /* throw exception */
                throw ErrorInDataException();
            }
        }
        else
        {
            /* perform shallow copy of object data */
            this->pErrorInDataStreamItem = NULL;
        }

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Get the error type of the error in data.
    *
    * @return The error type.
    */
    STAR_ERROR_IN_DATA_TYPE GetErrorType() const
    {
        /* return the contained error type */
        return errorType;
    }

    /**
    * Free any resources held by the ErrorInData object, e.g. pointers
    * to memory resources, etc.
    */
    void DestroyErrorInData()
    {
        /* if valid error in data to destroy */
        if (pErrorInDataStreamItem != NULL)
        {
            /* if the error in data which is held by this object was created
             * during an earlier create operation (i.e. it isn't just a
             * reference to an externally created and provided stream item).
             */
            if (holdsReferenceToExternallyCreatedStreamItem == false)
            {
                /* call 'c' API to destroy the error in data */
                STAR_destroyStreamItem(pErrorInDataStreamItem);
            }
            else
            {
                /* Memory pointed to for the error in data won't be freed since
                 * it was created externally to this object. Instead, the
                 * pointer will be set to NULL, to detach it from the error in
                 * data pointed to. This occurs later below.
                 */

                /* The flag which indicates that the memory pointed to was
                 * created externally now has to be reset */
                holdsReferenceToExternallyCreatedStreamItem = false;
            }

            /* ensure error in data is set as empty */
            pErrorInDataStreamItem = NULL;
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
    */
    STAR_STREAM_ITEM *GetStreamItem()
    {
        /* return pointer to error in data stream item */
        return this->pErrorInDataStreamItem;
    }

    /**
    * Gets the stream item type associated with the object.
    *
    * @return The stream item type associated with the object.
    */
    StreamItemType GetStreamItemType()
    {
        /* return 'error in data' type */
        return STREAM_ITEM_TYPE_ERROR_INJECT;
    }
};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

