#include "Det1stHitByXYZPos.hh"
#include "DetRecHitCone.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "DetVerbosity.hh"
#include "CLHEP/Random/RandFlat.h"


//----------------------------------------------------------------------
Det1stHitByXYZPos::Det1stHitByXYZPos()
{
  theName = "XYZPos";
  InitVariables();
}

//----------------------------------------------------------------------
GmRecHit* Det1stHitByXYZPos::Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet&  )
{
  //----- Order the rec hits by absolute X position
  std::set<GmRecHit*>::iterator ites;
  mmdrh recHitMap;
  for( ites = set1.begin(); ites != set1.end(); ites++ ) {
    G4double value = (*ites)->GetPosition().mag();
    recHitMap.insert(mmdrh::value_type(value,*ites));
    if( DetVerb(debugVerb) ) G4cout << " GetRecHitByXYZRadius add" << value << " " << (*ites) << " " << (*ites)->GetPosition() << G4endl;
  }

  return GetRecHitByX( recHitMap, 0 );

}

