/**
* \file pxi_router_trigger_action_manager.hpp
*
* \brief Provides functions for handling trigger actions on the PXI router.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling trigger actions on the PXI router.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_ACTIONS_PXI
*/

#ifndef __PXI_ROUTER_TRIGGER_ACTION_MANAGER_CPP__
#define __PXI_ROUTER_TRIGGER_ACTION_MANAGER_CPP__

#include "trigger_action_manager.hpp"
#include "time_code_trigger_action_manager.hpp"

#include "triggering_pxi_ro.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {
                                       namespace pxi {

/**
* Functions in this class deal with getting and setting trigger action
* parameters for the PXI router.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to TriggerFactory::CreateTriggerActionManager() or by 
*       instantiating directly with a STAR-System device ID.
*/
class PXIRouterTriggerActionManager : public TriggerActionManager,
    TimeCodeTriggerActionManager
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PXIRouterTriggerActionManager() : TriggerActionManager()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this trigger state is
    *                 associated with and describes.
    */
    explicit PXIRouterTriggerActionManager(STAR_DEVICE_ID deviceID) :
        TriggerActionManager(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~PXIRouterTriggerActionManager()
    {
    }

    int GetCounterOutputActions(U32 counter, U32 trigger,
        COUNTER_ACTION_MASK *pActions)
    {
        /* call 'c' API to get counter output actions */
        return TRIGGER_PXI_ROUTER_getCounterOutputActions(idOfOwningDevice,
            counter, trigger, pActions);
    }

    int GetPortOutputActions(U32 port, U32 trigger, PORT_ACTION_MASK *pActions)
    {
        /* call 'c' API to get port output actions */
        return TRIGGER_PXI_ROUTER_getPortOutputActions(idOfOwningDevice, port,
            trigger, pActions);
    }

    int SetCounterOutputActions(U32 counter, U32 trigger,
        COUNTER_ACTION_MASK actions)
    {
        /* call 'c' API to set counter output actions */
        return TRIGGER_PXI_ROUTER_setCounterOutputActions(idOfOwningDevice,
            counter, trigger, actions);
    }

    int SetPortOutputActions(U32 port, U32 trigger, PORT_ACTION_MASK actions)
    {
        /* call 'c' API to set port output actions */
        return TRIGGER_PXI_ROUTER_setPortOutputActions(idOfOwningDevice, port,
            trigger, actions);
    }

    int GetTimeCodeOutputActions(U32 timeCode, U32 trigger,
        TIME_CODE_ACTION_MASK *pActions)
    {
        /* call 'c' API to get time-code output actions */
        return TRIGGER_PXI_ROUTER_getTimeCodeOutputActions(idOfOwningDevice,
            timeCode, trigger, pActions);
    }

    int SetTimeCodeOutputActions(U32 timeCode, U32 trigger,
        TIME_CODE_ACTION_MASK actions)
    {
        /* call 'c' API to set time-code output actions */
        return TRIGGER_PXI_ROUTER_setTimeCodeOutputActions(idOfOwningDevice,
            timeCode, trigger, actions);
    }
};

/* end namespace tags */
} /* pxi */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

