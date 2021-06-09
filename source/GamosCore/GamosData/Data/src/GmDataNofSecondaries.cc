#include "GmDataNofSecondaries.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataNofSecondaries::GmDataNofSecondaries()
{
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataNofSecondaries::~GmDataNofSecondaries()
{
}


//----------------------------------------------------------------
G4double GmDataNofSecondaries::GetValueFromStep( const G4Step* , G4int )
{
  std::vector<G4Track*> secos = GmG4Utils::GetSecondariesOfCurrentStep();
  return secos.size();
}

//----------------------------------------------------------------
G4double GmDataNofSecondaries::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  const G4TrackVector* secos = aTrack->GetStep()->GetSecondary(); // secondaries of G4step are of all track 
  return secos->size();

}

