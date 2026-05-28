/**
* \file pxi_rmap_target_notification_manager.hpp
*
* \brief Provides functions for handling RMAP target notifications specific to
*        PXI devices.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling RMAP target notifications on PXI devices.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET_NOTIF_PXI
*/

#ifndef __PXI_RMAP_TARGET_NOTIFICATION_MANAGER_CPP__
#define __PXI_RMAP_TARGET_NOTIFICATION_MANAGER_CPP__

#include "rmap_target_notification_manager.hpp"

#include "rmap_target_pxi_if.h"

#include "authorisation_request_notification.hpp"
#include "command_complete_notification.hpp"
#include "device_factory.hpp"

#include "cpp_api_macros.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {
                                       namespace notifications {
                                       namespace pxi {

/**
* Functions in this class deal with the RMAP target notification handling on a
* PXI card.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to 
*       RMAPTargetFactory::CreateRMAPTargetNotificationManager() or by 
*       instantiating directly with a STAR-System device ID.
*/
class PXIRMAPTargetNotificationManager : public RMAPTargetNotificationManager
{
private:
    /**
    * Call-back function which will be registered with STAR-System to receive
    * RMAP command notifications.
    *
    * @param deviceID   The ID of the device that the notification corresponds
    *                   to.
    * @param target     The RMAP target that the notification relates to.
    * @param pNotif     Notification packet that has been received.
    * @param pContext   A pointer to user-supplied context information which
    *                   was provided when the device listener was being
    *                   registered and which is expected to contain a pointer
    *                   to a DeviceListenerCallback object which will be used
    *                   to issue notifications back to the user of whether a
    *                   device has been added or removed.
    */
    static void STAR_API_CC NotificationCallback(STAR_DEVICE_ID deviceId,
        U32 target, void *pNotif, void *pContext)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(deviceId);
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(target);

        /* get callback from context data */
        RMAPNotificationCallback *pNotificationCallback =
            (RMAPNotificationCallback *)pContext;

        /* if valid callback is present */
        if(pNotificationCallback != NULL)
        {
            /* create instance of Device object that represents the device on
             * which the notification was received on */
            com::starsystem::general::Device *pDevice = 
                com::starsystem::general::DeviceFactory::CreateDevice(deviceId);
            
            /* convert notification packet buffer into RMAPNotification */
            RMAPNotification *pNotification =
                RMAPNotificationFromBuffer((U8 *)pNotif);

            /** \note The Device and RMAPNotification instances are deleted 
             *        after the callback has taken place to ensure that its 
             *        resources have been freed.
             */

            /* notify the callback that RMAP notification was received */
            pNotificationCallback->NotificationReceived(pDevice, pNotification);

            /* free device and notification objects after use */
            if (pDevice)
            {
                delete pDevice;
            }
            if (pNotification)
            {
                delete pNotification;
            }
        }
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PXIRMAPTargetNotificationManager() : RMAPTargetNotificationManager()
    {
    }

    /**
    * Constructor - use given parameters to initialise RMAP target notification
    * manager for given PXI device.
    *
    * @param deviceID The device ID of the PXI device that the RMAP target
    *                 notification manager relates to.
    */
    explicit PXIRMAPTargetNotificationManager(STAR_DEVICE_ID deviceID) :
        RMAPTargetNotificationManager(deviceID)
    {
    }

    int RegisterNotificationListener(U32 target, NOTIF_TYPE type,
        RMAPNotificationCallback *pNotificationCallback)
    {
        /* call 'c' API to register for notifications */
        return RMAP_TARGET_PXI_IF_registerNotificationListenerWithContext(
            idOfOwningDevice, target, type, NotificationCallback,
            pNotificationCallback);
    }

    int GetEnabledNotifications(U32 target, NOTIF_TYPE_MASK *pNotifications)
    {
        /* call 'c' API to get enabled notifications */
        return RMAP_TARGET_PXI_IF_getEnabledNotifications(idOfOwningDevice,
            target, pNotifications);
    }

    int SetEnabledNotifications(U32 target, NOTIF_TYPE_MASK notifications)
    {
        /* call 'c' API to set enabled notifications */
        return RMAP_TARGET_PXI_IF_setEnabledNotifications(idOfOwningDevice,
            target, notifications);
    }

    int StartReceivingNotifications()
    {
        /* call 'c' API to start receiving notifications */
        return RMAP_TARGET_PXI_IF_startReceivingNotifications(idOfOwningDevice);
    }

    int StopReceivingNotifications()
    {
        /* call 'c' API to unregister from notifications */
        return RMAP_TARGET_PXI_IF_stopReceivingNotifications(idOfOwningDevice);
    }

    int UnregisterNotificationListener(U32 target, NOTIF_TYPE type)
    {
        /* call 'c' API to set enabled notifications */
        return RMAP_TARGET_PXI_IF_unregisterNotificationListenerWithContext(
            idOfOwningDevice, target, type);
    }
};

/* end namespace tags */
} /* pxi */
} /* notifications */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

