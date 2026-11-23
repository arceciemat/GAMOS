#include "SealTest/SealTest.h" 
#include "SealBase/Filename.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::Filename06(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    // FIXME: status, setStatus, times

    // << "\ndrive           = " << x.drive ()
    // << "\nshare           = " << x.share ()

    // FIXME: append
	
    // FIXME: findBackupName

    // FIXME: remove
    // FIXME: rename
    // FIXME: move
    // FIXME: copy

    return 0;
}
