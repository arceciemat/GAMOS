#include "SealTest/SealTest.h" 
#include "SealBase/StringOps.h"
#include "SealBase/Signal.h"
#include <iostream>

using namespace seal;

int seal_test::StringOps01_01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    StringList l;
    l.push_back ("abc");
    l.push_back ("123");
    seal_test::out << StringOps::join (l, ":")
	      << std::endl;
    return 0;
}
