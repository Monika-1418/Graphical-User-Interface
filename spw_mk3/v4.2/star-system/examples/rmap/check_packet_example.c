/**
 * \file check_packet_example.c
 *
 * \brief Example call to check the format of packets in the RMAP Packet
 *        Library.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains an example call to the RMAP Packet Library function to
 * check the format of an RMAP packet.  It also contains calls to the functions
 * to access the fields in the RMAP packet.
 *
 * Copyright &copy; 2009 STAR-Dundee Ltd
 */

#include <stdio.h>

#include "rmap_packet_library.h"

/* Display the type of a packet. */
void display_packet_type(RMAP_PACKET_TYPE type)
{
	switch (type)
	{
		case RMAP_WRITE_COMMAND:
			puts("The packet is an RMAP write command.");
			break;
		
		case RMAP_WRITE_REPLY:
			puts("The packet is an RMAP write reply.");
			break;
		
		case RMAP_READ_COMMAND:
			puts("The packet is an RMAP read command.");
			break;
		
		case RMAP_READ_REPLY:
			puts("The packet is an RMAP read reply.");
			break;
		
		case RMAP_READ_MODIFY_WRITE_COMMAND:
			puts("The packet is an RMAP read/modify/write command.");
			break;
		
		case RMAP_READ_MODIFY_WRITE_REPLY:
			puts("The packet is an RMAP read/modify/write reply.");
			break;
		
		case RMAP_INVALID_PACKET_TYPE:
			puts("The packet is of an invalid RMAP packet type.");
			break;
			
		default:
			puts("The packet is of an unexpected type.");
	}
}

/* Display the status of a packet. */
void display_packet_status(RMAP_STATUS status)
{
	switch (status)
	{
		case RMAP_SUCCESS:
			puts("The status indicates the command completed successfully.");
			break;

		case RMAP_GENERAL_ERROR:
			puts("The status indicates a general error that does not fit into the other error cases.");
			break;

		case RMAP_UNUSED_PACKET_TYPE_OR_COMMAND_CODE:
			puts("The status indicates the packet type or command is an unexpected value.");
			break;

		case RMAP_INVALID_KEY:
			puts("The status indicates the key did not match that expected by the target user application.");
			break;

		case RMAP_INVALID_DATA_CRC:
			puts("The status indicates an invalid data CRC.");
			break;

		case RMAP_EARLY_EOP:
			puts("The status indicates an EOP was detected before the end of the data.");
			break;

		case RMAP_TOO_MUCH_DATA:
			puts("The status indicates there was more data than was expected.");
			break;

		case RMAP_EEP:
			puts("The status indicates an EEP was  encountered in the packet after the header.");
			break;

		case RMAP_VERIFY_BUFFER_OVERRUN:
			puts("The status indicates verify before write was enabled in the command but not enough buffer space was available to receive the full command.");
			break;

		case RMAP_COMMAND_NOT_IMPLEMENTED_OR_AUTHORISED:
			puts("The status indicates the target user application did not authorise the requested operation.");
			break;

		case RMAP_RMW_DATA_LENGTH_ERROR:
			puts("The status indicates the amount of data in a read/modify/write command is invalid.");
			break;

		case RMAP_INVALID_TARGET_LOGICAL_ADDRESS:
			puts("The status indicates the target logical address was not the value expected by the target.");
			break;

		default:
			puts("The status is an unexpected value.");
	}
}

