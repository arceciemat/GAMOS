#include "GmDataVertexDirTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexDirTheta::GmDataVertexDirTheta()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexDirTheta::~GmDataVertexDirTheta()
{
}


//----------------------------------------------------------------
G4double GmDataVertexDirTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataVertexDirTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().theta()/CLHEP::deg;
}

