/**
 * \file my_device_listener.h
 *
 * \brief Provides an example of a custom DeviceListenerCallback, required when
 *        registering device listeners with the STAR-System C++ API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides an example of a custom DeviceListenerCallback, required when
 * registering device listeners with the STAR-System C++ API.
 *
 * Users are expected to create their own class for receiving device event
 * notifications, which must be derived as a child class of the following
 * C++ API class: DeviceListenerCallback.
 *
 * Users must then provide their own custom implementations for the call-back
 * functions inherited from the parent class which will be automatically
 * invoked by the STAR-System API when a device is added or removed.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include <cstring>
#include <cstdio>
#include "device_listener_callback.hpp"
#include "device.hpp"
#include "driver.hpp"
#include <iostream>

using namespace stardundee::com::starsystem::general;
using namespace std;

#ifndef MY_DEVICE_LISTENER_H
#define MY_DEVICE_LISTENER_H

/**
 * Define a custom DeviceListenerCallback object.
 */
class MyDeviceListenerCallback : public DeviceListenerCallback
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
    MyDeviceListenerCallback()
    {
    }

    /**
     * Overloaded constructor - initialise with given context information for
     * object.
     *
     * @param info A user supplied string which will be used to provide
     *             contextual or identifying information for the call-back
     *             object when displaying status messages.
     */
    explicit MyDeviceListenerCallback(const char *info)
    {
        strcpy(contextInfo, info);
    }

    /**
     * Destructor - made virtual to support correct polymorphic
     * deletion of child classes if required.
     */
    virtual ~MyDeviceListenerCallback()
    {
    }

    /**
     * Custom user implementation of the Call-back function which will be 
     * called to provide notification of when a device has been added.
     *
     * @param pDriver A pointer to a Driver object which can be used to obtain
     *                information about the driver on which the device was 
     *                added.
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
     *                information about the device which was added.
     *
     * \note The value of this parameter may be NULL if the relevant device
     *       failed to be identified. Even if the value of this parameter is
     *       non-NULL there are cases where only limited information for the
     *       device may be available, e.g. if the device is no longer present.
     *
     * \note The pointer to the Device object provided will no longer be valid
     *       once the processing of the call-back function completes. It will 
     *       be automatically freed by the caller (the STAR-System API).
     */
    virtual void DeviceAdded(Driver *pDriver, Device *pDevice)
    {
        /* \todo Add your own code here - see sample below */
        cout << "Device added [in " << contextInfo << "]:" << endl;

        /* if valid driver object provided */
        if (pDriver)
        {
            /* display driver information */
            Driver::DriverType driverType = pDriver->GetDriverType();
            switch (driverType)
            {
            case Driver::DRIVER_TYPE_USB:
                cout << "Driver: USB" << endl;
                break;
            case Driver::DRIVER_TYPE_PCI:
                cout << "Driver: PCI" << endl;
                break;
            case Driver::DRIVER_TYPE_TCPIP:
                cout << "Driver: TCPIP" << endl;
                break;
            case Driver::DRIVER_TYPE_VIRTUAL:
                cout << "Driver: Virtual" << endl;
                break;
            case Driver::DRIVER_TYPE_INVALID:
                cout << "Driver: Invalid" << endl;
                break;
            default:
                cout << "Driver: Unknown" << endl;
                break;
            }
        }
        else
        {
            cout << "No driver information provided" << endl;
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
                cout << "Device: " << deviceName << endl;
            }
            else
            {
                cout << "Failed to obtain device name!" << endl;
            }
            cout << "Device ID: " << pDevice->GetDeviceID() << endl;
        }
        else
        {
            cout << "No device information provided" << endl;
        }
        cout << "" << endl;
    }

    /**
     * Custom user implementation of the Call-back function which will be 
     * called to provide notification of when a device has been removed.
     *
     * @param pDriver A pointer to a Driver object which can be used to obtain
     *                information about the driver on which the device was
     *                removed.
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
     * @param pDevice A pointer to a Driver object which can be used to obtain
     *                information about the device which was removed.
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
     */
    virtual void DeviceRemoved(Driver *pDriver, Device *pDevice)
    {
        /* \todo Add your own code here - see sample below */
        cout << "Device removed [in " << contextInfo << "]:" << endl;

        /* if valid driver object provided */
        if (pDriver)
        {
            /* display driver information */
            Driver::DriverType driverType = pDriver->GetDriverType();
            switch (driverType)
            {
            case Driver::DRIVER_TYPE_USB:
                cout << "Driver: USB" << endl;
                break;
            case Driver::DRIVER_TYPE_PCI:
                cout << "Driver: PCI" << endl;
                break;
            case Driver::DRIVER_TYPE_TCPIP:
                cout << "Driver: TCPIP" << endl;
                break;
            case Driver::DRIVER_TYPE_VIRTUAL:
                cout << "Driver: Virtual" << endl;
                break;
            case Driver::DRIVER_TYPE_INVALID:
                cout << "Driver: Invalid" << endl;
                break;
            default:
                cout << "Driver: Unknown" << endl;
                break;
            }
        }
        else
        {
            cout << "No driver information provided" << endl;
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
                cout << "Device: " << deviceName << endl;
            }
            else
            {
                cout << "Failed to obtain device name!" << endl;
            }
            cout << "Device ID: " << pDevice->GetDeviceID() << endl;
        }
        else
        {
            cout << "No device information provided" << endl;
        }

        cout << "" << endl;
    }
};

#endif /* MY_DEVICE_LISTENER_H */

