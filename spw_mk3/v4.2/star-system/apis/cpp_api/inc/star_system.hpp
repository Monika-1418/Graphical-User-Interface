/**
 * \file star_system.hpp
 *
 * \brief Provides general functions for the STAR-System API, e.g. retrieving
 *        the list of devices which are present and the versions of API
 *        modules.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides general functions for the STAR-System API, e.g. retrieving the list
 * of devices which are present and the versions of API modules.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_GENERAL
 */

#ifndef __STAR_SYSTEM_CPP__
#define __STAR_SYSTEM_CPP__

#include "version_information.hpp"
#include "star-dundee_annotations.h"
#include "device_factory.hpp"
#include "star_driver_factory.hpp"
#include "remote_device.hpp"
#include "device_listener_callback.hpp"
#include "device_listener.hpp"
#include "channel_listener_callback.hpp"
#include "channel_listener.hpp"
#include "driver_listener_callback.hpp"
#include "driver_listener.hpp"
#include "transfer_completion_listener_callback.hpp"
#include "transfer_completion_listener.hpp"
#include "device_list.hpp"
#include "cpp_api_macros.hpp"
#include <new>

namespace stardundee { namespace com { namespace starsystem {
                                       namespace general {

/**
* Provides general functions for the STAR-System API, e.g. retrieving the list
* of devices which are present and the versions of API modules.
*/
class STARSystem
{
public:
    /** String constants, e.g. maximum expected length of strings provided as
      * parameters into functions. */
    enum StringConstants
    {
        /** maxmimum expected length of application name strings */
        STR_APPLICATION_NAME_LEN = STAR_STR_MAX_LEN
    };

private:
    /**
    * Call-back function which will be registered with STAR-System to receive
    * notifications of when devices are added or removed.
    *
    * @param deviceListenerID The ID of the device listener that the event
    *                         corresponds to.
    * @param driverIdentifier Identifies the driver on which the device was
    *                         added or removed.
    * @param deviceIdentifier Identifies the device which was added or removed.
    * @param deviceAdded Whether the device has been added (1) or removed (0).
    * @param pContextInfo A pointer to user-supplied context information which
    *                     was provided when the device listener was being
    *                     registered and which is expected to contain a pointer
    *                     to a DeviceListenerCallback object which will be used
    *                     to issue notifications back to the user of whether a
    *                     device has been added or removed.
    */
    static void STAR_API_CC DeviceListenerCallBack(
                            STAR_DEVICE_LISTENER_ID deviceListenerID,
                            STAR_DRIVER_ID driverIdentifier,
                            STAR_DEVICE_ID deviceIdentifier,
                            int deviceAdded, void *pContextInfo)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(deviceListenerID);

        /* get pointer to DeviceListenerCallback object which is expected to be
         * contained within the context information provided */
        DeviceListenerCallback *pDeviceListenerCallbackObj;
        pDeviceListenerCallbackObj = (DeviceListenerCallback*)pContextInfo;

        /* if valid object obtained */
        if (pDeviceListenerCallbackObj)
        {
            /* create instance of Driver object that represents the driver on
             * which the device was added or removed */
            Driver *pDriver;
            pDriver = STARDriverFactory::CreateDriverFromID(driverIdentifier);

            /* create instance of Device object that represents the device that
             * was added or removed */
            Device *pDevice = DeviceFactory::CreateDevice(deviceIdentifier);

            /* if a device has been added */
            if (deviceAdded)
            {
                /* notify the user that a device has been added */
                pDeviceListenerCallbackObj->DeviceAdded(pDriver, pDevice);
            }
            else
            {
                /* notify the user that a device has been removed */
                pDeviceListenerCallbackObj->DeviceRemoved(pDriver, pDevice);
            }

            /* free device and driver objects after use */
            if (pDriver)
                delete pDriver;
            if (pDevice)
                delete pDevice;
        }
    }

    /**
    * Call-back function which will be registered with STAR-System to receive
    * notifications of when channels are opened or closed.
    *
    * @param channelListenerID The channel listener that this event
    *                                  corresponds to.
    * @param driverIdentifier The driver of the device on which the channel was
    *                         opened or closed.
    * @param deviceIdentifier The device on which the channel was opened or
    *                         closed.
    * @param channelIdentifier The identifier of the channel which has been
    *                          opened or closed.
    * @param channelOpened Whether the channel was opened (1) or closed (0).
    * @param channelNumber The channel number on the device of the channel
    *                      which was opened or closed.
    * @param pContextInfo A pointer to user-supplied context information which
    *                     was provided when the channel listener was being
    *                     registered and which is expected to contain a pointer
    *                     to a ChannelListenerCallback object which will be used
    *                     to issue notifications back to the user of whether a
    *                     channel has been opened or closed.
    */
    static void STAR_API_CC ChannelListenerCallBack(
                            STAR_CHANNEL_LISTENER_ID channelListenerID,
                            STAR_DRIVER_ID driverIdentifier,
                            STAR_DEVICE_ID deviceIdentifier,
                            STAR_CHANNEL_ID channelIdentifier,
                            int channelOpened, U8 channelNumber,
                            void *pContextInfo)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(channelListenerID);

        /* get pointer to ChannelListenerCallback object which is expected to be
         * contained within the context information provided */
        stardundee::com::starsystem::channels::ChannelListenerCallback
                                          *pChannelListenerCallbackObj;
        pChannelListenerCallbackObj =
           (stardundee::com::starsystem::channels::ChannelListenerCallback*)pContextInfo;

