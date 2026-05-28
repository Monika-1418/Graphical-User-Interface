/**
* \file trigger_factory.hpp
*
* \brief Provides convenience functions for creating triggering related objects.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides convenience functions for creating triggering related objects.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_TRIGGER
*/

#ifndef __TRIGGER_FACTORY_CPP__
#define __TRIGGER_FACTORY_CPP__

#include "device.hpp"
#include "brick_mk3_trigger_action_manager.hpp"
#include "pcie_trigger_action_manager.hpp"
#include "pxi_trigger_action_manager.hpp"
#include "pxi_router_trigger_action_manager.hpp"
#include "brick_mk3_trigger_configuration.hpp"
#include "pcie_trigger_configuration.hpp"
#include "pxi_trigger_configuration.hpp"
#include "pxi_router_trigger_configuration.hpp"
#include "brick_mk3_trigger_event_manager.hpp"
#include "pcie_trigger_event_manager.hpp"
#include "pxi_trigger_event_manager.hpp"
#include "pxi_router_trigger_event_manager.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace triggering {

/**
* Functions in this class deal with getting and setting general trigger 
* parameters.
*/
class TriggerFactory
{
public:
    /**
    * Create and return an instance of a TriggerActionManager object 
    * corresponding to the given Device.
    *
    * @param pDevice A pointer to a Device, used to determine the type of 
    *                TriggerActionManager object to create.
    *
    * @return A pointer to a new TriggerActionManager object, or NULL if device 
    *         doesn't support triggering.
    *
    * \note The \link stardundee::com::starsystem::triggering::TriggerActionManager TriggerActionManager \endlink
    *       object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    static _Check_return_ TriggerActionManager*
    CreateTriggerActionManager(_In_ com::starsystem::general::Device* pDevice)
    {
        /* if no valid device provided */
        if (pDevice == NULL)
        {
            /* can't create useful instance */
            return NULL;
        }

        /* pointer to a new TriggerActionManager object, initially null */
        stardundee::com::starsystem::triggering::TriggerActionManager 
            *pTriggerActionManager = NULL;

        /* the type of device needs to be determined so the appropriate
         * trigger action manager for the device can be instantiated and 
         * returned
         */

        /* first, get the device identification info for this device, which
         * contains the device's chip type */

        /* declare struct required by 'c' API to hold device identifier info */
        STAR_CFG_DEVICE_IDENTIFIER_INFO deviceInfo;

        /* call 'c' API to get identifier info for this device */
        int result = CFG_ROUTER_getDeviceIdentificationInfo(
            pDevice->GetDeviceID(), &deviceInfo);

        /* if failed to obtain device identifier info */
        if (result == 0)
        {
            /* return error / null */
            return NULL;
        }

        /* create an instance of a TriggerActionManager object based on the 
         * device's chip type */
        try
        {
            switch (deviceInfo.chipType)
            {
            case 8:
                /* a PCIe device */
                pTriggerActionManager = new
                stardundee::com::starsystem::triggering::pcie::PCIeTriggerActionManager(pDevice->GetDeviceID());
                break;
            case 19:
                /* a Brick Mk3 device */
                pTriggerActionManager = new
                stardundee::com::starsystem::triggering::brickmk3::BrickMk3TriggerActionManager(pDevice->GetDeviceID());
                break;
            case 21:
            case 22:
                /* a PXI device */
                pTriggerActionManager = new stardundee::com::starsystem::triggering::pxi::PXITriggerActionManager(pDevice->GetDeviceID());
                break;
            case 24:
                /* a PXI router device */
                pTriggerActionManager = new stardundee::com::starsystem::triggering::pxi::PXIRouterTriggerActionManager(pDevice->GetDeviceID());
                break;
            default:
                /* device does not support triggering */
                pTriggerActionManager = NULL;
                break;
            }
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for new object */
            pTriggerActionManager = NULL;
        }

