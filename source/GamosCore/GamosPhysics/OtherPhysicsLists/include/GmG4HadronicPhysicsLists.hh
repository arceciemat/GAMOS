/////////////////////////////////////////////////////////////////////////
//
// GmG4HadronicPhysicsLists
//
////////////////////////////////////////////////////////////////////////
// 

#ifndef GmG4HadronicPhysicsLists_h
#define GmG4HadronicPhysicsLists_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class GmG4HadronicPhysicsListsMessenger;

//....oooOO0Oooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmG4HadronicPhysicsLists: public G4VModularPhysicsList
{
public:

  GmG4HadronicPhysicsLists();
  virtual ~GmG4HadronicPhysicsLists();

  virtual void ConstructParticle();
  virtual void ConstructProcess();    

  void ReplacePhysicsList(const G4String& name);
  void List();
  
private:

  void SetBuilderList0(G4bool flagHP = false);
  void SetBuilderList1(G4bool flagHP = false);
  void SetBuilderList2();

  G4VPhysicsConstructor*  fEmPhysicsList;
  G4VPhysicsConstructor*  fParticleList;
  std::vector<G4VPhysicsConstructor*>  fHadronPhys;
    
  GmG4HadronicPhysicsListsMessenger* fMessenger;

  G4bool bPhysicsReplaced;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

