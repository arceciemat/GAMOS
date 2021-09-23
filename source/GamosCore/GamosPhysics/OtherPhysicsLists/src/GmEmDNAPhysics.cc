//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GmEmDNAPhysics.hh"
#include "GmEmDNAPhysicsMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmDNAPhysics_option1.hh"
#include "G4EmDNAPhysics_option2.hh"
#include "G4EmDNAPhysics_option3.hh"
#include "G4EmDNAPhysics_option4.hh"
#include "G4EmDNAPhysics_option5.hh"
#include "G4EmDNAPhysics_option6.hh"
#include "G4EmDNAPhysics_option7.hh"
#include "G4ParticleTable.hh"
#include "G4VEmProcess.hh"
#include "G4VEmModel.hh"

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
  RegisterPhysics( new G4EmDNAPhysics());

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
  G4cout << "GmEMPhysics::Replaceing PhysicsList component " << name << G4endl;
  
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
  }
								
  if( bFound ) G4cout << "GmEMPhysics::ReplacePhysicsList: " << name << " is registered" << G4endl;

  return bFound;

}
