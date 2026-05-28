/**
 * \file transmit_operation.hpp
 *
 * \brief Represents a transmit operation, used to transmit packets
 *        or other stream items on an opened channel.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a transmit operation, used to transmit packets or other
 * stream items on an opened channel.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_TRANSFEROPS
 */

#ifndef __STAR_TRANSMIT_OPERATION_CPP__
#define __STAR_TRANSMIT_OPERATION_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"
#include "stream_item.hpp"
#include "transfer_operation_exception.hpp"
#include "transfer_operation.hpp"
#include "deprecation_exceptions.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace transferoperations {

/**
* Represents a transmit operation, used to transmit packets or other
* stream items on an opened channel.
*/
class TransmitOperation : public TransferOperation
{
public:

    /**
    * Default constructor. Initialise object to default state.
    */
    TransmitOperation() : TransferOperation()
    {
    }

    /**
    * Overloaded constructor to automatically create a transmit operation using
    * the stream items provided, which can then be submitted later.
    *
    * \note This constructor will throw an exception if more than one stream
    *       item is passed and has been deprecated. Use alternative that takes
    *       an array of pointers instead.
    *
    * @param pStreamItems An array of \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *                     objects to be sent.
    * \note It is safe to free this array after the function is completed but
    *       any contained \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       objects should only be freed after the transmit has completed.
    * @param streamItemCount The number of \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *                            objects in the array.
    *
    * \throws TransferOperationException If the transmit operation fails to
    *                                    create.
    * \throws ConstructorDeprecatedException If used with more than one stream
    *                                        item.
    *
    * \deprecated Please use alternative TransmitOperation constructor taking
    *             an array of pointers instead.
    */
    TransmitOperation(_In_count_(streamItemCount)
        stardundee::com::starsystem::streamitems::StreamItem *pStreamItems,
        unsigned int streamItemCount) : TransferOperation()
    {
        /* constructor is not compatible with more than one item and has been
         * deprecated
         */
        if (streamItemCount > 1)
        {
            throw stardundee::com::starsystem::general::ConstructorDeprecatedException();
        }

        /* set empty transfer operation */
        pTransferOp = NULL;

        /* create the transmit operation */
        int result = CreateTxOperation(pStreamItems, streamItemCount);

        /* if error creating transmit operation */
        if (result == 0)
        {
            /* throw exception */
            throw TransferOperationException();
        }
    }

    /**
    * Overloaded constructor to automatically create a transmit operation using
    * the stream items provided, which can then be submitted later.
    *
    * @param ppStreamItems An array of pointers to \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *                      objects which are to be sent.
    * \note It is safe to free this array after the function is completed but
    *       any contained \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       objects should only be freed after the transmit has completed.
    * @param streamItemCount The number of \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *                        objects in the array.
    *
    * \throws TransferOperationException If the transmit operation fails to
    *                                    create.
    */
    TransmitOperation(_In_count_(streamItemCount)
        stardundee::com::starsystem::streamitems::StreamItem **ppStreamItems,
        unsigned int streamItemCount)
    {
        /* set empty transfer operation */
        pTransferOp = NULL;

        /* create the transmit operation */
        int result = CreateTxOperation(ppStreamItems, streamItemCount);

        /* if error creating transmit operation */
        if (result == 0)
        {
            /* throw exception */
            throw TransferOperationException();
        }
    }

    /**
    * Destructor. Automatically dispose of the transmit operation if required,
    * if it hasn't already been manually disposed of by the user (by calling the
    * DisposeTransferOperation() function).
    */
    virtual ~TransmitOperation()
    {
        /* Allow base class destructor (which will be called after this)
         * to clean up as necessary */
    }

    /**
    * Create a transmit operation using the stream items provided, which can
    * then be submitted later.
    *
    * \note This function will throw an exception if more than one stream item
    *       is passed and has been deprecated. Use alternative that takes an
    *       array of pointers instead.
    *
    * \note Any existing transmit operation currently held by this object will
    *       be disposed of and replaced with a new transmit operation using the
    *       values specified.
    *
    * @param pStreamItems An array of StreamItem objects to be sent.
    * \note It is safe to free this array after the function is completed but
    *       any contained \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       objects should only be freed after the transmit has completed.
    * @param streamItemCount The number of StreamItems in the array.
    *
    * @return 1 if transmit operation created successfully, otherwise 0.
    *
    * \throws MethodDeprecatedException If used with more than one stream item.
    *
    * \deprecated Please use alternative CreateTxOperation() method taking an
    *             array of pointers instead.
    */
    int CreateTxOperation(_In_count_(streamItemCount)
        stardundee::com::starsystem::streamitems::StreamItem *pStreamItems,
        unsigned int streamItemCount)
    {
        /* method is not compatible with more than one item and has been
         * deprecated
         */
        if (streamItemCount > 1)
        {
            throw stardundee::com::starsystem::general::MethodDeprecatedException();
        }

        /* if no stream items to process */
        if ((pStreamItems == NULL) || (streamItemCount == 0))
        {
            /* return error code */
            return 0;
        }

        /* if there is already a transfer operation in place */
        if (pTransferOp != NULL)
        {
            /* dispose of the existing transfer operation */
            DisposeTransferOperation();
        }

        /* transform array of StreamItem objects into a temporary array of
         * STAR_STREAM_ITEM pointers.
         */

        /* try to create temporary array of STAR_STREAM_ITEM pointers */
        STAR_STREAM_ITEM** ppStreamItems = NULL;
        try
        {
            ppStreamItems = new STAR_STREAM_ITEM*[streamItemCount];
        }
        catch (std::bad_alloc& allocException)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(allocException);
            ppStreamItems = NULL;
        }

