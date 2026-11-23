#include "SealTest/SealTest.h" 
#include "SealBase/ProcessInfo.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::ProcessInfo01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "process information\n"
	      << "  pid       : " << ProcessInfo::pid () << std::endl
	      << "  ppid      : " << ProcessInfo::ppid () << "\n\n";

    for (char **args = ProcessInfo::argv (); args && *args; ++args, ++argv)
	seal_test::out << "args <" << *args << ">; argv <" << *argv << ">\n";

    seal_test::out << "\nfull program name: " << ProcessInfo::fullProgramName () << "\n";
    seal_test::out << "short program name: " << ProcessInfo::shortProgramName () << "\n";

    return 0;
}
