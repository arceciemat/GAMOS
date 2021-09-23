#include "GmBOptnEWAnnihilation.hh"
#include "GmBiasingVerbosity.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWAnnihilation::GmBOptnEWAnnihilation(G4String name)
: GmVEWBiasingOperation(name)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWAnnihilation::~GmBOptnEWAnnihilation()
{
}

G4VParticleChange*  
GmBOptnEWAnnihilation::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* callingProcess,
                        const G4Track*                            track,
                        const G4Step*                              step,
                        G4bool&                                         )
{

  G4String history = "";
  G4String newHistory = "";
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWAnnihilation " << callingProcess->GetProcessName() << " " << fNSplit << " WEI " <<  track->GetWeight() << G4endl; 
#endif
  
  // -- a special case here: the process corrects for cross-section change
  // -- over the step due to energy loss by sometimes "abandoning" the interaction,
  // -- returning an unchanged incoming electron/positron.
  // -- We respect this correction, and if no secondary is produced, its means this
  // -- case is happening:
  //  if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;

  // -- Now deal with the gamma's:
  // -- their common weight:
  G4int nSplit = fNSplit;
  // --- Only split fat positrons
  if( track->GetWeight() < 1. ) {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) history += "_FAT";
#endif
    nSplit = 1; // only for fat photons
  }
  G4double gammaWeight = track->GetWeight() / nSplit;
  G4double gammaRRSurvivesWeight = gammaWeight * fNSplit;
  
  // -- inform we will have fNSplit gamma's:
  G4int nSecos = 0;

  // -- now start the fNSplit calls to the process to store each
  // -- related gamma:
  G4int nCalls = 1;
  while ( nCalls <= nSplit ) {
    
    // ( note: we don't need to cast to actual type here, as methods for accessing
    //   secondary particles are from base class G4VParticleChange )
    G4VParticleChange* processFinalState = callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
    if( nCalls == 1 ) {
      //-      if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;
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
      
      // -- inform we will have 2*nSplit gamma's:
      fParticleChange.SetNumberOfSecondaries( nSplit*2 );
      // -- inform we take care of secondaries weight (otherwise these
      // -- secondaries are by default given the primary weight).
      fParticleChange.SetSecondaryWeightByProcess(true);
//      G4cout << nCalls << " GmBOptnEWAnnihilation nsecos " << fParticleChange.GetNumberOfSecondaries() << " = " <<  processFinalState->GetNumberOfSecondaries()  << " = " << nSecos <<  G4endl; //GDEB
      //-      actualParticleChange->Clear();
    
    }

  //  G4cout << nCalls << " GmBOptnEWAnnihilation nsecos " << fParticleChange.GetNumberOfSecondaries() << G4endl; //GDEB

    for ( G4int ii = 0 ; ii < processFinalState->GetNumberOfSecondaries(); ii++) {
      //	if( ii > 10 ) break;
      G4Track* gammaTrack = processFinalState->GetSecondary(ii);
      if( IsInPlane( gammaTrack->GetMomentumDirection(), gammaTrack->GetPosition() ) ) {
 	fParticleChange.AddSecondary( gammaTrack );
	nSecos++;
	gammaTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) {
	  G4cout << nCalls << " " << ii << " GmBOptnEWAnnihilation ACCEPTED in plane WEI " << gammaWeight << G4endl; 
	  newHistory = "_Aip";
	  G4cout << "CHK " << " Annihilation "  <<gammaTrack->GetDefinition()->GetParticleName() << " " << gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	}
#endif
      } else {
	//	G4double randn = CLHEP::RandFlat::shoot();
	G4double nrand = CLHEP::RandFlat::shoot();
	if( nrand < fNSplitInv ) {
	  fParticleChange.AddSecondary( gammaTrack );
	  nSecos++;
	  gammaTrack->SetWeight( gammaRRSurvivesWeight );
#ifndef GAMOS_NO_VERBOSE
	  if( BiasingVerb(testVerb) ) {
	    G4cout << nCalls << " " << ii << " GmBOptnEWAnnihilation ACCEPTED in plane WEI " << gammaWeight << G4endl; 
	    newHistory = "_Arr";
	  G4cout << "CHK " << " Annihilation "  <<gammaTrack->GetDefinition()->GetParticleName() << " " << gammaTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
	  }
#endif
	  
	} else {
#ifndef GAMOS_NO_VERBOSE
	  if( BiasingVerb(testVerb) ) G4cout << nCalls << " " << ii << " GmBOptnEWAnnihilation REJECTED RR WEI " << gammaRRSurvivesWeight << "  " <<nrand << " < " << fNSplitInv << G4endl; //GDEB
#endif
	  delete gammaTrack;
	}
      }
      
    }
    processFinalState->Clear();

    //-    if( track->GetWeight() < 1. ) break; 
    
    nCalls++;
    
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << "GmBOptnEWAnnihilation  NSECOS_FINAL " << fParticleChange.GetNumberOfSecondaries() << " = " << nSecos << G4endl; //GDEB
#endif
  
  // -- we are done:
  // -- we are done:
  //-!?  fParticleChange.SetNumberOfSecondaries( nSecos );
  return &fParticleChange;
}



