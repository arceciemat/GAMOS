#include "GmPSPhotoElectricEffect.hh"
#include "GmEWPSPEEffectModel.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

GmPSPhotoElectricEffect::GmPSPhotoElectricEffect(const G4String& processName,
  G4ProcessType type):GmVPSGammaProcess (processName, type),
    isInitialised(false)
{
  SetProcessSubType(fPhotoElectricEffect);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmPSPhotoElectricEffect::~GmPSPhotoElectricEffect()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool GmPSPhotoElectricEffect::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmPSPhotoElectricEffect::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    SetBuildTableFlag(false);
    SetSecondaryParticle(G4Electron::Electron());
    if(!Model()) SetModel(new GmEWPSPEEffectModel());
    Model()->SetLowEnergyLimit(MinKinEnergy());
    Model()->SetHighEnergyLimit(MaxKinEnergy());
    AddEmModel(1, Model());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmPSPhotoElectricEffect::PrintInfo()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
