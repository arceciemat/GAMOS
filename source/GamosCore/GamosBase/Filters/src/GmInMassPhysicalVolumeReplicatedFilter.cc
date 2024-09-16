#include "GmInMassPhysicalVolumeReplicatedFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//-----------------------------------------------------------------
GmInMassPhysicalVolumeReplicatedFilter::GmInMassPhysicalVolumeReplicatedFilter(G4String name)
  :GmVFilter(name)
{
  theVPhysicalVolumes.clear();
}

//-----------------------------------------------------------------
GmInMassPhysicalVolumeReplicatedFilter::~GmInMassPhysicalVolumeReplicatedFilter()
{ 
  theVPhysicalVolumes.clear();
}

//-----------------------------------------------------------------
G4bool GmInMassPhysicalVolumeReplicatedFilter::AcceptTrack(const G4Track* aTrack )
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptTrack " << G4endl;
#endif

  G4VPhysicalVolume* physvol = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndSetup( aTrack->GetPosition(), (G4ThreeVector*)0, false, false ); 
  if( physvol ) {
    G4String pvName = physvol->GetName() + ":" + GmGenUtils::ftoa(aTrack->GetTouchable()->GetReplicaNumber());
    if( theVPhysicalVolumes.find(pvName) != theVPhysicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptTrack  return 1 " << physvol->GetName() << G4endl;
#endif
      return TRUE;
    }
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptTrack  return 0 " << physvol->GetName() << G4endl;
#endif
    return FALSE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptTrack  return 0 , no vol " << G4endl;
#endif
    return FALSE;
  }
}

//-----------------------------------------------------------------
G4bool GmInMassPhysicalVolumeReplicatedFilter::AcceptStep(const G4Step* aStep)
{
  G4VPhysicalVolume* physvol = aStep->GetTrack()->GetVolume();
  G4String pvName = physvol->GetName() + ":" + GmGenUtils::ftoa(aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber());
  if( theVPhysicalVolumes.find(pvName) != theVPhysicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptStep  return 1 " << physvol->GetName() << G4endl;
#endif
    return TRUE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeReplicatedFilter::AcceptStep  return 0 " << physvol->GetName() << G4endl;
#endif
  return FALSE;
}

//-----------------------------------------------------------------
void GmInMassPhysicalVolumeReplicatedFilter::show()
{
  G4cout << "----G4VPhysicalVolumeFilter volume list------"<<G4endl; 
  std::set<G4String>::const_iterator iter;
  for ( iter = theVPhysicalVolumes.begin(); iter != theVPhysicalVolumes.end(); iter++ ){
    G4cout << (*iter) << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//-------------------------------------------------------------
void GmInMassPhysicalVolumeReplicatedFilter::SetParameters( std::vector<G4String>& params)
{
  GmTouchableFilterPhysicalVolumeReplicated::SetParameters(params);
}

//-------------------------------------------------------------------------
G4bool GmInMassPhysicalVolumeReplicatedFilter::AcceptStackedTrack(const G4Track* aTrack)
{
  G4Exception(" GmInMassPhysicalVolumeReplicatedFilter::AcceptStackedTrack",
	      "",
	      FatalException,
	      "Cannot be called for a stacking action");

  return FALSE;
}
