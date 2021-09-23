#include "SealTest/SealTest.h" 
#include "SealBase/Pipe.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::Pipe01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Pipe p;
    return 0;
}

