#include "GmPositronFilter.hh"
#include "G4Track.hh"
#include "G4Positron.hh"

GmPositronFilter::GmPositronFilter(G4String name)
  :GmVFilter(name)
{;}

GmPositronFilter::~GmPositronFilter()
{;}

G4bool GmPositronFilter::AcceptTrack(const G4Track* aTrack)
{
  G4ParticleDefinition* part = aTrack->GetDefinition();
  if( part == G4Positron::Positron() ) return TRUE;
  return FALSE;
}
