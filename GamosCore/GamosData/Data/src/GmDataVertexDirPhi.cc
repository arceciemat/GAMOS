#include "GmDataVertexDirPhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexDirPhi::GmDataVertexDirPhi()
{
  bInitial = true;
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexDirPhi::~GmDataVertexDirPhi()
{
}


//----------------------------------------------------------------
G4double GmDataVertexDirPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexMomentumDirection().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataVertexDirPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().phi()/CLHEP::deg;
}

