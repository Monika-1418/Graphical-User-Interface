/**
 * \file brickMk2_configuration_example.c
 *
 * \brief Example usage of the Brick Mk2 Configuration API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example code demonstrating the use of the Brick Mk2
 * specific configuration functions.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
*/

#include <stdio.h>
#include "star-api.h"
#include "cfg_api_generic.h"
#include "ui.h"


int __cdecl main()
{
    STAR_DEVICE_ID deviceID;
    STAR_CFG_FPGA_INFO fpgaInfo;
    char versionStr[STAR_CFG_MK2_VERSION_STR_MAX_LEN];
    char buildDateStr[STAR_CFG_MK2_BUILD_DATE_STR_MAX_LEN];
    int enabled;
    U16 linkSpeed;

    STAR_CHANNEL_ID channelID;
    STAR_TRANSFER_OPERATION *linkEventRxOp;
    STAR_STREAM_ITEM *linkEvent;
    STAR_CFG_BRICK_MK2_ERRORS linkErrors;
    U32 newLinkSpeed;

    STAR_DEVICE_TYPE aDeviceTypes[2];
    aDeviceTypes[0] = STAR_DEVICE_BRICK_MK2;
    aDeviceTypes[1] = STAR_DEVICE_ROUTER_MK2S;

    /* Select device to configure */
    deviceID = STAR_UI_chooseDevice(aDeviceTypes, 2);
    if (!deviceID)
    {
        return 0;
    }

    STAR_getDeviceType(deviceID);

    /* Get hardware info*/
    CFG_getFPGAInfo(deviceID, &fpgaInfo);

    CFG_FPGAInfoToString(deviceID, &fpgaInfo, versionStr, buildDateStr);

    /* Display the hardware info*/
    printf("\nVersion: %s", versionStr);
    printf("\nBuildDate: %s", buildDateStr);

    /* Set general purpose register to 0xABCD */
    CFG_setGeneralPurpose(deviceID, 0xABCD);

    /* Flash the device's LEDs*/
    CFG_identify(deviceID);

    /* Set the device to be a time-code master*/
    CFG_enableTimeCodeMaster(deviceID);

    /* Set 2 second delay between time-codes */
    CFG_setTimeCodePeriod(deviceID, 2000000);

    /* Enable interface mode */
    CFG_enableInterfaceMode(deviceID);

    /* Enable interface mode on port 1 only */
    CFG_enableInterfaceModeOnPort(deviceID, 1);

    /* Disable interface mode on port 2 only */
    CFG_disableInterfaceModeOnPort(deviceID, 2);

    CFG_getInterfaceModeOnPortEnabled(deviceID, 2, &enabled);

    printf("\nInterface mode %s on port 2", enabled ? "enabled" : "disabled");

    /* Enable adding the source port number as a leading byte
       to received packets on ports 1 and 2 */

    CFG_enableIdentifySource(deviceID);
    CFG_enableIdentifySourceOnPort(deviceID, 1);
    CFG_enableIdentifySourceOnPort(deviceID, 2);

    /* Check if identify source is enabled for port 1 */
    CFG_getIdentifySourceOnPortEnabled(deviceID, 1, &enabled);
    printf("\nIdentify source %s on port 1", enabled ? "enabled" : "disabled");

    CFG_disableIdentifySourceOnPort(deviceID, 1);

    /* Set link speed for link 1 to 100 Mbit/s */
    CFG_setTransmitSignallingRate(deviceID, 1, 100);

    /* Get measured link speed */
    CFG_getMeasuredLinkSpeed(deviceID, 1, &linkSpeed);

    printf("\nMeasured Link Speed: %d Kbit/s",
            linkSpeed * STAR_CFG_MK2_LINK_SPEED_UNITS_KBPS);

    /* Inject an error */
    linkErrors.suppressFCT = 1;
    CFG_injectErrors(deviceID, 2, &linkErrors);

    /* Enable Link Speed Events */
    CFG_enableSpeedChangeEventsOnPort(deviceID, 1);

    /* To receive link speed events, we must open a connection to device
       channel 0. This will prevent configuration operations from being
       performed until we close this channel again. */

    channelID = STAR_openChannelToLocalDevice(deviceID,
                                              STAR_CHANNEL_DIRECTION_IN,
                                              0,
                                              0);

    linkEventRxOp = STAR_createRxOperation(1, STAR_RECEIVE_LINK_SPEED_EVENTS);

    STAR_submitTransferOperation(channelID, linkEventRxOp);

    /* Wait for up to 10 seconds for link speed event to occur */
    printf("\nWaiting for Link speed event...");
    STAR_waitOnTransferOperationCompletion(linkEventRxOp, 10000);

    /* Display new link speed */
    switch(STAR_getTransferOperationStatus(linkEventRxOp)){
    case STAR_TRANSFER_STATUS_COMPLETE:
        linkEvent = STAR_getTransferItem(linkEventRxOp, 0);
        newLinkSpeed = STAR_getLinkSpeedEventSpeed(
            (STAR_LINK_SPEED_EVENT*)(linkEvent->item));
        printf("\nNew Link Speed: %u Kbit/s", newLinkSpeed/1000);
        break;
    case STAR_TRANSFER_STATUS_STARTED:
    case STAR_TRANSFER_STATUS_NOT_STARTED:
        printf("\nLink speed event did not occur");
        STAR_cancelTransferOperation(linkEventRxOp);
        break;
    default:
        printf("\nError occurred while waiting for link speed event");
    }

    STAR_disposeTransferOperation(linkEventRxOp);
    STAR_closeChannel(channelID);

    CFG_disableSpeedChangeEventsOnPort(deviceID, 1);

    return 0;
}
