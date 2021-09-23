#include "SealTest/SealTest.h" 
#include "SealBase/Filename.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace seal;

int seal_test::Filename03(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Filename			prefix ("r");
    std::vector<Filename>	vec;
    bool			exact;

    prefix.complete (vec, Filename::cwd ());
    seal_test::out << "\ncomplete (" << prefix << "):\n";
    for (size_t i = 0; i < vec.size (); ++i)
	seal_test::out << "  " << vec [i] << "\n";

    if (prefix.complete (Filename::cwd (), &exact))
    {
	seal_test::out << " ==> " << prefix
		  << (exact ? " (exact)\n" : "\n");
    }
    else
	seal_test::out << " ... no match\n";

    return 0;
}
