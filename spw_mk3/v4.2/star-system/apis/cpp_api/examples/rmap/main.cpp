/**
 * \file main.cpp
 *
 * \brief Main program to run the STAR-System RMAP C++ API sample.
 *
 * \author STAR-Dundee Ltd\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * Main program to run the STAR-System RMAP C++ API sample.
 *
 * Copyright &copy; 2013 STAR-Dundee Ltd
 */

#include "rmap_examples.h"
#include <iostream>
using namespace std;

#include "utility.h"

/**
 * Main program.
 */
int main(int argc, char **argv)
{
    Device* pDevice = NULL;

#ifdef _WIN32
    /* declare instance of utility object which will report any memory leaks
     * detected when it goes out of scope at the end of the program
     */
    MemoryTest memoryTestObj;
#endif

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    /* perform RMAP examples */
    RMAPExamples rmapExamples;
    rmapExamples.PerformCrcExample();
    rmapExamples.PerformCheckPacketExample();
    rmapExamples.PerformReadCommandPacketExample();
    rmapExamples.PerformReadReplyPacketExample();
    rmapExamples.PerformReadModifyWriteCommandPacketExample();
    rmapExamples.PerformReadModifyWriteReplyPacketExample();
    rmapExamples.PerformWriteCommandPacketExample();
    rmapExamples.PerformWriteReplyPacketExample();
    rmapExamples.PerformVersionExample();
    rmapExamples.WriteToUserRegister(&pDevice);
    rmapExamples.ReadFromUserRegister(pDevice);

    if (pDevice != NULL)
    {
        delete (pDevice);
    }

    /* terminate program */
    return 0;
}

