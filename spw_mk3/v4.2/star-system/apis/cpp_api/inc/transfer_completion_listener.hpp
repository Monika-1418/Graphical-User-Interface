/**
 * \file transfer_completion_listener.hpp
 *
 * \brief Identifies a listener which has been registered to receive transfer
 *        operation completion events.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Identifies a listener which has been registered to receive transfer operation
 * completion events.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_TRANSFEROPS
 */

#ifndef __STAR_TRANSFER_COMPLETION_LISTENER_CPP__
#define __STAR_TRANSFER_COMPLETION_LISTENER_CPP__

#include "star_system_listener.hpp"
#include "star-api.h"
#include "cpp_api_macros.hpp"
#include "transfer_operation.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace transferoperations {

/* forward declarations of classes required */
class TransferOperation;

/**
* Identifies a listener which has been registered to receive transfer operation
* completion events.
*/
class TransferCompletionListener : public stardundee::com::starsystem::general::STARSystemListener
{
private:
    /** the transfer operation for which a transfer operation completion
     *  listener is being registered */
    TransferOperation *pTransferOp;

    /** the underlying 'c' API struct for the transfer operation which a
     *  transfer completion listener is being registered for */
    STAR_TRANSFER_OPERATION *pSTAR_TRANSFER_OP;

    /** the callback function which receives notifications of transfer
     *  operation completion events */
    STAR_TransferOperationListenerFunc pListenerFunc;

    /** a reference count for the transfer operation which has been registered
     *  for transfer completion events, and which is used to determine the
     *  necessity of having to dispose of the transfer operation when the
     *  listener itself is disposed of */
    TransferOpReferenceCount *pTransferOpReferenceCountandStatus;

public:
    /**
    * Default constructor. Initialise object to default state.
    *
    * \note Listener objects are usually constructed within the C++ API and
    *       provided as return values to functions, rather than being
    *       constructed by the user directly.
    */
    TransferCompletionListener() : pTransferOp(NULL), pSTAR_TRANSFER_OP(NULL),
                                   pListenerFunc(NULL),
                                   pTransferOpReferenceCountandStatus(NULL)
    {
    }

    /**
    * Overloaded constructor, initialise with a specified transfer operation
    * and pointer to callback function.
    *
    * \note Listener objects are usually constructed within the C++ API and
    *       provided as return values to functions, rather than being
    *       constructed by the user directly. Calling this constructor with
    *       a valid parameters implies a listener has been successfully
    *       registered (and will be later unregistered automatically when this
    *       object is deleted or goes out of scope).
    *
    * @param pTxOp Pointer to the transfer operation that the listener relates
    *              to.
    * @param pSTAROp Pointer to the STAR_TRANSFER_OPERATION struct associated
    *                with the transfer operation.
    * @param pFunc Pointer to the call-back function which will be called by
    *              STAR-System to notify when the transfer operation has
    *              completed.
    * @param pRefCounter Pointer to a reference count for the transfer operation
    *                    which has been provided (and for which a transfer
    *                    completion listener has been registered)
    *
    */
    TransferCompletionListener(TransferOperation *pTxOp,
                               STAR_TRANSFER_OPERATION *pSTAROp,
                               STAR_TransferOperationListenerFunc pFunc,
                               TransferOpReferenceCount *pRefCounter)
                               : pTransferOp(pTxOp),
                                 pSTAR_TRANSFER_OP(pSTAROp),
                                 pListenerFunc(pFunc),
                                 pTransferOpReferenceCountandStatus(pRefCounter)
    {
        /* if a valid transfer op reference count is provided */
        if (pTransferOpReferenceCountandStatus)
        {
            /* increment the reference count, to signify that another object
             * now holds a reference to it */
            pTransferOpReferenceCountandStatus->IncrementReferenceCount();

            /* record that a listener is registered to the transfer op */
        }

        /* create a reference count for the listener itself so we can
         * determine when the last instance of it has been removed and
         * automatically unregister it if required */
        pListenerReferenceCountandStatus = new
            stardundee::com::starsystem::general::ListenerReferenceCount();
        pListenerReferenceCountandStatus->IncrementReferenceCount();
        pListenerReferenceCountandStatus->SetRegistered();
    }

