#include "GmDataVertexDirPerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexDirPerp::GmDataVertexDirPerp()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexDirPerp::~GmDataVertexDirPerp()
{
}


//----------------------------------------------------------------
G4double GmDataVertexDirPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexMomentumDirection().perp();
}

//----------------------------------------------------------------
G4double GmDataVertexDirPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexMomentumDirection().perp();
}

