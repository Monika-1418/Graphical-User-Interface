/**
* \file triggering_pcie.h
*
* \brief Functions used to configure and control the triggering capabilities of the STAR-Dundee
*        PCIe Interface devices. The PCIe Interface devices have the following triggering resources
*        available: 6 internal counters (indexed from 0-5), 3 SpaceWire ports (indexed from 1-3)
*        and 6 internal triggers (indexed from 0-5).
*
* \author STAR-Dundee Ltd.\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Copyright &copy; 2016 STAR-Dundee Ltd.
*/

#ifndef TRIGGERING_PCIE_H
#define TRIGGERING_PCIE_H

#ifdef __cplusplus
extern "C" {
#endif


#include <star-api.h>

#include "triggering_types.h"

/**
* \ingroup triggeringEvents
*
* Sets the input events for a counter which will cause an internal trigger to be set.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to set input events for.
* @param trigger            Internal trigger which is affected by the input events.
* @param events             Mask describing the input events.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_setCounterInputEvents(STAR_DEVICE_ID deviceId,
    U32 counter, U32 trigger, COUNTER_EVENT_MASK events);

/**
* \ingroup triggeringEvents
*
* Sets the input events for a port which will cause an internal trigger to be set.
*
* @param deviceId           Device containing the port.
* @param port               Port to set input events for.
* @param trigger            Internal trigger which is affected by the input events.
* @param events             Mask describing the input events.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_setPortInputEvents(STAR_DEVICE_ID deviceId,
    U32 port, U32 trigger, PORT_EVENT_MASK events);

/**
* \ingroup triggeringEvents
*
* Sets the input events for an internal trigger which will cause another internal trigger to be set.
*
* @param deviceId           Device containing the internal trigger.
* @param causeTrigger       Internal trigger to set input events for.
* @param trigger            Internal trigger which is affected by the input events.
* @param events             Mask describing the input events.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_setTriggerInputEvents(STAR_DEVICE_ID deviceId,
    U32 causeTrigger, U32 trigger, TRIGGER_EVENT_MASK events);

/**
* \ingroup triggeringEvents
*
* Gets the input events from a counter which will cause an internal trigger to be set.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to get input events from.
* @param trigger            Internal trigger which is affected by the input events.
* @param pEvents            Pointer to a value which will be updated with the events mask.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getCounterInputEvents(STAR_DEVICE_ID deviceId,
    U32 counter, U32 trigger, COUNTER_EVENT_MASK *pEvents);

/**
* \ingroup triggeringEvents
*
* Gets the input events from a port which will cause an internal trigger to be set.
*
* @param deviceId           Device containing the port.
* @param port               Port to get input events from.
* @param trigger            Internal trigger which is affected by the input events.
* @param pEvents            Pointer to a value which will be updated with the events mask.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getPortInputEvents(STAR_DEVICE_ID deviceId,
    U32 port, U32 trigger, PORT_EVENT_MASK *pEvents);

/**
* \ingroup triggeringEvents
*
* Gets the input events for an internal trigger which will cause another internal trigger to be set.
*
* @param deviceId           Device containing the internal trigger.
* @param causeTrigger       Internal trigger to get input events from.
* @param trigger            Internal trigger which is affected by the input events.
* @param pEvents            Pointer to a value which will be updated with the events mask.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getTriggerInputEvents(STAR_DEVICE_ID deviceId,
    U32 causeTrigger, U32 trigger, TRIGGER_EVENT_MASK *pEvents);

/**
* \ingroup triggeringActions
*
* Sets the output actions for a counter that are caused by an internal trigger being set.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to set output actions for.
* @param trigger            Internal trigger which causes the output actions.
* @param actions            Mask describing the output actions
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_setCounterOutputActions(STAR_DEVICE_ID deviceId,
    U32 counter, U32 trigger, COUNTER_ACTION_MASK actions);

/**
* \ingroup triggeringActions
*
* Sets the output actions for a port that are caused by an internal trigger being set.
*
* @param deviceId           Device containing the port.
* @param port               Port to set output actions for.
* @param trigger            Internal trigger which causes the output actions.
* @param actions            Mask describing the output actions
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_setPortOutputActions(STAR_DEVICE_ID deviceId,
    U32 port, U32 trigger, PORT_ACTION_MASK actions);

/**
* \ingroup triggeringActions
*
* Gets the output actions from a counter that are caused by an internal trigger being set.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to get output actions from.
* @param trigger            Internal trigger which causes the output actions.
* @param pActions           Pointer to a value which will be updated with the actions mask.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getCounterOutputActions(STAR_DEVICE_ID deviceId,
    U32 counter, U32 trigger, COUNTER_ACTION_MASK *pActions);

/**
* \ingroup triggeringActions
*
* Gets the output actions from a port that are caused by an internal trigger being set.
*
* @param deviceId           Device containing the port.
* @param port               Port to get output actions from.
* @param trigger            Internal trigger which causes the output actions.
* @param pActions           Pointer to a value which will be updated with the actions mask.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getPortOutputActions(STAR_DEVICE_ID deviceId,
    U32 port, U32 trigger, PORT_ACTION_MASK *pActions);

/**
* \ingroup triggeringConf
*
* Enables auto reload for a specific counter.
* When enabled, the counter will automatically reload with its reload value when the
* counter reaches zero.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to enable auto reload on.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_enableCounterAutoReload(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Disables auto reload for a specific counter.
* When disabled, the counter will not automatically reload with its reload value when
* the counter reaches zero.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to disable auto reload on.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_disableCounterAutoReload(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Gets whether auto reload is enabled for a specific counter.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to check.
* @param pEnabled           User supplied value that will be updated to 1
*                           if auto reload is enabled, else 0.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getCounterAutoReloadEnabled(STAR_DEVICE_ID deviceId,
    U32 counter, U32 *pEnabled);

/**
* \ingroup triggeringConf
*
* Enables trigger count for a specific counter.
* When enabled, the counter will only be decremented when a count action occurs.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to enable trigger count on.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_enableCounterTriggerCount(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Disables trigger count for a specific counter.
* When disabled, the counter will be decremented every clock cycle.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to disable trigger count on.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_disableCounterTriggerCount(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Gets whether trigger count is enabled for a specific counter.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to check.
* @param pEnabled           User supplied value that will be updated to 1
*                           if trigger count is enabled, else 0.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getCounterTriggerCountEnabled(STAR_DEVICE_ID deviceId,
    U32 counter, U32 *pEnabled);

/**
* \ingroup triggeringConf
*
* Enables start mode for a specific counter.
* When enabled, the counter will wait for a counter start action to occur before any counting or
* reloading can take place. When the counter reaches zero, the counter may be reloaded but
* no counting or reloading can take place until the next start action occurs.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to enable start mode on.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_enableCounterStartMode(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Disables start mode for a specific counter.
* When disabled, the counter will not wait for a start action to occur before any counting or
* reloading can take place.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to disable start mode on.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_disableCounterStartMode(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Gets whether start mode is enabled for a specific counter.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to check.
* @param pEnabled           User supplied value that will be updated to 1
*                           if start mode is enabled, else 0.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getCounterStartModeEnabled(STAR_DEVICE_ID deviceId,
    U32 counter, U32 *pEnabled);

/**
* \ingroup triggeringConf
*
* Enables start stop mode for a specific counter.
* When enabled, the counter will wait for a start action to occur before any counting or
* reloading can take place. The counter will continue to count and reload until a stop action
* occurs.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to enable start stop mode on.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_enableCounterStartStopMode(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Disables start stop mode for a specific counter.
* When disabled, the counter will not wait for a start action to occur before any counting or
* reloading can take place.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to disable start stop mode on.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_disableCounterStartStopMode(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Gets whether start stop mode is enabled for a specific counter.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to check.
* @param pEnabled           User supplied value that will be updated to 1
*                           if start stop mode is enabled, else 0.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getCounterStartStopModeEnabled(STAR_DEVICE_ID deviceId,
    U32 counter, U32 *pEnabled);

/**
* \ingroup triggeringConf
*
* Enables load zero for a specific counter.
* When enabled, reload triggered events only occur when the counter is zero.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to enable load zero on.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_enableCounterLoadZero(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Disables load zero for a specific counter.
* When disabled, reload triggered events occur even when the counter is not zero.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to disable load zero on.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_disableCounterLoadZero(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Gets whether load zero is enabled for a specific counter.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to check.
* @param pEnabled           User supplied value that will be updated to 1
*                           if load zero is enabled, else 0.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getCounterLoadZeroEnabled(STAR_DEVICE_ID deviceId,
    U32 counter, U32 *pEnabled);

/**
* \ingroup triggeringConf
*
* Sets the reload value for a specific counter.
* The reload value is the value that will be loaded into the counter when a reload occurs.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to set the reload value for.
* @param reloadValue        Reload value.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_setCounterReloadValue(STAR_DEVICE_ID deviceId,
    U32 counter, U32 reloadValue);

/**
* \ingroup triggeringConf
*
* Gets the reload value for a specific counter.
* The reload value is the value that will be loaded into the counter when a reload occurs.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to get the reload value for.
* @param pReloadValue       Pointer to value to be updated with the reload value.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getCounterReloadValue(STAR_DEVICE_ID deviceId,
    U32 counter, U32 *pReloadValue);

/**
* \ingroup triggeringConf
*
* Force the reload of a specific counter.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to reload.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_forceCounterReload(STAR_DEVICE_ID deviceId,
    U32 counter);

/**
* \ingroup triggeringConf
*
* Gets the counter value for a specific counter.
*
* @param deviceId           Device containing the counter.
* @param counter            Counter to get the value for.
* @param pValue             Pointer to value to be updated with the counter value.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getCounterValue(STAR_DEVICE_ID deviceId,
    U32 counter, U32 *pValue);

/**
* \ingroup triggeringConf
*
* Sets the input mode for a specific internal trigger.
* The trigger input mode can be either TRIGGER_INPUT_MODE_AND or TRIGGER_INPUT_MODE_OR. If set to
* TRIGGER_INPUT_MODE_AND, the trigger occurs when all source input events occur. If set to
* TRIGGER_INPUT_MODE_OR, the trigger occurs when any source input events occur.
*
* @param deviceId           Device containing the internal trigger.
* @param trigger            Internal trigger to set the input mode for.
* @param mode               The input mode.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_setTriggerInputMode(STAR_DEVICE_ID deviceId,
    U32 trigger, TRIGGER_INPUT_MODE mode);

/**
* \ingroup triggeringConf
*
* Gets the input mode for a specific internal trigger.
* The trigger input mode can be either TRIGGER_INPUT_MODE_AND or TRIGGER_INPUT_MODE_OR. If set to
* TRIGGER_INPUT_MODE_AND, the trigger occurs when all of its input events occur. If set to
* TRIGGER_INPUT_MODE_OR, the trigger occurs when any of its input events occur.
*
* @param deviceId           Device containing the internal trigger.
* @param trigger            Internal trigger to get trigger input mode for.
* @param pMode              Pointer to value to be updated with the input mode.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getTriggerInputMode(STAR_DEVICE_ID deviceId,
    U32 trigger, TRIGGER_INPUT_MODE *pMode);

/**
* \ingroup triggeringConf
*
* Enables a specific internal trigger.
* When enabled, the trigger can be set by its input events and cause output actions to occur.
*
* @param deviceId           Device containing the internal trigger.
* @param trigger            Internal trigger to enable.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_enableTrigger(STAR_DEVICE_ID deviceId,
    U32 trigger);

/**
* \ingroup triggeringConf
*
* Disables a specific internal trigger.
* When disabled, the trigger is not set by its input events and does not cause output actions to
* occur.
*
* @param deviceId           Device containing the internal trigger.
* @param trigger            Internal trigger to disable.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_disableTrigger(STAR_DEVICE_ID deviceId,
    U32 trigger);

/**
* \ingroup triggeringConf
*
* Gets whether a trigger is enabled.
*
* @param deviceId           Device containing the internal trigger.
* @param trigger            Internal trigger to check.
* @param pEnabled           User supplied value that will be updated to 1
*                           if the trigger is enabled, else 0.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getTriggerEnabled(STAR_DEVICE_ID deviceId,
    U32 trigger, U32 *pEnabled);

/**
* \ingroup triggeringConf
*
* Force the triggering of a specific internal trigger.
*
* @param deviceId           Device containing the internal trigger.
* @param trigger            Internal trigger to trigger.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_forceTrigger(STAR_DEVICE_ID deviceId,
    U32 trigger);

/**
* \ingroup triggeringConf
*
* Sets the invert mask for a trigger type for a specific internal trigger.
* The invert mask for a trigger type determines if a source of input events should have its signal
* inverted before reaching the internal trigger.
*
* @param deviceId           Device containing the internal trigger.
* @param trigger            Internal trigger to set invert mask for a trigger type on.
* @param type               Trigger type.
* @param mask               Invert mask.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_setTriggerInvertMask(STAR_DEVICE_ID deviceId,
    U32 trigger, TRIGGER_TYPE type, U32 mask);

/**
* \ingroup triggeringConf
*
* Gets the invert mask for a trigger type for a specific internal trigger.
* The invert mask for a trigger type determines if a source of input events should have its signal
* inverted before reaching the internal trigger.
*
* @param deviceId           Device containing the internal trigger.
* @param trigger            Internal trigger to get the invert mask for a trigger type for.
* @param type               Trigger type.
* @param pMask              Pointer to value to be updated with the invert mask.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getTriggerInvertMask(STAR_DEVICE_ID deviceId,
    U32 trigger, TRIGGER_TYPE type, U32 *pMask);

/**
* \ingroup triggeringConf
*
* Sets the AND/OR mask for a trigger type for a specific internal trigger.
*
* @param deviceId           Device containing the internal trigger.
* @param trigger            Internal trigger to set AND/OR mask for a trigger type on.
* @param type               Trigger type.
* @param mask               AND/OR mask.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_setTriggerAndMask(STAR_DEVICE_ID deviceId,
    U32 trigger, TRIGGER_TYPE type, U32 mask);

/**
* \ingroup triggeringConf
*
* Gets the AND/OR mask for a trigger type for a specific internal trigger.
*
* @param deviceId           Device containing the internal trigger.
* @param trigger            Internal trigger to get the AND/OR mask for a trigger type for.
* @param type               Trigger type.
* @param pMask              Pointer to value to be updated with the AND/OR mask.
*
* @return  1 if the operation was successful, else 0
*
* \versionAdded \ref changes_v3_1
*
* \devicesSupported \ref PCIe
*/
int STAR_API_CC TRIGGER_PCIE_IF_getTriggerAndMask(STAR_DEVICE_ID deviceId,
    U32 trigger, TRIGGER_TYPE type, U32 *pMask);


#ifdef __cplusplus
}
#endif

#endif // TRIGGERING_PCIE_H
