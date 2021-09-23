#include "SealTest/SealTest.h" 
#include "SealBase/StringFormat.h"
#include "SealBase/Signal.h"
#include <iostream>

using namespace seal;

int seal_test::StringFormat03_04(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << StringFormat ("Testing `%1%2'\n")
	.arg (10.0, 0, 'f', 2)
	.arg ('%');
    return 0;
}
