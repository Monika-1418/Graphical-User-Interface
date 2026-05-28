/**
 * \file my_transfer_completion_listener.h
 *
 * \brief Provides an example of a custom TransferCompletionListenerCallback,
 *        required when registering transfer operation completion listeners
 *        with the STAR-System C++ API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides an example of a custom TransferCompletionListenerCallback,
 * required when registering transfer operation completion listeners with the
 * STAR-System C++ API.
 *
 * Users are expected to create their own class for receiving transfer 
 * operation completion notifications, which must be derived as a child class 
 * of the following C++ API class: TransferCompletionListenerCallback.
 *
 * Users must then provide their own custom implementations for the call-back
 * function(s) inherited from the parent class which will be automatically
 * invoked by the STAR-System API when a transfer operation completes.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include <cstring>
#include <cstdio>
#include "transfer_completion_listener_callback.hpp"
#include "transfer_operation.hpp"
#include "channel.hpp"
#include <iostream>

using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::transferoperations;
using namespace std;

#ifndef MY_TRANSFER_COMPLETION_LISTENER_H
#define MY_TRANSFER_COMPLETION_LISTENER_H

/**
 * Define a custom TransferCompletionListenerCallback object.
 */
class MyTransferCompletionListenerCallback
                                    : public TransferCompletionListenerCallback
{
private:
    /* the transfer operation that the listener was registered to */
    TransferOperation *pBaseTransferOp;

    /* the channel that the transfer operation is being submitted on */
    Channel *pBaseChannel;

    /* a string which will hold contextual or identifying information */
    char contextInfo[256];

    /* \note You can add any other context information you may require to
     * assist your own processing. */

public:
    /**
     * Default constructor.
     */
    MyTransferCompletionListenerCallback() : pBaseTransferOp(NULL),
                                             pBaseChannel(NULL)
    {
        contextInfo[0] = '\0';
    }

    /**
     * Overloaded constructor - initialise with given context information for
     * object.
     *
     * @param info A user supplied string which will be used to provide
     *             contextual or identifying information for the call-back
     *             object when displaying status messages.
     */
    explicit MyTransferCompletionListenerCallback(const char *info)
                                           : pBaseTransferOp(NULL),
                                             pBaseChannel(NULL)
    {
        strcpy(contextInfo, info);
    }

    /**
     * Overloaded constructor, initialise to the given transfer operation
     * and channel and context info
     *
     * @param pTxOp The transfer operation to associate with this call-back
     *              object and for which a listener is being registered.
     * @param pChannel The channel which the transfer operation is being
     *                 submitted on.
     * @param info A user supplied string which will be used to provide
     *             contextual or identifying information for the call-back
     *             object when displaying status messages.
     */
    MyTransferCompletionListenerCallback(TransferOperation *pTxOp,
                                         Channel *pChannel, const char *info)
                                        : pBaseTransferOp(pTxOp),
                                          pBaseChannel(pChannel)
    {
        strcpy(contextInfo, info);
    }

    /**
     * Destructor - made virtual to support correct polymorphic
     * deletion of child classes if required.
     */
    virtual ~MyTransferCompletionListenerCallback()
    {
    }

    /**
     * Set the transfer operation to be associated with this call-back object
     * and for which a listener is being registered.
     *
     * @param pTxOp The transfer operation to associate with this call-back
     *              object.
     */
    void SetTransferOperation(TransferOperation *pTxOp)
    {
        pBaseTransferOp = pTxOp;
    }

    /**
     * Get the transfer operation which is associated with this call-back 
     * object.
     *
     * @return The transfer operation associated with this call-back object.
     */
    TransferOperation* GetTransferOperation()
    {
        return pBaseTransferOp;
    }

    /**
     * Set the channel which the transfer operation associated with this call-
     * back object is being submitted on.
     *
     * @param pChan The channel that the transfer operation associated with this
     *              call-back object is being submitted on.
     */
    void SetChannel(Channel *pChannel)
    {
        pBaseChannel = pChannel;
    }

    /**
     * Get the channel which the transfer operation associated with this call-
     * back object is being submitted on.
     *
     * @return The channel that the transfer operation associated with this
     *         call-back object is being submitted on.
     */
    Channel* GetChannel()
    {
        return pBaseChannel;
    }

    /**
     * Custom user implementation of the Call-back function which will be
     * called to provide notification of when a transfer operation has 
     * completed.
     *
     * @param status The status of the operation.
     *
     */
    virtual void TransferOperationCompleted(TransferOperation::TransferStatus status)
    {
        /* \todo Add your own code here - see sample below */
        cout << "Transfer operation completed [in " << contextInfo << "]:" << endl;
        switch (status)
        {
        case TransferOperation::TRANSFER_STATUS_NOT_STARTED:
            cout << "Transfer status: Not yet started." << endl;
            break;
        case TransferOperation::TRANSFER_STATUS_STARTED:
            cout << "Transfer status: Transfer has begun." << endl;
            break;
        case TransferOperation::TRANSFER_STATUS_COMPLETE:
            cout << "Transfer status: Transfer has completed." << endl;
            break;
        case TransferOperation::TRANSFER_STATUS_CANCELLED:
            cout << "Transfer status: Transfer was cancelled." << endl;
            break;
        case TransferOperation::TRANSFER_STATUS_ERROR:
            cout << "Transfer status: Error." << endl;
            break;
        default:
            break;
        }

        /* get a pointer to the transfer operation associated with this
         * call-back object.
         *
         * \note This is held as a member variable of the class from which
         * this class is derived as a child, and will have been provided by
         * the user when registering the transfer completion call-back object
         * along with the pointer to the channel it was submitted on. */
        TransferOperation *pTransferOp = GetTransferOperation();

        /* if valid transfer operation obtained */
        if (pTransferOp)
        {
            /* do things with the transfer operation if required, e.g. compare
             * it to instances of transfer operations that were created to
             * determine which transfer operation has completed, if there are
             * several to choose from OR re-submit the transfer operation
             * again on its channel to receive more items, for example:

             * Channel *pChannel = GetChannel();
             *
             * if (pChannel)
             * {
             *     pChannel->SubmitTransferOperation(pTransferOp);
             *
             * }
             */
        }
        else
        {
            cout << "No transfer operation found" << endl;
        }

        cout << endl;
    }
};

#endif /* MY_TRANSFER_COMPLETION_LISTENER_H */

