#include "GmPSdx_LET_dEdx_unrestr.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"

GmPSdx_LET_dEdx_unrestr::GmPSdx_LET_dEdx_unrestr(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV");
  theEmCalculator = new G4EmCalculator;
  G4EmParameters::Instance()->SetBuildCSDARange(true); // to build G4VEnergyLossProcess::theDEDXunRestrictedTable
}

GmPSdx_LET_dEdx_unrestr::~GmPSdx_LET_dEdx_unrestr()
{;}

G4bool GmPSdx_LET_dEdx_unrestr::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  
  G4double let = 0.;

  G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  if( procType == fTransportation || procType == fElectromagnetic ) {
    let = GetElectronicDEDX( aStep, false);
  }
  
  G4double stepLen = aStep->GetStepLength();
  G4double dxlet = stepLen*let;
  
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << "  GmPSdx_LET_dEdx_unrestr::ProcessHits dxlet " << dxlet
	   << " let " << let
	   << " stepLength " << stepLen
	   << G4endl;
    G4cout << aStep->GetTrack()->GetKineticEnergy() << "  GmPSdx_LET_dEdx_unrestr::ProcessHits let TEST " << (1.-let/(aStep->GetTotalEnergyDeposit()/aStep->GetStepLength())) << " " << let << " = " << aStep->GetTotalEnergyDeposit()/aStep->GetStepLength() << G4endl;
  }
#endif
  
  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, dxlet, weight );
  
  return TRUE;
} 

