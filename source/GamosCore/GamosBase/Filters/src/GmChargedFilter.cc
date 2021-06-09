#include "GmChargedFilter.hh"
#include "G4Track.hh"

GmChargedFilter::GmChargedFilter(G4String name)
  :GmVFilter(name)
{;}

GmChargedFilter::~GmChargedFilter()
{;}

G4bool GmChargedFilter::AcceptTrack(const G4Track* aTrack)
{
  if (aTrack->GetDefinition()->GetPDGCharge() != 0. ) return TRUE;
  return FALSE;
}
