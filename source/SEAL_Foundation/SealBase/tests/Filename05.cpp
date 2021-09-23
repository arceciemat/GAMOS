#include "SealTest/SealTest.h" 
#include "SealBase/Filename.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace seal;

int seal_test::Filename05(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Filename			pat ("[a-p]*test*");
    std::vector<Filename>	vec;

    pat.glob (vec, Filename::cwd ());
    seal_test::out << "\nglob (" << pat << "):\n";
    for (size_t i = 0; i < vec.size (); ++i)
	seal_test::out << "  " << vec [i] << "\n";

    // FIXME: remove
    // FIXME: rename
    // FIXME: move
    // FIXME: copy

    return 0;
}









