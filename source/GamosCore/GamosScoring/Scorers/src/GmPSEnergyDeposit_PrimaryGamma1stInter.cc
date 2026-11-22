#include "GmPSEnergyDeposit_PrimaryGamma1stInter.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4Gamma.hh"
#include "G4Event.hh"

//----------------------------------------------------------------------
GmPSEnergyDeposit_PrimaryGamma1stInter::GmPSEnergyDeposit_PrimaryGamma1stInter(G4String name)
     :GmPSEnergyDeposit_VPrimaryGamma1stInter(name)
{
}

//----------------------------------------------------------------------
GmPSEnergyDeposit_PrimaryGamma1stInter::~GmPSEnergyDeposit_PrimaryGamma1stInter()
{}

//----------------------------------------------------------------------
G4bool GmPSEnergyDeposit_PrimaryGamma1stInter::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  CheckProcessInCavity( aStep );

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4Track* aTrack = aStep->GetTrack();
  if( !IsFromGoodGamma( aTrack ) ) return false;
  //   if( theExitingState == ESExited ) return false;
  //  if( theDistanceToBorder == DBL_MAX ) return false;

  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSEnergyDeposit_PrimaryGamma1stInter::ProcessHits edep " << edep 
	   << G4endl;
#endif

 // use index from classifier, unless you are skipping borders of equal material voxels in G4RegularNavigation
  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, edep, weight );
  
  return TRUE;
} 
									       

