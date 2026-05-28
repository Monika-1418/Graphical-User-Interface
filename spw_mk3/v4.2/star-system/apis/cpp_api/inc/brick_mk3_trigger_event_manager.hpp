/**
* \file brick_mk3_trigger_event_manager.hpp
*
* \brief Provides functions for handling trigger events on the Brick Mk3.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling trigger events on the Brick Mk3.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_EVENTS_BRICK_MK3
*/

#ifndef __BRICK_MK3_TRIGGER_EVENT_MANAGER_CPP__
#define __BRICK_MK3_TRIGGER_EVENT_MANAGER_CPP__

#include "trigger_event_manager.hpp"
#include "external_trigger_event_manager.hpp"
#include "time_code_trigger_event_manager.hpp"

#include "triggering_brick_mk3.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {
                                       namespace brickmk3 {

/**
* Functions in this class deal with getting and setting trigger event
* parameters for the Brick Mk3.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to TriggerFactory::CreateTriggerEventManager() or by 
*       instantiating directly with a STAR-System device ID.
*/
class BrickMk3TriggerEventManager : public TriggerEventManager,
    ExternalTriggerEventManager, TimeCodeTriggerEventManager
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    BrickMk3TriggerEventManager() : TriggerEventManager()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this trigger state is
    *                 associated with and describes.
    */
    explicit BrickMk3TriggerEventManager(STAR_DEVICE_ID deviceID) :
        TriggerEventManager(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~BrickMk3TriggerEventManager()
    {
    }

    int GetCounterInputEvents(U32 counter, U32 trigger,
        COUNTER_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get counter input events */
        return TRIGGER_BRICK_MK3_getCounterInputEvents(idOfOwningDevice,
            counter, trigger, pEvents);
    }

    int GetPortInputEvents(U32 port, U32 trigger,
        PORT_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get port input events */
        return TRIGGER_BRICK_MK3_getPortInputEvents(idOfOwningDevice, port,
            trigger, pEvents);
    }

    int GetTriggerInputEvents(U32 causeTrigger, U32 trigger,
        TRIGGER_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get trigger input events */
        return TRIGGER_BRICK_MK3_getTriggerInputEvents(idOfOwningDevice,
            causeTrigger, trigger, pEvents);
    }

    int SetCounterInputEvents(U32 counter, U32 trigger,
        COUNTER_EVENT_MASK events)
    {
        /* call 'c' API to set counter input events */
        return TRIGGER_BRICK_MK3_setCounterInputEvents(idOfOwningDevice,
            counter, trigger, events);
    }

    int SetPortInputEvents(U32 port, U32 trigger,
        PORT_EVENT_MASK events)
    {
        /* call 'c' API to set port input events */
        return TRIGGER_BRICK_MK3_setPortInputEvents(idOfOwningDevice, port,
            trigger, events);
    }

    int SetTriggerInputEvents(U32 causeTrigger, U32 trigger,
        TRIGGER_EVENT_MASK events)
    {
        /* call 'c' API to set trigger input events */
        return TRIGGER_BRICK_MK3_setTriggerInputEvents(idOfOwningDevice,
            causeTrigger, trigger, events);
    }

    int GetExtTriggerInputEvents(U32 extTrigger, U32 trigger,
        EXT_TRIGGER_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get external trigger input events */
        return TRIGGER_BRICK_MK3_getExtTriggerInputEvents(idOfOwningDevice,
            extTrigger, trigger, pEvents);
    }

    int SetExtTriggerInputEvents(U32 extTrigger, U32 trigger,
        EXT_TRIGGER_EVENT_MASK events)
    {
        /* call 'c' API to set external trigger input events */
        return TRIGGER_BRICK_MK3_setExtTriggerInputEvents(idOfOwningDevice,
            extTrigger, trigger, events);
    }

    int GetTimeCodeInputEvents(U32 timeCode, U32 trigger,
        TIME_CODE_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get time-code input events */
        return TRIGGER_BRICK_MK3_getTimeCodeInputEvents(idOfOwningDevice,
            timeCode, trigger, pEvents);
    }

    int SetTimeCodeInputEvents(U32 timeCode, U32 trigger,
        TIME_CODE_EVENT_MASK events)
    {
        /* call 'c' API to set time-code input events */
        return TRIGGER_BRICK_MK3_setTimeCodeInputEvents(idOfOwningDevice,
            timeCode, trigger, events);
    }
};

/* end namespace tags */
} /* brickmk3 */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

