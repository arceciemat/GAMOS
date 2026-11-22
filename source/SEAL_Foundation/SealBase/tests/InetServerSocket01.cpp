#include "SealTest/SealTest.h" 
#include "SealBase/InetServerSocket.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::InetServerSocket01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    InetServerSocket s (InetAddress (5678));
    return 0;
}

