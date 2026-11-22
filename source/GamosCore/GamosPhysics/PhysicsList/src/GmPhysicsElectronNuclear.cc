#include "GmPhysicsElectronNuclear.hh"
#include "G4ProcessManager.hh"
#include "G4Electron.hh"
#include "G4ElectronNuclearProcess.hh"
#include "G4ElectroVDNuclearModel.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

GmPhysicsElectronNuclear::GmPhysicsElectronNuclear(const G4String& name, G4int type): G4VPhysicsConstructor(name, type)
{ }

GmPhysicsElectronNuclear::~GmPhysicsElectronNuclear()
{ }

void GmPhysicsElectronNuclear::ConstructProcess()
{
  G4ElectroVDNuclearModel * electronReaction = new G4ElectroVDNuclearModel();
  
  G4ProcessManager * pmanager(G4Electron::ElectronDefinition()->GetProcessManager());
  if( GmG4Utils::CheckProcessExists( pmanager, "ElectronNuclear" )) return;
  G4ElectronNuclearProcess * electronNuclearProcess(new G4ElectronNuclearProcess);
  electronNuclearProcess->RegisterMe(electronReaction);
  pmanager->AddDiscreteProcess(electronNuclearProcess);
  
}
