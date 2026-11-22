#include "GmDataVertexDirZ.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexDirZ::GmDataVertexDirZ()
{
  bInitial = true;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexDirZ::~GmDataVertexDirZ()
{
}


//----------------------------------------------------------------
G4double GmDataVertexDirZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexMomentumDirection().z();
}

//----------------------------------------------------------------
G4double GmDataVertexDirZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().z();
}

