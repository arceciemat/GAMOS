#ifndef GmExtraPhysicsMessenger_HH
#define GmExtraPhysicsMessenger_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

#include <map>
#include <vector>

class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;
class GmUIcmdWithAString;
class G4LivermorePhotoElectricModel;
class G4LivermoreIonisationModel;
class G4VProcess;
class G4VEmProcess;
class G4VEmModel;
class G4EmModelManager;
class G4VEnergyLossProcess;

typedef std::multimap<G4String,G4String> mmss;

class GmExtraPhysicsMessenger: public G4UImessenger {

public:
  
  GmExtraPhysicsMessenger();
  
  ~GmExtraPhysicsMessenger();
  
  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  void AddPhysics(G4String newValue);
  void RemoveProcessesByType(G4String newValue);
  void RemoveProcessesByName(G4String newValue);
  void RemoveProcessesByParticleAndName(G4String newValue);
  void ReplaceModelSet(G4String newValue);  
  void ReplaceModel(G4String newValue);
  void AddDeexcitation();
  void Replace1Model(G4String newParticleName, G4String newProcessName, G4String newModelName);

  std::pair<G4VEmModel*,G4VEmModel*> ChangeMsc(G4String newParticleName, G4String newProcessName, G4String newModelName);
  std::pair<G4VEmModel*,G4VEmModel*> ChangeIoni(G4String newParticleName, G4String newProcessName, G4String newModelName);
  std::pair<G4VEmModel*,G4VEmModel*> ChangeBrem(G4VEnergyLossProcess* process, G4String newParticleName, G4String newProcessName, G4String newModelName);
  void ReplaceG4Model(G4VProcess* process, G4VEmModel* newModel, G4VEmModel* newModel2 );
    
private:
  G4UIcmdWithABool* theApplyCutsCmd;
  G4UIcmdWithoutParameter* theParallelPhysCmd;
  GmUIcmdWithAString* theAddPhysicsCmd;
  GmUIcmdWithAString* theRemoveProcessesCmd;
  GmUIcmdWithAString* theRemoveProcessesByNameCmd;
  GmUIcmdWithAString* theRemoveProcessesByTypeCmd;
  GmUIcmdWithAString* theRemoveProcessesByParticleCmd;
  GmUIcmdWithAString* theRemoveProcessesByParticleAndNameCmd;
  GmUIcmdWithAString* theReplaceModelSetCmd;
  GmUIcmdWithAString* theReplaceModelCmd;

};
#endif
  
