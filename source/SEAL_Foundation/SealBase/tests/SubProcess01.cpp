#include "SealTest/SealTest.h" 
#include "SealBase/SubProcess.h"
#include "SealBase/Argz.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::SubProcess01(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (argc == 1)
    {
	// NB: The "0 + argv [0]" below is purely for disambiguation
	// for GCC 2.95.2.  It appears to parse the s1 declaration as
	// a function instead of object constructor and fails to
	// backtrack as specified in the standard.  Any number of
	// things can be done to it (e.g. cast), but it must be done
	// to make "argv" argument to Argz appear as an expression,
	// anything else is too late.
	seal_test::out << "main running\n" << std::flush;
	SubProcess s1 (Argz (0 + argv [0], "sub").argz ());
	int val = s1.wait ();
	seal_test::out << "child returned " << val << std::endl
		  << " normal? " << SubProcess::exitNormal (val) << "\n"
		  << " signal? " << SubProcess::exitBySignal (val) << "\n"
		  << " status  " << SubProcess::exitStatus (val) << "\n"
		  << " signr   " << SubProcess::exitSignal (val) << "\n";
    }
    else if (! strcmp (argv [1], "sub"))
	seal_test::out << " * child\n";
    else
	abort ();

    return EXIT_SUCCESS;
}
