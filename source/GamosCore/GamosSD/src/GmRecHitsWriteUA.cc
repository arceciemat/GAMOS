#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
 
#include "GmRecHitsWriteUA.hh"
#include "GmRecHitsIOtextMgr.hh"
#include "GmRecHitsIObinMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//---------------------------------------------------------------------
GmRecHitsWriteUA::GmRecHitsWriteUA()
{
   bBinFile = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("SD:GmRecHitsWriteUA:BinFile",1));

   if( bBinFile ) {
     theRecHitsIOMgr = new GmRecHitsIObinMgr;
   } else {
     theRecHitsIOMgr = new GmRecHitsIOtextMgr;
   }
}

//---------------------------------------------------------------------
void GmRecHitsWriteUA::EndOfEventAction(const G4Event* )
{
  //  std::vector<GmRecHit*> rhits = GmHitsEventMgr::GetInstance()->GetAllRecHits();
  //  G4cout << " GmRecHitsWriteUA::BeginOfEventAction n rechits " << rhits.size() << G4endl;
  theRecHitsIOMgr->WriteAllHits();
}
