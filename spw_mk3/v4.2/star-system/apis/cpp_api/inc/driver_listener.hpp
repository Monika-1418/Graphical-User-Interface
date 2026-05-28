/**
 * \file driver_listener.hpp
 *
 * \brief Identifies a listener or call-back object which has been registered to
 *        receive notifications of driver-related events, e.g. drivers added
 *        or removed.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Identifies a listener or call-back object which has been registered to
 * receive notifications of driver-related events, e.g. drivers added or
 * removed, and which must subsequently be unregistered when driver
 * notifications are no longer required.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_DRIVER_LISTENER_CPP__
#define __STAR_DRIVER_LISTENER_CPP__

#include "star_system_listener.hpp"
#include "star-api.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
 * Identifies a listener or call-back object which has been registered to
 * receive notifications of driver-related events, e.g. drivers added or
 * removed, and which must subsequently be unregistered when driver
 * notifications are no longer required.
*/
class DriverListener : STARSystemListener
{
private:
    /** the ID of the driver listener, received when the listener was
      * registered and required to later un-register the listener. */
    STAR_DRIVER_LISTENER_ID listenerID;

public:
    /**
    * Default constructor. Initialise object to default state.
    *
    * \note Listener objects are usually constructed within the C++ API and
    *       provided as return values to functions, rather than being
    *       constructed by the user directly.
    */
    DriverListener() : listenerID(0)
    {
    }

    /**
    * Overloaded constructor, initialise with a given driver listener ID.
    *
    * @param id The ID of the driver listener.
    *
    * \note Listener objects are usually constructed within the C++ API and
    *       provided as return values to functions, rather than being
    *       constructed by the user directly. Calling this constructor with
    *       a valid listener ID implies a listener has been successfully
    *       registered (and will be later unregistered automatically when this
    *       object is deleted or goes out of scope).
    *
    */
    explicit DriverListener(STAR_DRIVER_LISTENER_ID id)
                        : listenerID(id)
    {
        /* if valid listener id (e.g. non-zero) */
        if (id)
        {
            /* create reference count for listener and set status to
             * registered */
            pListenerReferenceCountandStatus = new ListenerReferenceCount();
            pListenerReferenceCountandStatus->IncrementReferenceCount();
            pListenerReferenceCountandStatus->SetRegistered();
        }
    }

    /**
    * Destructor. Free any resources created by object.
    *
    * The DriverListener will be automatically unregistered if required,
    * if there are no other references to the same listener still active.
    */
    virtual ~DriverListener()
    {
        /* if a reference count exists for the listener */
        if (pListenerReferenceCountandStatus)
        {
            /* decrement reference count */
            STAR_CPP_LISTENER_ATOMIC_TYPE refCount =
                pListenerReferenceCountandStatus->DecrementReferenceCount();

            /* if reference count is less than 1 */
            if (refCount < 1)
            {
                /* last reference to listener has now been removed */

                /* if the listener is still registered */
                if (pListenerReferenceCountandStatus->IsRegistered())
                {
                    /* unregister the listener since nothing else refers to
                     * it now */
                    STAR_unregisterDriverListener(listenerID);
                }

                /* destroy the listener reference count */
                delete pListenerReferenceCountandStatus;
            }
        }
    }

    /**
    * Copy constructor - to allow copies of DriverListener objects to be made
    * when being passed by value in functions or returned by value from
    * functions.
    *
    * When a copy of an existing DriverListener is made, the copy
    * shares a reference to the existing DriverListener. This is
    * rather than registering a new DriverListener which is a
    * duplicate of that being copied. A reference counting mechanism
    * is employed to manage shared references to a driver listener.
    */
    DriverListener(const DriverListener& listenerToCopyFrom) :
        STARSystemListener()
    {
        /* if listener being copied from holds a valid listener ID */
        if (listenerToCopyFrom.listenerID)
        {
            /* copy the listener's ID  */
            listenerID = listenerToCopyFrom.listenerID;

            /* copy the pointer to the listener reference count */
            pListenerReferenceCountandStatus =
                listenerToCopyFrom.pListenerReferenceCountandStatus;

            /* increment the reference count by 1 */
            pListenerReferenceCountandStatus->IncrementReferenceCount();
        }
        else
        {
            /* no useful listener data to copy */
            listenerID = 0;
        }
    }

    /**
    * Overloaded assignment operator - to allow instances of DriverListener
    * objects to have their contents assigned to each other.
    *
    * When the contents of one DriverListener object are assigned to another,
    * they will both share a reference to the same underlying driver listener.
    * This is rather than a duplicate being made, which would involve
    * registering an entirely new DriverListener. Any existing listener
    * which is currently held will be removed and unregistered if required (if
    * there are no other references held to it shared amongst other objects)
    * before assigning the new listener to it. A reference counting mechanism
    * is employed to manage shared references to a driver listener.
    */
    DriverListener& operator=(const DriverListener& listenerBeingAssignedFrom)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &listenerBeingAssignedFrom)
            return *this;

        /* if 'this' object already holds information about a listener */
        if ((listenerID) && (pListenerReferenceCountandStatus))
        {
            /* decrement reference count */
            STAR_CPP_LISTENER_ATOMIC_TYPE refCount =
                pListenerReferenceCountandStatus->DecrementReferenceCount();

            /* if reference count is less than 1 */
            if (refCount < 1)
            {
                /* last reference to listener has now been removed */

                /* if the listener is still registered */
                if (pListenerReferenceCountandStatus->IsRegistered())
                {
                    /* unregister the listener since nothing else refers to
                     * it now */
                    STAR_unregisterDriverListener(listenerID);
                }

                /* destroy the listener reference count */
                delete pListenerReferenceCountandStatus;
            }

            /* set this object's pointer to the reference count to
             * NULL since it has freed its interest in the reference
             * count it had */
            pListenerReferenceCountandStatus = NULL;

            /* reset the listener data **/
            listenerID = 0;
        }

        /* if listener being assigned from holds a valid listener ID */
        if (listenerBeingAssignedFrom.listenerID)
        {
            /* copy the listener's ID  */
            listenerID = listenerBeingAssignedFrom.listenerID;

            /* copy the pointer to the listener reference count */
            pListenerReferenceCountandStatus =
                listenerBeingAssignedFrom.pListenerReferenceCountandStatus;

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
    * Get the ID of this listener.
    *
    * @return The listener ID.
    *
    */
    STAR_DRIVER_LISTENER_ID GetListenerID()
    {
        return listenerID;
    }

    /**
    * Unregister the listener and the Call-back object that was previously
    * registered to be called whenever a driver is added or removed.
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
                /* call 'c' API to unregister driver listener */
                if (STAR_unregisterDriverListener(listenerID))
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
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

