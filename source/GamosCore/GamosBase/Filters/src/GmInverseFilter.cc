#include "GmInverseFilter.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterMgr.hh"

//----------------------------------------------------------------
GmInverseFilter::GmInverseFilter(G4String name)
  :GmVFilter(name)
{
}

//----------------------------------------------------------------
GmInverseFilter::~GmInverseFilter()
{ 
}

//----------------------------------------------------------------
G4bool GmInverseFilter::AcceptTrack(const G4Track* aTrack)
{
  G4bool bAccept = !(theFilters->AcceptTrack(aTrack));
  
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmInverseFilter::AcceptTrack " << bAccept << G4endl;
#endif
  
  return bAccept;
}


//----------------------------------------------------------------
G4bool GmInverseFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bAccept = !(theFilters->AcceptStep(aStep));
  
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmInverseFilter::AcceptStep " << bAccept << G4endl;
#endif

  return bAccept;
}

//----------------------------------------------------------------
G4bool GmInverseFilter::AcceptStackedTrack(const G4Track* aTrack)
{
  G4bool bAccept = !(theFilters->AcceptStackedTrack(aTrack));
  
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmInverseFilter::AcceptStackedTrack " << bAccept << G4endl;
#endif
  
  return bAccept;
}

//----------------------------------------------------------------
void GmInverseFilter::show()
{
  G4cout << "GmInverseFilter" << G4endl;
  theFilters->show();
}

//----------------------------------------------------------------
void GmInverseFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmVFilterOfFilters::SetParameters","There should be at one parameter: FILTER TO INVERSE",FatalErrorInArgument,G4String("Parameters are:"+parastr).c_str());
  }

  GmFilterMgr* filterMgr = GmFilterMgr::GetInstance();
  std::vector<G4String> paramsF; 
  paramsF.push_back( params[0] );
  paramsF.push_back( params[0] );
  theFilters = filterMgr->FindOrBuildFilter(paramsF, true);
}

