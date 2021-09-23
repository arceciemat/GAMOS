#include "GmPhysicsGammaNuclear.hh"
#include "G4ProcessManager.hh"
#include "G4Gamma.hh"
#include "G4PhotoNuclearProcess.hh"
#include "G4CascadeInterface.hh"
#include "G4TheoFSGenerator.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4GammaParticipants.hh"
#include "G4QGSModel.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
using namespace CLHEP;

GmPhysicsGammaNuclear::GmPhysicsGammaNuclear(const G4String& name, G4int type): G4VPhysicsConstructor(name,type)
{ }

GmPhysicsGammaNuclear::~GmPhysicsGammaNuclear()
{ }

void GmPhysicsGammaNuclear::ConstructProcess()
{
  G4CascadeInterface * lowEGammaModel = new G4CascadeInterface();
  lowEGammaModel->SetMaxEnergy(3.5*CLHEP::GeV);
  
  G4TheoFSGenerator * highEGammaModel(new G4TheoFSGenerator);
  highEGammaModel->SetTransport(new G4GeneratorPrecompoundInterface);
  
  G4QGSModel<G4GammaParticipants> * stringModel(new G4QGSModel<G4GammaParticipants>);
  stringModel->SetFragmentationModel(new G4ExcitedStringDecay(new G4QGSMFragmentation));
  
  highEGammaModel->SetHighEnergyGenerator(stringModel);
  highEGammaModel->SetMinEnergy(3.*CLHEP::GeV);
  highEGammaModel->SetMaxEnergy(100.*TeV); 
  
  G4ProcessManager * pmanager=G4Gamma::GammaDefinition()->GetProcessManager();
  if( GmG4Utils::CheckProcessExists( pmanager, "" )) return;
  G4PhotoNuclearProcess * photoNuclearProcess(new G4PhotoNuclearProcess);
  photoNuclearProcess->RegisterMe(lowEGammaModel);
  photoNuclearProcess->RegisterMe(highEGammaModel);
  pmanager->AddDiscreteProcess(photoNuclearProcess);
}
