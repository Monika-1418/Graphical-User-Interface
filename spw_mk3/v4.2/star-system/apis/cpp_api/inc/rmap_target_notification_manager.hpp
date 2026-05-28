/**
* \file rmap_target_notification_manager.hpp
*
* \brief Provides functions for handling RMAP target notifications.
*
* \author STAR-Dundee Ltd\n
*         STAR House\n
*         166 Nethergate\n
*         Dundee, DD1 4EE\n
*         Scotland, UK\n
*         e-mail: support@star-dundee.com
*
* Provides functions for handling RMAP target notifications.
*
* Copyright &copy; 2017 STAR-Dundee Ltd
*
* \ingroup CPP_RMAP_TARGET_NOTIF
*/

#ifndef __RMAP_TARGET_NOTIFICATION_MANAGER_CPP__
#define __RMAP_TARGET_NOTIFICATION_MANAGER_CPP__

#include "rmap_target_pxi_if.h"

#include "rmap_notification_callback.hpp"
#include "authorisation_request_notification.hpp"
#include "command_complete_notification.hpp"

namespace stardundee { namespace com { namespace starsystem {
                                       namespace rmaptarget {
                                       namespace notifications {

/**
* Functions in this class deal with the RMAP target notification handling.
*/
class RMAPTargetNotificationManager
{
protected:
    /** the ID of device which RMAP target notification manager relates to */
    STAR_DEVICE_ID idOfOwningDevice;

    /**
    * Returns an RMAPNotification instance based on the given buffer.
    *
    * @param pNotificationPacket the notification packet to convert.
    *
    * @return RMAPNotification based on input packet.
    */
    static RMAPNotification * RMAPNotificationFromBuffer(
        U8 *pNotificationPacket)
    {
        /* initialise RMAPNotification to null before it is populated below */
        RMAPNotification *pNotification = NULL;

        /* get common fields */
        U8 targetIndex = pNotificationPacket[4];
        U8 currentTimeCode = pNotificationPacket[5];

        /* if this is a authorisation request notification */
        if(pNotificationPacket[2] == 0x10)
        {
            /* initialise new authorisation request notification */
            pNotification = new AuthorisationRequestNotification(targetIndex, 
                currentTimeCode);
        }
        /* else if this is a command complete notification */
        else if(pNotificationPacket[2] == 0x12)
        {
            /* get command complete parameters */
            U8 targetLogicalAddress = pNotificationPacket[6];
            U8 protocolId = pNotificationPacket[7];
            U8 command = pNotificationPacket[8];
            U8 key = pNotificationPacket[9];
            U8 initiatorLogicalAddress = pNotificationPacket[10];
            U16 transactionId = (pNotificationPacket[11] << 8) |
                pNotificationPacket[12];
            U8 extendedAddress = pNotificationPacket[13];
            U32 address = (pNotificationPacket[14] << 24) |
                (pNotificationPacket[15] << 16) |
                (pNotificationPacket[16] << 8) |
                pNotificationPacket[17];
            U32 dataLength = (pNotificationPacket[18] << 16) |
                (pNotificationPacket[19] << 8) |
                pNotificationPacket[20];
            U8 status = pNotificationPacket[21];

            /* initialise new command complete notfication */
            pNotification = new CommandCompleteNotification(targetIndex, 
                currentTimeCode, targetLogicalAddress, protocolId,
                command, key, initiatorLogicalAddress, transactionId,
                extendedAddress, address, dataLength, status);
        }

        /* return the notification instance */
        return pNotification;
    }

public:
    /**
    * Default constructor. Initialise object to default state.
    */
    RMAPTargetNotificationManager()
    {
    }

    /**
    * Destructor. Free any resources created.
    */
    virtual ~RMAPTargetNotificationManager()
    {
        /* no resources to free at present */
    }

    /**
    * Constructor - use given parameters to initialise RMAP target notification
    * authorisation manager.
    *
    * @param deviceID The device ID that the RMAP target notification manager
    *                 relates to.
    */
    explicit RMAPTargetNotificationManager(STAR_DEVICE_ID deviceId) :
        idOfOwningDevice(deviceId)
    {
    }

    /**
    * Register a notification callback.
    *
    * @param target Target to register a notification listener function for.
    * @param type Type of notification to register the listener function for.
    * @param[in] pNotificationCallback Pointer to a notification callback.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int RegisterNotificationListener(U32 target, NOTIF_TYPE type,
        RMAPNotificationCallback *pNotificationCallback) = 0;

    /**
    * Get the notifications that are enabled for a target.
    *
    * @param target Target to get enabled notifications from.
    * @param[out] pNotifications Pointer to a value to be updated with the
    *             enabled notifications.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int GetEnabledNotifications(U32 target,
        NOTIF_TYPE_MASK *pNotifications) = 0;

    /**
    * Set the notifications that are enabled for a target.
    *
    * @param target Target to set enabled notifications for.
    * @param notifications Enabled notifications mask
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int SetEnabledNotifications(U32 target,
        NOTIF_TYPE_MASK notifications) = 0;

    /**
    * Start receiving notifications for a device.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int StartReceivingNotifications() = 0;

    /**
    * Stop receiving notifications for a device.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int StopReceivingNotifications() = 0;

    /**
    * Unregister a notification callback.
    *
    * @param target Target to unregister a notification listener with context
    *               function for.
    * @param type Type of notification to unregister the listener with context
    *             function for.
    *
    * @return 1 if the operation was successful, else 0.
    */
    virtual int UnregisterNotificationListener(U32 target, NOTIF_TYPE type) = 0;
};

/* end namespace tags */
} /* notifications */
} /* rmaptarget */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

