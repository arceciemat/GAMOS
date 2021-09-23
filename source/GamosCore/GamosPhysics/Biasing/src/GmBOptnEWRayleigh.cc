#include "GmBOptnEWRayleigh.hh"
#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForGamma.hh"
#include "G4Electron.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWRayleigh::GmBOptnEWRayleigh(G4String name)
: GmVEWBiasingOperation(name)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWRayleigh::~GmBOptnEWRayleigh()
{
}

G4VParticleChange*  
GmBOptnEWRayleigh::
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
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWRayleigh " << callingProcess->GetProcessName() << " " << fNSplit << " WEI " <<  track->GetWeight() << G4endl; 
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
	  G4cout << " GmBOptnEWRayleigh gamma made fat " << gammaWeight << G4endl; 
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
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWRayleigh density " << track->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity()/(CLHEP::g/CLHEP::cm3) << " < " << theGasDensity/(CLHEP::g/CLHEP::cm3) << " WEI " << gammaWeight << " NSPLIT " << nSplit << G4endl; 
#endif
  
  // -- now start the fNSplit calls to the process to store each
  // -- related gamma:
  G4int nCalls = 1;
  G4bool bGammaAccepted = false;
  G4int nGammaAccepted = 0;
  while ( nCalls <= nSplit ) {
    newHistory = "";
    G4VParticleChange* processFinalState = callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
    
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWRayleigh NSECOS0 " <<  processFinalState->GetNumberOfSecondaries() << G4endl;
#endif
        
    //---- TREAT gamma 
    bGammaAccepted = false;
    G4double newWeight = gammaWeight/nSplit;
    G4ParticleChangeForGamma* actualParticleChange =
      ( G4ParticleChangeForGamma* ) processFinalState ;
    // IN PLAY OR RR
    if( IsInPlane( actualParticleChange->GetProposedMomentumDirection(), track->GetPosition() ) ) {
      nGammaAccepted++;
      bGammaAccepted = true;
      //      newWeight = gammaWeight/nSplit;
#ifndef GAMOS_NO_VERBOSE
      if( BiasingVerb(testVerb) ) {
	G4cout << " GmBOptnEWRayleigh gamma " << track->GetKineticEnergy() << " ACCEPTED In Plane " << " " << actualParticleChange->GetProposedKineticEnergy() << G4endl;
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
	  G4cout << " GmBOptnEWRayleigh gamma " << track->GetKineticEnergy() << " ACCEPTED by RR " << randn << " < " << fNSplitInv << " " << actualParticleChange->GetProposedKineticEnergy() << G4endl;
	  newHistory = "_gArr";
	}
#endif
      } else {
#ifndef GAMOS_NO_VERBOSE
        if( BiasingVerb(testVerb) ) {
	  G4cout << " GmBOptnEWRayleigh gamma " << track->GetKineticEnergy() << " REJECTED by RR " << randn << " >= " << fNSplitInv << G4endl;
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
	  //t	  fParticleChange.SetNumberOfSecondaries( nSplit );
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
      if( BiasingVerb(testVerb) ) G4cout << "1CHK " << " Rayleigh "  <<track->GetDefinition()->GetParticleName() << " " << newWeight << " " << track->GetWeight() << " " << history+newHistory << G4endl;
#endif
	
	//      actualParticleChange->Clear();
      } else { 
	// NOT FIRST GAMMA THAT IS ACCEPTED: ADD SECONDARY
        G4Track* gammaTrack = new G4Track( *track );
	gammaTrack->SetKineticEnergy( actualParticleChange->GetProposedKineticEnergy() );
	gammaTrack->SetMomentumDirection( actualParticleChange->GetProposedMomentumDirection() );
	addTrackVector.push_back( gammaTrack );
        gammaTrack->SetWeight( newWeight );
#ifndef GAMOS_NO_VERBOSE
  	if( BiasingVerb(testVerb) ) G4cout << "nCHK " << " Rayleigh "  <<gammaTrack->GetDefinition()->GetParticleName() << " " << gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
#endif
      }

    }

    processFinalState->Clear(); 
      
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << nCalls << " NSECO 1: " <<  fParticleChange.GetNumberOfSecondaries() << G4endl;  
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
    if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWRayleigh NO GAMMA ACCEPTED, kill original gamma" << G4endl; 
#endif  
  }
  
  // -- we are done:
  G4int nSecos = addTrackVector.size();
  fParticleChange.SetNumberOfSecondaries( nSecos );
  for( G4int iit = 0; iit < nSecos; iit++ ) {
    fParticleChange.AddSecondary( addTrackVector[iit] );
  }

#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWRayleigh NSECOS_FINAL e- " << fParticleChange.GetNumberOfSecondaries()-std::max(0,nGammaAccepted-1) <<  " gamma " << nGammaAccepted << " : " << fParticleChange.GetNumberOfSecondaries() << " = " << nSecos <<G4endl; 
#endif  
  //  fParticleChange.SetNumberOfSecondaries( nSecos );
  return &fParticleChange;
}

