#include "GmTrajPointSD.hh"
#include "G4Step.hh"


//----------------------------------------------------------------------
GmTrajPointSD::GmTrajPointSD( const G4StepPoint* aStepPoint ) : GmTrajPoint(aStepPoint)
{

  GmVSD* sd = (GmVSD*)aStepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetSensitiveDetector();
  if( sd != 0 ){
    //    theSDType = GmSDType::GetType( sd->GetPathName() );
  }

}


//----------------------------------------------------------------------
GmTrajPointSD::~GmTrajPointSD()
{

}
