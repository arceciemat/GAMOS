#include "SealTest/SealTest.h" 
#include "SealBase/Callback.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

static void cbfunc (int x, int y)
{ seal_test::out << "cbfunc " << x << ", " << y << std::endl; }


int seal_test::Callback01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Callback c;
    seal_test::out << "c defined? " << c << std::endl;

    c = CreateCallback (&cbfunc, 1, 2);
    seal_test::out << "c defined? " << c << std::endl;
    c ();

    return 0;
}

