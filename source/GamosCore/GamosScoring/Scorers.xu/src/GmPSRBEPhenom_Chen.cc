#include "GmPSRBEPhenom_Chen.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

//--------------------------------------------------------------------
GmPSRBEPhenom_Chen::GmPSRBEPhenom_Chen(G4String name)
  : GmVPSRBEPhenom(name)
{
  theUnit = 1.;
  theUnitName = G4String("");
  // new G4EmCalculator;
}

//--------------------------------------------------------------------
GmPSRBEPhenom_Chen::~GmPSRBEPhenom_Chen()
{
}

//--------------------------------------------------------------------
G4double GmPSRBEPhenom_Chen::GetRBEMin( G4double, G4double )
{
  G4double rbeMin = 1.;
  return rbeMin;
}

//--------------------------------------------------------------------
G4double GmPSRBEPhenom_Chen::GetRBEMax( G4double LETd, G4double alphaBetaX )
{
  return GetRBEMaxAlpha( LETd, alphaBetaX );
}

//--------------------------------------------------------------------
G4double GmPSRBEPhenom_Chen::GetRBEMaxAlpha( G4double LETd, G4double alphaX )
{
  G4double rbeMax = 0.1/alphaX + ( 1 - exp(-0.0013*LETd*LETd) )/ ( alphaX*0.045*LETd );
  if( ScoringVerb(debugVerb) ) G4cout << " GmPSRBEPhenom_Chen::GetRBEMax " << rbeMax << G4endl; 
  return rbeMax;
}

//--------------------------------------------------------------------
G4bool GmPSRBEPhenom_Chen::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( bOnlyEndOfEvent ) return false;;
  
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event
  if( !AcceptByFilter( aStep ) ) return false;
  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4double rbe = GetRBE(aStep);

  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSRBEPhenom_Chen::ProcessHits rbe " << rbe 
	   << G4endl;
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, rbe, weight );
  
  return TRUE;
} 

