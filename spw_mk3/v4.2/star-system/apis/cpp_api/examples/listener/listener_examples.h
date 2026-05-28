/**
 * \file listener_examples.h
 *
 * \brief Provides examples of registering various types of listeners for
 *        events such as devices being added or removed, channels being opened
 *        or closed, etc.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides examples of registering various types of listeners for events such
 * as devices being added or removed, channels being opened or closed, etc.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#ifndef LISTENER_EXAMPLES_H
#define LISTENER_EXAMPLES_H

/**
 * Provides examples of registering various types of listeners for events such
 * as devices being added or removed, channels being opened or closed, etc.
 */
class ListenerExamples
{
public:
    /**
     * Provides an example of registering a device listener to detect when
     * devices are added or removed for a specific driver.
     */
    void Example_RegisterDeviceListenerForDriver();

    /**
     * Provides an example of registering a channel listener to detect whenever
     * any channels are opened or closed.
     */
    void Example_RegisterChannelListener();

    /**
     * Provides an example of registering a channel listener to detect when
     * channels are opened or closed on a specific device.
     */
    void Example_RegisterChannelListenerForDevice();

    /**
     * Provides an example of registering a channel listener to detect when
     * channels are opened or closed on any device for a specific driver.
     */
    void Example_RegisterChannelListenerForDriver();

    /**
     * Provides an example of registering a device listener to detect whenever
     * any device is added or removed.
     */
    void Example_RegisterDeviceListener();

    /**
     * Provides an example of registering a device listener to detect when
     * a specific device is removed.
     */
    void Example_RegisterDeviceListenerForDevice();

    /**
     * Provides an example of registering a driver listener to detect whenever
     * any drivers are added or removed.
     */
    void Example_RegisterDriverListener();

    /**
     * Provides an example of registering transfer operation completion listeners
     * to detect when transfer operations complete, e.g. receive and transmit
     * operations.
     */
    void Example_RegisterTransferCompletionListener();

    int DoListenerExample();
};

#endif /* LISTENER_EXAMPLES_H */

