#include "GmDataVertexKineticEnergy.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataVertexKineticEnergy::GmDataVertexKineticEnergy()
{
  bInitial = true;
  theHMax = 1.;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataVertexKineticEnergy::~GmDataVertexKineticEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataVertexKineticEnergy::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetVertexKineticEnergy();
}

//----------------------------------------------------------------
G4double GmDataVertexKineticEnergy::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexKineticEnergy();
}

