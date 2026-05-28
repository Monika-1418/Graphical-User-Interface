/**
 * \file rmap_examples.h
 *
 * \brief Provides definitions of classes which provide examples of using
 *        objects and functions in the RMAP packet library.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Provides definitions of classes which provide examples of using objects and
 * functions in the RMAP packet library.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "star_system.hpp"
#include "rmap_packet_library.hpp"
#include "rmap_read_command_packet.hpp"
#include "rmap_read_reply_packet.hpp"
#include "rmap_read_modify_write_command_packet.hpp"
#include "rmap_read_modify_write_reply_packet.hpp"
#include "rmap_write_command_packet.hpp"
#include "rmap_write_reply_packet.hpp"
#include <stdio.h>

using namespace stardundee::com::starsystem::general;
using namespace stardundee::com::starsystem::rmap;

#ifndef RMAP_EXAMPLES_H
#define RMAP_EXAMPLES_H

#define BUFFER_LEN          32
#define HEADER_LEN          10

/**
 * Provides examples of using objects and functions in the RMAP packet library.
 */
class RMAPExamples
{
public:
    /**
     * Provides an example of using the CRC functions in the RMAP
     * packet library.
     */
    void PerformCrcExample();

    /**
     * Display the type of a packet based upon the given type value.
     *
     * @param type The packet's type
     */
    void display_packet_type(RMAPPacket::PacketType type);

    /**
     * Display the status of a packet from the given status value.
     *
     * @param status The packet's status
     *
     */
    void display_packet_status(RMAPPacket::RMAPStatus status);

    /**
     * Check the fields of the given packet to ensure that they are valid and
     * as expected, reporting their values.
     *
     * @param pPacketToCheck A pointer to an RMAP packet object, representing 
     *                       the packet to be checked.
     */
    void CheckPacket(RMAPPacket *pPacketToCheck);

    /**
     * Provides an example of how to check that the format of a received RMAP
     * packet is correct, and then to access the fields in the packet.
     *
     * The example creates an RMAP write command packet, to write to a 4-byte
     * register (a 'write register' packet).
     */
    void PerformCheckPacketExample();

    /**
     * Perform example calls to build read command packets and to
     * determine the length of these packets.
     */
    void PerformReadCommandPacketExample();

    /**
     * Perform example calls to build read reply packets and to
     * determine the length of these packets.
     */
    void PerformReadReplyPacketExample();

    /**
     * Perform example calls to build read/modify/write command packets and to
     * determine the length of these packets.
     */
    void PerformReadModifyWriteCommandPacketExample();

    /**
     * Perform example calls to build read/modify/write reply packets and to
     * determine the length of these packets.
     */
    void PerformReadModifyWriteReplyPacketExample();

    /**
     * Perform example calls to build write command packets and to
     * determine the length of these packets.
     */
    void PerformWriteCommandPacketExample();

    /**
     * Perform example calls to build write reply packets and to
     * determine the length of these packets.
     */
    void PerformWriteReplyPacketExample();

    /**
     * Perform example call to obtain the version information of the RMAP Packet
     * Library and to access the fields in the version information.
     */
    void PerformVersionExample();

    /**
     * Provides an example of using an RMAP write command to write a
     * value to the general purpose / user register on a device. The example
     * includes creating a packet stream item to contain the RMAP packet and 
     * then transmitting it on an opened channel on a device.
     */
    void WriteToUserRegister(Device **ppDevice);

    /**
     * Provides an example of reading from the general purpose / user register 
     * on a Brick device
     */
    void ReadFromUserRegister(Device *pDevice);

    /**
     * Compare the contents of two RMAPPackets to determine if they are
     * identical or not.
     *
     * @param pPacket1 The first packet to compare.
     * @param pPacket2 The second packet to compare.
     *
     * @return 1 if the packets are identical, otherwise 0.
     *
     */
    char PacketsAreIdentical(RMAPPacket *pPacket1, RMAPPacket *pPacket2);
};

#endif /* RMAP_EXAMPLES_H */

