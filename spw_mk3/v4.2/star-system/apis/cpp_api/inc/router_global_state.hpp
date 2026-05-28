/**
 * \file router_global_state.hpp
 *
 * \brief Holds global settings for a router.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds global settings for a router.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __ROUTER_GLOBAL_STATE_CPP__
#define __ROUTER_GLOBAL_STATE_CPP__

#include "cfg_api_router_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Holds global settings for a router.
*/
class RouterGlobalState
{
private:
    /** struct required by STAR-API to hold a router's global state */
    STAR_CFG_ROUTER_GLOBAL_STATE routerGlobalState;

public:
    /** length of port timeout */
    enum PortTimeout
    {
        /** 60-80 us */
        PORT_TIMEOUT_100US = STAR_CFG_PORT_TIMEOUT_100US,
        /** ~1.3ms */
        PORT_TIMEOUT_1MS = STAR_CFG_PORT_TIMEOUT_1MS,
        /** ~10ms */
        PORT_TIMEOUT_10MS = STAR_CFG_PORT_TIMEOUT_10MS,
        /** ~82ms */
        PORT_TIMEOUT_100MS = STAR_CFG_PORT_TIMEOUT_100MS,
        /** ~1.3s */
        PORT_TIMEOUT_1S = STAR_CFG_PORT_TIMEOUT_1S
    };

    /** Timeout mode used by the router. This affects how blocked packets
      * will be handled. */
    enum TimeoutMode
    {
        /** Blocking allowed.
          *
          * When blocking mode is enabled packets will wait forever to be
          * routed unless the packet is routed to a port that is not started.
          * In this case the packet will be discarded.
          */
        TIMEOUT_MODE_BLOCKING = STAR_CFG_TIMEOUT_MODE_BLOCKING,
        /** Watchdog timer mode.
          *
          * When watchdog mode is enabled packets which are waiting to be
          * routed at source ports will be discarded after the timeout period.
          * Packet tails will also be discarded if the packet becomes blocked
          * for the timeout period. In this case the packet will be ended with
          * an EEP.
          */
        TIMEOUT_MODE_WATCHDOG = STAR_CFG_TIMEOUT_MODE_WATCHDOG
    };

