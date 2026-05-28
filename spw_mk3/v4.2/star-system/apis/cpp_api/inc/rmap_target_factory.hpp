/**
* \file rmap_target_factory.hpp
*
* \brief Provides convenience functions for creating RMAP target related 
*        objects.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides convenience functions for creating RMAP target related objects.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET
*/

#ifndef __RMAP_TARGET_FACTORY_CPP__
#define __RMAP_TARGET_FACTORY_CPP__

#include "device.hpp"

#include "pxi_rmap_target_configuration.hpp"
#include "pxi_rmap_target_manual_authorisation_manager.hpp"
#include "pxi_rmap_target_notification_manager.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {

/**
* Functions in this class deal with getting and setting general RMAP target 
* parameters.
*/
class RMAPTargetFactory
{
public:
    /**
    * Create and return an instance of an RMAPTargetConfiguration object 
    * corresponding to the given Device.
    *
    * @param pDevice A pointer to a Device, used to determine the type of 
    *                RMAPTargetConfiguration object to create.
    *
    * @return A pointer to a new RMAPTargetConfiguration object, or NULL if 
    *         device doesn't support an RMAP target.
    *
    * \note The \link stardundee::com::starsystem::rmaptarget::RMAPTargetConfiguration RMAPTargetConfiguration \endlink
    *       object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    static _Check_return_ RMAPTargetConfiguration*
    CreateRMAPTargetConfiguration(
    _In_ com::starsystem::general::Device* pDevice)
    {
        /* if no valid device provided */
        if (pDevice == NULL)
        {
            /* can't create useful instance */
            return NULL;
        }

        /* pointer to a new RMAPTargetConfiguration object, initially null */
        stardundee::com::starsystem::rmaptarget::RMAPTargetConfiguration 
            *pRMAPTargetConfiguration = NULL;

        /* the type of device needs to be determined so the appropriate
         * RMAP target configuration for the device can be instantiated and 
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

        /* create an instance of a RMAPTargetConfiguration object based on 
         * the device's chip type */
        try
        {
            switch (deviceInfo.chipType)
            {
            case 22:
                /* a PXI with RMAP target device */
                pRMAPTargetConfiguration = new stardundee::com::starsystem::rmaptarget::pxi::PXIRMAPTargetConfiguration(pDevice->GetDeviceID());
                break;
            default:
                /* device does not support an RMAP target */
                pRMAPTargetConfiguration = NULL;
                break;
            }
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for new object */
            pRMAPTargetConfiguration = NULL;
        }

        /* return the configuration, or NULL if an error occurred */
        return pRMAPTargetConfiguration;
    }

    /**
    * Create and return an instance of an RMAPTargetManualAuthorisationManager 
    * object corresponding to the given Device.
    *
    * @param pDevice A pointer to a Device, used to determine the type of 
    *                RMAPTargetManualAuthorisationManager object to create.
    *
    * @return A pointer to a new RMAPTargetManualAuthorisationManager object, 
    *         or NULL if device doesn't support an RMAP target.
    *
    * \note The \link stardundee::com::starsystem::rmaptarget::authorisation::RMAPTargetManualAuthorisationManager RMAPTargetManualAuthorisationManager \endlink
    *       object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    static _Check_return_ authorisation::RMAPTargetManualAuthorisationManager*
    CreateRMAPTargetManualAuthorisationManager(
    _In_ com::starsystem::general::Device* pDevice)
    {
        /* if no valid device provided */
        if (pDevice == NULL)
        {
            /* can't create useful instance */
            return NULL;
        }

        /* pointer to a new RMAPTargetManualAuthorisationManager object, 
         * initially null */
        stardundee::com::starsystem::rmaptarget::authorisation::RMAPTargetManualAuthorisationManager 
            *pRMAPTargetManualAuthorisationManager = NULL;

        /* the type of device needs to be determined so the appropriate
         * RMAP target manual authorisation manager for the device can be 
         * instantiated and returned
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

        /* create an instance of a RMAPTargetManualAuthorisationManager object 
         * based on the device's chip type */
        try
        {
            switch (deviceInfo.chipType)
            {
            case 22:
                /* a PXI with RMAP target device */
                pRMAPTargetManualAuthorisationManager = new stardundee::com::starsystem::rmaptarget::authorisation::pxi::PXIRMAPTargetManualAuthorisationManager(pDevice->GetDeviceID());
                break;
            default:
                /* device does not support an RMAP target */
                pRMAPTargetManualAuthorisationManager = NULL;
                break;
            }
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for new object */
            pRMAPTargetManualAuthorisationManager = NULL;
        }

        /* return the manual authorisation manager, or NULL if an error 
         * occurred */
        return pRMAPTargetManualAuthorisationManager;
    }

    /**
    * Create and return an instance of an RMAPTargetNotificationManager 
    * object corresponding to the given Device.
    *
    * @param pDevice A pointer to a Device, used to determine the type of 
    *                RMAPTargetNotificationManager object to create.
    *
    * @return A pointer to a new RMAPTargetNotificationManager object, 
    *         or NULL if device doesn't support an RMAP target.
    *
    * \note The \link stardundee::com::starsystem::rmaptarget::notifications::RMAPTargetNotificationManager RMAPTargetNotificationManager \endlink
    *       object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    static _Check_return_ notifications::RMAPTargetNotificationManager*
    CreateRMAPTargetNotificationManager(
    _In_ com::starsystem::general::Device* pDevice)
    {
        /* if no valid device provided */
        if (pDevice == NULL)
        {
            /* can't create useful instance */
            return NULL;
        }

        /* pointer to a new RMAPTargetNotificationManager object, initially 
         * null */
        stardundee::com::starsystem::rmaptarget::notifications::RMAPTargetNotificationManager 
            *pRMAPTargetNotificationManager = NULL;

        /* the type of device needs to be determined so the appropriate
         * RMAP target notification manager for the device can be 
         * instantiated and returned
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

        /* create an instance of a RMAPTargetNotificationManager object based on 
         * the device's chip type */
        try
        {
            switch (deviceInfo.chipType)
            {
            case 22:
                /* a PXI with RMAP target device */
                pRMAPTargetNotificationManager = new stardundee::com::starsystem::rmaptarget::notifications::pxi::PXIRMAPTargetNotificationManager(pDevice->GetDeviceID());
                break;
            default:
                /* device does not support an RMAP target */
                pRMAPTargetNotificationManager = NULL;
                break;
            }
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* failed to allocate memory for new object */
            pRMAPTargetNotificationManager = NULL;
        }

        /* return the notification manager, or NULL if an error 
         * occurred */
        return pRMAPTargetNotificationManager;
    }
};

/* end namespace tags */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

