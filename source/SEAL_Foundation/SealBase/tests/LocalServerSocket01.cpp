#include "SealTest/SealTest.h" 
#include "SealBase/LocalServerSocket.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::LocalServerSocket01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    LocalServerSocket s ("/tmp/foo");
    return 0;
}

