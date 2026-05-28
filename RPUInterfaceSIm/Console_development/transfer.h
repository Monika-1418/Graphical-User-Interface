/**
 * \file transfer.h
 *
 * \brief Declarations of functions provided by STAR-API relating to transfer
 *        operations.
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

#ifndef STAR_API_TRANSFER_OPERATIONS_H
#define STAR_API_TRANSFER_OPERATIONS_H

#include "types.h"
#include "transfer_types.h"
#include "stream_item_types.h"

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \ingroup transfers
 * Receive a single packet on a previously opened channel in to the buffer
 * specified.  This function is provided to simplify the process of receiving
 * packets, but is not as efficient or as flexible as using
 * STAR_createRxOperation(), STAR_submitTransferOperation(), etc.  This function
 * should not be used if high performance or low CPU utilisation is required.
 * Note that if the received packet is longer than the buffer provided, the end
 * of the packet will be dropped and the end of packet marker will indicate
 * there was no end of packet marker.  Note also that a limitation of this
 * function is that the operation may timeout, but a packet is consumed and will
 * not be received in a subsequent call to the function.  To work around these
 * issues use the alternative functions mentioned above.
 *
 * @param channelId     The identifier of a previously opened channel.  The
 *                      channel must have been opened as an in or in/out channel
 * @param pPacketData   Pointer to a previously allocated buffer which will be
 *                      updated to contain the received packet
 * @param pPacketLength Pointer to a variable which should contain the length
 *                      of the buffer, and which will be updated to contain the
 *                      length of the packet
 * @param pEopType      Pointer to a variable which will be updated to contain
 *                      the end of packet marker type of the packet
 * @param timeout       The maximum time in milliseconds to wait for the packet
 *                      to be received, or -1 to wait indefinitely
 *
 * @return the status of the operation
 */
STAR_TRANSFER_STATUS STAR_API_CC STAR_receivePacket(
    _In_ STAR_CHANNEL_ID channelId,
    _Out_bytecap_(*pPacketLength) void *pPacketData,
    _Inout_ unsigned int *pPacketLength, _Out_ STAR_EOP_TYPE *pEopType,
    _In_ int timeout);


/**
 * \ingroup transfers
 * Transmit a single packet on a previously opened channel from the buffer
 * specified.  This function is provided to simplify the process of transmitting
 * packets, but is not as efficient or as flexible as using
 * STAR_createTxOperation(), STAR_submitTransferOperation(), etc. This function
 * should not be used if high performance or low CPU utilisation is required.
 *
 *
 * @param channelId    The identifier of a previously opened channel.  The
 *                     channel must have been opened as an out or in/out channel
 * @param pPacketData  A pointer to a previously allocated buffer which should
 *                     contain the packet to be transmitted
 * @param packetLength The length of the buffer, and therefore the length of the
 *                     packet
 * @param eopType      The end of packet marker type to be added to the end of
 *                     the packet
 * @param timeout      The maximum time in milliseconds to wait for the packet
 *                     to be transmitted, or -1 to wait indefinitely
 *
 * @return the status of the operation
 */
STAR_TRANSFER_STATUS STAR_API_CC STAR_transmitPacket(
    _In_ STAR_CHANNEL_ID channelId,
    _In_opt_bytecount_(packetLength) void *pPacketData,
    _In_ unsigned int packetLength, _In_ STAR_EOP_TYPE eopType,
    _In_ int timeout);


/**
 * \ingroup transfers
 * Creates a transmit operation that can be submitted later.  The transmit
 * operation returned should be disposed of by calling
 * STAR_disposeTransferOperation() when it is no longer required.
 *
 * @param ppItems               Array of pointers to Stream Items to be sent
 *                              \note It is safe to free this array after the
 *                                    function has completed.
 * @param streamItemCount       Count of the items in the ppItems array
 *
 * @return            Pointer to a STAR_TRANSFER_OPERATION
 *
 */
_Check_return_ STAR_TRANSFER_OPERATION* STAR_API_CC STAR_createTxOperation(
    _In_count_(streamItemCount) STAR_STREAM_ITEM **ppItems,
    unsigned int streamItemCount);

