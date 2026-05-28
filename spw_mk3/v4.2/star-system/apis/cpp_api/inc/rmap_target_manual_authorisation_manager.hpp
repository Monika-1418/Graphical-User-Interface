/**
* \file rmap_target_manual_authorisation_manager.hpp
*
* \brief Provides functions for handling manual RMAP target authorisation.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling manual RMAP target authorisation.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET_MANUAL_AUTH
*/

#ifndef __RMAP_TARGET_MANUAL_AUTHORISATION_MANAGER_CPP__
#define __RMAP_TARGET_MANUAL_AUTHORISATION_MANAGER_CPP__

#include "star-api.h"
#include "rmap_target_types.h"

#include "rmap_command_parameters.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {
                                       namespace authorisation {

/**
* Functions in this class deal with the manual RMAP target authorisation.
*/
class RMAPTargetManualAuthorisationManager
{
protected:
    /** the ID of device which RMAP target authorisation manager relates to */
    STAR_DEVICE_ID idOfOwningDevice;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPTargetManualAuthorisationManager()
    {
    }

    /**
    * Destructor. Free any resources created.
    */
    virtual ~RMAPTargetManualAuthorisationManager()
    {
        /* no resources to free at present */
    }

    /**
    * Constructor - use given parameters to initialise RMAP target manual
    * authorisation manager.
    *
    * @param deviceID The device ID that the RMAP target authorisation manager
    *                 relates to.
    */
    explicit RMAPTargetManualAuthorisationManager(STAR_DEVICE_ID deviceID) :
        idOfOwningDevice(deviceID)
    {
    }

    /**
    * Copy constructor - to allow copies of
    * RMAPTargetManualAuthorisationManager objects to be made when being passed
    * by value in functions or returned by value from functions.
    *
    */
    RMAPTargetManualAuthorisationManager(
        const RMAPTargetManualAuthorisationManager&
        manualAuthorisationManagerStateToCopyFrom)
        : idOfOwningDevice(0)
    {
        /* copy required contents of object being copied into this object */
        idOfOwningDevice =
            manualAuthorisationManagerStateToCopyFrom.idOfOwningDevice;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * RMAPTargetManualAuthorisationManager objects to have their contents
    * assigned to each other.
    *
    */
    RMAPTargetManualAuthorisationManager& operator=(
        const RMAPTargetManualAuthorisationManager&
        manualAuthorisationManagerStateToCopyFrom)
    {
        /* check for self-assignment, no point in copying one's self */
        if(this == &manualAuthorisationManagerStateToCopyFrom)
        {
            return *this;
        }

        /* copy required contents of object being copied into this object */
        idOfOwningDevice =
            manualAuthorisationManagerStateToCopyFrom.idOfOwningDevice;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Authorises a waiting RMAP command.
    *
    * @param target Target to authorise a waiting RMAP command for.
    *
    * @return 1 If the operation was successful, else 0.
    */
    virtual int AuthoriseWaitingCommand(U32 target) = 0;

    /**
    * Gets the parameters of the RMAP command waiting to be authorised.
    *
    * @param target Target to get the waiting RMAP command parameters from.
    * @param pRMAPCommandParameters Pointer to an object to be updated with the
    *                               RMAP command parameters.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetWaitingCommand(U32 target,
        _Out_ RMAPCommandParameters *pRMAPCommandParameters) = 0;

    /**
    * Gets whether or not there is an RMAP command waiting to be authorised.
    *
    * @param target Target to check whether authorisation is required.
    *
    * @return 1 if the target has a command waiting to be authorised, else 0.
    */
    virtual int IsAuthRequired(U32 target) = 0;

    /**
    * Gets whether or not there is an RMAP command waiting to be authorised.
    *
    * @param target Target to check whether authorisation is required.
    *
    * @return 1 if the target has a command waiting to be authorised, else 0.
    */
    virtual int RejectWaitingCommand(U32 target,
        REJECTION_REASON rejectionReason) = 0;
};

/* end namespace tags */
} /* authorisation */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

