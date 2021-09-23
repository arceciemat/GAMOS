#include "SealTest/SealTest.h" 
#include "SealBase/File.h"
#include "SealBase/FileLock.h"
#include "SealBase/Filename.h"
#include "SealBase/Signal.h"
#include "SealBase/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::FileLock01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    FileLock l;
    return 0;
}
