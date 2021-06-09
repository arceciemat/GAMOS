#include "GmDataSumSecoKineticEnergy.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataSumSecoKineticEnergy::GmDataSumSecoKineticEnergy()
{
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataSumSecoKineticEnergy::~GmDataSumSecoKineticEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataSumSecoKineticEnergy::GetValueFromStep( const G4Step* , G4int )
{
  std::vector<G4Track*> secos = GmG4Utils::GetSecondariesOfCurrentStep();
  double sumseco = 0.;
  for( unsigned int ii = 0; ii < secos.size(); ii++ ) {
    sumseco += secos[ii]->GetKineticEnergy();
  }
  return sumseco;
}

//----------------------------------------------------------------
G4double GmDataSumSecoKineticEnergy::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  const G4TrackVector* secos = aTrack->GetStep()->GetSecondary(); // secondaries of G4step are of all track 
  double sumseco = 0.;
  for( unsigned int ii = 0; ii < secos->size(); ii++ ) {
    sumseco += (*secos)[ii]->GetKineticEnergy();
  }
  return sumseco;

}

//----------------------------------------------------------------
G4double GmDataSumSecoKineticEnergy::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetKineticEnergy();
}