    /**
    * Destructor. Free any resources created by object.
    *
    * The TransferCompletionListener will be automatically unregistered if
    * required, if there are no other references to the same listener still
    * active. The TransferOperation which the listener was registered to will
    * also be automatically disposed of if there are no other objects making
    * use of the transfer operation, i.e. its reference count has reached
    * zero.
    */
    virtual ~TransferCompletionListener()
    {
        STAR_CPP_LISTENER_ATOMIC_TYPE listenerRefCount;
        STAR_CPP_TRANSFER_OP_ATOMIC_TYPE txOpRefCount;

        /* if a reference count exists for the listener */
        if (pListenerReferenceCountandStatus)
        {
            /* decrement reference count */
            listenerRefCount =
                pListenerReferenceCountandStatus->DecrementReferenceCount();

            /* if the reference count is less than 1 */
            if (listenerRefCount < 1)
            {
                /* last reference to listener has now been removed */

                /* if the listener is still registered */
                if (pListenerReferenceCountandStatus->IsRegistered())
                {
                    /* unregister the listener since nothing else refers to
                     * it now */
                    STAR_unregisterTransferCompletionListener(
                                                pSTAR_TRANSFER_OP,
                                                pListenerFunc);

                }

                /* destroy the listener reference count */
                delete pListenerReferenceCountandStatus;
            }
        }

        /* if a transfer op reference counts exist for the listener */
        if (pTransferOpReferenceCountandStatus)
        {
            /* decrement transfer op reference count */
            txOpRefCount =
                pTransferOpReferenceCountandStatus->DecrementReferenceCount();

            /* if reference count is less than 1 */
            if (txOpRefCount < 1)
            {
                /* last reference to transfer op has now been removed */

                /* dispose of the transfer operation */
                STAR_disposeTransferOperation(pSTAR_TRANSFER_OP);

                /* delete the transfer op reference count */
                delete pTransferOpReferenceCountandStatus;
            }
        }
        else
        {
            /* the listener hasn't been registered */
        }
    }

    /**
    * Copy constructor.
    *
    * When a copy of an existing TransferCompletionListener is made, the copy
    * shares a reference to the existing TransferCompletionListener. This is
    * rather than registering a new TransferCompletionListener which is a
    * duplicate of that being copied. A reference counting mechanism
    * is employed to manage shared references to a transfer completion listener.
    */
    TransferCompletionListener(const TransferCompletionListener& objToCopyFrom)
        : STARSystemListener()
    {
        /* if listener being copied from holds a reference to valid transfer
         * op reference count */
        if (objToCopyFrom.pTransferOpReferenceCountandStatus)
        {
            /* copy the reference to the transfer op ref count */
            pTransferOpReferenceCountandStatus =
                objToCopyFrom.pTransferOpReferenceCountandStatus;

            /* increment the reference count */
            pTransferOpReferenceCountandStatus->IncrementReferenceCount();

            /* copy references to other fields */
            this->pTransferOp = objToCopyFrom.pTransferOp;
            this->pSTAR_TRANSFER_OP = objToCopyFrom.pSTAR_TRANSFER_OP;
            this->pListenerFunc = objToCopyFrom.pListenerFunc;

            /* copy the pointer to the listener reference count */
            pListenerReferenceCountandStatus =
                objToCopyFrom.pListenerReferenceCountandStatus;

            /* increment the reference count by 1 */
            pListenerReferenceCountandStatus->IncrementReferenceCount();
        }
        else
        {
            /* no useful listener data to copy */
            pTransferOp = NULL;
            pSTAR_TRANSFER_OP = NULL;
            pListenerFunc = NULL;
            pTransferOpReferenceCountandStatus = NULL;
        }
    }

