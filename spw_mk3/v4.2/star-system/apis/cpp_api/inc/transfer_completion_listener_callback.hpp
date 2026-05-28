/**
 * \file transfer_completion_listener_callback.hpp
 *
 * \brief Parent call-back object for transfer operation completion events, e.g.
 *        for when a transmit or receive of data over a SpaceWire link has
 *        completed.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Parent call-back object for transfer operation completion events, e.g. for
 * when a transmit or receive of data over a SpaceWire link has completed.
 *
 * The STAR-System API allows you to register 'listeners' for various events
 * of interest, e.g. such as devices being added or removed, channels being
 * opened or closed, and so on. When registering a listener one must provide an
 * instance of a call-back object. The STAR-System API will automatically
 * invoke methods on the call-back object provided to notify it of when the
 * events of interest occur, e.g. such as channels being opened or closed.
 *
 * You must provide your own implementation for any call-back object which
 * you register with the API. In this case this would be acheived by deriving
 * your own call-back object as a child-class of the
 * TransferCompletionListenerCallback class and overriding the event
 * notification methods it contains to provide implementations for them. You
 * can then provide an instance of your custom call-back object to the
 * STAR-System API when registering your transfer operation completion
 * listener.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_TRANSFEROPS
 */

#ifndef __STAR_TRANSFER_COMPLETION_LISTENER_CALLBACK_CPP__
#define __STAR_TRANSFER_COMPLETION_LISTENER_CALLBACK_CPP__

#include "star_system_listener_callback.hpp"
#include "cpp_api_macros.hpp"
#include "transfer_operation.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace transferoperations {

/**
 * Parent call-back object for transfer operation completion events, e.g. for
 * when a transmit or receive of data over a SpaceWire link has completed.
 *
 * The STAR-System API allows you to register 'listeners' for various events
 * of interest, e.g. such as devices being added or removed, channels being
 * opened or closed, and so on. When registering a listener one must provide an
 * instance of a call-back object. The STAR-System API will automatically
 * invoke methods on the call-back object provided to notify it of when the
 * events of interest occur, e.g. such as channels being opened or closed.
 *
 * You must provide your own implementation for any call-back object which
 * you register with the API. In this case this would be acheived by deriving
 * your own call-back object as a child-class of the
 * TransferCompletionListenerCallback class and overriding the event
 * notification methods it contains to provide implementations for them. You
 * can then provide an instance of your custom call-back object to the
 * STAR-System API when registering your transfer operation completion
 * listener.
 */
class TransferCompletionListenerCallback : public stardundee::com::starsystem::general::STARSystemListenerCallback
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    TransferCompletionListenerCallback()
    {
    }

    /**
    * Destructor - made virtual to support correct polymorphic
    * deletion of child classes.
    */
    virtual ~TransferCompletionListenerCallback()
    {
    }

    /**
    * Copy constructor.
    */
    TransferCompletionListenerCallback(
        const TransferCompletionListenerCallback& objToCopyFrom) :
        STARSystemListenerCallback()
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(objToCopyFrom);
    }

    /**
    * Overloaded assignment operator.
    */
    TransferCompletionListenerCallback& operator=(
                    const TransferCompletionListenerCallback& objBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &objBeingAssigned)
            return *this;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Call-back function  which will be automatically invoked by the STAR-System
    * API to provide notification of when a transfer operation has completed.
    *
    * \note This is a pure virtual function. Child classes which represent
    *       more specific forms of TransferCompletionListenerCallback must
    *       override this function to undertake whatever processing is required
    *       when receiving notification of a transfer opertion being completed.
    *
    * @param status The status of the operation.
    *
    */
    virtual void TransferOperationCompleted(
                                  TransferOperation::TransferStatus status)=0;
};

/* end namespace tags */
} /* transferops */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

