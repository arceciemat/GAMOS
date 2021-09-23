#include "RTReuseAtZPlaneMessenger.hh"
#include "RTReuseAtZPlaneProcess.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
RTReuseAtZPlaneMessenger::RTReuseAtZPlaneMessenger()
{ 
  theRTDir = new G4UIdirectory("/gamos/RT/");
  theRTDir->SetGuidance("Command to define uniform magnetic fields");
  
  theReuseCmd = new G4UIcmdWithoutParameter("/gamos/RT/ReuseAtZPlane",this);  
  theReuseCmd->SetGuidance("Define magnetic field vector: FIELD_X FIELD_Y FIELD_Z");
  theReuseCmd->AvailableForStates(G4State_Idle); 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
RTReuseAtZPlaneMessenger::~RTReuseAtZPlaneMessenger()
{
  delete theReuseCmd;
  delete theRTDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
void RTReuseAtZPlaneMessenger::SetNewValue(G4UIcommand* command,G4String)
{ 
  if( command == theReuseCmd ) {
    G4ParticleTable::G4PTblDicIterator* theParticleIterator = G4ParticleTable::GetParticleTable()->GetIterator();
    theParticleIterator->reset();

    RTReuseAtZPlaneProcess* reuseProc = new RTReuseAtZPlaneProcess;
    while( (*theParticleIterator)() ){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();
      pmanager->AddDiscreteProcess( reuseProc );
    }
  }

}
