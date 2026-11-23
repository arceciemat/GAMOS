#include "GmPrimaryFilter.hh"
#include "G4Track.hh"

GmPrimaryFilter::GmPrimaryFilter(G4String name)
  :GmVFilter(name)
{
;}

GmPrimaryFilter::~GmPrimaryFilter()
{;}

G4bool GmPrimaryFilter::AcceptTrack(const G4Track* aTrack)
{
  if (aTrack->GetParentID() == 0 ) return TRUE;
  return FALSE;

}
