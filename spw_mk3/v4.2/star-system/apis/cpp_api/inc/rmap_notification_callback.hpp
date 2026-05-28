/**
 * \file rmap_notification_callback.hpp
 *
 * \brief Parent call-back object for RMAP target notifications.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Parent call-back object for RMAP target notifications when authorisation is
 * requested or the command is completed.
 *
 * The STAR-System API allows you to register 'listeners' for various events
 * of interest, e.g. such as devices being added or removed, channels being
 * opened or closed, and so on. When registering a listener one must provide an
 * instance of a call-back object. The STAR-System API will automatically
 * invoke methods on the call-back object provided to notify it of when the
 * events of interest occur, e.g. such as channels being opened or closed.
 *
 * You must provide your own implementation for any call-back object which
 * you register with the API. In this case this would be acheived by deriving
 * your own call-back object as a child-class of the
 * NotificationListenerCallback class and overriding the event notification
 * methods it contains to provide implementations for them. You can then
 * provide an instance of your custom call-back object to the STAR-System API
 * when registering your notification listener.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 *
 * \ingroup CPP_RMAP_TARGET_NOTIF
 */

#ifndef __STAR_RMAP_NOTIFICATION_CALLBACK_CPP__
#define __STAR_RMAP_NOTIFICATION_CALLBACK_CPP__

#include "star_system_listener_callback.hpp"
#include "device.hpp"
#include "rmap_notification.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {
                                       namespace notifications {

/**
 * Parent call-back object for receiving notifications of when RMAP operations
 * are authorised or completed.
 *
 * The STAR-System API allows you to register 'listeners' for various events
 * of interest, e.g. such as devices being added or removed, channels being
 * opened or closed, and so on. When registering a listener one must provide an
 * instance of a call-back object. The STAR-System API will automatically
 * invoke methods on the call-back object provided to notify it of when the
 * events of interest occur, e.g. such as channels being opened or closed.
 *
 * You must provide your own implementation for any call-back object which
 * you register with the API. In this case this would be acheived by deriving
 * your own call-back object as a child-class of the
 * NotificationListenerCallback class and overriding the event notification
 * methods it contains to provide implementations for them. You can then
 * provide an instance of your custom call-back object to the STAR-System API
 * when registering your notification listener.
*/
class RMAPNotificationCallback :
    public stardundee::com::starsystem::general::STARSystemListenerCallback
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPNotificationCallback() : stardundee::com::starsystem::general::STARSystemListenerCallback()
    {
    }

    /**
    * Destructor - made virtual to support correct polymorphic
    * deletion of child classes.
    */
    virtual ~RMAPNotificationCallback()
    {
    }

    /**
    * Called when notification has been received from the target.
    *
    * \note The device and notification will be disposed of after the code in 
    *       this method has been executed.
    *
    * @param[in] pDevice the device that the notification has been received on.
    * @param[in] pNotification The notification that has been received.
    */
    virtual void NotificationReceived(
        stardundee::com::starsystem::general::Device *pDevice,
        stardundee::com::starsystem::rmaptarget::notifications::
        RMAPNotification *pNotification) = 0;
};

/* end namespace tags */
} /* notifications */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

