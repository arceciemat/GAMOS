#include "SealTest/SealTest.h" 
#include "SealBase/HostInfo.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::HostInfo01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "host information\n"
	      << "  name      : " << HostInfo::name () << std::endl
	      << "  dnsname   : " << HostInfo::dnsname () << std::endl
	      << "  dnsdomain : " << HostInfo::dnsdomain () << std::endl
	      << "  fqdn      : " << HostInfo::fqdn () << "\n";

    return 0;
}
