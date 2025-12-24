// GmG4PSDoseDeposit_NoSkipEqualMate
#include "GmG4PSDoseDeposit_NoSkipEqualMate.hh"
#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

GmG4PSDoseDeposit_NoSkipEqualMate::GmG4PSDoseDeposit_NoSkipEqualMate(G4String name)
  :GmVPrimitiveScorer(name)
{
  theUnit = CLHEP::gray;
  theUnitName = G4String("Gy");

  bUseVolume = true;
}

GmG4PSDoseDeposit_NoSkipEqualMate::~GmG4PSDoseDeposit_NoSkipEqualMate()
{;}

G4bool GmG4PSDoseDeposit_NoSkipEqualMate::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double volume = GetVolume(aStep);
  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  G4double dose    = edep / ( density * volume );
  G4double weight = 1.;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "  GmG4PSDoseDeposit_NoSkipEqualMate::ProcessHits dose " << dose << " edep " << edep << " density " << density << " volume " << volume << " g= " << CLHEP::g << " joule " << CLHEP::joule << G4endl;
#endif
  if( fWeighted ) {
    weight = aStep->GetPreStepPoint()->GetWeight(); 
    //    dose *= weight; 
  }

  FillScorer( aStep, dose, weight );
  //  FillScorer( aStep, dose, aStep->GetPreStepPoint()->GetWeight());
  return TRUE;
}


