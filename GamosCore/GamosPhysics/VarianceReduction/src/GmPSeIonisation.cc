#include "GmPSeIonisation.hh"
#include "G4Electron.hh"
#include "G4MollerBhabhaModel.hh"
#include "G4UniversalFluctuation.hh"
#include "G4BohrFluctuations.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

GmPSeIonisation::GmPSeIonisation(const G4String& name)
  : GmVPSEnergyLossProcess(name)
{
  //  SetStepFunction(0.2, 1*mm);
  // SetIntegral(true);
  // SetVerboseLevel(1);
  SetProcessSubType(fIonisation);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmPSeIonisation::~GmPSeIonisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double GmPSeIonisation::MinPrimaryEnergy(const G4ParticleDefinition*,
					 const G4Material*,
					 G4double cut)
{
  G4double x = cut;
  if(isElectron) x += cut;
  return x;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool GmPSeIonisation::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Electron::Electron() || &p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmPSeIonisation::InitialiseEnergyLossProcess(
		    const G4ParticleDefinition* part,
		    const G4ParticleDefinition*)
{
  if(!isInitialised) {
    if(part == G4Positron::Positron()) isElectron = false;
    SetSecondaryParticle(theElectron);
    if (!EmModel()) SetEmModel(new G4MollerBhabhaModel());
    EmModel()->SetLowEnergyLimit (MinKinEnergy());
    EmModel()->SetHighEnergyLimit(MaxKinEnergy());
    if (!FluctModel()) SetFluctModel(new G4UniversalFluctuation());
                
    AddEmModel(1, EmModel(), FluctModel());
    isInitialised = true;
  }

#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) G4cout << "GmPSeIonisation:GmPSeIonisation  registering model " << MinKinEnergy() << " " << MaxKinEnergy() << " " << EmModel(1) << " " << EmModel(2) << "NM  " << NumberOfModels() << G4endl;
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmPSeIonisation::PrintInfo()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
