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
class G4VEmProcess;
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

private:
  G4UIcmdWithABool* theApplyCutsCmd;
  G4UIcmdWithoutParameter* theParallelPhysCmd;
  GmUIcmdWithAString* theAddPhysicsCmd;
  GmUIcmdWithAString* theRemoveProcessesByTypeCmd;
  GmUIcmdWithAString* theRemoveProcessesByNameCmd;
  GmUIcmdWithAString* theRemoveProcessesByParticleAndNameCmd;
  GmUIcmdWithAString* theReplaceModelSetCmd;
  GmUIcmdWithAString* theReplaceModelCmd;

};
#endif
  
