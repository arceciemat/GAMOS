#ifndef Det1stHitByComptonCone_HH
#define Det1stHitByComptonCone_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <set>
#include <map>
#include <vector>
#include <iostream>
class GmRecHit;

#include "DetV1stHitAlgorithm.hh"

class Det1stHitByComptonCone : public DetV1stHitAlgorithm
{

public:
  Det1stHitByComptonCone();

  void Initialize();
  
  virtual ~Det1stHitByComptonCone(){};

  virtual GmRecHit* Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 );

};

#endif

