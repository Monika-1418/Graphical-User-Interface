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
#include "cfg_api_brick_mk2.h"
#include "cfg_api_router.h"
#include "ui.h"


int SetConfiguration(int linkNum)
{
    STAR_DEVICE_ID deviceID;
    STAR_CFG_MK2_HARDWARE_INFO hardwareInfo;
    char versionStr[STAR_CFG_MK2_VERSION_STR_MAX_LEN];
    char buildDateStr[STAR_CFG_MK2_BUILD_DATE_STR_MAX_LEN];
    int enabled;
    U16 linkSpeed;
    U8 timeCode;
    STAR_CHANNEL_ID channelID;
    STAR_TRANSFER_OPERATION *linkEventRxOp;
    STAR_STREAM_ITEM *linkEvent;
    STAR_CFG_BRICK_MK2_ERRORS linkErrors;
    U32 newLinkSpeed;
STAR_CFG_MK2_BASE_TRANSMIT_CLOCK clockRateParams;
STAR_CFG_MK2_BASE_TRANSMIT_CLOCK clockRateParamsRead;
    

    STAR_DEVICE_TYPE aDeviceTypes[3];
    aDeviceTypes[0] = STAR_DEVICE_BRICK_MK2;
    aDeviceTypes[1] = STAR_DEVICE_ROUTER_MK2S;
    aDeviceTypes[2] = STAR_DEVICE_BRICK_MK3;

linkNum=1;

    /* Select device to configure */
    deviceID = STAR_UI_chooseDevice(aDeviceTypes, 3);
    if (!deviceID)
    {
        return 0;
    }

    STAR_getDeviceType(deviceID);

    /* Get hardware info*/
    CFG_MK2_getHardwareInfo(deviceID, &hardwareInfo);

    CFG_MK2_hardwareInfoToString(hardwareInfo, versionStr, buildDateStr);

    /* Display the hardware info*/
    printf("\nVersion: %s", versionStr);
    printf("\nBuildDate: %s", buildDateStr);
   



    /* Set link speed for link 1 to 100Mbps (200/2)*/
    CFG_BRICK_MK2_setLinkClockFrequency(deviceID, linkNum, STAR_CFG_BRICK_MK2_LINK_FREQ_200);
 

    CFG_MK2_setLinkRateDivider(deviceID, linkNum, 16);

    /* Get measured link speed */
    
    CFG_BRICK_MK2_getMeasuredLinkSpeed(deviceID, linkNum, &linkSpeed);

    printf("\nMeasured Link Speed: %d Kbits/s",
            linkSpeed * STAR_CFG_BRICK_MK2_LINK_SPEED_UNITS_KBPS);
 
    
    CFG_disableInterfaceModeOnPort(deviceID,1);
    CFG_disableInterfaceModeOnPort(deviceID,2);
    
    
       // Enable time-code forwarding on all ports of the device  
    if (!CFG_ROUTER_setTimeCodeDistributionPorts(deviceID, 0x00000fff))
    {
        puts("\nUnable to enable time-code distribution on all ports");
    }
    
    else
      
    puts("\n\nEnabled time-code distribution on all ports.");
   
    
    if (CFG_MK2_enableExternalTimeCodeSelection(deviceID))
    {
        puts("\nExternal time-code selection enabled for the device.");
    }
    else
    {
        puts("\nError enabling external time-code selection for the device.");
    } 
    
    if(CFG_ROUTER_clearPortErrors(deviceID, 0)==0)
      puts("\nCouldn't Clear port errors for the device.");
    else
      puts("\nCleared port errors for the device.");
    
    if(CFG_ROUTER_getTimeCodeValue(deviceID, &timeCode)==0)
    {
      printf("\nCouldn't read time-code register.\n");
      return;
    }
    else
      printf("\nTimeCode register read.\n");



    return 0;
}
