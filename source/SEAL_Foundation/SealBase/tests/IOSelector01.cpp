#include "SealTest/SealTest.h" 
#include "SealBase/IOSelector.h"
#include "SealBase/IOSelectEvent.h"
#include "SealBase/PipeCmd.h"
#include "SealBase/TimeInfo.h"
#include "SealBase/Signal.h"
#include "SealBase/DebugAids.h"
#include <iostream>
#include <iomanip>

using namespace seal;

static bool reader (IOSelectEvent *event)
{
    bool		done = false;
    unsigned char	buf [1024];
    IOSize		sz;

    do
	if ((sz = event->source->read (buf, sizeof (buf))))
	{
	    seal_test::out << "<" << std::flush;
	    seal_test::out.write ((char *) buf, sz);
	    seal_test::out << ">\n" << std::flush;
	}
	else
	    done = true;
    while (sz == sizeof (buf));

    return done;
}


int seal_test::IOSelector01(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (argc == 1)
    {
	const char	*inargs [] = { argv [0], "input", 0 };
	PipeCmd		in (inargs, IOFlags::OpenRead);
	IOSelector	s;

	in.setBlocking (false);
	s.attach (&in, IORead, CreateHook (reader));
	while (s.dispatch ())
	    ;

	// close pipes gracefully (FIXME: .terminate () / .abort ()?)
	// FIXME: do this in PipeCmd destructor?
	in.close (); // FIXME: protection against already closed?
	return in.wait ();
    }
    else if (argc == 2 && ! strcmp (argv [1], "input"))
    {
	// Output with delay
	static const char message [] = "Hello world!";
	for (const char *s = message; *s; ++s)
	{
	    TimeInfo::sleep (1);
	    seal_test::out << *s;
	    if (*s >= 'l')
		seal_test::out << *++s;
	    seal_test::out.flush ();
	}

	return EXIT_SUCCESS;
    }
    else
	abort ();

    return 0;
}

