#include "SealTest/SealTest.h" 
#include "Filename02-00.h"

int seal_test::Filename02_09(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Filename x ("../zoo");
    dumpl ("base", x);
    dumpl ("expanded", x.expand ());
    dumpl ("substituted", x.substitute (ShellEnvironment ()));
    return 0;
}