/**
 * \ingroup transfers
 * Creates a receive operation that can then be submitted.  The receive
 * operation returned should be disposed of by calling
 * STAR_disposeTransferOperation() when it is no longer required.
 *
 * @param    itemCount      The maximum number of stream items to receive (the
 *                          size of an individual stream item is not limited).
 *                          This can be -1 to receive an unlimited number of
 *                          items
 *                          \note When a receive operation receiving an
 *                          unlimited (or very large) number of items, the
 *                          received stream items must be destroyed to ensure
 *                          the received stream items do not consume all the
 *                          PC's memory.
 * @param   mask            Bitmask with flags set for the type of traffic one
 *                          wishes to receive
 *                          \note It is not possible to receive whole packets at
 *                          the same time as link control tokens/data chunks
 *
 * @return        a pointer to a #STAR_TRANSFER_OPERATION object
 *
 */
_Check_return_ STAR_TRANSFER_OPERATION* STAR_API_CC STAR_createRxOperation(
    int itemCount, STAR_RECEIVE_MASK mask);

/**
* \ingroup transfers
*
 * Submits a single transfer operation.  Once a transfer operation has
 * completed, it can be resubmitted.  This can be useful for receiving a number
 * of packets in a loop, or transmitting the same packet repeatedly.  Note that
 * if an operation is resubmitted before it has completed, the original
 * operation will be cancelled and then the new one submitted.
 *
 * @param     channelId        Channel to submit operation on
 * @param[in] transferOp       #STAR_TRANSFER_OPERATION to submit
 *
 * @return      1 if operation was successfully submitted, else 0
 *
 */
int STAR_API_CC STAR_submitTransferOperation(
    _In_ STAR_CHANNEL_ID channelId,
    _In_ STAR_TRANSFER_OPERATION *transferOp);
/**
 * \ingroup transfers
 *
 * Submits a list of transfer operations.  Once a transfer operation has
 * completed, it can be resubmitted.  This can be useful for receiving a number
 * of packets in a loop, or transmitting the same packet repeatedly.  Note that
 * if an operation is resubmitted before it has completed, the original
 * operation will be cancelled and then the new one submitted.  This means it is
 * not useful to submit a list containing multiple instances of the same
 * operation.
 *
 * @param        channelId      Channel to submit operations on
 * @param[in]     transferOps   Array of pointers to #STAR_TRANSFER_OPERATION to be submitted
 * @param        count          Count of operations in array
 *
 * @return      1 if all operations were successfully submitted, else 0
 *
 */
int STAR_API_CC STAR_submitTransferOperationList(
    STAR_CHANNEL_ID channelId,
    _In_count_(count) STAR_TRANSFER_OPERATION **transferOps,
    unsigned int count);

/**
 * \ingroup transfers
 *
 * Blocks until a given transfer operation has completed, or the timeout period
 * expires.  Note that the operation will continue to transmit or receive once
 * this function returns if it has not yet completed.  The operation will only
 * be stopped if it completes, there is an error, or it is cancelled by calling
 * STAR_cancelTransferOperation().  If the operation passed to this function has
 * been submitted but has not yet completed when the function returns, the
 * return value will be #STAR_TRANSFER_STATUS_STARTED as the operation has
 * started but not yet completed.  This function can be called multiple times
 * for the same operation to wait until that operation completes.
 *
 * @param[in] pTransferOperation    Operation to wait on
 * @param     timeout               Max time in milliseconds to wait, or -1 to
 *                                  wait indefinitely
 *
 * @return        result status of the operation
 */
STAR_TRANSFER_STATUS STAR_API_CC STAR_waitOnTransferOperationCompletion(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation, int timeout);

/**
* \ingroup transfers
 *
 * Gets the status of a transfer operation.
 *
 * @param pTransferOperation    Operation to get status of
 *
 * @return        result status of the operation
 */
STAR_TRANSFER_STATUS STAR_API_CC STAR_getTransferOperationStatus(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation);

/**
 * \ingroup transfers
 *
 * Cancels any waits in progress on a transfer operation.  Note that this does
 * not cancel the transmit operation it simply cancels any calls to
 * STAR_waitOnTransferOperationCompletion().
 *
 * @param pTransferOperation    Operation to cancel waits on
 *
 */
void STAR_API_CC STAR_cancelTransferOperationWaits(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation);

