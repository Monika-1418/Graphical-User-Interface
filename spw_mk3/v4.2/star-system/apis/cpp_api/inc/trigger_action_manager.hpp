/**
* \file trigger_action_manager.hpp
*
* \brief Provides functions for handling basic trigger actions.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling basic trigger actions.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_ACTIONS
*/

#ifndef __TRIGGER_ACTION_MANAGER_CPP__
#define __TRIGGER_ACTION_MANAGER_CPP__

#include "star-api.h"
#include "triggering_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {

/**
* Functions in this class deal with getting and setting general trigger action
* parameters.
*/
class TriggerActionManager
{
protected:
    /** the ID of the device which the RMAP target configuration relates to */
    STAR_DEVICE_ID idOfOwningDevice;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    TriggerActionManager()
    {
    }

    /**
    * Destructor. Free any resources created.
    */
    virtual ~TriggerActionManager()
    {
        /* no resources to free at present */
    }

    /**
    * Constructor - use given parameters to initialise trigger action manager.
    *
    * @param deviceID The device ID that the RMAP target configuration relates
    *                 to.
    */
    explicit TriggerActionManager(STAR_DEVICE_ID deviceID) :
        idOfOwningDevice(deviceID)
    {
    }

    /**
    * Copy constructor - to allow copies of TriggerActionManager objects to be 
    * made when being passed by value in functions or returned by value from 
    * functions.
    *
    */
    TriggerActionManager(const TriggerActionManager&
        triggerActionManagerStateToCopyFrom) : idOfOwningDevice(0)
    {
        /* copy required contents of object being copied into this object */
        idOfOwningDevice =
            triggerActionManagerStateToCopyFrom.idOfOwningDevice;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * TriggerActionManager objects to have their contents assigned to each
    * other.
    *
    */
    TriggerActionManager& operator=(const TriggerActionManager&
        triggerActionManagerStateToCopyFrom)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &triggerActionManagerStateToCopyFrom)
        {
            return *this;
        }

        /* copy required contents of object being copied into this object */
        idOfOwningDevice =
            triggerActionManagerStateToCopyFrom.idOfOwningDevice;

        /* return a reference to this object, to allow chaining of the
        * assignment operator.
        */
        return *this;
    }

    /**
    * Gets the output actions from a counter that are caused by an internal
    * trigger being set.
    *
    * @param counter Counter to get output actions from.
    * @param trigger Internal trigger which causes the output actions.
    * @param pActions Pointer to a value which will be updated with the actions
    *                 mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetCounterOutputActions(U32 counter, U32 trigger,
        COUNTER_ACTION_MASK *pActions) = 0;

    /**
    * Gets the output actions from a port that are caused by an internal
    * trigger being set.
    *
    * @param port Port to get output actions from.
    * @param trigger Internal trigger which causes the output actions.
    * @param pActions Pointer to a value which will be updated with the actions
    *                 mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetPortOutputActions(U32 port, U32 trigger,
        PORT_ACTION_MASK *pActions) = 0;

    /**
    * Sets the output actions for a counter that are caused by an internal
    * trigger being set.
    *
    * @param counter Counter to set output actions for.
    * @param trigger Internal trigger which causes the output actions.
    * @param actions Mask describing the output actions.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetCounterOutputActions(U32 counter, U32 trigger,
        COUNTER_ACTION_MASK actions) = 0;

    /**
    * Sets the output actions for a port that are caused by an internal trigger
    * being set.
    *
    * @param port Port to set output actions for.
    * @param trigger Internal trigger which causes the output actions.
    * @param actions Mask describing the output actions
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetPortOutputActions(U32 port, U32 trigger,
        PORT_ACTION_MASK actions) = 0;
};

/* end namespace tags */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

