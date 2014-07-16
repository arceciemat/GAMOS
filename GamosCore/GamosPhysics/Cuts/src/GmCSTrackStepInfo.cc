#include "GmCSTrackStepInfo.hh"
#include "GmCutsStudyMgr.hh"
#include "GmCutsEnergy2Range.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Region.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4EnergyLossTables.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

GmCSTrackStepInfo::GmCSTrackStepInfo(const G4Track* aTrack)
{
  if( GetEnergy2Range() == 0 ) CreateEnergy2Range();
  theTrackID = aTrack->GetTrackID();
  G4LogicalVolume* lv = aTrack->GetVolume()->GetLogicalVolume();
  theRegion = lv->GetRegion();
  theProcess = 0; //no interest 
  theParticle = aTrack->GetDefinition();
  theEnergy = aTrack->GetStep()->GetPreStepPoint()->GetKineticEnergy(); //use energy before step (min range would have cut it and this step would have not been produced)
  //  G4cout << " particle " << theParticle << " = " << aTrack->GetDefinition() << G4endl;
  theRange = GetEnergy2Range()->Convert( theEnergy, theParticle, lv->GetMaterialCutsCouple() );
  theSafety = GmGeometryUtils::GetInstance()->GetDistanceToOutNoDirection(aTrack);
  
  bAccepted = FALSE;

  GmCutsStudyMgr::GetInstance()->AddTrackInfo( this );

 //  G4cout << " GmCSTrackStepInfo::GmCSTrackStepInfo track " << theTrackID << " theEnergy " << theEnergy << " theRange " << theRange << " theParticle " << theParticle->GetParticleName() << " material " << lv->GetMaterialCutsCouple()->GetMaterial()->GetName() << G4endl;
  //  G4cout << " particle " << theParticle << " = " << aTrack->GetDefinition() << G4endl;

  thePosition = aTrack->GetPosition();
  theTrackLength = aTrack->GetTrackLength();
  theParent = 0;
  if( PhysicsVerb(debugVerb) ) Print();
}

GmCSTrackStepInfo::~GmCSTrackStepInfo()
{;}

void GmCSTrackStepInfo::Print(std::ostream& out ) const
{
  out << " GmCSTrackStepInfo ID= "<< theTrackID
      << " parent= ";
 if( theParent ) { 
    out  << theParent->GetTrackID();
  } else {
    out << "-1"; 
  }
  out << " energy= " << theEnergy
      << " range= " << theRange
      << " accepted= " << bAccepted 
      << " reg= " << theRegion->GetName()
      << " part= " << theParticle->GetParticleName()
      << " energy= " << theEnergy
      << " accepted= " << bAccepted 
      << " trkLength= " << theTrackLength
      << " position= " << thePosition
      << G4endl;

}
