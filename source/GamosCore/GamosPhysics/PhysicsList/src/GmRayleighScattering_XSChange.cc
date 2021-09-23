#include "GmRayleighScattering_XSChange.hh"
#include "GmPhysicsVerbosity.hh"
#include "GmLivermoreRayleighModel_XSChange.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmRayleighScattering_XSChange::GmRayleighScattering_XSChange(const G4String& processName,
  G4ProcessType type):G4RayleighScattering (processName, type), GmXSChange_DistributionUser("XS_Change")
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
GmRayleighScattering_XSChange::~GmRayleighScattering_XSChange()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmRayleighScattering_XSChange::InitialiseProcess(const G4ParticleDefinition* part)
{
  //  G4RayleighScattering::InitialiseProcess(part);
  //  SetEmModel(new GmLivermoreRayleighModel_XSChangeNEW, 0); //GDEB
  SetEmModel(new GmLivermoreRayleighModel_XSChange, 0); //GDEB
  AddEmModel(0, EmModel());
}

void GmRayleighScattering_XSChange::ProcessDescription(std::ostream& out) const
{
  out << "  Rayleigh scattering XS Change";
  G4VEmProcess::ProcessDescription(out);
}
