#include "GmRangeRejectionUA.hh"
#include "G4Run.hh"
#include "G4Track.hh"
#include "G4RToEConvForGamma.hh"
#include "G4RToEConvForElectron.hh"
#include "G4RToEConvForPositron.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ProductionCuts.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4EnergyLossTables.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------
GmRangeRejectionUA::GmRangeRejectionUA()
{
}

//----------------------------------------------------------------
void GmRangeRejectionUA::BeginOfRunAction( const G4Run* )
{
  bUseDirection = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":UseDirection",0));
}

//----------------------------------------------------------------
G4ClassificationOfNewTrack GmRangeRejectionUA::ClassifyNewTrack(const G4Track* aTrack)
{
  G4ParticleDefinition* part = aTrack->GetDefinition();
  if( part->GetPDGCharge() == 0 ) return fUrgent; // ONLY FOR CHARGED PARTICLES

  // G4double safety = aTrack->GetStep()->GetPreStepPoint()->GetSafety();
  G4VPhysicalVolume* pv = aTrack->GetVolume();
  if( !pv ) return fUrgent;
  G4LogicalVolume* lv = pv->GetLogicalVolume();
  G4double range = G4EnergyLossTables::GetRange(part,aTrack->GetKineticEnergy(),lv->GetMaterialCutsCouple());

  G4double safety = 0;
  if(bUseDirection) {
     safety = GmGeometryUtils::GetInstance()->GetDistanceToOutDirection(aTrack);
  } else {
    safety = GmGeometryUtils::GetInstance()->GetDistanceToOutNoDirection(aTrack);  
  }

  //  G4cout << part->GetParticleName() << " VOL " << lv->GetName() << " E " << aTrack->GetKineticEnergy() << " safety " << safety << " range " << range << G4endl;
  if( safety < range ) {
    return fUrgent;
  } else {
    return fKill;
  }

  return fUrgent;
}
