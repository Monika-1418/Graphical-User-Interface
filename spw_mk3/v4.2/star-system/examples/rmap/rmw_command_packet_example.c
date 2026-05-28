/**
 * \file rmw_command_packet_example.c
 *
 * \brief Example calls to build and fill read/modify/write command packets.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example calls to the RMAP Packet Library function to build
 * and fill RMAP read/modify/write command packets.  It also contains calls to
 * determine the length of these packets.
 *
 * Copyright &copy; 2009 STAR-Dundee Ltd
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rmap_packet_library.h"

void rmw_command_packet_example()
{
    void *pFillPacket, *pBuildPacket, *pBuildPacketRegister;
    unsigned long fillPacketLenCalculated, fillPacketLen;
    unsigned long buildPacketLen, buildPacketRegisterLen;
    U8 pTarget[] = {0, 254};
    U8 pReply[] = {254};
    U8 pData[] = {0x12, 0x34, 0x56, 0x78};
    U8 pMask[] = {0x0f, 0x0f, 0x0f, 0x0f};
    char status;

    /* Calculate the length of a read/modify/write command packet, */
    /* with 4 bytes of data and mask */
    fillPacketLenCalculated = RMAP_CalculateReadModifyWriteCommandPacketLength(
        2, 1, 8, 1);
    printf("The read/modify/write command packet will have a length of %ld bytes\n",
        fillPacketLenCalculated);

    /* Allocate memory for the packet */
    pFillPacket = malloc(fillPacketLenCalculated);
    if (!pFillPacket)
    {
        puts("Couldn't allocate the memory for the command packet");
        return;
    }

    /* Fill the memory with the packet */
    status = RMAP_FillReadModifyWriteCommandPacket(pTarget, 2, pReply, 1, 0x20,
        0, 0x106, 0, 8, pData, pMask, &fillPacketLen, NULL, 1,
        (U8 *)pFillPacket, fillPacketLenCalculated);
    if (!status)
    {
        puts("Couldn't fill the read/modify/write command packet");
        free(pFillPacket);
        return;
    }

    /* Build an RMAP read/modify/write command packet to write 4-bytes */
    pBuildPacket = RMAP_BuildReadModifyWriteCommandPacket(pTarget, 2, pReply, 1,
        0x20, 0, 0x106, 0, 8, pData, pMask, &buildPacketLen, NULL, 1);
    if (!pBuildPacket)
    {
        puts("Couldn't build the read/modify/write command packet");
        free(pFillPacket);
        return;
    }

    /* Build an RMAP read/modify/write command packet */
    /* to write to a 4-byte register */
    pBuildPacketRegister = RMAP_BuildReadModifyWriteRegisterPacket(pTarget, 2,
        pReply, 1, 0x20, 0, 0x106, 0, 0x12345678, 0x0f0f0f0f,
        &buildPacketRegisterLen, NULL, 1);
    if (!pBuildPacketRegister)
    {
        puts("Couldn't build the read/modify/write register packet");
        RMAP_FreeBuffer(pBuildPacket);
        free(pFillPacket);
        return;
    }

    /* Check the lengths are all the same */
    if ((fillPacketLenCalculated != fillPacketLen) ||
        (fillPacketLen != buildPacketLen) ||
        (buildPacketLen != buildPacketRegisterLen))
    {
        puts("The lengths are different:");
        printf("\tLength calculated for packet: %ld\n",
            fillPacketLenCalculated);
        printf("\tLength of filled packet: %ld\n", fillPacketLen);
        printf("\tLength of built packet: %ld\n", buildPacketLen);
        printf("\tLength of built register packet: %ld\n", buildPacketLen);
    }
    else
    {
        puts("The packet lengths are all the same.");

        /* Check the packets are identical, despite being built differently */
        if (memcmp(pFillPacket, pBuildPacket, fillPacketLen) != 0)
        {
            puts("The filled packet is different to the built packet.");
        }
        else if (memcmp(pFillPacket, pBuildPacketRegister, fillPacketLen) != 0)
        {
            puts("The built register packet is different to the built packet and the filled packet.");
        }
        else
        {
            puts("The packets are identical.");
        }
    }

    /* Free the memory allocated for each of the packets.  Note that the */
    /* memory allocated by the library should be freed by calling */
    /* RMAP_FreeBuffer. */
    RMAP_FreeBuffer(pBuildPacketRegister);
    RMAP_FreeBuffer(pBuildPacket);
    free(pFillPacket);
}


