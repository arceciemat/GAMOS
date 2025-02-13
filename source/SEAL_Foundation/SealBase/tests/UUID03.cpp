#include "SealTest/SealTest.h" 
#include "SealBase/UUID.h"
#include "SealBase/Signal.h"
#include <iostream>

using namespace seal;
typedef seal::UUID TestUUID;

int seal_test::UUID03(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    seal_test::out << TestUUID::generateRandom () << std::endl;
    return 0;
}
