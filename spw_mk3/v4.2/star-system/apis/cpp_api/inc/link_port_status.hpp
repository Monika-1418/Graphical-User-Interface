/**
 * \file link_port_status.hpp
 *
 * \brief Holds information about the status of a SpaceWire link.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Holds information about the status of a SpaceWire link.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_DEVICE_CONFIG
 */

#ifndef __DEVICE_LINK_PORT_STATUS_CPP__
#define __DEVICE_LINK_PORT_STATUS_CPP__

#include "port_status.hpp"
#include "cfg_api_router_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace deviceconfig {

/**
* Holds information about the status of a SpaceWire link.
*/
class LinkStatus : public PortStatus
{
public:
    /** the state of the interface state machine in the SpaceWire link */
    enum LinkState
    {
        /** error reset */
        LINK_STATE_ERROR_RESET = STAR_CFG_SPW_LINK_STATE_ERROR_RESET,
        /** error wait */
        LINK_STATE_ERROR_WAIT = STAR_CFG_SPW_LINK_STATE_ERROR_WAIT,
        /** ready */
        LINK_STATE_READY = STAR_CFG_SPW_LINK_STATE_READY,
        /** started */
        LINK_STATE_STARTED = STAR_CFG_SPW_LINK_STATE_STARTED,
        /** connecting */
        LINK_STATE_CONNECTING = STAR_CFG_SPW_LINK_STATE_CONNECTING,
        /** run */
        LINK_STATE_RUN = STAR_CFG_SPW_LINK_STATE_RUN,
        /** invalid value */
        LINK_STATE_INVALID = STAR_CFG_SPW_LINK_STATE_INVALID
    };

private:
     /** struct required by STAR-API to hold link status */
     STAR_CFG_SPW_LINK_STATUS linkPortStatus;

     /**
     * Copy the contents of the given STAR_CFG_SPW_LINK_STATUS struct into
     * the STAR_CFG_SPW_LINK_STATUS held by this object.
     *
     * @param linkStatus A reference to a STAR_CFG_SPW_LINK_STATUS struct to
     *                  copy from.
     */
     void CopyFromSTAR_CFG_SPW_LINK_STATUS(STAR_CFG_SPW_LINK_STATUS& linkStatus)
     {
         linkPortStatus.autoStart = linkStatus.autoStart;
         linkPortStatus.disable = linkStatus.disable;
         linkPortStatus.linkState = linkStatus.linkState;
         linkPortStatus.running = linkStatus.running;
         linkPortStatus.start = linkStatus.start;
         linkPortStatus.triState = linkStatus.triState;
     }

     /**
     * Copy the values of the member variables of the given LinkStatus
     * object into this object.
     *
     * @param linkStatus A reference to a LinkStatus object to copy from.
     *
     */
     void CopyFromLinkStatus(const LinkStatus& linkStatus)
     {
         linkPortStatus.autoStart = linkStatus.linkPortStatus.autoStart;
         linkPortStatus.disable = linkStatus.linkPortStatus.disable;
         linkPortStatus.linkState = linkStatus.linkPortStatus.linkState;
         linkPortStatus.running = linkStatus.linkPortStatus.running;
         linkPortStatus.start = linkStatus.linkPortStatus.start;
         linkPortStatus.triState = linkStatus.linkPortStatus.triState;
     }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    LinkStatus() : PortStatus()
    {
         linkPortStatus.autoStart = 0;
         linkPortStatus.disable = 0;
         linkPortStatus.linkState = STAR_CFG_SPW_LINK_STATE_INVALID;
         linkPortStatus.running = 0;
         linkPortStatus.start = 0;
         linkPortStatus.triState = 0;
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param linkStatus A STAR_CFG_SPW_LINK_STATUS struct which contains
    *              the link status values to assign to this object.
    *
    * \note It is not usually necessary to use this constructor, it is
    *       used to support internal processing within the C++ API.
    */
    explicit LinkStatus(STAR_CFG_SPW_LINK_STATUS& linkStatus) :
        PortStatus()
    {
        /* copy the contents of the given struct into the struct held by
         * this object */
        CopyFromSTAR_CFG_SPW_LINK_STATUS(linkStatus);
    }

    /**
    * Overloaded constructor. Initialise object to specified value(s).
    *
    * @param autoStart Specifies whether the link should auto start or not.
    * @param disabled Specifies whether the link is disabled or not.
    * @param running Specifies whether the link is currently running or not.
    * @param initiateStartUp Specifies whether the link will initiate start-up
    *                        or not.
    * @param triState Specifies whether the link is in tri-state mode.
    *
    */
    LinkStatus(char autoStart, char disabled, char running,
               char initiateStartUp, char triState) : PortStatus()
    {
        /* initialise the corresponding fields of the STAR_CFG_SPW_LINK_STATUS
         * struct held by this object with the values given.
         * NOTE: the value of the 'linkState' field cannot be set by the user
         * and is therefore set to a default value.
         */
         linkPortStatus.autoStart = autoStart;
         linkPortStatus.disable = disabled;
         linkPortStatus.linkState = STAR_CFG_SPW_LINK_STATE_INVALID;
         linkPortStatus.running = running;
         linkPortStatus.start = initiateStartUp;
         linkPortStatus.triState = triState;
    }

    /**
    * Copy constructor - to allow copies of LinkStatus objects to be
    * made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    LinkStatus(const LinkStatus& linkStatusToCopyFrom) : PortStatus()
    {
        /* copy contents of object being copied into this object */
        CopyFromLinkStatus(linkStatusToCopyFrom);
    }

