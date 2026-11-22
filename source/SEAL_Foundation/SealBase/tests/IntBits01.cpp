#include "SealTest/SealTest.h" 
#include "SealBase/IntBits.h"
#include "SealBase/Signal.h"
#include <typeinfo>
#include <iostream>

using namespace seal;

int seal_test::IntBits01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "8  : "
	      << "fast = (" << typeid(IntBits<8>::UFast).name ()
	      << "), exact = (" << typeid(IntBits<8>::UExact).name ()
	      << "), least = (" << typeid(IntBits<8>::ULeast).name ()
	      << ")\n";

    seal_test::out << "16 : "
	      << "fast = (" << typeid(IntBits<16>::UFast).name ()
	      << "), exact = (" << typeid(IntBits<16>::UExact).name ()
	      << "), least = (" << typeid(IntBits<16>::ULeast).name ()
	      << ")\n";

    seal_test::out << "32 : "
	      << "fast = (" << typeid(IntBits<32>::UFast).name ()
	      << "), exact = (" << typeid(IntBits<32>::UExact).name ()
	      << "), least = (" << typeid(IntBits<32>::ULeast).name ()
	      << ")\n";

    // FIXME: conditionalise for systems without a 64-bit type?  most
    // systems have one though -- and we can work harder to find one.
    seal_test::out << "64 : "
	      << "fast = (" << typeid(IntBits<64>::UFast).name ()
	      << "), exact = (" << typeid(IntBits<64>::UExact).name ()
	      << "), least = (" << typeid(IntBits<64>::ULeast).name ()
	      << ")\n";

    return 0;
}
