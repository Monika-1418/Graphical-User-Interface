/**
 * \file channel_callback_example.c
 *
 * \brief Demonstrates callback function for channel notifications.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Opens and closes various channels, printing messages when the channel
 * callback notifications are received.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#include "general.h"
#include "utilities.h"

#include "star-dundee_types.h"
#include "star-api.h"

void STAR_API_CC ChannelCallback(STAR_CHANNEL_LISTENER_ID
    channelListenerIdentifier, STAR_DRIVER_ID driverIdentifier,
    STAR_DEVICE_ID deviceIdentifier, STAR_CHANNEL_ID channelIdentifier,
    int channelOpened, U8 channelNumber, void *pContextInfo)
{
    /* Unreferenced parameters */
    UNREFERENCED_PARAMETER(pContextInfo);
    UNREFERENCED_PARAMETER(deviceIdentifier);
    UNREFERENCED_PARAMETER(driverIdentifier);
    UNREFERENCED_PARAMETER(channelListenerIdentifier);
    UNREFERENCED_PARAMETER(channelIdentifier);

    /* If channel was opened */
    if(channelOpened)
    {
        /* Print channel opened message */
        printf("Opened channel %u.\n", channelNumber);
    }
    else
    {
        /* Print channel closed message */
        printf("Closed channel %u.\n", channelNumber);
    }
}

void channelCallbackExample()
{
    /* Define first out channel */
    STAR_CHANNEL_ID outChannel1;

    /* Define second out channel */
    STAR_CHANNEL_ID outChannel2;

    /* Define first in channel */
    STAR_CHANNEL_ID inChannel1;

    /* Define second in channel */
    STAR_CHANNEL_ID inChannel2;

    /* Get first device */
    STAR_DEVICE_ID firstDevice = getFirstDevice();

    /* Register channel listener */
    STAR_CHANNEL_LISTENER_ID channelListener = STAR_registerChannelListener(
        ChannelCallback, NULL, 0);

    /* If channel listener was registered */
    if(channelListener)
    {
        /* Open first channel with out direction */
        outChannel1 = STAR_openChannelToLocalDevice(firstDevice,
            STAR_CHANNEL_DIRECTION_OUT, CHANNEL1, 0);

        /* Open second channel with out direction */
        outChannel2 = STAR_openChannelToLocalDevice(firstDevice,
            STAR_CHANNEL_DIRECTION_OUT, CHANNEL2, 0);

        /* Open first channel with in direction */
        inChannel1 = STAR_openChannelToLocalDevice(firstDevice,
            STAR_CHANNEL_DIRECTION_IN, CHANNEL1, 0);

        /* Open second channel with in direction */
        inChannel2 = STAR_openChannelToLocalDevice(firstDevice,
            STAR_CHANNEL_DIRECTION_IN, CHANNEL2, 0);

        /* Close first out channel */
        STAR_closeChannel(outChannel1);

        /* Close second out channel */
        STAR_closeChannel(outChannel2);

        /* Close first in channel */
        STAR_closeChannel(inChannel1);

        /* Close second in channel */
        STAR_closeChannel(inChannel2);

        /* Unregister channel listener */
        STAR_unregisterChannelListener(channelListener);
    }
}
