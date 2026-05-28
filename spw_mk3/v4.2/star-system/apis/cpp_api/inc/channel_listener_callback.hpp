/**
 * \file channel_listener_callback.hpp
 *
 * \brief Parent call-back object for channel events for receiving notifications
 *        of when channels are opened or closed.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Parent call-back object for channel events for receiving notifications of
 * when channels are opened or closed.
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
 * your own call-back object as a child-class of the ChannelListenerCallback
 * class and overriding the event notification methods it contains to provide
 * implementations for them. You can then provide an instance of your custom
 * call-back object to the STAR-System API when registering your channel
 * listener.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_CHANNELS
 */

#ifndef __STAR_CHANNEL_LISTENER_CALLBACK_CPP__
#define __STAR_CHANNEL_LISTENER_CALLBACK_CPP__

#include "star_system_listener_callback.hpp"
#include "channel.hpp"
#include "device.hpp"
#include "driver.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace channels {

/**
 * Parent call-back object for channel events for receiving notifications of
 * when channels are opened or closed.
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
 * your own call-back object as a child-class of the ChannelListenerCallback
 * class and overriding the event notification methods it contains to provide
 * implementations for them. You can then provide an instance of your custom
 * call-back object to the STAR-System API when registering your channel
 * listener.
*/
class ChannelListenerCallback :
       public stardundee::com::starsystem::general::STARSystemListenerCallback
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    ChannelListenerCallback()
    {
    }

    /**
    * Destructor - made virtual to support correct polymorphic
    * deletion of child classes.
    */
    virtual ~ChannelListenerCallback()
    {
    }

    /**
    * Call-back function which will be automatically invoked by the STAR-System
    * API to provide notification of when a channel has been opened.
    *
    * \note This is a pure virtual function. Child classes which represent
    *       more specific forms of ChannelListenerCallback must override this
    *       function to undertake whatever processing is required when
    *       receiving notification of a channel being opened.
    *
    * @param pDriver A pointer to a \link stardundee::com::starsystem::general::Driver Driver \endlink
    *                object which can be used to obtain information about the
    *                driver for the device on which the channel was opened.
    *
    * \note The value of this parameter may be NULL if the relevant driver
    *       failed to be identified. Even if the value of this parameter is
    *       non-NULL there are cases where only limited information for the
    *       driver may be available, e.g. if the driver is no longer present.
    *
    * \note The pointer to the \link stardundee::com::starsystem::general::Driver Driver \endlink
    *       object provided will no longer be valid once the processing of the
    *       call-back function completes. It will be automatically freed by the
    *       caller (the STAR-System API).
    *
    * @param pDevice A pointer to a \link stardundee::com::starsystem::general::Device Device \endlink
    *                object which can be used to obtain information about the
    *                device on which the channel has been opened.
    *
    * \note The value of this parameter may be NULL if the relevant device
    *       failed to be identified. Even if the value of this parameter is
    *       non-NULL there are cases where only limited information for the
    *       device may be available, e.g. if the device is no longer present.
    *
    * \note The pointer to the \link stardundee::com::starsystem::general::Device Device \endlink
    *       object provided will no longer be valid once the processing of the
    *       call-back function completes. It will be automatically freed by the
    *       caller (the STAR-System API).
    *
    * @param pChannel A pointer to a Channel object which can be used to obtain
    *                 information about the channel which has been opened.
    *
    * \note The value of this parameter may be NULL if an error occurred
    *       creating the relevant channel object, e.g. a memory allocation
    *       error.
    *
    * \note The pointer to the Channel object provided will no longer be valid
    *       once the processing of the call-back function completes. It will be
    *       automatically freed by the caller (the STAR-System API).
    *
    */
    virtual void ChannelOpened(
                        stardundee::com::starsystem::general::Driver *pDriver,
                        stardundee::com::starsystem::general::Device *pDevice,
                        Channel *pChannel)=0;

    /**
    * Call-back function  which will be automatically invoked by the STAR-System
    * API to provide notification of when a channel has been closed.
    *
    * \note This is a pure virtual function. Child classes which represent
    *       more specific forms of ChannelListenerCallback must override this
    *       function to undertake whatever processing is required when receiving
    *       notification of a channel being closed.
    *
    * @param pDriver A pointer to a \link stardundee::com::starsystem::general::Driver Driver \endlink
    *                object which can be used to obtain information about the
    *                driver for the device on which the channel was closed.
    *
    * \note The value of this parameter may be NULL if the relevant driver
    *       failed to be identified. Even if the value of this parameter is
    *       non-NULL there are cases where only limited information for the
    *       driver may be available, e.g. if the driver is no longer present.
    *
    * \note The pointer to the \link stardundee::com::starsystem::general::Driver Driver \endlink
    *       object provided will no longer be valid once the processing of the
    *       call-back function completes. It will be automatically freed by the
    *       caller (the STAR-System API).
    *
    * @param pDevice A pointer to a \link stardundee::com::starsystem::general::Device Device \endlink
    *                object which can be used to obtain information about the
    *                device on which the channel was closed.
    *
    * \note The value of this parameter may be NULL if the relevant device
    *       failed to be identified. Even if the value of this parameter is
    *       non-NULL there are cases where only limited information for the
    *       device may be available, e.g. if the device is no longer present.
    *
    * \note The pointer to the \link stardundee::com::starsystem::general::Device Device \endlink
    *       object provided will no longer be valid
    *       once the processing of the call-back function completes. It will be
    *       automatically freed by the caller (the STAR-System API).
    *
    * @param pChannel A pointer to a Channel object which can be used to obtain
    *                 information about the channel which was closed.
    *
    * \note The value of this parameter may be NULL if an error occurred
    *       creating the relevant channel object, e.g. a memory allocation
    *       error.
    *
    * \note The pointer to the Channel object provided will no longer be valid
    *       once the processing of the call-back function completes. It will be
    *       automatically freed by the caller (the STAR-System API).
    *
    */
    virtual void ChannelClosed(
                        stardundee::com::starsystem::general::Driver *pDriver,
                        stardundee::com::starsystem::general::Device *pDevice,
                        Channel *pChannel)=0;
};

/* end namespace tags */
} /* channels */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

