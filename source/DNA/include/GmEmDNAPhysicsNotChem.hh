#ifndef GmEmDNAPhysicsNotChem_h
#define GmEmDNAPhysicsNotChem_h 1

#include "GmVEmDNAPhysics.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmEmDNAPhysicsNotChem: public GmVEmDNAPhysics
{
  public:
    GmEmDNAPhysicsNotChem();
   ~GmEmDNAPhysicsNotChem();

  virtual void BuildDNAChemistry();

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

