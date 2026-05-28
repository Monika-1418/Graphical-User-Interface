/**
 * \file device_listener_callback.hpp
 *
 * \brief Parent call-back object for device events for receiving notifications
 *        of when devices are added or removed.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Parent call-back object for device events for receiving notifications of when
 * devices are added or removed.
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
 * your own call-back object as a child-class of the DeviceListenerCallback
 * class and overriding the event notification methods it contains to provide
 * implementations for them. You can then provide an instance of your custom
 * call-back object to the STAR-System API when registering your device
 * listener.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_DEVICE_LISTENER_CALLBACK_CPP__
#define __STAR_DEVICE_LISTENER_CALLBACK_CPP__

#include "star_system_listener_callback.hpp"
#include "device.hpp"
#include "driver.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
 * Parent call-back object for device events for receiving notifications of when
 * devices are added or removed.
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
 * your own call-back object as a child-class of the DeviceListenerCallback
 * class and overriding the event notification methods it contains to provide
 * implementations for them. You can then provide an instance of your custom
 * call-back object to the STAR-System API when registering your device
 * listener.
 */
class DeviceListenerCallback : public STARSystemListenerCallback
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    DeviceListenerCallback()
    {
    }

    /**
    * Destructor - made virtual to support correct polymorphic
    * deletion of child classes.
    */
    virtual ~DeviceListenerCallback()
    {
    }

    /**
    * Call-back function  which will be automatically invoked by the STAR-System
    * API to provide notification of when a device has been added.
    *
    * \note This is a pure virtual function. Child classes which represent
    *       more specific forms of DeviceListenerCallback must override this
    *       function to undertake whatever processing is required when receiving
    *       notification of a device being added.
    *
    * @param pDriver A pointer to a Driver object which can be used to obtain
    *                information for the driver on which the device was added.
    *
    * \note The value of this parameter may be NULL if the relevant driver
    *       failed to be identified. Even if the value of this parameter is
    *       non-NULL there are cases where only limited information for the
    *       driver may be available, e.g. if the driver is no longer present.
    *
    * \note The pointer to the Driver object provided will no longer be valid
    *       once the processing of the call-back function completes. It will be
    *       automatically freed by the caller (the STAR-System API).
    *
    * @param pDevice A pointer to a Device object which can be used to obtain
    *                information for the device which was added.
    *
    * \note The value of this parameter may be NULL if the relevant device
    *       failed to be identified. Even if the value of this parameter is
    *       non-NULL there are cases where only limited information for the
    *       device may be available, e.g. if the device is no longer present.
    *
    * \note The pointer to the Device object provided will no longer be valid
    *       once the processing of the call-back function completes. It will be
    *       automatically freed by the caller (the STAR-System API).
    *
    */
    virtual void DeviceAdded(Driver *pDriver, Device *pDevice)=0;

    /**
    * Call-back function  which will be automatically invoked by the STAR-System
    * API to provide notification of when a device has been removed.
    *
    * \note This is a pure virtual function. Child classes which represent
    *       more specific forms of DeviceListenerCallback must override this
    *       function to undertake whatever processing is required when receiving
    *       notification of a device being removed.
    *
    * @param pDriver A pointer to a Driver object which can be used to obtain
    *                information for the driver on which the device was removed.
    *
    * \note The value of this parameter may be NULL if the relevant driver
    *       failed to be identified. Even if the value of this parameter is
    *       non-NULL there are cases where only limited information for the
    *       driver may be available, e.g. if the driver is no longer present.
    *
    * \note The pointer to the Driver object provided will no longer be valid
    *       once the processing of the call-back function completes. It will be
    *       automatically freed by the caller (the STAR-System API).
    *
    * @param pDevice A pointer to a Device object which can be used to obtain
    *                information for the device which was removed.
    *
    * \note The value of this parameter may be NULL if the relevant device
    *       failed to be identified. Even if the value of this parameter is
    *       non-NULL there are cases where only limited information for the
    *       device may be available, e.g. if the device is no longer present.
    *
    * \note The pointer to the Device object provided will no longer be valid
    *       once the processing of the call-back function completes. It will be
    *       automatically freed by the caller (the STAR-System API).
    *
    */
    virtual void DeviceRemoved(Driver *pDriver, Device *pDevice)=0;
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

