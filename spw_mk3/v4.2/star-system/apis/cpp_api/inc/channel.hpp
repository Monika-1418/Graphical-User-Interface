/**
 * \file channel.hpp
 *
 * \brief Represents a channel which may be opened on a device in order to
 *        send and/or receive stream items.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Represents a channel which may be opened on a device in order to send and/or
 * receive stream items.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 *
 * \ingroup CPP_CHANNELS
 */

#ifndef __STAR_CHANNEL_CPP__
#define __STAR_CHANNEL_CPP__

#include "star-api.h"
#include "transfer_operation.hpp"
#include "packet.hpp"
#include <new>
#include "cpp_api_macros.hpp"

/* support for 'atomic' operations, e.g. increment or decrement of values
 * which may be shared amongst several threads */
#if defined(_WIN32)
    #include <windows.h>
    #define STAR_CPP_ATOMIC_TYPE LONG
#elif (defined(__linux__) || defined(__CYGWIN__) || defined(__APPLE__))
    #ifndef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
        #warning "Atomic compare and swap not supported natively in this version of GCC"
    #endif
    #include <limits.h>
    #if (INT_MAX == 0x7FFFFFFF)
        #define STAR_CPP_ATOMIC_TYPE int
    #elif (LONG_MAX == 0x7FFFFFFF)
        #define STAR_CPP_ATOMIC_TYPE long
    #elif (SHRT_MAX == 0x7FFFFFFF)
        #define STAR_CPP_ATOMIC_TYPE short
    #elif (CHAR_MAX == 0x7FFFFFFF)
        #define STAR_CPP_ATOMIC_TYPE char
    #else
        #error "No valid definition of STAR_CPP_ATOMIC_TYPE available"
    #endif
#elif defined(__QNX__)
    #include <atomic.h>
    #include _NTO_CPU_HDR_(smpxchg.h)
    #define STAR_CPP_ATOMIC_TYPE    volatile unsigned
#elif defined(__vxworks)
    #include <intLib.h>
    #define STAR_CPP_ATOMIC_TYPE    int
#endif

namespace stardundee
{
namespace com
{
namespace starsystem
{
/**
 * Classes for the Channels which can be opened on a Device to
 * transmit and receive data over a SpaceWire link and to listen for
 * channel events.
 **/
namespace channels {

/**
* Helper class used during channel processing which holds a reference count for
* and the status of an opened channel.
*
* \note This is an internal API class and is not required when using the API.
*
* There are situations where it may be possible for several different copies of
* a Channel object to share a reference to the same underlying channel. In such
* cases it is necessary to maintain a reference count of the shared channel and
* to manage the status of the channel appropriately. For example, when the
* underlying channel is closed by one of its shared owners, the other shared
* owners must be able to determine this change of state too.
*
*/
class ChannelReferenceCount
{
private:
    /** the reference count for a channel - this will be treated as an 'atomic'
     * value as it may be accessible by different threads */
    STAR_CPP_ATOMIC_TYPE referenceCount;

    /** records whether the channel is open (1) or not (0) */
    int isOpen;

public:
    /**
    * Default constructor
    */
    ChannelReferenceCount() : referenceCount(0), isOpen(0)
    {
    }

    /**
    * Set the status to indicate an open channel
    */
    void SetOpen()
    {
        isOpen = 1;
    }

    /**
    * Set the status to indicate a closed channel
    */
    void SetClosed()
    {
        isOpen = 0;
    }

    /**
    * Get whether the channel is open.
    *
    * @return Whether the channel is open (1) or not (0)
    */
    int IsOpen()
    {
        return isOpen;
    }

    /**
    * Increment the reference count for the channel, and return the updated
    * value.
    *
    * @return The new value of the reference count
    *
    * \note This in an 'atomic' operation, i.e. designed to be thread-safe
    *       with regards to accessing the reference count for the channel.
    */
    STAR_CPP_ATOMIC_TYPE IncrementReferenceCount()
    {
#if defined(_WIN32)
        return InterlockedIncrement(&referenceCount);
#elif (defined(__linux__) || defined(__CYGWIN__))
        return __sync_add_and_fetch(&referenceCount, 1);
#elif defined(__QNX__)
        return atomic_add_value(&referenceCount, 1) + 1;
#elif defined(__vxworks)
        int lockkey = intLock();
        referenceCount++;
        intUnlock(lockkey);
        return referenceCount;
#endif
    }

