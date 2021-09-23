#include "GmPEEffectFluoModel_XSChange.hh"
#include "GmPhysicsVerbosity.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmPEEffectFluoModel_XSChange::GmPEEffectFluoModel_XSChange(const G4String& nam)
  : G4PEEffectFluoModel(nam+"_XSChange"), GmXSChange_DistributionUser("XS_Change")
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmPEEffectFluoModel_XSChange::~GmPEEffectFluoModel_XSChange()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double 
GmPEEffectFluoModel_XSChange::ComputeCrossSectionPerAtom(const G4ParticleDefinition* part,
						G4double energy,
						G4double Z, G4double A,
						G4double cutEnergy, G4double maxEnergy)
{
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange START GmPEEffectFluoModel_XSChange::ComputeCrossSectionPerAtom " << G4endl; 

  G4double XS = G4PEEffectFluoModel::ComputeCrossSectionPerAtom(part, energy, Z, A, cutEnergy, maxEnergy);
  G4double XStransformation = GetDistributionValue(energy);

  if(PhysicsVerb(debugVerb)) G4cout << " GmPEEffectFluoModel_XSChange::ComputeCrossSectionPerAtom " <<  XS*XStransformation << " = " << XS << "*" << XStransformation << " energy= " << energy << G4endl; 
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange END GmPEEffectFluoModel_XSChange::ComputeCrossSectionPerAtom " << G4endl; 

  return XS*XStransformation;

}
