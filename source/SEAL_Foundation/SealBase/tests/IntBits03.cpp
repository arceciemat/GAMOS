#include "SealTest/SealTest.h" 
#include "SealBase/IntBits.h"
#include "SealBase/Signal.h"
#include <typeinfo>
#include <iostream>

using namespace seal;

int seal_test::IntBits03(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "8 :"
	      << " min=" << SEAL_INT8_MIN
	      << " max=" << SEAL_INT8_MAX
	      << " umax=" << SEAL_UINT8_MAX
	      << "\n";
	
    seal_test::out << "16 :"
	      << " min=" << SEAL_INT16_MIN
	      << " max=" << SEAL_INT16_MAX
	      << " umax=" << SEAL_UINT16_MAX
	      << "\n";
	
    seal_test::out << "32 :"
	      << " min=" << SEAL_INT32_MIN
	      << " max=" << SEAL_INT32_MAX
	      << " umax=" << SEAL_UINT32_MAX
	      << "\n";
	
    seal_test::out << "64 :"
	      << " min=" << SEAL_INT64_MIN
	      << " max=" << SEAL_INT64_MAX
	      << " umax=" << SEAL_UINT64_MAX
	      << "\n";

    return 0;
}

