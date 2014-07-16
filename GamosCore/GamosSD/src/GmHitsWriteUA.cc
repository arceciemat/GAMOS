#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
 
#include "GmHitsWriteUA.hh"
#include "GmHitsIObinMgr.hh"
#include "GmHitsIOtextMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//---------------------------------------------------------------------
GmHitsWriteUA::GmHitsWriteUA()
{
   bBinFile = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("SD:GmHitsWriteUA:BinFile",1));

   if( bBinFile ) {
     theHitsIOMgr = new GmHitsIObinMgr;
   } else {
     theHitsIOMgr = new GmHitsIOtextMgr;
   }
}

//---------------------------------------------------------------------
void GmHitsWriteUA::EndOfEventAction(const G4Event* evt )
{
  std::vector<GmHit*> hits = GmHitsEventMgr::GetInstance()->GetHitsInEvent( evt->GetEventID() );
  //  G4cout << " GmHitsWriteUA::BeginOfEventAction nhits " << hits.size() << G4endl;
  theHitsIOMgr->WriteAllHits();
}
