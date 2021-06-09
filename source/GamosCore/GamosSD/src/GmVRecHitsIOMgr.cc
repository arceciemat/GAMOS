#include "GmRecHitsIOtextMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GmSDVerbosity.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserAction.hh"

//-----------------------------------------------------------------------
GmVRecHitsIOMgr::GmVRecHitsIOMgr()
{ 
  theHitsEventMgr = GmHitsEventMgr::GetInstance(); // store a copy for optimisation
}

//-----------------------------------------------------------------------
void GmVRecHitsIOMgr::WriteAllHits()
{
  std::vector<GmRecHit*> rhits = theHitsEventMgr->GetAllRecHits();
 
  std::vector<GmRecHit*>::const_iterator iteh;
  for( iteh = rhits.begin(); iteh != rhits.end(); iteh++ ){
    WriteHit( *iteh );
  }
  
}


