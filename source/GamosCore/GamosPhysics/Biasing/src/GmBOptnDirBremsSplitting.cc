#include "GmBOptnDirBremsSplitting.hh"
#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnDirBremsSplitting::GmBOptnDirBremsSplitting(G4String name)
: G4VBiasingOperation(name),
  fNSplit(1),
  fNKill(1),
  fNKillInv(1.),
  fParticleChange()
{
  thePlaneX = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":XDim",100.*CLHEP::mm);
  thePlaneY = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":YDim",100.*CLHEP::mm);
  thePlaneZ = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":ZPos",1000.*CLHEP::mm);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnDirBremsSplitting::~GmBOptnDirBremsSplitting()
{
}

G4VParticleChange*  
GmBOptnDirBremsSplitting::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* callingProcess,
                        const G4Track*                            track,
                        const G4Step*                              step,
                        G4bool&                                         )
{
  
  // -- Collect brem. process (wrapped process) final state:
  G4VParticleChange* processFinalState =
    callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);

  // -- if no splitting requested, let the brem. process to return directly its
  // -- generated final state:
  if ( fNSplit == 1 ) return processFinalState;

  // -- a special case here: the brem. process corrects for cross-section change
  // -- over the step due to energy loss by sometimes "abandoning" the interaction,
  // -- returning an unchanged incoming electron/positron.
  // -- We respect this correction, and if no secondary is produced, its means this
  // -- case is happening:
  if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;

  // -- Now start the biasing:
  // --   - the electron state will be taken as the first one produced by the brem.
  // --     process, hence the one stored in above processFinalState particle change.
  // --     This state will be stored in our fParticleChange object.
  // --   - the photon accompagnying the electron will be stored also this way.
  // --   - we will then do fNSplit - 1 call to the brem. process to collect
  // --     fNSplit - 1 additionnal gammas. All these will be stored in our
  // --     fParticleChange object.

  // -- We called the brem. process above. Its concrete particle change is indeed
  // -- a "G4ParticleChangeForLoss" object. We cast this particle change to access
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

  // -- Now deal with the gamma's:
  // -- their common weight:
  G4double gammaWeight = track->GetWeight() / fNSplit;
  G4double gammaRRSurvivesWeight = gammaWeight * fNKill ;
  
  // -- inform we will have fNSplit gamma's:
  fParticleChange.SetNumberOfSecondaries( fNSplit );

  // -- inform we take care of secondaries weight (otherwise these
  // -- secondaries are by default given the primary weight).
  fParticleChange.SetSecondaryWeightByProcess(true);

  // -- Store first gamma:
  G4Track* gammaTrack = actualParticleChange->GetSecondary(0);
  if( IsInPlane( gammaTrack->GetMomentumDirection(), gammaTrack->GetPosition() ) ) {
    fParticleChange.AddSecondary( gammaTrack );
    gammaTrack->SetWeight( gammaWeight );
  } else {
    if( CLHEP::RandFlat::shoot() < fNKillInv ) {
      fParticleChange.AddSecondary( gammaTrack );
      gammaTrack->SetWeight( gammaRRSurvivesWeight );
    } else {
      delete gammaTrack;
    }
  }
  // -- and clean-up the brem. process particle change:
  actualParticleChange->Clear();

  // -- now start the fNSplit-1 calls to the brem. process to store each
  // -- related gamma:
  G4int nCalls = 1;
  while ( nCalls < fNSplit )
    {
      // ( note: we don't need to cast to actual type here, as methods for accessing
      //   secondary particles are from base class G4VParticleChange )
      processFinalState = callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
      for ( G4int ii = 0 ; ii < processFinalState->GetNumberOfSecondaries() ; ii++) {
	//	if( ii > 10 ) break;
	gammaTrack = processFinalState->GetSecondary(ii);
	if( IsInPlane( gammaTrack->GetMomentumDirection(), gammaTrack->GetPosition() ) ) {
	  fParticleChange.AddSecondary( gammaTrack );
	  gammaTrack->SetWeight( gammaWeight );
	} else {
 	  if( CLHEP::RandFlat::shoot() < fNKillInv ) {
	    fParticleChange.AddSecondary( gammaTrack );
	    gammaTrack->SetWeight( gammaRRSurvivesWeight );
	  } else {
	    delete gammaTrack;
	  }
	}

	nCalls++;
      }
      processFinalState->Clear(); 
    }

  // -- we are done:
  return &fParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//---------------------------------------------------------------------------------------
G4bool GmBOptnDirBremsSplitting::IsInPlane(const G4ThreeVector dir, const G4ThreeVector pos)
{
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(debugVerb) ) G4cout << " GmBOptnDirBremsSplitting::IsInPlane dir " << dir << " pos " << pos << G4endl;
#endif

  G4double diffZ = thePlaneZ - pos.z();

#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(debugVerb) ) G4cout << "GmBOptnDirBremsSplitting: diffZ " << diffZ << " dir.z " << dir.z() << G4endl;
#endif

  //----- Check if dir is pointing towards plane
  if ( diffZ * dir.z() <= 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << "GmBOptnDirBremsSplitting: Rejected because direction is not towards plane " << G4endl;
#endif
    return FALSE;
  }

  //G4double xInPlane = pos.x()+diffZ*(std::tan(std::acos(dir.z())));
  G4double xInPlane = pos.x()+diffZ*dir.x()/dir.z();
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << "GmBOptnDirBremsSplitting: xInPlane / posX / dirX  " << xInPlane << " / " << pos.x() << " / " << dir.x() << " " << thePlaneX << G4endl;
#endif

  if ( fabs(xInPlane) > thePlaneX ) {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << "GmBOptnDirBremsSplitting: Rejected because out of plane in X " << G4endl;
#endif
    return FALSE;
  }
  //yInPlane = pos.y()+diffZ*(std::tan(std::acos(dir.z())));
  G4double yInPlane = pos.y()+diffZ*dir.y()/dir.z();
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << "GmBOptnDirBremsSplitting: / posY / dirY  " << yInPlane << " / " << pos.y() << " / " << dir.y() << " " << thePlaneY << G4endl;
#endif

  if ( fabs(yInPlane) > thePlaneY ) {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << "GmBOptnDirBremsSplitting: Rejected because out of plane in Y " << G4endl;
#endif
    return FALSE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << "GmBOptnDirBremsSplitting: Accepted xInPlane/yInPlane = " << xInPlane << " / " << yInPlane
		 << " thePlaneX/Y= " << thePlaneX << " / " << thePlaneY << G4endl;
#endif
  return TRUE;
}

