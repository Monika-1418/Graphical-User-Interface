/**
 * \file write_reply_packet_example.c
 *
 * \brief Example calls to build and fill write reply packets.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example calls to the RMAP Packet Library function to build
 * and fill RMAP write reply packets.  It also contains calls to determine the
 * length of these packets.
 *
 * Copyright &copy; 2009 STAR-Dundee Ltd
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rmap_packet_library.h"

void write_reply_packet_example()
{
	void *pFillPacket, *pBuildPacket;
	unsigned long fillPacketLenCalculated, fillPacketLen, buildPacketLen;
	U8 pReply[] = {254};
	U8 target = 254;
	char status;

	/* Calculate the length of a write reply packet */
	fillPacketLenCalculated = RMAP_CalculateWriteReplyPacketLength(1, 1);
	printf("The write reply packet will have a length of %ld bytes\n",
		fillPacketLenCalculated);

	/* Allocate memory for the packet */
	pFillPacket = malloc(fillPacketLenCalculated);
	if (!pFillPacket)
	{
		puts("Couldn't allocate the memory for the reply packet");
		return;
	}

	/* Fill the memory with the packet */
	status = RMAP_FillWriteReplyPacket(pReply, 1, target, 1, 0, RMAP_SUCCESS, 0,
		&fillPacketLen, NULL, 1, (U8 *)pFillPacket, fillPacketLenCalculated);
	if (!status)
	{
		puts("Couldn't fill the write reply packet");
		free(pFillPacket);
		return;
	}

	/* Build an RMAP write reply packet */
	pBuildPacket = RMAP_BuildWriteReplyPacket(pReply, 1, target, 1, 0,
		RMAP_SUCCESS, 0, &buildPacketLen, NULL, 1);
	if (!pBuildPacket)
	{
		puts("Couldn't build the write reply packet");
		free(pFillPacket);
		return;
	}

	/* Check the lengths are all the same */
	if ((fillPacketLenCalculated != fillPacketLen) ||
		(fillPacketLen != buildPacketLen))
	{
		puts("The lengths are different:");
		printf("\tLength calculated for packet: %ld\n",
			fillPacketLenCalculated);
		printf("\tLength of filled packet: %ld\n", fillPacketLen);
		printf("\tLength of built packet: %ld\n", buildPacketLen);
	}
	else
	{
		puts("The packet lengths are all the same.");

		/* Check the packets are identical, despite being built differently */
		if (memcmp(pFillPacket, pBuildPacket, fillPacketLen) != 0)
		{
			puts("The filled packet is different to the built packet.");
		}
		else
		{
			puts("The packets are identical.");
		}
	}

	/* Free the memory allocated for each of the packets.  Note that the */
	/* memory allocated by the library should be freed by calling */
	/* RMAP_FreeBuffer. */
	RMAP_FreeBuffer(pBuildPacket);
	free(pFillPacket);
}


