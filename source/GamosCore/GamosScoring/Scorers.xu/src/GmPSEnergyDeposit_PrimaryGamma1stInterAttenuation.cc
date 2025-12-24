#include "GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4Gamma.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

//----------------------------------------------------------------------
GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation(G4String name)
     :GmPSEnergyDeposit_VPrimaryGamma1stInter(name)
{
  theProbAtt = 0.;
  bComputeProbAtt = true;
}

//----------------------------------------------------------------------
GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::~GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation()
{}

//----------------------------------------------------------------------
G4bool GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event
  //  G4cout << " 1 GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::ProcessHits " << G4endl; //GDEB

  CheckProcessInCavity( aStep );
  //  G4cout << " 2 GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::ProcessHits " << G4endl; //GDEB

  if( !AcceptByFilter( aStep ) ) return false;
  //  G4cout << " 3 GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::ProcessHits " << G4endl; //GDEB

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  //  G4cout << " 4 GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::ProcessHits " << G4endl; //GDEB

  // ONLY FOR PRIMARY GAMMAS CHAIN (NO IF IN ANCESTOR CHAIN THERE IS A GAMMA NON-PRIMARY, OR IF THERE IS NO GAMMA
  G4Track* aTrack = aStep->GetTrack();
  if( !IsFromGoodGamma( aTrack ) ) return false;
  //  G4cout << " 5 GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::ProcessHits " << G4endl; //GDEB

  //  if( theExitingState == ESExited ) return false;
  //  G4cout << " 6 GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::ProcessHits " << G4endl; //GDEB
  //  if( theDistanceToBorder == DBL_MAX ) return false;
  //  G4cout << " 7 GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::ProcessHits " << G4endl; //GDEB
  
  edep *= theProbAtt;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::ProcessHits  edep scaled " << edep/theProbAtt << " -> " << edep << " : " << theProbAtt << G4endl;
#endif
  
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 

 // use index from classifier, unless you are skipping borders of equal material voxels in G4RegularNavigation
  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, edep, weight );

  //  G4cout << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID() << " theExitingState " << theExitingState << " probAtt= " << theProbAtt << " E=" << thePrimaryGammaVtxEnergy << " step= " << theStepLength << " " << " stepTotal= " << theStepLengthTotal << " distToBorder= " << theDistanceToBorder << G4endl; //GDEB
  return TRUE;
} 

