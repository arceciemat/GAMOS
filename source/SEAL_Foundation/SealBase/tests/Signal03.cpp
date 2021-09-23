#include "SealTest/SealTest.h" 
#include "SealBase/Signal.h"
#include "SealBase/TimeInfo.h"
#include "SealBase/SubProcess.h"
#include "SealBase/Argz.h"
#include <iostream>
#include <cstdlib>

using namespace seal;

static void inthandler (int n, siginfo_t *, void *)
{
    seal_test::out << "** child: got " << n << ", sleeping\n";
    TimeInfo::sleep (1);
    seal_test::out << "** child: woke up\n";
}

int seal_test::Signal03(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (argc == 1)
    {
	// NB: See SubProcessXX.cxx for the "0 + argv[0]" magic.
	seal_test::out << "main: starting subprocess\n";
	SubProcess s (Argz (0 + argv [0], "sub").argz ());

	TimeInfo::msleep (500);

	seal_test::out << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (300);

	seal_test::out << "main: sending SIGTERM\n";
	Signal::kill (s.pid (), SIGTERM);

	TimeInfo::msleep (2500);

	seal_test::out << "main: sending SIGUSR1\n";
	Signal::kill (s.pid (), SIGUSR1);

	seal_test::out << "main: sending SIGINT\n";
	Signal::kill (s.pid (), SIGINT);

	TimeInfo::msleep (2500);

	seal_test::out << "main: sending SIGUSR1\n";
	Signal::kill (s.pid (), SIGUSR1);

	return s.wait ();
    }
    else if (! strcmp (argv [1], "sub"))
    {
	seal_test::out << "* child: handling SIGINT + SIGTERM with all signals unblocked\n";
	sigset_t nix;
	sigemptyset (&nix);
	Signal::handle (SIGINT, &inthandler, &nix);
	Signal::handle (SIGTERM, &inthandler, &nix);

	TimeInfo::msleep (1000);

	seal_test::out << "* child: blocking SIGINT\n";
	Signal::block (SIGINT, true);

	seal_test::out << "* child: waiting for USR1 with default USR1 handler\n";
	Signal::wait (SIGUSR1);
	seal_test::out << "* child: got USR1\n";

	TimeInfo::msleep (1000);

	seal_test::out << "* child: pending SIGINT? = " << Signal::pending (SIGINT) << "\n";
	seal_test::out << "* child: unblocking SIGINT\n";
	Signal::block (SIGINT, false);

	seal_test::out << "* child: waiting for USR1 with USR1 ignored\n";
	Signal::ignore (SIGUSR1);
	bool gotit = Signal::wait (SIGUSR1, 0, 1000);
	seal_test::out << "* child: got USR1? " << gotit << "\n";
    }
    else
	abort ();

    return EXIT_SUCCESS;
}
