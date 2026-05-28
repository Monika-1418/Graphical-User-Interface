/**
* \file pcie_trigger_action_manager.hpp
*
* \brief Provides functions for handling trigger actions on the PCIe.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling trigger actions on the PCIe.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_ACTIONS_PCIE
*/

#ifndef __PCIE_TRIGGER_ACTION_MANAGER_CPP__
#define __PCIE_TRIGGER_ACTION_MANAGER_CPP__

#include "trigger_action_manager.hpp"
#include "external_trigger_action_manager.hpp"
#include "time_code_trigger_action_manager.hpp"

#include "triggering_pcie.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {
                                       namespace pcie {

/**
* Functions in this class deal with getting and setting trigger action
* parameters for the PCIe.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to TriggerFactory::CreateTriggerActionManager() or by 
*       instantiating directly with a STAR-System device ID.
*/
class PCIeTriggerActionManager : public TriggerActionManager
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PCIeTriggerActionManager() : TriggerActionManager()
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param deviceID The ID of the device which this trigger state is
    *                 associated with and describes.
    */
    explicit PCIeTriggerActionManager(STAR_DEVICE_ID deviceID) :
        TriggerActionManager(deviceID)
    {
        /* call base class constructor to initialise object - see initialisation
         * list above */
    }

    /**
    * Destructor. Free any resources created or managed by the object.
    */
    virtual ~PCIeTriggerActionManager()
    {
    }

    int GetCounterOutputActions(U32 counter, U32 trigger,
        COUNTER_ACTION_MASK *pActions)
    {
        /* call 'c' API to get counter output actions */
        return TRIGGER_PCIE_IF_getCounterOutputActions(idOfOwningDevice,
            counter, trigger, pActions);
    }

    int GetPortOutputActions(U32 port, U32 trigger, PORT_ACTION_MASK *pActions)
    {
        /* call 'c' API to get port output actions */
        return TRIGGER_PCIE_IF_getPortOutputActions(idOfOwningDevice, port,
            trigger, pActions);
    }

    int SetCounterOutputActions(U32 counter, U32 trigger,
        COUNTER_ACTION_MASK actions)
    {
        /* call 'c' API to set counter output actions */
        return TRIGGER_PCIE_IF_setCounterOutputActions(idOfOwningDevice,
            counter, trigger, actions);
    }

    int SetPortOutputActions(U32 port, U32 trigger, PORT_ACTION_MASK actions)
    {
        /* call 'c' API to set port output actions */
        return TRIGGER_PCIE_IF_setPortOutputActions(idOfOwningDevice, port,
            trigger, actions);
    }
};

/* end namespace tags */
} /* pcie */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

