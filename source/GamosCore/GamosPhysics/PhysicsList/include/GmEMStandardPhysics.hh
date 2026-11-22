#ifndef GmEMStandardPhysics_h
#define GmEMStandardPhysics_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class GmEMStandardPhysicsMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmEMStandardPhysics: public G4VModularPhysicsList
{
public:
  GmEMStandardPhysics();
  virtual ~GmEMStandardPhysics();

  void ConstructParticle();
        
  void ReplacePhysicsList(const G4String& name);
    
  void ConstructProcess();    
  void AddDecay();
  void SelectBremssAngularDist();
    
  void SetCuts();
  //  void SetCutForGamma(G4double);
  //  void SetCutForElectron(G4double);
  //  void SetCutForPositron(G4double);
    
private:
  
  GmEMStandardPhysicsMessenger* pMessenger; 

  G4String emName;
  G4VPhysicsConstructor*  emPhysics;
    
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

