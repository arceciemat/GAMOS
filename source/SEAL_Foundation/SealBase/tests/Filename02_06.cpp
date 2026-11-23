#include "SealTest/SealTest.h" 
#include "Filename02-00.h"

int seal_test::Filename02_06(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Filename x ("/tmp/..");
    dump ("base", x);
    dump ("expanded", x.expand ());
    dump ("substituted", x.substitute (ShellEnvironment ()));
    return 0;
}
