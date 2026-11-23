#include "SealTest/SealTest.h" 
#include "SealBase/Filename.h"
#include "SealBase/TempFile.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::Filename01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out <<   "tmpdir          = " << TempFile::tempdir ()
	      << "\ncwd             = " << Filename::cwd ()
	      << "\nnull            = " << Filename::null ()
	      << "\n";

    return 0;
}
