/**
 * \file api_test_app.c
 *
 * \brief Main program for STAR-API example functions.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Main program for STAR-Dundee example functions. Uncomment the example
 * function that you wish to run. The channels should be configured in
 * general.h to match the device and link configuration that you are using.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

#include "examples.h"

#if !defined(UNREFERENCED_PARAMETER)
    #define UNREFERENCED_PARAMETER(a)   ((void)(a))
#endif

#include <stdlib.h>

#include "star-api.h"

int __cdecl main(int argc, char *argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    STAR_setApplicationName("STAR-System API Test Application");

    /* Configure the transmit and receive channels in general.h
       Uncomment the example(s) below to run */

    //advancedAddressExample();
    //advancedContinuousReceiveExample();
    //advancedMultiplePacketExample();
    //advancedReceiveExample();
    //advancedSendAndReceiveExample();
    //advancedSendExample();
    //advancedTwoWayExample();
    //allVersionExample();
    //apiVersionExample();
    //applicationNameExample();
    //channelCallbackExample();
    //driverListExample();
    //firmwareVersionExample();
    //simpleReceiveExample();
    //simpleSendAndReceiveExample();
    //simpleSendExample();
    //simpleTwoWayExample();
    //timecodeExample();
    //transferCallbackExample();
    //transferOperationListSendExample();
    //transferOperationListSendReceiveExample();
    //transmitErrorsExample();

    return 0;
}
