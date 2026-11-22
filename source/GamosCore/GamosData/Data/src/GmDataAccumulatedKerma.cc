#include "GmDataAccumulatedKerma.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

//----------------------------------------------------------------
GmDataAccumulatedKerma::GmDataAccumulatedKerma()
{
  bInitial = false;
  theHMax = 1.;

  theExcludedTypes.clear(); // delete DTRun
  theExcludedTypes.insert(DTSeco);

  theGeomUtils= GmGeometryUtils::GetInstance();
}

//----------------------------------------------------------------
GmDataAccumulatedKerma::~GmDataAccumulatedKerma()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedKerma::Accumulate( const G4Step* aStep, G4int index )
{
  if( aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0 ) return;
  G4TrackVector secondaryTracks = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  G4double eseco = 0.;
  for( ite = secondaryTracks.begin(); ite != secondaryTracks.end(); ite++ ){
    G4Track* secoTrack = *ite;
    if( secoTrack->GetDefinition()->GetPDGCharge() != 0 ){
      eseco += secoTrack->GetKineticEnergy();
    }
  }
  if ( eseco == 0. ) return;

  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  theDataAccumulated[index] += eseco/ ( density * theGeomUtils->GetCubicVolume( aStep->GetTrack()->GetVolume() ));
}

//----------------------------------------------------------------
G4double GmDataAccumulatedKerma::GetValueFromStep( const G4Step* aStep, G4int )
{
  if( aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0 ) return 0.;
  G4TrackVector secondaryTracks = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  G4double eseco = 0.;
  for( ite = secondaryTracks.begin(); ite != secondaryTracks.end(); ite++ ){
    G4Track* secoTrack = *ite;
    if( secoTrack->GetDefinition()->GetPDGCharge() != 0 ){
      eseco += secoTrack->GetKineticEnergy();
    }
  }
  if ( eseco == 0. ) return 0.;

  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  return eseco/ ( density * theGeomUtils->GetCubicVolume( aStep->GetTrack()->GetVolume() ));
}
