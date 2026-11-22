#ifndef GmEmDNAPhysics_h
#define GmEmDNAPhysics_h 1

#include "GmVEmDNAPhysics.hh"

class G4VPhysicsConstructor;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmEmDNAPhysics: public GmVEmDNAPhysics
{
  public:
    GmEmDNAPhysics();
   ~GmEmDNAPhysics();

  virtual void BuildDNAChemistry();
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

