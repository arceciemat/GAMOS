#include "SealTest/SealTest.h" 
#include "SealBase/MapIterator.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <string>
#include <map>

using namespace seal;
int seal_test::MapIterator02(int, char **argv)
{
    Signal::handleFatal (argv[0]);
    typedef std::map<int, std::string> TestMap;
    TestMap m;
    m [0] = "foo";
    m [-1] = "bar";
    m [2] = "zoo";
    m [-2] = "goo";

    std::copy (MapKeyIterator<TestMap> (m.begin ()),
	       MapKeyIterator<TestMap> (m.end ()),
	       std::ostream_iterator<int> (seal_test::out, "\n"));

    return 0;
}

