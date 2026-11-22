#ifndef DetV1stHitAlgorithm_HH
#define DetV1stHitAlgorithm_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <set>
#include <map>
#include "DetRecHitSet.hh" 

typedef std::multimap<G4double,GmRecHit*> mmdrh;

class DetV1stHitAlgorithm
{

public:
  DetV1stHitAlgorithm();
  virtual ~DetV1stHitAlgorithm(){};

  virtual GmRecHit* Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 ) = 0;

protected:
  void InitVariables();

protected:
  GmRecHit* GetRecHitByX( mmdrh& recHitMap, G4bool startByLast );

  G4String theName;
  G4int theSelectionOrder;

};

#endif

