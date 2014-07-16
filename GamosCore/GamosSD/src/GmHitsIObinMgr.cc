#include "GmHitsIObinMgr.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"

//-----------------------------------------------------------------------
GmHitsIObinMgr::GmHitsIObinMgr()
{ 
  theOutputType = "hits.bin";
  Init();

  OpenFileOut();

}

//-----------------------------------------------------------------------
void GmHitsIObinMgr::WriteHit(GmHit* hit)
{
  if( bDebugOut ) hit->Print( G4cout, true );
  hit->Print( theFileOut, bDebugOut );

}

//-----------------------------------------------------------------------
GmHitsIObinMgr::~GmHitsIObinMgr()
{
  CloseFileIn();
  CloseFileOut();
}

