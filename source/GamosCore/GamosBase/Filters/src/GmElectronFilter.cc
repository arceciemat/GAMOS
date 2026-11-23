#include "GmElectronFilter.hh"
#include "G4Track.hh"
#include "G4Electron.hh"

GmElectronFilter::GmElectronFilter(G4String name)
  :GmVFilter(name)
{;}

GmElectronFilter::~GmElectronFilter()
{;}

G4bool GmElectronFilter::AcceptTrack(const G4Track* aTrack)
{
  G4ParticleDefinition* part = aTrack->GetDefinition();
  if( part == G4Electron::Electron() ) return TRUE;
  return FALSE;
}
