#include "SealTest/SealTest.h" 
#include "SealBase/Hook.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

static int hfunc (int x, int y)
{ seal_test::out << "hfunc " << x << ", " << y << std::endl; return 3; }

int seal_test::Hook01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Hook<int> h;
    seal_test::out << "h defined? " << h << std::endl;

    h = CreateHook (&hfunc, 1, 2);
    seal_test::out << "h defined? " << h << std::endl;
    int n = h ();
    seal_test::out << "h = " << n << std::endl;

    return 0;
}
