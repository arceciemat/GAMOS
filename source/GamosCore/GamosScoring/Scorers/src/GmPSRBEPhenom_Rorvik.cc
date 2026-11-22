#include "GmPSRBEPhenom_Rorvik.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

//--------------------------------------------------------------------
GmPSRBEPhenom_Rorvik::GmPSRBEPhenom_Rorvik(G4String name)
  :GmVPSRBEPhenom(name)
{
  theUnit = 1.;
  theUnitName = G4String("");
  // new G4EmCalculator;
}

//--------------------------------------------------------------------
GmPSRBEPhenom_Rorvik::~GmPSRBEPhenom_Rorvik()
{;}

//--------------------------------------------------------------------
G4double GmPSRBEPhenom_Rorvik::GetRBEMin( G4double , G4double )
{

  G4double rbeMin = 1.;
  if( ScoringVerb(debugVerb) ) G4cout << " GmPSRBEPhenom_Rorvik::GetRBEMin " << rbeMin << G4endl; 
  return rbeMin;
}

//--------------------------------------------------------------------
G4double GmPSRBEPhenom_Rorvik::GetRBEMax( G4double LETd, G4double alphaBetaX )
{
  G4double rbeMax = 1. + 0.645 * LETd/alphaBetaX;
  if( ScoringVerb(debugVerb) ) G4cout << " GmPSRBEPhenom_Rorvik::GetRBEMax " << rbeMax << G4endl; 
  return rbeMax;
}

//--------------------------------------------------------------------
G4bool GmPSRBEPhenom_Rorvik::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( bOnlyEndOfEvent ) return false;
  
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event
  if( !AcceptByFilter( aStep ) ) return false;
  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4double rbe = GetRBE(aStep);

  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSRBEPhenom_Rorvik::ProcessHits rbe " << rbe 
	   << G4endl;
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, rbe, weight );
  
  return TRUE;
} 

