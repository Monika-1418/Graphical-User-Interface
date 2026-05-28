/**
* \file rmap_command_parameters.hpp
*
* \brief Represents the parameters of an RMAP command.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Represents the parameters that make up an RMAP command.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET
*/

#ifndef __RMAP_COMMAND_PARAMETERS_CPP__
#define __RMAP_COMMAND_PARAMETERS_CPP__

#include "rmap_target_types.h"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {

/**
* This class encapsulates parameters that make up an RMAP command.
*/
class RMAPCommandParameters
{
private:
    /** struct required by RMAP target API to hold RMAP command parameters */
    RmapCommandParameters rmapCommandParameters;

public:
    /**
    * Constructor - use given values to initialise RMAP command parameters.
    *
    * @param[in] targetLogicalAddress The input target logical address
    *                                 value.
    * @param[in] command The input command value.
    * @param[in] key The input key value.
    * @param[in] protocolId The input protocol id value.
    * @param[in] extendedAddress The input extended address value.
    * @param[in] dataLength The input data length value.
    * @param[in] address The input address value.
    * @param[in] initiatorLogicalAddress The input initiator logical address
    *                                    value.
    * @param[in] transactionId The input transaction id value.
    */
    RMAPCommandParameters(_In_opt_ U8 targetLogicalAddress = 0,
        _In_opt_ U8 command = 0, _In_opt_ U8 key = 0,
        _In_opt_ U8 protocolId = 0, _In_opt_ U8 extendedAddress = 0,
        _In_opt_ U32 dataLength = 0, _In_opt_ U32 address = 0,
        _In_opt_ U8 initiatorLogicalAddress = 0, _In_opt_ U16 transactionId = 0)
    {
        /* populate the RMAP command parameters */
        rmapCommandParameters.targetLogicalAddress = targetLogicalAddress;
        rmapCommandParameters.command = command;
        rmapCommandParameters.key = key;
        rmapCommandParameters.protocolId = protocolId;
        rmapCommandParameters.extendedAddress = extendedAddress;
        rmapCommandParameters.dataLength = dataLength;
        rmapCommandParameters.address = address;
        rmapCommandParameters.initiatorLogicalAddress = initiatorLogicalAddress;
        rmapCommandParameters.transactionId = transactionId;
    }

    /**
    * Constructor - use given struct to initialise RMAP command parameters.
    *
    * @param commandParameters The input command parameters struct to populate
    *                          from.
    */
    explicit RMAPCommandParameters(RmapCommandParameters commandParameters)
    {
        /* populate the RMAP command parameters */
        rmapCommandParameters.targetLogicalAddress =
            commandParameters.targetLogicalAddress;
        rmapCommandParameters.command = commandParameters.command;
        rmapCommandParameters.key = commandParameters.key;
        rmapCommandParameters.protocolId = commandParameters.protocolId;
        rmapCommandParameters.extendedAddress =
            commandParameters.extendedAddress;
        rmapCommandParameters.dataLength = commandParameters.dataLength;
        rmapCommandParameters.address = commandParameters.address;
        rmapCommandParameters.initiatorLogicalAddress =
            commandParameters.initiatorLogicalAddress;
        rmapCommandParameters.transactionId = commandParameters.transactionId;
    }

    /**
    * Copy constructor - to allow copies of
    * RMAPCommandParameters objects to be made when being passed by value in
    * functions or returned by value from functions.
    *
    */
    RMAPCommandParameters(const RMAPCommandParameters&
        rmapCommandParametersStateToCopyFrom)
    {
        /* copy required contents of object being copied into this object */
        rmapCommandParameters.targetLogicalAddress =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            targetLogicalAddress;
        rmapCommandParameters.command =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.command;
        rmapCommandParameters.key =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.key;
        rmapCommandParameters.protocolId =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            protocolId;
        rmapCommandParameters.extendedAddress =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            extendedAddress;
        rmapCommandParameters.dataLength =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            dataLength;
        rmapCommandParameters.address =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.address;
        rmapCommandParameters.initiatorLogicalAddress =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            initiatorLogicalAddress;
        rmapCommandParameters.transactionId =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            transactionId;
    }

