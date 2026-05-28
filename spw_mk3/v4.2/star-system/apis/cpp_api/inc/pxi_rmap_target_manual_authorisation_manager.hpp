/**
* \file pxi_rmap_target_manual_authorisation_manager.hpp
*
* \brief Provides functions for handling manual RMAP target authorisation
*        specific to PXI devices.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling manual RMAP target authorisation on PXI
* devices.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET_MANUAL_AUTH_PXI
*/

#ifndef __PXI_RMAP_TARGET_MANUAL_AUTHORISATION_MANAGER_CPP__
#define __PXI_RMAP_TARGET_MANUAL_AUTHORISATION_MANAGER_CPP__

#include "rmap_target_pxi_if.h"

#include "rmap_target_manual_authorisation_manager.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {
                                       namespace authorisation {
                                       namespace pxi {

/**
* Functions in this class deal with the manual RMAP target authorisation on a
* PXI card.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to 
*       RMAPTargetFactory::CreateRMAPTargetManualAuthorisationManager() or by 
*       instantiating directly with a STAR-System device ID.
*/
class PXIRMAPTargetManualAuthorisationManager :
    public RMAPTargetManualAuthorisationManager
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PXIRMAPTargetManualAuthorisationManager() : 
        RMAPTargetManualAuthorisationManager()
    {
    }

    /**
    * Constructor - use given parameters to initialise RMAP target manual
    * authorisation manager for given PXI device.
    *
    * @param deviceID The device ID of the PXI device that the RMAP target
    *                 manual authorisation manager relates to.
    */
    explicit PXIRMAPTargetManualAuthorisationManager(STAR_DEVICE_ID deviceID) :
        RMAPTargetManualAuthorisationManager(deviceID)
    {
    }

    int AuthoriseWaitingCommand(U32 target)
    {
        /* call 'c' API to authorise the waiting command */
        return RMAP_TARGET_PXI_IF_authoriseWaitingCommand(idOfOwningDevice,
            target);
    }

    int GetWaitingCommand(U32 target,
        _Out_ RMAPCommandParameters *pRMAPCommandParameters)
    {
        /* call 'c' API to get the waiting command */
        return RMAP_TARGET_PXI_IF_getWaitingCommand(idOfOwningDevice, target,
            pRMAPCommandParameters->GetAsRmapCommandParameters());
    }

    int IsAuthRequired(U32 target)
    {
        /* call 'c' API to check if authorisation is required */
        return RMAP_TARGET_PXI_IF_isAuthRequired(idOfOwningDevice, target);
    }

    int RejectWaitingCommand(U32 target,
        REJECTION_REASON rejectionReason)
    {
        /* call 'c' API to reject waiting command */
        return RMAP_TARGET_PXI_IF_rejectWaitingCommand(idOfOwningDevice,
            target, rejectionReason);
    }
};

/* end namespace tags */
} /* pxi */
} /* authorisation */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

