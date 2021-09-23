#include "SealTest/SealTest.h" 
#include "SealBase/ShellEnvironment.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::ShellEnvironment01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    ShellEnvironment env;
    std::string t;

    seal_test::out << "$PATH = " << (env.get ("PATH", t) ? t : "") << std::endl
	      << "have $SHELL? " << env.has ("SHELL") << std::endl
	      << "have $TEST? " << env.has ("TEST") << std::endl;

    env.unset ("TEST");
    seal_test::out << "have $TEST? " << env.get ("TEST", t) << std::endl
	      << "$TEST = " << t << std::endl;

    env.set ("TEST", "foo");
    seal_test::out << "have $TEST? " << env.has ("TEST") << std::endl
	      << "$TEST = " << (env.get ("TEST", t), t) << std::endl;

    return 0;
}
