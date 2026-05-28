/**
* \file command_complete_notification.hpp
*
* \brief Represents an RMAP notification that has been returned by the
*        RMAP Target API when a command has been completed.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Represents an RMAP notification that has been returned by the RMAP Target API
* in relation to a command being completed.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET_NOTIF
*/

#include "rmap_notification.hpp"

#ifndef __COMMAND_COMPLETE_NOTIFICATION_CPP__
#define __COMMAND_COMPLETE_NOTIFICATION_CPP__

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {
                                       namespace notifications {

/**
* Class that encapsulates data for a command complete notification.
*/
class CommandCompleteNotification : public RMAPNotification
{
private:
    /** target logical address */
    U8 targetLogicalAddress;

    /** protocol ID */
    U8 protocolId;

    /** RMAP command bit */
    U8 command;

    /** RMAP key */
    U8 key;

    /** initiator logical address */
    U8 initiatorLogicalAddress;

    /** transaction ID (MSB first) */
    U16 transactionId;

    /** extended address */
    U8 extendedAddress;

    /** address (MSB first) */
    U32 address;

    /** data length (MSB first) */
    U32 dataLength;

    /** RMAP command status */
    U8 status;

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    CommandCompleteNotification() : RMAPNotification(NOTIF_TYPE_CMD_COMPLETE)
    {
    }

    /**
    * Constructor - use given parameters to initialise command complete
    * notification.
    *
    * @param targetIndex The target index value.
    * @param currentTimeCode The current time-code value.
    * @param targetLogicalAddress The target logical address.
    * @param protocolId The protocol id value.
    * @param command The RMAP command bit.
    * @param key The key value.
    * @param initiatorLogicalAddress The initiator logical address.
    * @param transactionId The transaction id value.
    * @param extendedAddress The extended address value.
    * @param address The address value.
    * @param dataLength The length of the data.
    * @param status The RMAP status value.
    */
    CommandCompleteNotification(U8 targetIndex, U8 currentTimeCode, 
        U8 targetLogicalAddress, U8 protocolId, U8 command,
        U8 key, U8 initiatorLogicalAddress, U16 transactionId,
        U8 extendedAddress, U32 address, U32 dataLength, U8 status) :
        RMAPNotification(NOTIF_TYPE_CMD_COMPLETE, targetIndex, currentTimeCode),
        targetLogicalAddress(targetLogicalAddress),
        protocolId(protocolId),
        command(command),
        key(key),
        initiatorLogicalAddress(initiatorLogicalAddress),
        transactionId(transactionId),
        extendedAddress(extendedAddress),
        address(address),
        dataLength(dataLength),
        status(status)
    {
    }

    /**
    * Copy constructor - invoked when a copy of a
    * CommandCompleteNotification object is made, e.g. such as through being a
    * return value from a function or when being passed into a function as a
    * 'value' parameter.
    *
    * A new CommandCompleteNotification will be created which is a duplicate of
    * the CommandCompleteNotification being copied from.
    *
    * @param commandCompleteNotification The CommandCompleteNotification object
    *                                    which is being copied.
    *
    */
    CommandCompleteNotification(const CommandCompleteNotification&
        commandCompleteNotification) : RMAPNotification()
    {
        /* copy contents of object being copied into this object */
        targetIndex = commandCompleteNotification.targetIndex;
        currentTimeCode = commandCompleteNotification.currentTimeCode;
        targetLogicalAddress = commandCompleteNotification.targetLogicalAddress;
        protocolId = commandCompleteNotification.protocolId;
        command = commandCompleteNotification.command;
        key = commandCompleteNotification.key;
        initiatorLogicalAddress =
            commandCompleteNotification.initiatorLogicalAddress;
        transactionId = commandCompleteNotification.transactionId;
        extendedAddress = commandCompleteNotification.extendedAddress;
        address = commandCompleteNotification.address;
        dataLength = commandCompleteNotification.dataLength;
        status = commandCompleteNotification.status;
    }

    /**
    * Overloaded assignment operator - invoked when an attempt is made to assign
    * the contents of one CommandCompleteNotification object into another.
    *
    * Any existing CommandCompleteNotification will be destroyed and a new
    * CommandCompleteNotification created which is a duplicate of the
    * CommandCompleteNotification being assigned.
    *
    * @param commandCompleteNotification The CommandCompleteNotification object
    *                                    whose contents are being assigned to
    *                                    'this' object.
    *
    */
    CommandCompleteNotification& operator=(
        const CommandCompleteNotification& commandCompleteNotification)
    {
        /* check for self-assignment, no point in copying one's self */
        if(this == &commandCompleteNotification)
        {
            return *this;
        }

        /* copy contents of object being copied into this object */
        targetIndex = commandCompleteNotification.targetIndex;
        currentTimeCode = commandCompleteNotification.currentTimeCode;
        targetLogicalAddress = commandCompleteNotification.targetLogicalAddress;
        protocolId = commandCompleteNotification.protocolId;
        command = commandCompleteNotification.command;
        key = commandCompleteNotification.key;
        initiatorLogicalAddress =
            commandCompleteNotification.initiatorLogicalAddress;
        transactionId = commandCompleteNotification.transactionId;
        extendedAddress = commandCompleteNotification.extendedAddress;
        address = commandCompleteNotification.address;
        dataLength = commandCompleteNotification.dataLength;
        status = commandCompleteNotification.status;

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Get the target logical address of the notification.
    *
    * @return The target logical address.
    *
    */
    U8 GetTargetLogicalAddress()
    {
        return targetLogicalAddress;
    }

    /**
    * Get the protocol id of the notification.
    *
    * @return The protocol id.
    *
    */
    U8 GetProtocolId()
    {
        return protocolId;
    }

    /**
    * Get the RMAP command bit of the notification.
    *
    * @return The command bit.
    *
    */
    U8 GetCommand()
    {
        return command;
    }

    /**
    * Get the RMAP key of the notification.
    *
    * @return The key value.
    *
    */
    U8 GetKey()
    {
        return key;
    }

    /**
    * Get the initiator logical address of the notification.
    *
    * @return The initiator logical address.
    *
    */
    U8 GetInitiatorLogicalAddress()
    {
        return initiatorLogicalAddress;
    }

    /**
    * Get the transaction id of the notification.
    *
    * @return The transaction id.
    *
    */
    U16 GetTransactionId()
    {
        return transactionId;
    }

    /**
    * Get the extended address of the notification.
    *
    * @return The extended address.
    *
    */
    U8 GetExtendedAddress()
    {
        return extendedAddress;
    }

    /**
    * Get the address of the notification.
    *
    * @return The address.
    *
    */
    U32 GetAddress()
    {
        return address;
    }

    /**
    * Get the data length of the notification.
    *
    * @return The data length.
    *
    */
    U32 GetDataLength()
    {
        return dataLength;
    }

    /**
    * Get the RMAP command status of the notification.
    *
    * @return The status value.
    *
    */
    U8 GetStatus()
    {
        return status;
    }
};

/* end namespace tags */
} /* notifications */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

