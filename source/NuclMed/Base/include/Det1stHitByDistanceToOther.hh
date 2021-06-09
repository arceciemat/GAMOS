#ifndef Det1stHitByDistanceToOther_HH
#define Det1stHitByDistanceToOther_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <set>
#include <map>
#include <vector>
#include <iostream>
class GmRecHit;

#include "DetV1stHitAlgorithm.hh"

class Det1stHitByDistanceToOther : public DetV1stHitAlgorithm
{

public:
  Det1stHitByDistanceToOther();

  void Initialize();
  
  virtual ~Det1stHitByDistanceToOther(){};

  virtual GmRecHit* Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 );

};

#endif

