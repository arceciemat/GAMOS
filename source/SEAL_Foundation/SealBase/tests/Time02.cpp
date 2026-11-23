#include "SealTest/SealTest.h" 
#include "SealBase/Time.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::Time02(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Time	t = Time::current ();
    unsigned	d = Time::toDosDate (t);
    Time        t2 = Time::fromDosDate (d);

    seal_test::out << "time:  " << t << std::endl
	      << "dos:   " << d << std::endl
	      << "undos: " << t2 << std::endl << std::endl;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( t.second(true),  t2.second(true), 1 ); 
   
    t += 5 * 30 * TimeConst::SECS_PER_DAY * TimeConst::SEC_NSECS;
    d = Time::toDosDate (t);
    t2 = Time::fromDosDate (d);

    seal_test::out << "time:  " << t << std::endl
	      << "dos:   " << d << std::endl
	      << "undos: " << t2 << std::endl;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( t.second(true),  t2.second(true), 1 ); 

    return 0;
}
