/**
 * \file my_time_code_listener.h
 *
 * \brief A custom TransferCompletionListenerCallback object, to receive
 *        notification of when time-codes are received as part of a completed
 *        receive operation.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * A custom TransferCompletionListenerCallback object, to receive notification
 * of when time-codes are received as part of a completed receive operation.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include <cstring>
#include <cstdio>
#include "transfer_completion_listener_callback.hpp"
#include "transfer_operation.hpp"
#include "receive_operation.hpp"
#include "time_code.hpp"
#include <iostream>

using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::transferoperations;
using namespace std;

#ifndef MY_TIME_CODE_LISTENER_H
#define MY_TIME_CODE_LISTENER_H

/**
 * Define a custom TransferCompletionListenerCallback object for receiving
 * notification of when time-codes are received.
 */
class MyTimeCodeListener : public TransferCompletionListenerCallback
{
private:
    /* the transfer operation that the listener was registered to */
    TransferOperation *pBaseTransferOp;

    /* the channel that the transfer operation is being submitted on */
    Channel *pBaseChannel;

public:
    /* flag to indicate whether completed receives should be re-submitted
     * to receive more items
     */
    static int RECEIVE_MORE_ITEMS;

    /* flag to specify a limit on the number of times to repeat a receive */
    static int RECEIVE_LIMIT;

    /* count of how many times a receive has been repeated */
    static int RECEIVE_COUNT;

    /**
     * Default constructor.
     */
    MyTimeCodeListener() : pBaseTransferOp(NULL),
                           pBaseChannel(NULL)
    {
    }

    /**
     * Overloaded constructor, initialise to the given transfer operation
     * and channel and context info
     *
     * @param pTxOp The transfer operation to associate with this call-back
     *              object and for which a listener is being registered.
     * @param pChannel The channel which the transfer operation is being
     *                 submitted on.
     */
    MyTimeCodeListener(TransferOperation *pTxOp,
                                         Channel *pChannel)
                                        : pBaseTransferOp(pTxOp),
                                          pBaseChannel(pChannel)
    {
    }

    /**
     * Destructor - made virtual to support correct polymorphic
     * deletion of child classes if required.
     */
    virtual ~MyTimeCodeListener()
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
     * Get the transfer operation which is associated with this call-back object.
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
     * @param pChan The channel that the transfer operation associated with 
     *              this call-back object is being submitted on.
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
     * Call-back function which will be called when a transfer operation has
     * completed. In this case, the relevent transfer operation is expected to
     * be a receive operation and is expected to contain a received time-code.
     *
     * @param status The status of the operation.
     */
    virtual void TransferOperationCompleted(TransferOperation::TransferStatus status)
    {
        /** display information for the received time-code **/

        /* if receive operation didn't complete successfully */
        if (status != TransferOperation::TRANSFER_STATUS_COMPLETE)
        {
            /* report error */
            cout << "Receive operation complete with status: ";
            switch (status)
            {
            case TransferOperation::TRANSFER_STATUS_NOT_STARTED:
                cout << "not yet started.";
                break;
            case TransferOperation::TRANSFER_STATUS_STARTED:
                cout << "transfer has begun";
                break;
            case TransferOperation::TRANSFER_STATUS_COMPLETE:
                cout << "completed";
                break;
            case TransferOperation::TRANSFER_STATUS_CANCELLED:
                cout << "cancelled";
                break;
            case TransferOperation::TRANSFER_STATUS_ERROR:
                cout << "Error";
                break;
            default:
                cout << "Unknown";
                break;
            }
            cout << endl;
        }
        else
        {
            /* get a pointer to the transfer operation associated with this
             * call-back object, expected to be a receive operation
             */
            ReceiveOperation *pReceiveOp =
                                  (ReceiveOperation*)GetTransferOperation();

            /* if valid transfer operation exists */
            if (pReceiveOp)
            {
                /* get the received time-code */
                TimeCode *pRxTimeCode =
                                   (TimeCode*)pReceiveOp->GetTransferItem(0);

                /* if failed to obtain item received */
                if (!pRxTimeCode)
                {
                    /* report error */
                    cout << 
                        "Failed to obtain time-code from receive op" << endl;
                }
                else
                {
                    /* display the received time-code */
                    cout << "Received time-code with value: ";
                    cout << (int)pRxTimeCode->GetTimeCodeValue() << endl;

                    /* destroy the time-code */
                    delete pRxTimeCode;

                    /* if the receive op has to be re-submitted, to receive
                     * any further items and the receive limit hasn't been
                     * reached */
                    if ((MyTimeCodeListener::RECEIVE_MORE_ITEMS) &&
                        (MyTimeCodeListener::RECEIVE_COUNT <
                        MyTimeCodeListener::RECEIVE_LIMIT))
                    {
                        /* increment count of times that receive has been
                         * repeated
                         */
                        MyTimeCodeListener::RECEIVE_COUNT++;

                        /* get the channel that the receive op was submitted on,
                         * if available
                         */
                        Channel *pChannel = GetChannel();

                        /* if channel obtained */
                        if (pChannel)
                        {
                            /* resubmit the receive op, to receive the next
                             * time-code
                             */
                            if (!pChannel->SubmitTransferOperation(pReceiveOp))
                            {
                                cout << "Failed to resubmit receive operation"
                                     " after receiving a time-code" << endl;
                                cout << "Receive completed" << endl;
                            }
                        }
                        else
                        {
                            cout << "No channel available to resubmit receive"
                                     " operation to receive more time-codes" << endl;
                            cout << "Receive completed" << endl;
                        }
                    }
                    else
                    {
                        /* receive completed */
                        cout << "Receive completed" << endl;
                    }
                }
            }
            else
            {
                /* report error */
                cout << "No receive operation exists in call-back object" << endl;
            }
        }
    }
};

#endif /* MY_TIME_CODE_LISTENER_H */

