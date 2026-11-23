#include "GmPSLET_LETESpect.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLogLog.hh"

#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4MaterialTable.hh"
#include "G4Proton.hh"

//-----------------------------------------------------------------
GmPSLET_LETESpect::GmPSLET_LETESpect(G4String name)
     :GmPSLET_ESpect(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm*MeV/mm");
}

//-----------------------------------------------------------------
GmPSLET_LETESpect::~GmPSLET_LETESpect()
{;}

//-----------------------------------------------------------------
G4bool GmPSLET_LETESpect::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  if( aStep->GetTrack()->GetDefinition() != G4Proton::Proton() ) {
    G4Exception("GmPSLET_LETESpect::processHits",
		"",
		JustWarning,
		("LET can only be calculated for protons, while particle is " +  aStep->GetTrack()->GetDefinition()->GetParticleName() + " , please set the appropiate filter for your scorer " + GetName()).c_str());
  }
  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4double let = GetLETFromEnergy( aStep );
  let *= let;  
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSLET_LETESpect::ProcessHits let " << let
	   << G4endl;
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, let, weight );
  
  return TRUE;
} 

