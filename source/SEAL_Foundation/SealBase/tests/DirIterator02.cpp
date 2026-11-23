#include "SealTest/SealTest.h" 
#include "SealBase/DirIterator.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <algorithm>

using namespace seal;

int seal_test::DirIterator02(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "scanning files in `.' using std::copy:\n";
    std::ostream_iterator<std::string> out (seal_test::out, "\n");
    std::copy (DirIterator ("."), DirIterator (), out);

    return EXIT_SUCCESS;
}
