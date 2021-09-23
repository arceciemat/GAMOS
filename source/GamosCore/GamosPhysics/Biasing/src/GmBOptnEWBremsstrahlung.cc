#include "GmBOptnEWBremsstrahlung.hh"
#include "GmBiasingVerbosity.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWBremsstrahlung::GmBOptnEWBremsstrahlung(G4String name)
: GmVEWBiasingOperation(name)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWBremsstrahlung::~GmBOptnEWBremsstrahlung()
{
}

G4VParticleChange*  
GmBOptnEWBremsstrahlung::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* callingProcess,
                        const G4Track*                            track,
                        const G4Step*                              step,
                        G4bool&                                         )
{
  std::vector<G4Track*> addTrackVector;

  G4String history = "";
  G4String newHistory = "";
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWBremsstrahlung " << callingProcess->GetProcessName() << " " << fNSplit << G4endl;
#endif

  G4int nSplit = fNSplit;
  if( track->GetWeight() < 1 ) nSplit = 1;

  // -- a special case here: the process corrects for cross-section change
  // -- over the step due to energy loss by sometimes "abandoning" the interaction,
  // -- returning an unchanged incoming electron/positron.
  // -- We respect this correction, and if no secondary is produced, its means this
  // -- case is happening:
  //  if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;

  // -- Now deal with the gamma's:
  // -- their common weight:
  G4double gammaWeight = track->GetWeight() / nSplit;
  G4double gammaRRSurvivesWeight = gammaWeight * fNSplit ;
  
  // -- now start the fNSplit calls to the process to store each
  // -- related gamma:
  G4int nCalls = 1;
  while ( nCalls <= nSplit ) {
    
    // ( note: we don't need to cast to actual type here, as methods for accessing
    //   secondary particles are from base class G4VParticleChange )
    G4VParticleChange* processFinalState = callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);

    //t ALSO RR ON IT   if ( nSplit == 1 ) {
#ifndef GAMOS_NO_VERBOSE
      if( BiasingVerb(testVerb) ) {
	if( processFinalState->GetNumberOfSecondaries() != 0 )  {
	  G4cout << " GmBOptnEWBremsstrahlung NOT FAT = NO SPLITTING " <<  processFinalState->GetNumberOfSecondaries() << G4endl; 
	  G4Track* gammaTrack = processFinalState->GetSecondary(0);
	  history = "_NONFAT";
	  G4cout << "CHK " << " Bremsstrahlung " << gammaTrack->GetDefinition()->GetParticleName() << " " << gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	}
      }
#endif
      //      return processFinalState;
      //  }

    //    G4cout << " processFinalState NSECOS " <<  processFinalState->GetNumberOfSecondaries() << G4endl;
    if( nCalls == 1 ) {
      //      if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;
      // --   - the electron state will be taken as the first one produced by the 
      // --     process, hence the one stored in above processFinalState particle change.
      // --     This state will be stored in our fParticleChange object.
      // -- methods of the concrete G4ParticleChangeForLoss type:
      G4ParticleChangeForLoss* actualParticleChange =
	( G4ParticleChangeForLoss* ) processFinalState ;
      
      fParticleChange.Initialize(*track);
      
      // -- Store electron final state:
      fParticleChange.
	ProposeTrackStatus      ( actualParticleChange->GetTrackStatus() );
      fParticleChange.
	ProposeEnergy           ( actualParticleChange->GetProposedKineticEnergy() );
      fParticleChange.
	ProposeMomentumDirection( actualParticleChange->GetProposedMomentumDirection() );
      
      // -- inform we will have nSplit gamma's:
      fParticleChange.SetNumberOfSecondaries( nSplit );
      // -- inform we take care of secondaries weight (otherwise these
      // -- secondaries are by default given the primary weight).
      fParticleChange.SetSecondaryWeightByProcess(true);
      
      //-      actualParticleChange->Clear();
      
    }

#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWBremsstrahlung NSECOS0 " <<  processFinalState->GetNumberOfSecondaries() << G4endl; 
#endif

    for ( G4int ii = 0 ; ii < processFinalState->GetNumberOfSecondaries() ; ii++) {
      //	if( ii > 10 ) break;
      G4Track* gammaTrack = processFinalState->GetSecondary(ii);

      if( IsInPlane( gammaTrack->GetMomentumDirection(), gammaTrack->GetPosition() ) ) {
	addTrackVector.push_back( gammaTrack );
	gammaTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) {
	  G4cout << " GmBOptnEWBremsstrahlung ACCEPTED In Plane " << G4endl; 
	  newHistory = "_Aip";
	  G4cout << "CHK " << " Bremsstrahlung "  <<gammaTrack->GetDefinition()->GetParticleName() << " " << gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	}
#endif
      } else {
	G4double randn = CLHEP::RandFlat::shoot();
	if(randn < fNSplitInv ) {
	  addTrackVector.push_back( gammaTrack );
	  gammaTrack->SetWeight( gammaRRSurvivesWeight );
#ifndef GAMOS_NO_VERBOSE
	  if( BiasingVerb(testVerb) ) {
	    G4cout << " GmBOptnEWBremsstrahlung ACCEPTED by RR " << randn << " < " << fNSplitInv << G4endl; 
	    newHistory = "_Arr";
	    G4cout << "CHK " << " Bremsstrahlung "  <<gammaTrack->GetDefinition()->GetParticleName() << " " << gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	  }
#endif
	} else {
	  delete gammaTrack;
#ifndef GAMOS_NO_VERBOSE
	  if( BiasingVerb(testVerb) ) {
	    G4cout << " GmBOptnEWBremsstrahlung REJECTED by RR " << randn << " >= " << fNSplitInv << G4endl; 
	  }
#endif
	}
      }
      
    }
    processFinalState->Clear(); 

    nCalls++;

  }
  
  // -- we are done:
  G4int nSecos = addTrackVector.size();
  fParticleChange.SetNumberOfSecondaries( nSecos );
  for( G4int iit = 0; iit < nSecos; iit++ ) {
    fParticleChange.AddSecondary( addTrackVector[iit] );
  }
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWBremsstrahlung NSECOS_FINAL " << fParticleChange.GetNumberOfSecondaries() << " = " << nSecos << G4endl; 
#endif
  
  //-? fParticleChange.SetNumberOfSecondaries( nSecos );
  return &fParticleChange;
}



