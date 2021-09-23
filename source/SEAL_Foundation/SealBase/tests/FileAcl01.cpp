#include "SealTest/SealTest.h" 
#include "SealBase/FileAcl.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::FileAcl01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    FileAcl acl;
    return 0;
}

