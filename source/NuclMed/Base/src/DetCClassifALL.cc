#include "DetCClassifALL.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

//----------------------------------------------------------------------
DetCClassifALL::DetCClassifALL()
{
  theName = "ALL";
  theIntervalLimits.insert(0);
}

//----------------------------------------------------------------------
DetCClassifALL::~DetCClassifALL()
{
}

//----------------------------------------------------------------------
void DetCClassifALL::BookSpecificHistos(G4int )
{
 

}

//----------------------------------------------------------------------
int64_t DetCClassifALL::GetClassification( DetRecHitSet& , G4int  )
{
  return 0;

}
