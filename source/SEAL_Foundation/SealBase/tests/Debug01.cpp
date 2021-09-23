#include "SealTest/SealTest.h" 
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"

using namespace seal;

static void h (void);
static void g (void);
static void f (void);

int seal_test::Debug01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    f ();
    return 0;
}

void f (void)
{
    g ();
}

void g (void)
{
    h ();
}


void h (void)
{
    ASSERT (false);
}

