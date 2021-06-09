#include "DetCAlgoXPos.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosAnalysis/include/GmTrajStep.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"

//----------------------------------------------------------------------
DetCAlgoXPos::DetCAlgoXPos()
{
  theName = "XPos";
  InitVariables();
}


//----------------------------------------------------------------------
void DetCAlgoXPos::FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh )
{
  std::vector<G4double> values;
  mmits::iterator itemm;
  for( itemm = itemmp.first; itemm != itemmp.second; itemm++ ){
    values.push_back( fabs((*itemm).second->GetPositionPost().x()) );
  }

  FillHistosFromValues( values, nh+0 );

}


//----------------------------------------------------------------------
void DetCAlgoXPos::FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh )
{
  std::vector<G4double> values;
  
  std::vector<std::pair<GmTrajStep*,GmRecHit*> >::const_iterator iterho;
  for( iterho = recHitOrdered.begin(); iterho != recHitOrdered.end(); iterho++ ){
    //      GmTrajStep* ts = (*iterho).first;
    GmRecHit* rh = (*iterho).second;
    values.push_back( fabs(rh->GetPosition().x()) );
  }
  
  FillHistosFromValues( values, nh+100 );
}

