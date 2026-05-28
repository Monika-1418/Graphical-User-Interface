/**
* \file authorisation_request_notification.hpp
*
* \brief Represents an RMAP notification that has been returned by the
*        RMAP Target API when a authorisation has been requested.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Represents an RMAP notification that has been returned by the RMAP Target API
* in relation to an authorisation request.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET_NOTIF
*/

#include "rmap_notification.hpp"

#ifndef __AUTHORISATION_REQUEST_NOTIFICATION_CPP__
#define __AUTHORISATION_REQUEST_NOTIFICATION_CPP__

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {
                                       namespace notifications {

/**
* Class that encapsulates data for an authorisation request notification.
*/
class AuthorisationRequestNotification : public RMAPNotification
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    AuthorisationRequestNotification() : RMAPNotification(
        NOTIF_TYPE_AUTH_REQUEST)
    {
    }

    /**
    * Constructor - use given parameters to initialise authorisation request
    * notification.
    *
    * @param targetIndex The target index value.
    * @param currentTimeCode The current time-code value.
    */
    AuthorisationRequestNotification(U8 targetIndex, U8 currentTimeCode) : 
        RMAPNotification(NOTIF_TYPE_AUTH_REQUEST, targetIndex, currentTimeCode)
    {
    }
};

/* end namespace tags */
} /* notifications */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

