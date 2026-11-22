#include "GmTrajPointProcess.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"


//----------------------------------------------------------------------
GmTrajPointProcess::GmTrajPointProcess( const G4StepPoint* aStepPoint )
{
  if( !aStepPoint->GetProcessDefinedStep() ) {
    theProcessName = "NULL";
  } else {
    theProcessName = aStepPoint->GetProcessDefinedStep()->GetProcessName();
    //    G4cout << " GmTrajPointProcess theProcessName " <<theProcessName << G4endl; 
  }
}


//----------------------------------------------------------------------
GmTrajPointProcess::~GmTrajPointProcess()
{

}


//----------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const GmTrajPointProcess & tp)
{
  os << "GmTrajPointProcess: ProcessName= " << tp.GetProcessName()
     << G4endl;
  return os;

}
