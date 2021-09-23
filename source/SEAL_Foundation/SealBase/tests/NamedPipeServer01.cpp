#include "SealTest/SealTest.h" 
#include "SealBase/NamedPipeServer.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::NamedPipeServer01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    NamedPipeServer p;
    return 0;
}

