/**
 * \file link_events.h
 *
 * \brief Definition of class which provides an example of the link event
 *        features of STAR-System.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Definition of class which provides an example of the link event features of
 * STAR-System using the C++ API.
 *
 * Copyright &copy; 2019 STAR-Dundee Ltd
 */

#ifndef TIMESTMAP_TEST_H
#define TIMESTMAP_TEST_H

#include "device.hpp"
#include "remote_device.hpp"
#include "channel.hpp"
#include "device_configuration.hpp"
#include "mk2_device_configuration.hpp"
#include "hardware_info.hpp"
#include "receive_operation.hpp"

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::transferoperations;
using namespace stardundee::com::starsystem::deviceconfig;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices;

/**
 * Provides an example of the link event features of STAR-System using the
 * C++ API.
 */
class LinkEvents
{
private:
    /**
     * Returns true if the given device type only supports link events on
     * channel 0.
     *
     * @param deviceType The type of device to check.
     *
     * @return true if link events are only supported on channel 0 for device.
     */
    bool AreLinkEventsOnChannel0Only(U32 deviceType);

    /**
     * Prompts the user to choose which device to use for the test if more than
     * one device is available.
     *
     * @return A pointer to a Device object representing the selected
     *         device, or NULL if no devices are available or an error
     *         occurred during processing.
     *
     * \note The object returned must be freed when no longer required by using
     *       the C++ delete operator.
     */
    Device* ChooseDevice();

    /**
     * Waits for link events to be received on the specified channel and
     * receive operation.
     *
     * @param pChannel The channel to wait for events on.
     * @param pRxOperation The receive operation to receive the events.
     *
     * @return 0 if there were errors waiting for events.
     */
    int WaitForEvents(Channel *pChannel, ReceiveOperation *pRxOperation);

    /**
     * Sets the link speed divisor for the specified device and port.
     *
     * @param pDevice The device to set link speed for.
     * @param deviceType The type of device to set link speed for.
     * @param pHardwareInfo The hardware version information.
     * @param port The port to set link speed for.
     * @param divisor The divisor to use to set the link speed.
     *
     * @return 0 if there were errors setting the link speed.
     */
    int SetLinkSpeed(Device *pDevice, U32 deviceType,
        HardwareInfo *pHardwareInfo, U8 port, U8 divisor);

    /**
     * Prints the measured link speed for the specified device and port.
     *
     * @param pDevice The device to print link speed for.
     * @param port The port to print link speed for.
     * @param pText The text to include in the output.
     *
     * @return 0 if there were errors printing the link speed.
     */
    int PrintLinkSpeed(Device *pDevice, U8 port, const char *pText);

    /**
     * Manipulates the link state and speed in various ways to cause link
     * events to be generated.
     *
     * @param pDevice The device to test link events for.
     * @param pRemoteDevice A remote device is used on devices where channel 0
     *                      is open for events.
     * @param pRemoteConfig Device configuration instance for remote device.
     * @param deviceType The type of the device being tested.
     * @param channelNumber The channel number to receive events on.
     * @param pHardwareInfo The hardware version information.
     *
     * @return 0 if there were errors testing the link events.
     */
    int TestLinkEvents(Device *pDevice, Device *pRemoteDevice,
        Mk2DeviceConfiguration *pRemoteConfig, U32 deviceType,
        U8 channelNumber, HardwareInfo *pHardwareInfo);

    /**
     * Tests link events on the specified device with the device configuration
     * instance.
     *
     * @param pDevice The device to test.
     * @param pConfig The device configuration of the device.
     *
     * @return 0 if there were errors testing the device.
     */
    int TestDevice(Device *pDevice, Mk2DeviceConfiguration *pConfig);

public:
    int DoLinkEventsExample();
};

#endif /* TIMESTMAP_TEST_H */

