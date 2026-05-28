/**
* \file brick_mk3_trigger_configuration.hpp
*
* \brief Provides functions for configuring triggering on a Brick Mk3 device.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for configuring triggering on a Brick Mk3 device.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_CONF_BRICK_MK3
*/

#ifndef __BRICK_MK3_TRIGGER_CONFIGURATION_CPP__
#define __BRICK_MK3_TRIGGER_CONFIGURATION_CPP__

#include "trigger_configuration.hpp"
#include "external_trigger_configuration.hpp"

#include "triggering_brick_mk3.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {
                                       namespace brickmk3 {

/**
* Functions in this class deal with getting and setting trigger configuration
* parameters on the Brick Mk3.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to TriggerFactory::CreateTriggerConfiguration() or by 
*       instantiating directly with a STAR-System device ID.
*/
class BrickMk3TriggerConfiguration : public TriggerConfiguration,
    ExternalTriggerConfiguration
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    BrickMk3TriggerConfiguration() : TriggerConfiguration()
    {
    }

    /**
    * Constructor - use given parameters to initialise Brick Mk3 trigger
    * configuration.
    *
    * @param deviceID The device ID that the triggering configuration relates
    *                 to.
    */
    explicit BrickMk3TriggerConfiguration(STAR_DEVICE_ID deviceID) :
        TriggerConfiguration(deviceID)
    {
    }

    int EnableCounterAutoReload(U32 counter)
    {
        /* call 'c' API to enable counter auto reload */
        return TRIGGER_BRICK_MK3_enableCounterAutoReload(idOfOwningDevice,
            counter);
    }

    int GetCounterTriggerCountEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter trigger count enabled */
        return TRIGGER_BRICK_MK3_getCounterTriggerCountEnabled(
            idOfOwningDevice, counter, pEnabled);
    }

    int DisableCounterStartMode(U32 counter)
    {
        /* call 'c' API to disable counter start mode */
        return TRIGGER_BRICK_MK3_disableCounterStartMode(idOfOwningDevice,
            counter);
    }

    int ForceCounterReload(U32 counter)
    {
        /* call 'c' API to force counter reload */
        return TRIGGER_BRICK_MK3_forceCounterReload(idOfOwningDevice, counter);
    }

    int GetCounterReloadValue(U32 counter, U32 *pReloadValue)
    {
        /* call 'c' API to get counter reload value */
        return TRIGGER_BRICK_MK3_getCounterReloadValue(idOfOwningDevice,
            counter, pReloadValue);
    }

    int GetCounterStartStopModeEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter start stop mode enabled */
        return TRIGGER_BRICK_MK3_getCounterStartStopModeEnabled(
            idOfOwningDevice, counter, pEnabled);
    }

    int DisableCounterLoadZero(U32 counter)
    {
        /* call 'c' API to disable counter load zero */
        return TRIGGER_BRICK_MK3_disableCounterLoadZero(idOfOwningDevice,
            counter);
    }

    int EnableCounterStartStopMode(U32 counter)
    {
        /* call 'c' API to enable counter start stop mode */
        return TRIGGER_BRICK_MK3_enableCounterStartStopMode(idOfOwningDevice,
            counter);
    }

    int DisableCounterTriggerCount(U32 counter)
    {
        /* call 'c' API to disable counter trigger count */
        return TRIGGER_BRICK_MK3_disableCounterTriggerCount(idOfOwningDevice,
            counter);
    }

    int DisableCounterStartStopMode(U32 counter)
    {
        /* call 'c' API to disable counter start stop mode */
        return TRIGGER_BRICK_MK3_disableCounterStartStopMode(idOfOwningDevice,
            counter);
    }

    int SetTriggerInputMode(U32 trigger, TRIGGER_INPUT_MODE mode)
    {
        /* call 'c' API to set trigger input mode */
        return TRIGGER_BRICK_MK3_setTriggerInputMode(idOfOwningDevice, trigger,
            mode);
    }

    int SetCounterReloadValue(U32 counter, U32 reloadValue)
    {
        /* call 'c' API to set counter reload value */
        return TRIGGER_BRICK_MK3_setCounterReloadValue(idOfOwningDevice,
            counter, reloadValue);
    }

    int GetTriggerAndMask(U32 trigger, TRIGGER_TYPE type, U32 *pMask)
    {
        /* call 'c' API to get trigger and mask */
        return TRIGGER_BRICK_MK3_getTriggerAndMask(idOfOwningDevice, trigger,
            type, pMask);
    }

    int EnableCounterTriggerCount(U32 counter)
    {
        /* call 'c' API to enable counter trigger count */
        return TRIGGER_BRICK_MK3_enableCounterTriggerCount(idOfOwningDevice,
            counter);
    }

    int EnableCounterLoadZero(U32 counter)
    {
        /* call 'c' API to enable counter load zero */
        return TRIGGER_BRICK_MK3_enableCounterLoadZero(idOfOwningDevice,
            counter);
    }

    int GetCounterLoadZeroEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter load zero enabled */
        return TRIGGER_BRICK_MK3_getCounterLoadZeroEnabled(idOfOwningDevice, 
            counter, pEnabled);
    }

    int EnableCounterStartMode(U32 counter)
    {
        /* call 'c' API to enable counter start mode */
        return TRIGGER_BRICK_MK3_enableCounterStartMode(idOfOwningDevice,
            counter);
    }

    int GetCounterAutoReloadEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter auto reload enabled */
        return TRIGGER_BRICK_MK3_getCounterAutoReloadEnabled(idOfOwningDevice,
            counter, pEnabled);
    }

    int ForceTrigger(U32 trigger)
    {
        /* call 'c' API to force trigger */
        return TRIGGER_BRICK_MK3_forceTrigger(idOfOwningDevice, trigger);
    }

    int EnableTrigger(U32 trigger)
    {
        /* call 'c' API to enable trigger */
        return TRIGGER_BRICK_MK3_enableTrigger(idOfOwningDevice, trigger);
    }

    int SetTriggerAndMask(U32 trigger, TRIGGER_TYPE type, U32 mask)
    {
        /* call 'c' API to set trigger and mask */
        return TRIGGER_BRICK_MK3_setTriggerAndMask(idOfOwningDevice, trigger,
            type, mask);
    }

    int GetTriggerInvertMask(U32 trigger, TRIGGER_TYPE type,
        U32 *pMask)
    {
        /* call 'c' API to get trigger invert mask */
        return TRIGGER_BRICK_MK3_getTriggerInvertMask(idOfOwningDevice,
            trigger, type, pMask);
    }

    int DisableCounterAutoReload(U32 counter)
    {
        /* call 'c' API to disable counter auto reload */
        return TRIGGER_BRICK_MK3_disableCounterAutoReload(idOfOwningDevice,
            counter);
    }

    int GetCounterStartModeEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter start mode enabled */
        return TRIGGER_BRICK_MK3_getCounterStartModeEnabled(idOfOwningDevice,
            counter, pEnabled);
    }

    int GetCounterValue(U32 counter, U32 *pValue)
    {
        /* call 'c' API to get counter value */
        return TRIGGER_BRICK_MK3_getCounterValue(idOfOwningDevice, counter,
            pValue);
    }

    int GetTriggerEnabled(U32 trigger, U32 *pEnabled)
    {
        /* call 'c' API to get trigger enabled */
        return TRIGGER_BRICK_MK3_getTriggerEnabled(idOfOwningDevice, trigger,
            pEnabled);
    }

    int GetTriggerInputMode(U32 trigger, TRIGGER_INPUT_MODE *pMode)
    {
        /* call 'c' API to get trigger input mode */
        return TRIGGER_BRICK_MK3_getTriggerInputMode(idOfOwningDevice, trigger,
            pMode);
    }

    int DisableTrigger(U32 trigger)
    {
        /* call 'c' API to disable trigger */
        return TRIGGER_BRICK_MK3_disableTrigger(idOfOwningDevice, trigger);
    }

    int SetTriggerInvertMask(U32 trigger, TRIGGER_TYPE type, U32 mask)
    {
        /* call 'c' API to set trigger invert mask */
        return TRIGGER_BRICK_MK3_setTriggerInvertMask(idOfOwningDevice,
            trigger, type, mask);
    }

    int EnablePortTransmitPacketMode(U32 port)
    {
        /* call 'c' API to enable port transmit packet mode */
        return TRIGGER_BRICK_MK3_enablePortTransmitPacketMode(idOfOwningDevice,
            port);
    }

    int DisablePortTransmitPacketMode(U32 port)
    {
        /* call 'c' API to disable port transmit packet mode */
        return TRIGGER_BRICK_MK3_disablePortTransmitPacketMode(
            idOfOwningDevice, port);
    }

    int GetPortTransmitPacketModeEnabled(U32 port, U32 *pEnabled)
    {
        /* call 'c' API to get port transmit packet mode enabled */
        return TRIGGER_BRICK_MK3_getPortTransmitPacketModeEnabled(
            idOfOwningDevice, port, pEnabled);
    }

    int DisableExtTriggerOutput(U32 extTrigger)
    {
        /* call 'c' API to disable external trigger output */
        return TRIGGER_BRICK_MK3_disableExtTriggerOutput(idOfOwningDevice,
            extTrigger);
    }

    int EnableExtTriggerOutput(U32 extTrigger)
    {
        /* call 'c' API to enable external trigger output */
        return TRIGGER_BRICK_MK3_enableExtTriggerOutput(idOfOwningDevice,
            extTrigger);
    }

    int DisableExtTriggerInvert(U32 extTrigger)
    {
        /* call 'c' API to disable external trigger invert */
        return TRIGGER_BRICK_MK3_disableExtTriggerInvert(idOfOwningDevice,
            extTrigger);
    }

    int EnableExtTriggerEdgeDetectMode(U32 extTrigger)
    {
        /* call 'c' API to enable external trigger edge detect mode */
        return TRIGGER_BRICK_MK3_enableExtTriggerEdgeDetectMode(
            idOfOwningDevice, extTrigger);
    }

    int EnableExtTriggerInvert(U32 extTrigger)
    {
        /* call 'c' API to enable external trigger invert */
        return TRIGGER_BRICK_MK3_enableExtTriggerInvert(idOfOwningDevice,
            extTrigger);
    }

    int SetExtTriggerExtend(U32 extTrigger, U32 extend)
    {
        /* call 'c' API to set external trigger extend */
        return TRIGGER_BRICK_MK3_setExtTriggerExtend(idOfOwningDevice,
            extTrigger, extend);
    }

    int GetExtTriggerExtend(U32 extTrigger, U32 *pExtend)
    {
        /* call 'c' API to get external trigger extend */
        return TRIGGER_BRICK_MK3_getExtTriggerExtend(idOfOwningDevice,
            extTrigger, pExtend);
    }

    int DisableExtTriggerEdgeDetectMode(U32 extTrigger)
    {
        /* call 'c' API to disable external trigger edge detect mode */
        return TRIGGER_BRICK_MK3_disableExtTriggerEdgeDetectMode(
            idOfOwningDevice, extTrigger);
    }

    int GetExtTriggerInvertEnabled(U32 extTrigger, U32 *pEnabled)
    {
        /* call 'c' API to get external trigger invert enabled */
        return TRIGGER_BRICK_MK3_getExtTriggerInvertEnabled(idOfOwningDevice,
            extTrigger, pEnabled);
    }

    int GetExtTriggerOutputEnabled(U32 extTrigger, U32 *pEnabled)
    {
        /* call 'c' API to get external trigger output enabled */
        return TRIGGER_BRICK_MK3_getExtTriggerOutputEnabled(idOfOwningDevice,
            extTrigger, pEnabled);
    }

    int GetExtTriggerEdgeDetectModeEnabled(U32 extTrigger,
        U32 *pEnabled)
    {
        /* call 'c' API to get external trigger edge detect mode enabled */
        return TRIGGER_BRICK_MK3_getExtTriggerEdgeDetectModeEnabled(
            idOfOwningDevice, extTrigger, pEnabled);
    }
};

/* end namespace tags */
} /* brickmk3 */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

