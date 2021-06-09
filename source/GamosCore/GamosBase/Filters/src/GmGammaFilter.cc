#include "GmGammaFilter.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"

GmGammaFilter::GmGammaFilter(G4String name)
  :GmVFilter(name)
{;}

GmGammaFilter::~GmGammaFilter()
{;}

G4bool GmGammaFilter::AcceptTrack(const G4Track* aTrack)
{
  if (aTrack->GetDefinition() == G4Gamma::Gamma() ) return TRUE;
  return FALSE;
}
