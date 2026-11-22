#include "DetCClassifNRecHit.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------------
DetCClassifNRecHit::DetCClassifNRecHit()
{
  theName = "NRecHit";
  InitVariables();
}

//----------------------------------------------------------------------
DetCClassifNRecHit::~DetCClassifNRecHit()
{
}

//----------------------------------------------------------------------
void DetCClassifNRecHit::BookSpecificHistos(G4int histoNumber)
{

  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  G4String hnam = "DetCompton"+sepa+"Classif"+sepa+" RecHit " + theName;
  theAnaMgr->CreateHisto1D(hnam,50,0,50,histoNumber+80);
  
}

//----------------------------------------------------------------------
int64_t DetCClassifNRecHit::GetClassification( DetRecHitSet& prhs, G4int nh )
{
  DetRecHitSet::iterator iterhs;
  std::set<GmRecHit*>::iterator iterh;

  // Get minimum energy
  G4double nHits = G4double( prhs.size() );
  if( nh != -1 ) {
    theAnaMgr->GetHisto1(nh + 80 )->Fill(nHits);
  }

  //  G4cout << theName << " GetClassification " << nHits << G4endl;
  return GetIndexFromValue( nHits );

}


