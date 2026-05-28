/**
 * \file main.cpp
 *
 * \brief Main program to run the STAR-System time-code C++ API sample.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Main program to run the STAR-System time-code C++ API sample.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "time_code_example.h"
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

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    /* perform time-code example */
    TimeCodeExample timeCodeExample;
    timeCodeExample.Run();

    /* terminate program */
    return 0;
}

