/**
 * \file examples.h
 *
 * \brief Example functions for STAR-API.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains declarations of the example programs for STAR-API.
 *
 * Copyright &copy; 2016 STAR-Dundee Ltd.
 */

/**
 * Sends a packet out of a link, to a Brick or other routing device which is
 * then routed back to where it started using the address features of STAR-API.
 */
void advancedAddressExample();

/**
 * Loops continuously until 10 packets have been received. After each packet is
 * received, the contents are printed.
 */
void advancedContinuousReceiveExample();

/**
 * Sends multiple packets using the advanced transmit operations.
 */
void advancedMultiplePacketExample();

/**
 * Receives a packet using the receive operations, prompting the user for input.
 */
void advancedReceiveExample();

/**
 * Sends a simple packet using an out channel, then receives the packet using an
 * in channel. Uses the advanced transmit and receive operations.
 */
void advancedSendAndReceiveExample();

/**
 * Sends a packet using the transmit operations, prompting the user for input.
 */
void advancedSendExample();

/**
 * Sends and receives simple packets in both directions using an inout channel.
 * Uses the advanced transmit and receive operations.
 */
void advancedTwoWayExample();

/**
 * Retreives the version of all modules and prints it.
 */
void allVersionExample();

/**
 * Retreives the version of the API and prints it.
 */
void apiVersionExample();

/**
 * Demonstrates setting the application name.
 */
void applicationNameExample();

/*
 * Demonstrates the channel callback functions.
 */
void channelCallbackExample();

/**
 * Lists all drivers that are available and prints details about them.
 */
void driverListExample();

/**
 * Retreives the version of the firmware and prints it.
 */
void firmwareVersionExample();

/**
 * Receives a packet using the STAR_receivePacket function, prompting the user
 * for input.
 */
void simpleReceiveExample();

/**
 * Sends a simple packet using an out channel, then receives the packet using an
 * in channel. Uses the basic transmit and receive operations.
 */
void simpleSendAndReceiveExample();

/**
 * Sends a packet using the STAR_transmitPacket function, prompting the user
 * for input.
 */
void simpleSendExample();

/**
 * Sends and receives simple packets in both directions using an inout channel.
 * Uses the simple transmit and receive operations.
 */
void simpleTwoWayExample();

/**
 * Example to demonstrate sending and receiving timecodes.
 */
void timecodeExample();

/**
 * Demonstrates the transfer callback functions.
 */
void transferCallbackExample();

/**
 * Creates a send and receive transfer operation and submits them both together
 * as an array. The packet is routed back over the same channel.
 */
void transferOperationListSendReceiveExample();

/**
 * Creates an array of transfer operations and submits them all together as a
 * transfer operation list.
 */
void transferOperationListSendExample();

/**
 * Sends two packets using the transmit operations, prompting the user for
 * input.  The first packet will contain a disconnect error, while the second
 * will contain a parity error.
 * \note This example should only be run on devices which support transmission
 *       of errors.
 */
void transmitErrorsExample();

