#include "Det1stHitByEnergy.hh"
#include "DetRecHitCone.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "DetVerbosity.hh"
#include "CLHEP/Random/RandFlat.h"


//----------------------------------------------------------------------
Det1stHitByEnergy::Det1stHitByEnergy()
{
  theName = "Energy";
  InitVariables();
}

//----------------------------------------------------------------------
GmRecHit* Det1stHitByEnergy::Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& )
{
  //----- Order the rec hits by energy
  std::set<GmRecHit*>::iterator ites;
  mmdrh recHitMap;
  for( ites = set1.begin(); ites != set1.end(); ites++ ) {
    recHitMap.insert(mmdrh::value_type((*ites)->GetEnergy(),*ites));
  }

  return GetRecHitByX( recHitMap, 1 );
}