        /* if valid object obtained */
        if (pChannelListenerCallbackObj)
        {
            /* create instance of Driver object that represents the driver of
             * the device on which the channel was opened or closed */
            Driver *pDriver;
            pDriver = STARDriverFactory::CreateDriverFromID(driverIdentifier);

            /* create instance of Device object that represents the device on
             * which the channel was opened or closed */
            Device *pDevice = DeviceFactory::CreateDevice(deviceIdentifier);

            /* create instance of Channel object to contain information about
             * the channel which has been opened or closed */
            stardundee::com::starsystem::channels::Channel *pChannel;
            try
            {
                /* if channel has been opened */
                if (channelOpened)
                {
                    /* create instance of opened channel */
                    pChannel = new
                        stardundee::com::starsystem::channels::Channel(
                                    channelIdentifier, channelNumber, true);

                    /* \note The channel is created with an 'isCloseable' status
                     *       of false. The Channel object is provided for
                     *       informatory purposes only to channel listeners. Its
                     *       status can be queried and the channel can be used
                     *       if it is open (e.g. to transmit or receive traffic
                     *       on) but it cannot be closed. The Channel will be
                     *       closed elsewhere, in the code it was opened from
                     *       in the first place.
                     */

                    /* \note This strategy has been revised. It should be
                     *       possible to close a channel when received as a
                     *       parameter to a channel listener. Therefore, the
                     *       'isCloseable' status is set to 'true'. The above
                     *       note has been left here to clarify the reasoning
                     *       for the 'isCloseable' parameter should it be
                     *       required in the future.
                     */
                }
                else
                {
                    /* create instance of channel, but with an 'open' status
                     * of false (i.e. closed) */
                    pChannel = new
                        stardundee::com::starsystem::channels::Channel(
                                    channelIdentifier, channelNumber,
                                    true, false);

                    /* \note Along with an 'isCloseable' status of 'true',
                     *       a final parameter to the constructor indicates
                     *       that, although a valid channel ID is passed to
                     *       the object (which usually signifies an opened
                     *       channel), the 'open' status of the channel is
                     *       actually false since the channel has been closed.
                     */
                }
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory failure */
                pChannel = NULL;
            }

            /* if a channel has been opened */
            if (channelOpened)
            {
                /* notify the user that a channel has been opened */
                pChannelListenerCallbackObj->ChannelOpened(pDriver, pDevice,
                                                           pChannel);
            }
            else
            {
                /* notify the user that a channel has been closed */
                pChannelListenerCallbackObj->ChannelClosed(pDriver, pDevice,
                                                           pChannel);
            }

            /* free device, channel and driver objects after use */
            if (pDriver)
                delete pDriver;
            if (pDevice)
                delete pDevice;
            if (pChannel)
                delete pChannel;
        }
    }

    /**
    * Call-back function which will be registered with STAR-System to receive
    * notifications of when devices are added or removed.
    *
    * @param driverListenerID The driver listener that this event corresponds
    *                         to.
    * @param driverIdentifier The driver which has been added or removed.
    * @param driverAdded Whether the driver was added (1) or removed (0)
    * @param pContextInfo A pointer to user-supplied context information which
    *                     was provided when the driver listener was being
    *                     registered and which is expected to contain a pointer
    *                     to a DriverListenerCallback object which will be used
    *                     to issue notifications back to the user of whether a
    *                     driver has been added or removed.
    */
    static void STAR_API_CC DriverListenerCallBack(
                            STAR_DRIVER_LISTENER_ID driverListenerID,
                            STAR_DRIVER_ID driverIdentifier,
                            int driverAdded, void *pContextInfo)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(driverListenerID);

        /* get pointer to DriverListenerCallback object which is expected to be
         * contained within the context information provided */
        DriverListenerCallback *pDriverListenerCallbackObj;
        pDriverListenerCallbackObj = (DriverListenerCallback*)pContextInfo;

        /* if valid object obtained */
        if (pDriverListenerCallbackObj)
        {
            /* create instance of Driver object that represents the driver
             * which has been added or removed */
            Driver *pDriver;
            pDriver = STARDriverFactory::CreateDriverFromID(driverIdentifier);

            /* if a driver has been added */
            if (driverAdded)
            {
                /* notify the user that a driver has been added */
                pDriverListenerCallbackObj->DriverAdded(pDriver);
            }
            else
            {
                /* notify the user that a driver has been removed */
                pDriverListenerCallbackObj->DriverRemoved(pDriver);
            }

            /* free driver object after use */
            if (pDriver)
                delete pDriver;
        }
    }

    /**
    * Call-back function which will be registered with STAR-System to receive
    * notifications of when the given transfer operation completes.
    *
    * @param pOperation the operation which has completed
    * @param status the status of the operation
    * @param pContextInfo A pointer to user-supplied context information which
    *                     was provided when the transfer completion listener was
    *                     being registered and which is expected to contain a
    *                     pointer to a TransferCompletionListenerCallback object
    *                     which will be used to issue notifications back to the
    *                     user of the transfer operation completion.
    */
    static void STAR_API_CC TransferCompletionListenerCallBack(
                                            STAR_TRANSFER_OPERATION *pOperation,
                                            STAR_TRANSFER_STATUS status,
                                            void *pContextInfo)
    {
        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(pOperation);

        /* get pointer to TransferCompletionListenerCallback object which is
         * expected to be contained within the context information provided */
        stardundee::com::starsystem::transferoperations::TransferCompletionListenerCallback
                                                         *pTransferCompletionCallbackObj;
        pTransferCompletionCallbackObj =
        (stardundee::com::starsystem::transferoperations::TransferCompletionListenerCallback*)pContextInfo;

        /* if valid object obtained */
        if (pTransferCompletionCallbackObj)
        {
            /* notify the user that the transfer operation has completed. */
            pTransferCompletionCallbackObj->TransferOperationCompleted(
           (stardundee::com::starsystem::transferoperations::TransferOperation::TransferStatus)status);
        }
    }

public:
    /**
    * Retrieve version information for the STAR API.
    *
    * @return A pointer to a new VersionInformation object.
    *
    * \note The object returned by this function must be freed when no longer
    *       required by calling the C++ delete operator.
    */
    static VersionInformation* GetApiVersion()
    {
        STAR_VERSION_INFO *pSTARVersion;
        VersionInformation* pCppVersion;

        /* get api version from STAR-System */
        pSTARVersion = STAR_getApiVersion();

        /* if version obtained */
        if (pSTARVersion)
        {
            /* attempt to create a new VersionInformation object to hold
             * the version information for the CPP API - providing it with
             * the same version info as STAR-System */
            try
            {
                pCppVersion = new VersionInformation(pSTARVersion);

                /* change the name of the API to be the C++ API */
                pCppVersion->SetName("STAR-System C++ API");
                pCppVersion->SetAuthor("STAR-Dundee Ltd");
            }
            catch (std::bad_alloc& ba)
            {
                /* error allocating memory */
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);

                /* ensure empty version information object */
                pCppVersion = NULL;
            }

            /* destroy the STAR version obtained */
            STAR_destroyVersionInfo(pSTARVersion);

            /* return pointer to the C++ API version (or NULL if error) */
            return pCppVersion;
        }
        else
        {
            /* failed to obtain version, return NULL */
            return NULL;
        }
    }

