#include "GmGammaConversion_XSChange.hh"
#include "GmBetheHeitlerModel_XSChange.hh"
#include "GmPhysicsVerbosity.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

GmGammaConversion_XSChange::GmGammaConversion_XSChange(const G4String& processName,
  G4ProcessType type): G4GammaConversion(processName, type), GmXSChange_DistributionUser("XS_Change")
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
GmGammaConversion_XSChange::~GmGammaConversion_XSChange()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmGammaConversion_XSChange::InitialiseProcess(const G4ParticleDefinition*)
{
  G4EmParameters* param = G4EmParameters::Instance();
  G4double emin = std::max(param->MinKinEnergy(), 2*electron_mass_c2);
  
  SetMinKinEnergy(emin);

  SetEmModel(new GmBetheHeitlerModel_XSChange(),0);
  EmModel(0)->SetLowEnergyLimit(emin);
  G4double energyLimit = std::min(EmModel(0)->HighEnergyLimit(), 80*GeV);
  EmModel(0)->SetHighEnergyLimit(energyLimit);
  AddEmModel(0, EmModel(0));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmGammaConversion_XSChange::ProcessDescription(std::ostream& out) const
{
  out << "  Gamma conversion XS Change";
  G4VEmProcess::ProcessDescription(out);
}

