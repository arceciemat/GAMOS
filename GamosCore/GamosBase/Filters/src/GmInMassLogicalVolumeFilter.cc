#include "GmInMassLogicalVolumeFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//-------------------------------------------------------------
GmInMassLogicalVolumeFilter::GmInMassLogicalVolumeFilter(G4String name)
  :GmVFilter(name)
{
  theLogicalVolumes.clear();
}

//-------------------------------------------------------------
GmInMassLogicalVolumeFilter::~GmInMassLogicalVolumeFilter()
{ 
  theLogicalVolumes.clear();
}

//-------------------------------------------------------------
G4bool GmInMassLogicalVolumeFilter::AcceptTrack(const G4Track* aTrack )
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmInMassLogicalVolumeFilter::AcceptTrack " << G4endl;
#endif

  G4VPhysicalVolume* physvol = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndSetup( aTrack->GetPosition(), (G4ThreeVector*)0, false, false ); 
  if( physvol ) {
    G4LogicalVolume* logvol = physvol->GetLogicalVolume();
    
    if( theLogicalVolumes.find(logvol) != theLogicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassLogicalVolumeFilter::AcceptTrack  return 1 " << logvol->GetName() << G4endl;
#endif
      return TRUE;
    }
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassLogicalVolumeFilter::AcceptTrack  return 0 " << logvol->GetName() << G4endl;
#endif
    return FALSE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassLogicalVolumeFilter::AcceptTrack  return 0 , no vol " << G4endl;
#endif
    return FALSE;
  }
}

//-------------------------------------------------------------
G4bool GmInMassLogicalVolumeFilter::AcceptStep(const G4Step* aStep)
{
  G4LogicalVolume* logvol = aStep->GetTrack()->GetVolume()->GetLogicalVolume();
  if( theLogicalVolumes.find(logvol) != theLogicalVolumes.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmInMassLogicalVolumeFilter::AcceptStep  return 1 " << logvol->GetName() << G4endl;
#endif
    return TRUE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmInMassLogicalVolumeFilter::AcceptStep  return 0 " << logvol->GetName() << G4endl;
#endif
  return FALSE;
}

//-------------------------------------------------------------
void GmInMassLogicalVolumeFilter::show()
{
  G4cout << "----G4LogicalVolumeFilter volume list------"<<G4endl; 
  std::set<G4LogicalVolume*>::const_iterator iter;
  for ( iter = theLogicalVolumes.begin(); iter != theLogicalVolumes.end(); iter++ ){
    G4cout << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//-------------------------------------------------------------
void GmInMassLogicalVolumeFilter::SetParameters( std::vector<G4String>& params)
{
  GmTouchableFilterLogicalVolume::SetParameters(params);
}
