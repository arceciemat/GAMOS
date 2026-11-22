#include "SealTest/SealTest.h" 
#include "SealBase/File.h"
#include "SealBase/FileMapping.h"
#include "SealBase/Filename.h"
#include "SealBase/SystemInfo.h"
#include "SealBase/Signal.h"
#include "SealBase/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::FileMapping03(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    IOSize	n = SystemInfo::pagesize () * 10;
    FileMapping	null (n);
    IOBuffer	sect = null.map (FileMapping::ProtRead
				 | FileMapping::ProtWrite,
				 FileMapping::MapPrivate);
    char	*data = (char *) sect.data ();

    ASSERT (sect.size () == n);

    for (IOSize i = 0; i < sect.size (); ++i)
    {
	ASSERT (data [i] == 0);
	data [i] = '0' + i % 10;
    }

    // FIXME: What do we want here?  To share the previous mapping or
    // to get a new anonymous mapping?  Currently we get a brand new
    // data hunk every time...
    IOBuffer	sect2 = null.map ();
    data = (char *) sect2.data ();
    ASSERT (sect2.size () == n);

    for (IOSize i = 0; i < n; ++i)
	ASSERT (data [i] == 0);

    return 0;
}