    /**
    * Retrieve version information for all modules of STAR-System.
    *
    * @param[out] count Number of version information objects returned in array.
    *
    * @return An array of pointers to VersionInformation objects containing
    *         version details for all modules present in STAR-System or NULL
    *         if no version information is available and/or an error occurred
    *         allocating memory to hold the version information data.
    *
    * \note The array returned by this function and its contents must be freed
    *       when no longer required by using the C++ delete operator or by
    *       calling the following function: STARSystem::DestroyVersionList().
    */
    _Ret_opt_cap_(*count) static VersionInformation**
    GetAllVersions(_Out_ U32 *count)
    {
        U32 totalVersionNum = 0;

        /* get version information for this (the C++) API */
        VersionInformation *cppVersionInfo = STARSystem::GetApiVersion();
        if (cppVersionInfo)
            totalVersionNum = 1;

        /* request other version information info from STAR 'c' API */
        U32 cApiVersionNum = 0;
        STAR_VERSION_INFO *versionList = STAR_getAllVersions(&cApiVersionNum);
        totalVersionNum += cApiVersionNum;

        /* if no version information is found */
        if (totalVersionNum == 0)
        {
            /* record zero versions found */
            *count = 0;

            /* return NULL / empty array */
            return NULL;
        }

        /* try to allocate array of pointers to version information objects */
        VersionInformation **versions = NULL;
        try
        {
            versions = new VersionInformation*[totalVersionNum];
        }
        catch (std::bad_alloc)
        {
            /* memory allocation error, set null/empty versions */
            versions = NULL;
        }

        /* if memory allocated successfully */
        if (versions != NULL)
        {
            int versionListIndex = 0;
            U32 i;

            /* if cpp version info obtained */
            if (cppVersionInfo)
            {
                /* add it to the list of versions */
                versions[versionListIndex] = cppVersionInfo;
                versionListIndex++;
            }

            /* for each version info obtained from the 'c' API */
            for (i=0; i<cApiVersionNum; i++)
            {
                /* create a corresponding VersionInformation object and
                 * add it to the list of versions being prepared */
                versions[versionListIndex] = new VersionInformation(
                                                             &versionList[i]);
                versionListIndex++;

            }

            /* record the number of versions found */
            *count = totalVersionNum;
        }
        else
        {
            /* set count of versions to be zero */
            *count = 0;

            /* free cpp version info if required */
            if (cppVersionInfo)
                delete cppVersionInfo;
        }

        /* free array of STAR_VERSION_INFOs obtained from the STAR API */
        STAR_destroyVersionInfoList(versionList);

        /* return array of pointers to version objects (may be NULL) */
        return versions;
    }

    /**
    * Destroy a given list of VersionInformation objects.
    *
    * @param versionList An array of pointers to VersionInformation objects to
    *                    be freed.
    * @param count The number of elements in the array.
    *
    * @return 1 if list successfully freed, otherwise 0.
    */
    static int DestroyVersionList(VersionInformation **versionList, U32 count)
    {
        /* if list is null */
        if (versionList == NULL)
        {
            /* return error */
            return 0;
        }

        /* for each item in the list */
        for (U32 i=0; i<count; i++)
        {
            /* if it isn't already freed */
            if (versionList[i] != NULL)
            {
                /* free it */
                delete versionList[i];
            }
        }

        /* free the list itself */
        delete [] versionList;

        /* return success */
        return 1;
    }

    /**
    * Destroy a given list of RemoteDevice objects.
    *
    * @param devices An array of pointers to RemoteDevice objects to be freed.
    * @param count The number of elements in the array.
    *
    * @return 1 if list successfully freed, otherwise 0.
    */
    static int DestroyRemoteDeviceList(RemoteDevice **devices, U32 count)
    {
        /* if list is null */
        if (devices == NULL)
        {
            /* return error */
            return 0;
        }

        /* for each item in the list */
        for (U32 i=0; i<count; i++)
        {
            /* if it isn't already freed */
            if (devices != NULL)
            {
                /* free it */
                delete devices[i];
            }
        }

        /* free the list itself */
        delete [] devices;

        /* return success */
        return 1;
    }

    /**
    * Destroy a given list of Device objects.
    *
    * @param devices An array of pointers to Device objects to be freed.
    * @param count The number of elements in the array.
    *
    * @return 1 if list successfully freed, otherwise 0.
    */
    static int DestroyDeviceList(Device **devices, U32 count)
    {
        /* if list is null */
        if (devices == NULL)
        {
            /* return error */
            return 0;
        }

        /* for each item in the list */
        for (U32 i=0; i<count; i++)
        {
            /* if it isn't already freed */
            if (devices != NULL)
            {
                /* free it */
                delete devices[i];
            }
        }

        /* free the list itself */
        delete [] devices;

        /* return success */
        return 1;
    }

    /**
    * Get the application ID of the calling process.
    *
    * @return The application ID for the calling process.
    */
    static STAR_APP_ID GetApplicationID()
    {
        return STAR_getApplicationID();
    }

    /**
    * Gets the name of the calling process, as set by
    * STARSystem::SetApplicationName().
    *
    * @param[out] buffer  A user supplied buffer of length
    *                     STARSystem::STR_APPLICATION_NAME_LEN which will be
    *                     updated to contain the application name, as a null-
    *                     terminated string.
    *
    * @return The length of the string read into the buffer or zero if failed
    *         to read the application name.
    */
    static size_t GetApplicationName(_Out_z_cap_c_(STR_APPLICATION_NAME_LEN)
                                     char *buffer)
    {
        /* if invalid buffer provided */
        if (!buffer)
        {
            /* return error */
            return 0;
        }

        /* set user-supplied buffer to an empty string */
        buffer[0] = '\0';

        /* call 'c' API to get the application name */
        char *name = STAR_getApplicationName();

        /* if no string obtained */
        if (!name)
        {
            /* return error / zero length string */
            return 0;
        }

        /* get the size of the string obtained */
        size_t len = strlen(name);

        /* if string has zero length */
        if (len == 0)
        {
            /* destroy the string obtained from the STAR API */
            STAR_destroyString(name);

            /* return error / zero length string */
            return 0;
        }

        /* if size exceeds the expected buffer size */
        if (len > (STR_APPLICATION_NAME_LEN-1))
        {
            /* adjust the length to be the maximum size (this will truncate
             * the string) */
            len = STR_APPLICATION_NAME_LEN-1;
        }

        /* copy the string obtained to the user-supplied buffer */
        for (unsigned int i=0; i<len; i++)
            buffer[i] = name[i];

        /* append with a NULL */
        buffer[len] = '\0';

        /* destroy the string obtained from the STAR API */
        STAR_destroyString(name);

        /* return the length of string obtained */
        return strlen(buffer);
    }

