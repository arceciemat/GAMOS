#ifndef GmEmDNAPhysics_h
#define GmEmDNAPhysics_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmEmDNAPhysics: public G4VModularPhysicsList
{
  public:
    GmEmDNAPhysics();
   ~GmEmDNAPhysics();

    void ConstructParticle();
    void ConstructProcess();
    G4bool ReplacePhysicsList(const G4String& name);

    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);

  
  private:
    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;
    G4double currentDefaultCut;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

