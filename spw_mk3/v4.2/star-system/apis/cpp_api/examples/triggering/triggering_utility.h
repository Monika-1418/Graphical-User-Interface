/**
 * \file triggering_utility.h
 *
 * \brief Declarations of utility functions for the Triggering API program.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Declarations of various utility functions used by the Triggering API example.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 */

#include "device.hpp"
#include "triggering_types.h"

using namespace stardundee::com::starsystem::general;

const U32 NUM_EXT_TRIGGERS_BRICK_MK3        = 2;
const U32 NUM_COUNTERS_BRICK_MK3            = 4;
const U32 NUM_PORTS_BRICK_MK3               = 2;
const U32 NUM_TIME_CODES_BRICK_MK3          = 1;
const U32 NUM_TRIGGERS_BRICK_MK3            = 8;

const U32 NUM_EXT_TRIGGERS_PXI_INTERFACE    = 4;
const U32 NUM_COUNTERS_PXI_INTERFACE        = 4;
const U32 NUM_PORTS_PXI_INTERFACE           = 4;
const U32 NUM_TIME_CODES_PXI_INTERFACE      = 1;
const U32 NUM_TRIGGERS_PXI_INTERFACE        = 8;

const U32 NUM_EXT_TRIGGERS_PXI_ROUTER       = 0;
const U32 NUM_COUNTERS_PXI_ROUTER           = 4;
const U32 NUM_PORTS_PXI_ROUTER              = 12;
const U32 NUM_TIME_CODES_PXI_ROUTER         = 1;
const U32 NUM_TRIGGERS_PXI_ROUTER           = 4;

const U32 NUM_EXT_TRIGGERS_PCIE             = 0;
const U32 NUM_COUNTERS_PCIE                 = 6;
const U32 NUM_PORTS_PCIE                    = 3;
const U32 NUM_TIME_CODES_PCIE               = 0;
const U32 NUM_TRIGGERS_PCIE                 = 6;

const U32 NUM_EXT_TRIGGERS_MAX              = 6;
const U32 NUM_COUNTERS_MAX                  = 6;
const U32 NUM_PORTS_MAX                     = 12;
const U32 NUM_TIME_CODES_MAX                = 1;
const U32 NUM_TRIGGERS_MAX                  = 8;

/**
 * Provides utilities associated with the triggering API example.
 */
class TriggeringUtility
{
private:
    /**
     * The type of trigger device.
     */
    TRIGGER_DEVICE m_triggerDevice;

public:
    /**
     * Creates a new triggering utility instance based on the given device type.
     *
     * @param triggerDevice the device type being tested.
     */
    TriggeringUtility(TRIGGER_DEVICE triggerDevice);

    /**
     * Resets all external triggers for the given device.
     *
     * @param pDevice the device to reset external triggers for.
     */
    void ResetExtTriggers(Device *pDevice);

    /**
     * Resets all counters for the given device.
     *
     * @param pDevice the device to reset counters for.
     */
    void ResetCounters(Device *pDevice);

    /**
     * Resets all events and actions on each port for the given device.
     *
     * @param pDevice the device to reset ports for.
     */
    void ResetPorts(Device *pDevice);

    /**
     * Resets each time-code interface for the given device.
     *
     * @param pDevice the device to reset time-codes for.
     */
    void ResetTimeCodes(Device *pDevice);

    /**
     * Resets each trigger for the given device.
     *
     * @param pDevice the device to reset triggers for.
     */
    void ResetTriggers(Device *pDevice);

    /**
     * Resets the triggering API configuration for the given device.
     *
     * @param pDevice the device to reset triggering API configuration for.
     */
    void Reset(Device *pDevice);

    /**
     * Queues a 4-byte packet to be transmitted over channel 1.
     *
     * @param pDevice the device to queue packets for.
     * @param count the number of packets to be queued.
     */
    void QueuePackets(Device *pDevice, U32 count);
};