    /**
    * Overloaded assignment operator - to allow instances of LinkStatus
    * objects to have their contents assigned to each other.
    *
    */
    LinkStatus& operator=(const LinkStatus& linkStatusBeingAssigned)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &linkStatusBeingAssigned)
            return *this;

        /* copy contents of object being assigned into this object */
        CopyFromLinkStatus(linkStatusBeingAssigned);

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Destructor. Free any resources created by object.
    */
    ~LinkStatus()
    {
        /* no resources to free at present */
    }

    /**
    * Get whether the SpaceWire link will auto start. As defined in the
    * SpaceWire standard, the SpaceWire port will wait until the other end of
    * the link tries to make a connection (sending NULLs) and will then
    * automatically start.
    *
    * @return 1 if auto start is on, otherwise 0.
    *
    */
    char IsAutoStart()
    {
        return linkPortStatus.autoStart;
    }

    /**
    * Set whether the SpaceWire link will auto start. As defined in the
    * SpaceWire standard, the SpaceWire port will wait until the other end of
    * the link tries to make a connection (sending NULLs) and will then
    * automatically start.
    *
    * @param autoStart 1 to set autostart, otherwise 0.
    *
    */
    void SetAutoStart(char autoStart)
    {
        linkPortStatus.autoStart = autoStart;
    }

    /**
    * Get whether the SpaceWire link is disabled. As defined in the
    * SpaceWire standard, the SpaceWire port will not start and will not respond
    * to any attempt to make a connection by the other end of the link.
    *
    * @return 1 if link is disabled, otherwise 0.
    *
    */
    char IsDisabled()
    {
        return linkPortStatus.disable;
    }

    /**
    * Set whether the SpaceWire link is disabled. As defined in the
    * SpaceWire standard, the SpaceWire port will not start and will not respond
    * to any attempt to make a connection by the other end of the link.
    *
    * @param disabled 1 if link is to be disabled, otherwise 0.
    *
    */
    void SetDisabled(char disabled)
    {
        linkPortStatus.disable = disabled;
    }

    /**
    * Get the current state of the link, e.g. ready, started, connecting.
    *
    * @return A LinkStatus::LinkState value indicating the current state of
    *         the link.
    *
    */
    LinkState GetLinkState()
    {
        return (LinkState)linkPortStatus.linkState;
    }

    /**
    * Set the current state of the link, e.g. ready, started, connecting.
    *
    * @param state A LinkStatus::LinkState value indicating the state to set
    *              for the link.
    *
    */
    void SetLinkState(LinkState state)
    {
        linkPortStatus.linkState = (STAR_CFG_SPW_LINK_STATE)state;
    }

    /**
    * Get whether the SpaceWire link is in the run state.
    *
    * @return 1 if link is in the run state, otherwise 0.
    *
    */
    char IsRunning()
    {
        return linkPortStatus.running;
    }

    /**
    * Set whether the SpaceWire link is to be in the run state.
    *
    * @param running 1 if link is to be in the run state, otherwise 0.
    *
    */
    void SetRunning(char running)
    {
        linkPortStatus.running = running;
    }

    /**
    * Get whether the SpaceWire link is set to initiate start-up as defined in
    * the SpaceWire standard: the SpaceWire port will try to make a connection
    * with the other end of the link.
    *
    * @return 1 if link is set to initiate start-up, otherwise 0.
    *
    */
    char InitiatesStartUp()
    {
        return linkPortStatus.start;
    }

    /**
    * Set whether the SpaceWire link is to be set to initiate start-up as
    * defined in the SpaceWire standard: the SpaceWire port will try to make a
    * connection with the other end of the link.
    *
    * @param initiateStartUp 1 if link is to be set to initiate start-up,
    *                        otherwise 0.
    *
    */
    void SetInitiateStartUp(char initiateStartUp)
    {
        linkPortStatus.start = initiateStartUp;
    }

    /**
    * Get whether the SpaceWire link is set to tri-state mode. When set the
    * SpaceWire link LVDS drivers are in tri-state mode.
    *
    * @return 1 if link is set to tri-state mode, otherwise 0.
    *
    */
    char IsTriState()
    {
        return linkPortStatus.triState;
    }

    /**
    * Set whether the SpaceWire link is to be set to tri-state mode. When set
    * the SpaceWire link LVDS drivers are in tri-state mode.
    *
    * @param triState 1 if link is to be set to tri-state mode, otherwise 0.
    *
    */
    void SetTriState(char triState)
    {
        linkPortStatus.triState = triState;
    }

    /**
    * Return the STAR_CFG_SPW_LINK_STATUS struct which is held
    * internally by this object.
    *
    * @return A pointer to the STAR_CFG_SPW_LINK_STATUS structure which
    *         is held internally by this object.
    *
    * \note This is provided to support internal processing of other objects
    *       within the C++ API and would not normally require to be invoked by
    *       client applications using the API.
    */
    STAR_CFG_SPW_LINK_STATUS *GetAsSTAR_CFG_SPW_LINK_STATUS()
    {
        /* return pointer to the STAR_CFG_SPW_LINK_STATUS struct held
         * internally by this object. */
        return &linkPortStatus;
    }
};

/* end namespace tags */
} /* deviceconfig */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

