#include "GmLivermoreRayleighModel_XSChange.hh"
#include "GmPhysicsVerbosity.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmLivermoreRayleighModel_XSChange::GmLivermoreRayleighModel_XSChange()
  : G4LivermoreRayleighModel(), GmXSChange_DistributionUser("XS_Change")
{
  if(PhysicsVerb(testVerb)) G4cout << "GmLivermoreRayleighModel_XSChange::GmLivermoreRayleighModel_XSChange " << G4endl; 
  //  SetModelName("LivermoreRayleigh_XSChange");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmLivermoreRayleighModel_XSChange::~GmLivermoreRayleighModel_XSChange()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double GmLivermoreRayleighModel_XSChange::ComputeCrossSectionPerAtom(const G4ParticleDefinition* part,
						G4double energy,
						G4double Z, G4double A,
						G4double cutEnergy, G4double maxEnergy)
{
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange START GmLivermoreRayleighModel_XSChange::ComputeCrossSectionPerAtom " << G4endl; 

  G4double XS = G4LivermoreRayleighModel::ComputeCrossSectionPerAtom(part, energy, Z, A, cutEnergy, maxEnergy);
  G4double XStransformation = GetDistributionValue(energy);

  if(PhysicsVerb(debugVerb)) G4cout << " GmPEEffectFluoModel_XSChange::ComputeCrossSectionPerAtom " <<  XS*XStransformation << " = " << XS << "*" << XStransformation << " energy= " << energy << G4endl; 
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange END GmLivermoreRayleighModel_XSChange::ComputeCrossSectionPerAtom " << G4endl; 

  return XS*XStransformation;

}

