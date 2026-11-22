#include "SealTest/SealTest.h" 
#include "SealBase/SharedLibrary.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <cstdlib>
#include <string>

using namespace seal;

static void
dumplibs (const SharedLibrary::LibraryInfo &info)
{
    seal_test::out << "  " << info.m_filename << std::hex << " text = 0x"
	      << info.m_text_start << " - 0x" << info.m_text_end << ", data = 0x"
	      << info.m_data_start << " - 0x" << info.m_data_end << ", bss = 0x"
	      << info.m_bss_start << " - 0x" << info.m_bss_end << "\n";
}

int seal_test::SharedLibrary01 (int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "shared library search path is: "
	      << SharedLibrary::path () << std::endl;

    std::string libname = SharedLibrary::libname ("SharedLibrary01");
    seal_test::out << "loading `" << libname << "'... " << std::flush;
    SharedLibrary *lib;
    try 
    {
	lib = SharedLibrary::load (libname);
    }
    catch (SharedLibraryError &error) 
    {
	seal_test::out << "failed: " << error.explain () << std::endl;
	return EXIT_FAILURE;
    }
    seal_test::out << "done!\n";

    static const char symname [] = "libfunc";
    seal_test::out << "retrieving `" << symname << "'... " << std::flush;
    SharedLibrary::Function address;

    try
    {
	address = lib->function (symname);
    }
    catch (SharedLibraryError &error)
    {
	seal_test::out << "failed: " << error.explain () << std::endl;
	return EXIT_FAILURE;
    }
    seal_test::out << "done!\n";

    seal_test::out << "calling `" << symname << "'... " << std::flush;
    (*address) ();
    seal_test::out << "done!\n";

    seal_test::out << "loaded shared libraries list:\n";
    try {
	SharedLibrary::loaded (CreateCallback (&dumplibs));
    }
    catch (SharedLibraryError &error)
    {
	seal_test::out << "failed: " << error.explain () << std::endl;
	return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
