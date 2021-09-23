#include "SealTest/SealTest.h" 
#include "SealBase/Filename.h"
#include "SealBase/TempFile.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::Filename09(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    static const char aaa [] = "a/a/a/a/a/a/a/a/a";
    Filename n (Filename::thisdir ().asDirectory ());
    TempFile::dir (n);
    Filename sub (n, aaa);
    Filename::makedir (sub, 0755, true);
    seal_test::out << "temporary dir = " << n << std::endl
	      << "sub = " << sub << std::endl
	      << "exists? " << sub.exists () << std::endl
	      << "is dir? " << sub.isDirectory () << std::endl;
    // << "mode? " << n.status ().m_mode << std::endl;
    Filename::remove (n, true);
    return 0;
}
