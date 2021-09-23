#include "SealTest/SealTest.h" 
#include "SealBase/TimeInfo.h"
#include "SealBase/LongLong.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace seal;
int seal_test::TimeInfo02 (int, char **argv)
{
    TimeInfo::NanoTicks rstartc = 0;
    TimeInfo::NanoSecs rstartt = 0;
    TimeInfo::init ();    
    if (TimeInfo::features () & TimeInfo::FEATURE_REAL_COUNT_EPOCH)
    {
	rstartc = TimeInfo::realCycles ();
	rstartt = TimeInfo::realNsecs ();
    }

    Signal::handleFatal (argv[0]);

    seal_test::out << "processor information\n"
	      << "  MHz         : " << TimeInfo::mhz () << "\n\n";

    CPPUNIT_ASSERT (TimeInfo::mhz () > 100 && TimeInfo::ghz () < 100); 

    // Get run duration so far.  Virtual cycles may appear to be
    // larger than the real cycles: if FEATURE_REAL_COUNT_EPOCH was
    // present, the substraction below removes proces start-up costs.  
    TimeInfo::NanoTicks virtc = TimeInfo::virtualCycles ();
    TimeInfo::NanoSecs virtt = TimeInfo::virtualNsecs ();
    TimeInfo::NanoTicks realc = TimeInfo::realCycles ();
    TimeInfo::NanoSecs realt = TimeInfo::realNsecs ();
    
    seal_test::out << "------------------------------------------------\n"
	      << "Start    " << rstartc << " ticks, " << rstartt*1e-9 << " secs\n" 
	      << "Real     " << (realc-rstartc) << " ticks, "
	      << (realt-rstartt)*1e-9 << " secs\n"
	      << "Virtual  " << virtc << " ticks, " << virtt*1e-9 << " secs\n"
	      << "Process  " << TimeInfo::processUserTime () * 1e-9 << " user, "
	      << TimeInfo::processSystemTime () * 1e-9 << " system, "
	      << TimeInfo::processRealTime () * 1e-9 << " real ("
	      << TimeInfo::processIdleTime () * 1e-9 << " idle)\n"
	      << "System   " << std::setprecision(9) << TimeInfo::time () * 1e-9
	      << " since epoch, "
	      << TimeInfo::elapsedTime () * 1e-9 << " elapsed; resolution: "
	      << TimeInfo::resolution () * 1e-9 << std::endl;

    seal_test::out << "Features";

    if (TimeInfo::features () & TimeInfo::FEATURE_EXACT_MHZ)
	seal_test::out << " EXACT_MHZ";
    if (TimeInfo::features () & TimeInfo::FEATURE_TIME_EPOCH)
	seal_test::out << " TIME_EPOCH";
    if (TimeInfo::features () & TimeInfo::FEATURE_REAL_COUNT_EPOCH)
	seal_test::out << " REAL_COUNT_EPOCH";
    if (TimeInfo::features () & TimeInfo::FEATURE_PROCESS_TIMES)
	seal_test::out << " PROCESS_TIMES";

    seal_test::out << std::endl << std::endl;
    return 0;
}
