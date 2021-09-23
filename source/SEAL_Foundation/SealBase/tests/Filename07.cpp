#include "SealTest/SealTest.h" 
#include "SealBase/Filename.h"
#include "SealBase/TempFile.h"
#include "SealBase/File.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::Filename07(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    static const char	sample [] = "Hello World\n";
    Filename		n (Filename::thisdir ().asDirectory ());
    File		*f = TempFile::file (n);

    seal_test::out << "temporary file = " << n << std::endl;
    f->write (sample, sizeof (sample)-1);
    f->close ();
    Filename::remove (n);
    return 0;
}
