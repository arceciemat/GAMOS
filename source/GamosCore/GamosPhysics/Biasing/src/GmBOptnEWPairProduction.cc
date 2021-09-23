#include "GmBOptnEWPairProduction.hh"
#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWPairProduction::GmBOptnEWPairProduction(G4String name)
: GmVEWBiasingOperation(name)
{
#ifndef BEAMZPOS
  theRRZ = GmParameterMgr::GetInstance()->GetNumericValue("EWBS:RussianRoulettePlaneZ",-DBL_MAX);
#else 
  theRRZ = GmParameterMgr::GetInstance()->GetNumericValue("EWBS:RussianRoulettePlaneZ",DBL_MAX);
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptnEWPairProduction::~GmBOptnEWPairProduction()
{
}

G4VParticleChange*  
GmBOptnEWPairProduction::
ApplyFinalStateBiasing( const G4BiasingProcessInterface* callingProcess,
                        const G4Track*                            track,
                        const G4Step*                              step,
                        G4bool&                                         )
{
  G4String history = "";
  G4String newHistory = "";
  
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWPairProduction " << callingProcess->GetProcessName() << " " << fNSplit << " WEI " <<  track->GetWeight() << " density= " << track->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity()/(CLHEP::g/CLHEP::cm3) << G4endl; 
#endif
  
  // -- Now deal with the gamma's:
  // -- their common weight:
  G4double gammaWeight = track->GetWeight();
  
  G4bool bSurvives = true;
  if( track->GetWeight() < 1. ) {
//---- NOT IN GAS
    if( track->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity() >= theGasDensity ) {
      if( CLHEP::RandFlat::shoot() < fNSplitInv ) { // if it survives RR, increase weight
	gammaWeight *= fNSplit;
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) {
	  G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPairProduction gamma ACCEPTED by RR increase weight " << gammaWeight << G4endl; 
	  history += "_NOFAT_NOGAS_Arr";
	}
#endif
//---- IN GAS
      } else {  
	bSurvives = false;
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) {
	  G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPairProduction gamma REJECTED by RR "  << G4endl; 
	  //-	  history += "_NOFAT_NOGAS_Rrr";
	}
#endif
      }
    }
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) {
      G4cout << fParticleChange.GetNumberOfSecondaries() << " GmBOptnEWPairProduction gamma in GAS " << gammaWeight << G4endl; 
      history += "_FAT";
    }
#endif
  }

  fParticleChange.Initialize(*track);

  if( bSurvives ) {
    // -- inform we will have nSplit e+ e-:
    G4int nSplit = 1;  // no splitting
#ifndef BEAMZPOS
    if( gammaWeight == 1 && track->GetPosition().z() < theRRZ ) {
      G4cout << "PAIRPRODUCTION RRZ " <<  track->GetPosition().z() <<  " " << theRRZ << G4endl;//GDEB
      nSplit = fNSplit; // create fNSplit e-e+ for fat photons
    }
#else
    if( gammaWeight == 1 && track->GetPosition().z() > theRRZ ) nSplit = fNSplit; // create fNSplit e-e+ for fat photons
#endif
    G4int nCalls = 1;
    while ( nCalls <= nSplit ) {
      G4VParticleChange* processFinalState = callingProcess->GetWrappedProcess()->PostStepDoIt(*track, *step);
      //    G4cout << " processFinalState NSECOS " <<  processFinalState->GetNumberOfSecondaries() << G4endl; //GDEB
      //-  if ( processFinalState->GetNumberOfSecondaries() == 0 )  return processFinalState;
      if( nCalls == 1 ) {
	G4ParticleChangeForLoss* actualParticleChange =
	  ( G4ParticleChangeForLoss* ) processFinalState ;
	std::vector<G4Track*> firstTracks;
	
	// -- Store gamma final state:
	fParticleChange.
	  ProposeTrackStatus      ( actualParticleChange->GetTrackStatus() );
	fParticleChange.
	  ProposeEnergy           ( actualParticleChange->GetProposedKineticEnergy() );
	fParticleChange.
	  ProposeMomentumDirection( actualParticleChange->GetProposedMomentumDirection() );
	
	// -- inform we take care of secondaries weight (otherwise these
	// -- secondaries are by default given the primary weight).
	fParticleChange.SetSecondaryWeightByProcess(true);
	fParticleChange.ProposeWeight( gammaWeight );
	fParticleChange.SetNumberOfSecondaries( 2*nSplit );
      
	//  actualParticleChange->Clear();
      }

      //--- This will happen /gamos/physics/applyCutsForAllProcesses 
      /*     if(  processFinalState->GetNumberOfSecondaries() != 2 ) {
      for ( G4int ii = 0 ; ii < processFinalState->GetNumberOfSecondaries(); ii++) {
	G4Track* eTrack = processFinalState->GetSecondary(ii);
	G4cerr << ii << " SECO " << eTrack->GetDefinition()->GetParticleName() << " E= " << eTrack->GetKineticEnergy() << G4endl;
      }
	G4Exception("GmBOptnEWPairProduction::ApplyFinalStateBiasing",
		    "",
		    FatalException,
		    "Not 2 secondaries, they will not be treated properly");
		    }*/
      for ( G4int ii = 0 ; ii < processFinalState->GetNumberOfSecondaries(); ii++) {
	G4Track* eTrack = processFinalState->GetSecondary(ii);
	fParticleChange.AddSecondary( eTrack );
	eTrack->SetWeight( gammaWeight );
#ifndef GAMOS_NO_VERBOSE
	if( BiasingVerb(testVerb) ) G4cout << "CHK " << " PairProduction "  << eTrack->GetDefinition()->GetParticleName() << " " <<  eTrack->GetWeight() << " " << track->GetWeight() << " " << history+newHistory << G4endl;
#endif
      }
      processFinalState->Clear(); 

      nCalls++;

    }
  } else {
    fParticleChange.ProposeTrackStatus( fStopAndKill );
  }
    
  // -- we are done:
  //-?  fParticleChange.SetNumberOfSecondaries( nSecos );
 
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << " GmBOptnEWPairProduction NSECOS_FINAL " << fParticleChange.GetNumberOfSecondaries() << G4endl; 
#endif  

  return &fParticleChange;
}



