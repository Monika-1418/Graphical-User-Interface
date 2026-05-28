/**
 * \file main.cpp
 *
 * \brief Main program to run the STAR-System performance tester C++ API sample.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Main program to run the STAR-System performance tester C++ API sample.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "performance_tester.h"
#include <iostream>
using namespace std;

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

    /* create an instance of the STAR-System performance tester program and
     * run it
     */
    StarSystemPerformanceTester performanceTester;
    performanceTester.Run(argc, argv);

    /* terminate program */
    return 0;
}

