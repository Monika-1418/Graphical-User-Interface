/**
 * \file time_code_example.h
 *
 * \brief Provides definitions of class(es) which provide examples of using
 *        time-codes in the STAR-System C++ API.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides definitions of class(es) which provide examples of using time-codes
 * in the STAR-System C++ API, e.g. transmitting and receiving time-codes,
 * enabling and disabling a time-code master, setting the period of the 
 * time-code master.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#ifndef TIME_CODE_EXAMPLE_H
#define TIME_CODE_EXAMPLE_H

#include "channel.hpp"
#include "device.hpp"
#include "receive_operation.hpp"
#include "my_time_code_listener.h"
#include "transfer_completion_listener.hpp"

#ifndef UNREFERENCED_PARAMETER
    #define UNREFERENCED_PARAMETER(a) ((void)(a))
#endif

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::channels;
using namespace stardundee::com::starsystem::transferoperations;

/**
 * Provide examples of using time-codes in the STAR-System C++ API, e.g.
 * transmitting and receiving time-codes, enabling and disabling a time-code
 * master, setting the period of the time-code master.
 */
class TimeCodeExample
{
private:
    /* The next time-code to be transmitted, when transmitting individual
     * time-codes.
     */
    U8 currentTimecode;

    /* The channel to receive time-codes on. */
    Channel receiveChannel;

    /* A receive operation, for receiving time-codes */
    ReceiveOperation receiveOpForTimeCodes;

    /* whether a receive for time-codes has been started */
    bool receiveStarted;

    /* A call-back object, for receiving notifications of when transfer
     * operations have completed.
     */
    MyTimeCodeListener timeCodeListenerCallbackObj;

    /* Identifies a listener, registered to receive notifications of when
     * transfer operations have completed.
     */
    TransferCompletionListener *pTimeCodeListener;

    /**
     * Transmit a time-code on the given device.
     *
     * @param pDevice The device to transmit a time-code on.
     */
    void TransmitTimecode(Device *pDevice);

    /**
     * Start receiving time-codes on the specified device.
     *
     * @param pDevice The device to receive time-codes on.
     */
    void StartReceivingTimecodes(Device *pDevice);

    /**
     * Stop receiving time-codes.
     */
    void StopReceivingTimecodes();

    /**
     * Set the time-code period for the given device, on the basis that it is a
     * time-code master.
     *
     * @param pDevice The device to set the time-code period on.
     */
    void SetTimecodePeriod(Device *pDevice);


    /**
     * Enable the given device as a time-code master.
     *
     * @param pDevice The device to enable as a time-code master.
     */
    void EnableTimecodeMaster(Device *pDevice);

    /**
     * Disable the given device as a time-code master.
     *
     * @param pDevice The device to disable as a time-code master.
     */
    void DisableTimecodeMaster(Device *pDevice);

    /**
     * Enable external time-code selection on the given device.
     *
     * @param pDevice The device to enable external time-code selection on.
     */
    void EnableExternalTimecodeSelection(Device *pDevice);

    /**
     * Disable external time-code selection on the given device.
     *
     * @param pDevice The device to disable external time-code selection on.
     */
    void DisableExternalTimecodeSelection(Device *pDevice);

public:
    /**
     * Default constructor.
     */
    TimeCodeExample();

    /**
     * Destructor.
     */
    ~TimeCodeExample();

    /**
     * Runs the time-code example program.
     *
     * @param argc The number of arguments passed to the program.
     * @param argv An array of command line arguments passed to the program.
     *
     * @return 0 if program completed normally, otherwise 1.
     */
    int Run();
};

#endif /* TIME_CODE_EXAMPLE_H */

