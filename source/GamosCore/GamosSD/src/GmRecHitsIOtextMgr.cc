#include "GmRecHitsIOtextMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GmSDVerbosity.hh"
#include "GmEventTimeExtractorFromHit.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"

//-----------------------------------------------------------------------
GmRecHitsIOtextMgr::GmRecHitsIOtextMgr()
{ 
  theOutputType = "rechits";
  Init();
  OpenFileOut();
}

//-----------------------------------------------------------------------
void GmRecHitsIOtextMgr::WriteHit(GmRecHit* rhit)
{ 
  if( bDebugOut ) rhit->Print( G4cout, bDebugOut );
  rhit->Print( *theFileOut, bDebugOut );
}


//-----------------------------------------------------------------------
void GmRecHitsIOtextMgr::ReadHits()
{
}

//-----------------------------------------------------------------------
GmRecHitsIOtextMgr::~GmRecHitsIOtextMgr()
{
  CloseFileIn();
  CloseFileOut();
}

