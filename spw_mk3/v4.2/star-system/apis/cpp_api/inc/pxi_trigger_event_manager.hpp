/**
* \file pxi_trigger_event_manager.hpp
*
* \brief Provides functions for handling trigger events on the PXI interface.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling trigger events on the PXI interface.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_EVENTS_PXI
*/

#ifndef __PXI_TRIGGER_EVENT_MANAGER_CPP__
#define __PXI_TRIGGER_EVENT_MANAGER_CPP__

#include "trigger_event_manager.hpp"
#include "external_trigger_event_manager.hpp"
#include "time_code_trigger_event_manager.hpp"

#include "triggering_pxi_if.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {
                                       namespace pxi {

/**
* Functions in this class deal with getting and setting trigger event
* parameters for the PXI.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to TriggerFactory::CreateTriggerEventManager() or by 
*       instantiating directly with a STAR-System device ID.
*/
class PXITriggerEventManager : public TriggerEventManager,
    ExternalTriggerEventManager, TimeCodeTriggerEventManager
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PXITriggerEventManager() : TriggerEventManager()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this trigger state is
    *                 associated with and describes.
    */
    explicit PXITriggerEventManager(STAR_DEVICE_ID deviceID) :
        TriggerEventManager(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~PXITriggerEventManager()
    {
    }

    int GetCounterInputEvents(U32 counter, U32 trigger,
        COUNTER_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get counter input events */
        return TRIGGER_PXI_IF_getCounterInputEvents(idOfOwningDevice,
            counter, trigger, pEvents);
    }

    int GetPortInputEvents(U32 port, U32 trigger,
        PORT_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get port input events */
        return TRIGGER_PXI_IF_getPortInputEvents(idOfOwningDevice, port,
            trigger, pEvents);
    }

    int GetTriggerInputEvents(U32 causeTrigger, U32 trigger,
        TRIGGER_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get trigger input events */
        return TRIGGER_PXI_IF_getTriggerInputEvents(idOfOwningDevice,
            causeTrigger, trigger, pEvents);
    }

    int SetCounterInputEvents(U32 counter, U32 trigger,
        COUNTER_EVENT_MASK events)
    {
        /* call 'c' API to set counter input events */
        return TRIGGER_PXI_IF_setCounterInputEvents(idOfOwningDevice,
            counter, trigger, events);
    }

    int SetPortInputEvents(U32 port, U32 trigger,
        PORT_EVENT_MASK events)
    {
        /* call 'c' API to set port input events */
        return TRIGGER_PXI_IF_setPortInputEvents(idOfOwningDevice, port,
            trigger, events);
    }

    int SetTriggerInputEvents(U32 causeTrigger, U32 trigger,
        TRIGGER_EVENT_MASK events)
    {
        /* call 'c' API to set trigger input events */
        return TRIGGER_PXI_IF_setTriggerInputEvents(idOfOwningDevice,
            causeTrigger, trigger, events);
    }

    int GetExtTriggerInputEvents(U32 extTrigger, U32 trigger,
        EXT_TRIGGER_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get external trigger input events */
        return TRIGGER_PXI_IF_getExtTriggerInputEvents(idOfOwningDevice,
            extTrigger, trigger, pEvents);
    }

    int SetExtTriggerInputEvents(U32 extTrigger, U32 trigger,
        EXT_TRIGGER_EVENT_MASK events)
    {
        /* call 'c' API to set external trigger input events */
        return TRIGGER_PXI_IF_setExtTriggerInputEvents(idOfOwningDevice,
            extTrigger, trigger, events);
    }

    int GetTimeCodeInputEvents(U32 timeCode, U32 trigger,
        TIME_CODE_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get time-code input events */
        return TRIGGER_PXI_IF_getTimeCodeInputEvents(idOfOwningDevice,
            timeCode, trigger, pEvents);
    }

    int SetTimeCodeInputEvents(U32 timeCode, U32 trigger,
        TIME_CODE_EVENT_MASK events)
    {
        /* call 'c' API to set time-code input events */
        return TRIGGER_PXI_IF_setTimeCodeInputEvents(idOfOwningDevice,
            timeCode, trigger, events);
    }
};

/* end namespace tags */
} /* pxi */
} /* triggering */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

