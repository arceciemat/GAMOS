#include "SealTest/SealTest.h" 
#include "SealBase/Filename.h"
#include "SealBase/TempFile.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::Filename08(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Filename n (Filename::thisdir ().asDirectory ());
    TempFile::dir (n);

    seal_test::out << "temporary dir = " << n << std::endl
	      << "exists? " << n.exists () << std::endl
	      << "is dir? " << n.isDirectory () << std::endl;
    // << "mode? " << n.status ().m_mode << std::endl;
    Filename::remove (n);
    return 0;
}