    /**
    * Decrement the reference count for the channel, and return the updated
    * value.
    *
    * @return The new value of the reference count
    *
    * \note This in an 'atomic' operation, i.e. designed to be thread-safe
    *       with regards to accessing the reference count for the channel.
    */
    STAR_CPP_ATOMIC_TYPE DecrementReferenceCount()
    {
#if defined(_WIN32)
        return InterlockedDecrement(&referenceCount);
#elif (defined(__linux__) || defined(__CYGWIN__))
        return __sync_add_and_fetch(&referenceCount, -1);
#elif defined(__QNX__)
        return atomic_sub_value(&referenceCount, 1) - 1;
#elif defined(__vxworks)
        int lockkey = intLock();
        referenceCount--;
        intUnlock(lockkey);
        return referenceCount;
#endif
    }
};

/**
* Represents a channel which may be opened on a device in order to send and/or
* receive stream items.
*/
class Channel
{
public:
    /** channel directions */
    enum ChannelDirection
    {
        /** channel may be used to receive traffic */
        DIRECTION_IN = STAR_CHANNEL_DIRECTION_IN,
        /** channel may be used to transmit traffic */
        DIRECTION_OUT = STAR_CHANNEL_DIRECTION_OUT,
        /** channel may be used to both receive and transmit traffic */
        DIRECTION_INOUT = STAR_CHANNEL_DIRECTION_INOUT
    };

    /** channel types */
    enum ChannelType
    {
        /** channel not open */
        CHANNEL_TYPE_NOT_OPEN = STAR_CHANNEL_TYPE_NOT_OPEN,
        /** attached to a device */
        CHANNEL_TYPE_DEVICE = STAR_CHANNEL_TYPE_DEVICE,
        /** attached to an application */
        CHANNEL_TYPE_APPLICATION = STAR_CHANNEL_TYPE_APPLICATION
    };

protected:
    /** the ID of the channel */
    STAR_CHANNEL_ID channelID;

    /** the channel number */
    U8 channelNumber;

    /** the reference count and status of the channel */
    ChannelReferenceCount *pChannelReferenceCountandStatus;

    /** record whether the channel is closeable. The default is True - any
     *  channel is inherently closeable. However, there may be situations where
     *  this isn't the case and where the user should be able to query the
     *  status of the channel and use the channel (if it is open) but not close
     *  it. In which case the value of this flag will be 'false' */
    bool isCloseable;

public:

    /**
    * Default constructor. Initialise object to default state.
    */
    Channel() : channelID(0), channelNumber(0),
                pChannelReferenceCountandStatus(NULL),
                isCloseable(true)
    {
        /* set as empty channel */
    }

    /**
    * Overloaded constructor. Initialise object to given channel ID.
    *
    * \note A valid channel ID is obtained when a channel is opened. It is not
    *       usually necessary for the user to process channel ID values
    *       directly, instead they are used internally by the C++ API. Provision
    *       of a valid channel ID to this constructor automatically puts this
    *       channel object into an 'open' state, and assumes the channel can be
    *       used for transmitting and/or receiving traffic.
    *
    * @param id The ID of the channel which has been opened.
    */
    explicit Channel(STAR_CHANNEL_ID id)  : channelID(id), channelNumber(0),
                                   pChannelReferenceCountandStatus(NULL),
                                   isCloseable(true)
    {
        /* if valid channel id (e.g. non-zero) */
        if (id)
        {
            /* create reference count for channel and set status to open */
            pChannelReferenceCountandStatus = new ChannelReferenceCount();
            pChannelReferenceCountandStatus->IncrementReferenceCount();
            pChannelReferenceCountandStatus->SetOpen();
        }
    }

    /**
    * Overloaded constructor. Initialise object to given channel ID and given
    *                         channel number;
    *
    * \note A valid channel ID is obtained when a channel is opened. It is not
    *       usually necessary for the user to process channel ID values
    *       directly, instead they are used internally by the C++ API. Provision
    *       of a valid channel ID to this constructor automatically puts this
    *       channel object into an 'open' state, and assumes the channel can be
    *       used for transmitting and/or receiving traffic.
    *
    * @param id The ID of the channel which has been opened.
    * @param channelNum The channel number.
    *
    */
    Channel(STAR_CHANNEL_ID id, U8 channelNum)
                                 : channelID(id), channelNumber(channelNum),
                                   pChannelReferenceCountandStatus(NULL),
                                   isCloseable(true)
    {
        /* if valid channel id (e.g. non-zero) */
        if (id)
        {
            /* create reference count for channel and set status to open */
            pChannelReferenceCountandStatus = new ChannelReferenceCount();
            pChannelReferenceCountandStatus->IncrementReferenceCount();
            pChannelReferenceCountandStatus->SetOpen();
        }
    }

