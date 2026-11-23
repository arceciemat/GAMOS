#include "GmRecHitsIObinMgr.hh"
#include "GmHitsEventMgr.hh"
#include "GmSDVerbosity.hh"
#include "GmEventTimeExtractorFromHit.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"

//-----------------------------------------------------------------------
GmRecHitsIObinMgr::GmRecHitsIObinMgr()
{ 
  theOutputType = "rechits";
  Init();
  OpenFileOut();
}

//-----------------------------------------------------------------------
void GmRecHitsIObinMgr::WriteHit(GmRecHit* rhit)
{ 
  if( bDebugOut ) rhit->Print( G4cout, bDebugOut );
  rhit->Print( theFileOut, bDebugOut );
}

//-----------------------------------------------------------------------
void GmRecHitsIObinMgr::ReadHits()
{
}

//-----------------------------------------------------------------------
GmRecHitsIObinMgr::~GmRecHitsIObinMgr()
{
  CloseFileIn();
  CloseFileOut();
}

