/***
 * \file star_test.h
 *
 * \brief Declarations for STAR-System Test program.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Contains the declarations used by the Star-System Test program.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */



#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "star-dundee_types.h"




/* Macros used to time operations, specific to individual operating systems */
#ifdef _WIN32
    #include <time.h>
    #define GET_TIME()  clock()
    #define TIME_DIVIDER    CLOCKS_PER_SEC
#else
    #include <sys/time.h>
    clock_t GET_TIME()
    {
        struct timeval tv;
        struct timezone tz;

        gettimeofday(&tv, &tz);
        return tv.tv_sec * 1000000 + tv.tv_usec;
    }
    #define TIME_DIVIDER    1000000
#endif


