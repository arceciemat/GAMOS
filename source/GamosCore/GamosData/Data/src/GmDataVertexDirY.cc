#include "GmDataVertexDirY.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexDirY::GmDataVertexDirY()
{
  bInitial = true;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexDirY::~GmDataVertexDirY()
{
}


//----------------------------------------------------------------
G4double GmDataVertexDirY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexMomentumDirection().y();
}

//----------------------------------------------------------------
G4double GmDataVertexDirY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().y();
}

