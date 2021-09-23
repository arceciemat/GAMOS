#include "SealTest/SealTest.h" 
#include "SealBase/Unwind.h"
#include "SealBase/Signal.h"

using namespace seal;

int seal_test::Unwind01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Unwind<int> p;
    return 0;
}
