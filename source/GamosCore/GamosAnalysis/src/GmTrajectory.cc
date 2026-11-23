#include "G4EventManager.hh"

#include "GmTrajectory.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "GmTrajPoint.hh"

//----------------------------------------------------------------------
GmTrajectory::GmTrajectory() : GmVTrajectory()
{}

//----------------------------------------------------------------------
GmTrajectory::GmTrajectory( const G4Track* aTrack) : GmVTrajectory(aTrack)
{
}

//----------------------------------------------------------------------
GmTrajectory::~GmTrajectory()
{
  Clear();
}

//----------------------------------------------------------------------
void GmTrajectory::Clear()
{
 fTrackID = 0;
 fParentID = 0;
 fPDGEncoding = 0;
 fPDGCharge = 0.;
 fParticleName = "";
 fInitialMomentum = G4ThreeVector();

 std::vector<GmTrajPoint*>::iterator ite;
 for( ite = thePoints.begin(); ite != thePoints.end(); ite++ ){
   delete *ite;
 }

 thePoints.clear();

}
