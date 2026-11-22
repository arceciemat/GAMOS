#include "SealTest/SealTest.h" 
#include "SealBase/File.h"
#include "SealBase/Filename.h"
#include "SealBase/Time.h"
#include "SealBase/Signal.h"
#include "SealBase/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::File05(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    const char	data [] = "hello world\n";
    File	f ("test.out", IOFlags::OpenWrite | IOFlags::OpenCreate);
    Time	ctime, mtime, atime;

    f.write (data, sizeof (data)-1);
    f.times (&ctime, &mtime, &atime);

    seal_test::out << "ctime = " << ctime << std::endl
	      << "mtime = " << mtime << std::endl
	      << "atime = " << atime << std::endl;

    f.close ();
    return 0;
}
