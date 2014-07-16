#include "GmWrapperProcess.hh"
#include "G4SteppingManager.hh"

GmWrapperProcess::GmWrapperProcess(const G4String& aName,
				   G4VProcess* proc, G4double efact )
  : G4WrapperProcess(aName, proc->GetProcessType()), eFactor(efact), BiasMode2(false)
{
  RegisterProcess( proc );
  G4cout << " GmWrapperProcess::GmWrapperProcess " << G4endl;
}

/*GmWrapperProcess::GmWrapperProcess(const GmWrapperProcess& right)
  : G4WrapperProcess(right), eFactor(right.eFactor), BiasMode2(right.BiasMode2)
{
}*/

GmWrapperProcess::~GmWrapperProcess()
{
}

G4double GmWrapperProcess::AlongStepGetPhysicalInteractionLength( const G4Track& track,
                                     G4double previousStepSize,
                                     G4double currentMinimumStep,
                                     G4double& proposedSafety,
                                     G4GPILSelection* selection )
{
  G4cout <<"GmWrapperProcess  ###AlongStepGetPhysicalInteractionLength  " << G4endl;  
    if (BiasMode2) 
     return (1./eFactor) * 
      pRegProcess->AlongStepGetPhysicalInteractionLength( track,previousStepSize,
  							  currentMinimumStep,proposedSafety,selection);
    else
      return DBL_MAX ;
}

G4double GmWrapperProcess::AtRestGetPhysicalInteractionLength( const G4Track& track,
                                 G4ForceCondition* condition )
{
   G4cout <<"GmWrapperProcess ###AtRestGetPhysicalInteractionLength  " << G4endl;
  return (1./eFactor) * pRegProcess->AtRestGetPhysicalInteractionLength( track,
                                  condition );
}

G4double GmWrapperProcess::PostStepGetPhysicalInteractionLength( const G4Track& track,
					  G4double   previousStepSize,
					  G4ForceCondition* condition )
{
  G4cout <<"GmWrapperProcess ###PostStepGetPhysicalInteractionLength  " << eFactor << G4endl;
   return (1./eFactor) * pRegProcess->PostStepGetPhysicalInteractionLength( track,
                                     previousStepSize,
                                     condition );
}
      

G4VParticleChange* GmWrapperProcess::PostStepDoIt(
                                                   const G4Track& track,
                                                   const G4Step&  stepData
                                                   )
{
  G4cout <<"GmWrapperProcess ###PostStepDoIt " << G4endl;
  G4VParticleChange* pChange = pRegProcess->PostStepDoIt( track, stepData );
  pChange->SetVerboseLevel(0);
  pChange->SetSecondaryWeightByProcess(true);
  //
  if (!BiasMode2) { //mode1
    G4double s = stepData.GetStepLength();
    G4double x = pRegProcess->GetCurrentInteractionLength();  
    G4double w =  pChange->GetParentWeight();
    // need to undo the change made to the parent track (/wc)
    w *= 1/wc * (1. - exp(-s/x))/(1 - exp(-eFactor*s/x)) ;
    //  G4cout << " original w = " <<  pChange->GetParentWeight() << " eF = " << eFactor << G4endl;
    // G4cout << " New weight = " << w << " wc = " << wc << " s= " << s <<" x = " << x <<  G4endl;
    ((G4ParticleChange*)pChange)->ProposeWeight(w) ;
    
    for (G4int i = 0; i < pChange->GetNumberOfSecondaries(); i++) 
      pChange->GetSecondary(i)->SetWeight(w);
  } else { // mode2
      G4double nw = pChange->GetParentWeight()*XBiasSecondaryWeight();
      ((G4ParticleChange*)pChange)->ProposeWeight(nw) ;
      for (G4int i = 0; i < pChange->GetNumberOfSecondaries(); i++) 
	pChange->GetSecondary(i)->SetWeight(nw);
      if(G4UniformRand()<XBiasSurvivalProbability()) { // need to add the primary back in to balance the weight
	// the position of the extra track should be moved to the next volume, but I don't know how yet.
	// need to change number of secondary first before adding an additional track
	pChange->SetNumberOfSecondaries(pChange->GetNumberOfSecondaries()+1);
	G4ThreeVector mDirection = track.GetDynamicParticle()->GetMomentumDirection();
	G4DynamicParticle* aD = new G4DynamicParticle((track.GetDynamicParticle())->GetDefinition(),
						      mDirection,
						      (track.GetDynamicParticle())->GetKineticEnergy()+stepData.GetTotalEnergyDeposit());    
	G4bool isGoodForTracking = true;
	((G4ParticleChange*)pChange)->
	  AddSecondary(aD,isGoodForTracking); 
	pChange->GetSecondary(pChange->GetNumberOfSecondaries()-1)->
	  SetWeight(XBiasSurvivalProbability()*track.GetWeight());

      }
  }
  return     pChange;
}

