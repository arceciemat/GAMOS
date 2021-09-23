#include "SealTest/SealTest.h" 
#include "SealBase/Environment.h"
#include "SealBase/Signal.h"

using namespace seal;

int seal_test::Environment01(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    return 0;
}
