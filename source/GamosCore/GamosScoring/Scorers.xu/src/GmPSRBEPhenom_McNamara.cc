#include "GmPSRBEPhenom_McNamara.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

//--------------------------------------------------------------------
GmPSRBEPhenom_McNamara::GmPSRBEPhenom_McNamara(G4String name)
  :GmVPSRBEPhenom(name)
{
  theUnit = 1.;
  theUnitName = G4String("");
  // new G4EmCalculator;
}

//--------------------------------------------------------------------
GmPSRBEPhenom_McNamara::~GmPSRBEPhenom_McNamara()
{;}

//--------------------------------------------------------------------
G4double GmPSRBEPhenom_McNamara::GetRBEMin( G4double LETd, G4double alphaBetaX )
{

  G4double rbeMin = 1.1012 - 0.0039*sqrt(alphaBetaX)*LETd;
  if( ScoringVerb(debugVerb) ) G4cout << " GmPSRBEPhenom_McNamara::GetRBEMin " << rbeMin << G4endl; 
  return rbeMin;
}

//--------------------------------------------------------------------
G4double GmPSRBEPhenom_McNamara::GetRBEMax( G4double LETd, G4double alphaBetaX )
{
  G4double rbeMax = 0.99604 + 0.35605 * LETd/alphaBetaX;
  if( ScoringVerb(debugVerb) ) G4cout << " GmPSRBEPhenom_McNamara::GetRBEMax " << rbeMax << G4endl; 
  return rbeMax;
}

//--------------------------------------------------------------------
G4bool GmPSRBEPhenom_McNamara::ProcessHits(G4Step* aStep,G4TouchableHistory*)
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
    G4cout << "  GmPSRBEPhenom_McNamara::ProcessHits rbe " << rbe 
	   << G4endl;
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, rbe, weight );
  
  return TRUE;
} 

