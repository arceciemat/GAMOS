#include "G4Run.hh"
#include "G4Event.hh"

#include "Exer4UserActionB.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"


//-----------------------------------------------------------------------
Exer4UserActionB::Exer4UserActionB() : GmUserRunAction(), GmUserEventAction(), GmUserSteppingAction()
{
}


//-----------------------------------------------------------------------
void Exer4UserActionB::BeginOfRunAction( const G4Run* )
{
  G4double hisMax = GmParameterMgr::GetInstance()->GetNumericValue("Exer4UserActionB:HisMax",10.);
  theAnaMgr = GmAnalysisMgr::GetInstance("exercise");
  theAnaMgr->CreateHisto1D("E depo",100,0,hisMax,1234);
 
} 

//-----------------------------------------------------------------------
void Exer4UserActionB::BeginOfEventAction( const G4Event* )
{
  theEDepo = 0.;
}

//-----------------------------------------------------------------------
void Exer4UserActionB::UserSteppingAction(const G4Step* aStep)
{
  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if( proc == 0 ) { 
    G4cerr << " proc 0 " << G4endl;
    return;
  }
  G4String procName = proc->GetProcessName();
  if( procName.find("Ioni") != std::string::npos && aStep->GetTrack()->GetVolume()->GetName() == "Crystal" ){
    theEDepo += aStep->GetTotalEnergyDeposit();
  }

}

//-----------------------------------------------------------------------
void Exer4UserActionB::EndOfEventAction( const G4Event* evt )
{
  theAnaMgr->GetHisto1(1234)->Fill(theEDepo);
  G4cout << "Event ID " << evt->GetEventID() << " EDepo = " << theEDepo << G4endl; 
}


