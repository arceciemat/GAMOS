#include "GmEmDNAPhysicsNotChem.hh"
#include "GmEmDNAPhysicsMessenger.hh"
#include "GmDNAVerbosity.hh"

#include "G4SystemOfUnits.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmDNAPhysics_option1.hh"
#include "G4EmDNAPhysics_option2.hh"
#include "G4EmDNAPhysics_option3.hh"
#include "G4EmDNAPhysics_option4.hh"
#include "G4EmDNAPhysics_option5.hh"
#include "G4EmDNAPhysics_option6.hh"
#include "G4EmDNAPhysics_option7.hh"
#include "G4EmDNAPhysics_option8.hh"
#include "G4EmDNAChemistry.hh"
#include "G4EmDNAChemistry_option1.hh"
#include "G4EmDNAChemistry_option2.hh"
#include "G4EmDNAChemistry_option3.hh"
#include "G4ParticleTable.hh"
#include "G4VEmProcess.hh"
#include "G4VEmModel.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmEmDNAPhysicsNotChem::GmEmDNAPhysicsNotChem() 
: GmVEmDNAPhysics()
{
  currentDefaultCut   = 1.0*micrometer;
  cutForGamma         = currentDefaultCut;
  cutForElectron      = currentDefaultCut;
  cutForPositron      = currentDefaultCut;
  new GmEmDNAPhysicsMessenger(this);

  SetVerboseLevel(1);

  BuildDNAPhysics();
  G4cout << "NOT BUILT G4EmDNAChemistry " << G4endl;


}

void GmEmDNAPhysicsNotChem::BuildDNAChemistry()
{
  /*     
  G4String chemName = GmParameterMgr::GetInstance()->GetStringValue("GmEmDNAPhysicsNotChem:registerChemistry","DNA_option2");
  // Replace standard processes for gammas
  if (chemName == "DNA") { 
    RegisterPhysics( new G4EmDNAChemistry() );
    
  } else if (chemName == "DNA_option1") {
    RegisterPhysics( new G4EmDNAChemistry_option1() );
  
  } else if (chemName == "DNA_option2") {
    RegisterPhysics( new G4EmDNAChemistry_option2() );
  
  } else if (chemName == "DNA_option3") {
    RegisterPhysics( new G4EmDNAChemistry_option3() );
  } else {
    G4Exception("GmEmDNAPhysicsNotChem::GmEmDNAPhysicsNotChem",
		"",
		FatalErrorInArgument,
		("Physics can only be DNA, DNA_option1, DNA_option2, DNA_option3, it is "+chemName).c_str());
  }
  */

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmEmDNAPhysicsNotChem::~GmEmDNAPhysicsNotChem()
{
}

