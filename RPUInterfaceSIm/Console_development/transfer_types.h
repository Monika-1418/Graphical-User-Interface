/**
 * \file transfer_types.h
 *
 * \brief Types used to describe transmit and receive operations
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
 */

#ifndef STAR_API_TXRX_TYPES_H
#define STAR_API_TXRX_TYPES_H

#include "types.h"

#include "stream_item_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \ingroup transfers
 *
 * Possible states a transfer operation can be in.
 */
typedef enum star_api_transfer_status
{
    /**
     * Not yet started.  When a transfer operation is created, this will be its
     * status.
     */
    STAR_TRANSFER_STATUS_NOT_STARTED,

    /**
     * Transfer has begun.  When a transfer operation is submitted, this will be
     * its status, until it has completed. */
    STAR_TRANSFER_STATUS_STARTED,

    /**
     * Transfer has completed.  Once a transmit operation has successfully
     * transmitted all its traffic, or a receive operation has received all its
     * requested traffic, this will be its status.
     */
    STAR_TRANSFER_STATUS_COMPLETE,

    /**
     * Transfer was cancelled.  When a transfer is cancelled by calling
     * STAR_cancelTransferOperation(), this will be its status.
     */
    STAR_TRANSFER_STATUS_CANCELLED,

    /**
     * An error occurred while processing the transfer.  This will be the status
     * of a transfer operation if there was an error creating it, submitting it,
     * or there was an error while transmitting or receiving.
     */
    STAR_TRANSFER_STATUS_ERROR

} STAR_TRANSFER_STATUS;

/**
 * \ingroup transfers
 *
 * Flags used to determine what kind of stream items
 * to receive on a receive operation.
 *
 */
typedef enum
{
    /** #STAR_SPACEWIRE_PACKET  */
    STAR_RECEIVE_PACKETS            = 1U << 0,
    /** #STAR_DATA_CHUNK */
    STAR_RECEIVE_CHUNKS             = 1U << 1,
    /** #STAR_TIMECODE */
    STAR_RECEIVE_TIMECODES          = 1U << 2,
    /** Flow control characters. Note that transmitting and receiving FCTs is not currently supported. */
    STAR_RECEIVE_FCTS               = 1U << 3,
    /** Null characters. Note that transmitting and receiving Nulls is not currently supported. */
    STAR_RECEIVE_NULL               = 1U << 4,
    /** #STAR_LINK_STATE_EVENT */
    STAR_RECEIVE_LINK_STATE_EVENTS  = 1U << 5,
    /** #STAR_LINK_SPEED_EVENT */
    STAR_RECEIVE_LINK_SPEED_EVENTS  = 1U << 6,
    /** #STAR_TIMESTAMP_EVENT */
    STAR_RECEIVE_TIMESTAMP_EVENTS   = 1U << 7
} STAR_RECEIVE_MASK;


/**
 * A structure used to identify a transfer operation.
 *
 * \note It is not intended for users of this library to access the members of
 *      this structure directly. Instead, use the provided accessor functions.
 */
typedef struct star_api_transfer_operation
{
    /**  \internal The connection this operation is being performed on */
    void* stepConn;

    /**  \internal The current status of this operation*/
    volatile STAR_TRANSFER_STATUS status;

    /**  \internal Whether or not the operation is a send operation. */
    int sendOperation;

    /**  \internal Whether this is an unlimited receive operation (allow realloc of rx buf)*/
    int infiniteRx;

    /** \internal  Flags showing what sort of data to receive*/
    STAR_RECEIVE_MASK rxMask;

    /** \internal  Array of buffers holding STEP formatted traffic to be sent */
    void* pTrafficItems;

    /** \internal  Count of traffic Items in array */
    unsigned int trafficCount;

    /** \internal  List of pointers to received stream items */
    STAR_STREAM_ITEM *pStreamItems;

    /**
     * \internal
     * The next element in the list of stream item pointers to be populated, or
     * NULL if there's not one available.
     */
    STAR_STREAM_ITEM *pNextStreamItem;

    /**
    * \internal
    * The last element in the list of stream item pointers.
    */
    STAR_STREAM_ITEM *pLastStreamItem;

    /** \internal
     * Pointer to a critical section used to control access to the array of
     * received stream items.  Not used if the operation is a transmit
     * operation.
     */
    void *pStreamItemsCS;

    /**
    * \internal
    * Count of the total number of stream items which have been allocated.
    */
    unsigned int itemsAllocated;

    /**
     * \internal
     * Count of the number of stream items to be received, or -1 if this is an
     * infinite receive.
     */
    int itemCount;

    /**
     * \internal
     * Count of the total number of stream items which have been received.  Note
     * that some of these may have since been disposed, and so will not be
     * present in the list.
     */
    unsigned int itemsReceived;

    /** \internal Pointer to the packet that is currently being assembled
                  from incoming data */
    STAR_SPACEWIRE_PACKET* packetUnderConstruction;

    /** \internal  Number of received items in the stream items list. */
    unsigned int currIndex;

    /**  \internal
        Marks whether this operation is currently copying data from STAR-Core
        If dispose is called and this flag is set, dispose is postponed until
        the data currently being received from STAR-Core is complete. */
    void* copyingFromStarCore;

    /** \internal  Fires when the operation completes */
    void* operationComplete;

    /**@}*/
}STAR_TRANSFER_OPERATION;


/**
 * \ingroup transfers
 * The function type for transfer operation listener functions which are called
 * when a transfer operation completes, is cancelled, or encounters an error.
 *
 * @param pOperation the operation which has completed
 * @param status the status of the operation
 * @param pContextInfo a pointer to user-specific data that is provided when the
 *                     completion listener is registered
 *
 * \note The calling convention for this function type is #STAR_API_CC
 */
typedef void (STAR_API_CC *STAR_TransferOperationListenerFunc)(STAR_TRANSFER_OPERATION *pOperation,
                            STAR_TRANSFER_STATUS status,
                            void *pContextInfo);


#ifdef __cplusplus
}
#endif

#endif  /* STAR_API_TXRX_TYPES_H */
