#include "GmNoUseG4RadDecayTimeUA.hh"
#include "SHRadDecayChainSet.hh"
#include "ShieldingVerbosity.hh"

#include "GamosCore/GamosGenerator/include/GmGenerDistTimeDecay.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosAnalysis/include/GmCheckOriginalGamma.hh"

#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4RadioactiveDecay.hh"

//---------------------------------------------------------------------------
GmNoUseG4RadDecayTimeUA::GmNoUseG4RadDecayTimeUA()
{

  theDecayChainSet = new SHRadDecayChainSet;
#ifndef WIN32
  GmCheckOriginalGamma::SetCheckParentID(false);
#else
  GmCheckOriginalGamma* cog = new GmCheckOriginalGamma();
  cog->SetCheckParentID(false);
#endif
}

//---------------------------------------------------------------------------
void GmNoUseG4RadDecayTimeUA::BeginOfRunAction( const G4Run* )
{
  theMinimumActivity = GmParameterMgr::GetInstance()->GetNumericValue(theName+":MinimumActivity",1.E-30);
  theMaxPrimaryTime = 0.;
  theMaxTracksTime = GmParameterMgr::GetInstance()->GetNumericValue(theName+":MaximumTracksTime",1.e-8*CLHEP::second);
}

//---------------------------------------------------------------------------
void GmNoUseG4RadDecayTimeUA::PreUserTrackingAction(const G4Track* aTrack)
{
  /*t  if( aTrack->GetParentID() == 0 ) {
    thePrimaries.insert(aTrack->GetTrackID());
    }*/
  theDecayChainSet->StoreParentChildren( aTrack, true );
  theDecayChainSet->StoreDecayBranchingRatios( aTrack );

  /*t  //---- Set lifetime to 0, so that G4RadioactiveDecay::PostStepDoIt does not change time (and then we can use the time set by GmGenerator)
  G4Track* ncTrack = const_cast<G4Track*>(aTrack);
  G4ParticleDefinition* ncPart = const_cast<G4ParticleDefinition*>(ncTrack->GetDefinition());
  // store lifetime for resetting it
  theLifeTimes[ncPart] = ncPart->GetPDGLifeTime();
  // set it to 0
  ncPart->SetPDGLifeTime(0.);
  */

}


