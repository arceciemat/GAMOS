#include "RTProdCutsStudyUA.hh"
#include "RTVerbosity.hh"

#include "RTCutsStudyFilter.hh"

//---------------------------------------------------------------------
RTProdCutsStudyUA::RTProdCutsStudyUA()
{
  BuildFilter();
}

//---------------------------------------------------------------------
void RTProdCutsStudyUA::BuildFilter()
{
  theFilters.push_back( new RTCutsStudyFilter("RTCutsStudyFilter") );
}
