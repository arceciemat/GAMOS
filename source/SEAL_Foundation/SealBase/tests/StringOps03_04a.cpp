#include "SealTest/SealTest.h" 
#include "SealBase/StringOps.h"
#include "SealBase/Signal.h"
#include <iostream>

using namespace seal;

int seal_test::StringOps03_04a(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << StringOps::join
	(StringOps::split ("::abc:123:", ':'), "\n")
	      << "\n";
    return 0;
}
