#include "GmComptonScattering_XSChange.hh"
#include "GmKleinNishinaCompton_XSChange.hh"
#include "GmPhysicsVerbosity.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

GmComptonScattering_XSChange::GmComptonScattering_XSChange(const G4String& processName,
  G4ProcessType type):G4ComptonScattering(processName, type), GmXSChange_DistributionUser("XS_Change")
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
GmComptonScattering_XSChange::~GmComptonScattering_XSChange()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmComptonScattering_XSChange::InitialiseProcess(const G4ParticleDefinition* )
{
  //  G4ComptonScattering::InitialiseProcess(part);
  SetEmModel(new GmKleinNishinaCompton_XSChange(),0);
  G4EmParameters* param = G4EmParameters::Instance();
  EmModel(0)->SetLowEnergyLimit(param->MinKinEnergy());
  EmModel(0)->SetHighEnergyLimit(param->MaxKinEnergy());
  AddEmModel(0, EmModel(0)); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmComptonScattering_XSChange::ProcessDescription(std::ostream& out) const
{
  out << "  Compton scattering XS Change";
  G4VEmProcess::ProcessDescription(out);
}


