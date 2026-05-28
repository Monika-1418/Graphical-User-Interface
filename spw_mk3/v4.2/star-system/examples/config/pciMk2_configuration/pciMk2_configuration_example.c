/**
 * \file pciMk2_configuration_example.c
 *
 * \brief Example usage of the PCI Mk2 configuration API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example code demonstrating the use of the PCI Mk2 specific
 * configuration functions.
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
    U32 signallingRate;

    STAR_DEVICE_TYPE aDeviceTypes[1];
    aDeviceTypes[0] = STAR_DEVICE_PCI_MK2;

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

    /* Disable interface mode on port 2 only */
    CFG_disableInterfaceModeOnPort(deviceID, 2);

    /* Enable adding the source port number as a leading byte
       to received packets on ports 1 and 3 */

    CFG_enableIdentifySource(deviceID);
    CFG_enableIdentifySourceOnPort(deviceID, 1);
    CFG_enableIdentifySourceOnPort(deviceID, 3);

    /* Set port 2 to have a link speed of 100 Mbit/s */
    CFG_setTransmitSignallingRate(deviceID, 2, 100);

    /* Read Link speed of port 1 */
    CFG_getTransmitSignallingRate(deviceID, 1, &signallingRate);
    printf("\nLink 1 transmit rate: %d Mbit/s", signallingRate);

    /* Inject a disconnect error on link 1 */
    CFG_injectError(deviceID, 1, SPW_ERROR_DISCONNECT);

    return 0;
}
