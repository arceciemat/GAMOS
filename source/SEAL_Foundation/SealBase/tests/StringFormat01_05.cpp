#include "SealTest/SealTest.h" 
#include "SealBase/StringFormat.h"
#include "SealBase/Signal.h"
#include <iostream>

using namespace seal;

int seal_test::StringFormat01_05(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << StringFormat ("Testing `%1'").arg (10, 0, 16) << std::endl;
    return 0;
}