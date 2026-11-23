//#define private public
#include "G4EventManager.hh"
//#define private private

#include "GmVTrajectory.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"

//----------------------------------------------------------------------
GmVTrajectory::GmVTrajectory()
:  fTrackID(0), fParentID(0),
   fPDGEncoding( 0 ), fPDGCharge(0.0), fParticleName(""),
   fInitialMomentum( G4ThreeVector() )
{;}

//----------------------------------------------------------------------
GmVTrajectory::GmVTrajectory( const G4Track* aTrack)
{
   fTrackID = aTrack->GetTrackID();
   fParentID = aTrack->GetParentID();
   G4ParticleDefinition * fpParticleDefinition = aTrack->GetDefinition();
   fParticleName = fpParticleDefinition->GetParticleName();
   fPDGCharge = fpParticleDefinition->GetPDGCharge();
   fPDGEncoding = fpParticleDefinition->GetPDGEncoding();
   fInitialMomentum = aTrack->GetMomentum();
   fVertexKineticEnergy = aTrack->GetVertexKineticEnergy();
   fVertexMomentumDirection = aTrack->GetVertexMomentumDirection();
}
