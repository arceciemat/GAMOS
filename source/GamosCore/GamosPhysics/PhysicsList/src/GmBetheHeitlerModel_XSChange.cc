#include "GmBetheHeitlerModel_XSChange.hh"
#include "GmPhysicsVerbosity.hh"

GmBetheHeitlerModel_XSChange::GmBetheHeitlerModel_XSChange(const G4ParticleDefinition* part, 
                                         const G4String& nam)
  : G4BetheHeitlerModel(part, nam+"_XSChange"), GmXSChange_DistributionUser("XS_Change")
{
 }

GmBetheHeitlerModel_XSChange::~GmBetheHeitlerModel_XSChange()
{
}


G4double 
GmBetheHeitlerModel_XSChange::ComputeCrossSectionPerAtom(const G4ParticleDefinition* part,
						G4double energy,
						G4double Z, G4double A,
						G4double cutEnergy, G4double maxEnergy)
{
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange START GmBetheHeitlerModel_XSChange::ComputeCrossSectionPerAtom " << G4endl; 
  G4double XS = G4BetheHeitlerModel::ComputeCrossSectionPerAtom(part, energy, Z, A, cutEnergy, maxEnergy);
 G4double XStransformation = GetDistributionValue(energy);

  if(PhysicsVerb(debugVerb)) G4cout << " GmBetheHeitlerModel_XSChange::ComputeCrossSectionPerAtom " <<  XS*XStransformation << " = " << XS << "*" << XStransformation << " energy= " << energy << G4endl; 
  if(PhysicsVerb(testVerb)) G4cout << "CHECK_XSChange END GmBetheHeitlerModel_XSChange::ComputeCrossSectionPerAtom " << G4endl; 

  return XS*XStransformation;

}





