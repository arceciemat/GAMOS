#include "GmRadiotherapyPhysics.hh"

#include "GmPhysicsGammaStandard.hh"
#include "GmPhysicsElectronStandard.hh"
#include "GmPhysicsPositronStandard.hh"
#include "GmExtraPhysicsMessenger.hh"

#include "G4EmParameters.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GmRadiotherapyPhysics::GmRadiotherapyPhysics() : GmEMPhysics()
{   
  // EM physics
  G4String name;
  name = "gamma-standard";
  G4cout << "GmRadiotherapyPhysics PhysicsList: " << name << " is registered" << G4endl;
  ReplacePhysics( new GmPhysicsGammaStandard(name,22) );

  name = "electron-standard";
  G4cout << "GmRadiotherapyPhysics PhysicsList: " << name << " is registered" << G4endl;
  ReplacePhysics( new GmPhysicsElectronStandard(name, 11) );

  /*  name = "positron-standard"; 
  G4cout << "GmRadiotherapyPhysics PhysicsList: " << name << " is registered" << G4endl;
  RegisterPhysics( new GmPhysicsPositronStandard(name, 111) );
  */
  
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetDefaults();
  param->SetMinEnergy(10*eV);
  param->SetLowestElectronEnergy(1*keV);
  param->SetNumberOfBinsPerDecade(20);
  param->ActivateAngularGeneratorForIonisation(true);
  param->SetUseMottCorrection(true);  
  param->SetStepFunction(0.2, 100*um);
  param->SetStepFunctionMuHad(0.2, 50*um);
  param->SetMscStepLimitType(fUseSafetyPlus);
  param->SetMscRangeFactor(0.05);
  param->SetMscSkin(3);
  param->SetMuHadLateralDisplacement(true);
  param->SetLateralDisplacementAlg96(true);
  param->SetFluo(true);
 
  //  new GmEMPhysicsMessenger(this);
  new GmExtraPhysicsMessenger();
  SetVerboseLevel(1);
  ConstructParticles();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmRadiotherapyPhysics::~GmRadiotherapyPhysics()
{
  //  delete pMessenger;  
}

