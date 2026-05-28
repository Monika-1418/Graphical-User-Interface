/**
 * \file time_code_example.cpp
 *
 * \brief Provides implementations of class(es) which provide examples of using
 *        time-codes in the STAR-System C++ API.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides implementations of class(es) which provide examples of using
 * time-codes in the STAR-System C++ API, e.g. transmitting and receiving time-,
 * codes, enabling and disabling a time-code master, setting the period of the
 * time-code master.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "time_code_example.h"
#include "device_selecter.h"
#include "device_configuration.hpp"
#include "transmit_operation.hpp"
#include "time_code.hpp"
#include "receive_operation.hpp"
#include "device_type_resolver.h"
#include "cpp_api_macros.hpp"
#include <iostream>

using namespace stardundee::com::starsystem::deviceconfig;
using namespace stardundee::com::starsystem::deviceconfig::mk2devices;
using namespace stardundee::com::starsystem::transferoperations;
using namespace stardundee::com::starsystem::streamitems;
using namespace stardundee::com::starsystem::general;
using namespace std;

/** initialise static members of MyTimeCodeListener class **/
/* don't resubmit receives when they complete */
int MyTimeCodeListener::RECEIVE_MORE_ITEMS = 0;
/* only repeat receives up to 100 times */
int MyTimeCodeListener::RECEIVE_LIMIT = 100;
/* set count of times a receive has been repeated to zero */
int MyTimeCodeListener::RECEIVE_COUNT = 0;

/**
 * Default constructor.
 */
TimeCodeExample::TimeCodeExample() : currentTimecode(0),
                                     receiveStarted(0),
                                     pTimeCodeListener(NULL)
{
}

/**
 * Destructor.
 */
TimeCodeExample::~TimeCodeExample()
{
    /* if receive channel still open */
    if (receiveChannel.IsOpen())
    {
        /* close it */
        receiveChannel.CloseChannel();
    }

    /* un-register time-code listener if required */
    if (pTimeCodeListener)
    {
        if (!pTimeCodeListener->Unregister())
        {
            cout << "TimeCodeExample::~TimeCodeExample - failed to unregister"
                 " time-code listener" << endl;
        }
        delete pTimeCodeListener;
    }

    /* dispose of receive operation if required */
    if (receiveStarted)
        receiveOpForTimeCodes.DisposeTransferOperation();
}

/**
 * Runs the time-code example program.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv An array of command line arguments passed to the program.
 *
 * @return 0 if program completed normally, otherwise 1.
 */
