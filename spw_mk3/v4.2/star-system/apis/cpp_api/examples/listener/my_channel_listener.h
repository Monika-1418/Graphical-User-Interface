/**
 * \file my_channel_listener.h
 *
 * \brief Provides an example of a custom ChannelListenerCallback, required when
 *        registering channel listeners with the STAR-System C++ API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides an example of a custom ChannelListenerCallback, required when
 * registering channel listeners with the STAR-System C++ API.
 *
 * Users are expected to create their own class for receiving channel event
 * notifications, which must be derived as a child class of the following
 * C++ API class: ChannelListenerCallback.
 *
 * Users must then provide their own custom implementations for the call-back
 * functions inherited from the parent class which will be automatically invoked
 * by the STAR-System API when a channel is opened or closed.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include <cstring>
#include <cstdio>
#include "channel_listener_callback.hpp"
#include "device.hpp"
#include "driver.hpp"
#include "channel.hpp"
#include <iostream>

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::channels;
using namespace std;

#ifndef MY_CHANNEL_LISTENER_H
#define MY_CHANNEL_LISTENER_H

/**
 * Define a custom ChannelListenerCallback object.
 */
class MyChannelListenerCallback : public ChannelListenerCallback
{
private:
    /* a string which will hold contextual or identifying information */
    char contextInfo[256];

    /* \note You can add any other context information you may require to
     * assist your own processing.
     */

public:
    /**
     * Default constructor.
     */
    MyChannelListenerCallback()
    {
        contextInfo[0] = '\0';
    }

    /**
     * Overloaded constructor - initialise with given context information for
     * object.
     *
     * @param info A user supplied string which will be used to provide
     *             contextual or identifying information for the call-back
     *             object when displaying status messages.
     */
    explicit MyChannelListenerCallback(const char *info)
    {
        strcpy(contextInfo, info);
    }

    /**
     * Destructor - made virtual to support correct polymorphic
     * deletion of child classes if required.
     */
    virtual ~MyChannelListenerCallback()
    {
    }

    /**
     * Custom user implementation of the Call-back function which will be called
     * to provide notification of when a channel has been opened.
     *
     * @param pDriver A pointer to a Driver object which can be used to obtain
     *                information about the device on which the channel
     *                was opened.
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
     *                information about the device on which the channel has been
     *                opened.
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
     * @param pChannel A pointer to a Channel object which can be used to obtain
     *                information about the channel which has been opened.
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
    virtual void ChannelOpened(Driver *pDriver, Device *pDevice,
                               Channel *pChannel)
    {
        /* \todo Add your own code here - see sample below */
        cout << "Channel opened [in " << contextInfo << "]:" << endl;

        /* if valid driver object provided */
        if (pDriver)
        {
            /* display driver information */
            Driver::DriverType driverType = pDriver->GetDriverType();
            switch (driverType)
            {
            case Driver::DRIVER_TYPE_USB:
                cout << "Driver type for device on which channel was opened: USB" << endl;
                break;
            case Driver::DRIVER_TYPE_PCI:
                cout << "Driver type for device on which channel was opened: PCI" << endl;
                break;
            case Driver::DRIVER_TYPE_TCPIP:
                cout << "Driver type for device on which channel was opened:"
                     " TCPIP" << endl;
                break;
            case Driver::DRIVER_TYPE_VIRTUAL:
                cout << "Driver type for device on which channel was opened:"
                     " Virtual" << endl;
                break;
            case Driver::DRIVER_TYPE_INVALID:
                cout << "Driver type for device on which channel was opened:"
                     " Invalid" << endl;
                break;
            default:
                cout << "Driver type for device on which channel was opened:"
                     " Unknown" << endl;
                break;
            }
        }
        else
        {
            cout << "No information provided about the driver type for the"
                " device on which channel the was opened" << endl;
        }

        /* if valid device object provided */
        if (pDevice)
        {
            /* display device information */
            char deviceName[Device::STR_DEVICE_NAME_LEN];
            size_t stringLen;
            stringLen = pDevice->GetDeviceName(deviceName);
            if (stringLen)
            {
                cout << "Name of device on which channel was opened: ";
                cout << deviceName << endl;
            }
            else
            {
                cout << "Failed to obtain name of device on which channel was"
                     " opened" << endl;
            }
            cout << "Device ID: " << pDevice->GetDeviceID() << endl;
        }
        else
        {
            cout << "No information provided about the device on which the"
                 " channel was opened" << endl;
        }