    /**
    * Sets the name of the application using the STAR-System API.
    *
    * This is the name provided to other processes calling the
    * STARSystem::GetApplicationName() function.
    *
    * @param[in] name A null-terminated string containing the application name.
    *
    * \note If the length of the string provided exceeds
    *       STARSystem::STR_APPLICATION_NAME_LEN it will be truncated.
    *
    * @return 1 if the application name has been set successfully,
    *           otherwise 0
    *
    */
    static int SetApplicationName(_In_z_count_c_(STR_APPLICATION_NAME_LEN)
                                  char *name)
    {
        return STAR_setApplicationName(name);
    }


    /**
    * Gets the name of the application with the given ID.
    *
    * @param[in] id The application ID to obtain a corresponding name for
    * @param[out] buffer  A user supplied buffer of length
    *                     STARSystem::STR_APPLICATION_NAME_LEN which will be
    *                     updated to contain the application name, as a null-
    *                     terminated string.
    *
    * @return The length of the string read into the buffer or zero if failed
    *         to read the application name.
    */
     static size_t GetApplicationNameForID(
                          STAR_APP_ID id,
                          _Out_z_cap_c_(STR_APPLICATION_NAME_LEN) char *buffer)
    {

        /* if invalid buffer provided */
        if (!buffer)
        {
            /* return error */
            return 0;
        }

        /* set user-supplied buffer to an empty string */
        buffer[0] = '\0';

        /* call 'c' API to get the application name for the given ID */
        char *name = STAR_getApplicationNameForID(id);

        /* if no string obtained */
        if (!name)
        {
            /* return error / zero length string */
            return 0;
        }

        /* get the size of the string obtained */
        size_t len = strlen(name);

        /* if string has zero length */
        if (len == 0)
        {
            /* destroy the string obtained from the STAR API */
            STAR_destroyString(name);

            /* return error / zero length string */
            return 0;
        }

        /* if size exceeds the expected buffer size */
        if (len > (STR_APPLICATION_NAME_LEN-1))
        {
            /* adjust the length to be the maximum size (this will truncate
             * the string) */
            len = STR_APPLICATION_NAME_LEN-1;
        }

        /* copy the string obtained to the user-supplied buffer */
        for (unsigned int i=0; i<len; i++)
            buffer[i] = name[i];

        /* append with a NULL */
        buffer[len] = '\0';

        /* destroy the string obtained from the STAR API */
        STAR_destroyString(name);

        /* return the length of string obtained */
        return strlen(buffer);
    }

    /**
    * Get a list of all devices present for all drivers.
    *
    * @return A pointer to a new DeviceList object which contains the list of
    *         devices found or NULL if failed to create the device list.
    *
    * \note The DeviceList returned contains a snapshot of the current state of
    *       the system and is not automatically updated. Call the \link stardundee::com::starsystem::general::DeviceList::Populate() DeviceList::Populate() \endlink
    *       method on the list to re-populate its contents with a newer
    *       snapshot of the current state of the system.
    *
    * \note The object returned must be freed when no longer required using
    *       the C++ delete operator.
    */
    _Ret_opt_cap_(*count) static DeviceList* GetDeviceList()
    {
        /* create instance of a new DeviceList object which will automatically
         * populate itself with any devices present */
        DeviceList *pDeviceList;
        try
        {
            pDeviceList = new DeviceList(DeviceList::AUTOPOPULATE);
        }
        catch (std::bad_alloc& ba)
        {
            /* failed to allocate memory for new object */
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            pDeviceList = NULL;
        }

        /* return the list (may be NULL if failed to allocate) */
        return pDeviceList;
    }

    /**
    * Get a list of all devices present for the specified device type.
    *
    * @param deviceType The type of device to get device list for.
    *
    * @return A pointer to a new DeviceList object which contains the list of
    *         devices found or NULL if failed to create the device list.
    *
    * \note The DeviceList returned contains a snapshot of the current state of
    *       the system and is not automatically updated. Call the
    *       \link stardundee::com::starsystem::general::DeviceList::PopulateForType() DeviceList::PopulateForType() \endlink
    *       method on the list to re-populate its contents with a newer
    *       snapshot of the current state of the system.
    *
    * \note The object returned must be freed when no longer required using
    *       the C++ delete operator.
    */
    _Ret_opt_cap_(*count) static DeviceList* GetDeviceListForType(
        STAR_DEVICE_TYPE deviceType)
    {
        /* create empty instance of a new DeviceList object  */
        DeviceList *pDeviceList;
        try
        {
            pDeviceList = new DeviceList();
        }
        catch (std::bad_alloc& ba)
        {
            /* failed to allocate memory for new object */
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            pDeviceList = NULL;
        }

        /* if device list has been created */
        if(pDeviceList != NULL)
        {
            /* populate for the specified device type */
            pDeviceList->PopulateForType(deviceType);
        }

        /* return the list (may be NULL if failed to allocate) */
        return pDeviceList;
    }

    /**
    * Get a list of all devices present for the specified device types.
    *
    * @param deviceTypes The types of device to get device list for.
    * @param deviceTypeCount The number of devices in the given array.
    *
    * @return A pointer to a new DeviceList object which contains the list of
    *         devices found or NULL if failed to create the device list.
    *
    * \note The DeviceList returned contains a snapshot of the current state of
    *       the system and is not automatically updated. Call the
    *       'PopulateForTypes()' method on the list to re-populate its contents
    *       with a newer snapshot of the current state of the system.
    *
    * \note The object returned must be freed when no longer required using
    *       the C++ delete operator.
    */
    _Ret_opt_cap_(*count) static DeviceList* GetDeviceListForTypes(
        STAR_DEVICE_TYPE deviceTypes[], U32 deviceTypeCount)
    {
        /* create empty instance of a new DeviceList object  */
        DeviceList *pDeviceList;
        try
        {
            pDeviceList = new DeviceList();
        }
        catch (std::bad_alloc& ba)
        {
            /* failed to allocate memory for new object */
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            pDeviceList = NULL;
        }

        /* if device list has been created */
        if(pDeviceList != NULL)
        {
            /* populate for the specified device type */
            pDeviceList->PopulateForTypes(deviceTypes, deviceTypeCount);
        }

        /* return the list (may be NULL if failed to allocate) */
        return pDeviceList;
    }