int TimeCodeExample::Run()
{
    /* define variables required */
    char exitSelected = 0;
    char s[256];
    Device *pDevice = NULL;
    DeviceConfiguration *pDeviceConfiguration = NULL;

    /* get the device to be used */
    pDevice = DeviceSelector::ChooseDevice();

    /* if no device obtained */
    if (!pDevice)
    {
        /* report error and quit */
        cout << "Failed to obtain device to work with." << endl;
        return 0;
    }

    /* get the device configuration */
    pDeviceConfiguration = pDevice->GetDeviceConfiguration();

    /* if failed to obtain the device configuration */
    if (!pDeviceConfiguration)
    {
        /* report error, clean up and quit" */
        cout << "Failed to obtain device configuration." << endl;
        delete pDevice;
        return 0;
    }

    /* enable time-code forwarding on all ports of the device */
    if (!pDeviceConfiguration->SetTimeCodeDistributionPorts(0x00000fff))
    {
        /* failed to enable forwarding */
        cout << "Failed to enable time-code distribution on all ports." << endl;
    }

    do
    {
        /* display  Menu */
        cout << "\n" << endl;
        cout << "Select option:" << endl;
        cout << "  0: Exit" << endl;
        cout << "  1: Transmit a time-code" << endl;
        cout << "  2: Set period of time-code master" << endl;
        cout << "  3: Enable time-code master (cannot be done while receiving)" << endl;
        cout << "  4: Disable time-code master (cannot be done while receiving)" << endl;
        cout << "  5: Start receiving time-codes" << endl;
        cout << "  6: Stop receiving time-codes" << endl;
        cout << "  7: Enable external time-code selection (cannot be done while"
             " receiving)" << endl;
        cout << "  8: Disable external time-code selection (cannot be done while"
             " receiving)" << endl;
        cout << "" << endl;

        /* read in the operation type */
        fgets(s, 256, stdin);
        cout << "" << endl;
        switch (s[0])
        {
            case '0':
                exitSelected = 1;
                break;

            case '1':
                TransmitTimecode(pDevice);
                break;

            case '2':
                SetTimecodePeriod(pDevice);
                break;

            case '3':
                EnableTimecodeMaster(pDevice);
                break;

            case '4':
                DisableTimecodeMaster(pDevice);
                break;

            case '5':
                StartReceivingTimecodes(pDevice);
                break;

            case '6':
                StopReceivingTimecodes();
                break;

            case '7':
                EnableExternalTimecodeSelection(pDevice);
                break;

            case '8':
                DisableExternalTimecodeSelection(pDevice);
                break;

            default:
                cout << "Invalid menu option selected: " << (char)s[0] << endl;
        }

    } while (!exitSelected);

    /* display end of program */
    cout << "Exiting" << endl;

    /* clean up */
    delete pDeviceConfiguration;
    delete pDevice;

    /* end program */
    return 0;
}

/**
 * Transmit a time-code on the given device.
 *
 * @param pDevice The device to transmit a time-code on.
 */
void TimeCodeExample::TransmitTimecode(Device *pDevice)
{
    /* if a valid device hasn't been provided */
    if (!pDevice)
    {
        /* report error and quit */
        cout << "A valid device hasn't been provided to transmit the time-code"
             " on." << endl;
        return;
    }

    /* Open channel 0 on the device to transmit the time-code.
     *
     * \note STAR-System devices currently only support transmitting and
     * receiving time-codes on channel 0.
     */
    Channel transmitChannel;
    transmitChannel = pDevice->OpenChannel(Channel::DIRECTION_OUT, 0,
                                           0);

    /* if transmit channel was opened */
    if(transmitChannel.IsOpen())
    {
        TransferOperation::TransferStatus status;
        TransmitOperation transmitOp;
        TimeCode timeCode;
        int result;

        /* get pointer to time-code required for transmit operation */
        TimeCode *pTimeCode = &timeCode;

        /* create a time-code */
        result = timeCode.CreateTimeCode(currentTimecode);

        /* if time-code was created */
        if (result)
        {
            /* create a transmit operation to transmit the time-code */
            result = transmitOp.CreateTxOperation((StreamItem **)&pTimeCode, 1);

            /* if transmit op created successfully */
            if (result)
            {
                /* start transmitting the time-code */
                transmitChannel.SubmitTransferOperation(&transmitOp);

                /* wait indefinitely for transfer to complete */
                status = transmitOp.WaitOnTransferOperationCompletion(
                                        TransferOperation::WAIT_INDEFINITELY);

                /* if time-code was transmitted successfully */
                if (status == TransferOperation::TRANSFER_STATUS_COMPLETE)
                {
                    /* print success message */
                    cout << "Time-code " << (int)currentTimecode << " transmitted successfully." << endl;
                }
                else
                {
                    /* print error message */
                    cout << "Error transmitting time-code " << (int)currentTimecode;
                    cout << " status = " << status << endl;
                }

                /* dispose of the transmit operation */
                transmitOp.DisposeTransferOperation();
            }
            else
            {
                cout << "Failed to create transmit-op to transmit time-code." << endl;
            }

            /* destroy time-code */
            timeCode.DestroyTimeCode();

            /* move to the next time-code value */
            currentTimecode++;
            if (currentTimecode == 64)
            {
                currentTimecode = 0;
            }
        }
        else
        {
            /* report failure to create time-code */
            cout << "Failed to create time-code to transmit." << endl;
        }

        /* close the transmit channel */
        transmitChannel.CloseChannel();
    }
    else
    {
        /* report: failed to open channel */
        cout << "Failed to open channel to transmit time-code on." << endl;
    }
}

