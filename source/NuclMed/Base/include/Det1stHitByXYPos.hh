#ifndef Det1stHitByXYPos_HH
#define Det1stHitByXYPos_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <set>
#include <map>
#include <vector>
#include <iostream>
class GmRecHit;

#include "DetV1stHitAlgorithm.hh"

class Det1stHitByXYPos : public DetV1stHitAlgorithm
{

public:
  Det1stHitByXYPos();

  void Initialize();
  
  virtual ~Det1stHitByXYPos(){};

  virtual GmRecHit* Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 );

};

#endif

