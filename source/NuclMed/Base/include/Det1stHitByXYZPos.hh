#ifndef Det1stHitByXYZPos_HH
#define Det1stHitByXYZPos_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <set>
#include <map>
#include <vector>
#include <iostream>
class GmRecHit;

#include "DetV1stHitAlgorithm.hh"

class Det1stHitByXYZPos : public DetV1stHitAlgorithm
{

public:
  Det1stHitByXYZPos();

  void Initialize();
  
  virtual ~Det1stHitByXYZPos(){};

  virtual GmRecHit* Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 );

};

#endif

