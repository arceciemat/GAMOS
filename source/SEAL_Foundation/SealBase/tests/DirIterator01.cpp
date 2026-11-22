#include "SealTest/SealTest.h" 
#include "SealBase/DirIterator.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"
#include <iostream>

using namespace seal;

int seal_test::DirIterator01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "scanning files in `.' using for loop:\n";
    DirIterator first (".");
    DirIterator last;
    for ( ; first != last; ++first)
	seal_test::out << *first << '\n';

    return EXIT_SUCCESS;
}
