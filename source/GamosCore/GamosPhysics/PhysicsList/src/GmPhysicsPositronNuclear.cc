#include "GmPhysicsPositronNuclear.hh"
#include "G4ProcessManager.hh"
#include "G4Positron.hh"
#include "G4PositronNuclearProcess.hh"
#include "G4ElectroVDNuclearModel.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

GmPhysicsPositronNuclear::GmPhysicsPositronNuclear(const G4String& name,G4int type): G4VPhysicsConstructor(name,type)
{ }

GmPhysicsPositronNuclear::~GmPhysicsPositronNuclear()
{ }

void GmPhysicsPositronNuclear::ConstructProcess()
{
  G4ElectroVDNuclearModel * electronReaction = new G4ElectroVDNuclearModel();

  G4ProcessManager * pmanager = G4Positron::PositronDefinition()->GetProcessManager();
  if( GmG4Utils::CheckProcessExists( pmanager, "PositronNuclear" )) return;
  G4PositronNuclearProcess * positronNuclearProcess(new G4PositronNuclearProcess);
  positronNuclearProcess->RegisterMe(electronReaction);
  pmanager->AddDiscreteProcess(positronNuclearProcess);
  
}
