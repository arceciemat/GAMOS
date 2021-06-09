#include "Det1stHitByXPos.hh"
#include "DetRecHitCone.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "DetVerbosity.hh"
#include "CLHEP/Random/RandFlat.h"


//----------------------------------------------------------------------
Det1stHitByXPos::Det1stHitByXPos()
{
  theName = "XYPos";
  InitVariables();
}

//----------------------------------------------------------------------
GmRecHit* Det1stHitByXPos::Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet&  )
{
  //----- Order the rec hits by absolute X position
  std::set<GmRecHit*>::iterator ites;
  mmdrh recHitMap;
  for( ites = set1.begin(); ites != set1.end(); ites++ ) {
    G4double value = fabs((*ites)->GetPosition().x());
    recHitMap.insert(mmdrh::value_type(value,*ites));
    if( DetVerb(debugVerb) ) G4cout << " GetRecHitByXYRadius add" << value << " " << (*ites) << " " << (*ites)->GetPosition() << G4endl;
  }

  return GetRecHitByX( recHitMap, 0 );

}