/**
 * Start receiving time-codes on the specified device.
 *
 * @param pDevice The device to receive time-codes on.
 */
void TimeCodeExample::StartReceivingTimecodes(Device *pDevice)
{
    /* if a valid device hasn't been provided */
    if (!pDevice)
    {
        /* report error and quit */
        cout << "A valid device hasn't been provided to receive time-codes"
             " on." << endl;
        return;
    }

    /* if receive operation already set up to receive time-codes */
    if (receiveStarted)
    {
        cout << "Already receiving time-codes" << endl;
        return;
    }

    /* Open channel 0 on the device to receive the time-code.
     *
     * \note STAR-System devices currently only support transmitting and
     * receiving time-codes on channel 0
     */
    receiveChannel = pDevice->OpenChannel(Channel::DIRECTION_IN, 0, 1);

    /* if receive channel was opened */
    if(receiveChannel.IsOpen())
    {
        /* create a receive operation to receive 1 time-code */
        int result = receiveOpForTimeCodes.CreateRxOperation(1,
                                           ReceiveOperation::RECEIVE_TIMECODES);

        /* if receive op created */
        if (result)
        {
            /* register a call-back object to receive notifications of transfer
             * operation completion events on the receive operation
             */
            timeCodeListenerCallbackObj.SetTransferOperation(
                                                       &receiveOpForTimeCodes);
            timeCodeListenerCallbackObj.SetChannel(&receiveChannel);
            pTimeCodeListener =
                STARSystem::RegisterTransferCompletionListener(
                                                 &receiveOpForTimeCodes,
                                                 &timeCodeListenerCallbackObj);

            /* if failed to register call-back listener */
            if (!pTimeCodeListener)
            {
                /* report error */
                cout << "Failed to register time-code listener for receive op" << endl;

                /* dispose of the receive op */
                receiveOpForTimeCodes.DisposeTransferOperation();

                /* close the receive channel */
                receiveChannel.CloseChannel();
            }
            else
            {

                /* enable repeats/resubmits of the receive operation which is
                 * being used to receive time-codes in the call-back object
                 * which will receive notifications of when receive operations
                 * have completed (otherwise only the the first time-code
                 * emitted may be received).
                 */
                MyTimeCodeListener::RECEIVE_MORE_ITEMS = 1;

                /* if a device emits time-codes as a time-code master an
                 * infinite number of time-codes may be received (until the
                 * time-code master is disabled). For testing purposes, impose
                 * a limit on the number of time-codes to receive.
                 */
                MyTimeCodeListener::RECEIVE_LIMIT = 50;

                /* record that receive has started */
                receiveStarted = true;

                /* submit the receive operation so that time-codes can now
                 * start being received on the receive channel.
                 */
                receiveChannel.SubmitTransferOperation(&receiveOpForTimeCodes);

                /* notify user that receive has started */
                cout << "Started receiving time-codes" << endl;
            }
        }
        else
        {
            /* report failure to create receive op */
            cout << "Failed to create receive-op to receive the time-code." << endl;

            /* close the receive channel */
            receiveChannel.CloseChannel();
        }
    }
    else
    {
        /* report: failed to open channel */
        cout << "Failed to open channel to receive time-codes on." << endl;
    }
}

/**
 * Stop receiving time-codes.
 */
