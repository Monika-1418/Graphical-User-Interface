/**
 * \file version_example.c
 *
 * \brief Example call to get the version information of the RMAP Packet
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
 * obtain the version information of the RMAP Packet Library.  It also contains
 * calls to the macros to access the fields in the version information.
 *
 * Copyright &copy; 2009 STAR-Dundee Ltd
 */

#include <stdio.h>

#include "rmap_packet_library.h"

void version_example()
{
	U32 version;
	U16 edit;
	U8 patch;

	version = RMAP_GetVersion();

	printf("RMAP Packet Library v%d.%02d", RMAP_GET_VERSION_MAJOR(version),
		RMAP_GET_VERSION_MINOR(version));
	edit = RMAP_GET_VERSION_EDIT(version);
	if (edit)
	{
		printf(" edit %d", edit);
	}
	patch = RMAP_GET_VERSION_PATCH(version);
	if (patch)
	{
		printf(" patch level %d", patch);
	}
	puts("");
}


