#include "GmProtonFilter.hh"
#include "G4Track.hh"
#include "G4Proton.hh"

GmProtonFilter::GmProtonFilter(G4String name)
  :GmVFilter(name)
{;}

GmProtonFilter::~GmProtonFilter()
{;}

G4bool GmProtonFilter::AcceptTrack(const G4Track* aTrack)
{
  if (aTrack->GetDefinition() == G4Proton::Proton() ) return TRUE;
  return FALSE;
}
