#include "GmInMassPhysicalVolumeFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//-----------------------------------------------------------------
GmInMassPhysicalVolumeFilter::GmInMassPhysicalVolumeFilter(G4String name)
  :GmVFilter(name)
{
  theVPhysicalVolumes.clear();
}

//-----------------------------------------------------------------
GmInMassPhysicalVolumeFilter::~GmInMassPhysicalVolumeFilter()
{ 
  theVPhysicalVolumes.clear();
}

//-----------------------------------------------------------------
G4bool GmInMassPhysicalVolumeFilter::AcceptTrack(const G4Track* aTrack )
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeFilter::AcceptTrack " << G4endl;
#endif

  G4VPhysicalVolume* physvol = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndSetup( aTrack->GetPosition(), (G4ThreeVector*)0, false, false ); 
  if( physvol ) {
    if( theVPhysicalVolumes.find(physvol) != theVPhysicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeFilter::AcceptTrack  return 1 " << physvol->GetName() << G4endl;
#endif
      return TRUE;
    }
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeFilter::AcceptTrack  return 0 " << physvol->GetName() << G4endl;
#endif
    return FALSE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeFilter::AcceptTrack  return 0 , no vol " << G4endl;
#endif
    return FALSE;
  }
}

//-----------------------------------------------------------------
G4bool GmInMassPhysicalVolumeFilter::AcceptStep(const G4Step* aStep)
{
  G4VPhysicalVolume* physvol = aStep->GetTrack()->GetVolume();
  if( theVPhysicalVolumes.find(physvol) != theVPhysicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeFilter::AcceptStep  return 1 " << physvol->GetName() << G4endl;
#endif
    return TRUE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmInMassPhysicalVolumeFilter::AcceptStep  return 0 " << physvol->GetName() << G4endl;
#endif
  return FALSE;
}

//-----------------------------------------------------------------
void GmInMassPhysicalVolumeFilter::show()
{
  G4cout << "----G4VPhysicalVolumeFilter volume list------"<<G4endl; 
  std::set<G4VPhysicalVolume*>::const_iterator iter;
  for ( iter = theVPhysicalVolumes.begin(); iter != theVPhysicalVolumes.end(); iter++ ){
    G4cout << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//-------------------------------------------------------------
void GmInMassPhysicalVolumeFilter::SetParameters( std::vector<G4String>& params)
{
  GmTouchableFilterPhysicalVolume::SetParameters(params);
}
