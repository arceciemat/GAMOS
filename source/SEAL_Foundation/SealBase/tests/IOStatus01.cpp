#include "SealTest/SealTest.h" 
#include "SealBase/IOStatus.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::IOStatus01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    IOStatus s;
    return 0;
}

