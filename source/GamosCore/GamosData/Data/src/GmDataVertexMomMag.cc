#include "GmDataVertexMomMag.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmDataVertexMomMag::GmDataVertexMomMag()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataVertexMomMag::~GmDataVertexMomMag()
{
}


//----------------------------------------------------------------
G4double GmDataVertexMomMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  const G4Track* aTrack = aStep->GetTrack();
  return GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() );
}

//----------------------------------------------------------------
G4double GmDataVertexMomMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return GmGenUtils::GetMomentum( aTrack->GetDefinition()->GetPDGMass(), aTrack->GetVertexKineticEnergy() );
}

