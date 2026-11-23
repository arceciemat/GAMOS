#include "DetCClassifEnergyAverage.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------------
DetCClassifEnergyAverage::DetCClassifEnergyAverage()
{
  theName = "EnergyAverage";
  InitVariables();
}

//----------------------------------------------------------------------
DetCClassifEnergyAverage::~DetCClassifEnergyAverage()
{
}

//----------------------------------------------------------------------
void DetCClassifEnergyAverage::BookSpecificHistos(G4int histoNumber)
{
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  G4String hnam = "DetCompton"+sepa+"Classif"+sepa+" RecHit " + theName;
  theAnaMgr->CreateHisto1D(hnam,300,theMin,theMax,histoNumber+80);
  hnam =  "DetCompton"+sepa+"Classif"+sepa+" RecHit Energy";
  theAnaMgr->CreateHisto1D(hnam,300,theMin,theMax,histoNumber+81);
  
}

//----------------------------------------------------------------------
int64_t DetCClassifEnergyAverage::GetClassification( DetRecHitSet& prhs, G4int nh )
{
  DetRecHitSet::iterator iterhs;
  std::set<GmRecHit*>::iterator iterh;

  // Get average energy energy
  G4double sumEner = 0.;
  G4int ii = 0;
  for( iterhs = prhs.begin(); iterhs != prhs.end(); iterhs++, ii++ ){
    G4double val = (*iterhs)->GetEnergy();
    if( nh != -1 ) {
      theAnaMgr->GetHisto1(nh + 81 )->Fill(val);
    }
    sumEner += val;
  }

  G4double averEner = sumEner / ii;
  if( nh != -1 ) {
    theAnaMgr->GetHisto1(nh + 80 )->Fill(averEner);
  }

  return GetIndexFromValue( averEner );

}

