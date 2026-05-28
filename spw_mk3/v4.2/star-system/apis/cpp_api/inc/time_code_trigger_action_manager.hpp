/**
* \file time_code_trigger_action_manager.hpp
*
* \brief Provides functions for handling time-code trigger actions.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling time-code trigger actions.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_ACTIONS
*/

#ifndef __TIMECODE_TRIGGER_ACTION_MANAGER_CPP__
#define __TIMECODE_TRIGGER_ACTION_MANAGER_CPP__

#include "triggering_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {

/**
* Functions in this class deal with getting and setting time-code trigger event
* parameters.
*/
class TimeCodeTriggerActionManager
{
public:
    /**
    * Gets the output actions from a time-code engine that are caused by an
    * internal trigger being set.
    *
    * @param timeCode Time-code engine to get output actions from.
    * @param trigger Internal trigger which causes the output actions.
    * @param pActions Pointer to a value which will be updated with the actions
    *                 mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetTimeCodeOutputActions(U32 timeCode, U32 trigger,
        TIME_CODE_ACTION_MASK *pActions) = 0;

    /**
    * Sets the output actions for a time-code engine that are caused by an
    * internal trigger being set.
    *
    * @param timeCode Time-code engine to set output actions for.
    * @param trigger Internal trigger which causes the output actions.
    * @param actions Mask describing the output actions
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetTimeCodeOutputActions(U32 timeCode, U32 trigger,
        TIME_CODE_ACTION_MASK actions) = 0;
};

/* end namespace tags */
} /* triggering */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

