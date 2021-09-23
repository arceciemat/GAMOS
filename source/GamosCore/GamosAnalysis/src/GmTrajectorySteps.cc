#include "G4EventManager.hh"

#include "GmTrajectorySteps.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "GmTrajStep.hh"


//----------------------------------------------------------------------
GmTrajectorySteps::GmTrajectorySteps() : GmVTrajectory()
{;}

//----------------------------------------------------------------------
GmTrajectorySteps::GmTrajectorySteps( const G4Track* aTrack): GmVTrajectory(aTrack)
{
}

//----------------------------------------------------------------------
GmTrajectorySteps::~GmTrajectorySteps()
{
  Clear();
}

//----------------------------------------------------------------------
void GmTrajectorySteps::Clear()
{
 fTrackID = 0;
 fParentID = 0;
 fPDGEncoding = 0;
 fPDGCharge = 0.;
 fParticleName = "";
 fInitialMomentum = G4ThreeVector();

 std::vector<GmTrajStep*>::iterator ite;
 for( ite = theSteps.begin(); ite != theSteps.end(); ite++ ){
   delete *ite;
 }

 theSteps.clear();

}
