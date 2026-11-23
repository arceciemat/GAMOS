#include "SealTest/SealTest.h" 
#include "SealBase/DirIterator.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <algorithm>

using namespace seal;

static void dump_file (const std::string &file)
{
    seal_test::out << file << "\n";
}

int seal_test::DirIterator03(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "scanning files in `.' using std::for_each:\n";
    std::for_each (DirIterator ("."), DirIterator (), dump_file);

    return EXIT_SUCCESS;
}
