#include "DetCClassifEnergyMax.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------------
DetCClassifEnergyMax::DetCClassifEnergyMax()
{
  theName = "EnergyMax";
  InitVariables();
}

//----------------------------------------------------------------------
DetCClassifEnergyMax::~DetCClassifEnergyMax()
{
}

//----------------------------------------------------------------------
void DetCClassifEnergyMax::BookSpecificHistos(G4int histoNumber)
{
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  G4String hnam = "DetCompton"+sepa+"Classif"+sepa+" RecHit " + theName;
  theAnaMgr->CreateHisto1D(hnam,300,theMin,theMax,histoNumber+80);
  hnam =  "DetCompton"+sepa+"Classif"+sepa+" RecHit Energy";
  theAnaMgr->CreateHisto1D(hnam,300,theMin,theMax,histoNumber+81);
  
}

//----------------------------------------------------------------------
int64_t DetCClassifEnergyMax::GetClassification( DetRecHitSet& prhs, G4int nh )
{
  DetRecHitSet::iterator iterhs;
  std::set<GmRecHit*>::iterator iterh;

  // Get maximum energy
  G4double maxEner = -DBL_MAX;
  for( iterhs = prhs.begin(); iterhs != prhs.end(); iterhs++ ){
    G4double val = (*iterhs)->GetEnergy();
    if( nh != -1 ) {
      theAnaMgr->GetHisto1(nh + 81 )->Fill(val);
    }
    if( val > maxEner ) maxEner = val;
  }
  if( nh != -1 ) {
    theAnaMgr->GetHisto1(nh + 80 )->Fill(maxEner);
  }

  return GetIndexFromValue( maxEner );

}

