/**
 * \file transfer_operation.hpp
 *
 * \brief Base class for transfer operations. Transfer operations are used to
 *        send and receive stream items (such as packets and time-codes) over a
 *        SpaceWire link.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Base class for transfer operations. Transfer operations are used to send and
 * receive stream items (such as packets and time-codes) over a SpaceWire link.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_TRANSFEROPS
 */

#ifndef __STAR_TRANSFER_OPERATION_CPP__
#define __STAR_TRANSFER_OPERATION_CPP__

#include "star-dundee_annotations.h"
#include "star-api.h"
#include "cpp_api_macros.hpp"

/* support for 'atomic' operations, e.g. increment or decrement of values
 * which may be shared amongst several threads */
#if defined(_WIN32)
    #include <windows.h>
    #define STAR_CPP_TRANSFER_OP_ATOMIC_TYPE LONG
#elif (defined(__linux__) || defined(__CYGWIN__) || defined(__APPLE__))
    #ifndef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
        #warning "Atomic compare and swap not supported natively in this version of GCC"
    #endif
    #include <limits.h>
    #if (INT_MAX == 0x7FFFFFFF)
        #define STAR_CPP_TRANSFER_OP_ATOMIC_TYPE int
    #elif (LONG_MAX == 0x7FFFFFFF)
        #define STAR_CPP_TRANSFER_OP_ATOMIC_TYPE long
    #elif (SHRT_MAX == 0x7FFFFFFF)
        #define STAR_CPP_TRANSFER_OP_ATOMIC_TYPE short
    #elif (CHAR_MAX == 0x7FFFFFFF)
        #define STAR_CPP_TRANSFER_OP_ATOMIC_TYPE char
    #else
        #error "No valid definition of STAR_CPP_TRANSFER_OP_ATOMIC_TYPE available"
    #endif
#elif defined(__QNX__)
    #include <atomic.h>
    #include _NTO_CPU_HDR_(smpxchg.h)
    #define STAR_CPP_TRANSFER_OP_ATOMIC_TYPE    volatile unsigned
#elif defined(__vxworks)
    #include <intLib.h>
    #define STAR_CPP_TRANSFER_OP_ATOMIC_TYPE    int
#endif

namespace stardundee
{
namespace com
{
namespace starsystem
{
/**
 * Classes representing transfer operations which are used to transmit and
 * receive packets, time-codes and other stream item types.
 **/
namespace transferoperations {

/**
* Helper class which holds a reference count for possible shared ownership
* of a transfer operation.
*
* \note This is an internal API class and is not required when using the API.
*
*/
class TransferOpReferenceCount
{
private:
    /** the reference count for a transfer op - this will be treated as an
     *  'atomic' value as it may be accessible by different threads */
    STAR_CPP_TRANSFER_OP_ATOMIC_TYPE referenceCount;

    /** record whether there is a listener registered for the transfer op */
    bool hasListenerRegistered;

public:
    /**
    * Default constructor
    */
    TransferOpReferenceCount() : referenceCount(0), hasListenerRegistered(false)
    {
    }

    /**
    * Set whether a listener is registered for the transfer op.
    *
    * @param value True if listener is registered, otherwise false.
    *
    */
    void SetListenerRegistered(bool value)
    {
        hasListenerRegistered = value;
    }

    /**
    * Get whether a listener is registered for the transfer op.
    *
    * @return True if listener is registered, otherwise false.
    *
    */
    bool IsListenerRegistered()
    {
        return hasListenerRegistered;
    }

    /**
    * Increment the reference count for the transfer op, and return the updated
    * value.
    *
    * @return The new value of the reference count
    *
    * \note This in an 'atomic' operation, i.e. designed to be thread-safe
    *       with regards to accessing the reference count for the transfer op.
    */
    STAR_CPP_TRANSFER_OP_ATOMIC_TYPE IncrementReferenceCount()
    {
#if defined(_WIN32)
        return InterlockedIncrement(&referenceCount);
#elif (defined(__linux__) || defined(__CYGWIN__))
        return __sync_add_and_fetch(&referenceCount, 1);
#elif defined(__QNX__)
        return atomic_add_value(&referenceCount, 1) + 1;
#elif defined(__vxworks)
        int lockkey = intLock();
        referenceCount++;
        intUnlock(lockkey);
        return referenceCount;
#endif
    }