//---------------------------------------------------------------------------
void GmNoUseG4RadDecayTimeUA::UserSteppingAction(const G4Step* aStep )
{
  const G4Track* aTrack = aStep->GetTrack();

  //  const G4RadioactiveDecay* procDec = dynamic_cast<const G4RadioactiveDecay*>(aTrack->GetCreatorProcess());
  const G4RadioactiveDecay* procDec = dynamic_cast<const G4RadioactiveDecay*>(aStep->GetPostStepPoint()->GetProcessDefinedStep());
  //  G4cout << " GmNoUseG4RadDecayTimeUA::UserSteppingAction Creator rad " << procDec << G4endl; //GDEB
  if( procDec == 0 ) return; // only when radioactive decay happens

  //---- Particles created by radioactive decay 
  //  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  //  G4double lifeTimeT = GetLifeTime(aTrack->GetDefinition());

  //---- Set time of track, as if G4RadioactiveDecay did not change it
  SetTime( aStep );
  //  G4cout << " GmNoUseG4RadDecayTimeUA::UserSteppingAction(SetTime " << aStep->GetTrack()->GetGlobalTime() << G4endl; //GDEB

  //--- For primary tracks, set the secondary time as the primary one
  if( aTrack->GetParentID() == 0 ) {
    G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
    for( size_t ii = 0; ii < secos.size(); ii++ ) {
      secos[ii]->SetGlobalTime( aTrack->GetGlobalTime() );
      //      G4cout << " GmNoUseG4RadDecayTimeUA::UserSteppingAction(Set secos " << secos[ii]->GetGlobalTime() << G4endl; //GDEB
    }

    return;
  }

  //--- For secondary tracks, set the secondary time as the one they would have as decay with the secondary activity
  //---- Get activity of the creator ion at this time (= creator ion time, as lifetime was set to 0)
  //-- Get first which decay chain produced it
  vi chain = theDecayChainSet->BuildChainFromTrack( aTrack->GetTrackID() );
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::UserSteppingAction chain " << SHRadDecayChainSet::GetChainName( &chain ) << G4endl;
#endif
  if( chain.size() == 0 ) {  // secondary is not unstable 
    return;
    /*	G4Exception("GmNoUseG4RadDecayTimeUA",
	"Warning",
	JustWarning,
	("No decay chain originated particle " + aTrack->GetDefinition()->GetParticleName()).c_str());*/
  }
  
  //--- Build list of lambdas and branching ratios
  std::vector<G4double> lambdas;
  std::vector<G4double> BRs;
  G4double BR = 1.;
  G4double lambdaMult = 1.;
  G4double BRMult = 1.;
  for( size_t ii = 0; ii < chain.size(); ii++ ) {
    if( ii != 0 ) {
      BR *= theDecayChainSet->GetDecayBR( chain[ii-1], chain[ii], 0);
      //	  G4cout << " BR " << chain[ii-1]->GetParticleName() << " " << chain[ii]->GetParticleName() << " " << theDecayChainSet->GetDecayBR( chain[ii-1], chain[ii], 0) << G4endl;
      if( !chain[ii]->GetPDGStable() ) {
	BRs.push_back( BR );
	BRMult *= BR;
#ifndef GAMOS_NO_VERBOSE
	if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::UserSteppingAction " << ii << " BR " << BR << G4endl;
#endif
	BR = 1.;
      }
    }
    if( !chain[ii]->GetPDGStable() ) {
      //t	  G4double lifeTime = GetLifeTime( chain[ii] );
      G4double lifeTime = chain[ii]->GetPDGLifeTime();
      G4double lambda = 1./lifeTime ;
      lambdas.push_back( lambda );
      if( ii != 0 ) { 
	lambdaMult *= lambda;
      }
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::UserSteppingAction " << ii << " lambda " << lambda << " lifeTime " << lifeTime << G4endl;
#endif
    }
  }
  
  //----- Get activity of this isotope
#ifndef WIN32
  G4double activity = GmGenerDistTimeDecay::GetCurrentActivity(); // activity of generator     
#else
  GmGenerDistTimeDecay* dist = new GmGenerDistTimeDecay();
  G4double activity = dist->GetCurrentActivity(); // activity of generator               
#endif
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::UserSteppingAction  primary generator activity " << activity << G4endl;
#endif
  
  activity *= lambdaMult * BRMult;
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::UserSteppingAction activity X lambdaMult X BRMult " << activity << G4endl;
#endif
  
  activity /= GetLambdaTerm( lambdas ); // all combinations of lambdas
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::UserSteppingAction activity / GetLambadTerm " << activity << G4endl;
#endif
  
  G4double trkTime = aTrack->GetGlobalTime();
  G4double expSum = 1.;
  if( lambdas.size() >= 2 ) {
    for( size_t ic = 0; ic < lambdas.size()-2; ic++ ) {
      expSum += GetExpLambdaTerm( trkTime, lambdas, ic ) * std::pow(float(-1),int(ic == 1)); // only second term is negative!!
    }
  } else if( lambdas.size() == 2 ) {
    expSum = exp(-lambdas[0]*trkTime);
  }
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::UserSteppingAction expSum FINAL " << expSum << G4endl;
#endif 
  
  activity *= expSum;
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::UserSteppingAction activity FINAL " << activity << G4endl;
#endif
  
  //--- If activity is too low, kill secondary particles (they will be created too far in time)
  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  if( activity < theMinimumActivity ) {
    for( size_t ii = 0; ii < secos.size(); ii++ ) {
      secos[ii]->SetTrackStatus(fStopAndKill);
    }
  } else {
    double rnd_poiss = -(1.0/ activity ) * log(CLHEP::RandFlat::shoot());
    G4double time = aStep->GetPostStepPoint()->GetGlobalTime() + rnd_poiss;
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::UserSteppingAction time add = " << rnd_poiss << " total time " << time << G4endl;
#endif
    for( size_t ii = 0; ii < secos.size(); ii++ ) {
      secos[ii]->SetGlobalTime( time );
    }
  } 
  
}

//---------------------------------------------------------------------------
void GmNoUseG4RadDecayTimeUA::SetTime( const G4Step* aStep )
{
  G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
  G4StepPoint* preSP = aStep->GetPreStepPoint();
  G4StepPoint* postSP = aStep->GetPostStepPoint();
  // G4cout << " DEC TIME pre " << preSP->GetGlobalTime() << " post " << postSP->GetGlobalTime() << G4endl;
  
  G4double velocity = preSP->GetVelocity();
  G4double timeChange = 0.;
  if( velocity != 0 ) timeChange = aStep->GetStepLength()/preSP->GetVelocity();
  G4double gtime = preSP->GetGlobalTime();
  preSP->SetGlobalTime( gtime );
  //  G4cout << " GmNoUseG4RadDecayTimeUA::SetTime " << gtime << G4endl; //GDEB
  gtime += timeChange;
  postSP->SetGlobalTime( gtime );
  aTrack->SetGlobalTime( gtime );
  G4double ltime = preSP->GetLocalTime();
  preSP->SetLocalTime( ltime );
  ltime += timeChange;
  postSP->SetLocalTime( ltime );
  aTrack->SetLocalTime( gtime );

}