    /**
    * Overloaded constructor. Initialise object to given channel ID and given
    *                         channel number, and specifying whether the
    *                         channel is closeable or not.
    *
    * \note A valid channel ID is obtained when a channel is opened. It is not
    *       usually necessary for the user to process channel ID values
    *       directly, instead they are used internally by the C++ API. Provision
    *       of a valid channel ID to this constructor automatically puts this
    *       channel object into an 'open' state, and assumes the channel can be
    *       used for transmitting and/or receiving traffic.
    *
    * @param id The ID of the channel which has been opened.
    * @param channelNum The channel number.
    * @param closeable Whether the channel is closeable (true) or not (false)
    *
    */
    Channel(STAR_CHANNEL_ID id, U8 channelNum, bool closeable)
                                 : channelID(id), channelNumber(channelNum),
                                   pChannelReferenceCountandStatus(NULL),
                                   isCloseable(closeable)
    {
        /* if valid channel id (e.g. non-zero) */
        if (id)
        {
            /* create reference count for channel and set status to open */
            pChannelReferenceCountandStatus = new ChannelReferenceCount();
            pChannelReferenceCountandStatus->IncrementReferenceCount();
            pChannelReferenceCountandStatus->SetOpen();
        }
    }

    /**
    * Overloaded constructor. Initialise object to given channel ID and given
    *                         channel number, and specifying whether the
    *                         channel is closeable or not and also specifying
    *                         whether the channel is currently open or not.
    *
    * \note A valid channel ID is obtained when a channel is opened. It is not
    *       usually necessary for the user to process channel ID values
    *       directly, instead they are used internally by the C++ API. Provision
    *       of a valid channel ID to this constructor automatically puts this
    *       channel object into an 'open' state, and assumes the channel can be
    *       used for transmitting and/or receiving traffic.
    *
    * @param id The ID of the channel which has been opened.
    * @param channelNum The channel number.
    * @param closeable Whether the channel is closeable (true) or not (false)
    * @param isOpen Whether the channel is currently open (true) or not (false).
    *
    */
    Channel(STAR_CHANNEL_ID id, U8 channelNum, bool closeable, bool isOpen)
                                 : channelID(id), channelNumber(channelNum),
                                   pChannelReferenceCountandStatus(NULL),
                                   isCloseable(closeable)
    {
        /* if valid channel id (e.g. non-zero) */
        if (id)
        {
            /* create reference count for channel and set status to open */
            pChannelReferenceCountandStatus = new ChannelReferenceCount();
            pChannelReferenceCountandStatus->IncrementReferenceCount();

            /* if the channel is open */
            if (isOpen)
            {
                /* set its status to open */
                pChannelReferenceCountandStatus->SetOpen();
            }
            else
            {
                /* set its status to closed */
                pChannelReferenceCountandStatus->SetClosed();
            }
        }
    }

    /**
    * Destructor. Free any resources created by object.
    *
    * If a Channel is still open it will NOT automatically be closed. It is up
    * to the user to close a channel when they no longer wish to use it by
    * calling the Channel.CloseChannel() function.
    */
    ~Channel()
    {
        /* if a reference count exists for the channel */
        if (pChannelReferenceCountandStatus)
        {
            /* decrement reference count */
            STAR_CPP_ATOMIC_TYPE refCount =
                pChannelReferenceCountandStatus->DecrementReferenceCount();

            /* if reference count is less than 1 */
            if (refCount < 1)
            {
                /* last reference to channel has now been removed */

                /* if the channel is still open */
                if (IsOpen())
                {
                    /* if the channel is closeable by this object */
                    if (isCloseable)
                    {
                        /* close the channel since nothing else refers to it
                         * now */

                        /* REVISION: channel should not be closed automatically
                         * upon object destruction. It is up to the user to
                         * ensure they close channels when they no longer
                         * require them by calling Channel.CloseChannel(). The
                         * following line of code has been commented out:
                         *
                         * STAR_closeChannel(this->channelID);
                         *
                         */
                    }
                }

                /* destroy the channel reference count */
                delete pChannelReferenceCountandStatus;
            }
        }
    }

