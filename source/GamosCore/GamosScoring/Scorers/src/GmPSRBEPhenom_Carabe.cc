#include "GmPSRBEPhenom_Carabe.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

//--------------------------------------------------------------------
GmPSRBEPhenom_Carabe::GmPSRBEPhenom_Carabe(G4String name)
  :GmVPrimitiveScorer(name),GmComputeRBEPhenom(name)
{
  theUnit = 1.;
  theUnitName = G4String("");
  // new G4EmCalculator;
}

//--------------------------------------------------------------------
GmPSRBEPhenom_Carabe::~GmPSRBEPhenom_Carabe()
{;}

//--------------------------------------------------------------------
G4double GmPSRBEPhenom_Carabe::GetRBEMin( G4double LETd, G4double alphaBetaX )
{

  G4double rbeMin = 1.09 + 0.01612 * LETd/alphaBetaX;
  if( ScoringVerb(debugVerb) ) G4cout << " GmPSRBEPhenom_Carabe::GetRBEMin " << rbeMin << G4endl; 
  return rbeMin;
}

//--------------------------------------------------------------------
G4double GmPSRBEPhenom_Carabe::GetRBEMax( G4double LETd, G4double alphaBetaX )
{
  G4double rbeMax = 0.843 + 0.413644 * LETd/alphaBetaX;
  if( ScoringVerb(debugVerb) ) G4cout << " GmPSRBEPhenom_Carabe::GetRBEMax " << rbeMax << G4endl; 
  return rbeMax;
}

//--------------------------------------------------------------------
G4bool GmPSRBEPhenom_Carabe::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event
  if( !AcceptByFilter( aStep ) ) return false;
  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4double rbe = GetRBE(aStep);

  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSRBEPhenom_Carabe::ProcessHits rbe " << rbe 
	   << G4endl;
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, rbe, weight );
  
  return TRUE;
} 

