#ifndef GmVEmDNAPhysics_h
#define GmVEmDNAPhysics_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmVEmDNAPhysics: public G4VModularPhysicsList
{
  public:
    GmVEmDNAPhysics();
   ~GmVEmDNAPhysics();

    void ConstructParticle();
    void ConstructProcess();
    G4bool ReplacePhysicsList(const G4String& name);

    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);

  void BuildDNAPhysics();
  virtual void BuildDNAChemistry() = 0; 

protected:
    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;
    G4double currentDefaultCut;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

