#include "SealTest/SealTest.h" 
#include "SealBase/File.h"
#include "SealBase/Filename.h"
#include "SealBase/Signal.h"
#include "SealBase/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::File02(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    const char	data [] = "hello world\n";
    File	f ("test.out", IOFlags::OpenWrite | IOFlags::OpenCreate);

    f.write (data, sizeof (data)-1);

    return 0;
}
