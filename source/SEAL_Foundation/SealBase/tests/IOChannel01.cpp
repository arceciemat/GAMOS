#include "SealTest/SealTest.h" 
#include "SealBase/IOChannel.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::IOChannel01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    IOChannel ch;
    return 0;
}

