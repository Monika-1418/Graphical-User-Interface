/**
* \file pxi_rmap_target_configuration.hpp
*
* \brief Provides functions for setting and getting RMAP target configuration
*        parameters on PXI devices.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for setting and getting RMAP target configuration
* parameters on PXI devices.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET_CONF_PXI
*/

#ifndef __PXI_RMAP_TARGET_CONFIGURATION_CPP__
#define __PXI_RMAP_TARGET_CONFIGURATION_CPP__

#include "rmap_target_configuration.hpp"

#include "rmap_target_pxi_if.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {
                                       namespace pxi {

/**
* Functions in this class deal with setting and getting RMAP target
* configuration parameters on PXI devices.
*
* \note Can either be created by passing a \link stardundee::com::starsystem::general::Device Device \endlink 
*       instance to RMAPTargetFactory::CreateRMAPTargetConfiguration() or by 
*       instantiating directly with a STAR-System device ID.
*/
class PXIRMAPTargetConfiguration : public RMAPTargetConfiguration
{
public:
    /**
    * Default constructor. Initialise object to default state.
    */
    PXIRMAPTargetConfiguration() : RMAPTargetConfiguration()
    {
    }

    /**
    * Constructor - use given parameters to initialise PXI RMAP target
    * configuration.
    *
    * @param deviceID The device ID that the PXI RMAP target configuration
    *                 relates to.
    */
    explicit PXIRMAPTargetConfiguration(STAR_DEVICE_ID deviceID) :
        RMAPTargetConfiguration(deviceID)
    {
    }

    int GetAddressOffset(U32 target, U32 *pOffset)
    {
        /* call 'c' API to get address offset */
        return RMAP_TARGET_PXI_IF_getAddressOffset(idOfOwningDevice, target,
            pOffset);
    }

    int GetAuthCommands(U32 target, TARGET_AUTH_CMD_MASK *pCommands)
    {
        /* call 'c' API to get auth commands */
        return RMAP_TARGET_PXI_IF_getAuthCommands(idOfOwningDevice, target,
            pCommands);
    }

    int GetAuthControlMode(U32 target, TARGET_AUTH_MODE *pMode)
    {
        /* call 'c' API to get auth control mode */
        return RMAP_TARGET_PXI_IF_getAuthControlMode(idOfOwningDevice, target,
            pMode);
    }

    int GetAuthKeyRange(U32 target, U8 *pLowest, U8 *pHighest)
    {
        /* call 'c' API to get auth key range */
        return RMAP_TARGET_PXI_IF_getAuthKeyRange(idOfOwningDevice, target,
            pLowest, pHighest);
    }

    int GetAuthLogicalAddressRange(U32 target, U8 *pLowest, U8 *pHighest)
    {
        /* call 'c' API to get auth logical address range */
        return RMAP_TARGET_PXI_IF_getAuthLogicalAddressRange(idOfOwningDevice,
            target, pLowest, pHighest);
    }

    int GetAuthMemoryAddressRange(U32 target, U32 *pLowerBoundary,
        U32 *pUpperBoundary)
    {
        /* call 'c' API to get auth memory address range */
        return RMAP_TARGET_PXI_IF_getAuthMemoryAddressRange(idOfOwningDevice,
            target, pLowerBoundary, pUpperBoundary);
    }

    int GetAuthProtocolId(U32 target, U8 *pProtocolId)
    {
        /* call 'c' API to get auth protocol id */
        return RMAP_TARGET_PXI_IF_getAuthProtocolId(idOfOwningDevice, target,
            pProtocolId);
    }

    int GetInterfaceMode(U32 port, IF_MODE *pMode)
    {
        /* call 'c' API to get interface mode */
        return RMAP_TARGET_PXI_IF_getInterfaceMode(idOfOwningDevice, port,
            pMode);
    }

    int GetStatus(U32 target, TARGET_STATUS *pStatus)
    {
        /* call 'c' API to get status */
        return RMAP_TARGET_PXI_IF_getStatus(idOfOwningDevice, target, pStatus);
    }

    int ReadMemory(U32 target, U32 address, U32 length, unsigned char *pBuffer)
    {
        /* call 'c' API to read memory */
        return RMAP_TARGET_PXI_IF_readMemory(idOfOwningDevice, target, address,
            length, pBuffer);
    }

    int SetAddressOffset(U32 target, U32 offset)
    {
        /* call 'c' API to set address offset */
        return RMAP_TARGET_PXI_IF_setAddressOffset(idOfOwningDevice, target,
            offset);
    }

    int SetAuthCommands(U32 target, TARGET_AUTH_CMD_MASK commands)
    {
        /* call 'c' API to set auth commands */
        return RMAP_TARGET_PXI_IF_setAuthCommands(idOfOwningDevice, target,
            commands);
    }

    int SetAuthControlMode(U32 target, TARGET_AUTH_MODE mode)
    {
        /* call 'c' API to set auth control mode */
        return RMAP_TARGET_PXI_IF_setAuthControlMode(idOfOwningDevice, target,
            mode);
    }

    int SetAuthKeyRange(U32 target, U8 lowest, U8 highest)
    {
        /* call 'c' API to set auth key range */
        return RMAP_TARGET_PXI_IF_setAuthKeyRange(idOfOwningDevice, target,
            lowest, highest);
    }

    int SetAuthLogicalAddressRange(U32 target, U8 lowest, U8 highest)
    {
        /* call 'c' API to set auth logical address range */
        return RMAP_TARGET_PXI_IF_setAuthLogicalAddressRange(idOfOwningDevice,
            target, lowest, highest);
    }

    int SetAuthMemoryAddressRange(U32 target, U32 lowerBoundary,
        U32 upperBoundary)
    {
        /* call 'c' API to set auth memory address range */
        return RMAP_TARGET_PXI_IF_setAuthMemoryAddressRange(idOfOwningDevice,
            target, lowerBoundary, upperBoundary);
    }

    int SetAuthProtocolId(U32 target, U8 protocolId)
    {
        /* call 'c' API to set auth protocol id */
        return RMAP_TARGET_PXI_IF_setAuthProtocolId(idOfOwningDevice, target,
            protocolId);
    }

    int SetInterfaceMode(U32 port, IF_MODE mode)
    {
        /* call 'c' API to set interface mode */
        return RMAP_TARGET_PXI_IF_setInterfaceMode(idOfOwningDevice, port,
            mode);
    }

    int WriteMemory(U32 target, U32 address, U32 length,
        const unsigned char *pBuffer)
    {
        /* call 'c' API to write memory */
        return RMAP_TARGET_PXI_IF_writeMemory(idOfOwningDevice, target,
            address, length, pBuffer);
    }
};

/* end namespace tags */
} /* pxi */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

