#include "GmPhysicalVolumeFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------                   
GmPhysicalVolumeFilter::GmPhysicalVolumeFilter(G4String name)
  :GmVFilter(name)
{
  theVPhysicalVolumes.clear();
}

//----------------------------------------------------------------                   
GmPhysicalVolumeFilter::~GmPhysicalVolumeFilter()
{ 
  theVPhysicalVolumes.clear();
}

G4bool GmPhysicalVolumeFilter::AcceptStep(const G4Step* aStep)
{
  if( theVPhysicalVolumes.find(aStep->GetTrack()->GetVolume()) != theVPhysicalVolumes.end() ) {
    return TRUE;
  }
  return FALSE;
}

//----------------------------------------------------------------                   
G4bool GmPhysicalVolumeFilter::AcceptTrack(const G4Track* aTrack)
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), touch, false );
  G4VPhysicalVolume* pv = touch->GetVolume();

  if( theVPhysicalVolumes.find(pv) != theVPhysicalVolumes.end() ) {
    return TRUE;
  }
  return FALSE;
}

//----------------------------------------------------------------                   
G4bool GmPhysicalVolumeFilter::AcceptStackedTrack(const G4Track* aTrack)
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetPosition(), touch, false );
  G4VPhysicalVolume* pv = touch->GetVolume();

  if( theVPhysicalVolumes.find(pv) != theVPhysicalVolumes.end() ) {
    return TRUE;
  }
  return FALSE;
}


//----------------------------------------------------------------                   
void GmPhysicalVolumeFilter::show()
{
  G4cout << "----G4VPhysicalVolumeFilter volume list------"<<G4endl;
  std::set<G4VPhysicalVolume*>::const_iterator iter;
  for ( iter = theVPhysicalVolumes.begin(); iter != theVPhysicalVolumes.end(); iter++ ){
    G4cout << (*iter)->GetName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//----------------------------------------------------------------                   
void GmPhysicalVolumeFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPhysicalVolumeFilter::SetParameters","There should be at least one parameter: LOGICAL_VOLUME_NAME",FatalErrorInArgument,"There is no parameter");
  }

  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4VPhysicalVolume*> volumes = GmGeometryUtils::GetInstance()->GetPhysicalVolumes( params[ii] );
    for(unsigned int jj = 0; jj < volumes.size(); jj++ ){
      theVPhysicalVolumes.insert( volumes[jj] );
    }
  }
}
