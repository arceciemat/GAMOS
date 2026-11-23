#include "SealTest/SealTest.h" 
#include "SealBase/LocalSocket.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::LocalSocket01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    LocalSocket s;
    return 0;
}

