/**
* \file external_trigger_action_manager.hpp
*
* \brief Provides functions for handling external trigger actions.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling external trigger actions.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_ACTIONS
*/

#ifndef __EXTERNAL_TRIGGER_ACTION_MANAGER_CPP__
#define __EXTERNAL_TRIGGER_ACTION_MANAGER_CPP__

#include "triggering_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {

/**
* Functions in this class deal with getting and setting external trigger action
* parameters.
*/
class ExternalTriggerActionManager
{
public:
    /**
    * Gets the output actions from an external trigger that are caused by an
    * internal trigger being set.
    *
    * @param extTrigger External trigger to get output actions from.
    * @param trigger Internal trigger which causes the output actions.
    * @param pActions Pointer to a value which will be updated with the actions
    *                 mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetExtTriggerOutputActions(U32 extTrigger, U32 trigger,
        EXT_TRIGGER_ACTION_MASK *pActions) = 0;

    /**
    * Sets the output actions for an external trigger that are caused by an
    * internal trigger being set.
    *
    * @param extTrigger External trigger to set output actions for.
    * @param trigger Internal trigger which causes the output actions.
    * @param actions Mask describing the output actions.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetExtTriggerOutputActions(U32 extTrigger, U32 trigger,
        EXT_TRIGGER_ACTION_MASK actions) = 0;
};

/* end namespace tags */
} /* triggering */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

