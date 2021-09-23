#include "GmKleinNishinaCompton_XSChange.hh"
#include "GmPhysicsVerbosity.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

GmKleinNishinaCompton_XSChange::GmKleinNishinaCompton_XSChange(const G4ParticleDefinition* part,
                                             const G4String& nam)
  : G4KleinNishinaCompton(part,nam+"_XSChange"), GmXSChange_DistributionUser("XS_Change")
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
GmKleinNishinaCompton_XSChange::~GmKleinNishinaCompton_XSChange()
{}


G4double GmKleinNishinaCompton_XSChange::ComputeCrossSectionPerAtom(const G4ParticleDefinition* part,
						G4double energy,
						G4double Z, G4double A,
						G4double cutEnergy, G4double maxEnergy)
{
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange START GmKleinNishinaCompton_XSChange::ComputeCrossSectionPerAtom " << G4endl; 
  
  G4double XS = G4KleinNishinaCompton::ComputeCrossSectionPerAtom(part, energy, Z, A, cutEnergy, maxEnergy);
  G4double XStransformation = GetDistributionValue(energy);
  
  if(PhysicsVerb(debugVerb)) G4cout << " GmKleinNishinaCompton_XSChange::ComputeCrossSectionPerAtom " <<  XS*XStransformation << " = " << XS << "*" << XStransformation << " energy= " << energy << G4endl; 
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange END GmKleinNishinaCompton_XSChange::ComputeCrossSectionPerAtom " << G4endl; 

  return XS*XStransformation;
}





