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
/// \file electromagnetic/TestEm7/src/PhysListEmStandardSS.cc
/// \brief Implementation of the PhysListEmStandardSS class
//
// $Id: PhysListEmStandardSS.cc,v 1.5 2018/06/28 15:27:34 arce Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "PhysListEmStandardSS.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4LossTableManager.hh"
//#include "G4EmProcessOptions.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4PEEffectFluoModel.hh"
#include "G4KleinNishinaModel.hh"
#include "G4LowEPComptonModel.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4CoulombScattering.hh"
#include "G4eCoulombScatteringModel.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4Generator2BS.hh"
#include "G4SeltzerBergerModel.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eplusAnnihilation.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmStandardSS::PhysListEmStandardSS(const G4String& name)
   :  G4VPhysicsConstructor(name)
{
  G4LossTableManager::Instance();
  SetPhysicsType(bElectromagnetic);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysListEmStandardSS::~PhysListEmStandardSS()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysListEmStandardSS::ConstructProcess()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  // muon & hadron bremsstrahlung and pair production
  G4MuBremsstrahlung* mub = new G4MuBremsstrahlung();
  G4MuPairProduction* mup = new G4MuPairProduction();

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4String particleName = particle->GetParticleName();
     
    if (particleName == "gamma") {

       // Compton scattering
      G4ComptonScattering* cs = new G4ComptonScattering;
      cs->SetEmModel(new G4KleinNishinaModel(),1);
      G4VEmModel* theLowEPComptonModel = new G4LowEPComptonModel();
      theLowEPComptonModel->SetHighEnergyLimit(20*MeV);
      cs->AddEmModel(0, theLowEPComptonModel);
      ph->RegisterProcess(cs, particle);

      // Photoelectric
      G4PhotoElectricEffect* pe = new G4PhotoElectricEffect();
      G4VEmModel* theLivermorePEModel = new G4LivermorePhotoElectricModel();
      theLivermorePEModel->SetHighEnergyLimit(10*GeV);
      pe->SetEmModel(theLivermorePEModel,1);
      ph->RegisterProcess(pe, particle);

      // Gamma conversion
      G4GammaConversion* gc = new G4GammaConversion();
      G4VEmModel* thePenelopeGCModel = new G4PenelopeGammaConversionModel();
      thePenelopeGCModel->SetHighEnergyLimit(1*GeV);
      gc->SetEmModel(thePenelopeGCModel,1);
      ph->RegisterProcess(gc, particle);

      // Rayleigh scattering
      ph->RegisterProcess(new G4RayleighScattering(), particle);
      
    } else if (particleName == "e-") {

      // ionisation
      G4eIonisation* eIoni = new G4eIonisation();
      eIoni->SetStepFunction(0.2, 100*um);
      G4VEmModel* theIoniPenelope = new G4PenelopeIonisationModel();
      theIoniPenelope->SetHighEnergyLimit(0.1*MeV);
      eIoni->AddEmModel(0, theIoniPenelope, new G4UniversalFluctuation());

      // bremsstrahlung
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();

      ph->RegisterProcess(eIoni, particle);
      ph->RegisterProcess(eBrem, particle);
      ph->RegisterProcess(new G4CoulombScattering(), particle);
            
    } else if (particleName == "e+") {
      // ionisation
      G4eIonisation* eIoni = new G4eIonisation();
      eIoni->SetStepFunction(0.2, 100*um);
      G4VEmModel* theIoniPenelope = new G4PenelopeIonisationModel();
      theIoniPenelope->SetHighEnergyLimit(0.1*MeV);
      eIoni->AddEmModel(0, theIoniPenelope, new G4UniversalFluctuation());

      // bremsstrahlung
      G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();

      ph->RegisterProcess(eIoni, particle);
      ph->RegisterProcess(eBrem, particle);
      ph->RegisterProcess(new G4CoulombScattering(), particle);

      // annihilation at rest and in flight
      ph->RegisterProcess(new G4eplusAnnihilation(), particle);
            
    } else if (particleName == "mu+" || 
               particleName == "mu-"    ) {

      G4MuIonisation* muIoni = new G4MuIonisation();
      muIoni->SetStepFunction(0.2, 50*um);          

      ph->RegisterProcess(muIoni, particle);
      ph->RegisterProcess(mub, particle);
      ph->RegisterProcess(mup, particle);
      ph->RegisterProcess(new G4CoulombScattering(), particle);
             
    } else if (particleName == "alpha" || particleName == "He3") {

      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetStepFunction(0.1, 10*um);
      ph->RegisterProcess(ionIoni, particle);

      G4CoulombScattering* cs = new G4CoulombScattering();
      cs->SetBuildTableFlag(false);
      ph->RegisterProcess(cs, particle);

    } else if (particleName == "GenericIon" ) { 

      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ionIoni->SetStepFunction(0.1, 1*um);
      ph->RegisterProcess(ionIoni, particle);

      G4CoulombScattering* cs = new G4CoulombScattering();
      cs->SetBuildTableFlag(false);
      ph->RegisterProcess(cs, particle);
     
    } else if ((!particle->IsShortLived()) &&
               (particle->GetPDGCharge() != 0.0) && 
               (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino

      ph->RegisterProcess(new G4hIonisation(), particle);
      G4CoulombScattering* cs = new G4CoulombScattering();
      cs->SetBuildTableFlag(false);
      ph->RegisterProcess(cs, particle);

    }
  }
  
  // Em options
  //
  // Main options and setting parameters are shown here.
  // Several of them have default values.
  //
  /*   G4EmProcessOptions emOptions;
  
  //physics tables
  //
    emOptions.SetMinEnergy(10*eV);        
  emOptions.SetMaxEnergy(10*TeV);      
  emOptions.SetDEDXBinning(12*20);      
  emOptions.SetLambdaBinning(12*20); 

  // scattering
  emOptions.SetPolarAngleLimit(0.0);
  */
  // Deexcitation
  G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
  G4LossTableManager::Instance()->SetAtomDeexcitation(de);
  de->SetFluo(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

