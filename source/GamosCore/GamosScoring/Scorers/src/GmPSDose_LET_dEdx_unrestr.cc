#include "GmPSDose_LET_dEdx_unrestr.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4EmParameters.hh"

#include "G4ProcessType.hh"
#include "G4VProcess.hh"

GmPSDose_LET_dEdx_unrestr::GmPSDose_LET_dEdx_unrestr(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
  theEmCalculator = new G4EmCalculator;
  G4EmParameters::Instance()->SetBuildCSDARange(true); // to build G4VEnergyLossProcess::theDEDXunRestrictedTable
}

GmPSDose_LET_dEdx_unrestr::~GmPSDose_LET_dEdx_unrestr()
{;}

G4bool GmPSDose_LET_dEdx_unrestr::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4double let = 0.;
  //  G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  //  if( procType == fTransportation || procType == fElectromagnetic ) {
    let = GetElectronicDEDX( aStep, false);
    // }

  G4double eSeco = GetSecondaryElectronEnergy(aStep); // secondary energy of electrons
  G4double eeLoss = edep + eSeco;
  G4double doselet = eeLoss*let;
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << "  GmPSDose_LET_dEdx_unrestr::ProcessHits doselet " << doselet
	   << " let " << let
	   << " doselet " << doselet
	   << " eSeco " << eSeco
	   << " eeLoss " << eeLoss
	   << G4endl;
}
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, doselet, weight );
  
  return TRUE;
} 