    /**
    * Copy constructor - to allow copies of Channel objects to be made when
    * being passed by value in functions or returned by value from functions.
    *
    * Channels are limited resources on a \link stardundee::com::starsystem::general::Device Device \endlink.
    * When a copy of an existing Channel is made, the copy shares a reference
    * to the existing Channel. This is rather than attempting to create a new
    * Channel which is a duplicate of that being copied. A reference counting
    * mechanism is employed to manage shared references to a channel.
    */
    Channel(const Channel& channelToCopyFrom)
    {
        /* if channel being copied from holds a valid channel */
        if (channelToCopyFrom.channelID)
        {
            /* copy the channel's ID and number */
            channelID = channelToCopyFrom.channelID;
            channelNumber = channelToCopyFrom.channelNumber;

            /* copy the pointer to the channel reference count */
            pChannelReferenceCountandStatus =
                channelToCopyFrom.pChannelReferenceCountandStatus;

            /* increment the reference count by 1 */
            pChannelReferenceCountandStatus->IncrementReferenceCount();

            /* copy the channel's 'closeable' status */
            isCloseable = channelToCopyFrom.isCloseable;
        }
        else
        {
            /* no useful channel data to copy */
            channelID = 0;
            channelNumber = 0;
            pChannelReferenceCountandStatus = NULL;
            isCloseable = true;
        }
    }

    /**
    * Overloaded assignment operator - to allow instances of Channel objects to
    * have their contents assigned to each other.
    *
    * Channels are limited resources on a \link stardundee::com::starsystem::general::Device Device \endlink.
    * When an existing Channel is assigned to another, they will both share a
    * reference to the same underlying channel. This is rather than attempting
    * to create a new Channel which is a duplicate of the channel being
    * assigned. Any existing reference to a channel held by this object will be
    * overwritten with a reference to the new channel being assigned. The
    * existing channel will not automatically be closed if it is still open
    * before being overwritten. Therefore, please ensure the existing channel
    * is closed if required.
    */
    Channel& operator=(const Channel& channelBeingAssignedFrom)
    {
        /* check for self-assignment, no point in copying one's self */
        if (this == &channelBeingAssignedFrom)
            return *this;

        /* if 'this' object already holds information about a channel */
        if (channelID)
        {
            /* decrement reference count */
            STAR_CPP_ATOMIC_TYPE refCount =
                pChannelReferenceCountandStatus->DecrementReferenceCount();

            /* if reference count is less than 1 */
            if (refCount < 1)
            {
                /* last reference to channel has now been removed */

                /* if the channel is still open */
                if (IsOpen())
                {
                    /* if the channel is closeable by this object */
                    if (isCloseable)
                    {
                        /* close the channel since nothing else refers to it
                         * now */

                        /* REVISION: channel should not be closed automatically.
                         * It is up to the user to ensure they close channels
                         * when they no longer require them by calling
                         * Channel.CloseChannel(). The following line of code
                         * has been commented out:
                         *
                         * STAR_closeChannel(this->channelID);
                         *
                         */
                    }
                }

                /* destroy the channel reference count */
                delete pChannelReferenceCountandStatus;
            }

            /* set this object's pointer to the reference count to
             * NULL since it has freed its interest in the reference
             * count it had */
            pChannelReferenceCountandStatus = NULL;

            /* reset the channel data **/
            channelID = 0;
            channelNumber = 0;
            isCloseable = true;
        }

        /* if channel being assigned to 'this' holds a valid channel */
        if (channelBeingAssignedFrom.channelID)
        {
            /* copy the channel's ID and number */
            channelID = channelBeingAssignedFrom.channelID;
            channelNumber = channelBeingAssignedFrom.channelNumber;

            /* copy the pointer to the channel reference count */
            pChannelReferenceCountandStatus =
                channelBeingAssignedFrom.pChannelReferenceCountandStatus;

            /* increment the reference count by 1 */
            pChannelReferenceCountandStatus->IncrementReferenceCount();

            /* copy the channel's 'closeable' status */
            isCloseable = channelBeingAssignedFrom.isCloseable;
        }

        else
        {
            /* no useful channel data to copy */
        }

        /* return a reference to this object, to allow chaining of the
         * assignment operator.
         */
        return *this;
    }