//---------------------------------------------------------------------------
G4double GmNoUseG4RadDecayTimeUA::GetLambdaTerm( std::vector<G4double>& lambdas, G4int iExclude )
{
  G4double lambdaT = 1.;
  size_t nL = lambdas.size();
  if( nL < 2 ) return lambdaT;

  //  G4cout << " GetLambdaTerm nL " << nL << " iex " << iExclude << G4endl;
  for( size_t i1 = 0; i1 < nL-1; i1++ ) {    
    for( size_t i2 = i1+1; i2 < nL-1; i2++ ) {
      if( iExclude != G4int(i1) && iExclude != G4int(i2) ) lambdaT *= (-lambdas[i1]+lambdas[i2] );
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::GetLambdaTerm " << i1 << "," << i2 << " = " << lambdaT << G4endl;
#endif
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::GetLambdaTerm FINAL " << lambdaT << G4endl;
#endif
  
  return lambdaT;
}

//---------------------------------------------------------------------------
G4double GmNoUseG4RadDecayTimeUA::GetExpLambdaTerm( G4double trkTime, std::vector<G4double>& lambdas, G4int iInclude )
{
  G4double expT = 0.;

  size_t nL = lambdas.size();
  G4double exp1 =lambdas[iInclude]*trkTime;
  G4double exp2 =lambdas[nL-2]*trkTime;

  if( exp1 > 1.E-9 || exp2 > 1.E-9 ){
    expT = exp(-exp1) - exp(-exp2);
  } else {
    expT = -exp1+exp2+exp1*exp1-exp2*exp2;
  }

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::GetExpLambdaTerm = " << expT << " exp1 " << exp1 << " exp2 " << exp2 << G4endl;
#endif
	
  for( size_t i1 = 0; i1 < nL-1; i1++ ) {    
    expT *= GetLambdaTerm( lambdas, iInclude );
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::GetExpLambdaTerm add lambdaTerm = " << expT << G4endl;
#endif
  }

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << "GmNoUseG4RadDecayTimeUA::GetExpLambdaTerm FINAL = " << expT << G4endl;
#endif

  return expT;

}
	
//-----------------------------------------------------------------
void GmNoUseG4RadDecayTimeUA::PostUserTrackingAction(const G4Track* )
{
  /*t  G4Track* ncTrack = const_cast<G4Track*>(aTrack);
  G4ParticleDefinition* ncPart = const_cast<G4ParticleDefinition*>(ncTrack->GetDefinition());
    // store lifetime for resetting it
    std::map<G4ParticleDefinition*,G4double>::const_iterator ite =theLifeTimes.find(ncPart);
  // reset it to original value
  ncPart->SetPDGLifeTime((*ite).second);
  */
}

//---------------------------------------------------------------------------
G4double GmNoUseG4RadDecayTimeUA::GetLifeTime( G4ParticleDefinition* part )
{
  std::map<G4ParticleDefinition*,G4double>::const_iterator ite = theLifeTimes.find(part);
  return (*ite).second;
}

 
//---------------------------------------------------------------------------
void GmNoUseG4RadDecayTimeUA::EndOfEventAction(const G4Event*)
{
  theDecayChainSet->Clean(); //t ?
}


//---------------------------------------------------------------------------
G4ClassificationOfNewTrack GmNoUseG4RadDecayTimeUA::ClassifyNewTrack(const G4Track* aTrack )
{
  if( aTrack->GetParentID() == 0 ) {
    theMaxPrimaryTime = std::max(theMaxPrimaryTime, aTrack->GetGlobalTime() );
  }
  if( SHVerb( debugVerb ) ) G4cout << " GmNoUseG4RadDecayTimeUA::ClassifyNewTrack " << aTrack->GetDefinition()->GetParticleName() << " " << aTrack->GetGlobalTime() << " " << aTrack->GetKineticEnergy() << G4endl;

  //postpone secondary tracks with big time 
  if( aTrack->GetGlobalTime() > theMaxPrimaryTime + theMaxTracksTime ) {
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb( debugVerb ) ) G4cout << " GmNoUseG4RadDecayTimeUA::ClassifyNewTrack  track " << aTrack->GetTrackID() << " " << aTrack->GetDefinition()->GetParticleName() << " postponed " << aTrack->GetGlobalTime() << " > " << theMaxPrimaryTime + theMaxTracksTime 
				   << " pos " << aTrack->GetPosition() << G4endl; 
#endif
    return fPostpone;
  }

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb( debugVerb ) ) G4cout << " GmNoUseG4RadDecayTimeUA::ClassifyNewTrack  track " <<  aTrack->GetTrackID() << " " <<aTrack->GetDefinition()->GetParticleName() << " urgent " << aTrack->GetGlobalTime() << " > " << theMaxPrimaryTime + theMaxTracksTime << G4endl; 
#endif

  return fUrgent;
  
}

