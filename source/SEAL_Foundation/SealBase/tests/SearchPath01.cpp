#include "SealTest/SealTest.h" 
#include "SealBase/SearchPath.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace seal;

static void dosearch (const SearchPath &path, const std::string &name)
{
    SearchPath::const_iterator pos = path.search (name);
    if (pos == path.end ())
	seal_test::out << "`" << name << "' is not found";
    else
	seal_test::out << "`" << name << "' is in `" << *pos << "'";

    seal_test::out << " (expanded: `" << path.expand (name) << "')\n";
}

int seal_test::SearchPath01(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    SearchPath path (getenv ("PATH"));

    seal_test::out << "$PATH is:\n";
    for (SearchPath::iterator i = path.begin (); i != path.end (); ++i)
	seal_test::out << "  " << *i << "\n";

    dosearch (path, "echo");
    dosearch (path, "gobbledygoo");
    return 0;
}
