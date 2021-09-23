#include "SealTest/SealTest.h" 
#include "SealBase/UUID.h"
#include "SealBase/Time.h"
#include "SealBase/Signal.h"
#include <iostream>

using namespace seal;

int seal_test::UUID05(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    const char *text = "7087E77F-8789-D911-9FCB-0030482B37E8";
    UUID t (text);
    seal_test::out << "value = " << text << "\n";
    seal_test::out << "uuid = " << t << "\n";
    seal_test::out << "time = " << t.time ().ns ()
		   << " (UTC " << t.time ().format (false, "%Y-%m-%d %H:%M:%S.")
		   << t.time ().nanoformat() << ")\n";
    seal_test::out << "type = " << t.type () << "\n";
    seal_test::out << "variant = " << t.type () << "\n";

    t = UUID::generateTime();
    seal_test::out << "uuid = " << t << "\n";
    seal_test::out << "time = " << t.time ().ns ()
		   << " (UTC " << t.time ().format (false, "%Y-%m-%d %H:%M:%S.")
		   << t.time ().nanoformat() << ")\n";
    seal_test::out << "type = " << t.type () << "\n";
    seal_test::out << "variant = " << t.type () << "\n";

    t = UUID::generate();
    seal_test::out << "uuid = " << t << "\n";
    seal_test::out << "time = " << t.time ().ns ()
		   << " (UTC " << t.time ().format (false, "%Y-%m-%d %H:%M:%S.")
		   << t.time ().nanoformat() << ")\n";
    seal_test::out << "type = " << t.type () << "\n";
    seal_test::out << "variant = " << t.type () << "\n";
    return 0;
}
