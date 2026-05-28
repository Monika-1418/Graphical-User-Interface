/**
* \file time_code_trigger_event_manager.hpp
*
* \brief Provides functions for handling time-code trigger events.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling time-code trigger events.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_EVENTS
*/

#ifndef __TIMECODE_TRIGGER_EVENT_MANAGER_CPP__
#define __TIMECODE_TRIGGER_EVENT_MANAGER_CPP__

#include "triggering_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {

/**
* Functions in this class deal with getting and setting time-code trigger event
* parameters.
*/
class TimeCodeTriggerEventManager
{
public:
    /**
    * Gets the input events from a time-code engine which will cause an
    * internal trigger to be set.
    *
    * @param timeCode Time-code engine to get input events from.
    * @param trigger Internal trigger which is affected by the input events.
    * @param pEvents Pointer to a value which will be updated with the events
    *                mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetTimeCodeInputEvents(U32 timeCode, U32 trigger,
        TIME_CODE_EVENT_MASK *pEvents) = 0;

    /**
    * Sets the input events for a time-code engine which will cause an internal
    * trigger to be set.
    *
    * @param timeCode Time-code engine to set input events for.
    * @param trigger Internal trigger which is affected by the input events.
    * @param events Mask describing the input events.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetTimeCodeInputEvents(U32 timeCode, U32 trigger,
        TIME_CODE_EVENT_MASK events) = 0;
};

/* end namespace tags */
} /* triggering */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

