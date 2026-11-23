#include "SealTest/SealTest.h" 
#include "SealBase/InetSocket.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::InetSocket01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    InetSocket s;
    return 0;
}

