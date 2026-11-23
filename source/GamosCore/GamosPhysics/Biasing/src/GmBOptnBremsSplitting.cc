#include "GmBOptnBremsSplitting.hh"
#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnBremsSplitting::GmBOptnBremsSplitting(G4String name)
: G4VBiasingOperation(name),
  fNSplit(1),
  fParticleChange()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnBremsSplitting::~GmBOptnBremsSplitting()
{
}

G4VParticleChange*  
GmBOptnBremsSplitting::
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
  
  // -- inform we will have fNSplit gamma's:
  fParticleChange.SetNumberOfSecondaries( fNSplit );

  // -- inform we take care of secondaries weight (otherwise these
  // -- secondaries are by default given the primary weight).
  fParticleChange.SetSecondaryWeightByProcess(true);

  // -- Store first gamma:
  G4Track* gammaTrack = actualParticleChange->GetSecondary(0);
  fParticleChange.AddSecondary( gammaTrack );
  gammaTrack->SetWeight( gammaWeight );
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
      if ( processFinalState->GetNumberOfSecondaries() == 1 )
        {
          gammaTrack = processFinalState->GetSecondary(0);
          fParticleChange.AddSecondary( gammaTrack );
          gammaTrack->SetWeight( gammaWeight );
          nCalls++;
        }
      // -- very rare special case: we ignore for now.
      else if (  processFinalState->GetNumberOfSecondaries() > 1 ) 
        {
          for ( G4int i = 0 ; i < processFinalState->GetNumberOfSecondaries() ; i++)
            delete processFinalState->GetSecondary(i);
        }
      processFinalState->Clear(); 
    }

  // -- we are done:
  return &fParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
