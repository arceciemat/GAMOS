#include "RTMinRangeLimitsStudyUA.hh"
#include "RTVerbosity.hh"

#include "RTCutsStudyFilter.hh"

//---------------------------------------------------------------------
RTMinRangeLimitsStudyUA::RTMinRangeLimitsStudyUA()
{
  BuildFilter();
}

//---------------------------------------------------------------------
void RTMinRangeLimitsStudyUA::BuildFilter()
{
  theFilters.push_back( new RTCutsStudyFilter("RTCutsStudyFilter") );
}
