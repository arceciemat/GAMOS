#include "SealTest/SealTest.h" 
#include "SealBase/BitTraits.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::BitTraits01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "built-in type information\n"
	      << "  char      : "
	      << BitTraits<unsigned char>::Bits << "/"
	      << BitTraits<unsigned char>::Bytes << "/"
	      << BitTraits<unsigned char>::Digits << "/"
	      << BitTraits<unsigned char>::HexDigits << std::endl
	      << "  short     : "
	      << BitTraits<unsigned short>::Bits << "/"
	      << BitTraits<unsigned short>::Bytes << "/"
	      << BitTraits<unsigned short>::Digits << "/"
	      << BitTraits<unsigned short>::HexDigits << std::endl
	      << "  int       : "
	      << BitTraits<unsigned int>::Bits << "/"
	      << BitTraits<unsigned int>::Bytes << "/"
	      << BitTraits<unsigned int>::Digits << "/"
	      << BitTraits<unsigned int>::HexDigits << std::endl
	      << "  long      : "
	      << BitTraits<unsigned long>::Bits << "/"
	      << BitTraits<unsigned long>::Bytes << "/"
	      << BitTraits<unsigned long>::Digits << "/"
	      << BitTraits<unsigned long>::HexDigits << std::endl
#if HAVE_LONG_LONG
	      << "  long long : "
	      << BitTraits<unsigned long long>::Bits << "/"
	      << BitTraits<unsigned long long>::Bytes << "/"
	      << BitTraits<unsigned long long>::Digits << "/"
	      << BitTraits<unsigned long long>::HexDigits << std::endl
#elif HAVE_INT64
	      << "  __int64   : "
	      << BitTraits<unsigned __int64>::Bits << "/"
	      << BitTraits<unsigned __int64>::Bytes << "/"
	      << BitTraits<unsigned __int64>::Digits << "/"
	      << BitTraits<unsigned __int64>::HexDigits << std::endl
#endif
	      << std::endl;

    return 0;
}
