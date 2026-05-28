/**
* \file brick_mk3_trigger_action_manager.hpp
*
* \brief Provides functions for handling trigger actions on the Brick Mk3.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling trigger actions on the Brick Mk3.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_ACTIONS_BRICK_MK3
*/

#ifndef __BRICK_MK3_TRIGGER_ACTION_MANAGER_CPP__
#define __BRICK_MK3_TRIGGER_ACTION_MANAGER_CPP__

#include "trigger_action_manager.hpp"
#include "external_trigger_action_manager.hpp"
#include "time_code_trigger_action_manager.hpp"

#include "triggering_brick_mk3.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {
                                       namespace brickmk3 {

/**
* Functions in this class deal with getting and setting trigger action
* parameters for the Brick Mk3.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to TriggerFactory::CreateTriggerActionManager() or by 
*       instantiating directly with a STAR-System device ID.
*/
class BrickMk3TriggerActionManager : public TriggerActionManager,
    ExternalTriggerActionManager, TimeCodeTriggerActionManager
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    BrickMk3TriggerActionManager() : TriggerActionManager()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this trigger state is
    *                 associated with and describes.
    */
    explicit BrickMk3TriggerActionManager(STAR_DEVICE_ID deviceID) :
        TriggerActionManager(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~BrickMk3TriggerActionManager()
    {
    }

    int GetCounterOutputActions(U32 counter, U32 trigger,
        COUNTER_ACTION_MASK *pActions)
    {
        /* call 'c' API to get counter output actions */
        return TRIGGER_BRICK_MK3_getCounterOutputActions(idOfOwningDevice,
            counter, trigger, pActions);
    }

    int GetPortOutputActions(U32 port, U32 trigger, PORT_ACTION_MASK *pActions)
    {
        /* call 'c' API to get port output actions */
        return TRIGGER_BRICK_MK3_getPortOutputActions(idOfOwningDevice, port,
            trigger, pActions);
    }

    int SetCounterOutputActions(U32 counter, U32 trigger,
        COUNTER_ACTION_MASK actions)
    {
        /* call 'c' API to set counter output actions */
        return TRIGGER_BRICK_MK3_setCounterOutputActions(idOfOwningDevice,
            counter, trigger, actions);
    }

    int SetPortOutputActions(U32 port, U32 trigger, PORT_ACTION_MASK actions)
    {
        /* call 'c' API to set port output actions */
        return TRIGGER_BRICK_MK3_setPortOutputActions(idOfOwningDevice, port,
            trigger, actions);
    }

    int GetExtTriggerOutputActions(U32 extTrigger, U32 trigger,
        EXT_TRIGGER_ACTION_MASK *pActions)
    {
        /* call 'c' API to get external trigger output actions */
        return TRIGGER_BRICK_MK3_getExtTriggerOutputActions(idOfOwningDevice,
            extTrigger, trigger, pActions);
    }

    int SetExtTriggerOutputActions(U32 extTrigger, U32 trigger,
        EXT_TRIGGER_ACTION_MASK actions)
    {
        /* call 'c' API to set external trigger output actions */
        return TRIGGER_BRICK_MK3_setExtTriggerOutputActions(idOfOwningDevice,
            extTrigger, trigger, actions);
    }

    int GetTimeCodeOutputActions(U32 timeCode, U32 trigger,
        TIME_CODE_ACTION_MASK *pActions)
    {
        /* call 'c' API to get time-code output actions */
        return TRIGGER_BRICK_MK3_getTimeCodeOutputActions(idOfOwningDevice,
            timeCode, trigger, pActions);
    }

    int SetTimeCodeOutputActions(U32 timeCode, U32 trigger,
        TIME_CODE_ACTION_MASK actions)
    {
        /* call 'c' API to set time-code output actions */
        return TRIGGER_BRICK_MK3_setTimeCodeOutputActions(idOfOwningDevice,
            timeCode, trigger, actions);
    }
};

/* end namespace tags */
} /* brickmk3 */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

