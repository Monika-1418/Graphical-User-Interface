/**
 * \file my_driver_listener.h
 *
 * \brief Provides an example of a custom DriverListenerCallback, required when
 *        registering driver listeners with the STAR-System C++ API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides an example of a custom DriverListenerCallback, required when
 * registering driver listeners with the STAR-System C++ API.
 *
 * Users are expected to create their own class for receiving driver event
 * notifications, which must be derived as a child class of the following
 * C++ API class: DriverListenerCallback.
 *
 * Users must then provide their own custom implementations for the call-back
 * functions inherited from the parent class which will be automatically 
 * invoked by the STAR-System API when a driver is added or removed.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include <cstring>
#include <cstdio>
#include "driver_listener_callback.hpp"
#include "driver.hpp"
#include <iostream>

using namespace stardundee::com::starsystem::general;
using namespace std;

#ifndef MY_DRIVER_LISTENER_H
#define MY_DRIVER_LISTENER_H

/**
 * Define a custom DriverListenerCallback object.
 */
class MyDriverListenerCallback : public DriverListenerCallback
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
    MyDriverListenerCallback()
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
    explicit MyDriverListenerCallback(const char *info)
    {
        strcpy(contextInfo, info);
    }

    /**
     * Destructor - made virtual to support correct polymorphic
     * deletion of child classes if required.
     */
    virtual ~MyDriverListenerCallback()
    {
    }

    /**
     * Custom user implementation of the Call-back function which will be 
     * called to provide notification of when a driver has been added.
     *
     * @param pDriver A pointer to a Driver object which can be used to obtain
     *                information about the driver which was added.
     *
     * \note The value of this parameter may be NULL if the relevant driver
     *       failed to be identified. Even if the value of this parameter is
     *       non-NULL there are cases where only limited information for the
     *       driver may be available, e.g. if the driver is no longer present.
     *
     * \note The pointer to the Driver object provided will no longer be valid
     *       once the processing of the call-back function completes. It will 
     *       be automatically freed by the caller (the STAR-System API).
     */
    virtual void DriverAdded(Driver *pDriver)
    {
        /* \todo Add your own code here - see sample below */
        cout << "Driver added [in " << contextInfo << "]:" << endl;

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
            cout << "Driver ID: " << pDriver->GetDriverID() << endl;
            VersionInformation *pVersionInfo = pDriver->GetDriverVersion();
            if (pVersionInfo)
            {
                cout << "Driver version: v" << (int)pVersionInfo->GetMajor();
                cout << "." << (int)pVersionInfo->GetMinor();
                if (pVersionInfo->GetEdit())
                {
                    cout << "(" << (int)pVersionInfo->GetEdit() << ")";
                }
                if (pVersionInfo->GetPatch())
                {
                    cout << "p" << (int)pVersionInfo->GetPatch();
                }
                cout << "" << endl;

                /* free the version information */
                delete pVersionInfo;
            }
            else
            {
                cout << "Failed to obtain driver version" << endl;
            }
        }
        else
        {
            cout << "No driver information provided" << endl;
        }

        cout << "" << endl;
    }

    /**
     * Custom user implementation of the Call-back function which will be 
     * called to provide notification of when a driver has been removed.
     *
     * @param pDriver A pointer to a Driver object which can be used to obtain
     *                information about the driver which was removed.
     *
     * \note The value of this parameter may be NULL if the relevant driver
     *       failed to be identified. Even if the value of this parameter is
     *       non-NULL there are cases where only limited information for the
     *       driver may be available, e.g. if the driver is no longer present.
     *
     * \note The pointer to the Driver object provided will no longer be valid
     *       once the processing of the call-back function completes. It will 
     *       be automatically freed by the caller (the STAR-System API).
     */
    virtual void DriverRemoved(Driver *pDriver)
    {
        /* \todo Add your own code here - see sample below */
        cout << "Driver removed [in " << contextInfo << "]:" << endl;

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
            cout << "Driver ID: " << pDriver->GetDriverID() << endl;
            VersionInformation *pVersionInfo = pDriver->GetDriverVersion();
            if (pVersionInfo)
            {
                cout << "Driver version: v" << (int)pVersionInfo->GetMajor();
                cout << "." << (int)pVersionInfo->GetMinor();
                if (pVersionInfo->GetEdit())
                {
                    cout << "(" << (int)pVersionInfo->GetEdit() << ")";
                }
                if (pVersionInfo->GetPatch())
                {
                    cout << "p" << (int)pVersionInfo->GetPatch();
                }
                cout << "" << endl;

                /* free the version information */
                delete pVersionInfo;
            }
            else
            {
                cout << "Failed to obtain driver version" << endl;
            }
        }
        else
        {
            cout << "No driver information provided" << endl;
        }

        cout << "" << endl;
    }
};

#endif /* MY_DRIVER_LISTENER_H */