    /**
    * Get a list of all drivers of the requested type(s).
    *
    * @param physicalDeviceDrivers Whether to include physical device drivers
                                    in the list.
    * @param virtualDeviceDrivers  Whether to include virtual device drivers in
                                    the list.
    * @param[out] count Number of driver identifiers held in the list.
    *
    * @return An array of pointers to Driver objects, representing the
    *         drivers which were found for the specified type(s).
    *
    * \note This function returns a snapshot of the current state of the
    *       the system and is not automatically updated.
    *
    * \note The array returned by this function and its contents must be freed
    *       when no longer required by using the C++ delete operator or by
    *       calling the following function: STARSystem::DestroyDriverList().
    */
    _Ret_opt_cap_(*count) static Driver**
    GetDriverList(int physicalDeviceDrivers,
                    int virtualDeviceDrivers,
                    _Out_ U32* count)
    {
        /* set empty / zero drivers  */
        Driver **driverList = NULL;
        *count = 0;

        /* get list of IDs of requested drivers from STAR API */
        U32 driverIDNum;
        STAR_DEVICE_ID *driverIDs = STAR_getDriverList(physicalDeviceDrivers,
                                                       virtualDeviceDrivers,
                                                       &driverIDNum);

        /* if drivers found */
        if (driverIDs != NULL)
        {
            /* try to allocate array of pointers to Drivers */
            try
            {
                driverList = new Driver*[driverIDNum];
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* error allocating memory */

                /* ensure empty driver list */
                driverList = NULL;
            }

            /* if driver list allocated successfully */
            if (driverList != NULL)
            {
                /* set number of drivers found */
                *count = driverIDNum;

                /* for each driver ID */
                for (U32 i=0; i<driverIDNum; i++)
                {
                    /* create a corresponding Driver object */
                    driverList[i] = new Driver(driverIDs[i]);
                }
            }

            /* free list of driver IDs obtained from STAR API */
            STAR_destroyDriverList(driverIDs);
        }

        /* return list of pointers to Drivers, may be NULL */
        return driverList;
    }

    /**
    * Destroy a given list of Driver objects.
    *
    * @param driverList An array of pointers to Driver objects to be freed.
    * @param count The number of elements in the array.
    *
    * @return 1 if list successfully freed, otherwise 0.
    */
    static int DestroyDriverList(Driver **driverList, U32 count)
    {
        /* if list is null */
        if (driverList == NULL)
        {
            /* return error */
            return 0;
        }

        /* for each item in the list */
        for (U32 i=0; i<count; i++)
        {
            /* if it isn't already freed */
            if (driverList[i] != NULL)
            {
                /* free it */
                delete driverList[i];
            }
        }

        /* free the list itself */
        delete [] driverList;

        /* return success */
        return 1;
    }

    /**
    * Returns a 'factory' object which can be used to create instances of
    * Driver objects.
    *
    * \note The factory object returned is required for certain internal
    *       processing steps in some of the C++ API functions. In such cases,
    *       the factory object is expected to be provided as a parameter. See
    *       example use below.
    *
    * \note The factory object returned must be freed when no longer required
    *       by using the C++ delete operator.
    *
    * @return A pointer to a new Driver factory object.
    *
    * \verbatim
    *       Sample use:
    *       DriverFactory *pDriverFactory;
    *       pDriverFactory = STARSystem::GetDriverFactory();
    *       if (pDriverFactory != NULL)
    *       {
    *           ..provide factory object to functions required..
    *
    *           ..delete when no longer required..
    *           delete pDriverFactory;
    *       }
    * \endverbatim
    *
    */
    static DriverFactory *GetDriverFactory()
    {
        /* create null driver factory */
        STARDriverFactory *newStarDriverFactory = NULL;

        /* try to create instance of new STARDriverFactory */
        try
        {
            newStarDriverFactory = new STARDriverFactory();
        }
        catch (std::bad_alloc& ba)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
            /* error allocating memory */
            newStarDriverFactory = NULL;

        }

