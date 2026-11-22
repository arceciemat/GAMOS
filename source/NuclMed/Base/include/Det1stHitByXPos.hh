#ifndef Det1stHitByXPos_HH
#define Det1stHitByXPos_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <set>
#include <map>
#include <vector>
#include <iostream>
class GmRecHit;

#include "DetV1stHitAlgorithm.hh"

class Det1stHitByXPos : public DetV1stHitAlgorithm
{

public:
  Det1stHitByXPos();

  void Initialize();
  
  virtual ~Det1stHitByXPos(){};

  virtual GmRecHit* Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 );

};

#endif

