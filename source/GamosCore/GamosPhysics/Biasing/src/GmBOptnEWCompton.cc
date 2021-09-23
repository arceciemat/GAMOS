#include "GmBOptnEWCompton.hh"
#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForGamma.hh"
#include "G4Electron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWCompton::GmBOptnEWCompton(G4String name)
: GmVEWBiasingOperation(name)
{
#ifndef BEAMZPOS
  theRRZ = GmParameterMgr::GetInstance()->GetNumericValue("EWBS:RussianRoulettePlaneZ",-DBL_MAX);
#else
  theRRZ = GmParameterMgr::GetInstance()->GetNumericValue("EWBS:RussianRoulettePlaneZ",DBL_MAX);
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWCompton::~GmBOptnEWCompton()
{
}

G4VParticleChange*  
GmBOptnEWCompton::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* callingProcess,
                        const G4Track*                            track,
                        const G4Step*                              step,
                        G4bool&                                         )
{
  std::vector<G4Track*> addTrackVector;
  
  G4String history = "";
  G4String newHistory = "";
  G4bool bParticleChangeInit = false; // it can be initialised by first gamma or first e-
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWCompton " << callingProcess->GetProcessName() << " " << fNSplit << " WEI " <<  track->GetWeight() << G4endl; 
#endif
  
  // -- Now deal with the gamma's:
  // -- their common weight:
  G4double gammaWeight = track->GetWeight();
  
  G4int nSplit = fNSplit;
  
  //--> NON FAT PHOTON
  if( track->GetWeight() < 1. ) {
    if( track->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity() < theGasDensity ) {
      //--> IN GAS: NO SPLIT
      nSplit = 1;
      //-     gammaWeight *= fNSplit;
#ifndef GAMOS_NO_VERBOSE
      if( BiasingVerb(testVerb) ) history += "_NOFAT-GAS";
#endif
    } else {
      //--> NOT IN GAS: PLAY RR
      if( CLHEP::RandFlat::shoot() < fNSplitInv ) { // if it survives RR, increase weight
	//--> SURVIVES RR: SPLIT 
	gammaWeight *= fNSplit;
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) {
	  G4cout << " GmBOptnEWCompton gamma made fat " << gammaWeight << G4endl; 
	  history += "_NOFAT-NOGAS-Arr";
	}
#endif
      } else {
	//--> DOESN'T SURVIVE RR: NO SPLIT AND RETURN
	/*	nSplit = 1; 
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) history += "_NOFAT-NOGAS-Rrr";
	#endif */
	fParticleChange.Initialize(*track);
	fParticleChange.ProposeTrackStatus( fStopAndKill );
	return &fParticleChange;

      }
    }
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) history += "_FAT";
#endif
  }

