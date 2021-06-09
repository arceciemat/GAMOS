#ifndef GmPSEMPhysics_h
#define GmPSEMPhysics_h 1

#include "GamosCore/GamosPhysics/PhysicsList/include/GmEMPhysics.hh"
#include "globals.hh"


enum PSType{ UniBS, DirBS, EWPS };

class GmPSEMPhysics: public GmEMPhysics
{
public:
  GmPSEMPhysics();
  virtual ~GmPSEMPhysics();

  virtual void ConstructParticles();  

  virtual G4bool ReplacePhysicsList(const G4String& name);  

protected:
  /*  G4bool electronIsRegistered;
  G4bool positronIsRegistered;
  G4bool gammaIsRegistered;
  G4bool opticalphotonIsRegistered;
*/

  PSType thePSType;
};

#endif
