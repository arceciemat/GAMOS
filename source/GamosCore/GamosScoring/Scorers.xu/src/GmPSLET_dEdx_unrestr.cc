#include "GmPSLET_dEdx_unrestr.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"
#include "G4EmParameters.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"


GmPSLET_dEdx_unrestr::GmPSLET_dEdx_unrestr(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
  theEmCalculator = new G4EmCalculator;
  G4EmParameters::Instance()->SetBuildCSDARange(true); // to build G4VEnergyLossProcess::theDEDXunRestrictedTable
}

GmPSLET_dEdx_unrestr::~GmPSLET_dEdx_unrestr()
{;}

G4bool GmPSLET_dEdx_unrestr::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4double let = 0.;
  G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  if( procType == fTransportation || procType == fElectromagnetic ) {
    /*  G4cout << " TLET_R " << GetDEDX( aStep, true) << G4endl; //GDEB
    G4cout << " TLET_U " << GetDEDX( aStep, false) << G4endl; //GDEB
    G4cout << " TLETElec_R " << GetElectronicDEDX( aStep, true) << G4endl; //GDEB
    G4cout << " TLETElec_U " << GetElectronicDEDX( aStep, false) << G4endl; //GDEB
    if( GetDEDX( aStep, true) != GetElectronicDEDX( aStep, true) ) {
       const std::vector<const G4Track*> * secondary = aStep->GetSecondaryInCurrentStep();
       size_t SecondarySize = (*secondary).size();
       G4double EnergySecondary = 0.;
       if (SecondarySize) { // calculate only secondary particles
	 for (size_t numsec = 0; numsec< SecondarySize ; numsec ++)  {
	   //Get the PDG code of every secondaty particles in current step       
	   G4int PDGSecondary=(*secondary)[numsec]->GetDefinition()->GetPDGEncoding();
	   if(PDGSecondary == 11) { // calculate only secondary electrons  
	     // calculate the energy deposit of secondary electrons in current step          
	     EnergySecondary += (*secondary)[numsec]->GetKineticEnergy();
	   }
	 }
       }
       G4cout << " DIFFTLET_R TLETElec_R " << GetDEDX( aStep, true)  - GetElectronicDEDX( aStep, true) << " ESECO " << EnergySecondary << G4endl; //GDEB
       }*/
    let = GetElectronicDEDX( aStep, false);
  }
    
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4ParticleDefinition* part =  aStep->GetTrack()->GetDefinition();
    G4cout << "  GmPSLET_dEdx_unrestr::ProcessHits let " << let
	   << " edep " << edep
	   << " Ekin " << preStepPoint->GetKineticEnergy()
	   << " mate " << preStepPoint->GetMaterial()->GetName()
	   << " part " << part->GetParticleName() 
	   << G4endl;
    //    G4cout << aStep->GetTrack()->GetKineticEnergy() << "  GmPSDose_LET_dEdx::ProcessHits let TEST " << (1.-let/(aStep->GetTotalEnergyDeposit()/aStep->GetStepLength())) << " " << let << " = " << aStep->GetTotalEnergyDeposit()/aStep->GetStepLength() << G4endl;
}
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, let, weight );
  
  return TRUE;
} 