/**
 * \ingroup transfers
 *
 * Gets the current count of stream items received by the operation and
 * available to be obtained using STAR_getTransferItem().  Note that this may be
 * less than the number of stream items received if some of those stream items
 * have been destroyed by calling STAR_destroyStreamItem() or
 * STAR_destroyTransferItemList() with the destroyItems parameter set to a
 * non-zero value.
 *
 * @param pReceiveOperation        Operation to get received item count for
 *
 * @return      Count of received stream items
 */
unsigned int STAR_API_CC STAR_getTransferItemCount(
    _In_ STAR_TRANSFER_OPERATION *pReceiveOperation);

/**
 * \ingroup transfers
 *
 * Gets the stream item at a given index of a receive operation.
 *
 * @param pReceiveOperation     Operation to get item from
 * @param index                 Item within operation to access
 *
 * @return      Item requested
 */
STAR_STREAM_ITEM* STAR_API_CC STAR_getTransferItem(
    _In_ STAR_TRANSFER_OPERATION *pReceiveOperation, unsigned int index);

/**
 * \ingroup transfers
 *
 * Gets the entire list of transfer items associated with a receive.  The array
 * returned must be destroyed by calling STAR_destroyTransferItemList().
 *
 * @param[in]  pReceiveOperation    Operation to get items from
 * @param[out] count                Count of items in returned array
 *
 * @return      Array of pointers stream item associated with this receive
 */
_Ret_opt_cap_(*count) STAR_STREAM_ITEM** STAR_API_CC STAR_getTransferItemList(
    _In_ STAR_TRANSFER_OPERATION *pReceiveOperation, _Out_ unsigned int* count);


/**
 * \ingroup transfers
 *
 * Cancels a given transfer operation.
 * \note It is not currently possible to cancel a transmit operation.
 *
 * @param pTransferOperation    Operation to cancel
 *
 * @return      1 if a receive was cancelled, else 0
 *
 */
int STAR_API_CC STAR_cancelTransferOperation(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation);


/**
 * \ingroup transfers
 *
 * Cancels a transfer operation (if it is not already complete) and indicates
 * that the memory associated with the operation should be freed when the
 * operation is no longer in use.  Note that this is a dispose function rather
 * than a destroy function, as it only indicates to the API that the transfer
 * operation should be destroyed when it is possible to do so.  If the API is in
 * the process of transmitting or receiving traffic on the operation it may not
 * be possible to destroy the operation immediately.
 *
 * @param pTransferOperation    a pointer to the operation to be disposed of
 *
 * @return      1 if operation was disposed of successfully, otherwise 0
 *
 */
int STAR_API_CC STAR_disposeTransferOperation(
    _In_ _Post_ptr_invalid_ STAR_TRANSFER_OPERATION *pTransferOperation);



/**
 * \ingroup transfers
 *
 * Registers a call-back function that will be called when an operation completes.
 *
 * \note Each callback is made using a separate thread, to enaure that STAR-System
 * internal processing is not blocked by long-running callbacks.  Due to operating
 * system scheduling it is not possible to guarantee that callbacks will execute in
 * the same order as they are triggered.
 *
 * @param pTransferOperation    Operation to add listener for
 * @param listenerFunc          Call-back function
 * @param pContextInfo          A pointer to memory which will be provided when
 *                              the call-back function is called
 *
 * @return    Non-Zero if call-back was registered successfully
 */
int STAR_API_CC STAR_registerTransferCompletionListener(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation,
    STAR_TransferOperationListenerFunc listenerFunc,
    _In_opt_ void* pContextInfo);

/**
 * \ingroup transfers
 *
 * Unregisters a call-back function that will be called when an operation completes.
 *
 *
 * @param pTransferOperation    Operation to remove listener for
 * @param listenerFunc          Call-back function
 *
 * @return    Non-Zero if call-back was unregistered successfully
 */
int STAR_API_CC STAR_unregisterTransferCompletionListener(
    _In_ STAR_TRANSFER_OPERATION *pTransferOperation,
    STAR_TransferOperationListenerFunc listenerFunc);





#ifdef __cplusplus
}
#endif



#endif    /* STAR_API_TRANSFER_OPERATIONS_H */