        /* if memory failed to create */
        if (ppStreamItems == NULL)
        {
            /* return error */
            return 0;
        }

        /* fill array with pointers to STAR_STREAM_ITEMs which are contained
         * within the StreamItem objects provided. */
        for (unsigned int i=0; i<streamItemCount; i++)
        {
            /* get reference to STAR_STREAM_ITEM pointer held in StreamItem
             * object and add a copy into the STAR_STREAM_ITEM pointer array.
             */
            ppStreamItems[i] = pStreamItems[i].GetStreamItem();
        }

        /* call 'c' API to create a transmit operation, providing the array of
         * pointers to STAR_STREAM_ITEMs and the number of items in the array */
        pTransferOp = STAR_createTxOperation(ppStreamItems, streamItemCount);

        /* free the temporary array of STAR_STREAM_ITEM pointers.
         * NOTE: the elements of the array are not freed, they contain pointers
         * which are simply references to the STAR_STREAM_ITEM pointers which are
         * contained within the associated StreamItem objects. The memory which
         * is occupied by these pointers will be freed by the caller of this
         * function, when the StreamItem objects themselves are destroyed.
         */
        delete [] ppStreamItems;

        /* if transmit operation created successfully */
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
    * Create a transmit operation using the stream items provided, which can
    * then be submitted later.
    *
    * \note Any existing transmit operation currently held by this object will
    *       be disposed of and replaced with a new transmit operation using the
    *       values specified.
    *
    * @param ppStreamItems An array of pointers to StreamItems to be sent.
    *
    * \note It is safe to free this array after the function is completed but
    *       any contained \link stardundee::com::starsystem::streamitems::StreamItem StreamItem \endlink
    *       objects should only be freed after the transmit has completed.
    *
    * @param streamItemCount The number of StreamItems in the array.
    *
    * @return 1 if transmit operation created successfully, otherwise 0.
    *
    */
    int CreateTxOperation(_In_count_(streamItemCount)
        stardundee::com::starsystem::streamitems::StreamItem **ppStreamItems,
        unsigned int streamItemCount)
    {
        /* if no stream items to process */
        if ((ppStreamItems == NULL) || (streamItemCount == 0))
        {
            /* return error code */
            return 0;
        }

        /* if there is already a transfer operation in place */
        if (pTransferOp != NULL)
        {
            /* dispose of the existing transfer operation */
            DisposeTransferOperation();
        }

        /* transform array of StreamItem pointers into a temporary array of
         * STAR_STREAM_ITEM pointers.
         */

        /* try to create temporary array of STAR_STREAM_ITEM pointers */
        STAR_STREAM_ITEM** ppSTAR_STREAM_ITEMs = NULL;
        try
        {
            ppSTAR_STREAM_ITEMs = new STAR_STREAM_ITEM*[streamItemCount];
        }
        catch (std::bad_alloc& allocException)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(allocException);
            /* memory allocation failed */
            ppSTAR_STREAM_ITEMs = NULL;
        }

        /* if memory failed to create */
        if (ppSTAR_STREAM_ITEMs == NULL)
        {
            /* return error */
            return 0;
        }

        /* fill array with pointers to STAR_STREAM_ITEMs which are contained
         * within the StreamItem objects provided. */

        /* for each StreamItem provided */
        for (unsigned int i=0; i<streamItemCount; i++)
        {
            /* get reference to STAR_STREAM_ITEM pointer held in StreamItem
             * object and add a copy into the STAR_STREAM_ITEM pointer array.
             */
            ppSTAR_STREAM_ITEMs[i] = ppStreamItems[i]->GetStreamItem();
        }

        /* call 'c' API to create a transmit operation, providing the array of
         * pointers to STAR_STREAM_ITEMs and the number of items in the array */
        pTransferOp = STAR_createTxOperation(ppSTAR_STREAM_ITEMs, streamItemCount);

        /* free the temporary array of STAR_STREAM_ITEM pointers.
         * NOTE: the elements of the array are not freed, they contain pointers
         * which are simply references to the STAR_STREAM_ITEM pointers which are
         * contained within the associated StreamItem objects. The memory which
         * is occupied by these pointers will be freed by the caller of this
         * function, when the StreamItem objects themselves are destroyed.
         */
        delete [] ppSTAR_STREAM_ITEMs;

        /* if transmit operation created successfully */
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
};

/* end namespace tags */
} /* transferops */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

