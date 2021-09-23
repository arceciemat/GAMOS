#include "SealTest/SealTest.h" 
#include "SealBase/Signal.h"
#include "SealBase/TimeInfo.h"
#include "SealBase/SubProcess.h"
#include "SealBase/Argz.h"
#include <iostream>
#include <cstdlib>

using namespace seal;

static void inthandler (int, siginfo_t *, void *)
{
    seal_test::out << "** child: got SIGINT, sleeping\n";
    TimeInfo::sleep (1);
    seal_test::out << "** child: woke up\n";
}

int seal_test::Signal02(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (argc == 1)
    {
	// NB: See SubProcessXX.cxx for the "0 + argv[0]" magic.
	seal_test::out << "main: starting subprocess\n";
	SubProcess s (Argz (0 + argv [0], "sub").argz ());

	TimeInfo::msleep (100);

	seal_test::out << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (300);

	seal_test::out << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (300);

	seal_test::out << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (300);

	seal_test::out << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (2000);

	seal_test::out << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	return s.wait ();
    }
    else if (! strcmp (argv [1], "sub"))
    {
	seal_test::out << "* child: ignoring SIGINT\n";
	Signal::ignore (SIGINT);

	TimeInfo::msleep (300);

	seal_test::out << "* child: handling SIGINT\n";
	Signal::handle (SIGINT, &inthandler);

	TimeInfo::msleep (300);

	seal_test::out << "* child: blocking SIGINT\n";
	Signal::block (SIGINT, true);

	TimeInfo::msleep (1000);

	seal_test::out << "* child: pending SIGINT? = " << Signal::pending (SIGINT) << "\n";
	seal_test::out << "* child: unblocking SIGINT\n";
	Signal::block (SIGINT, false);
    }
    else
	abort ();

    return EXIT_SUCCESS;
}
