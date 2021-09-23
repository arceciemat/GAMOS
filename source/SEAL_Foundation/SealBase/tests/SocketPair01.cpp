#include "SealTest/SealTest.h" 
#include "SealBase/SocketPair.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::SocketPair01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    SocketPair s;
    return 0;
}