void TimeCodeExample::StopReceivingTimecodes()
{
    /* if no receive operation has been set up to receive time-codes yet */
    if (!receiveStarted)
    {
        cout << "Receiving of time-codes hasn't been initiated yet" << endl;
    }

    /** perform clean-up steps anyway **/

    /* close the receive channel */
    receiveChannel.CloseChannel();

    /* if time-code lisenter exists */
    if (pTimeCodeListener)
    {
        /* un-register it and remove it */
        if (!pTimeCodeListener->Unregister())
        {
            cout << "TimeCodeExample::StopReceivingTimecodes - failed to"
                 " unregister time-code listener" << endl;
        }
        delete pTimeCodeListener;
        pTimeCodeListener = NULL;
    }

    /* dispose of the receive op */
    receiveOpForTimeCodes.DisposeTransferOperation();

    /* record that the receive has stopped */
    receiveStarted = false;

    cout << "Stopped receiving time-codes" << endl;
}

/**
 * Set the time-code period for the given device, on the basis that it is a
 * time-code master.
 *
 * @param pDevice The device to set the time-code period on.
 */
void TimeCodeExample::SetTimecodePeriod(Device *pDevice)
{
    char s[256];
    Mk2DeviceConfiguration *pMk2DeviceConfiguration = NULL;
    U32 period;

    /* if device isn't a Mk2 device */
    if (!DeviceTypeResolver::IsMk2Device(pDevice))
    {
        /* error: time-code period only available for Mk2 devices */
        cout << "Device is not a Mk2 device. Cannot access time-code period" << endl;
        return;
    }

    /* get the device configuration */
    pMk2DeviceConfiguration =
                    (Mk2DeviceConfiguration*)pDevice->GetDeviceConfiguration();

    /* if failed to obtain device configuration */
    if (!pMk2DeviceConfiguration)
    {
        /* report error and quit */
        cout << "Failed to obtain device configuration." << endl;
        return;
    }

    /* get the current time-code period */
    if (!pMk2DeviceConfiguration->GetTimeCodePeriod(&period))
    {
        /* report error, clean up and quit */
        cout << "Failed to obtain time-code period for device." << endl;
        delete pMk2DeviceConfiguration;
        return;
    }

    /* display the current time-code period */
    cout << "Current time-code period: " << (U32)period << " microseconds" << endl;

    /* read in the time-code period to be set */
    cout << "Please enter the time-code period in microseconds: ";
    if (!fgets(s, 256, stdin))
    {
        cout << "No period specified." << endl;
    }
    else
    {
        int status = sscanf(s, "%u", &period);
        if (!status)
        {
            cout << "Invalid time-code period specified." << endl;
        }
        else
        {
            /* Set the time-code period */
            if (pMk2DeviceConfiguration->SetTimeCodePeriod(period))
            {
                cout << "Time-code period set to " << (U32)period;
            }
            else
            {
                cout << "Error setting time-code period" << endl;
            }
        }
    }

    /* free the device configuration */
    delete pMk2DeviceConfiguration;
}

/**
 * Enable the given device as a time-code master.
 *
 * @param pDevice The device to enable as a time-code master.
 */
void TimeCodeExample::EnableTimecodeMaster(Device *pDevice)
{
    Mk2DeviceConfiguration *pMk2DeviceConfiguration = NULL;

    /* if device isn't a Mk2 device */
    if (!DeviceTypeResolver::IsMk2Device(pDevice))
    {
        /* error: can only enable a Mk2 device as a time-code master */
        cout << "Device is not a Mk2 device. Cannot enable as time-code master" << endl;
        return;
    }

    /* get the device configuration */
    pMk2DeviceConfiguration =
                    (Mk2DeviceConfiguration*)pDevice->GetDeviceConfiguration();

    /* if failed to obtain device configuration */
    if (!pMk2DeviceConfiguration)
    {
        /* report error and quit */
        cout << "Failed to obtain device configuration." << endl;
        return;
    }

    /* enable the device as a time-code master */
    if (pMk2DeviceConfiguration->EnableAsTimeCodeMaster())
    {
        cout << "Device enabled as a time-code master" << endl;
    }
    else
    {
        cout << "Error enabling device as a time-code master" << endl;
    }

    /* free the device configuration */
    delete pMk2DeviceConfiguration;
}

