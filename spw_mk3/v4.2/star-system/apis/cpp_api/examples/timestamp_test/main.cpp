/**
 * \file main.cpp
 *
 * \brief Main program to run the STAR-System timestamp test C++ API sample.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Main program to run the STAR-System timestamp test C++ API sample.
 *
 * Copyright &copy; 2017 STAR-Dundee Ltd
 */

#include "timestamp_test.h"

#include "utility.h"

/**
 * Main program.
 */
int main(int argc, char **argv)
{
#ifdef _WIN32
    /* declare instance of utility object which will report any memory leaks
     * detected when it goes out of scope at the end of the program
     */
    MemoryTest memoryTestObj;
#endif

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    /* perform timestamp test example */
    TimestampTest timestampTest;
    return timestampTest.DoTimestampTest();
}

