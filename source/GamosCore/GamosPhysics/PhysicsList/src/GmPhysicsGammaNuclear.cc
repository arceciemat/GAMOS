#include "GmPhysicsGammaNuclear.hh"
#include "G4ProcessManager.hh"


#include "G4HadronInelasticProcess.hh"
#include "G4LowEGammaNuclearModel.hh"
#include "G4CascadeInterface.hh"

#include "G4PhotoNuclearCrossSection.hh"
using namespace CLHEP;

GmPhysicsGammaNuclear::GmPhysicsGammaNuclear(const G4String& name, G4int type): G4VPhysicsConstructor(name,type)
{ }

GmPhysicsGammaNuclear::~GmPhysicsGammaNuclear()
{ }

void GmPhysicsGammaNuclear::ConstructProcess()
{

   G4HadronInelasticProcess* process = new G4HadronInelasticProcess( "photonNuclear", G4Gamma::Definition() );
   process->AddDataSet( new G4PhotoNuclearCrossSection );
   
   // to not register a model, set Emax=0; eg. Emax1 = 0.
   const G4double Emax1 = 200*MeV, Emax2 = 10*GeV;

   if (Emax1 > 0.) {  // model 1
     G4LowEGammaNuclearModel* model1 = new G4LowEGammaNuclearModel();
     model1->SetMaxEnergy(Emax1);
     process->RegisterMe(model1);
   }
   
   if (Emax2 > 0.) {  // model 2   
     G4CascadeInterface* model2 = new G4CascadeInterface();
     G4double Emin2 = std::max(Emax1-1*MeV, 0.);
     model2->SetMinEnergy(Emin2);   
     model2->SetMaxEnergy(Emax2);
     process->RegisterMe(model2);
   }
   
   G4ProcessManager* pManager = G4Gamma::Gamma()->GetProcessManager();
   pManager->AddDiscreteProcess(process);
}
  
