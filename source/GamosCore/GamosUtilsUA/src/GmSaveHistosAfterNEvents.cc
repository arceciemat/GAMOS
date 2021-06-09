#include "G4Run.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4ios.hh"

#include "GmSaveHistosAfterNEvents.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

//----------------------------------------------------------------
GmSaveHistosAfterNEvents::GmSaveHistosAfterNEvents()
{
}

//----------------------------------------------------------------
void GmSaveHistosAfterNEvents::BeginOfRunAction( const G4Run* )
{

  theFirstEvent = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":FirstEvent", 1));
  theEachNEvent = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":EachNEvent", 1000));

}

//----------------------------------------------------------------
void GmSaveHistosAfterNEvents::EndOfEventAction( const G4Event* event )
{

  G4int evtid = event->GetEventID();
  if( evtid >= theFirstEvent && evtid%theEachNEvent == 0 ) {
    std::map<G4String,GmAnalysisMgr*> anaMgrs = GmAnalysisMgr::GetInstances();
    std::map<G4String,GmAnalysisMgr*>::const_iterator itea;
    std::set<G4String> fileFormats = GmAnalysisMgr::GetFileFormats();
    std::set<G4String>::const_iterator ites;
    for( itea = anaMgrs.begin(); itea != anaMgrs.end(); itea++ ){
      GmAnalysisMgr* anaMgr = (*itea).second;
      for( ites = fileFormats.begin(); ites != fileFormats.end(); ites++) {
	anaMgr->Save( anaMgr->GetFileName(), *ites );
      }
    }
  }
}