    /**
    * Get the channel's ID.
    *
    * @return The ID of the channel.
    *
    */
    STAR_CHANNEL_ID GetChannelID() const
    {
        /* return the ID */
        return channelID;
    }

    /**
    * Get the channel number.
    *
    * @return The number of the channel.
    *
    */
    U8 GetChannelNumber()
    {
        /* return the channel number */
        return channelNumber;
    }

    /**
    * Gets whether the channel is open or not.
    *
    * @return 1, if the channel is open, otherwise 0.
    *
    */
    int IsOpen()
    {
        /* if a reference count exists for the channel */
        if (pChannelReferenceCountandStatus)
        {
            /* return the status held for the channel in the reference count */
            return pChannelReferenceCountandStatus->IsOpen();
        }
        else
        {
            /* the channel can only be open if it has a pointer to a valid
             * channel reference count - so it can't be open */
            return 0;
        }
    }

    /**
    * Close the channel, which has previously been opened.
    *
    * @return 1 if the channel was successfully closed, otherwise 0 if failed
    *         to close the channel OR if there is no channel to close, OR if
    *         this object doesn't have 'closeable' rights for the channel - this
    *         may be the case if the channel object was provided for
    *         informatory purposes only, e.g. as part of an event notification
    *         for a channel listener.
    *
    */
    int CloseChannel()
    {
        /* if this object contains a valid channel to close */
        if (channelID)
        {
            /* if channel is still open */
            if (IsOpen())
            {
                /* if this object has 'closeable' rights to the channel */
                if (isCloseable)
                {
                    /* call standard 'c' API to close the channel */
                    if (STAR_closeChannel(this->channelID))
                    {
                        /* set channel status to closed */
                        pChannelReferenceCountandStatus->SetClosed();

                        /* return success */
                        return 1;
                    }
                    else
                    {
                        /* channel failed to close */
                        return 0;
                    }
                }
                else
                {
                    /* doesn't have rights to close */
                    return 0;
                }
            }
            else
            {
                /* channel already closed */
                return 1;
            }
        }
        else
        {
            /* no channel to close */
            return 0;
        }
    }

    /**
    * Submit a transfer operation using the channel.
    *
    * @param pTransferOp A pointer to a \link stardundee::com::starsystem::transferoperations::TransferOperation TransferOperation \endlink
    *                    to submit.
    *
    * @return 1 if the operation was successfully submitted, otherwise 0.
    */
    int SubmitTransferOperation(
        _In_ stardundee::com::starsystem::transferoperations::TransferOperation *pTransferOp)
    {
        /* if channel is no longer open */
        if (!IsOpen())
        {
            /* can't do anything with it */
            return 0;
        }

        /* if a valid transfer operation is provided */
        if (pTransferOp != NULL)
        {
            /* get the underlying STAR_TRANSFER_OPERATION struct associated with
             * the transfer operation */
            STAR_TRANSFER_OPERATION *pSTAR_TRANSFER_OPERATION =
                                     pTransferOp->GetSTAR_TRANSFER_OPERATION();

            /* if transfer operation is valid */
            if (pSTAR_TRANSFER_OPERATION != NULL)
            {
                /* call 'c' API to submit the transfer operation */
                return STAR_submitTransferOperation(this->channelID,
                                                      pSTAR_TRANSFER_OPERATION);
            }
            else
            {
                /* return error */
                return 0;
            }
        }
        else
        {
            /* no transfer operation */
            return 0;
        }
    }

    /**
    * Submit a list of transfer operations to this channel.
    *
    * @param transferOps Array of \link stardundee::com::starsystem::transferoperations::TransferOperation TransferOperation \endlink
    *                    objects to submit.
    * @param count The number of transfer operations in the array.
    *
    * @return 1 if all operations were successfully submitted, otherwise 0.
    */
    int SubmitTransferOperationList(_In_count_(count)
        stardundee::com::starsystem::transferoperations::TransferOperation *transferOps,
        unsigned int count)
    {
        /* if channel is no longer open */
        if (!IsOpen())
        {
            /* can't do anything with it */
            return 0;
        }

        /* if no transfer operations to process */
        if ((transferOps == NULL) || (count == 0))
        {
            /* return error code */
            return 0;
        }

        /* transform array of TransferOperation objects into a temporary array
         * of STAR_TRANSFER_OPERATION pointers.
         */

        /* try to create temporary array of STAR_TRANSFER_OPERATION pointers */
        STAR_TRANSFER_OPERATION** ppTransferOps = NULL;
        try
        {
            ppTransferOps = new STAR_TRANSFER_OPERATION*[count];
        }
        catch (std::bad_alloc& allocException)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(allocException);
            ppTransferOps = NULL;
        }

