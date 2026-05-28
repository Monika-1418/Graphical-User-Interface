/**
* \file rmap_notification.hpp
*
* \brief Represents an RMAP notification that has been returned by the
*        RMAP Target API.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Represents an RMAP notification that has been returned by the RMAP Target API.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET_NOTIF
*/

#ifndef __RMAP_NOTIFICATION_CPP__
#define __RMAP_NOTIFICATION_CPP__

#include "rmap_target_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {
                                       namespace notifications {

/**
* An RMAP notification encapsulates the data returned during an RMAP target
* notification callback.
*/
class RMAPNotification
{
protected:
    /** the type of notification that is represented */
    NOTIF_TYPE notificationType;

    /** target index */
    U8 targetIndex;

    /** current time-code at time of event */
    U8 currentTimeCode;

    /**
    * Constructor. Initialise object to default state.
    *
    * @param notificationType The type of notification that is represented.
    */
    explicit RMAPNotification(NOTIF_TYPE notificationType = NOTIF_TYPE_NONE) :
        notificationType(notificationType), targetIndex(0), currentTimeCode(0)
    {
    }

    /**
    * Constructor - use given parameters to initialise RMAP notification.
    *
    * @param notificationType The type of notification that is represented.
    * @param targetIndex The target index value.
    * @param currentTimeCode The current time-code value.
    */
    RMAPNotification(NOTIF_TYPE notificationType, 
        U8 targetIndex, U8 currentTimeCode) :
        notificationType(notificationType), targetIndex(targetIndex), 
        currentTimeCode(currentTimeCode)
    {
    }

public:
    /**
    * Copy constructor - invoked when a copy of an
    * RMAPNotification object is made, e.g. such as through being a return
    * value from a function or when being passed into a function as a 'value'
    * parameter.
    *
    * A new RMAPNotification will be created which is a duplicate of the
    * RMAPNotification being copied from.
    *
    * @param rmapNotification The RMAPNotification object which is being copied.
    *
    */
    RMAPNotification(const RMAPNotification& rmapNotification)
    {
        /* copy contents of object being copied into this object */
        targetIndex = rmapNotification.targetIndex;
        currentTimeCode = rmapNotification.currentTimeCode;
    }

    /**
    * Overloaded assignment operator - invoked when an attempt is made to assign
    * the contents of one RMAPNotification object into another.
    *
    * Any existing RMAPNotification will be destroyed and a new
    * RMAPNotification created which is a duplicate of the RMAPNotification
    * being assigned.
    *
    * @param rmapNotification The RMAPNotification object whose contents are
    *                         being assigned to 'this' object.
    *
    */
    RMAPNotification& operator=(const RMAPNotification& rmapNotification)
    {
        /* check for self-assignment, no point in copying one's self */
        if(this == &rmapNotification)
        {
            return *this;
        }

        /* copy contents of object being copied into this object */
        targetIndex = rmapNotification.targetIndex;
        currentTimeCode = rmapNotification.currentTimeCode;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Get the notification type.
    *
    * @return The type of notification.
    *
    */
    NOTIF_TYPE GetNotificationType()
    {
        return notificationType;
    }

    /**
    * Get the target index of the notification.
    *
    * @return The target index.
    *
    */
    U8 GetTargetIndex()
    {
        return targetIndex;
    }

    /**
    * Get the current time-code of the notification.
    *
    * @return The current time-code.
    *
    */
    U8 GetCurrentTimeCode()
    {
        return currentTimeCode;
    }
};

/* end namespace tags */
} /* notifications */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

