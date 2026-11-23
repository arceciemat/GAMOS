#include "SealTest/SealTest.h" 
#include "SealBase/SaveErrno.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>
#include <cerrno>

using namespace seal;

int seal_test::SaveErrno01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "errno (default) = " << errno << std::endl;
    errno = EINVAL;
    seal_test::out << "errno (EINVAL) = " << errno << std::endl;
    {
	SaveErrno s;
	errno = EDOM;
	seal_test::out << "errno (EDOM) = " << errno << std::endl;
    }
    seal_test::out << "errno (EINVAL) = " << errno << std::endl;
    
    return 0;
}
