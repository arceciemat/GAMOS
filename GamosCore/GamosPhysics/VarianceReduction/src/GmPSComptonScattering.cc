#include "GmPSComptonScattering.hh"
#include "GmEWPSKleinNishinaComptonModel.hh"
#include "G4Electron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

GmPSComptonScattering::GmPSComptonScattering(const G4String& processName,
  G4ProcessType type):GmVPSGammaProcess (processName, type),
    isInitialised(false)
{
  SetProcessSubType(fComptonScattering);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
GmPSComptonScattering::~GmPSComptonScattering()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmPSComptonScattering::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    SetBuildTableFlag(true);
    SetSecondaryParticle(G4Electron::Electron());
    if(!Model()) SetModel(new GmEWPSKleinNishinaComptonModel);
    Model()->SetLowEnergyLimit(MinKinEnergy());
    Model()->SetHighEnergyLimit(MaxKinEnergy());
    AddEmModel(1, Model());
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
