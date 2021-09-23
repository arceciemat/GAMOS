#include "GmDataAccumulatedLength.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedLength::GmDataAccumulatedLength()
{
  bInitial = false;
  theHMax = 10.;

  theExcludedTypes.clear(); // delete DTRun
}

//----------------------------------------------------------------
GmDataAccumulatedLength::~GmDataAccumulatedLength()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedLength::Accumulate( const G4Step* aStep, G4int index )
{
  //  G4cout << "  GmDataAccumulatedLength::Accumulate( " << G4endl; //GDEB
  theDataAccumulated[index] += aStep->GetStepLength();
}

//----------------------------------------------------------------
G4double GmDataAccumulatedLength::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetStepLength();
}

//----------------------------------------------------------------
G4double GmDataAccumulatedLength::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* , G4int )
{ 
  return GetValueFromTrack( aTrack1 );
}
