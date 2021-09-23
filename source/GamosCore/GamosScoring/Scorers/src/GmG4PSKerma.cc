#include "GmG4PSKerma.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4UnitsTable.hh"
#include "G4VSolid.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring kerma (energy of charged secondaries from neutral primaries)
///////////////////////////////////////////////////////////////////////////////

GmG4PSKerma::GmG4PSKerma(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = CLHEP::gray;
  theUnitName = G4String("Gy");

  bUseVolume = true;
}

GmG4PSKerma::~GmG4PSKerma()
{;}

G4bool GmG4PSKerma::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return FALSE;

  const G4Track* aTrack = aStep->GetTrack();
  if( aTrack->GetDefinition()->GetPDGCharge() != 0 ) return FALSE;

  G4TrackVector secondaryTracks = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  G4double eseco = 0.;
  for( ite = secondaryTracks.begin(); ite != secondaryTracks.end(); ite++ ){
    G4Track* secoTrack = *ite;
    if( secoTrack->GetDefinition()->GetPDGCharge() != 0 ){
      eseco += secoTrack->GetKineticEnergy();
    }
  }

  if ( eseco == 0. ) return FALSE;
  G4double volume = GetVolume(aStep);
  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  G4double kerma   = eseco / ( density * volume );

  FillScorer( aStep, kerma, aStep->GetPreStepPoint()->GetWeight() );

  return TRUE;
}