G4VParticleChange* GmWrapperProcess::AlongStepDoIt(
                                                    const G4Track& track,
                                                    const G4Step& stepData
                                                    )
{ 
  if (BiasMode2) { // mode2 same as PostStepDoIt
    G4VParticleChange* pC;
    pC = pRegProcess->AlongStepDoIt( track, stepData );
    G4double nw = pC->GetParentWeight()*XBiasSecondaryWeight();
    ((G4ParticleChange*)pC)->ProposeWeight(nw) ;
    for (G4int i = 0; i < pC->GetNumberOfSecondaries(); i++) 
      pC->GetSecondary(i)->SetWeight(nw);
    //
    if(G4UniformRand()<XBiasSurvivalProbability()) {
      pC->SetNumberOfSecondaries(pC->GetNumberOfSecondaries()+1);
      G4ThreeVector mDirection = track.GetDynamicParticle()->GetMomentumDirection();
      G4DynamicParticle* aD = new G4DynamicParticle((track.GetDynamicParticle())->GetDefinition(),
						    mDirection,
						    (track.GetDynamicParticle())->GetKineticEnergy()+stepData.GetTotalEnergyDeposit());    
      G4bool isGoodForTracking = true;
      ((G4ParticleChange*)pC)->
	AddSecondary(aD,isGoodForTracking); 
      pC->GetSecondary(pC->GetNumberOfSecondaries()-1)->
	SetWeight(XBiasSurvivalProbability()*track.GetWeight());
    }
    return pC;
  } else { // mode 1
    G4double s = stepData.GetStepLength(); 
    G4double x = pRegProcess->GetCurrentInteractionLength();
    fParticleChange.Initialize(track) ; 
    wc =  exp(-(1. - eFactor)*s/x);
    G4double w = wc * fParticleChange.GetParentWeight();
    fParticleChange.SetParentWeightByProcess(false);
    fParticleChange.ProposeParentWeight(w) ;
    //  G4cout << " wc = " << wc << G4endl;
    return &fParticleChange;
  }
}
 
G4VParticleChange* GmWrapperProcess::AtRestDoIt(
                                                 const G4Track& track,
                                                 const G4Step& stepData
                                                 )
{
  //  G4cout <<" ###AtRestDoI  " << G4endl;
  G4VParticleChange* pC;
  pC = pRegProcess->AtRestDoIt( track, stepData );
  if (!BiasMode2) { //mode 1
    G4double s = stepData.GetStepLength(); 
    G4double x = pRegProcess->GetCurrentInteractionLength();
    G4double w =  pC->GetParentWeight();
    // need to undo the change made to the parent track (/wc)
    w *= 1/wc * (1. - exp(-s/x))/(1 - exp(-eFactor*s/x)) ;
    size_t nSecondary = size_t(pC->GetNumberOfSecondaries());
    for (size_t idx=0; idx < nSecondary; idx+=1)
      (pC->GetSecondary(idx))->SetWeight(w);
  } else { // mode 2
    G4double nw = pC->GetParentWeight()*XBiasSecondaryWeight();
    ((G4ParticleChange*)pC)->ProposeWeight(nw) ;
    for (G4int i = 0; i < pC->GetNumberOfSecondaries(); i++) 
      pC->GetSecondary(i)->SetWeight(nw);
    //
    if(G4UniformRand()<XBiasSurvivalProbability()) {   
      pC->SetNumberOfSecondaries(pC->GetNumberOfSecondaries()+1);
      G4ThreeVector mDirection = track.GetDynamicParticle()->GetMomentumDirection();
      G4DynamicParticle* aD = new G4DynamicParticle((track.GetDynamicParticle())->GetDefinition(),
						    mDirection,
						    (track.GetDynamicParticle())->GetKineticEnergy()+stepData.GetTotalEnergyDeposit());    
      G4bool isGoodForTracking = true;
      ((G4ParticleChange*)pC)->
	AddSecondary(aD,isGoodForTracking); 
      pC->GetSecondary(pC->GetNumberOfSecondaries()-1)->
	SetWeight(XBiasSurvivalProbability()*track.GetWeight());
    }
  }
  return pC;
}

G4double GmWrapperProcess::XBiasSurvivalProbability()
{
  G4double result = 0;
  G4double nLTraversed = GetTotalNumberOfInteractionLengthTraversed();
  G4double biasedProbability = 1.-std::exp(-nLTraversed);
  G4double realProbability = 1-std::exp(-nLTraversed/eFactor);
  result = (biasedProbability-realProbability)/biasedProbability;
  return result;
}

G4double GmWrapperProcess::XBiasSecondaryWeight()
{
  G4double result = 0;
  G4double nLTraversed = GetTotalNumberOfInteractionLengthTraversed();
  result = 1./eFactor*std::exp(-nLTraversed/eFactor*(1-1./eFactor));
  return result;
}
 


