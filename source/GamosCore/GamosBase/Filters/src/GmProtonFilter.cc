#include "GmProtonFilter.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "G4Track.hh"
#include "G4Proton.hh"

GmProtonFilter::GmProtonFilter(G4String name)
  :GmVFilter(name)
{;}

GmProtonFilter::~GmProtonFilter()
{;}

G4bool GmProtonFilter::AcceptTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmProtonFilter::AcceptTrack " << aTrack->GetDefinition()->GetParticleName() << G4endl;
#endif
  if (aTrack->GetDefinition() == G4Proton::Proton() ) return TRUE;
  return FALSE;
}
