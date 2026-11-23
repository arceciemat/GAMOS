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
    GmAnalysisMgr* anaMgr = GmAnalysisMgr::GetInstance("");
    std::map<G4String, GmAnalysisMgr*> anaMgrs = anaMgr->GetAllInstances();
    std::map<G4String,GmAnalysisMgr*>::const_iterator itea;
    for( itea = anaMgrs.begin(); itea != anaMgrs.end(); itea++ ){
      GmAnalysisMgr* anaMgr2 = (*itea).second;
      anaMgr2->SaveAllFormats( anaMgr2->GetFileName());
    }
  }
}
