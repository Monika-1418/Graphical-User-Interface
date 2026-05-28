/**
* \file pcie_trigger_event_manager.hpp
*
* \brief Provides functions for handling trigger events on the PCIe.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling trigger events on the PCIe.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_EVENTS_PCIE
*/

#ifndef __PCIE_TRIGGER_EVENT_MANAGER_CPP__
#define __PCIE_TRIGGER_EVENT_MANAGER_CPP__

#include "trigger_event_manager.hpp"

#include "triggering_pcie.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {
                                       namespace pcie {

/**
* Functions in this class deal with getting and setting trigger event
* parameters for the PCIe.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to TriggerFactory::CreateTriggerEventManager() or by 
*       instantiating directly with a STAR-System device ID.
*/
class PCIeTriggerEventManager : public TriggerEventManager
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PCIeTriggerEventManager() : TriggerEventManager()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this trigger state is
    *                 associated with and describes.
    */
    explicit PCIeTriggerEventManager(STAR_DEVICE_ID deviceID) :
        TriggerEventManager(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~PCIeTriggerEventManager()
    {
    }

    int GetCounterInputEvents(U32 counter, U32 trigger,
        COUNTER_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get counter input events */
        return TRIGGER_PCIE_IF_getCounterInputEvents(idOfOwningDevice, counter,
            trigger, pEvents);
    }

    int GetPortInputEvents(U32 port, U32 trigger,
        PORT_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get port input events */
        return TRIGGER_PCIE_IF_getPortInputEvents(idOfOwningDevice, port,
            trigger, pEvents);
    }

    int GetTriggerInputEvents(U32 causeTrigger, U32 trigger,
        TRIGGER_EVENT_MASK *pEvents)
    {
        /* call 'c' API to get trigger input events */
        return TRIGGER_PCIE_IF_getTriggerInputEvents(idOfOwningDevice,
            causeTrigger, trigger, pEvents);
    }

    int SetCounterInputEvents(U32 counter, U32 trigger,
        COUNTER_EVENT_MASK events)
    {
        /* call 'c' API to set counter input events */
        return TRIGGER_PCIE_IF_setCounterInputEvents(idOfOwningDevice, counter,
            trigger, events);
    }

    int SetPortInputEvents(U32 port, U32 trigger,
        PORT_EVENT_MASK events)
    {
        /* call 'c' API to set port input events */
        return TRIGGER_PCIE_IF_setPortInputEvents(idOfOwningDevice, port,
            trigger, events);
    }

    int SetTriggerInputEvents(U32 causeTrigger, U32 trigger,
        TRIGGER_EVENT_MASK events)
    {
        /* call 'c' API to set trigger input events */
        return TRIGGER_PCIE_IF_setTriggerInputEvents(idOfOwningDevice,
            causeTrigger, trigger, events);
    }
};

/* end namespace tags */
} /* pcie */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

