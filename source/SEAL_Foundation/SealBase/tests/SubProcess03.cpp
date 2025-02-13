#include "SealTest/SealTest.h" 
#include "SealBase/SubProcess.h"
#include "SealBase/Argz.h"
#include "SealBase/TimeInfo.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::SubProcess03(int argc, char **argv)
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
	SubProcess s1 (Argz (0 + argv [0], "output").argz (),
		       SubProcess::First);
	SubProcess s2 (Argz (0 + argv [0], "input").argz (),
		       SubProcess::Last | SubProcess::Write,
		       s1.pipe ());

	TimeInfo::msleep (80);
	seal_test::out << "main, s1 ready = " << s1.done () << std::endl;
	seal_test::out << "main, s2 ready = " << s2.done () << std::endl;

	TimeInfo::msleep (4000);
	seal_test::out << "main, s1 ready = " << s1.done () << std::endl;
	seal_test::out << "main, s2 ready = " << s2.done () << std::endl;

	int val = s1.wait ();
	seal_test::out << "first child returned " << val << std::endl
		  << " normal? " << SubProcess::exitNormal (val) << "\n"
		  << " signal? " << SubProcess::exitBySignal (val) << "\n"
		  << " status  " << SubProcess::exitStatus (val) << "\n"
		  << " signr   " << SubProcess::exitSignal (val) << "\n";

	val = s2.wait ();
	seal_test::out << "last child returned " << val << std::endl
		  << " normal? " << SubProcess::exitNormal (val) << "\n"
		  << " signal? " << SubProcess::exitBySignal (val) << "\n"
		  << " status  " << SubProcess::exitStatus (val) << "\n"
		  << " signr   " << SubProcess::exitSignal (val) << "\n";
    }
    else if (argc == 2 && ! strcmp (argv [1], "output"))
    {
	// Output with delay
	static const char message []
	    = "Hello world!\nHello world!\n"
	    "1234567890abcdefghijkmnopqrstuvwxyz\n"
	    "easldjslk lasjd laks jd!\nH";
	for (const char *s = message; *s; ++s)
	{
	    TimeInfo::msleep (37);
	    seal_test::out << *s;
	    if ((*s & 1) && s[1])
		seal_test::out << *++s;
	    seal_test::out.flush ();
	}
    }
    else if (argc == 2 && ! strcmp (argv [1], "input"))
    {
	// Input with delay
	char ch;
	while (! std::cin.get (ch).eof ())
	{
	    seal_test::out << ch << std::flush;
	    TimeInfo::msleep (11);
	}
    }
    else
	abort ();
    
    return EXIT_SUCCESS;
}