    /**
    * Default constructor. Initialise object to default state.
    */
    RouterGlobalState()
    {
        routerGlobalState.disableOnSilence = 0;
        routerGlobalState.enableSelfAddressing = 0;
        routerGlobalState.startOnRequest = 0;
        routerGlobalState.timeoutMode = STAR_CFG_TIMEOUT_MODE_WATCHDOG;
        routerGlobalState.timeoutPeriod = STAR_CFG_PORT_TIMEOUT_1S;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    *
    * @param state A STAR_CFG_ROUTER_GLOBAL_STATE struct which contains the
    *              global state values to assign to this object.
    */
    explicit RouterGlobalState(STAR_CFG_ROUTER_GLOBAL_STATE state)
    {
        routerGlobalState.disableOnSilence = state.disableOnSilence;
        routerGlobalState.enableSelfAddressing = state.enableSelfAddressing;
        routerGlobalState.startOnRequest = state.startOnRequest;
        routerGlobalState.timeoutMode = state.timeoutMode;
        routerGlobalState.timeoutPeriod = state.timeoutPeriod;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param disableOnSilence Specifies whether links will be disabled after the
    *                         timeout period when data transfer completes.
    * @param enableSelfAddressing Specifies whether a packet can be routed out
    *                             of the port on which it arrived.
    * @param startOnRequest Specifies whether links will be automatically
    *                       started when they have data to transfer.
    * @param timeoutMode Specifies the timeout mode.
    * @param timeoutPeriod Specifies the timeout period.
    *
    */
    RouterGlobalState(char disableOnSilence, char enableSelfAddressing,
                      char startOnRequest, TimeoutMode timeoutMode,
                      PortTimeout timeoutPeriod)
    {
        routerGlobalState.disableOnSilence = disableOnSilence;
        routerGlobalState.enableSelfAddressing = enableSelfAddressing;
        routerGlobalState.startOnRequest = startOnRequest;
        routerGlobalState.timeoutMode = (STAR_CFG_TIMEOUT_MODE)timeoutMode;
        routerGlobalState.timeoutPeriod = (STAR_CFG_PORT_TIMEOUT)timeoutPeriod;
    }

    /**
    * Copy constructor - to allow copies of RouterGlobalState objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    RouterGlobalState(const RouterGlobalState& stateToCopyFrom)
    {
        /* copy contents of object being copied into this object */
        routerGlobalState.disableOnSilence =
            stateToCopyFrom.routerGlobalState.disableOnSilence;
        routerGlobalState.enableSelfAddressing =
            stateToCopyFrom.routerGlobalState.enableSelfAddressing;
        routerGlobalState.startOnRequest =
            stateToCopyFrom.routerGlobalState.startOnRequest;
        routerGlobalState.timeoutMode =
            stateToCopyFrom.routerGlobalState.timeoutMode;
        routerGlobalState.timeoutPeriod =
            stateToCopyFrom.routerGlobalState.timeoutPeriod;
    }

    /**
    * Overloaded assignment operator - to allow instances of RouterGlobalState
    * objects to have their contents assigned to each other.
    *
    */
    RouterGlobalState& operator=(const RouterGlobalState& stateBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &stateBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        routerGlobalState.disableOnSilence = stateBeingAssigned.routerGlobalState.disableOnSilence;
        routerGlobalState.enableSelfAddressing = stateBeingAssigned.routerGlobalState.enableSelfAddressing;
        routerGlobalState.startOnRequest = stateBeingAssigned.routerGlobalState.startOnRequest;
        routerGlobalState.timeoutMode = stateBeingAssigned.routerGlobalState.timeoutMode;
        routerGlobalState.timeoutPeriod = stateBeingAssigned.routerGlobalState.timeoutPeriod;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~RouterGlobalState()
    {
        /* no resources to free at present */
    }

    /**
    * Get whether links will be disabled after the timeout period when data
    * transfer completes.
    *
    * @return 1 if links will be disabled, otherwise 0.
    *
    */
    char IsDisableOnSilenceSet()
    {
        return routerGlobalState.disableOnSilence;
    }

    /**
    * Set whether links will be disabled after the timeout period when data
    * transfer completes.
    *
    * @param disableOnSilence 1 if links are to be disabled, otherwise 0.
    *
    */
    void SetDisableOnSilence(char disableOnSilence)
    {
        routerGlobalState.disableOnSilence = disableOnSilence;
    }

    /**
    * Get whether a packet can be routed out of the port on which it arrived.
    *
    * \note  This is for debugging purposes. If this is false and a packet is to
    *        be routed through the same port an address error is reported and
    *        the packet is discarded. If false, and a group adaptive routing
    *        packet is received (a packet which can be routed through two or
    *        more ports, dependent on the group adaptive routing table contents)
    *        which can be routed through the port it arrived on then the packet
    *        is routed through one of the other ports and not the port on which
    *        the packet arrived on. An address error is not reported.
    *
    * @return 1 if self addressing is enabled, otherwise 0.
    *
    */
    char IsSelfAddressingEnabled()
    {
        return routerGlobalState.enableSelfAddressing;
    }

    /**
    * Set whether a packet can be routed out of the port on which it arrived.
    *
    * \note  This is for debugging purposes. If this is false and a packet is to
    *        be routed through the same port an address error is reported and
    *        the packet is discarded. If false, and a group adaptive routing
    *        packet is received (a packet which can be routed through two or
    *        more ports, dependent on the group adaptive routing table contents)
    *        which can be routed through the port it arrived on then the packet
    *        is routed through one of the other ports and not the port on which
    *        the packet arrived on. An address error is not reported.
    *
    * @param enableSelfAddressing 1 if self addressing is to be enabled,
    *                             otherwise 0.
    *
    */
    void EnableSelfAddressing(char enableSelfAddressing)
    {
        routerGlobalState.enableSelfAddressing = enableSelfAddressing;
    }

    /**
    * Get whether links will be automatically started when they have data to
    * transfer.
    *
    * \note If the link cannot be started, packets are discarded after the
    *       timeout period.
    *
    * @return 1 if links will be automatically started, otherwise 0.
    *
    */
    char IsStartOnRequestSet()
    {
        return routerGlobalState.startOnRequest;
    }

    /**
    * Set whether links will be automatically started when they have data to
    * transfer.
    *
    * \note If the link cannot be started, packets are discarded after the
    *       timeout period.
    *
    * @param startOnRequest 1 if links are to be automatically started,
    *                       otherwise 0.
    *
    */
    void SetStartOnRequest(char startOnRequest)
    {
        routerGlobalState.startOnRequest = startOnRequest;
    }

    /**
    * Get the timeout mode used by the router. This affects how blocked packets
    * will be handled.
    *
    * @return The timeout mode.
    *
    */
    TimeoutMode GetTimeoutMode()
    {
        return (TimeoutMode)routerGlobalState.timeoutMode;
    }

    /**
    * Set the timeout mode used by the router. This affects how blocked packets
    * will be handled.
    *
    * @param timeoutMode The timeout mode to set.
    *
    */
    void SetTimeoutMode(TimeoutMode timeoutMode)
    {
        routerGlobalState.timeoutMode = (STAR_CFG_TIMEOUT_MODE)timeoutMode;
    }

    /**
    * Get the timeout period for ports on the router.
    *
    * @return The timeout period.
    *
    */
    PortTimeout GetTimeoutPeriod()
    {
        return (PortTimeout)routerGlobalState.timeoutPeriod;
    }

    /**
    * Set the timeout period for ports on the router.
    *
    * @param timeoutPeriod The timeout period.
    *
    */
    void SetTimeoutPeriod(PortTimeout timeoutPeriod)
    {
        routerGlobalState.timeoutPeriod = (STAR_CFG_PORT_TIMEOUT)timeoutPeriod;
    }

    /**
    * Return the STAR_CFG_ROUTER_GLOBAL_STATE struct which is held
    * internally by this object.
    *
    * @return A pointer to the STAR_CFG_ROUTER_GLOBAL_STATE structure which
    *         is held internally by this object.
    *
    * \note This is provided to support internal processing of other objects
    *       within the C++ API and would not normally require to be invoked by
    *       client applications using the API.
    */
    STAR_CFG_ROUTER_GLOBAL_STATE *GetAsSTAR_CFG_ROUTER_GLOBAL_STATE()
    {
        /* return pointer to the STAR_CFG_ROUTER_GLOBAL_STATE struct held
         * internally by this object. */
        return &routerGlobalState;
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

