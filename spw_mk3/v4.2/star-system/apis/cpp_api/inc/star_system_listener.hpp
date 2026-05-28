/**
 * \file star_system_listener.hpp
 *
 * \brief Parent class for objects which are used to identify listeners or call-
 *        back objects which have been registered to receive notifications of
 *        STAR-System events, e.g. devices added or removed, channels opened
 *        or closed, etc.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Parent class for objects which are used to identify listeners or call-back
 * objects which have been registered to receive notifications of STAR-System
 * events, e.g. devices added or removed, channels opened or closed, etc, and
 * which must subsequently be unregistered when notifications are no longer
 * required.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_LISTENER_BASE_CPP__
#define __STAR_LISTENER_BASE_CPP__

/* support for 'atomic' operations, e.g. increment or decrement of values
 * which may be shared amongst several threads */
#if defined(_WIN32)
    #include <windows.h>
    #define STAR_CPP_LISTENER_ATOMIC_TYPE LONG
#elif (defined(__linux__) || defined(__CYGWIN__) || defined(__APPLE__))
    #ifndef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
        #warning "Atomic compare and swap not supported natively in this version of GCC"
    #endif
    #include <limits.h>
    #if (INT_MAX == 0x7FFFFFFF)
        #define STAR_CPP_LISTENER_ATOMIC_TYPE int
    #elif (LONG_MAX == 0x7FFFFFFF)
        #define STAR_CPP_LISTENER_ATOMIC_TYPE long
    #elif (SHRT_MAX == 0x7FFFFFFF)
        #define STAR_CPP_LISTENER_ATOMIC_TYPE short
    #elif (CHAR_MAX == 0x7FFFFFFF)
        #define STAR_CPP_LISTENER_ATOMIC_TYPE char
    #else
        #error "No valid definition of STAR_CPP_LISTENER_ATOMIC_TYPE available"
    #endif
#elif defined(__QNX__)
    #include <atomic.h>
    #include _NTO_CPU_HDR_(smpxchg.h)
    #define STAR_CPP_LISTENER_ATOMIC_TYPE    volatile unsigned
#elif defined(__vxworks)
    #include <intLib.h>
    #define STAR_CPP_LISTENER_ATOMIC_TYPE    int
#endif

#include <stdio.h>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Helper class which holds a reference count for possible shared ownership
* of a listener for STAR-System events, e.g. device added or removed, channels
* opened or closed.
*
* \note This is an internal API class and is not required when using the API.
*
*/
class ListenerReferenceCount
{
private:
    /** the reference count for a listener - this will be treated as an 'atomic'
     * value as it may be accessible by different threads */
    STAR_CPP_LISTENER_ATOMIC_TYPE referenceCount;

    /** records whether the listener is registered (1) or not (0) */
    int isRegistered;

public:
    /**
    * Default constructor
    */
    ListenerReferenceCount() : referenceCount(0), isRegistered(0)
    {
    }

    /**
    * Set the status to indicate listener is registered
    */
    void SetRegistered()
    {
        isRegistered = 1;
    }

    /**
    * Set the status to indicate listener is unregistered
    */
    void SetUnregistered()
    {
        isRegistered = 0;
    }

    /**
    * Get whether the listener is registered.
    *
    * @return Whether the listener is registered (1) or not (0)
    */
    int IsRegistered()
    {
        return isRegistered;
    }

    /**
    * Increment the reference count for the listener, and return the updated
    * value.
    *
    * @return The new value of the reference count
    *
    * \note This in an 'atomic' operation, i.e. designed to be thread-safe
    *       with regards to accessing the reference count for the listener.
    */
    STAR_CPP_LISTENER_ATOMIC_TYPE IncrementReferenceCount()
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
    * Decrement the reference count for the listener, and return the updated
    * value.
    *
    * @return The new value of the reference count
    *
    * \note This in an 'atomic' operation, i.e. designed to be thread-safe
    *       with regards to accessing the reference count for the listener.
    *
    */
    STAR_CPP_LISTENER_ATOMIC_TYPE DecrementReferenceCount()
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
 * Parent class for objects which are used to identify listeners or call-back
 * objects which have been registered to receive notifications of STAR-System
 * events, e.g. devices added or removed, channels opened or closed, etc, and
 * which must subsequently be unregistered when notifications are no longer
 * required.
*/
class STARSystemListener
{
protected:
    /** the reference count and status of the listener */
    ListenerReferenceCount *pListenerReferenceCountandStatus;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    STARSystemListener() : pListenerReferenceCountandStatus(NULL)
    {
    }

    /**
    * Destructor - made virtual to support correct polymorphic
    * deletion of child classes.
    */
    virtual ~STARSystemListener()
    {
    }

    /**
    * Unregister the listener and the Call-back object that was previously
    * registered to be called whenever an event of interest occurred, e.g.
    * a device added or removed, a channel opened or closed, etc.
    *
    * @return 1 if listener unregistered successfully, otherwise 0.
    *
    * \note This is a pure virtual function. Derived classes within the API
    *       which represent more specific forms of listener will override this
    *       to provide the necessary unregistration logic.
    *
    */
    virtual int Unregister()=0;
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

