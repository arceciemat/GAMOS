#include "SealTest/SealTest.h" 
#include "SealBase/SystemInfo.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::SystemInfo01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "system information\n"
	      << "  pagesize  : " << SystemInfo::pagesize () << std::endl
	      << "  allocgran : " << SystemInfo::allocGranularity () << std::endl
	      << "  physical  : " << SystemInfo::physicalPagesAvailable ()
	      << "/" << SystemInfo::physicalPages () << std::endl
	      << "  virtual   : " << SystemInfo::virtualPagesAvailable ()
	      << "/" << SystemInfo::virtualPages () << std::endl
	      << "  processors: " << SystemInfo::processors ()
	      << "/" << SystemInfo::processorsOnline () << std::endl

	      << "  node name : " << SystemInfo::nodename () << std::endl
	      << "  dom name  : " << SystemInfo::domainname () << std::endl
	      << "  os name   : " << SystemInfo::osname () << std::endl
	      << "  os release: " << SystemInfo::osrelease () << std::endl
	      << "  os version: " << SystemInfo::osversion () << std::endl
	      << "  machine   : " << SystemInfo::machine () << std::endl
	      << "  processor : " << SystemInfo::processor () << "\n";

    return 0;
}
