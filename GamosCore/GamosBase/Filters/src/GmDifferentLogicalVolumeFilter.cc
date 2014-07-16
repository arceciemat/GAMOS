#include "GmDifferentLogicalVolumeFilter.hh"
#include "G4Track.hh"

GmDifferentLogicalVolumeFilter::GmDifferentLogicalVolumeFilter(G4String name)
  :GmVFilter(name)
{
;}

GmDifferentLogicalVolumeFilter::~GmDifferentLogicalVolumeFilter()
{;}

G4bool GmDifferentLogicalVolumeFilter::AcceptStep(const G4Step* aStep)
{
  G4VPhysicalVolume* PVpre = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4VPhysicalVolume* PVpost = aStep->GetPostStepPoint()->GetPhysicalVolume();

  if( (PVpre == 0 && PVpost != 0 ) || 
      (PVpre != 0 && PVpost == 0 ) ) return FALSE;

  if (PVpre->GetLogicalVolume() == PVpost->GetLogicalVolume() ) return FALSE;

  return TRUE;

}

G4bool GmDifferentLogicalVolumeFilter::AcceptTrack(const G4Track* aTrack)
{
  G4VPhysicalVolume* PVpre = aTrack->GetVolume();
  G4VPhysicalVolume* PVpost = aTrack->GetNextVolume();

  if( (PVpre == 0 && PVpost != 0 ) || 
      (PVpre != 0 && PVpost == 0 ) ) return FALSE;

  if (PVpre->GetLogicalVolume() == PVpost->GetLogicalVolume() ) return FALSE;

  return TRUE;

}
