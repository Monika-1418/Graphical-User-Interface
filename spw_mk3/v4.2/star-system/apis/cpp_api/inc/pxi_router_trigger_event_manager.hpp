/**
* \file pxi_router_trigger_event_manager.hpp
*
* \brief Provides functions for handling trigger events on the PXI router.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling trigger events on the PXI router.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_EVENTS_PXI
*/

#ifndef __PXI_ROUTER_TRIGGER_EVENT_MANAGER_CPP__
#define __PXI_ROUTER_TRIGGER_EVENT_MANAGER_CPP__

#include "trigger_event_manager.hpp"
#include "external_trigger_event_manager.hpp"
#include "time_code_trigger_event_manager.hpp"

#include "triggering_pxi_ro.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {
                                       namespace pxi {

/**
* Functions in this class deal with getting and setting trigger event
* parameters for the PXI router.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to TriggerFactory::CreateTriggerEventManager() or by 
*       instantiating directly with a STAR-System device ID.
*/
class PXIRouterTriggerEventManager : public TriggerEventManager,
    TimeCodeTriggerEventManager
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PXIRouterTriggerEventManager() : TriggerEventManager()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this trigger state is
    *                 associated with and describes.
    */
    explicit PXIRouterTriggerEventManager(STAR_DEVICE_ID deviceID) :
        TriggerEventManager(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~PXIRouterTriggerEventManager()
    {
    }

    int GetCounterInputEvents(U32 counter, U32 trigger,
        COUNTER_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get counter input events */
        return TRIGGER_PXI_ROUTER_getCounterInputEvents(idOfOwningDevice,
            counter, trigger, pEvents);
    }

    int GetPortInputEvents(U32 port, U32 trigger,
        PORT_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get port input events */
        return TRIGGER_PXI_ROUTER_getPortInputEvents(idOfOwningDevice, port,
            trigger, pEvents);
    }

    int GetTriggerInputEvents(U32 causeTrigger, U32 trigger,
        TRIGGER_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get trigger input events */
        return TRIGGER_PXI_ROUTER_getTriggerInputEvents(idOfOwningDevice,
            causeTrigger, trigger, pEvents);
    }

    int SetCounterInputEvents(U32 counter, U32 trigger,
        COUNTER_EVENT_MASK events)
    {
        /* call 'c' API to set counter input events */
        return TRIGGER_PXI_ROUTER_setCounterInputEvents(idOfOwningDevice,
            counter, trigger, events);
    }

    int SetPortInputEvents(U32 port, U32 trigger,
        PORT_EVENT_MASK events)
    {
        /* call 'c' API to set port input events */
        return TRIGGER_PXI_ROUTER_setPortInputEvents(idOfOwningDevice, port,
            trigger, events);
    }

    int SetTriggerInputEvents(U32 causeTrigger, U32 trigger,
        TRIGGER_EVENT_MASK events)
    {
        /* call 'c' API to set trigger input events */
        return TRIGGER_PXI_ROUTER_setTriggerInputEvents(idOfOwningDevice,
            causeTrigger, trigger, events);
    }

    int GetTimeCodeInputEvents(U32 timeCode, U32 trigger,
        TIME_CODE_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get time-code input events */
        return TRIGGER_PXI_ROUTER_getTimeCodeInputEvents(idOfOwningDevice,
            timeCode, trigger, pEvents);
    }

    int SetTimeCodeInputEvents(U32 timeCode, U32 trigger,
        TIME_CODE_EVENT_MASK events)
    {
        /* call 'c' API to set time-code input events */
        return TRIGGER_PXI_ROUTER_setTimeCodeInputEvents(idOfOwningDevice,
            timeCode, trigger, events);
    }
};

/* end namespace tags */
} /* pxi */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

