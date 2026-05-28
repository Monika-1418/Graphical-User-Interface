/**
 * \file crc_example.c
 *
 * \brief Example calls to the CRC functions in the RMAP Packet Library.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains example calls to the CRC functions of the RMAP Packet
 * Library functions.
 *
 * Copyright &copy; 2009 STAR-Dundee Ltd
 */

#include <stdio.h>

#include "rmap_packet_library.h"

#define BUFFER_LEN	32
#define HEADER_LEN	10

void crc_example()
{
	U8 pBuffer[BUFFER_LEN], crc;
	int i;
	
	/* Fill the data buffer */
	for (i = 0; i < BUFFER_LEN; i++)
	{
		pBuffer[i] = i;
	}
	
	/* Calculate a CRC for the header part of the buffer */
	crc = RMAP_CalculateCRC(pBuffer, HEADER_LEN);
	
	printf("The CRC of the first %d bytes of the buffer is 0x%x\n", HEADER_LEN,
		crc);
	
	/* Calculate a CRC for the entire buffer, */
	/* using the CRC already calculated as a seed */
	crc = RMAP_CalculateCRCWithSeed(pBuffer + HEADER_LEN,
		BUFFER_LEN - HEADER_LEN, crc);
	
	printf("The CRC of the full buffer is 0x%x\n", crc);

	/* Check the CRC is correct for the buffer */
	printf("The CRC of the buffer is %svalid\n",
		RMAP_IsCRCValid(pBuffer, BUFFER_LEN, crc) ? "" : "not ");
}

