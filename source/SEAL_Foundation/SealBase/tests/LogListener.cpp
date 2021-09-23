#include "SealTest/SealTest.h" 
#include "SealBase/Signal.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Log.h"
#include "SealBase/Error.h"
#include "SealBase/LocalServerSocket.h"
#include "SealBase/Filename.h"
#include <iostream>
#include <cerrno>
#include <cstdlib>

using namespace seal;

static char			*sockname = 0;
static LocalServerSocket	*sock = 0;

static void cleanup (void)
{
    try
    {
	if (sock)
	{
	    sock->close ();
	    delete sock;
	    sock = 0;
	}

	if (sockname)
	{
	    Filename::remove (sockname, false, true);
	    sockname = 0;
	}
    }
    catch (...)
    {
    }
}

static void interrupt (int, siginfo_t *, void *)
{
    cleanup ();
    seal_test::out << "\nInterrupted, exiting...\n";
    exit (0);
}

int seal_test::LogListener(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    // Check parameters
    if (argc != 2)
    {
	// Print usage but don't make it an error to be qmtest happy
	std::cout << "usage: " << argv [0] << " socket-name\n";
	return EXIT_SUCCESS;
    }
    sockname = argv [1];

    if (Filename (sockname).exists ())
    {
	std::cerr << sockname << ": already exists\n";
	return EXIT_FAILURE;
    }

    Signal::handle (SIGINT, &interrupt);

    try
    {
	sock = new LocalServerSocket (sockname);
    }
    catch (Error &e)
    {
	std::cerr << argv [0] << ": error: " << e.explain () << std::endl;
	cleanup ();
	return EXIT_FAILURE;
    }

    while (true)
    {
	Socket *data = 0;
	try
	{
	    data = sock->accept ();
	}
	catch (Error &e)
	{
	    seal_test::out << "error: " << e.explain () << std::endl;
	    delete data;
	    cleanup ();
	    return EXIT_FAILURE;
	}

	LocalAddress peer = ((LocalSocket *) data)->peername ();
	if (strlen (peer.path ()))
	    seal_test::out << "connection established from `" << peer.path () << "'\n";
	else
	    seal_test::out << "connection established\n"; 
	seal_test::out.flush ();

	char	buffer [512];
	IOSize	n;

	while (true)
	{
	    try
	    {
		n = data->read (buffer, sizeof (buffer));
	    }
	    catch (Error &e)
	    {
		seal_test::out << "error: " << e.explain () << std::endl;
		delete data;
		break;
	    }

	    if (n == 0)
	    {
		seal_test::out << "connection closed\n";
		seal_test::out.flush ();
		data->close ();
		delete data;
		break;
	    }
	    else
		seal_test::out.write (buffer, n).flush ();
	}
    }

    std::cerr << "\n\noops\n"; // What happened? We shouldn't get here.
    return 1;
}