    /**
    * Decrement the reference count for the transfer op, and return the updated
    * value.
    *
    * @return The new value of the reference count
    *
    * \note This in an 'atomic' operation, i.e. designed to be thread-safe
    *       with regards to accessing the reference count for the transfer op.
    *
    */
    STAR_CPP_TRANSFER_OP_ATOMIC_TYPE DecrementReferenceCount()
    {
#if defined(_WIN32)
        return InterlockedDecrement(&referenceCount);
#elif (defined(__linux__) || defined(__CYGWIN__))
        return __sync_add_and_fetch(&referenceCount, -1);
#elif defined(__QNX__)
        return atomic_sub_value(&referenceCount, 1) - 1;
#elif defined(__vxworks)
        int lockkey = intLock();
        referenceCount--;
        intUnlock(lockkey);
        return referenceCount;
#endif
    }
};

/**
* Base class for transfer operations. Transfer operations are used to send and
* receive stream items (such as packets and time-codes) over a SpaceWire link.
*/
class TransferOperation
{
public:
    /** possible states that a transfer operation may be in */
    enum TransferStatus
    {
        /** Not yet started. When a transfer operation is created this will
         * be its status */
        TRANSFER_STATUS_NOT_STARTED = STAR_TRANSFER_STATUS_NOT_STARTED,
        /** Transfer has begun. When a transfer operation is submitted this
         * will be its status until it has completed */
        TRANSFER_STATUS_STARTED = STAR_TRANSFER_STATUS_STARTED,
        /** Transfer has completed. Once a transmit operation has successfully
         * transmitted all of its traffic, or a receive operation has
         * received all of its requested traffic this will be its status */
        TRANSFER_STATUS_COMPLETE = STAR_TRANSFER_STATUS_COMPLETE,
        /** Transfer was cancelled. When a transfer is cancelled by calling
         * CancelTransferOperation(), this will be its status */
        TRANSFER_STATUS_CANCELLED = STAR_TRANSFER_STATUS_CANCELLED,
        /** An error occurred while processing the transfer. This will be the
         * status of a transfer operation if there was an error creating it,
         * submitting it, or there was an error while transmitting or
         * receiving */
        TRANSFER_STATUS_ERROR = STAR_TRANSFER_STATUS_ERROR
    };

    /** timeout periods for transfer operations */
    enum Timeout
    {
        /** wait indefinitely for the transfer operation to complete */
        WAIT_INDEFINITELY = -1
    };

protected:
    /** Reference to a STAR_TRANSFER_OPERATION struct which this object will
     * wrap the associated transfer operation functionality around */
    STAR_TRANSFER_OPERATION *pTransferOp;

    /** the reference count and disposal status of the transfer op */
    TransferOpReferenceCount *pTransferOpReferenceCountandStatus;

    /** a callback function which receives notifications of transfer
     *  operation completion events */
    STAR_TransferOperationListenerFunc pListenerFunc;

public:
    /**
    * Copy constructor - to allow copies of transfer operation objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    * When a copy of an existing TransferOperation is made, the copy shares a
    * reference to the existing TransferOperation. This is rather than creating
    * a new TransferOperation which is a duplicate of that being copied. A
    * reference counting mechanism is employed to manage shared references to
    * a transfer operation.
    */
    TransferOperation(const TransferOperation& transferOp)
    {
        /* copy relevant fields from transfer op being copied from */
        pTransferOp = transferOp.pTransferOp;
        pTransferOpReferenceCountandStatus =
            transferOp.pTransferOpReferenceCountandStatus;
        pListenerFunc = transferOp.pListenerFunc;

        /* if a reference count exists for the transfer op copied from */
        if (pTransferOpReferenceCountandStatus)
        {
            /* increment the reference count */
            pTransferOpReferenceCountandStatus->IncrementReferenceCount();
        }
    }

    /**
    * Overloaded assignment operator - to allow instances of transfer operation
    * objects to have their contents assigned to each other.
    *
    * When one TransferOperation object is assigned to another,
    * they will both share a reference to the same underlying transfer
    * operation. This is rather than a duplicate of the transfer operation
    * being made. A reference counting mechanism is employed to manage shared
    * references to a transfer operation.
    *
    */
    TransferOperation& operator=(const TransferOperation& transferOp)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &transferOp)
            return *this;

        /* if 'this' object already holds a transfer op */
        if (pTransferOpReferenceCountandStatus)
        {
            /* decrement transfer op reference count */
            STAR_CPP_TRANSFER_OP_ATOMIC_TYPE txOpRefCount =
                pTransferOpReferenceCountandStatus->DecrementReferenceCount();

            /* if reference count is less than 1 */
            if (txOpRefCount < 1)
            {
                /* last reference to transfer op has now been removed */

                /* dispose of the transfer operation */
                STAR_disposeTransferOperation(pTransferOp);

                /* delete the transfer op reference count */
                delete pTransferOpReferenceCountandStatus;
            }

            /* reset the transfer op data **/
            pTransferOp = NULL;
            pListenerFunc = NULL;
            pTransferOpReferenceCountandStatus = NULL;
        }