        /* return the new driver factory, may be NULL */
        return newStarDriverFactory;
    }

    /**
    * Gets an array of all remote devices that have been created by all
    * processes.
    *
    * @param[out] count Number of devices in the array
    *
    * @return An array of pointers to RemoteDevice objects representing all
    *         devices found, or NULL if there are no remote devices present.
    *
    * \note The array returned by this function and its contents must be freed
    *       when no longer required by using the C++ delete operator or by
    *       calling the following function:
    *       STARSystem::DestroyRemoteDeviceList().
    */
    _Ret_opt_cap_(*count) static RemoteDevice**
                                 GetRemoteDeviceList(_Out_ U32* count)
    {
        /* set empty / zero Devices */
        RemoteDevice **deviceList = NULL;
        *count = 0;

        /* get list of IDs of all remote devices from STAR API */
        U32 deviceNum;
        STAR_DEVICE_ID *deviceIDs =
                          STAR_CFG_getRemoteDeviceDescriptionList(&deviceNum);

        /* if devices found */
        if (deviceIDs != NULL)
        {
            /* try to allocate array of pointers to RemoteDevices */
            try
            {
                deviceList = new RemoteDevice*[deviceNum];
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* error allocating memory */

                /* ensure empty device list */
                deviceList = NULL;
            }

            /* if devices allocated successfully */
            if (deviceList != NULL)
            {
                /* set number of remote devices found */
                *count = deviceNum;

                /* for each remote device ID */
                for (U32 i=0; i<deviceNum; i++)
                {
                    /* create a corresponding RemoteDevice object, add to
                     * list */
                    deviceList[i] = new RemoteDevice(deviceIDs[i]);
                }
            }

            /* free the list of device IDs obtained from the 'c' API */
            STAR_CFG_destroyRemoteDeviceDescriptionList(deviceIDs);
        }

        /* return list of pointers to RemoteDevices, may be NULL */
        return deviceList;
    }

    /**
    * Registers a Call-back object that will receive notifications of whenever
    * a device is added or removed for a specified driver.
    *
    * @param driverID The ID of the driver to receive notifications for.
    * @param pCallBackObj A pointer to a user-supplied object which will be
    *                     registered to receive notifications of devices being
    *                     added or removed for the specified driver.
    * @param notifyForCurrent Specifies whether notifications should be provided
    *                         for all existing devices, to indicate that they
    *                         have been added.
    *
    * @return A pointer to a new DeviceListener object which identifies the listener
    *         registered and can be used to unregister the listener when it is
    *         no longer required, OR NULL if failed to register the listener or
    *         if a memory allocation failure occurred.
    *
    * \note A DeviceListener must be unregistered when no longer required. This
    *       can be performed when required by calling the
    *       DeviceListener::Unregister function. Alternatively, the
    *       DeviceListener will be automatically unregistered if it hasn't been
    *       already when the object goes out of scope or is deleted, i.e. in
    *       the object destructor.
    *
    * \note The DeviceListener object returned must be freed when no longer
    *       required by calling the C++ delete operator.
    *
    */
    static DeviceListener* RegisterDeviceListenerForDriver(
                                          STAR_DRIVER_ID driverID,
                                          DeviceListenerCallback *pCallBackObj,
                                          int notifyForCurrent)
    {
        /* call standard 'c' API to register device listener for driver */
        STAR_DEVICE_LISTENER_ID deviceListenerID =
                            STAR_registerDeviceListenerForDriver(driverID,
                            STARSystem::DeviceListenerCallBack,
                            pCallBackObj, notifyForCurrent);

        /* if listener successfully registered */
        if (deviceListenerID)
        {
            /* create listener to return to user which contains the listener
             * ID (required later to unregister the listener) */
            DeviceListener *deviceListener;
            try
            {
                deviceListener = new DeviceListener(deviceListenerID);
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory allocation failure */
                deviceListener = NULL;

                /* unregister the device listener */
                STAR_unregisterDeviceListener(deviceListenerID);
            }

            /* return listener */
            return deviceListener;
        }
        else
        {
            /* return error / NULL */
            return NULL;
        }
    }

    /**
    * Registers a Call-back object that will receive notifications of whenever
    * any channel is opened or closed.
    *
    * @param pCallBackObj A pointer to a user-supplied object which will be
    *                     registered to receive notifications of any channels
    *                     being opened or closed.
    * @param notifyForCurrent Specifies whether notifications should be provided
    *                         for all existing channels, to indicate that they
    *                         have been opened.
    *
    * @return A pointer to a new \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *         object which identifies the listener registered and can be used
    *         to unregister the listener when it is no longer required, OR NULL
    *         if failed to register the listener or if a memory allocation
    *         failure occurred.
    *
    * \note A \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *       must be unregistered when no longer required. This can be performed
    *       when required by calling the \link stardundee::com::starsystem::channels::ChannelListener::Unregister() Unregister() \endlink
    *       function on the \link stardundee::com::starsystem::channels::ChannelListener ChannelListener\endlink.
    *       Alternatively, the \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *       will be automatically unregistered if it hasn't been already when
    *       the object goes out of scope or is deleted, i.e. in the object
    *       destructor.
    *
    * \note The \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *       object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    static stardundee::com::starsystem::channels::ChannelListener*
    RegisterChannelListener(
    stardundee::com::starsystem::channels::ChannelListenerCallback *pCallBackObj,
    int notifyForCurrent)
    {
        /* call standard 'c' API to register channel listener for whenever
         * any channels are opened or closed */
        STAR_CHANNEL_LISTENER_ID channelListenerID =
                            STAR_registerChannelListener(
                            STARSystem::ChannelListenerCallBack,
                            pCallBackObj, notifyForCurrent);

        /* if listener successfully registered */
        if (channelListenerID)
        {
            /* create listener to return to user which contains the listener
             * ID (required later to unregister the listener) */
            stardundee::com::starsystem::channels::ChannelListener
                                                  *channelListener;
            try
            {
                channelListener = new
                stardundee::com::starsystem::channels::ChannelListener(channelListenerID);
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory allocation failure */
                channelListener = NULL;

                /* unregister the channel listener */
                STAR_unregisterChannelListener(channelListenerID);
            }

            /* return listener */
            return channelListener;
        }
        else
        {
            /* return error / NULL */
            return NULL;
        }
    }

    /**
    * Registers a Call-back object that will receive notifications of whenever
    * a channel is opened or closed on a specified device.
    *
    * @param deviceID The ID of the device to monitor channel activity on.
    * @param pCallBackObj A pointer to a user-supplied object which will be
    *                     registered to receive notifications of channels
    *                     being opened or closed on the specified device.
    * @param notifyForCurrent Specifies whether notifications should be provided
    *                         for all existing channels, to indicate that they
    *                         have been opened.
    *
    * @return A pointer to a new \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *         object which identifies the listener registered and can be used
    *         to unregister the listener when it is no longer required, OR NULL
    *         if failed to register the listener or if a memory allocation
    *         failure occurred.
    *
    * \note A \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *       must be unregistered when no longer required. This can be performed
    *       when required by calling the
    *       \link stardundee::com::starsystem::channels::ChannelListener::Unregister() ChannelListener::Unregister()\endlink function.
    *       Alternatively, the \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *       will be automatically unregistered if it hasn't been already when
    *       the object goes out of scope or is deleted, i.e. in the object
    *       destructor.
    *
    * \note The \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *       object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    static stardundee::com::starsystem::channels::ChannelListener*
    RegisterChannelListenerForDevice(STAR_DEVICE_ID deviceID,
    stardundee::com::starsystem::channels::ChannelListenerCallback *pCallBackObj,
    int notifyForCurrent)
    {
        /* call 'c' API to register channel listener for device */
        STAR_CHANNEL_LISTENER_ID channelListenerID =
                                STAR_registerChannelListenerForDevice(deviceID,
                                STARSystem::ChannelListenerCallBack,
                                pCallBackObj, notifyForCurrent);

        /* if listener successfully registered */
        if (channelListenerID)
        {
            /* create listener to return to user which contains the listener
             * ID (required later to unregister the listener) */
            stardundee::com::starsystem::channels::ChannelListener
                                                  *channelListener;
            try
            {
                channelListener = new
                stardundee::com::starsystem::channels::ChannelListener(channelListenerID);
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory allocation failure */
                channelListener = NULL;

                /* unregister the channel listener */
                STAR_unregisterChannelListener(channelListenerID);
            }

            /* return listener */
            return channelListener;
        }
        else
        {
            /* return error / NULL */
            return NULL;
        }
    }

    /**
    * Registers a Call-back object that will be called whenever a channel
    * is opened or closed on a device for a specified driver.
    *
    * @param driverID The ID of the driver to monitor channel activity for.
    * @param pCallBackObj A pointer to a user-supplied object which will be
    *                     registered to receive notifications of channels
    *                     being opened or closed on devices for the specified
    *                     driver.
    * @param notifyForCurrent Specifies whether notifications should be provided
    *                         for all existing channels, to indicate that they
    *                         have been opened.
    *
    * @return A pointer to a new \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *         object which identifies the listener registered and can be used
    *         to unregister the listener when it is no longer required, OR NULL
    *         if failed to register the listener or if a memory allocation
    *         failure occurred.
    *
    * \note A \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *       must be unregistered when no longer required. This can be performed
    *       when required by calling the \link stardundee::com::starsystem::channels::ChannelListener::Unregister() ChannelListener::Unregister() \endlink function.
    *       Alternatively, the \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *       will be automatically unregistered if it hasn't been already when
    *       the object goes out of scope or is deleted, i.e. in the object
    *       destructor.
    *
    * \note The \link stardundee::com::starsystem::channels::ChannelListener ChannelListener \endlink
    *       object returned must be freed when no longer required by calling
    *       the C++ delete operator.
    *
    */
    static stardundee::com::starsystem::channels::ChannelListener*
    RegisterChannelListenerForDriver(STAR_DRIVER_ID driverID,
    stardundee::com::starsystem::channels::ChannelListenerCallback *pCallBackObj,
    int notifyForCurrent)
    {
        /* call 'c' API to register channel listener for driver */
        STAR_CHANNEL_LISTENER_ID channelListenerID =
                                STAR_registerChannelListenerForDriver(driverID,
                                STARSystem::ChannelListenerCallBack,
                                pCallBackObj, notifyForCurrent);

        /* if listener successfully registered */
        if (channelListenerID)
        {
            /* create listener to return to user which contains the listener
             * ID (required later to unregister the listener) */
            stardundee::com::starsystem::channels::ChannelListener
                                                   *channelListener;
            try
            {
                channelListener = new
                stardundee::com::starsystem::channels::ChannelListener(channelListenerID);
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory allocation failure */
                channelListener = NULL;

                /* unregister the channel listener */
                STAR_unregisterChannelListener(channelListenerID);
            }

            /* return listener */
            return channelListener;
        }
        else
        {
            /* return error / NULL */
            return NULL;
        }
    }

    /**
    * Registers a Call-back object that will be called whenever a device
    * is added or removed.
    *
    * @param pCallBackObj A pointer to a user-supplied object which will be
    *                     registered to receive notifications of any devices
    *                     being added or removed.
    * @param notifyForCurrent Specifies whether notifications should be provided
    *                         for all existing devices, to indicate that they
    *                         have been added.
    *
    * @return A pointer to a new DeviceListener object which identifies the
    *         listener registered and can be used to unregister the listener
    *         when it is no longer required, OR NULL if failed to register the
    *         listener or if a memory allocation failure occurred.
    *
    * \note A DeviceListener must be unregistered when no longer required.
    *       This can be performed when required by calling the
    *       DeviceListener::Unregister function. Alternatively, the
    *       DeviceListener will be automatically unregistered if it hasn't
    *       been already when the object goes out of scope or is deleted, i.e.
    *       in the object destructor.
    *
    * \note The DeviceListener object returned must be freed when no longer
    *       required by calling the C++ delete operator.
    *
    */
    static DeviceListener* RegisterDeviceListener(
                                     DeviceListenerCallback *pCallBackObj,
                                     int notifyForCurrent)
    {
        /* call 'c' API to register device listener */
        STAR_DEVICE_LISTENER_ID deviceListenerID = STAR_registerDeviceListener(
                        STARSystem::DeviceListenerCallBack,
                        pCallBackObj, notifyForCurrent);

        /* if listener successfully registered */
        if (deviceListenerID)
        {
            /* create listener to return to user which contains the listener
             * ID (required later to unregister the listener) */
            DeviceListener *deviceListener;
            try
            {
                deviceListener = new DeviceListener(deviceListenerID);
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory allocation failure */
                deviceListener = NULL;

                /* unregister the device listener */
                STAR_unregisterDeviceListener(deviceListenerID);
            }

            /* return listener */
            return deviceListener;
        }
        else
        {
            /* return error / NULL */
            return NULL;
        }
    }

    /**
    * Registers a Call-back object that will be called whenever a specifed
    * device is removed.
    *
    * @param deviceID The device to register a listener for.
    * @param pCallBackObj A pointer to a user-supplied object which will be
    *                     registered to receive a notifications of when the
    *                     specified device is removed.
    *
    * @return A pointer to a new DeviceListener object which identifies the
    *         listener registered and can be used to unregister the listener
    *         when it is no longer required, OR NULL if failed to register the
    *         listener or if a memory allocation failure occurred.
    *
    * \note A DeviceListener must be unregistered when no longer required.
    *       This can be performed when required by calling the
    *       DeviceListener::Unregister function. Alternatively, the
    *       DeviceListener will be automatically unregistered if it hasn't
    *       been already when the object goes out of scope or is deleted, i.e.
    *       in the object destructor.
    *
    * \note The DeviceListener object returned must be freed when no longer
    *       required by calling the C++ delete operator.
    *
    */
    static DeviceListener*
    RegisterDeviceListenerForDevice(STAR_DEVICE_ID deviceID,
                                    DeviceListenerCallback *pCallBackObj)
    {
        /* call 'c' API to register device listener for device */
        STAR_DEVICE_LISTENER_ID deviceListenerID =
                                 STAR_registerDeviceListenerForDevice(deviceID,
                                            STARSystem::DeviceListenerCallBack,
                                            pCallBackObj);

        /* if listener successfully registered */
        if (deviceListenerID)
        {
            /* create listener to return to user which contains the listener
             * ID (required later to unregister the listener) */
            DeviceListener *deviceListener;
            try
            {
                deviceListener = new DeviceListener(deviceListenerID);
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory allocation failure */
                deviceListener = NULL;

                /* unregister the device listener */
                STAR_unregisterDeviceListener(deviceListenerID);
            }

            /* return listener */
            return deviceListener;
        }
        else
        {
            /* return error / NULL */
            return NULL;
        }
    }

    /**
    * Registers a Call-back object that will be called whenever a driver
    * is added or removed.
    *
    * @param pCallBackObj A pointer to a user-supplied object which will be
    *                     registered to receive notifications of any drivers
    *                     being added or removed.
    * @param notifyForCurrent Specifies whether notifications should be provided
    *                         for all existing drivers, to indicate that they
    *                         have been added.
    *
    * @return A pointer to a new DriverListener object which identifies the
    *         listener registered and can be used to unregister the listener
    *         when it is no longer required, OR NULL if failed to register the
    *         listener or if a memory allocation failure occurred.
    *
    * \note A DriverListener must be unregistered when no longer required.
    *       This can be performed when required by calling the
    *       DriverListener::Unregister function. Alternatively, the
    *       DriverListener will be automatically unregistered if it hasn't
    *       been already when the object goes out of scope or is deleted, i.e.
    *       in the object destructor.
    *
    * \note The DriverListener object returned must be freed when no longer
    *       required by calling the C++ delete operator.
    *
    */
    static DriverListener* RegisterDriverListener(
                                     DriverListenerCallback *pCallBackObj,
                                     int notifyForCurrent)
    {
        /* call 'c' API to register driver listener */
        STAR_DRIVER_LISTENER_ID driverListenerID =
            STAR_registerDriverListener(STARSystem::DriverListenerCallBack,
                                        pCallBackObj, notifyForCurrent);

        /* if listener successfully registered */
        if (driverListenerID)
        {
            /* create listener to return to user which contains the listener
             * ID (required later to unregister the listener) */
            DriverListener *driverListener;
            try
            {
                driverListener = new DriverListener(driverListenerID);
            }
            catch (std::bad_alloc& ba)
            {
                _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);
                /* memory allocation failure */
                driverListener = NULL;

                /* unregister the driver listener */
                STAR_unregisterDriverListener(driverListenerID);
            }

            /* return listener */
            return driverListener;
        }
        else
        {
            /* return error / NULL */
            return NULL;
        }
    }

    /**
    * Registers a call-back object that will be called when a given transfer
    * operation completes.
    *
    * @param pTransferOp The transfer operation for which a transfer completion
    *                    listener is to be registered.
    * @param pCallBackObj A pointer to a user-supplied object which will be
    *                     registered to receive notifications of when the given
    *                     transfer operation completes.
    *
    * @return A pointer to a new \link stardundee::com::starsystem::transferoperations::TransferCompletionListener TransferCompletionListener \endlink
    *         object which identifies the listener registered and can be used
    *         to unregister the listener when it is no longer required, OR NULL
    *         if failed to register the listener or if a memory allocation
    *         failure occurred.
    *
    * \note A \link stardundee::com::starsystem::transferoperations::TransferCompletionListener TransferCompletionListener \endlink
    *       must be unregistered when no longer required. This can be performed
    *       when required by calling the \link stardundee::com::starsystem::transferoperations::TransferCompletionListener::Unregister() TransferCompletionListener::Unregister() \endlink function.
    *       Alternatively, the \link stardundee::com::starsystem::transferoperations::TransferCompletionListener TransferCompletionListener \endlink
    *       will be automatically unregistered if it hasn't been already when
    *       the object goes out of scope or is deleted, i.e. in the object
    *       destructor.
    *
    * \note The \link stardundee::com::starsystem::transferoperations::TransferCompletionListener TransferCompletionListener \endlink
    *       object returned must be freed when no longer required by calling the
    *       C++ delete operator.
    */
    static stardundee::com::starsystem::transferoperations::TransferCompletionListener*
    RegisterTransferCompletionListener(
    stardundee::com::starsystem::transferoperations::TransferOperation *pTransferOp,
    stardundee::com::starsystem::transferoperations::TransferCompletionListenerCallback *pCallBackObj)
    {
        /* if valid transfer op provided */
        if (pTransferOp)
        {
            /* get the STAR_TRANSFER_OPERATION for the TransferOperation.
             * This needs to be provided to the 'c' API */
            STAR_TRANSFER_OPERATION *pSTAR_TRANSFER_OPERATION;
            pSTAR_TRANSFER_OPERATION = pTransferOp->GetSTAR_TRANSFER_OPERATION();

            /* if valid STAR_TRANSFER_OPERATION obtained */
            if (pSTAR_TRANSFER_OPERATION)
            {
                /* call 'c' API to register transfer completion listener */
                int result = STAR_registerTransferCompletionListener(
                                 pSTAR_TRANSFER_OPERATION,
                                 STARSystem::TransferCompletionListenerCallBack,
                                 pCallBackObj);

                /* if listener registered successfully */
                if (result)
                {
                    /* create listener to return to user which contains the
                     * data required to later unregister the listener */
                    stardundee::com::starsystem::transferoperations::TransferCompletionListener *pListener;
                    try
                    {
                        pListener = new
                        stardundee::com::starsystem::transferoperations::TransferCompletionListener(
                                pTransferOp, pSTAR_TRANSFER_OPERATION,
                                STARSystem::TransferCompletionListenerCallBack,
                                pTransferOp->GetReferenceCount());
                    }
                    catch (std::bad_alloc& ba)
                    {
                        _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(ba);

                        /* memory allocation failure */
                        pListener = NULL;

                        /* unregister the listener */
                        STAR_unregisterTransferCompletionListener(
                                pSTAR_TRANSFER_OPERATION,
                                STARSystem::TransferCompletionListenerCallBack);
                    }

                    /* provide transfer op with a reference to the callback
                     * function which is used to receive transfer completion
                     * events, it may require this during clean-up stages */
                    pTransferOp->SetTransferCompletionCallback(
                        STARSystem::TransferCompletionListenerCallBack);

                    /* return listener */
                    return pListener;
                }
                else
                {
                    /* return error / failed to register listener*/
                    return NULL;
                }
            }
            else
            {
                /* return error / failed to register listener*/
                return NULL;
            }
        }
        else
        {
            /* return error / failed to register listener*/
            return NULL;
        }
    }

    /**
    * Get the device with the given ID.
    *
    * \note This is for use with local and virtual devices only, not Remote
    *       devices.
    *
    * @param deviceID The ID of the device being sought.
    *
    * @return A new Device object representing the device sought or NULL if
    *         failed to obtain the device (e.g. invalid deviceID) or if failed
    *         to allocate memory for the Device object.
    *
    * \note The object returned must be freed when no longer required by using
    *       the C++ delete operator.
    */
    static Device *GetDevice(STAR_DEVICE_ID deviceID)
    {
        return DeviceFactory::CreateDevice(deviceID);
    }
};

/* end namespace tags */
} /* general */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

