/**
 * \file examples.c
 *
 * \brief Example calls to the RMAP Packet Library.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains the main function which calls the examples for each of the
 * RMAP Packet Library functions.
 *
 * Copyright &copy; 2009 STAR-Dundee Ltd
 */

 #if defined(__rtems__)
    #include <bsp.h>
    #include <pthread.h>
    #include <stdlib.h>

    void *POSIX_Init(void *pArgs);
    
    #define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
    #define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

    #define CONFIGURE_OBJECTS_UNLIMITED
    #define CONFIGURE_UNIFIED_WORK_AREAS

    #define CONFIGURE_MAXIMUM_POSIX_THREADS rtems_resource_unlimited(4)

    #define CONFIGURE_POSIX_INIT_THREAD_TABLE

    #define CONFIGURE_INIT
    #include <rtems/confdefs.h>

    #if !defined(UNREFERENCED_PARAMETER)
        #define UNREFERENCED_PARAMETER(a)   ((void)(a))
    #endif

    int main(int argc, char *argv[]);

    void *POSIX_Init(void *pArgs)
    {
        UNREFERENCED_PARAMETER(pArgs);

        main(0, NULL);

        exit(0);
        return NULL;
    }

#endif

#include <stdio.h>

void version_example();
void crc_example();
void check_packet_example();
void write_command_packet_example();
void write_reply_packet_example();
void read_command_packet_example();
void read_reply_packet_example();
void rmw_command_packet_example();
void rmw_reply_packet_example();


int main(int argc, char *argv[])
{
	/* Version example */
	puts("Version Example");
	puts("===========");
	version_example();
	puts("");

	/* CRC example */
	puts("CRC Example");
	puts("===========");
	crc_example();
	puts("");

	/* Check packet valid example */
	puts("Check Packet Example");
	puts("====================");
	check_packet_example();
	puts("");

	/* Fill and build write command packets */
	puts("Write Command Example");
	puts("=====================");
	write_command_packet_example();
	puts("");

	/* Fill and build write reply packets */
	puts("Write Reply Example");
	puts("===================");
	write_reply_packet_example();
	puts("");

	/* Fill and build read command packets */
	puts("Read Command Example");
	puts("====================");
	read_command_packet_example();
	puts("");

	/* Fill and build read reply packets */
	puts("Read Reply Example");
	puts("==================");
	read_reply_packet_example();
	puts("");

	/* Fill and build read/modify/write command packets */
	puts("Read/Modify Write Command Example");
	puts("=================================");
	rmw_command_packet_example();
	puts("");

	/* Fill and build read/modify/write reply packets */
	puts("Read/Modify Write Reply Example");
	puts("===============================");
	rmw_reply_packet_example();
	puts("");

	return 0;
}