        /* if valid channel object provided */
        if (pChannel)
        {
            /* display the channel information */
            cout << "Channel with ID " << pChannel->GetChannelID();
            cout << " opened on channel number " << 
                (int)pChannel->GetChannelNumber() << endl;
        }
        else
        {
            cout << "No information provided about the channel that was opened" << endl;
        }

        cout << "" << endl;
    }

    /**
     * Custom user implementation of the Call-back function which will be 
     * called to provide notification of when a channel has been closed.
     *
     * @param pDriver A pointer to a Driver object which can be used to obtain
     *                information about the driver for the device on which the
     *                channel was closed.
     *
     * \note The value of this parameter may be NULL if the relevant driver
     *       failed to be identified. Even if the value of this parameter is
     *       non-NULL there are cases where only limited information for the
     *       driver may be available, e.g. if the driver is no longer present.
     *
     * \note The pointer to the Driver object provided will no longer be valid
     *       once the processing of the call-back function completes. It will 
     *       be automatically freed by the caller (the STAR-System API).
     *
     * @param pDevice A pointer to a Device object which can be used to obtain
     *                information about the device on which the channel was
     *                closed.
     *
     * \note The value of this parameter may be NULL if the relevant device
     *       failed to be identified. Even if the value of this parameter is
     *       non-NULL there are cases where only limited information for the
     *       device may be available, e.g. if the device is no longer present.
     *
     * \note The pointer to the Device object provided will no longer be valid
     *       once the processing of the call-back function completes. It will 
     *       be automatically freed by the caller (the STAR-System API).
     *
     * @param pChannel A pointer to a Channel object which can be used to 
     *                obtain information about the channel which was closed.
     *
     * \note The value of this parameter may be NULL if an error occurred
     *       creating the relevant channel object, e.g. a memory allocation
     *       error.
     *
     * \note The pointer to the Channel object provided will no longer be valid
     *       once the processing of the call-back function completes. It will 
     *       be automatically freed by the caller (the STAR-System API).
     *
     */
    virtual void ChannelClosed(Driver *pDriver, Device *pDevice,
                               Channel *pChannel)
    {
        /* \todo Add your own code here - see sample below */
        cout << "Channel closed [in " << contextInfo << "]:" << endl;

        /* if valid driver object provided */
        if (pDriver)
        {
            /* display driver information */
            Driver::DriverType driverType = pDriver->GetDriverType();
            switch (driverType)
            {
            case Driver::DRIVER_TYPE_USB:
                cout << "Driver type for device on which channel was closed: USB" << endl;
                break;
            case Driver::DRIVER_TYPE_PCI:
                cout << "Driver type for device on which channel was closed: PCI" << endl;
                break;
            case Driver::DRIVER_TYPE_TCPIP:
                cout << "Driver type for device on which channel was closed:"
                     " TCPIP" << endl;
                break;
            case Driver::DRIVER_TYPE_VIRTUAL:
                cout << "Driver type for device on which channel was closed:"
                     " Virtual" << endl;
                break;
            case Driver::DRIVER_TYPE_INVALID:
                cout << "Driver type for device on which channel was closed:"
                     " Invalid" << endl;
                break;
            default:
                cout << "Driver type for device on which channel was closed:"
                     " Unknown" << endl;
                break;
            }
        }
        else
        {
            cout << "No information provided about the driver type for the"
                " device on which channel the was closed" << endl;
        }

        /* if valid device object provided */
        if (pDevice)
        {
            /* display device information */
            char deviceName[Device::STR_DEVICE_NAME_LEN];
            size_t stringLen;
            stringLen = pDevice->GetDeviceName(deviceName);
            if (stringLen)
            {
                cout << "Name of device on which channel was closed: ";
                cout << deviceName << endl;
            }
            else
            {
                cout << "Failed to obtain name of device on which channel was"
                     " closed" << endl;
            }
            cout << "Device ID: " << pDevice->GetDeviceID() << endl;
        }
        else
        {
            cout << "No information provided about the device on which the"
                " channel was closed" << endl;
        }

        /* if valid channel object provided */
        if (pChannel)
        {
            /* display the channel information */
            cout << "Channel with ID " << pChannel->GetChannelID();
            cout << " closed on channel number " << 
                (int)pChannel->GetChannelNumber() << endl;
        }
        else
        {
            cout << "No information provided about the channel that was closed" << endl;
        }

        cout << "" << endl;
    }
};

#endif /* MY_CHANNEL_LISTENER_H */

