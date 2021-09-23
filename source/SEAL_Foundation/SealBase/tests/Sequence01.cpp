#include "SealTest/SealTest.h" 
#include "SealBase/Sequence.h"
#include "SealBase/Signal.h"
#include <iostream>

using namespace seal;

int seal_test::Sequence01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Sequence<int> s;
    seal_test::out << "s = [" << s.limits ().low ()
	      << ", " << s.limits ().high () << "]\n"
	      << "current = " << s.current () << "\n";

    s.next ();

    seal_test::out << "current = " << s.current () << "\n";
    return 0;
}
