#include "GmTrajPointSeco.hh"

#include "G4EventManager.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

//----------------------------------------------------------------------
GmTrajPointSeco::GmTrajPointSeco( const G4StepPoint* ) 
{
  G4TrackVector fSec = GmG4Utils::GetSecondariesOfCurrentStep();
  theEnergyOfSecondaries = 0.;
  for(unsigned int ii = 0; ii < fSec.size(); ii++ ) {
    theEnergyOfSecondaries += fSec[ii]->GetKineticEnergy();
  }

}


//----------------------------------------------------------------------
GmTrajPointSeco::~GmTrajPointSeco()
{

}
