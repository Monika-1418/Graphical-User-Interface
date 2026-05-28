/**
 * \file timestamp_raw_counters.hpp
 *
 * \brief Holds timestamp raw counter data.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds timestamp raw counter data.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 *
 * \ingroup CPP_STREAM_ITEMS
 */

#ifndef __TIMESTAMP_RAW_COUNTERS_CPP__
#define __TIMESTAMP_RAW_COUNTERS_CPP__

#include "star-api.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace streamitems {

/**
* Holds the raw counter values for a timestamp.
*/
class TimestampRawCounters
{
private:
    /** the start sync pulse count value */
    U32 timestampStartSyncPulseCount;

    /** the start clock cycle count value */
    U32 timestampStartClockCycleCount;

    /** the start total cycle count value */
    U32 timestampStartTotalCycleCount;

    /** the end sync pulse count value */
    U32 timestampEndSyncPulseCount;

    /** the end clock cycle count value */
    U32 timestampEndClockCycleCount;

    /* the end total cycle count value */
    U32 timestampEndTotalCycleCount;

    /** the timestamp clock frequency value */
    U32 timestampClockFrequency;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    TimestampRawCounters() : timestampStartSyncPulseCount(0), 
        timestampStartClockCycleCount(0), timestampStartTotalCycleCount(0), 
        timestampEndSyncPulseCount(0), timestampEndClockCycleCount(0), 
        timestampEndTotalCycleCount(0), timestampClockFrequency(0)
    {
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param startSyncPulseCount the input start sync pulse count value.
    * @param startClockCycleCount the input start clock cycle count value.
    * @param startTotalCycleCount the input start total clock cycle count value.
    * @param endSyncPulseCount the input end sync pulse count value.
    * @param endClockCycleCount the input end clock cycle count value.
    * @param endTotalCycleCount the input end total cycle count value.
    * @param clockFrequency the input clock frequency value.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    */
    TimestampRawCounters(U32 startSyncPulseCount, 
        U32 startClockCycleCount, U32 startTotalCycleCount, 
        U32 endSyncPulseCount, U32 endClockCycleCount, 
        U32 endTotalCycleCount, U32 clockFrequency) :
        timestampStartSyncPulseCount(startSyncPulseCount), 
        timestampStartClockCycleCount(startClockCycleCount), 
        timestampStartTotalCycleCount(startTotalCycleCount), 
        timestampEndSyncPulseCount(endSyncPulseCount), 
        timestampEndClockCycleCount(endClockCycleCount), 
        timestampEndTotalCycleCount(endTotalCycleCount), 
        timestampClockFrequency(clockFrequency)
    {
    }

    /**
    * Copy constructor - to allow copies of TimestampRawCounters objects to be 
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    TimestampRawCounters(const TimestampRawCounters& timestampRawCounters)
    {
        /* copy contents of object being copied into this object */
        timestampStartSyncPulseCount = 
            timestampRawCounters.timestampStartSyncPulseCount;
        timestampStartClockCycleCount = 
            timestampRawCounters.timestampStartClockCycleCount;
        timestampStartTotalCycleCount = 
            timestampRawCounters.timestampStartTotalCycleCount;
        timestampEndSyncPulseCount = 
            timestampRawCounters.timestampEndSyncPulseCount;
        timestampEndClockCycleCount = 
            timestampRawCounters.timestampEndClockCycleCount;
        timestampEndTotalCycleCount = 
            timestampRawCounters.timestampEndTotalCycleCount;
        timestampClockFrequency = 
            timestampRawCounters.timestampClockFrequency;
    }

    /**
    * Overloaded assignment operator - to allow instances of 
    * TimestampRawCounters objects to have their contents assigned to each 
    * other.
    *
    */
    TimestampRawCounters& operator=(
        const TimestampRawCounters& timestampRawCounters)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &timestampRawCounters)
            return *this;

        /* copy contents of object being assigned into this object */
        timestampStartSyncPulseCount = 
            timestampRawCounters.timestampStartSyncPulseCount;
        timestampStartClockCycleCount = 
            timestampRawCounters.timestampStartClockCycleCount;
        timestampStartTotalCycleCount = 
            timestampRawCounters.timestampStartTotalCycleCount;
        timestampEndSyncPulseCount = 
            timestampRawCounters.timestampEndSyncPulseCount;
        timestampEndClockCycleCount = 
            timestampRawCounters.timestampEndClockCycleCount;
        timestampEndTotalCycleCount = 
            timestampRawCounters.timestampEndTotalCycleCount;
        timestampClockFrequency = 
            timestampRawCounters.timestampClockFrequency;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~TimestampRawCounters()
    {
        /* no resources to free at present */
    }

    /**
    * Returns the start sync pulse count value.
    *
    * @return start sync pulse count.
    */
    U32 GetStartSyncPulseCount()
    {
        return timestampStartSyncPulseCount;
    }

    /**
    * Returns the start clock cycle count value.
    *
    * @return start clock cycle count.
    */
    U32 GetStartClockCycleCount()
    {
        return timestampStartClockCycleCount;
    }

    /**
    * Returns the start total cycle count value.
    *
    * @return start total cycle count.
    */
    U32 GetStartTotalCycleCount()
    {
        return timestampStartTotalCycleCount;
    }

    /**
    * Returns the end sync pulse count value.
    *
    * @return end sync pulse count.
    */
    U32 GetEndSyncPulseCount()
    {
        return timestampEndSyncPulseCount;
    }

    /**
    * Returns the end clock cycle count value.
    *
    * @return end clock cycle count.
    */
    U32 GetEndClockCycleCount()
    {
        return timestampEndClockCycleCount;
    }

    /**
    * Returns the end total cycle count value.
    *
    * @return end total cycle count.
    */
    U32 GetEndTotalCycleCount()
    {
        return timestampEndTotalCycleCount;
    }

    /**
    * Returns the clock frequency value.
    *
    * @return clock frequency.
    */
    U32 GetClockFrequency()
    {
        return timestampClockFrequency;
    }
};

/* end namespace tags */
} /* streamitems */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

