#include "SealTest/SealTest.h" 
#include "SealBase/Signal.h"
#include "SealBase/TimeInfo.h"
#include <iostream>
#include <cstdlib>

using namespace seal;

static void inthandler (int, siginfo_t *, void *)
{
    seal_test::out << "  Got SIGINT\n";
    TimeInfo::sleep (1);
    seal_test::out << "  Woke up\n";
}

int seal_test::Signal01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "Ignoring SIGINT\n";
    Signal::ignore (SIGINT);

    seal_test::out << "Raising SIGINT\n";
    Signal::raise (SIGINT);

    seal_test::out << "Handling SIGINT\n";
    Signal::handle (SIGINT, &inthandler);

    seal_test::out << "Raising SIGINT\n";
    Signal::raise (SIGINT);

    seal_test::out << "Blocking SIGINT\n";
    Signal::block (SIGINT, true);

    seal_test::out << "Raising SIGINT\n";
    Signal::raise (SIGINT);

    seal_test::out << "Pending SIGINT? = " << Signal::pending (SIGINT) << "\n";

    seal_test::out << "Unblocking SIGINT\n";
    Signal::block (SIGINT, false);
    
    seal_test::out << "Raising SIGINT\n";
    Signal::raise (SIGINT);

    return EXIT_SUCCESS;
}
