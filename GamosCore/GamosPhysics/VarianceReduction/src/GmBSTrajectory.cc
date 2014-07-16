#include "G4EventManager.hh"

#include "GmBSTrajectory.hh"

//----------------------------------------------------------------------
GmBSTrajectory::GmBSTrajectory() : GmVTrajectory()
{}

//----------------------------------------------------------------------
GmBSTrajectory::GmBSTrajectory( const G4Track* aTrack) : GmVTrajectory(aTrack)
{
  fInitialPosition = aTrack->GetPosition();
  fEnergy = aTrack->GetKineticEnergy();

}

//----------------------------------------------------------------------
GmBSTrajectory::~GmBSTrajectory()
{
  Clear();
}

//----------------------------------------------------------------------
void GmBSTrajectory::Clear()
{
  fInitialPosition = G4ThreeVector();
  fEnergy = 0;

}