    /**
    * Overloaded assignment operator.
    *
    * When the contents of one TransferCompletionListener object are assigned
    * to another, they will both share a reference to the same underlying
    * transfer completion listener. This is rather than a duplicate being made,
    * which would involve registering an entirely new
    * TransferCompletionListener.
    *
    * Any existing listener held by this object will be unregistered and removed
    * if required before the content of the new listener is assigned to it. The
    * TransferOperation which any existing listener held by this object is
    * registered to will also be disposed of if required, if its reference count
    * has reached zero.  A reference counting mechanism
    * is employed to manage shared references to a transfer completion listener.
    */
    TransferCompletionListener& operator=(
                    const TransferCompletionListener& objBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &objBeingAssigned)
            return *this;

        /* if a reference count currently exists for the listener */
        if (pListenerReferenceCountandStatus)
        {
            /* decrement reference count */
            STAR_CPP_LISTENER_ATOMIC_TYPE listenerRefCount =
                pListenerReferenceCountandStatus->DecrementReferenceCount();

            /* if the reference count is less than 1 */
            if (listenerRefCount < 1)
            {
                /* last reference to listener has now been removed */

                /* if the listener is still registered */
                if (pListenerReferenceCountandStatus->IsRegistered())
                {
                    /* unregister the listener since nothing else refers to
                     * it now */
                    STAR_unregisterTransferCompletionListener(
                                                pSTAR_TRANSFER_OP,
                                                pListenerFunc);

                }

                /* destroy the listener reference count */
                delete pListenerReferenceCountandStatus;
            }

            /* set this object's pointer to the reference count to
             * NULL since it has freed its interest in the reference
             * count it had */
            pListenerReferenceCountandStatus = NULL;
        }

        /* if 'this' object already holds information about a registered
         * transfer completion listener */
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
                STAR_disposeTransferOperation(pSTAR_TRANSFER_OP);

                /* delete the transfer op reference count */
                delete pTransferOpReferenceCountandStatus;
            }

            /* reset the listener data **/
            pTransferOp = NULL;
            pSTAR_TRANSFER_OP = NULL;
            pListenerFunc = NULL;
            pTransferOpReferenceCountandStatus = NULL;
        }

        /* if listener being assigned from holds a reference to valid transfer
         * op reference count */
        if (objBeingAssigned.pTransferOpReferenceCountandStatus)
        {
            /* copy the reference to the transfer op ref count */
            pTransferOpReferenceCountandStatus =
                objBeingAssigned.pTransferOpReferenceCountandStatus;

            /* increment the reference count */
            pTransferOpReferenceCountandStatus->IncrementReferenceCount();

            /* copy references to other fields */
            this->pTransferOp = objBeingAssigned.pTransferOp;
            this->pSTAR_TRANSFER_OP = objBeingAssigned.pSTAR_TRANSFER_OP;
            this->pListenerFunc = objBeingAssigned.pListenerFunc;

            /* copy the pointer to the listener reference count */
            pListenerReferenceCountandStatus =
                objBeingAssigned.pListenerReferenceCountandStatus;

            /* increment the reference count by 1 */
            pListenerReferenceCountandStatus->IncrementReferenceCount();
        }
        else
        {
            /* no useful listener data to copy */
        }

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Unregister the listener and the Call-back object that was previously
    * registered to be called whenever a transfer operation is completed.
    *
    * @return 1 if listener unregistered successfully, otherwise 0 if failed to
    *         unregister or if the listener has already been unregistered or
    *         hasn't been registered yet.
    *
    */
    virtual int Unregister()
    {
        /* if a reference count exists */
        if (pListenerReferenceCountandStatus)
        {
            /* if the listener is registered */
            if (pListenerReferenceCountandStatus->IsRegistered())
            {
                /* call 'c' API to unregister device listener */
                  if (STAR_unregisterTransferCompletionListener(
                                              pSTAR_TRANSFER_OP,
                                              pListenerFunc))
                {
                    /* set status to be un-registered */
                    pListenerReferenceCountandStatus->SetUnregistered();

                    /* return success */
                    return 1;
                }
                else
                {
                    /* failed to unregister */
                    return 0;
                }
            }
            else
            {
                /* already unregistered */
                return 0;
            }
        }
        else
        {
            /* nothing to unregister */
            return 0;
        }
    }
};

/* end namespace tags */
} /* transferops */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

