#include "GmEMParticleFilter.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

GmEMParticleFilter::GmEMParticleFilter(G4String name)
  :GmVFilter(name)
{;}

GmEMParticleFilter::~GmEMParticleFilter()
{;}

G4bool GmEMParticleFilter::AcceptTrack(const G4Track* aTrack)
{
  G4ParticleDefinition* part = aTrack->GetDefinition();
  if( part == G4Gamma::Gamma() 
      || part == G4Electron::Electron()
      || part == G4Positron::Positron() ) return TRUE;
  return FALSE;
}
