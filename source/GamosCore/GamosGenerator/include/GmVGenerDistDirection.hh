#ifndef GmVGenerDistDirection_HH
#define GmVGenerDistDirection_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GmVGenerDist.hh"
class GmParticleSource;

class GmVGenerDistDirection : public GmVGenerDist
{
public:
  GmVGenerDistDirection(){};
  virtual ~GmVGenerDistDirection(){};

  virtual G4ThreeVector GenerateDirection( GmParticleSource* source ) = 0;

  virtual void SetDirection( G4ThreeVector ) {
    G4Exception("GmVGenerDistDirection::SetDirection",
		"",
		FatalException,
		("Direction distribution "+GetName()+" dose not have a SetDirection(G4ThreeVector) method").c_str());
  }
};

#endif