        /* if memory failed to create */
        if (ppTransferOps == NULL)
        {
            /* return error */
            return 0;
        }

        /* fill array with pointers to STAR_TRANSFER_OPERATION which are
         * contained within the TransferOperation objects provided. */
        for (unsigned int i=0; i<count; i++)
        {
            /* get reference to STAR_TRANSFER_OPERATION pointer held in
             * TransferOperation object and add a copy into the
             * STAR_TRANSFER_OPERATION pointer array.
             */
            ppTransferOps[i] = transferOps[i].GetSTAR_TRANSFER_OPERATION();
        }

        /* call 'c' API to submit the list of transmit operations */
        int result = STAR_submitTransferOperationList(this->GetChannelID(),
                                                      ppTransferOps, count);

        /* free the temporary array of STAR_TRANSFER_OPERATION pointers.
         * NOTE: the elements of the array are not freed, they contain pointers
         * which are simply references to the STAR_TRANSFER_OPERATION pointers
         * which are contained within the associated TransferOperation objects.
         * The memory which is occupied by these pointers will be freed by the
         * caller of this function, when the TransferOperation objects
         * themselves are destroyed.
         */
        delete [] ppTransferOps;

        /* return the result of submitting the transfer operations */
        return result;
    }

    /**
    * Submit a list of transfer operations to this channel.
    *
    * @param transferOps Array of pointers to \link stardundee::com::starsystem::transferoperations::TransferOperation TransferOperation \endlink
    *                    objects to submit.
    * @param count The number of transfer operations in the array.
    *
    * @return 1 if all operations were successfully submitted, otherwise 0.
    */
    int SubmitTransferOperationList(_In_count_(count)
        stardundee::com::starsystem::transferoperations::TransferOperation **transferOps,
        unsigned int count)
    {
        /* if channel is no longer open */
        if (!IsOpen())
        {
            /* can't do anything with it */
            return 0;
        }

        /* if no transfer operations to process */
        if ((transferOps == NULL) || (count == 0))
        {
            /* return error code */
            return 0;
        }

        /* transform array of TransferOperation objects into a temporary array
         * of STAR_TRANSFER_OPERATION pointers.
         */

        /* try to create temporary array of STAR_TRANSFER_OPERATION pointers */
        STAR_TRANSFER_OPERATION** ppTransferOps = NULL;
        try
        {
            ppTransferOps = new STAR_TRANSFER_OPERATION*[count];
        }
        catch (std::bad_alloc& allocException)
        {
            _STAR_SYSTEM_CPP_API_UNREFERENCED_PARAMETER_(allocException);
            ppTransferOps = NULL;
        }

        /* if memory failed to create */
        if (ppTransferOps == NULL)
        {
            /* return error */
            return 0;
        }

        /* fill array with pointers to STAR_TRANSFER_OPERATION which are
         * contained within the TransferOperation objects provided. */
        for (unsigned int i=0; i<count; i++)
        {
            /* get reference to STAR_TRANSFER_OPERATION pointer held in
             * TransferOperation object and add a copy into the
             * STAR_TRANSFER_OPERATION pointer array.
             */
            ppTransferOps[i] = transferOps[i]->GetSTAR_TRANSFER_OPERATION();
        }

        /* call 'c' API to submit the list of transmit operations */
        int result = STAR_submitTransferOperationList(this->channelID,
                                                      ppTransferOps, count);

        /* free the temporary array of STAR_TRANSFER_OPERATION pointers.
         * NOTE: the elements of the array are not freed, they contain pointers
         * which are simply references to the STAR_TRANSFER_OPERATION pointers
         * which are contained within the associated TransferOperation objects.
         * The memory which is occupied by these pointers will be freed by the
         * caller of this function, when the TransferOperation objects
         * themselves are destroyed.
         */
        delete [] ppTransferOps;

        /* return the result of submitting the transfer operations */
        return result;
    }

