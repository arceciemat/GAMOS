#include "GmBOptnEWPhotoElectric.hh"
#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"
#include "G4Gamma.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWPhotoElectric::GmBOptnEWPhotoElectric(G4String name)
: GmVEWBiasingOperation(name)
{
#ifndef BEAMZPOS
  theRRZ = GmParameterMgr::GetInstance()->GetNumericValue("EWBS:RussianRoulettePlaneZ",DBL_MAX);
#else
  theRRZ = GmParameterMgr::GetInstance()->GetNumericValue("EWBS:RussianRoulettePlaneZ",DBL_MAX);
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWPhotoElectric::~GmBOptnEWPhotoElectric()
{
}

G4VParticleChange*  
GmBOptnEWPhotoElectric::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* callingProcess,
                        const G4Track*                            track,
                        const G4Step*                              step,
                        G4bool&                                         )
{ 
  std::vector<G4Track*> addTrackVector;

  G4String history = "";
  G4String newHistory = "";
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWPhotoElectric " << callingProcess->GetProcessName() << " " << fNSplit << " WEI " <<  track->GetWeight() << " density= " << track->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity()/(CLHEP::g/CLHEP::cm3)  << G4endl; 
#endif

  // -- Now deal with the gamma's:
  // -- their common weight:
  G4double gammaWeight = track->GetWeight();
  
  G4bool bSurvives = true;
  if( track->GetWeight() < 1. ) {
    if( track->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity() >= theGasDensity ) {
      if( CLHEP::RandFlat::shoot() < fNSplitInv ) { // if it survives RR, increase weight
	gammaWeight *= fNSplit;
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) {
	  G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric gamma ACCEPTED by RR increase weight " << gammaWeight << G4endl; 
	  history += "_NOFAT_NOGAS_Arr";
	}
#endif
      } else {
	bSurvives = false;
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric gamma REJECTED by RR "  << G4endl; 
#endif
      }
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( BiasingVerb(testVerb) ) {
	G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric gamma in GAS " << gammaWeight << G4endl; 
	history += "_NOFAT_GAS";
      }
#endif
    }
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) {
      G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric gamma in GAS " << gammaWeight << G4endl; 
      history += "_FAT";
    }
