/**
* \file trigger_event_manager.hpp
*
* \brief Provides functions for handling basic trigger events.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling basic trigger events.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_EVENTS
*/

#ifndef __TRIGGER_EVENT_MANAGER_CPP__
#define __TRIGGER_EVENT_MANAGER_CPP__

#include "star-api.h"
#include "triggering_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {

/**
* Functions in this class deal with getting and setting general trigger event
* parameters.
*/
class TriggerEventManager
{
protected:
    /** the ID of the device which the RMAP target configuration relates to */
    STAR_DEVICE_ID idOfOwningDevice;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    TriggerEventManager()
    {
    }

    /**
    * Destructor. Free any resources created.
    */
    virtual ~TriggerEventManager()
    {
        /* no resources to free at present */
    }

    /**
    * Constructor - use given parameters to initialise trigger event manager.
    *
    * @param deviceID The device ID that the trigger event manager relates to.
    */
    explicit TriggerEventManager(STAR_DEVICE_ID deviceID) : 
        idOfOwningDevice(deviceID)
    {
    }

    /**
    * Copy constructor - to allow copies of TriggerEventManager objects to be 
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    TriggerEventManager(const TriggerEventManager& 
        triggerEventManagerStateToCopyFrom) : idOfOwningDevice(0)
    {
        /* copy required contents of object being copied into this object */
        idOfOwningDevice =
            triggerEventManagerStateToCopyFrom.idOfOwningDevice;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * TriggerEventManager objects to have their contents assigned to each other.
    *
    */
    TriggerEventManager& operator=(const TriggerEventManager& 
        triggerEventManagerStateToCopyFrom)
    {
        /* check for self-assignment, no point in copying one's self */
        if(this == &triggerEventManagerStateToCopyFrom)
        {
            return *this;
        }

        /* copy required contents of object being copied into this object */
        idOfOwningDevice =
            triggerEventManagerStateToCopyFrom.idOfOwningDevice;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Gets the input events from a counter which will cause an internal trigger
    * to be set.
    *
    * @param counter Counter to get input events from.
    * @param trigger Internal trigger which is affected by the input events.
    * @param pEvents Pointer to a value which will be updated with the events
    *                mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetCounterInputEvents(U32 counter, U32 trigger,
        COUNTER_EVENT_MASK *pEvents) = 0;

    /**
    * Gets the input events from a port which will cause an internal trigger to
    * be set.
    *
    * @param port Port to get input events from.
    * @param trigger Internal trigger which is affected by the input events.
    * @param pEvents Pointer to a value which will be updated with the events
    *                mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetPortInputEvents(U32 port, U32 trigger,
        PORT_EVENT_MASK *pEvents) = 0;

    /**
    * Gets the input events for an internal trigger which will cause another
    * internal trigger to be set.
    *
    * @param causeTrigger Internal trigger to get input events from.
    * @param trigger Internal trigger which is affected by the input events.
    * @param pEvents Pointer to a value which will be updated with the events
    *                mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetTriggerInputEvents(U32 causeTrigger, U32 trigger,
        TRIGGER_EVENT_MASK *pEvents) = 0;

    /**
    * Sets the input events for a counter which will cause an internal trigger
    * to be set.
    *
    * @param counter Counter to set input events for.
    * @param trigger Internal trigger which is affected by the input events.
    * @param events Mask describing the input events.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetCounterInputEvents(U32 counter, U32 trigger,
        COUNTER_EVENT_MASK events) = 0;

    /**
    * Sets the input events for a port which will cause an internal trigger to
    * be set.
    *
    * @param port Port to set input events for.
    * @param trigger Internal trigger which is affected by the input events.
    * @param events Mask describing the input events.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetPortInputEvents(U32 port, U32 trigger,
        PORT_EVENT_MASK events) = 0;

    /**
    * Sets the input events for an internal trigger which will cause another
    * internal trigger to be set.
    *
    * @param causeTrigger Internal trigger to set input events for.
    * @param trigger Internal trigger which is affected by the input events.
    * @param events Mask describing the input events.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetTriggerInputEvents(U32 causeTrigger, U32 trigger,
        TRIGGER_EVENT_MASK events) = 0;
};

/* end namespace tags */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

