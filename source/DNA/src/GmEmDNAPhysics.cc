#include "GmEmDNAPhysics.hh"
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

GmEmDNAPhysics::GmEmDNAPhysics() 
: G4VModularPhysicsList()
{
  currentDefaultCut   = 1.0*micrometer;
  cutForGamma         = currentDefaultCut;
  cutForElectron      = currentDefaultCut;
  cutForPositron      = currentDefaultCut;
  new GmEmDNAPhysicsMessenger(this);

  SetVerboseLevel(1);

  // EM physics
  G4String physName = GmParameterMgr::GetInstance()->GetStringValue("GmEmDNAPhysics:registerPhysics","DNA_option2");
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(infoVerb) ) G4cout << "GmEmDNAPhysics::GmEmDNAPhysics " << physName << G4endl;
#endif
  if (physName == "DNA") { 
    RegisterPhysics( new G4EmDNAPhysics(1,"G4Em"+physName) ); // Type bElectromagnetic  
  } else if (physName == "DNA_option1") {
    RegisterPhysics( new G4EmDNAPhysics_option1(1,"G4Em"+physName) );
  
  } else if (physName == "DNA_option2") {
    RegisterPhysics( new G4EmDNAPhysics_option2(1,"G4Em"+physName) );
  
  } else if (physName == "DNA_option3") {
    RegisterPhysics( new G4EmDNAPhysics_option3(1,"G4Em"+physName) );
  
  } else if (physName == "DNA_option4") {
    RegisterPhysics( new G4EmDNAPhysics_option4(1,"G4Em"+physName) );
  
  } else if (physName == "DNA_option5") {
    RegisterPhysics( new G4EmDNAPhysics_option5(1,"G4Em"+physName) );
  
  } else if (physName == "DNA_option6") {
    RegisterPhysics( new G4EmDNAPhysics_option6(1,"G4Em"+physName) );
  
  } else if (physName == "DNA_option7") {
    RegisterPhysics( new G4EmDNAPhysics_option7(1,"G4Em"+physName) );

  } else if (physName == "DNA_option8") {
    RegisterPhysics( new G4EmDNAPhysics_option8(1,"G4Em"+physName) );
  } else {
    G4Exception("GmEmDNAPhysics::GmEmDNAPhysics",
		"",
		FatalErrorInArgument,
		("Physics can only be DNA, DNA_option1, DNA_option2, DNA_option3, DNA_option4, DNA_option5, DNA_option6, DNA_option7, DNA_option8, it is "+physName).c_str());
  }

    
  G4String chemName = GmParameterMgr::GetInstance()->GetStringValue("GmEmDNAPhysics:registerChemistry","DNA_option2");
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
    G4Exception("GmEmDNAPhysics::GmEmDNAPhysics",
		"",
		FatalErrorInArgument,
		("Physics can only be DNA, DNA_option1, DNA_option2, DNA_option3, it is "+chemName).c_str());
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmEmDNAPhysics::~GmEmDNAPhysics()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmEmDNAPhysics::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmEmDNAPhysics::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

void GmEmDNAPhysics::SetCuts()
{
 // fixe lower limit for cut
 G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV, 1*GeV);
  
 // set cut values for gamma at first and for e- second and next for e+,
 // because some processes for e+/e- need cut values for gamma
 SetCutValue(cutForGamma, "gamma");
 SetCutValue(cutForElectron, "e-");
 SetCutValue(cutForPositron, "e+");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmEmDNAPhysics::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmEmDNAPhysics::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmEmDNAPhysics::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool GmEmDNAPhysics::ReplacePhysicsList(const G4String& name)
{
  G4cout << "GmEMDNAPhysics::Replacing PhysicsList component " << name << G4endl;
  
  //
  // Electromagnetic GamosEMLowEnPhysics. 
  //
  // The user can choose three alternative approaches:
  // Standard, Low Energy based on the Livermore libraries and Low Energy Penelope
  //
  G4bool bFound = true;

  // Replace standard processes for gammas
  if (name == "DNA") { 
    ReplacePhysics( new G4EmDNAPhysics(1,"G4Em"+name) ); // Type bElectromagnetic  
  } else if (name == "DNA_option1") {
    ReplacePhysics( new G4EmDNAPhysics_option1(1,"G4Em"+name) );
  
  } else if (name == "DNA_option2") {
    ReplacePhysics( new G4EmDNAPhysics_option2(1,"G4Em"+name) );
  
  } else if (name == "DNA_option3") {
    ReplacePhysics( new G4EmDNAPhysics_option3(1,"G4Em"+name) );
  
  } else if (name == "DNA_option4") {
    ReplacePhysics( new G4EmDNAPhysics_option4(1,"G4Em"+name) );
  
  } else if (name == "DNA_option5") {
    ReplacePhysics( new G4EmDNAPhysics_option5(1,"G4Em"+name) );
  
  } else if (name == "DNA_option6") {
    ReplacePhysics( new G4EmDNAPhysics_option6(1,"G4Em"+name) );
  
  } else if (name == "DNA_option7") {
    ReplacePhysics( new G4EmDNAPhysics_option7(1,"G4Em"+name) );

  } else if (name == "DNA_option8") {
    ReplacePhysics( new G4EmDNAPhysics_option8(1,"G4Em"+name) );
  } else {
    bFound = false;
  }

  return bFound;

}
