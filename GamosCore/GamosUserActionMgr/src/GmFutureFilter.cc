#include "GmFutureFilter.hh"
#include "GmStepMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//------------------------------------------------------------------------
GmFutureFilter::GmFutureFilter(G4String name)
: GmVFilter(name)
{
  bFutureFilter = TRUE;
  bFilterWithChildren = FALSE;
  GmStepMgr::GetInstance(); // to create GmFutureFilterUA
}

//------------------------------------------------------------------------
GmFutureFilter::~GmFutureFilter()
{;}

//------------------------------------------------------------------------
void GmFutureFilter::SetParameters( std::vector<G4String>& params )
{
  if( params.size() != 2 ) {
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmFutureFilter::SetParameters",
		"There should be two parameters: past_filter future_filter",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  GmFilterMgr* filterMgr = GmFilterMgr::GetInstance();
  std::vector<G4String> filterParams;
  filterParams.push_back( params[0] );
  filterParams.push_back( params[0] );
  thePastFilter = filterMgr->FindOrBuildFilter( filterParams, TRUE );

  filterParams.clear();
  filterParams.push_back( params[1] );
  filterParams.push_back( params[1] );
  theFutureFilter = filterMgr->FindOrBuildFilter( filterParams, TRUE );

}

//------------------------------------------------------------------------
G4bool GmFutureFilter::AcceptFutureStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) {
    G4cout << "GmFutureFilter::AcceptFutureStep " << G4endl;
  }
#endif
  return theFutureFilter->AcceptStep( aStep );
}

//------------------------------------------------------------------------
G4bool GmFutureFilter::AcceptPastStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) {
    G4cout << "GmFutureFilter::AcceptPastStep " << G4endl;
  }
#endif
  return thePastFilter->AcceptStep( aStep );
}