/**
 * Disable the given device as a time-code master.
 *
 * @param pDevice The device to disable as a time-code master.
 */
void TimeCodeExample::DisableTimecodeMaster(Device *pDevice)
{
    Mk2DeviceConfiguration *pMk2DeviceConfiguration = NULL;

    /* if device isn't a Mk2 device */
    if (!DeviceTypeResolver::IsMk2Device(pDevice))
    {
        /* error: can only disable a Mk2 device as a time-code master */
        cout << "Device is not a Mk2 device. Cannot disable as time-code master" << endl;
        return;
    }

    /* get the device configuration */
    pMk2DeviceConfiguration =
                    (Mk2DeviceConfiguration*)pDevice->GetDeviceConfiguration();

    /* if failed to obtain device configuration */
    if (!pMk2DeviceConfiguration)
    {
        /* report error and quit */
        cout << "Failed to obtain device configuration." << endl;
        return;
    }
    /* disable the device as a time-code master */
    if (pMk2DeviceConfiguration->DisableAsTimeCodeMaster())
    {
        cout << "Device disabled as a time-code master" << endl;
    }
    else
    {
        cout << "Error disabling device as a time-code master" << endl;
    }

    /* free the device configuration */
    delete pMk2DeviceConfiguration;
}

/**
 * Enable external time-code selection on the given device.
 *
 * @param pDevice The device to enable external time-code selection on.
 */
void TimeCodeExample::EnableExternalTimecodeSelection(Device *pDevice)
{
    Mk2DeviceConfiguration *pMk2DeviceConfiguration = NULL;

    /* if device isn't a Mk2 device */
    if (!DeviceTypeResolver::IsMk2Device(pDevice))
    {
        /* error: can only enable external time-code selection on a Mk2
         * device
         */
        cout << "Device is not a Mk2 device. Cannot enable external time-code"
             " selection." << endl;
        return;
    }

    /* get the device configuration */
    pMk2DeviceConfiguration =
                    (Mk2DeviceConfiguration*)pDevice->GetDeviceConfiguration();

    /* if failed to obtain device configuration */
    if (!pMk2DeviceConfiguration)
    {
        /* report error and quit */
        cout << "Failed to obtain device configuration." << endl;
        return;
    }

    /* Enable external time-code selection for the device */
    if (pMk2DeviceConfiguration->EnableExternalTimeCodeSelection())
    {
        cout << "External time-code selection enabled for the device" << endl;
    }
    else
    {
        cout << "Error enabling external time-code selection for the device" << endl;
    }

    /* free the device configuration */
    delete pMk2DeviceConfiguration;
}

/**
 * Disable external time-code selection on the given device.
 *
 * @param pDevice The device to disable external time-code selection on.
 */
void TimeCodeExample::DisableExternalTimecodeSelection(Device *pDevice)
{
    Mk2DeviceConfiguration *pMk2DeviceConfiguration = NULL;

    /* if device isn't a Mk2 device */
    if (!DeviceTypeResolver::IsMk2Device(pDevice))
    {
        /* error: can only disable external time-code selection on a Mk2
         * device
         */
        cout << "Device is not a Mk2 device. Cannot disable external time-code"
             " selection." << endl;
        return;
    }

    /* get the device configuration */
    pMk2DeviceConfiguration =
                    (Mk2DeviceConfiguration*)pDevice->GetDeviceConfiguration();

    /* if failed to obtain device configuration */
    if (!pMk2DeviceConfiguration)
    {
        /* report error and quit */
        cout << "Failed to obtain device configuration." << endl;
        return;
    }

    /* Disable external time-codes selection for the device */
    if (pMk2DeviceConfiguration->DisableExternalTimeCodeSelection())
    {
        cout << "External time-code selection disabled for the device" << endl;
    }
    else
    {
        cout << "Error disabling external time-code selection for the device" << endl;
    }

    /* free the device configuration */
    delete pMk2DeviceConfiguration;
}

