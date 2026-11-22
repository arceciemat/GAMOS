#include "GmTrajPointPosMom.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"


//----------------------------------------------------------------------
GmTrajPointPosMom::GmTrajPointPosMom( const G4StepPoint* aStepPoint )
{
  thePos = aStepPoint->GetPosition();
  theMom = aStepPoint->GetMomentum();
  theKineticEnergy = aStepPoint->GetKineticEnergy();
  theTime = aStepPoint->GetProperTime();
}


//----------------------------------------------------------------------
GmTrajPointPosMom::~GmTrajPointPosMom()
{
}

//----------------------------------------------------------------------
G4ThreeVector GmTrajPointPosMom::GetMomentumDirection() const 
{ 
  G4double mommag = theMom.mag();

  if(mommag != 0. ) {
    return theMom/theMom.mag(); 
  } else {
    return G4ThreeVector();
  }

}
//----------------------------------------------------------------------
std::ostream & operator<<(std::ostream & os, const GmTrajPointPosMom & tp)
{
  os << "GmTrajPointPosMom: "
     << " pos= " << tp.GetPosition() 
     << " mom= " << tp.GetMomentum() 
     << " kinEner= " << tp.GetKineticEnergy()
     << " time= " << tp.GetTime() << G4endl;

  return os;
}
