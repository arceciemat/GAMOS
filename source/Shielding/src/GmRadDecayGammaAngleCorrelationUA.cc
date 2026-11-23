#include "GmRadDecayGammaAngleCorrelationUA.hh"
#include "SHRadDecayChainSet.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "ShieldingVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "G4RadioactiveDecay.hh"

//---------------------------------------------------------------------------
GmRadDecayGammaAngleCorrelationUA::GmRadDecayGammaAngleCorrelationUA()
{
  theDecayChainSet = new SHRadDecayChainSet;
}

//---------------------------------------------------------------------------
void GmRadDecayGammaAngleCorrelationUA::UserSteppingAction( const G4Step* aStep ) 
{

  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  for( ite = secos.begin(); ite != secos.end(); ite ++ ){
    G4Track* secoTrack = (*ite);
    if( secoTrack->GetDefinition() == G4Gamma::Gamma() ) {
      const G4RadioactiveDecay* radDecay = dynamic_cast<const G4RadioactiveDecay*>(secoTrack->GetCreatorProcess());
      if( radDecay ) {
	G4String ionName = aStep->GetTrack()->GetDefinition()->GetParticleName();
	if( theGammas.find( ionName ) != theGammas.end() ) {
	  G4Exception("GmRadDecayGammaAngleCorrelationUA::UserSteppingAction",
		      "Error",
		      FatalException,
		      G4String("Repeated ion, ask GAMOS developers " + ionName).c_str());
	}
#ifndef GAMOS_NO_VERBOSE
	if( SHVerb(debugVerb) ) {
	  G4cout << " GmRadDecayGammaAngleCorrelationUA::UserSteppingAction add gamma " 
		 << ionName << " " << secoTrack->GetMomentumDirection() << G4endl;
	}
#endif
	theGammas[ionName] = secoTrack->GetMomentumDirection();
      }
    }

  }


}

//--------------------------------------------------------------------------
void GmRadDecayGammaAngleCorrelationUA::PreUserTrackingAction(const G4Track* aTrack)
{

  theDecayChainSet->StoreIon( aTrack, true );
}

//---------------------------------------------------------------------------
G4ClassificationOfNewTrack GmRadDecayGammaAngleCorrelationUA::ClassifyNewTrack(const G4Track* aTrack )
{
  if( aTrack->GetDefinition() == G4Gamma::Gamma() 
      && aTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay" ) {

    vi chain = theDecayChainSet->BuildChainFromTrack( aTrack->GetParentID() );
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(debugVerb) ) {
	G4cout << " GmRadDecayGammaAngleCorrelationUA::ClassifyNewTrack gamma from rad decay " 
	       << theDecayChainSet->GetChainName(&chain) << " lastion " << chain[chain.size()-1]->GetParticleName() << G4endl;
      }
#endif
    if( chain[chain.size()-1]->GetParticleName() == "Ni60[1332.5]" ) {
      vi::const_iterator itevi;
      G4bool bFound = false;
      for( itevi = chain.begin(); itevi != chain.end(); itevi++ ) {
	if( (*itevi)->GetParticleName() == "Ni60[2505.7]") {
	  bFound = true;
	  break;
	}
      }
      if( bFound ) {	
	std::map<G4String,G4ThreeVector>::const_iterator iteg = theGammas.find("Ni60[2505.7]");
	G4double theta = (*iteg).second.theta();
  //  G4double angleDiff = theDistribution->Sample(
	G4double thetaDiff = CLHEP::RandGauss::shoot()*CLHEP::pi/2.;
	G4double thetaNew = theta + thetaDiff;
	G4ThreeVector newDir(1,0,0);
	newDir.setTheta(thetaNew);
	newDir.setPhi(aTrack->GetMomentumDirection().phi());

#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(debugVerb) ) {
	G4cout << " GmRadDecayGammaAngleCorrelationUA::ClassifyNewTrack change gamma direction to " 
	       << newDir << " oldDir " << aTrack->GetMomentumDirection() 
	       << " theta " << theta << " thetaDiff " << thetaDiff << " thetaNew " << thetaNew << G4endl;
      }
#endif
	G4Track* aTrackNC = const_cast<G4Track*>(aTrack);
	aTrackNC->SetMomentumDirection(newDir);

      }
    }
  }

  return fUrgent;
}

//--------------------------------------------------------------------------
void GmRadDecayGammaAngleCorrelationUA::BeginOfEventAction( const G4Event* ) 
{
  theGammas.clear();
}
