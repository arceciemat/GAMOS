#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
 
#include "GmHitsReadUA.hh"
#include "GmHitsIOtextMgr.hh"
#include "GmHitsIObinMgr.hh"

//---------------------------------------------------------------------
GmHitsReadUA::GmHitsReadUA()
{
  theHitsIOMgr = new GmHitsIOtextMgr;
  theHitsIOMgr->OpenFileIn();
}

//---------------------------------------------------------------------
void GmHitsReadUA::BeginOfEventAction(const G4Event* )
{
  theHitsIOMgr->ReadHits();
}
