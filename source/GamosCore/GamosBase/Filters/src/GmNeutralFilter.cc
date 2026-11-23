#include "GmNeutralFilter.hh"
#include "G4Track.hh"

GmNeutralFilter::GmNeutralFilter(G4String name)
  :GmVFilter(name)
{;}

GmNeutralFilter::~GmNeutralFilter()
{;}

G4bool GmNeutralFilter::AcceptTrack(const G4Track* aTrack)
{
  if (aTrack->GetDefinition()->GetPDGCharge()== 0. ) return TRUE;
  return FALSE;
}
