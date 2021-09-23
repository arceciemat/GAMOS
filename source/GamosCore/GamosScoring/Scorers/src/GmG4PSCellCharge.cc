#include "GmG4PSCellCharge.hh"
#include "G4Track.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"


///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring cell charge.
//   The Cell Charge is defined by  a sum of deposited charge inside the cell.
//
// 
///////////////////////////////////////////////////////////////////////////////

GmG4PSCellCharge::GmG4PSCellCharge(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnitName = " [e]";
}

GmG4PSCellCharge::~GmG4PSCellCharge()
{;}

G4bool GmG4PSCellCharge::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmG4PSCellCharge::ProcessHits " << G4endl;
#endif
  if( !AcceptByFilter( aStep ) ) return false;

  // Enter or First step of primary.
  if( aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary 
      || ( aStep->GetTrack()->GetParentID() == 0 &&
	   aStep->GetTrack()->GetCurrentStepNumber() == 1 ) ){
    G4double charge = aStep->GetPreStepPoint()->GetCharge();
    if( charge != 0. ) {
      FillScorer( aStep, charge, aStep->GetPreStepPoint()->GetWeight() );
    }
  }
  
  // Exit
  if( aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary){
    G4double charge = aStep->GetPreStepPoint()->GetCharge();
    if( charge != 0. ) {
      FillScorer( aStep, -charge, aStep->GetPreStepPoint()->GetWeight() );
    } 
  }
  
  return TRUE;
}