#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWCompton density " << track->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity()/(CLHEP::g/CLHEP::cm3) << " < " << theGasDensity/(CLHEP::g/CLHEP::cm3) << " WEI " << gammaWeight << " NSPLIT " << nSplit << G4endl; 
#endif
  
  // -- now start the fNSplit calls to the process to store each
  // -- related gamma:
  G4int nCalls = 1;
  G4bool bGammaAccepted = false;
  G4int nGammaAccepted = 0;
  while ( nCalls <= nSplit ) {
    newHistory = "";
    G4VParticleChange* processFinalState = callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);

    //    G4cout << " processFinalState NSECOS " <<  processFinalState->GetNumberOfSecondaries() << G4endl; //GDEB
    
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWCompton NSECOS0 " <<  processFinalState->GetNumberOfSecondaries() << G4endl;
#endif
        
    //---- TREAT gamma 
    bGammaAccepted = false;
    G4double newWeight = gammaWeight/nSplit;
    G4ParticleChangeForGamma* actualParticleChange =
      ( G4ParticleChangeForGamma* ) processFinalState ;
    // IN PLANE OR RR
    if( IsInPlane( actualParticleChange->GetProposedMomentumDirection(), track->GetPosition() ) ) {
      nGammaAccepted++;
      bGammaAccepted = true;
      //      newWeight = gammaWeight/nSplit;
#ifndef GAMOS_NO_VERBOSE
      if( BiasingVerb(testVerb) ) {
	G4cout << " GmBOptnEWCompton gamma ACCEPTED In Plane " << " " << actualParticleChange->GetProposedKineticEnergy() << G4endl;
	newHistory = "_gAip";
      }

#endif
    } else {
      G4double randn = CLHEP::RandFlat::shoot();
      if( randn < fNSplitInv ) {
        nGammaAccepted++; 
        bGammaAccepted = true;
	newWeight *= fNSplit;
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) {
	  G4cout << " GmBOptnEWCompton gamma ACCEPTED by RR " << randn << " < " << fNSplitInv << " " << actualParticleChange->GetProposedKineticEnergy() << G4endl;
	  newHistory = "_gArr";
	}
#endif
      } else {
#ifndef GAMOS_NO_VERBOSE
        if( BiasingVerb(testVerb) ) {
	  G4cout << " GmBOptnEWCompton gamma REJECTED by RR " << randn << " >= " << fNSplitInv << G4endl;
	  newHistory = "_gRrr";
	}
#endif
      }
    }

    if( bGammaAccepted ) {
      if( nGammaAccepted == 1 ) { 
	// NOT FIRST GAMMA THAT IS ACCEPTED: MODIFY ORIGINAL GAMMA	
	if( !bParticleChangeInit ) {
	  fParticleChange.Initialize(*track);
	  bParticleChangeInit = true;
	}

	fParticleChange.ProposeWeight( newWeight );

	// -- Store electron final state:
	fParticleChange.
	  ProposeTrackStatus      ( actualParticleChange->GetTrackStatus() );
	fParticleChange.
	  ProposeEnergy           ( actualParticleChange->GetProposedKineticEnergy() );
	fParticleChange.
	  ProposeMomentumDirection( actualParticleChange->GetProposedMomentumDirection() );
	
	// -- inform we will have fNSplit gamma's:
	// -- inform we take care of secondaries weight (otherwise these
	// -- secondaries are by default given the primary weight).
	fParticleChange.SetSecondaryWeightByProcess(true);

#ifndef GAMOS_NO_VERBOSE
      if( BiasingVerb(testVerb) ) G4cout << "1CHK " << " Compton "  <<track->GetDefinition()->GetParticleName() << " " << newWeight << " " << track->GetWeight() << " " << history+newHistory << G4endl;
#endif
      //      addTrackVector.push_back( const_cast<G4Track*>(track) );

	//      actualParticleChange->Clear();
      } else { 
	// NOT FIRST GAMMA THAT IS ACCEPTED: ADD SECONDARY
        G4Track* gammaTrack = new G4Track( *track );
	gammaTrack->SetKineticEnergy( actualParticleChange->GetProposedKineticEnergy() );
	gammaTrack->SetMomentumDirection( actualParticleChange->GetProposedMomentumDirection() );
	addTrackVector.push_back( gammaTrack );
        gammaTrack->SetWeight( newWeight );
#ifndef GAMOS_NO_VERBOSE
  	if( BiasingVerb(testVerb) ) G4cout << "nCHK " << " Compton "  <<gammaTrack->GetDefinition()->GetParticleName() << " " << gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory  << " N=" << addTrackVector.size() << G4endl;
#endif
      }

    }
    
    G4double trackZ = track->GetPosition().z();
    for( int ie = 0; ie < processFinalState->GetNumberOfSecondaries(); ie++) {
      G4Track* secoTrack = processFinalState->GetSecondary(ie);
      //---- PLAY RR WITH SECONDARY ELECTRONS IF GAMMA IS FAT
      if( secoTrack->GetDefinition() == G4Electron::Electron() ) {
#ifndef BEAMZPOS
	if( nSplit > 1 && trackZ < theRRZ ) { // no RR on e- below RR plane
#else
	if( nSplit > 1 && trackZ > theRRZ ) { // no RR on e- below RR plane
#endif
	  G4double randn = CLHEP::RandFlat::shoot();
	  if( randn < fNSplitInv ) {
	    if( !bParticleChangeInit ) {
	      fParticleChange.Initialize(*track);
	      bParticleChangeInit = true;
	    }
	    addTrackVector.push_back( secoTrack );
	    secoTrack->SetWeight( 1. );
#ifndef GAMOS_NO_VERBOSE
	    if( BiasingVerb(testVerb) ) {
	      G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWCompton e- ACCEPTED by RR " << secoTrack->GetWeight() << " " << secoTrack->GetKineticEnergy() << " N=" << addTrackVector.size() << G4endl;
	      //	    newHistory += "_eArr";
	      G4cout << "CHK " << " Compton "  << secoTrack->GetDefinition()->GetParticleName() << " " << secoTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory+"_eArr" << G4endl;
	    }
#endif
	  } else {
#ifndef GAMOS_NO_VERBOSE
	    if( BiasingVerb(testVerb) ) {
	      G4cout << " GmBOptnEWCompton e- REJECTED by RR " << randn << " >= " << fNSplitInv << G4endl;
	      //	  newHistory = "_Rrr";
	    }
#endif
	    delete secoTrack;
	  }
	} else {
	  addTrackVector.push_back( secoTrack );
	}
      	
      } else {
    //---- IN PLANE OR RR WITH SECONDARY GAMMAS (FLUORESCENCE)
	if( IsInPlane( secoTrack->GetMomentumDirection(), secoTrack->GetPosition() ) ) {
	  if( !bParticleChangeInit ) {
	    fParticleChange.Initialize(*track);
	    bParticleChangeInit = true;
	  }
	  addTrackVector.push_back( secoTrack );
	  secoTrack->SetWeight( gammaWeight/fNSplit );
#ifndef GAMOS_NO_VERBOSE
	  if( BiasingVerb(testVerb) ) {
	    G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWCompton fluo gamma  ACCEPTED in plane " <<  gammaWeight/fNSplit  << " N=" << addTrackVector.size() <<  G4endl; 
	    newHistory += "FLUO_Aip";
	    G4cout << "CHK " << " Compton "  << secoTrack->GetDefinition()->GetParticleName() << " " <<  secoTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	  } 
#endif
	} else {
	  if( CLHEP::RandFlat::shoot() < fNSplitInv ) {
	    if( !bParticleChangeInit ) {
	      fParticleChange.Initialize(*track);
	      bParticleChangeInit = true;
	    }
	    addTrackVector.push_back( secoTrack );
	    secoTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
	    if( BiasingVerb(testVerb) ) {
	      G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWCompton fluo gamma ACCEPTED by RR increase weight " << gammaWeight  << " N=" << addTrackVector.size() << G4endl; 
	      newHistory += "FLUO_Arr";
	      G4cout << "CHK " << " Compton "  << secoTrack->GetDefinition()->GetParticleName() << " " <<  secoTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	    } 
#endif
	  } else {
#ifndef GAMOS_NO_VERBOSE
	    if( BiasingVerb(testVerb) ) G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWCompton fluo gamma REJECTED by RR " << gammaWeight << G4endl; 
#endif
	    delete secoTrack;
	  }
	}
      }
 
    }

    processFinalState->Clear(); 
      
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << nCalls << " NSECO 1: " <<  fParticleChange.GetNumberOfSecondaries() << G4endl;  //GDEB
#endif
    nCalls++;
  }

  if( nGammaAccepted == 0 ) { 
    if( !bParticleChangeInit ) {
      fParticleChange.Initialize(*track);
      bParticleChangeInit = true;
    }
    fParticleChange.ProposeTrackStatus( fStopAndKill );
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWCompton NO GAMMA ACCEPTED, kill original gamma" << G4endl; 
#endif  
  }
  
  // -- we are done:
  G4int nSecos = addTrackVector.size();
  fParticleChange.SetNumberOfSecondaries( nSecos );
  for( G4int iit = 0; iit < nSecos; iit++ ) {
    fParticleChange.AddSecondary( addTrackVector[iit] );
  }

  #ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWCompton NSECOS_FINAL e- " << fParticleChange.GetNumberOfSecondaries()-std::max(0,nGammaAccepted-1) <<  " gamma " << nGammaAccepted << " : " << fParticleChange.GetNumberOfSecondaries() << " = " << nSecos <<G4endl; 
#endif  

  return &fParticleChange;
}



