#include "SealTest/SealTest.h" 
#include "SealBase/AutoLoad.h"
#include "SealBase/Signal.h"

using namespace seal;
int seal_test::AutoLoad01 (int, char **argv)
{
    Signal::handleFatal (argv[0]);

    AutoLoadLib			lib ("SharedLibrary01");
    AutoLoad<void (void)>	existing (lib, "libfunc");
    AutoLoad<void (void)>	nonexistent (lib, "no such thing");
    bool			trapped = false;

    (*existing) ();

    try {
	(*nonexistent) ();
    } catch (AutoLoadError &e) {
	seal_test::out << "AutoLoadError: " << e.explain () << "\n";
	trapped = true;
    }
    CPPUNIT_ASSERT (trapped);

    return EXIT_SUCCESS;
}
