#include "GmElectronOrPositronFilter.hh"
#include "G4Track.hh"
#include "G4Positron.hh"
#include "G4Electron.hh"

GmElectronOrPositronFilter::GmElectronOrPositronFilter(G4String name)
  :GmVFilter(name)
{;}

GmElectronOrPositronFilter::~GmElectronOrPositronFilter()
{;}

G4bool GmElectronOrPositronFilter::AcceptTrack(const G4Track* aTrack)
{
  G4ParticleDefinition* part = aTrack->GetDefinition();
  if( part == G4Electron::Electron()
      || part == G4Positron::Positron() ) return TRUE;
  return FALSE;
}
