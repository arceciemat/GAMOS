#include "GmSecondaryFilter.hh"
#include "G4Track.hh"

GmSecondaryFilter::GmSecondaryFilter(G4String name)
  :GmVFilter(name)
{;}

GmSecondaryFilter::~GmSecondaryFilter()
{;}

G4bool GmSecondaryFilter::AcceptTrack(const G4Track* aTrack)
{
  if (aTrack->GetParentID() != 0 ) return TRUE;
  return FALSE;
}
