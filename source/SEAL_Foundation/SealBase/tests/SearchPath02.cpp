#include "SealBase/SearchPath.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace seal;

int main (int, char **argv)
{
    Signal::handleFatal (argv[0]);
    SearchPath path ("");

    std::cout << "Empty path has " << path.size () << " elements\n";
    return 0;
}
