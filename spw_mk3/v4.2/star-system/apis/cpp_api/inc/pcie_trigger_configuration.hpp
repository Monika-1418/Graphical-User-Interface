/**
* \file pcie_trigger_configuration.hpp
*
* \brief Provides functions for configuring triggering on a PCIe device.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for configuring triggering on a PCIe device.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER_CONF_PCIE
*/

#ifndef __PCIE_TRIGGER_CONFIGURATION_CPP__
#define __PCIE_TRIGGER_CONFIGURATION_CPP__

#include "trigger_configuration.hpp"
#include "cpp_api_macros.hpp"
#include "not_supported_exception.hpp"

#include "triggering_pcie.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {
                                       namespace pcie {

/**
* Functions in this class deal with getting and setting trigger configuration
* parameters on the PCIe.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to TriggerFactory::CreateTriggerConfiguration() or by 
*       instantiating directly with a STAR-System device ID.
*/
class PCIeTriggerConfiguration : public TriggerConfiguration
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PCIeTriggerConfiguration() : TriggerConfiguration()
    {
    }

    /**
    * Constructor - use given parameters to initialise PCIe trigger
    * configuration.
    *
    * @param deviceID The device ID that the triggering configuration relates
    *                 to.
    */
    explicit PCIeTriggerConfiguration(STAR_DEVICE_ID deviceID) :
        TriggerConfiguration(deviceID)
    {
    }

    int EnableCounterAutoReload(U32 counter)
    {
        /* call 'c' API to enable counter auto reload */
        return TRIGGER_PCIE_IF_enableCounterAutoReload(idOfOwningDevice,
            counter);
    }

    int GetCounterTriggerCountEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter trigger count enabled */
        return TRIGGER_PCIE_IF_getCounterTriggerCountEnabled(idOfOwningDevice,
            counter, pEnabled);
    }

    int DisableCounterStartMode(U32 counter)
    {
        /* call 'c' API to disable counter start mode */
        return TRIGGER_PCIE_IF_disableCounterStartMode(idOfOwningDevice,
            counter);
    }

    int ForceCounterReload(U32 counter)
    {
        /* call 'c' API to force counter reload */
        return TRIGGER_PCIE_IF_forceCounterReload(idOfOwningDevice, counter);
    }

    int GetCounterReloadValue(U32 counter, U32 *pReloadValue)
    {
        /* call 'c' API to get counter reload value */
        return TRIGGER_PCIE_IF_getCounterReloadValue(idOfOwningDevice, counter,
            pReloadValue);
    }

    int GetCounterStartStopModeEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter start stop mode enabled */
        return TRIGGER_PCIE_IF_getCounterStartStopModeEnabled(idOfOwningDevice,
            counter, pEnabled);
    }

    int DisableCounterLoadZero(U32 counter)
    {
        /* call 'c' API to disable counter load zero */
        return TRIGGER_PCIE_IF_disableCounterLoadZero(idOfOwningDevice,
            counter);
    }

    int EnableCounterStartStopMode(U32 counter)
    {
        /* call 'c' API to enable counter start stop mode */
        return TRIGGER_PCIE_IF_enableCounterStartStopMode(idOfOwningDevice,
            counter);
    }

    int DisableCounterTriggerCount(U32 counter)
    {
        /* call 'c' API to disable counter trigger count */
        return TRIGGER_PCIE_IF_disableCounterTriggerCount(idOfOwningDevice,
            counter);
    }

    int DisableCounterStartStopMode(U32 counter)
    {
        /* call 'c' API to disable counter start stop mode */
        return TRIGGER_PCIE_IF_disableCounterStartStopMode(idOfOwningDevice,
            counter);
    }

    int SetTriggerInputMode(U32 trigger, TRIGGER_INPUT_MODE mode)
    {
        /* call 'c' API to set trigger input mode */
        return TRIGGER_PCIE_IF_setTriggerInputMode(idOfOwningDevice, trigger,
            mode);
    }

    int SetCounterReloadValue(U32 counter, U32 reloadValue)
    {
        /* call 'c' API to set counter reload value */
        return TRIGGER_PCIE_IF_setCounterReloadValue(idOfOwningDevice, counter,
            reloadValue);
    }

    int GetTriggerAndMask(U32 trigger, TRIGGER_TYPE type, U32 *pMask)
    {
        /* call 'c' API to get trigger and mask */
        return TRIGGER_PCIE_IF_getTriggerAndMask(idOfOwningDevice, trigger,
            type, pMask);
    }

    int EnableCounterTriggerCount(U32 counter)
    {
        /* call 'c' API to enable counter trigger count */
        return TRIGGER_PCIE_IF_enableCounterTriggerCount(idOfOwningDevice,
            counter);
    }

    int EnableCounterLoadZero(U32 counter)
    {
        /* call 'c' API to enable counter load zero */
        return TRIGGER_PCIE_IF_enableCounterLoadZero(idOfOwningDevice, counter);
    }

    int GetCounterLoadZeroEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter load zero enabled */
        return TRIGGER_PCIE_IF_getCounterLoadZeroEnabled(idOfOwningDevice,
            counter, pEnabled);
    }

    int EnableCounterStartMode(U32 counter)
    {
        /* call 'c' API to enable counter start mode */
        return TRIGGER_PCIE_IF_enableCounterStartMode(idOfOwningDevice,
            counter);
    }

    int GetCounterAutoReloadEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter auto reload enabled */
        return TRIGGER_PCIE_IF_getCounterAutoReloadEnabled(idOfOwningDevice,
            counter, pEnabled);
    }

    int ForceTrigger(U32 trigger)
    {
        /* call 'c' API to force trigger */
        return TRIGGER_PCIE_IF_forceTrigger(idOfOwningDevice, trigger);
    }

    int EnableTrigger(U32 trigger)
    {
        /* call 'c' API to enable trigger */
        return TRIGGER_PCIE_IF_enableTrigger(idOfOwningDevice, trigger);
    }

    int SetTriggerAndMask(U32 trigger, TRIGGER_TYPE type, U32 mask)
    {
        /* call 'c' API to set trigger and mask */
        return TRIGGER_PCIE_IF_setTriggerAndMask(idOfOwningDevice, trigger,
            type, mask);
    }

    int GetTriggerInvertMask(U32 trigger, TRIGGER_TYPE type,
        U32 *pMask)
    {
        /* call 'c' API to get trigger invert mask */
        return TRIGGER_PCIE_IF_getTriggerInvertMask(idOfOwningDevice, trigger,
            type, pMask);
    }

    int DisableCounterAutoReload(U32 counter)
    {
        /* call 'c' API to disable counter auto reload */
        return TRIGGER_PCIE_IF_disableCounterAutoReload(idOfOwningDevice,
            counter);
    }

    int GetCounterStartModeEnabled(U32 counter, U32 *pEnabled)
    {
        /* call 'c' API to get counter start mode enabled */
        return TRIGGER_PCIE_IF_getCounterStartModeEnabled(idOfOwningDevice,
            counter, pEnabled);
    }

    int GetCounterValue(U32 counter, U32 *pValue)
    {
        /* call 'c' API to get counter value */
        return TRIGGER_PCIE_IF_getCounterValue(idOfOwningDevice, counter,
            pValue);
    }

    int GetTriggerEnabled(U32 trigger, U32 *pEnabled)
    {
        /* call 'c' API to get trigger enabled */
        return TRIGGER_PCIE_IF_getTriggerEnabled(idOfOwningDevice, trigger,
            pEnabled);
    }

    int GetTriggerInputMode(U32 trigger, TRIGGER_INPUT_MODE *pMode)
    {
        /* call 'c' API to get trigger input mode */
        return TRIGGER_PCIE_IF_getTriggerInputMode(idOfOwningDevice, trigger,
            pMode);
    }

    int DisableTrigger(U32 trigger)
    {
        /* call 'c' API to disable trigger */
        return TRIGGER_PCIE_IF_disableTrigger(idOfOwningDevice, trigger);
    }

    int SetTriggerInvertMask(U32 trigger, TRIGGER_TYPE type, U32 mask)
    {
        /* call 'c' API to set trigger invert mask */
        return TRIGGER_PCIE_IF_setTriggerInvertMask(idOfOwningDevice, trigger,
            type, mask);
    }

    /**
    * This function is not not supported on the PCIe.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int EnablePortTransmitPacketMode(U32 port)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(port);

        /* throw exception - function not supported for PCIe */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCIe.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int DisablePortTransmitPacketMode(U32 port)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(port);

        /* throw exception - function not supported for PCIe */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }

    /**
    * This function is not not supported on the PCIe.
    *
    * \throws stardundee::com::starsystem::general::NotSupportedException
    *
    */
    int GetPortTransmitPacketModeEnabled(U32 port, U32 *pEnabled)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(port);
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(pEnabled);

        /* throw exception - function not supported for PCIe */
        throw stardundee::com::starsystem::general::NotSupportedException();
    }
};

/* end namespace tags */
} /* pcie */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

