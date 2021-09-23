#include "SealTest/SealTest.h" 
#include "SealBase/Log.h"
#include "SealBase/Signal.h"
#include <ostream>

using namespace seal;

int seal_test::Log01(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    LOG (2, trace, LFgeneral, "foo\n");
    LOG (2, trace, LFgeneral, "test manips" << std::endl);
    return 0;
}