        /* copy relevant fields from transfer op being assigned */
        pTransferOp = transferOp.pTransferOp;
        pTransferOpReferenceCountandStatus =
            transferOp.pTransferOpReferenceCountandStatus;
        pListenerFunc = transferOp.pListenerFunc;

        /* if a reference count exists for the transfer op assigned */
        if (pTransferOpReferenceCountandStatus)
        {
            /* increment the reference count */
            pTransferOpReferenceCountandStatus->IncrementReferenceCount();
        }

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Default constructor. Initialise object to default state.
    */
    TransferOperation() : pTransferOp(NULL),
                          pTransferOpReferenceCountandStatus(NULL),
                          pListenerFunc(NULL)
    {
    }

    /**
    * Destructor. Automatically dispose of the transfer operation if hasn't been
    * already and it isn't being shared with other objects who rely on it for
    * their own clean-up, e.g. any TransferCompletionListener objects which
    * have been registered to the transfer operation - in which case they will
    * dispose of the transfer operation during their own clean-up phase.
    */
    virtual ~TransferOperation()
    {
        DisposeTransferOperation();
    }

    /**
    * Cancels a transfer operation (if it is not already complete) and indicates
    * that the memory associated with the transfer operation should be freed
    * when the operation is no longer in use.
    *
    * @return 1 if the operation was disposed of successfully, otherwise 0. Zero
    *         may also be returned if there is no transfer operation to dispose
    *         of (i.e. it hasn't been created yet and/or has already been
    *         disposed of).
    *
    * \note If a reference to the transfer operation being disposed of is
    *       currently being shared amongst other objects in the API (e.g.
    *       if there are currently TransferCompletionListener objects registered
    *       to it), the transfer operation will not be disposed of immediately
    *       because the other objects in the API may still require access to it
    *       for their own clean-up and unregistration process. In this case,
    *       the behaviour of this function is to request the transfer operation
    *       to be cancelled, but not yet dispose of the transfer operation
    *       itself. It will eventually be disposed of by the other objects in
    *       the API that share a reference to it - whichever is the last to hold
    *       the reference (determined via a reference counting mechanism).
    *
    * \note This is a 'dispose' function rather than a 'destroy' function as it
    *       only indicates to the API that the transfer operation should be
    *       destroyed when it is possible to do so. If the API is in the process
    *       of transmitting or receiving traffic on the operation, it may not be
    *       possible to destroy the operation immediately.
    */
    int DisposeTransferOperation()
    {
        int result = 0;

        /* if there is a transfer operation to dispose of */
        if (pTransferOp != NULL)
        {
            /* if there is a reference count for the transfer op */
            if (pTransferOpReferenceCountandStatus)
            {
                /* decrement reference count */
                STAR_CPP_TRANSFER_OP_ATOMIC_TYPE refCount =
                  pTransferOpReferenceCountandStatus->DecrementReferenceCount();

                /* if reference count is less than 1 */
                if (refCount < 1)
                {
                    /* last reference to transfer op has now been removed */

                    /* call 'c' API to dispose of the transfer operation */
                    result = STAR_disposeTransferOperation(pTransferOp);
                    pTransferOp = NULL;

                    /* delete the transfer op reference count */
                    delete pTransferOpReferenceCountandStatus;
                    pTransferOpReferenceCountandStatus = NULL;
                }
                else
                {
                    /* other objects may still be using the transfer op,
                     * postpone disposing it here, allow other objects to
                     * dispose of it when finished with it */

                    /* however: request the transfer operation to be cancelled
                     * so that its activity will cease. The transferOp won't be
                     * freed yet (until its reference count reaches zero) but it
                     * will effectively dispose of the transfer op without
                     * freeing its memory yet (the memory may be required
                     * elsewhere for unregistering listeners, etc.) */
                    CancelTransferOperation();

                    /* free this object's interest in the transfer op now
                     * that it has decremented its association with it, it is
                     * now up to the other objects which share ownership of it
                     * to free it when required */
                    pTransferOp = NULL;
                    pTransferOpReferenceCountandStatus = NULL;

                    /* set result to '1' to indicate a successful dispose */
                    result = 1;
                }
            }
            else
            {
                /* no reference count, attempt to dispose of it anyway */

                /* call 'c' API to dispose the transfer operation */
                result = STAR_disposeTransferOperation(pTransferOp);
                pTransferOp = NULL;
            }
        }

        /* return result */
        return result;
    }