void check_packet_example()
{
	void *pPacket;
	unsigned long packetLen, addressLen, i;
	U8 pTarget[] = {0, 254};
	U8 pReply[] = {254};
	RMAP_PACKET packetStruct;
	RMAP_STATUS status;
	RMAP_PACKET_TYPE type;
	U8 *pAddress, *pData, *pMask;
	char enabled;
	U8 key, extendedMemoryAddress, crc, maskLen;
	U16 transactionID;
	U32 memoryAddress, dataLen;

	/* Build an RMAP write command packet to write to a 4-byte register */
	pPacket = RMAP_BuildWriteRegisterPacket(pTarget, 2, pReply, 1, 1, 1, 0,
		0x20, 0, 0x106, 0, 0x12345678, &packetLen, &packetStruct, 1);
	
	/* Check the format of the packet, making sure it's not longer than */
	/* expected. Note that the first byte is skipped as RMAP_CheckPacketValid */
	/* expects the address at the start to be 1 byte long. */
	status = RMAP_CheckPacketValid((U8 *)pPacket + 1, packetLen - 1,
		&packetStruct, 1);
	printf("The packet is %sa valid RMAP packet.\n",
		(status == RMAP_SUCCESS) ? "" : "not ");

	/* Get the type of the packet */
	type = RMAP_GetPacketType(&packetStruct); 
	
	/* Display the type of the packet */
	display_packet_type(type);

	/* Get the target address of the packet */
	pAddress = RMAP_GetTargetAddress(&packetStruct, &addressLen);
	if ((!pAddress) || (addressLen == 0))
	{
		puts("No valid target address is present in the packet.");
	}
	else
	{
		/* Display the target address of the packet */
		printf("The packet has a target address of:");
		for (i = 0; i < addressLen; i++)
		{
			printf(" %x", pAddress[i]);
		}
		puts("");
	}

	/* Determine if verify before write is enabled */
	enabled = RMAP_GetVerifyBeforeWrite(&packetStruct);

	/* Display whether verify before write is enabled */
	printf("Verify before write is %senabled for the packet.\n",
		enabled ? "" : "not ");

	/* Determine if acknowledgement is enabled */
	enabled = RMAP_GetPerformAcknowledgement(&packetStruct);

	/* Display whether acknowledgement is enabled */
	printf("Acknowledgement is %senabled for the packet.\n",
		enabled ? "" : "not ");

	/* Determine if incrementing addresses is enabled */
	enabled = RMAP_GetIncrementAddress(&packetStruct);

	/* Display whether incrementing addresses is enabled */
	printf("Incrementing addresses is %senabled for the packet.\n",
		enabled ? "" : "not ");

	/* Get the key of the packet */
	key = RMAP_GetKey(&packetStruct); 
	
	/* Display the key of the packet */
	printf("The value of the key is 0x%x.\n", key);

	/* Get the transaction identifier of the packet */
	transactionID = RMAP_GetTransactionID(&packetStruct); 
	
	/* Display the transaction identifier of the packet */
	printf("The value of the transaction identifier is %d.\n", transactionID);

	/* Get the reply address of the packet */
	pAddress = RMAP_GetReplyAddress(&packetStruct, &addressLen);
	if ((!pAddress) || (addressLen == 0))
	{
		puts("No valid reply address is present in the packet.");
	}
	else
	{
		/* Display the reply address of the packet */
		printf("The packet has a reply address of:");
		for (i = 0; i < addressLen; i++)
		{
			printf(" %x", pAddress[i]);
		}
		puts("");
	}

	/* Get the memory address and extended memory address of the packet */
	memoryAddress = RMAP_GetAddress(&packetStruct, &extendedMemoryAddress); 
	
	/* Display the transaction identifier of the packet */
	printf("The value of the memory address is 0x%x ", memoryAddress);
	printf("and the extended address is 0x%x.\n", extendedMemoryAddress);

	/* Get the data in the packet */
	pData = RMAP_GetData(&packetStruct, &dataLen);
	if ((!pData) || (dataLen == 0))
	{
		puts("No valid data is present in the packet.");
	}
	else
	{
		/* Display the data in the packet */
		printf("The packet has the following data:");
		for (i = 0; i < dataLen; i++)
		{
			printf(" %x", pData[i]);
		}
		puts("");
	}

	/* Get the status of the packet */
	status = RMAP_GetStatus(&packetStruct); 
	
	/* Display the status of the packet */
	display_packet_status(status);

	/* Get the header CRC of the packet */
	crc = RMAP_GetHeaderCRC(&packetStruct); 
	
	/* Display the header CRC of the packet */
	printf("The value of the header CRC is 0x%x.\n", crc);

	/* Get the data CRC of the packet */
	crc = RMAP_GetDataCRC(&packetStruct); 
	
	/* Display the data CRC of the packet */
	printf("The value of the data CRC is 0x%x.\n", crc);

	/* Get the mask in the packet */
	pMask = RMAP_GetMask(&packetStruct, &maskLen);
	if ((!pMask) || (maskLen == 0))
	{
		puts("No valid mask is present in the packet.");
	}
	else
	{
		/* Display the data in the packet */
		printf("The packet has the following mask:");
		for (i = 0; i < maskLen; i++)
		{
			printf(" %x", pMask[i]);
		}
		puts("");
	}

	/* Free the RMAP write command packet created above */
	RMAP_FreeBuffer(pPacket);
}


