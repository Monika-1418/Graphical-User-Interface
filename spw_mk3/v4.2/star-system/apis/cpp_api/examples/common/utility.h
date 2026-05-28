/**
 * \file utility.h
 *
 * \brief Declarations of general utility functions for the STAR-System Test
 *        program.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Declarations of utility functions, and definitions of macros, used by the
 * STAR-System Test program.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include <time.h>
#include "star-api.h"

#ifndef UTILITYFILE_H
#define UTILITYFILE_H

#if !defined(MIN)
    #define MIN(a,b)    ((a) < (b) ? (a) : (b))
#endif
#if !defined(MAX)
    #define MAX(a,b)    ((a) > (b) ? (a) : (b))
#endif

#define DATA_TYPE_0             0
#define DATA_TYPE_1             1
#define DATA_TYPE_RANDOM        2
#define DATA_TYPE_COUNT         3
#define DATA_TYPE_NOT_COUNT     4

#if !defined(SLEEP)
    #ifdef _WIN32
        #include "windows.h"
        #define SLEEP(milliseconds) Sleep(milliseconds);
    #else
        #include <unistd.h>
        #define SLEEP(milliseconds) usleep(milliseconds * 1000);
    #endif
#endif

#ifdef _WIN32
#include <iostream>
using namespace std;

/* memory checking */
#define __CHECK_FOR_MEMORY_LEAKS__

#ifdef __CHECK_FOR_MEMORY_LEAKS__
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

/**
 * Utility class to check for memory leaks after a program has run
 */
class MemoryTest
{
public:
    /* constructor - do nothing */
    MemoryTest() {}

    /* destructor - check for memory leaks when this object goes out of
     * scope */
    ~MemoryTest()
    {
        #ifdef __CHECK_FOR_MEMORY_LEAKS__
            _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
            if (_CrtDumpMemoryLeaks())
            {
                cout << "\nMEMORY LEAKS DETECTED!!\n" << endl;
            }
            else
            {
                cout << "No memory leaks detected\n" << endl;
            }

            /* halt automatic closure of console window  */
            cout << "Please press Enter to continue..\n" << endl;
            char tmp[256];
            fgets(tmp, 256, stdin);
        #endif
    }
};
#endif

#endif /* UTILITYFILE_H */
