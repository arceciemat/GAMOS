#include "SealTest/SealTest.h" 
#include "SealBase/PipeCmd.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::PipeCmd01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    PipeCmd p;
    return 0;
}

