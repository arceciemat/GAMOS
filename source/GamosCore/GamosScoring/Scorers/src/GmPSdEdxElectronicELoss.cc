#include "GmPSdEdxElectronicELoss.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"


GmPSdEdxElectronicELoss::GmPSdEdxElectronicELoss(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
  theEmCalculator = new G4EmCalculator;
}

GmPSdEdxElectronicELoss::~GmPSdEdxElectronicELoss()
{;}

G4bool GmPSdEdxElectronicELoss::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4double let = GetElectronicDEDX( aStep, true);
  G4double length = aStep->GetStepLength();
  G4double eSeco = GetSecondaryElectronEnergy(aStep); // secondary energy of electrons
  G4double eeLoss = let*length + eSeco;
  if ( eeLoss == 0. ) return FALSE;

  G4double weight = aStep->GetPreStepPoint()->GetWeight();
  
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << "  GmPSdEdxElectronicELoss::ProcessHits eeLoss " << eeLoss
	   << G4endl;
}
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, eeLoss, weight );
  
  return TRUE;
} 