    /**
    * Receive a single packet from the channel into the buffer specified.
    *
    * \note This function is provided to simplify the process of receiving
    *       packets, but is not as efficient or flexible as using the standard
    *       process of receiving packets (typically by creating a
    *       \link stardundee::com::starsystem::transferoperations::ReceiveOperation ReceiveOperation \endlink
    *       and submitting the receive operation on a channel by using the
    *       SubmitTransferOperation() method). This function should not be used
    *       if high performance or low CPU utilisation is required. Note also
    *       that if the received packet is longer than the buffer provided, the
    *       end of the packet will be dropped and the end of packet marker will
    *       indicate that there was no end of packet marker.
    *
    * @param packetData A pointer to a previously allocated buffer which will be
                        updated to contain the received packet.
    * @param pPacketLength A pointer to a variable which which should contain
    *                      the length of the input buffer provided and which
    *                      will be updated to contain the length of the packet
    *                      received.
    * @param pEopType A pointer to a variable which will be updated to contain
    *                 the end of packet marker type of the packet.
    * @param timeout The maximum time in milliseconds to wait for the packet to
    *                be transmitted, or -1 to wait indefinitely.
    *
    * @return The transfer status of the operation.
    */
    stardundee::com::starsystem::transferoperations::TransferOperation::TransferStatus ReceivePacket(
        _Out_bytecap_(*pPacketLength) void *packetData,
        _Inout_ unsigned int *pPacketLength,
        _Out_ stardundee::com::starsystem::streamitems::Packet::EopType *pEopType,
        _In_ int timeout)
    {
        /* if channel is no longer open */
        if (!IsOpen())
        {
            /* can't do anything with it */
            return stardundee::com::starsystem::transferoperations::TransferOperation::TRANSFER_STATUS_ERROR;
        }

        /* temporary variable to hold the EOP type and transfer status*/
        STAR_EOP_TYPE eopType;
        STAR_TRANSFER_STATUS transferStatus;

        /* call 'c' API to receive the packet */
        transferStatus = STAR_receivePacket(this->channelID, packetData,
                                  pPacketLength, &eopType, timeout);

        /* copy EOP type */
        (*pEopType) = (stardundee::com::starsystem::streamitems::Packet::EopType)eopType;

        /* return the result */
        return (stardundee::com::starsystem::transferoperations::TransferOperation::TransferStatus)transferStatus;
    }

    /**
    * Transmit a single packet on the channel from the buffer specified.
    *
    * \note This function is provided to simplify the process of transmitting
    *       packets, but is not as efficient or flexible as using the standard
    *       process of submitting packets (typically by creating a
    *       \link stardundee::com::starsystem::transferoperations::TransmitOperation TransmitOperation \endlink
    *       and submitting the transmit operation on a channel by using the
    *       SubmitTransferOperation() method). This function should not be used
    *       if high performance or low CPU utilisation is required.
    *
    * @param packetData A pointer to a buffer which contains the packet data
    *                   to be transmitted.
    * @param packetLength The length of the buffer / packet.
    * @param eopType The end of packet marker type to be added to the end of the
    *                packet.
    * @param timeout The maximum time in milliseconds to wait for the packet to
    *                be transmitted, or -1 to wait indefinitely.
    *
    * @return The transfer status of the operation.
    */
    stardundee::com::starsystem::transferoperations::TransferOperation::TransferStatus TransmitPacket(
        _In_opt_bytecount_(packetLength) void* packetData,
        _In_ unsigned int packetLength,
        _In_ stardundee::com::starsystem::streamitems::Packet::EopType eopType,
        _In_ int timeout)
    {
        /* if channel is no longer open */
        if (!IsOpen())
        {
            /* can't do anything with it */
            return stardundee::com::starsystem::transferoperations::TransferOperation::TRANSFER_STATUS_ERROR;
        }

        /* call 'c' API to transmit the packet */
        return (stardundee::com::starsystem::transferoperations::TransferOperation::TransferStatus)STAR_transmitPacket(
            this->channelID, packetData, packetLength, (STAR_EOP_TYPE)eopType,
            timeout);
    }
};

/* end namespace tags */
} /* channels */ } /* starsystem */ } /* com */  } /* stardundee */

#endif

