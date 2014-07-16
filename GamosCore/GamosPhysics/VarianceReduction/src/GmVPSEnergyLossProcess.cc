#include "GmVPSEnergyLossProcess.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
#include "G4Gamma.hh"
#include "G4BremsstrahlungCrossSectionHandler.hh"

#include "GmEWPSLivermoreBremsstrahlungModel.hh"

//-------------------------------------------------------------------------
GmVPSEnergyLossProcess::GmVPSEnergyLossProcess(const G4String& aName, G4ProcessType aType)
  : G4VEnergyLossProcess(aName,aType)
{
}

//-------------------------------------------------------------------------
G4VParticleChange* GmVPSEnergyLossProcess::PostStepDoIt(const G4Track& track,
							const G4Step& )
{
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << " G4VParticleChange* GmVPSEnergyLossProcess::PostStepDoIt(const G4Track& track " << G4endl;
#endif

  G4double finalT = track.GetKineticEnergy();
  if(finalT <= lowestKinEnergy) return &fParticleChange;

  G4double postStepScaledEnergy = finalT*massRatio;

  if(!currentModel->IsActive(postStepScaledEnergy)) return &fParticleChange;
  /*
  if(1 < verboseLevel) {
    G4cout << GetProcessName()
           << "::PostStepDoIt: E(MeV)= " << finalT/MeV
	   << G4endl;
  }
  */
  // Integral approach
  if (integral) {
    G4double lx = GetLambdaForScaledEnergy(postStepScaledEnergy);
    /*
    if(preStepLambda<lx && 1 < verboseLevel && nWarnings<200) {
      G4cout << "WARNING: for " << particle->GetParticleName()
             << " and " << GetProcessName()
             << " E(MeV)= " << finalT/MeV
             << " preLambda= " << preStepLambda 
	     << " < " << lx << " (postLambda) "
	     << G4endl;
      nWarnings++;
    }
    */
    if(preStepLambda*G4UniformRand() > lx) {
      ClearNumberOfInteractionLengthLeft();
      return &fParticleChange;
    }
  }

  SelectModel(postStepScaledEnergy);
  /*g494  if(useDeexcitation) {
    currentModel->SetDeexcitationFlag(idxDERegions[currentMaterialIndex]);
    }*/

  const G4DynamicParticle* dynParticle = track.GetDynamicParticle();
  //g495  G4double tcut = (*theCuts)[currentMaterialIndex];
  G4double tcut = (*theCuts)[currentCoupleIndex];

  // sample secondaries
  secParticles.clear();
  std::vector<G4double> secondaryWeights;
  
  thePSModel->SampleSecondariesPS(&secParticles, secondaryWeights, currentCouple, dynParticle, tcut, 100.*TeV, track );
  //  G4cout << " GmVPSEnergyLoss NSECO " << secParticles.size() << " Z " << track.GetPosition().z() << G4endl;

  //  secondaryWeights *= track.Getweight()

  // save secondaries
  G4int num = secParticles.size();
  fParticleChange.SetNumberOfSecondaries(num);
   for (G4int ii=0; ii<num; ++ii) {
    //  for (ites=secondaries->begin(), itew=secondaryWeights.begin(); ites!=secondaries->end(); ites++,itew++) {
    // create track
    G4Track* aTrack = new G4Track(secParticles[ii], track.GetGlobalTime(), track.GetPosition());
    // set IsGoodGorTrackingFlag
    aTrack->SetGoodForTrackingFlag();
    // touchable handle is copied to keep the pointer
    aTrack->SetTouchableHandle(track.GetTouchableHandle());
    // set weight
    fParticleChange.AddSecondary(aTrack); // fParticleChange is G4ParticleChangeForLoss
    aTrack->SetWeight(secondaryWeights[ii]); // this has to be done later, beacuse AddSecondary takes the one from the parent
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmVPSEnergyLossProcess: final wgt " << aTrack->GetWeight() << " = " << secondaryWeights[ii] << G4endl;
#endif
      // add the secondary

#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmVPSEnergyLossProcess: SECONDARY " << ii  
	   << " " << aTrack->GetDefinition()->GetParticleName()
	   << " MOM " << aTrack->GetMomentumDirection()
	   << " POS " << aTrack->GetPosition()
	   << " ENE " << aTrack->GetKineticEnergy()
	   << " WEIGHT " << aTrack->GetWeight() << G4endl;
#endif
  }
#ifndef GAMOS_NO_VERBOSE
  //  if( PhysicsVerb(debugVerb) ) G4cout << "GmVPSEnergyLossProcess: N SECO " << fParticleChange.GetNumberOfSecondaries() << " = "
  //	 << secParticles.size() << G4endl;
#endif

  /*
  if(1 < verboseLevel) {
    G4cout << "::PostStepDoIt: Sample secondary; Efin= " 
    << fParticleChange.GetProposedKineticEnergy()/MeV
           << " MeV; model= (" << currentModel->LowEnergyLimit()
           << ", " <<  currentModel->HighEnergyLimit() << ")"
           << "  preStepLambda= " << preStepLambda
           << "  dir= " << track.GetMomentumDirection()
           << "  status= " << track.GetTrackStatus()
           << G4endl;
  }
  */
  ClearNumberOfInteractionLengthLeft();
  return &fParticleChange;
}
