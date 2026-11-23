#include "GmNeutronFilter.hh"
#include "G4Track.hh"
#include "G4Neutron.hh"

GmNeutronFilter::GmNeutronFilter(G4String name)
  :GmVFilter(name)
{;}

GmNeutronFilter::~GmNeutronFilter()
{;}

G4bool GmNeutronFilter::AcceptTrack(const G4Track* aTrack)
{
  if (aTrack->GetDefinition() == G4Neutron::Neutron() ) return TRUE;
  return FALSE;
}
