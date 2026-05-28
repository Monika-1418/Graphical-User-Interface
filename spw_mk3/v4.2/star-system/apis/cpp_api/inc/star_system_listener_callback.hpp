/**
 * \file star_system_listener_callback.hpp
 *
 * \brief Parent class for call-back objects which are used to receive
 *        notifications of events occurring within STAR-System, e.g. when
 *        devices are added or removed, or channels opened or closed.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Parent class for call-back objects which are used to receive notifications
 * of events occurring within STAR-System, e.g. when devices are added or
 * removed, or channels opened or closed.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_LISTENER_CALLBACK_CPP__
#define __STAR_LISTENER_CALLBACK_CPP__

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
 * Parent class for call-back objects which are used to receive notifications
 * of events occurring within STAR-System, e.g. when devices are added or
 * removed, or channels opened or closed.
*/
class STARSystemListenerCallback
{
public:
    /**
    * Default constructor.
    */
    STARSystemListenerCallback()
    {
    }

    /**
    * Destructor - made virtual to support correct polymorphic
    * deletion of child classes.
    */
    virtual ~STARSystemListenerCallback()
    {
    }
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

