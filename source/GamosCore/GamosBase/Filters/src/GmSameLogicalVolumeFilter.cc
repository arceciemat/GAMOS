#include "GmSameLogicalVolumeFilter.hh"
#include "G4Track.hh"

GmSameLogicalVolumeFilter::GmSameLogicalVolumeFilter(G4String name)
  :GmVFilter(name)
{
;}

GmSameLogicalVolumeFilter::~GmSameLogicalVolumeFilter()
{;}

G4bool GmSameLogicalVolumeFilter::AcceptStep(const G4Step* aStep)
{
  G4VPhysicalVolume* PVpre = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4VPhysicalVolume* PVpost = aStep->GetPostStepPoint()->GetPhysicalVolume();
  if (PVpre == 0 || PVpost == 0 ) return FALSE;

  if (PVpre->GetLogicalVolume() != PVpost->GetLogicalVolume() ) return FALSE;

  return TRUE;

}

G4bool GmSameLogicalVolumeFilter::AcceptTrack(const G4Track* aTrack)
{
  G4VPhysicalVolume* PVpre = aTrack->GetVolume();
  G4VPhysicalVolume* PVpost = aTrack->GetNextVolume();

  if (PVpre == 0 || 
      PVpost == 0 ) return FALSE;

  if (PVpre->GetLogicalVolume() != PVpost->GetLogicalVolume() ) return FALSE;

  return TRUE;

}
