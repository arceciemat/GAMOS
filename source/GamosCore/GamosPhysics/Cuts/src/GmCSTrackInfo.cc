#include "GmCSTrackInfo.hh"
#include "GmCutsStudyMgr.hh"
#include "GmCutsEnergy2Range.hh"
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

GmCutsEnergy2Range* GmCSTrackInfo::theEnergy2Range = 0;

GmCSTrackInfo::GmCSTrackInfo(const G4Track* aTrack)
{
  if( theEnergy2Range == 0 ) theEnergy2Range = new GmCutsEnergy2Range;
  theTrackID = aTrack->GetTrackID();
  G4LogicalVolume* lv = aTrack->GetVolume()->GetLogicalVolume();
  theRegion = lv->GetRegion();
  theProcess = const_cast<G4VProcess*>(aTrack->GetCreatorProcess());
  theParticle = aTrack->GetDefinition();
  theEnergy = aTrack->GetKineticEnergy();
  //  G4cout << " particle " << theParticle << " = " << aTrack->GetDefinition() << G4endl;
  theRange = theEnergy2Range->Convert( theEnergy, theParticle, lv->GetMaterialCutsCouple() );
  //  theRange = G4EnergyLossTables::GetRange(theParticle,theEnergy,lv->GetMaterialCutsCouple());
  theSafety = GmGeometryUtils::GetInstance()->GetDistanceToOutNoDirection(aTrack);
  
  bAccepted = FALSE;

  GmCutsStudyMgr::GetInstance()->AddTrackInfo( this );

  //  G4cout << " GmCSTrackInfo::GmCSTrackInfo track " << theTrackID << " theEnergy " << theEnergy << " theRange " << theRange << " theParticle " << theParticle->GetParticleName() << " material " << lv->GetMaterialCutsCouple()->GetMaterial()->GetName() << G4endl;
  
  theParent = 0;
}

GmCSTrackInfo::~GmCSTrackInfo()
{;}

void GmCSTrackInfo::Print(std::ostream& out ) const
{
  out << " GmCSTrackInfo ID= "<< theTrackID
      << " parent= ";
  if( theParent ) { 
    out  << theParent->GetTrackID();
  } else {
    out << "-1"; 
  }
  out << " reg= " << theRegion->GetName()
      << " proc= " << theProcess->GetProcessName()
      << " part= " << theParticle->GetParticleName()
      << " range= " << theRange
      << " energy= " << theEnergy
      << " accepted " << bAccepted << G4endl;

}