    /**
    * Cancel the transfer operation.
    * \note It is not currently possible to cancel a transmit operation
    *
    * @return 1 if a receive was cancelled, otherwise 0. Zero may also be
    *         returned if there is no transfer operation to cancel (i.e. it
    *         hasn't been created yet and/or has already been cancelled).
    */
    int CancelTransferOperation()
    {
        /* if there is a transfer operation to cancel */
        if (pTransferOp != NULL)
        {
            /* try to cancel it, call 'c' API to do so */
            return STAR_cancelTransferOperation(pTransferOp);
        }
        else
        {
            /* no transfer operation to cancel */
            return 0;
        }
    }

    /**
    * Cancel any waits in progress on the transfer operation.
    * \note This does not cancel the transfer operation, it simply cancels any
    *       calls to the related method: WaitOnTransferOperationCompletion()
    *
    */
    void CancelTransferOperationWaits()
    {
        /* if there is a transfer operation to cancel waits on */
        if (pTransferOp != NULL)
        {
            /* try to cancel waits on it, call 'c' API to do so */
            STAR_cancelTransferOperationWaits(pTransferOp);
        }
    }

    /**
    * Gets the status of the transfer operation.
    *
    * @return The status of the operation. NOTE: a value of
    *         TRANSFER_STATUS_ERROR may be returned if no transfer
    *         operation exists yet (e.g. because it hasn't been created yet).
    */
    TransferStatus GetTransferOperationStatus()
    {
        /* if a transfer operation exists */
        if (pTransferOp != NULL)
        {
            /* call 'c' API to get its status */
            return (TransferStatus)STAR_getTransferOperationStatus(pTransferOp);
        }
        else
        {
            /* no transfer operation */
            return TRANSFER_STATUS_ERROR;
        }
    }

    /**
    * Blocks until the transfer operation has completed, or the timeout
    * period expires.
    *
    * \note The operation will continue to transmit or receive once this
    *       function returns if it has not yet completed. The operation will
    *       only be stopped if it completes, there is an error, or it is
    *       cancelled by calling CancelTransferOperation(). If the transfer
    *       operation has been submitted but has not yet completed when the
    *       function returns, the return value will be
    *       TRANSFER_STATUS_STARTED as the operation has started but but
    *       not yet completed. This function can be called multiple times for
    *       the same operation to wait until the operation completes.
    *
    * @param timeout Maximum time in milliseconds to wait, or -1 to wait
    *                indefinitely.
    *
    * @return The status of the operation. NOTE: a value of
    *         TRANSFER_STATUS_ERROR may be returned if no transfer
    *         operation exists yet (e.g. because it hasn't been created yet).
    */
    TransferStatus WaitOnTransferOperationCompletion(int timeout)
    {
        /* if a transfer operation exists */
        if (pTransferOp != NULL)
        {
            /* call 'c' API to wait on the transfer operation completion */
            return (TransferStatus)STAR_waitOnTransferOperationCompletion(
                                                         pTransferOp, timeout);
        }
        else
        {
            /* no transfer operation */
            return TRANSFER_STATUS_ERROR;
        }
    }

    /**
    * Return a pointer to the underlying STAR_TRANSFER_OPERATION struct which
    * is held by this object.
    *
    * @return A pointer to a STAR_TRANSFER_OPERATION, or NULL if the struct
    *         hasn't been created/initialised yet.
    *
    * \note This method is provided to support internal processing of other
    *       objects within the C++ API and would not normally require to be
    *       invoked by client applications using the API.
    *
    */
    STAR_TRANSFER_OPERATION *GetSTAR_TRANSFER_OPERATION()
    {
        /* return pointer to underlying STAR_TRANSFER_OPERATION held by this object */
        return pTransferOp;
    }

    /**
    * Set the callback function which is used to receive notifications of
    * transfer operation completion events.
    *
    * \note This method is provided to support internal processing of other
    *       objects within the C++ API and would not normally require to be
    *       invoked by client applications using the API.
    *
    * @param pFunc A pointer to a callback function for transfer completion
    *              events.
    */
    void SetTransferCompletionCallback(STAR_TransferOperationListenerFunc pFunc)
    {
        /* store reference to callback function */
        pListenerFunc = pFunc;
    }

    /**
    * Get the reference count for the transfer operation.
    *
    * \note This method is provided to support internal processing of other
    *       objects within the C++ API and would not normally require to be
    *       invoked by client applications using the API.
    *
    * @return A pointer to the reference count for the transfer operation.
    *
    */
    TransferOpReferenceCount *GetReferenceCount()
    {
        return pTransferOpReferenceCountandStatus;
    }
};

/* end namespace tags */
} /* transferops */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

