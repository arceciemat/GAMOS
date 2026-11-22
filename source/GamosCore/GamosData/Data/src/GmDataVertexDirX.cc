#include "GmDataVertexDirX.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexDirX::GmDataVertexDirX()
{
  bInitial = true;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexDirX::~GmDataVertexDirX()
{
}


//----------------------------------------------------------------
G4double GmDataVertexDirX::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexMomentumDirection().x();
}

//----------------------------------------------------------------
G4double GmDataVertexDirX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().x();
}