        /* return the action manager, or NULL if an error occurred */
        return pTriggerActionManager;
    }

    /**
    * Create and return an instance of a TriggerConfiguration object 
    * corresponding to the given Device.
    *
    * @param pDevice A pointer to a Device, used to determine the type of 
    *                TriggerConfiguration object to create.
    *
    * @return A pointer to a new TriggerConfiguration object, or NULL if 
    *         device doesn't support triggering.
    *
    * \note The \link stardundee::com::starsystem::triggering::TriggerConfiguration TriggerConfiguration \endlink
    *       object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    static _Check_return_ TriggerConfiguration*
    CreateTriggerConfiguration(_In_ com::starsystem::general::Device* pDevice)
    {
        /* if no valid device provided */
        if (pDevice == NULL)
        {
            /* can't create useful instance */
            return NULL;
        }

        /* pointer to a new TriggerConfiguration object, initially null */
        stardundee::com::starsystem::triggering::TriggerConfiguration 
            *pTriggerConfiguration = NULL;

        /* the type of device needs to be determined so the appropriate
         * trigger configuration for the device can be instantiated and 
         * returned
         */

        /* first, get the device identification info for this device, which
         * contains the device's chip type */

        /* declare struct required by 'c' API to hold device identifier info */
        STAR_CFG_DEVICE_IDENTIFIER_INFO deviceInfo;

        /* call 'c' API to get identifier info for this device */
        int result = CFG_ROUTER_getDeviceIdentificationInfo(
            pDevice->GetDeviceID(), &deviceInfo);
        
        /* if failed to obtain device identifier info */
        if (result == 0)
        {
            /* return error / null */
            return NULL;
        }

        /* create an instance of a TriggerConfiguration object based on the 
         * device's chip type */
        try
        {
            switch (deviceInfo.chipType)
            {
            case 8:
                /* a PCIe device */
                pTriggerConfiguration = new
                stardundee::com::starsystem::triggering::pcie::PCIeTriggerConfiguration(
                pDevice->GetDeviceID());
                break;
            case 19:
                /* a Brick Mk3 device */
                pTriggerConfiguration = new
                stardundee::com::starsystem::triggering::brickmk3::BrickMk3TriggerConfiguration(
                pDevice->GetDeviceID());
                break;
            case 21:
            case 22:
                /* a PXI device */
                pTriggerConfiguration = new stardundee::com::starsystem::triggering::pxi::PXITriggerConfiguration(
                    pDevice->GetDeviceID());
                break;
            case 24:
                /* a PXI router device */
                pTriggerConfiguration = new stardundee::com::starsystem::triggering::pxi::PXIRouterTriggerConfiguration(
                    pDevice->GetDeviceID());
                break;
            default:
                /* device does not support triggering */
                pTriggerConfiguration = NULL;
                break;
            }
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for new object */
            pTriggerConfiguration = NULL;
        }

        /* return the configuration, or NULL if an error occurred */
        return pTriggerConfiguration;
    }

    /**
    * Create and return an instance of a TriggerEventManager object 
    * corresponding to the given Device.
    *
    * @param pDevice A pointer to a Device, used to determine the type of 
    *                TriggerEventManager object to create.
    *
    * @return A pointer to a new TriggerEventManager object, or NULL if device 
    *         doesn't support triggering.
    *
    * \note The \link stardundee::com::starsystem::triggering::TriggerEventManager TriggerEventManager \endlink
    *       object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    static _Check_return_ TriggerEventManager* CreateTriggerEventManager(
        _In_ com::starsystem::general::Device* pDevice)
    {
        /* if no valid device provided */
        if (pDevice == NULL)
        {
            /* can't create useful instance */
            return NULL;
        }

        /* pointer to a new TriggerEventManager object, initially null */
        stardundee::com::starsystem::triggering::TriggerEventManager 
            *pTriggerEventManager = NULL;

        /* the type of device needs to be determined so the appropriate
         * trigger event manager for the device can be instantiated and 
         * returned
         */

        /* first, get the device identification info for this device, which
         * contains the device's chip type */

        /* declare struct required by 'c' API to hold device identifier info */
        STAR_CFG_DEVICE_IDENTIFIER_INFO deviceInfo;

        /* call 'c' API to get identifier info for this device */
        int result = CFG_ROUTER_getDeviceIdentificationInfo(
            pDevice->GetDeviceID(), &deviceInfo);
        
        /* if failed to obtain device identifier info */
        if (result == 0)
        {
            /* return error / null */
            return NULL;
        }

        /* create an instance of a TriggerEventManager object based on the 
         * device's chip type */
        try
        {
            switch (deviceInfo.chipType)
            {
            case 8:
                /* a PCIe device */
                pTriggerEventManager = new
                stardundee::com::starsystem::triggering::pcie::PCIeTriggerEventManager(pDevice->GetDeviceID());
                break;
            case 19:
                /* a Brick Mk3 device */
                pTriggerEventManager = new
                stardundee::com::starsystem::triggering::brickmk3::BrickMk3TriggerEventManager(pDevice->GetDeviceID());
                break;
            case 21:
            case 22:
                /* a PXI device */
                pTriggerEventManager = new stardundee::com::starsystem::triggering::pxi::PXITriggerEventManager(pDevice->GetDeviceID());
                break;
            case 24:
                /* a PXI router device */
                pTriggerEventManager = new stardundee::com::starsystem::triggering::pxi::PXIRouterTriggerEventManager(pDevice->GetDeviceID());
                break;
            default:
                /* device does not support triggering */
                pTriggerEventManager = NULL;
                break;
            }
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for new object */
            pTriggerEventManager = NULL;
        }

        /* return the event manager, or NULL if an error occurred */
        return pTriggerEventManager;
    }
};

/* end namespace tags */
} /* triggering */ } /* starsystem */ } /* com */ } /* stardundee */

#endif