#endif
  }


  fParticleChange.Initialize(*track);

  if( bSurvives ) {
    // -- inform we will have nSplit e-:
    G4VParticleChange* processFinalState = callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
    if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;
    //    G4cout << " processFinalState NSECOS " <<  processFinalState->GetNumberOfSecondaries() << G4endl; 
    // --   - the electron state will be taken as the first one produced by the 
    // --     process, hence the one stored in above processFinalState particle change.
    // --     This state will be stored in our fParticleChange object.
    // -- methods of the concrete G4ParticleChangeForLoss type:
    G4ParticleChangeForLoss* actualParticleChange =
      ( G4ParticleChangeForLoss* ) processFinalState ;
    
    // -- Store gamma final state:
    fParticleChange.
      ProposeTrackStatus      ( actualParticleChange->GetTrackStatus() );
    fParticleChange.
      ProposeEnergy           ( actualParticleChange->GetProposedKineticEnergy() );
    fParticleChange.
      ProposeMomentumDirection( actualParticleChange->GetProposedMomentumDirection() );
    
    // -- inform we will have fNSplit e-:
    //t    fParticleChange.SetNumberOfSecondaries( nSplit + fNSplit );
    // -- inform we take care of secondaries weight (otherwise these
    // -- secondaries are by default given the primary weight).
    fParticleChange.SetSecondaryWeightByProcess(true);
    fParticleChange.ProposeWeight( gammaWeight );
    
    //  actualParticleChange->Clear();
  
    
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWPhotoElectric fluo gamma  N= " << processFinalState->GetNumberOfSecondaries() << gammaWeight << G4endl; 
#endif
    for ( G4int ii = 0 ; ii < processFinalState->GetNumberOfSecondaries() ; ii++) {
      G4Track* secoTrack = processFinalState->GetSecondary(ii);
      //--- PROCESS FLUORESCENT GAMMAS
      if( secoTrack->GetDefinition() == G4Gamma::Gamma() ) { // fluorescent gammas
	//
	//	if( ii > 10 ) break;
	//--- FIRST GAMMA: IN PLANE OR RR
	newHistory = "";
	if( IsInPlane( secoTrack->GetMomentumDirection(), secoTrack->GetPosition() ) ) {
	  addTrackVector.push_back( secoTrack );
	  secoTrack->SetWeight( gammaWeight/fNSplit );
#ifndef GAMOS_NO_VERBOSE
	  if( BiasingVerb(testVerb) ) {
	    G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric 1st fluo gamma  ACCEPTED in plane " << gammaWeight << G4endl; 
	    newHistory += "FLUO_Aip";
	    G4cout << "CHK " << " PhotoElectric "  << secoTrack->GetDefinition()->GetParticleName() << " " <<  secoTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	  } 
#endif
	} else {
	  if( CLHEP::RandFlat::shoot() < fNSplitInv ) {
	    addTrackVector.push_back( secoTrack );
	    secoTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
	    if( BiasingVerb(testVerb) ) {
	      G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric 1st fluo gamma ACCEPTED by RR increase weight " << gammaWeight << G4endl; 
	      newHistory += "FLUO_Arr";
	      G4cout << "CHK " << " PhotoElectric "  << secoTrack->GetDefinition()->GetParticleName() << " " <<  secoTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	    } 
#endif
	  } else {
#ifndef GAMOS_NO_VERBOSE
	    if( BiasingVerb(testVerb) ) G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric 1st fluo gamma REJECTED by RR " << gammaWeight << G4endl; 
#endif
	    delete secoTrack;
	  }
	}
	if( fabs(gammaWeight-1.) < 1.e-6 )  {
	  // fat fluo gamma: split uniformly in other fNSplit-1
	  G4double gammaKinE = secoTrack->GetKineticEnergy();
	  G4ThreeVector gammaPos = secoTrack->GetPosition();
	  G4double gammaTime = secoTrack->GetGlobalTime();
	  //---- PRODUCE fNSplit-1 FLUO GAMMAS ISOTROPICALLY
	  for( G4int ifluo = 0; ifluo < fNSplit-1; ifluo++ ) {
	    newHistory = "";
	    G4double cosTeta = 2.*G4UniformRand()-1.; 
	    G4double sinTeta = sqrt((1.-cosTeta)*(1.0 + cosTeta));
	    G4double phi     = CLHEP::twopi * G4UniformRand();
	    G4ThreeVector direction(sinTeta*cos(phi), sinTeta*sin(phi), cosTeta);
	    
	    //--- NON FIRST GAMMAS: IN PLANE OR RR
	    if( IsInPlane( direction, track->GetPosition() ) ) {
	      G4DynamicParticle* dp =
		new G4DynamicParticle (G4Gamma::Gamma(),direction,gammaKinE);
	      G4Track* nGammaTrack = new G4Track(dp, gammaTime, gammaPos);
	      addTrackVector.push_back( nGammaTrack );
	      nGammaTrack->SetWeight( gammaWeight/fNSplit );
#ifndef GAMOS_NO_VERBOSE
	      if( BiasingVerb(testVerb) ) {
		G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric " << ifluo << " fluo gamma  ACCEPTED in plane " << gammaWeight/fNSplit << G4endl; 
		newHistory += "FLUO_Aip";
		G4cout << "CHK " << " PhotoElectric "  << nGammaTrack->GetDefinition()->GetParticleName() << " " << nGammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	      } 
#endif
	    } else {	      
	      if( CLHEP::RandFlat::shoot() < fNSplitInv ) {
		G4DynamicParticle* dp =
		  new G4DynamicParticle (G4Gamma::Gamma(),direction,gammaKinE);
		G4Track* nGammaTrack = new G4Track(dp, gammaTime, gammaPos);
		addTrackVector.push_back( nGammaTrack );
		nGammaTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
		if( BiasingVerb(testVerb) ) {
		  G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric " << ifluo << " fluo gamma ACCEPTED by RR increase weight " << gammaWeight << G4endl; 
		  newHistory += "FLUO_Arr";
		  G4cout << "CHK " << " PhotoElectric "  << nGammaTrack->GetDefinition()->GetParticleName() << " " << nGammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
		}
#endif
	      } else {
#ifndef GAMOS_NO_VERBOSE
		if( BiasingVerb(testVerb) ) G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPhotoElectric  " << ifluo << " fluo gamma REJECTED by RR " << G4endl; 
#endif
	      }
	    }
	  }
	} 
      } else {  // ELECTRONS
	G4int nSplit = 1;  // no splitting
#ifndef BEAMZPOS
	if( gammaWeight == 1 && track->GetPosition().z() < theRRZ ) nSplit = fNSplit; // create fNSplit e-e+ for fat photons
#else
	if( gammaWeight == 1 && track->GetPosition().z() > theRRZ ) nSplit = fNSplit; // create fNSplit e-e+ for fat photons
#endif
	G4double newWeight = gammaWeight/fNSplit;
	addTrackVector.push_back( secoTrack );
	secoTrack->SetWeight( newWeight );
	for( G4int jj = 0; jj < nSplit-1; jj++ ) {
	  G4Track* nSecoTrack = new G4Track( *secoTrack );
	  addTrackVector.push_back( nSecoTrack );
	  nSecoTrack->SetWeight( newWeight );
#ifndef GAMOS_NO_VERBOSE
	  if( BiasingVerb(testVerb) ) G4cout << jj << " CHK " << " PhotoElectric "  << nSecoTrack->GetDefinition()->GetParticleName() << " " << newWeight << " " << track->GetWeight() << " " << history+newHistory << G4endl;
#endif
	}
      }
      
    }
    processFinalState->Clear(); 
    
  } else {
    fParticleChange.ProposeTrackStatus( fStopAndKill );
  }
  
  // -- we are done:
  G4int nSecos = addTrackVector.size();
  fParticleChange.SetNumberOfSecondaries( nSecos );
  for( G4int iit = 0; iit < nSecos; iit++ ) {
    fParticleChange.AddSecondary( addTrackVector[iit] );
  }
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWPhotoElectric NSECOS_FINAL " << fParticleChange.GetNumberOfSecondaries() << G4endl; 
#endif  
  
  // t=? fParticleChange.SetNumberOfSecondaries( nSecos );
  return &fParticleChange;
}



