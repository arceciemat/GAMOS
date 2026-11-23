#include "Det1stHitByDistanceToOther.hh"
#include "DetRecHitCone.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "DetVerbosity.hh"
#include "CLHEP/Random/RandFlat.h"


//----------------------------------------------------------------------
Det1stHitByDistanceToOther::Det1stHitByDistanceToOther()
{
  theName = "DistanceToOther";
  InitVariables();
}

//----------------------------------------------------------------------
GmRecHit* Det1stHitByDistanceToOther::Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 )
{
  //----- Try all possible combinations of distances between pair of hits
  std::set<GmRecHit*>::iterator ites1,ites2, selectItes;
  selectItes = set1.begin(); // just in case no one is found
  G4double minDist = 9.E99;
  for( ites1 = set1.begin(); ites1 != set1.end(); ites1++ ) {
    //--- Distance to rec hit of second set
    if( theSelectionOrder != 0 ) {
      G4double dist = ((*ites1)->GetPosition()-set2.GetPosition()).mag();
      //	G4cout << " check ites2 dist " << dist << G4endl;
      if(dist < minDist ){
	if( DetVerb(debugVerb) ) G4cout << " Det1stHitByXYPos::GetRecHitComptonCone minDist " << minDist << " new min dist " << dist << (*ites1)->GetPosition() << G4endl;
	minDist = dist;
	selectItes = ites1;
      }
    } else {
      //	G4cout << " check ites " << set2.size() << G4endl;
      for( ites2 = set2.begin(); ites2 != set2.end(); ites2++ ) {
	G4double dist = ((*ites1)->GetPosition()-(*ites2)->GetPosition()).mag();
	//  G4cout << " check ites2 " << dist << G4endl;
	if(dist < minDist ){
	  minDist = dist;
	  selectItes = ites1;
	}
      }
    }
  }

  return *selectItes;
}

