#ifndef Det1stHitByEnergy_HH
#define Det1stHitByEnergy_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <set>
#include <map>
#include <vector>
#include <iostream>
class GmRecHit;

#include "DetV1stHitAlgorithm.hh"

class Det1stHitByEnergy : public DetV1stHitAlgorithm
{

public:
  Det1stHitByEnergy();

  void Initialize();
  
  virtual ~Det1stHitByEnergy(){};

  virtual GmRecHit* Get1stRecHit( const DetRecHitSet& set1, const DetRecHitSet& set2 );

};

#endif

