#include "SealTest/SealTest.h" 
#include "SealBase/UserInfo.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::UserInfo01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    seal_test::out << "user information\n"
	      << "  uid       : effective " << UserInfo::self ()->uid ()
	      << ", real " << UserInfo::self ()->uid (false) << std::endl
	      << "  gid       : effective " << UserInfo::self ()->gid ()
	      << ", real " << UserInfo::self ()->gid (false) << std::endl
	      << "  id        : " << UserInfo::self ()->id () << std::endl
	      << "  domain    : " << UserInfo::self ()->domain () << std::endl
	      << "  server    : " << UserInfo::self ()->logonserver () << std::endl
	      << "  name      : " << UserInfo::self ()->name () << std::endl
	      << "  homedir   : " << UserInfo::self ()->homedir () << std::endl
	      << "  homedrive : " << UserInfo::self ()->homedrive () << std::endl
	      << "  password  : " << UserInfo::self ()->password () << std::endl
	      << "  shell     : " << UserInfo::self ()->shell () << "\n\n";

    return 0;
}
