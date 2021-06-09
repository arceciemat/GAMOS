#include "DetCClassifYPosMin.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------------
DetCClassifYPosMin::DetCClassifYPosMin()
{
  theName = "YPosMin";
  InitVariables();
}

//----------------------------------------------------------------------
DetCClassifYPosMin::~DetCClassifYPosMin()
{
}
//----------------------------------------------------------------------
void DetCClassifYPosMin::BookSpecificHistos(G4int histoNumber)
{
  G4String separator = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");

  G4String hnam = "DetCompton" + separator + "Classif" + separator + " RecHit " + theName;
  theAnaMgr->CreateHisto1D(hnam,300,theMin,theMax,histoNumber+80);
  hnam =  "DetCompton:Classif: RecHit YPos";
  theAnaMgr->CreateHisto1D(hnam,300,theMin,theMax,histoNumber+81);
  
}


//----------------------------------------------------------------------
int64_t DetCClassifYPosMin::GetClassification( DetRecHitSet& prhs, G4int nh )
{
  DetRecHitSet::iterator iterhs;
  std::set<GmRecHit*>::iterator iterh;

  // Get minimum energy
  G4double minPos = DBL_MAX;
  for( iterhs = prhs.begin(); iterhs != prhs.end(); iterhs++ ){
    G4double val = fabs((*iterhs)->GetPosition().y());
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
