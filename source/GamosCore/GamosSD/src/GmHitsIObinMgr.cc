#include "GmHitsIObinMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"

//-----------------------------------------------------------------------
GmHitsIObinMgr::GmHitsIObinMgr()
{ 
  theOutputType = "hits.bin";
  Init();

  //  OpenFileOut();

}

//-----------------------------------------------------------------------
void GmHitsIObinMgr::WriteHit(GmHit* hit)
{
  G4String sdtype =  "/" + GmParameterMgr::GetInstance()->GetStringValue("GmHitsIObinMgr:SDType","") + "/";
  if( sdtype != "//" && sdtype != hit->GetSDType() )  return;
  if( bDebugOut ) hit->Print( G4cout, true );
  hit->Print( theFileOut, bDebugOut );

}

//-----------------------------------------------------------------------
GmHitsIObinMgr::~GmHitsIObinMgr()
{
  CloseFileIn();
  CloseFileOut();
}

