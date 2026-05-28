/**
* \file rmap_target_configuration.hpp
*
* \brief Provides functions for setting and getting RMAP target configuration
*        parameters.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for setting and getting RMAP target configuration
* parameters.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET_CONF
*/

#ifndef __RMAP_TARGET_CONFIGURATION_CPP__
#define __RMAP_TARGET_CONFIGURATION_CPP__

#include "star-api.h"
#include "rmap_target_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {

/**
* Functions in this class deal with setting and getting RMAP target
* configuration parameters.
*/
class RMAPTargetConfiguration
{
protected:
    /** the ID of the device which the RMAP target configuration relates to */
    STAR_DEVICE_ID idOfOwningDevice;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPTargetConfiguration()
    {
    }

    /**
    * Destructor. Free any resources created.
    */
    virtual ~RMAPTargetConfiguration()
    {
        /* no resources to free at present */
    }

    /**
    * Constructor - use given parameters to initialise RMAP target
    * configuration.
    *
    * @param deviceID The device ID that the RMAP target configuration relates
    *                 to.
    */
    explicit RMAPTargetConfiguration(STAR_DEVICE_ID deviceID) :
        idOfOwningDevice(deviceID)
    {
    }

    /**
    * Copy constructor - to allow copies of RMAPTargetConfiguration objects to
    * be made when being passed by value in functions or returned by value from
    * functions.
    *
    */
    RMAPTargetConfiguration(const RMAPTargetConfiguration&
        rmapTargetConfigurationStateToCopyFrom) : idOfOwningDevice(0)
    {
        /* copy required contents of object being copied into this object */
        idOfOwningDevice =
            rmapTargetConfigurationStateToCopyFrom.idOfOwningDevice;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * RMAPTargetConfiguration objects to have their contents assigned to each
    * other.
    *
    */
    RMAPTargetConfiguration& operator=(const RMAPTargetConfiguration&
        rmapTargetConfigurationStateToCopyFrom)
    {
        /* check for self-assignment, no point in copying one's self */
        if(this == &rmapTargetConfigurationStateToCopyFrom)
        {
            return *this;
        }

        /* copy required contents of object being copied into this object */
        idOfOwningDevice =
            rmapTargetConfigurationStateToCopyFrom.idOfOwningDevice;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Gets the address offset from a specific target.
    * The address offset determines where the target's memory region begins.
    *
    * @param target Target to get address offset from.
    * @param pOffset Pointer to a value to be updated with the target's address
    *                offset.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetAddressOffset(U32 target, U32 *pOffset) = 0;

    /**
    * Gets the authorised RMAP commands from a specific target.
    *
    * @param target Target to get authorised RMAP commands from.
    * @param pCommands Pointer to a value to be updated with the target's
    *                  authorised commands.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetAuthCommands(U32 target,
        TARGET_AUTH_CMD_MASK *pCommands) = 0;

    /**
    * Gets the authorisation control mode from a specific target.
    * Authorisation can be set to either TARGET_AUTH_MODE_MANUAL or
    * TARGET_AUTH_MODE_AUTOMATIC. When set to TARGET_AUTH_MODE_MANUAL, the
    * authorisation of RMAP commands is done manually by the user application.
    * When set to TARGET_AUTH_MODE_AUTOMATIC, the authorisation of RMAP
    * commands is done automatically using the expected parameter fields.
    *
    * @param deviceId Device containing the target.
    * @param target Target to get authorisation mode from.
    * @param pMode Pointer to a value to be updated with the target's
    *              authorisation mode.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetAuthControlMode(U32 target, TARGET_AUTH_MODE *pMode) = 0;

    /**
    * Gets the authorised key range from a specific target.
    *
    * @param target Target to get authorised key range from.
    * @param pLowest Pointer to a value to be updated with the lowest key
    *                authorised.
    * @param pHighest Pointer to a value to be updated with the highest key
    *                 authorised.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetAuthKeyRange(U32 target, U8 *pLowest, U8 *pHighest) = 0;

    /**
    * Gets the authorised target logical address range from a specific target.
    *
    * @param target Target to get authorised logical address range from.
    * @param pLowest Pointer to a value to be updated with the lowest target
    *                logical address authorised.
    * @param pHighest Pointer to a value to be updated with the highest target
    *                 logical address authorised.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetAuthLogicalAddressRange(U32 target, U8 *pLowest,
        U8 *pHighest) = 0;

    /**
    * Gets the authorised memory address range from a specific target.
    *
    * @param target Target to get authorised memory address range from.
    * @param pLowerBoundary Pointer to a value to be updated with the lowest
    *                       memory address authorised.
    * @param pUpperBoundary Pointer to a value to be updated with the highest
    *                       memory address authorised.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetAuthMemoryAddressRange(U32 target, U32 *pLowerBoundary,
        U32 *pUpperBoundary) = 0;

    /**
    * Gets the authorised protocol ID from a specific target.
    *
    * @param target Target to get authorised protocol ID from.
    * @param pProtocolId Pointer to a value to be updated with the target's
    *                    authorised protocol ID.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetAuthProtocolId(U32 target, U8 *pProtocolId) = 0;

    /**
    * Gets the RMAP interface mode for a specific port.
    *
    * @param port Number of the port (6 - 9).
    * @param pMode Pointer to a value to be updated with the port's interface
    *              mode.
    *
    * @return 1 if the port's RMAP interface mode was successfully modified,
    *           else 0.
    */
    virtual int GetInterfaceMode(U32 port, IF_MODE *pMode) = 0;

    /**
    * Gets the RMAP target status for a specific target.
    *
    * @param target Target to get status from.
    * @param pStatus Pointer to a value to be updated with the target's status.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetStatus(U32 target, TARGET_STATUS *pStatus) = 0;

    /**
    * Reads an area of RMAP target memory into a user-supplied buffer.
    *
    * @param target Target to read memory from.
    * @param address The address in the target to read from.
    * @param length The length to read.
    * @param pBuffer Pointer to a buffer to read memory into.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int ReadMemory(U32 target, U32 address, U32 length,
        unsigned char *pBuffer) = 0;

    /**
    * Sets the address offset for a specific target.
    * The address offset determines where the target's memory region begins.
    *
    * @param target Target to set address offset for.
    * @param offset Address offset in MBytes.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetAddressOffset(U32 target, U32 offset) = 0;

    /**
    * Sets the authorised RMAP commands for a specific target.
    *
    * @param target Target to set authorised RMAP commands for.
    * @param commands Authorised commands mask.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetAuthCommands(U32 target, TARGET_AUTH_CMD_MASK commands) = 0;

    /**
    * Sets the authorisation control mode for a specific target.
    * Authorisation can be set to either TARGET_AUTH_MODE_MANUAL or
    * TARGET_AUTH_MODE_AUTOMATIC. When set to TARGET_AUTH_MODE_MANUAL, the
    * authorisation of RMAP commands is done manually by the user application.
    * When set to TARGET_AUTH_MODE_AUTOMATIC, the authorisation of RMAP
    * commands is done automatically using the expected parameter fields.
    *
    * @param target Target to set authorisation mode for.
    * @param mode Authorisation mode.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetAuthControlMode(U32 target, TARGET_AUTH_MODE mode) = 0;

    /**
    * Sets the authorised key range for a specific target.
    *
    * @param target Target to set authorised key range for.
    * @param lowest Lowest key authorised.
    * @param highest Highest key authorised.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetAuthKeyRange(U32 target, U8 lowest, U8 highest) = 0;

    /**
    * Sets the authorised target logical address range for a specific target.
    *
    * @param target Target to set authorised logical address range for.
    * @param lowest Lowest target logical address authorised.
    * @param highest Highest target logical address authorised.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetAuthLogicalAddressRange(U32 target, U8 lowest,
        U8 highest) = 0;

    /**
    * Sets the authorised memory address range for a specific target.
    *
    * @param target Target to set authorised memory address range for.
    * @param lowerBoundary Lowest memory address authorised.
    * @param upperBoundary Highest memory address authorised.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetAuthMemoryAddressRange(U32 target, U32 lowerBoundary,
        U32 upperBoundary) = 0;

    /**
    * Sets the authorised protocol ID for a specific target.
    *
    * @param target Target to set authorised protocol ID for.
    * @param protocolId Authorised protocol ID.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetAuthProtocolId(U32 target, U8 protocolId) = 0;

    /**
    * Sets the RMAP interface mode for a specific port.
    *
    * @param port Number of the port (6 - 9).
    * @param mode Interface mode of the port.
    *
    * @return 1 if the port's RMAP interface mode was successfully modified,
    *         else 0.
    */
    virtual int SetInterfaceMode(U32 port, IF_MODE mode) = 0;

    /**
    * Writes an area of RMAP target memory from a user-supplied buffer.
    *
    * @param target Target to write memory to.
    * @param address The address in the target to write to.
    * @param length The length to write.
    * @param pBuffer Pointer to a buffer to write memory from.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int WriteMemory(U32 target, U32 address, U32 length,
        const unsigned char *pBuffer) = 0;
};

/* end namespace tags */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

