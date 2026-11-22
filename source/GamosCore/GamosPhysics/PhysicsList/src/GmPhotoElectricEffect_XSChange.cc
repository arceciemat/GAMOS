#include "GmPhotoElectricEffect_XSChange.hh"
#include "GmPEEffectFluoModel_XSChange.hh"
#include "GmPhysicsVerbosity.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmPhotoElectricEffect_XSChange::GmPhotoElectricEffect_XSChange(const G4String& processName,
							       G4ProcessType type):G4PhotoElectricEffect(processName, type), GmXSChange_DistributionUser("XS_Change") 
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmPhotoElectricEffect_XSChange::~GmPhotoElectricEffect_XSChange()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void GmPhotoElectricEffect_XSChange::InitialiseProcess(const G4ParticleDefinition*)
{
  SetEmModel(new GmPEEffectFluoModel_XSChange(),0); 
  G4EmParameters* param = G4EmParameters::Instance();
  EmModel(0)->SetLowEnergyLimit(param->MinKinEnergy());
  EmModel(0)->SetHighEnergyLimit(param->MaxKinEnergy());
  AddEmModel(0, EmModel(0));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmPhotoElectricEffect_XSChange::ProcessDescription(std::ostream& out) const
{
  out << "  Photoelectric effect XS Change";
  G4VEmProcess::ProcessDescription(out);
}

