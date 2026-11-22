#include "G4EventManager.hh"

#include "GmTrajectoryPosMom.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "GmTrajPointPosMom.hh"


//----------------------------------------------------------------------
GmTrajectoryPosMom::GmTrajectoryPosMom() : GmTrajectory()
{;}

//----------------------------------------------------------------------
GmTrajectoryPosMom::GmTrajectoryPosMom( const G4Track* aTrack) : GmTrajectory(aTrack)
{
}

//----------------------------------------------------------------------
GmTrajectoryPosMom::~GmTrajectoryPosMom()
{
  Clear();
}

//----------------------------------------------------------------------
void GmTrajectoryPosMom::Clear()
{
 fTrackID = 0;
 fParentID = 0;
 fPDGEncoding = 0;
 fPDGCharge = 0.;
 fParticleName = "";
 fInitialMomentum = G4ThreeVector();

 std::vector<GmTrajPointPosMom*>::iterator ite;
 for( ite = thePoints.begin(); ite != thePoints.end(); ite++ ){
   delete *ite;
 }

 thePoints.clear();

}
