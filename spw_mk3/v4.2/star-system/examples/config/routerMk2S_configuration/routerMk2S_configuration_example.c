/**
 * \file routerMk2S_configuration_example.c
 *
 * \brief Example usage of the Router Mk2S configuration API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example code demonstrating the use of the Router Mk2S
 * specific configuration functions.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
*/

#include <stdio.h>
#include "star-api.h"
#include "cfg_api_generic.h"
#include "ui.h"


int main()
{
    STAR_DEVICE_ID deviceID;
    STAR_CFG_FPGA_INFO fpgaInfo;
    char versionStr[STAR_CFG_MK2_VERSION_STR_MAX_LEN];
    char buildDateStr[STAR_CFG_MK2_BUILD_DATE_STR_MAX_LEN];
    int enabled;
    double precisionTxRate;

    STAR_DEVICE_TYPE aDeviceTypes[1];
    aDeviceTypes[0] = STAR_DEVICE_ROUTER_MK2S;

    /* Select device to configure */
    deviceID = STAR_UI_chooseDevice(aDeviceTypes, 1);
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

    /* Test for precision transmit rate */
    CFG_getPrecisionTransmitRateEnabled(deviceID, &enabled);
    printf("\nPrecision transmit rate is %s", enabled ? "enabled" : "disabled");

    /* Enable precision transmit rate */
    CFG_enablePrecisionTransmitRate(deviceID);

    /* Wait until precision transmit rate is in use. This can sometimes take
       250us */
    do
    {
        CFG_getPrecisionTransmitRateInUse(deviceID, &enabled);
    }
    while(!enabled);

    /* Get current precision transmit rate */
    CFG_getPrecisionTransmitRate(deviceID, &precisionTxRate);

    printf("\nPrecision transmit rate is %f Mbit/s", precisionTxRate);

    /* Set precision transmit rate */
    CFG_setPrecisionTransmitRate(deviceID, 123.4);
    CFG_getPrecisionTransmitRate(deviceID, &precisionTxRate);
    printf("\nPrecision transmit rate is %f Mbit/s", precisionTxRate);


    return 0;
}
