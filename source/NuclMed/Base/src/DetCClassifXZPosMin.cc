#include "DetCClassifXZPosMin.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------------
DetCClassifXZPosMin::DetCClassifXZPosMin()
{
  theName = "XZPosMin";
  InitVariables();
}

//----------------------------------------------------------------------
DetCClassifXZPosMin::~DetCClassifXZPosMin()
{
}
//----------------------------------------------------------------------
void DetCClassifXZPosMin::BookSpecificHistos(G4int histoNumber)
{
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  G4String hnam = "DetCompton" + sepa + "Classif" + sepa + " RecHit " + theName;
  theAnaMgr->CreateHisto1D(hnam,300,theMin,theMax,histoNumber+80); 
  hnam =  "DetCompton" + sepa + "Classif" + sepa + " RecHit XZPos";
  theAnaMgr->CreateHisto1D(hnam,300,theMin,theMax,histoNumber+81);
  
}


//----------------------------------------------------------------------
int64_t DetCClassifXZPosMin::GetClassification( DetRecHitSet& prhs, G4int nh )
{
  DetRecHitSet::iterator iterhs;
  std::set<GmRecHit*>::iterator iterh;

  // Get minimum energy
  G4double minPos = DBL_MAX;
  for( iterhs = prhs.begin(); iterhs != prhs.end(); iterhs++ ){
    G4ThreeVector pos = (*iterhs)->GetPosition();
    G4double val = sqrt(sqr(pos.x())+sqr(pos.z()));
    if( val< minPos ) minPos = val;
    if( nh != -1 ) {
      theAnaMgr->GetHisto1(nh + 81 )->Fill(val);
    }
  }
  if( nh != -1 ) {
    theAnaMgr->GetHisto1(nh + 80 )->Fill(minPos);
  }

  return GetIndexFromValue( minPos );
}
