#include "GmTrajPoint.hh"

#include "G4EventManager.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

//----------------------------------------------------------------------
GmTrajPoint::GmTrajPoint( const G4StepPoint* aStepPoint ) : GmTrajPointProcess(aStepPoint), GmTrajPointPosMom(aStepPoint), GmTrajPointSeco(aStepPoint)
{
}


//----------------------------------------------------------------------
GmTrajPoint::~GmTrajPoint()
{
}
