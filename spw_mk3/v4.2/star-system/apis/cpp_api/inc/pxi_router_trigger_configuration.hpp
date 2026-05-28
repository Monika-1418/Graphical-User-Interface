/**
* \file pxi_router_trigger_configuration.hpp
*
* \brief Provides functions for configuring triggering on a PXI router device.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for configuring triggering on a PXI router device.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_CONF_PXI
*/

#ifndef __PXI_ROUTER_TRIGGER_CONFIGURATION_CPP__
#define __PXI_ROUTER_TRIGGER_CONFIGURATION_CPP__

#include "trigger_configuration.hpp"

#include "triggering_pxi_ro.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {
                                       namespace pxi {

/**
* Functions in this class deal with getting and setting trigger configuration
* parameters on the PXI router.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to TriggerFactory::CreateTriggerConfiguration() or by 
*       instantiating directly with a STAR-System device ID.
*/
class PXIRouterTriggerConfiguration : public TriggerConfiguration
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PXIRouterTriggerConfiguration() : TriggerConfiguration()
    {
    }

    /**
    * Constructor - use given parameters to initialise PXI router trigger
    * configuration.
    *
    * @param deviceID The device ID that the triggering configuration relates
    *                 to.
    */
    explicit PXIRouterTriggerConfiguration(STAR_DEVICE_ID deviceID) :
        TriggerConfiguration(deviceID)
    {
    }

    int EnableCounterAutoReload(U32 counter)
    {
        /* call 'c' API to enable counter auto reload */
        return TRIGGER_PXI_ROUTER_enableCounterAutoReload(idOfOwningDevice,
            counter);
    }

    int GetCounterTriggerCountEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter trigger count enabled */
        return TRIGGER_PXI_ROUTER_getCounterTriggerCountEnabled(
            idOfOwningDevice, counter, pEnabled);
    }

    int DisableCounterStartMode(U32 counter)
    {
        /* call 'c' API to disable counter start mode */
        return TRIGGER_PXI_ROUTER_disableCounterStartMode(idOfOwningDevice,
            counter);
    }

    int ForceCounterReload(U32 counter)
    {
        /* call 'c' API to force counter reload */
        return TRIGGER_PXI_ROUTER_forceCounterReload(idOfOwningDevice, counter);
    }

    int GetCounterReloadValue(U32 counter, U32 *pReloadValue)
    {
        /* call 'c' API to get counter reload value */
        return TRIGGER_PXI_ROUTER_getCounterReloadValue(idOfOwningDevice,
            counter, pReloadValue);
    }

    int GetCounterStartStopModeEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter start stop mode enabled */
        return TRIGGER_PXI_ROUTER_getCounterStartStopModeEnabled(
            idOfOwningDevice, counter, pEnabled);
    }

    int DisableCounterLoadZero(U32 counter)
    {
        /* call 'c' API to disable counter load zero */
        return TRIGGER_PXI_ROUTER_disableCounterLoadZero(idOfOwningDevice,
            counter);
    }

    int EnableCounterStartStopMode(U32 counter)
    {
        /* call 'c' API to enable counter start stop mode */
        return TRIGGER_PXI_ROUTER_enableCounterStartStopMode(idOfOwningDevice,
            counter);
    }

    int DisableCounterTriggerCount(U32 counter)
    {
        /* call 'c' API to disable counter trigger count */
        return TRIGGER_PXI_ROUTER_disableCounterTriggerCount(idOfOwningDevice,
            counter);
    }

    int DisableCounterStartStopMode(U32 counter)
    {
        /* call 'c' API to disable counter start stop mode */
        return TRIGGER_PXI_ROUTER_disableCounterStartStopMode(idOfOwningDevice,
            counter);
    }

    int SetTriggerInputMode(U32 trigger, TRIGGER_INPUT_MODE mode)
    {
        /* call 'c' API to set trigger input mode */
        return TRIGGER_PXI_ROUTER_setTriggerInputMode(idOfOwningDevice,
            trigger, mode);
    }

    int SetCounterReloadValue(U32 counter, U32 reloadValue)
    {
        /* call 'c' API to set counter reload value */
        return TRIGGER_PXI_ROUTER_setCounterReloadValue(idOfOwningDevice,
            counter, reloadValue);
    }

    int GetTriggerAndMask(U32 trigger, TRIGGER_TYPE type, U32 *pMask)
    {
        /* call 'c' API to get trigger and mask */
        return TRIGGER_PXI_ROUTER_getTriggerAndMask(idOfOwningDevice, trigger,
            type, pMask);
    }

    int EnableCounterTriggerCount(U32 counter)
    {
        /* call 'c' API to enable counter trigger count */
        return TRIGGER_PXI_ROUTER_enableCounterTriggerCount(idOfOwningDevice,
            counter);
    }

    int EnableCounterLoadZero(U32 counter)
    {
        /* call 'c' API to enable counter load zero */
        return TRIGGER_PXI_ROUTER_enableCounterLoadZero(idOfOwningDevice,
            counter);
    }

    int GetCounterLoadZeroEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter load zero enabled */
        return TRIGGER_PXI_ROUTER_getCounterLoadZeroEnabled(idOfOwningDevice,
            counter, pEnabled);
    }

    int EnableCounterStartMode(U32 counter)
    {
        /* call 'c' API to enable counter start mode */
        return TRIGGER_PXI_ROUTER_enableCounterStartMode(idOfOwningDevice,
            counter);
    }

    int GetCounterAutoReloadEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter auto reload enabled */
        return TRIGGER_PXI_ROUTER_getCounterAutoReloadEnabled(idOfOwningDevice,
            counter, pEnabled);
    }

    int ForceTrigger(U32 trigger)
    {
        /* call 'c' API to force trigger */
        return TRIGGER_PXI_ROUTER_forceTrigger(idOfOwningDevice, trigger);
    }

    int EnableTrigger(U32 trigger)
    {
        /* call 'c' API to enable trigger */
        return TRIGGER_PXI_ROUTER_enableTrigger(idOfOwningDevice, trigger);
    }

    int SetTriggerAndMask(U32 trigger, TRIGGER_TYPE type, U32 mask)
    {
        /* call 'c' API to set trigger and mask */
        return TRIGGER_PXI_ROUTER_setTriggerAndMask(idOfOwningDevice, trigger,
            type, mask);
    }

    int GetTriggerInvertMask(U32 trigger, TRIGGER_TYPE type,
        U32 *pMask)
    {
        /* call 'c' API to get trigger invert mask */
        return TRIGGER_PXI_ROUTER_getTriggerInvertMask(idOfOwningDevice,
            trigger, type, pMask);
    }

    int DisableCounterAutoReload(U32 counter)
    {
        /* call 'c' API to disable counter auto reload */
        return TRIGGER_PXI_ROUTER_disableCounterAutoReload(idOfOwningDevice,
            counter);
    }

    int GetCounterStartModeEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter start mode enabled */
        return TRIGGER_PXI_ROUTER_getCounterStartModeEnabled(idOfOwningDevice,
            counter, pEnabled);
    }

    int GetCounterValue(U32 counter, U32 *pValue)
    {
        /* call 'c' API to get counter value */
        return TRIGGER_PXI_ROUTER_getCounterValue(idOfOwningDevice, counter,
            pValue);
    }

    int GetTriggerEnabled(U32 trigger, U32 *pEnabled)
    {
        /* call 'c' API to get trigger enabled */
        return TRIGGER_PXI_ROUTER_getTriggerEnabled(idOfOwningDevice, trigger,
            pEnabled);
    }

    int GetTriggerInputMode(U32 trigger, TRIGGER_INPUT_MODE *pMode)
    {
        /* call 'c' API to get trigger input mode */
        return TRIGGER_PXI_ROUTER_getTriggerInputMode(idOfOwningDevice,
            trigger, pMode);
    }

    int DisableTrigger(U32 trigger)
    {
        /* call 'c' API to disable trigger */
        return TRIGGER_PXI_ROUTER_disableTrigger(idOfOwningDevice, trigger);
    }

    int SetTriggerInvertMask(U32 trigger, TRIGGER_TYPE type, U32 mask)
    {
        /* call 'c' API to set trigger invert mask */
        return TRIGGER_PXI_ROUTER_setTriggerInvertMask(idOfOwningDevice,
            trigger, type, mask);
    }

    int EnablePortTransmitPacketMode(U32 port)
    {
        /* call 'c' API to enable port transmit packet mode */
        return TRIGGER_PXI_ROUTER_enablePortTransmitPacketMode(
            idOfOwningDevice, port);
    }

    int DisablePortTransmitPacketMode(U32 port)
    {
        /* call 'c' API to disable port transmit packet mode */
        return TRIGGER_PXI_ROUTER_disablePortTransmitPacketMode(
            idOfOwningDevice, port);
    }

    int GetPortTransmitPacketModeEnabled(U32 port, U32 *pEnabled)
    {
        /* call 'c' API to get port transmit packet mode enabled */
        return TRIGGER_PXI_ROUTER_getPortTransmitPacketModeEnabled(
            idOfOwningDevice, port, pEnabled);
    }
};

/* end namespace tags */
} /* pxi */ } /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