    /**
    * Overloaded assignment operator - to allow instances of
    * RMAPCommandParameters objects to have their contents assigned to each
    * other.
    *
    */
    RMAPCommandParameters& operator=(const RMAPCommandParameters&
        rmapCommandParametersStateToCopyFrom)
    {
        /* check for self-assignment, no point in copying one's self */
        if(this == &rmapCommandParametersStateToCopyFrom)
        {
            return *this;
        }

        /* copy required contents of object being copied into this object */
        rmapCommandParameters.targetLogicalAddress =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            targetLogicalAddress;
        rmapCommandParameters.command =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.command;
        rmapCommandParameters.key =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.key;
        rmapCommandParameters.protocolId =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            protocolId;
        rmapCommandParameters.extendedAddress =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            extendedAddress;
        rmapCommandParameters.dataLength =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            dataLength;
        rmapCommandParameters.address =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.address;
        rmapCommandParameters.initiatorLogicalAddress =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            initiatorLogicalAddress;
        rmapCommandParameters.transactionId =
            rmapCommandParametersStateToCopyFrom.rmapCommandParameters.
            transactionId;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Get the target logical address parameter.
    *
    * @return The target logical address value.
    *
    */
    U8 GetTargetLogicalAddress()
    {
        return rmapCommandParameters.targetLogicalAddress;
    }

    /**
    * Get the command parameter.
    *
    * @return The command value.
    *
    */
    U8 GetCommand()
    {
        return rmapCommandParameters.command;
    }

    /**
    * Get the key parameter.
    *
    * @return The key value.
    *
    */
    U8 GetKey()
    {
        return rmapCommandParameters.key;
    }

    /**
    * Get the protocol ID parameter.
    *
    * @return The protocol ID value.
    *
    */
    U8 GetProtocolId()
    {
        return rmapCommandParameters.protocolId;
    }

    /**
    * Get the extended address parameter.
    *
    * @return The extended address value.
    *
    */
    U8 GetExtendedAddress()
    {
        return rmapCommandParameters.extendedAddress;
    }

    /**
    * Get the data length parameter.
    *
    * @return The data length value.
    *
    */
    U32 GetDataLength()
    {
        return rmapCommandParameters.dataLength;
    }

    /**
    * Get the address parameter.
    *
    * @return The address value.
    *
    */
    U32 GetAddress()
    {
        return rmapCommandParameters.address;
    }

    /**
    * Get the initiator logical address parameter.
    *
    * @return The initiator logical address value.
    *
    */
    U8 GetInitiatorLogicalAddress()
    {
        return rmapCommandParameters.initiatorLogicalAddress;
    }

    /**
    * Get the transaction id parameter.
    *
    * @return The transaction id value.
    *
    */
    U16 GetTransactionId()
    {
        return rmapCommandParameters.transactionId;
    }

    /**
    * Set the target logical address parameter.
    *
    * @param targetLogicalAddress The target logical address value to set.
    *
    */
    void SetTargetLogicalAddress(U8 targetLogicalAddress)
    {
        rmapCommandParameters.targetLogicalAddress = targetLogicalAddress;
    }

    /**
    * Set the command parameter.
    *
    * @param command The command value to set.
    *
    */
    void SetCommand(U8 command)
    {
        rmapCommandParameters.command = command;
    }

    /**
    * Set the key parameter.
    *
    * @param key The key value to set.
    *
    */
    void SetKey(U8 key)
    {
        rmapCommandParameters.key = key;
    }

    /**
    * Set the protocol id parameter.
    *
    * @param protocolId The protocol id value to set.
    *
    */
    void SetProtocolId(U8 protocolId)
    {
        rmapCommandParameters.protocolId = protocolId;
    }

    /**
    * Set the extended address parameter.
    *
    * @param extendedAddress The extended address value to set.
    *
    */
    void SetExtendedAddress(U8 extendedAddress)
    {
        rmapCommandParameters.extendedAddress = extendedAddress;
    }

    /**
    * Set the data length parameter.
    *
    * @param dataLength The data length value to set.
    *
    */
    void SetDataLength(U32 dataLength)
    {
        rmapCommandParameters.dataLength = dataLength;
    }

    /**
    * Set the address parameter.
    *
    * @param address The address value to set.
    *
    */
    void SetAddress(U32 address)
    {
        rmapCommandParameters.address = address;
    }

    /**
    * Set the initiator logical address parameter.
    *
    * @param initiatorLogicalAddress The initiator logical address value to set.
    *
    */
    void SetInitiatorLogicalAddress(U8 initiatorLogicalAddress)
    {
        rmapCommandParameters.initiatorLogicalAddress = initiatorLogicalAddress;
    }

    /**
    * Set the transaction id parameter.
    *
    * @param transactionId The transaction id value to set.
    *
    */
    void SetTransactionId(U16 transactionId)
    {
        rmapCommandParameters.transactionId = transactionId;
    }

    /**
    * Return the RmapCommandParameters struct which is held internally by this
    * object.
    *
    * @return A pointer to the RmapCommandParameters structure which is held
    *         internally by this object.
    *
    * \note This is provided to support internal processing of other objects
    *       within the C++ API and would not normally require to be invoked by
    *       client applications using the API.
    */
    RmapCommandParameters *GetAsRmapCommandParameters()
    {
        /* return pointer to the RmapCommandParameters struct held internally
         * by this object. */
        return &rmapCommandParameters;
    }
};

/* end namespace tags */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

