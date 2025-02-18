#include "SealTest/SealTest.h" 
#include "SealBase/Time.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::Time01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    Time	t = Time::current ();
    Time	t0 = t; 
    TimeSpan	s (10 * TimeConst::SECS_PER_DAY * TimeConst::SEC_NSECS);

    seal_test::out << "time         = " << t << std::endl
	      << " = " << t.format (true,  " %Y-%m-%d %H:%M:%S (local)  .")
	      << t.nanoformat () << "\n"
	      << " = " << t.format (false, " %Y-%m-%d %H:%M:%S (UTC)   + ")
	      << TimeSpan (t).lastNSeconds () << " ns\n";

    // diff is 1 less when in summer due to daylight 
    int * daylight = 0;
    Time dutc(t.utcoffset(daylight)) ;
    seal_test::out << "Offset to UTC is " << dutc.hour(true) << " hours " << daylight << std::endl; 
    if (daylight) 
      CPPUNIT_ASSERT( t.hour(true)%24 == (t.hour(false) + dutc.hour(true) )%24); 
    else 
      CPPUNIT_ASSERT( t.hour(true)%24 == (t.hour(false) + dutc.hour(true) -1 )%24); 

    seal_test::out << "time2    = " << t << std::endl
	      << " = " << t.format (true,  " %Y-%m-%d %H:%M:%S (local)  .")
	      << t.nanoformat () << "\n"
	      << " = " << t.format (false, " %Y-%m-%d %H:%M:%S (UTC)   + ")
	      << t.nsecond () << " ns\n";

    t += s;


    // check difference in days
    int diff = t.day(true)-t0.day(true); 
    if (diff < 0) 
    { // add 31 or 30 or 28/29 according to month
      int m=t0.month(true);
      int daysInMonth = 31; 
      if (m == 3 || m == 5 || m == 8 || m == 10 )  daysInMonth = 30; 
      if (m == 1 ) {
	daysInMonth = 28;
	int y =  t0.year(true); 
	// make valid for more than 400 years ! 
	if ((y % 4 == 0 && y % 100 != 0 ) || y % 400 == 0 )  
	  daysInMonth = 29;
      }
      diff +=  daysInMonth;
    }
    CPPUNIT_ASSERT(diff == 10);  

    t += TimeSpan (5 * TimeConst::SECS_PER_HOUR, 0);

    seal_test::out << "time + 10d5h = " << t << std::endl
	      << " = " << t.format (true,  " %Y-%m-%d %H:%M:%S (local)  .")
	      << t.nanoformat () << "\n"
	      << " = " << t.format (false, " %Y-%m-%d %H:%M:%S (UTC)   + ")
	      << t.nsecond () << " ns\n";

    // check UTC 
    CPPUNIT_ASSERT_EQUAL( (t0.hour( false ) + 5)%24,  t.hour(false)%24 );
    // to check local time I have to see if t is daylight or not 
    bool daylight0 = t0.isdst(true); 
    bool daylight1 = t.isdst(true); 
    if ( daylight0 == daylight1 ) 
      CPPUNIT_ASSERT_EQUAL( (t0.hour( true ) + 5)%24,  t.hour(true)%24 );
    else { 
      if (daylight1)  
	// I am in spring : in 10 days I have daylight time
	CPPUNIT_ASSERT_EQUAL( (t0.hour( true ) + 5)%24,  t.hour(true)%24 -1 );
      else if (daylight0) 
	// I am in fall in 10 days I have winter time 
	CPPUNIT_ASSERT_EQUAL( (t0.hour( true ) + 5)%24 -1,  t.hour(true)%24  );
    }
     

    // check using predefined times 
    Time t1(2016,2,1,10,0,0,0,true);
    seal_test::out << "Future time1    = " << t1 << std::endl
		   << " = " << t1.format (true,  " %Y-%m-%d %H:%M:%S (local)\n");
    CPPUNIT_ASSERT (!t1.isdst(true));
    // check using predefined times 
    Time t2(2016,5,1,10,0,0,0,true);
    seal_test::out << "Future time2    = " << t2 << std::endl
		   << " = " << t2.format (true,  " %Y-%m-%d %H:%M:%S (local)\n");
    CPPUNIT_ASSERT ( t2.isdst(true));
    CPPUNIT_ASSERT_EQUAL(t1.month(true),t2.month(true)-3); 
    CPPUNIT_ASSERT_EQUAL(t1.hour(false)%24,t2.hour(false)%24+1); 


    return 0;
}
