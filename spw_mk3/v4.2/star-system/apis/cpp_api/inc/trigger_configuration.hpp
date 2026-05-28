/**
* \file trigger_configuration.hpp
*
* \brief Provides functions for configuring triggering on a device.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for configuring triggering on a device.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_CONF
*/

#ifndef __TRIGGER_CONFIGURATION_CPP__
#define __TRIGGER_CONFIGURATION_CPP__

#include "star-api.h"
#include "triggering_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {

/**
* Functions in this class deal with getting and setting general trigger
* configuration parameters.
*/
class TriggerConfiguration
{
protected:
    /** the ID of the device which the triggering configuration relates to */
    STAR_DEVICE_ID idOfOwningDevice;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    TriggerConfiguration()
    {
    }

    /**
    * Destructor. Free any resources created.
    */
    virtual ~TriggerConfiguration()
    {
        /* no resources to free at present */
    }

    /**
    * Constructor - use given parameters to initialise trigger configuration.
    *
    * @param deviceID The device ID that the triggering configuration relates
    *                 to.
    */
    explicit TriggerConfiguration(STAR_DEVICE_ID deviceID) :
        idOfOwningDevice(deviceID)
    {
    }

    /**
    * Copy constructor - to allow copies of TriggerConfiguration objects to be 
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    TriggerConfiguration(const TriggerConfiguration&
        triggerConfigurationStateToCopyFrom) : idOfOwningDevice(0)
    {
        /* copy required contents of object being copied into this object */
        idOfOwningDevice =
            triggerConfigurationStateToCopyFrom.idOfOwningDevice;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * TriggerConfiguration objects to have their contents assigned to each 
    * other.
    *
    */
    TriggerConfiguration& operator=(const TriggerConfiguration&
        triggerConfigurationStateToCopyFrom)
    {
        /* check for self-assignment, no point in copying one's self */
        if(this == &triggerConfigurationStateToCopyFrom)
        {
            return *this;
        }

        /* copy required contents of object being copied into this object */
        idOfOwningDevice =
            triggerConfigurationStateToCopyFrom.idOfOwningDevice;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Enables auto reload for a specific counter.
    * When enabled, the counter will automatically reload with its reload value
    * when the counter reaches zero.
    *
    * @param counter Counter to enable auto reload on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int EnableCounterAutoReload(U32 counter) = 0;

    /**
    * Gets whether trigger count is enabled for a specific counter.
    *
    * @param counter Counter to check.
    * @param pEnabled User supplied value that will be updated to 1 if trigger
    *                 count is enabled, else 0.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetCounterTriggerCountEnabled(U32 counter, U32 *pEnabled) = 0;

    /**
    * Disables start mode for a specific counter.
    * When disabled, the counter will not wait for a start action to occur
    * before any counting or reloading can take place.
    *
    * @param counter Counter to disable start mode on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int DisableCounterStartMode(U32 counter) = 0;

    /**
    * Force the reload of a specific counter.
    *
    * @param counter Counter to reload.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int ForceCounterReload(U32 counter) = 0;

    /**
    * Gets the reload value for a specific counter.
    * The reload value is the value that will be loaded into the counter when a
    * reload occurs.
    *
    * @param counter Counter to get the reload value for.
    * @param pReloadValue Pointer to value to be updated with the reload value.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetCounterReloadValue(U32 counter, U32 *pReloadValue) = 0;

    /**
    * Gets whether start stop mode is enabled for a specific counter.
    *
    * @param counter Counter to check.
    * @param pEnabled User supplied value that will be updated to 1 if start
    *                 stop mode is enabled, else 0.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetCounterStartStopModeEnabled(U32 counter, U32 *pEnabled) = 0;

    /**
    * Disables load zero for a specific counter.
    * When disabled, reload triggered events occur even when the counter is not
    * zero.
    *
    * @param counter Counter to disable load zero on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int DisableCounterLoadZero(U32 counter) = 0;

    /**
    * Enables start stop mode for a specific counter.
    * When enabled, the counter will wait for a start action to occur before
    * any counting or reloading can take place. The counter will continue to
    * count and reload until a stop action occurs.
    *
    * @param counter Counter to enable start stop mode on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int EnableCounterStartStopMode(U32 counter) = 0;

    /**
    * Disables trigger count for a specific counter.
    * When disabled, the counter will be decremented every clock cycle.
    *
    * @param counter Counter to disable trigger count on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int DisableCounterTriggerCount(U32 counter) = 0;

    /**
    * Disables start stop mode for a specific counter.
    * When disabled, the counter will not wait for a start action to occur
    * before any counting or reloading can take place.
    *
    * @param counter Counter to disable start stop mode on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int DisableCounterStartStopMode(U32 counter) = 0;

    /**
    * Sets the input mode for a specific internal trigger.
    * The trigger input mode can be either TRIGGER_INPUT_MODE_AND or
    * TRIGGER_INPUT_MODE_OR. If set to TRIGGER_INPUT_MODE_AND, the trigger
    * occurs when all source input events occur. If set to
    * TRIGGER_INPUT_MODE_OR, the trigger occurs when any source input events
    * occur.
    *
    * @param trigger Internal trigger to set the input mode for.
    * @param mode The input mode.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetTriggerInputMode(U32 trigger, TRIGGER_INPUT_MODE mode) = 0;

    /**
    * Sets the reload value for a specific counter.
    * The reload value is the value that will be loaded into the counter when a
    * reload occurs.
    *
    * @param counter Counter to set the reload value for.
    * @param reloadValue Reload value.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetCounterReloadValue(U32 counter, U32 reloadValue) = 0;

    /**
    * Gets the AND/OR mask for a trigger type for a specific internal trigger.
    *
    * @param trigger Internal trigger to get the AND/OR mask for a trigger type
    *                for.
    * @param type Trigger type.
    * @param pMask Pointer to value to be updated with the AND/OR mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetTriggerAndMask(U32 trigger, TRIGGER_TYPE type,
        U32 *pMask) = 0;

    /**
    * Enables trigger count for a specific counter.
    * When enabled, the counter will only be decremented when a count action
    * occurs.
    *
    * @param counter Counter to enable trigger count on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int EnableCounterTriggerCount(U32 counter) = 0;

    /**
    * Enables load zero for a specific counter.
    * When enabled, reload triggered events only occur when the counter is zero.
    *
    * @param counter Counter to enable load zero on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int EnableCounterLoadZero(U32 counter) = 0;

    /**
    * Gets whether load zero is enabled for a specific counter.
    *
    * @param counter Counter to check.
    * @param pEnabled User supplied value that will be updated to 1 if load 
    *                 zero is enabled, else 0.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetCounterLoadZeroEnabled(U32 counter, U32 *pEnabled) = 0;

    /**
    * Enables start mode for a specific counter.
    * When enabled, the counter will wait for a start action to occur before
    * any counting or reloading can take place. When the counter reaches zero,
    * the counter may be reloaded but no counting or reloading can take place
    * until the next start action occurs.
    *
    * @param counter Counter to enable start mode on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int EnableCounterStartMode(U32 counter) = 0;

    /**
    * Gets whether auto reload is enabled for a specific counter.
    *
    * @param counter Counter to check.
    * @param pEnabled User supplied value that will be updated to 1 if auto
    *                 reload is enabled, else 0.
    *
    * @return 1 if the operation was successful, else 0
    */
    virtual int GetCounterAutoReloadEnabled(U32 counter, U32 *pEnabled) = 0;

    /**
    * Force the triggering of a specific internal trigger.
    *
    * @param trigger Internal trigger to trigger.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int ForceTrigger(U32 trigger) = 0;

    /**
    * Enables a specific internal trigger.
    * When enabled, the trigger can be set by its input events and cause output
    * actions to occur.
    *
    * @param trigger Internal trigger to enable.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int EnableTrigger(U32 trigger) = 0;

    /**
    * Sets the AND/OR mask for a trigger type for a specific internal trigger.
    *
    * @param trigger Internal trigger to set AND/OR mask for a trigger type on.
    * @param type Trigger type.
    * @param mask AND/OR mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetTriggerAndMask(U32 trigger, TRIGGER_TYPE type, U32 mask) = 0;

    /**
    * Gets the invert mask for a trigger type for a specific internal trigger.
    * The invert mask for a trigger type determines if a source of input events
    * should have its signal inverted before reaching the internal trigger.
    *
    * @param trigger Internal trigger to get the invert mask for a trigger type
    *                for.
    * @param type Trigger type.
    * @param pMask Pointer to value to be updated with the invert mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetTriggerInvertMask(U32 trigger, TRIGGER_TYPE type,
        U32 *pMask) = 0;

    /**
    * Disables auto reload for a specific counter.
    * When disabled, the counter will not automatically reload with its reload
    * value when the counter reaches zero.
    *
    * @param counter Counter to disable auto reload on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int DisableCounterAutoReload(U32 counter) = 0;

    /**
    * Gets whether start mode is enabled for a specific counter.
    *
    * @param counter Counter to check.
    * @param pEnabled User supplied value that will be updated to 1 if start
    *                 mode is enabled, else 0.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetCounterStartModeEnabled(U32 counter, U32 *pEnabled) = 0;

    /**
    * Gets the counter value for a specific counter.
    *
    * @param counter Counter to get the value for.
    * @param pValue Pointer to value to be updated with the counter value.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetCounterValue(U32 counter, U32 *pValue) = 0;

    /**
    * Gets whether a trigger is enabled.
    *
    * @param trigger Internal trigger to check.
    * @param pEnabled User supplied value that will be updated to 1 if the
    *                 trigger is enabled, else 0.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetTriggerEnabled(U32 trigger, U32 *pEnabled) = 0;

    /**
    * Gets the input mode for a specific internal trigger.
    * The trigger input mode can be either TRIGGER_INPUT_MODE_AND or
    * TRIGGER_INPUT_MODE_OR. If set to TRIGGER_INPUT_MODE_AND, the trigger
    * occurs when all of its input events occur. If set to
    * TRIGGER_INPUT_MODE_OR, the trigger occurs when any of its input events
    * occur.
    *
    * @param trigger Internal trigger to get trigger input mode for.
    * @param pMode Pointer to value to be updated with the input mode.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetTriggerInputMode(U32 trigger, TRIGGER_INPUT_MODE *pMode) = 0;

    /**
    * Disables a specific internal trigger.
    * When disabled, the trigger is not set by its input events and does not
    * cause output actions to occur.
    *
    * @param trigger Internal trigger to disable.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int DisableTrigger(U32 trigger) = 0;

    /**
    * Sets the invert mask for a trigger type for a specific internal trigger.
    * The invert mask for a trigger type determines if a source of input events
    * should have its signal inverted before reaching the internal trigger.
    *
    * @param trigger Internal trigger to set invert mask for a trigger type on.
    * @param type Trigger type.
    * @param mask Invert mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetTriggerInvertMask(U32 trigger, TRIGGER_TYPE type,
        U32 mask) = 0;

    /**
    * Enables packet transmit mode for a specific port.
    * When enabled, packets are only transmitted when a transmit packet action
    * occurs.
    *
    * @param port Port to enable packet transmit mode on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int EnablePortTransmitPacketMode(U32 port) = 0;

    /**
    * Disables packet transmit mode for a specific port.
    * When disabled, packets do not wait for a transmit packet action to occur
    * before transmission.
    *
    * @param port Port to disable packet transmit mode on.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int DisablePortTransmitPacketMode(U32 port) = 0;

    /**
    * Gets whether packet transmit mode is enabled for a specific port.
    *
    * @param port Port to check.
    * @param pEnabled User supplied value that will be updated to 1 if packet
    *                 transmit mode is enabled, else 0.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetPortTransmitPacketModeEnabled(U32 port, U32 *pEnabled) = 0;
};

/* end namespace tags */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

