#include "SealTest/SealTest.h" 
#include "SealBase/File.h"
#include "SealBase/FileMapping.h"
#include "SealBase/Filename.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::FileMapping01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    File	*f = new File ("map.out",
			       IOFlags::OpenRead
			       | IOFlags::OpenWrite
			       | IOFlags::OpenCreate);
    f->write ("Hello world", 11);

    FileMapping	fmap (f);
    IOBuffer	section = fmap.map ();

    seal_test::out << "data = <";
    seal_test::out.write ((char *) section.data (), section.size ());
    seal_test::out << ">\n";

    return 0;
}
