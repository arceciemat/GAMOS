#include "GmSetG4RadDecayTime0UA.hh"
#include "SHRadDecayChainSet.hh"
#include "ShieldingVerbosity.hh"

#include "GamosCore/GamosGenerator/include/GmGenerDistTimeDecay.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosAnalysis/include/GmCheckOriginalGamma.hh"

#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4RadioactiveDecay.hh"
#include "G4RadioactiveDecayBase.hh"

//---------------------------------------------------------------------------
GmSetG4RadDecayTime0UA::GmSetG4RadDecayTime0UA()
{

#ifndef WIN32
  GmCheckOriginalGamma::SetCheckParentID(false);
#else 
  GmCheckOriginalGamma cog; 
  cog.SetCheckParentID(false);
#endif
}

//---------------------------------------------------------------------------
void GmSetG4RadDecayTime0UA::UserSteppingAction(const G4Step* aStep)
{
  const G4Track* aTrack = aStep->GetTrack();
  
  //  const G4RadioactiveDecay* procDec = dynamic_cast<const G4RadioactiveDecay*>(aTrack->GetCreatorProcess());
  const G4RadioactiveDecay* procDec = dynamic_cast<const G4RadioactiveDecay*>(aStep->GetPostStepPoint()->GetProcessDefinedStep());
  //  G4cout << " GmSetG4RadDecayTime0UA::UserSteppingAction Creator rad " << procDec << G4endl; //GDEB
  const G4RadioactiveDecayBase* procDecN;
  if (procDec == 0) {
    procDecN = dynamic_cast<const G4RadioactiveDecayBase*>(aStep->GetPostStepPoint()->GetProcessDefinedStep());
  }
  if ( procDec == 0 && procDecN == 0) return; // only when radioactive decay happens
    
  //---- Set time of track, as if G4RadioactiveDecay did not change it
  SetTime(aStep);
  //  G4cout << " GmSetG4RadDecayTime0UA::UserSteppingAction(SetTime " << aStep->GetTrack()->GetGlobalTime() << G4endl; //GDEB
  
  //--- Set the secondary time as the primary one
  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  for (size_t ii = 0; ii < secos.size(); ii++) {
    secos[ii]->SetGlobalTime(aTrack->GetGlobalTime());
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << " GmSetG4RadDecayTime0UA::UserSteppingAction(Set secos " << secos[ii]->GetGlobalTime() << G4endl; //GDEB
#endif
  }
    
}

//---------------------------------------------------------------------------
void GmSetG4RadDecayTime0UA::SetTime(const G4Step* aStep)
{
  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  G4StepPoint* preSP = aStep->GetPreStepPoint();
  G4StepPoint* postSP = aStep->GetPostStepPoint();
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " GmSetG4RadDecayTime0UA::SetTime  DEC TIME pre " << preSP->GetGlobalTime() << " post " << postSP->GetGlobalTime() << G4endl;
#endif
	
  G4double velocity = preSP->GetVelocity();
  G4double timeChange = 0.;
  if (velocity != 0) timeChange = aStep->GetStepLength() / preSP->GetVelocity();
  G4double gtime = preSP->GetGlobalTime();
  //  preSP->SetGlobalTime(gtime);
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " GmSetG4RadDecayTime0UA::SetTime " << gtime << " " << timeChange << G4endl; //GDEB
#endif
  gtime += timeChange;
  postSP->SetGlobalTime(gtime);
  aTrack->SetGlobalTime(gtime);
  G4double ltime = preSP->GetLocalTime();
  preSP->SetLocalTime(ltime);
  ltime += timeChange;
  postSP->SetLocalTime(ltime);
  aTrack->SetLocalTime(gtime);
  
}
