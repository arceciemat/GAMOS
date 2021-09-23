#include "SHActivationUA.hh"
#include "ShieldingVerbosity.hh"
#include "SHMetastableIsotMgr.hh"
#include "SHRadDecayChainSet.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4Track.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

//-----------------------------------------------------------------
SHActivationUA::SHActivationUA()
{
  theName = "SHActivationUA";

  theMaximumTime = -1;
  //  theMinimumTime = DBL_MAX;

  //  theMetastableIsotMgr = SHMetastableIsotMgr::GetInstance();

}


//-----------------------------------------------------------------
SHActivationUA::~SHActivationUA() 
{
}

//------------------------------------------------------------------
void SHActivationUA::BeginOfRunAction( const G4Run* )
{
  InitialiseRun();
}

//------------------------------------------------------------------
void SHActivationUA::InitialiseRun()
{
  theRadDecayChainSet = new SHRadDecayChainSet( theName );

  theTimes = GmParameterMgr::GetInstance()->GetVNumericValue(theName+":Times",theTimes);
  if( theTimes.size() == 0 ){
    theTimes.push_back(1.);
  }
  theRadDecayChainSet->SetTimes( theTimes );

  std::vector<G4String> periods;
  periods = GmParameterMgr::GetInstance()->GetVStringValue(theName+":Periods",periods);
  G4String fileName = GmParameterMgr::GetInstance()->GetStringValue(theName+":PeriodsFileName","");

  if( periods.size() != 0 && fileName != "" ) {
    G4Exception("SHActivationUA:BeginOfRunAction",
		"Wrong parameters",
		FatalErrorInArgument,
		G4String("You cannot define two parameters: a set of periods and a periods filename for user action " + theName).c_str());
  }

  if( periods.size() != 0 ) {
    SetPeriods( periods );
  }

  if( fileName != "" ) {
    if( periods.size() != 0 ) {
      G4Exception("SHActivation::BeginOfRun",
		  "Warning in argument",
		  JustWarning,
		  G4String(theName + " Setting periods as parameter and reading them from file, second will be used").c_str());
    }
    ReadPeriods(fileName);
  }

  G4double maxTime = 0.;
  for( size_t ii = 0; ii < theTimes.size(); ii++) {
    maxTime = std::max(maxTime, theTimes[ii]);
  }
  // if not periods, set the that all time production is ON
  if( thePeriods.size() == 0 ) {
    thePeriods[ maxTime ] = 1;
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << theName << "::BeginOfRunAction  no period defined, defining one : " << maxTime << " 1 " << G4endl;
#endif
  }
  // Add a last period with no production, from end of last period to biggest time 
  G4double maxPeriod = (*(thePeriods.rbegin())).first;
  if( maxPeriod < maxTime ) {
    thePeriods[ maxTime ] = 0; 
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << theName << "::BeginOfRunAction defining  last period " << maxTime << "  0, from time " << maxPeriod << G4endl;
#endif
  }

  theRadDecayChainSet->SetPeriods( thePeriods );

  G4double minHalfLife = GmParameterMgr::GetInstance()->GetNumericValue(theName+":MinHalfLife",1.*CLHEP::second);
  theRadDecayChainSet->SetMinLifeTime( minHalfLife / log(2.) );

}

//------------------------------------------------------------------
void SHActivationUA::SetPeriods( const std::vector<G4String>& periods )
{
  if( periods.size() % 2 != 0 ) {
    G4Exception("SHActivation::ReadPeriods",
		"Error",
		FatalException,
		(G4String("Problem setting periods, there should be an even number of arguments, there are ") + GmGenUtils::itoa(periods.size())).c_str());
  }
  G4double periodSum = 0.;
  for( size_t ii = 0; ii < periods.size(); ii+=2 ){
    periodSum += GmGenUtils::GetValue(periods[ii]);
    thePeriods[ periodSum ] = G4bool(GmGenUtils::GetValue(periods[ii+1])); 
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << theName << "::ReadPeriods defining period " << periodSum << " " << thePeriods[ periodSum ] << G4endl;
#endif
  }
}

//------------------------------------------------------------------
void SHActivationUA::ReadPeriods( const G4String& fileName )
{

  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  G4double periodSum = 0.;
  for(;;){
    if( ! fin.GetWordsInLine(wl)  ) break;
    if( wl.size() != 2 ) {
      G4Exception("SHActivation::ReadPeriods",
		  "Error",
		  FatalException,
		  (G4String("Problem reading file, there should be two words per line, there are ") + GmGenUtils::itoa(wl.size())).c_str());
    }
    periodSum += GmGenUtils::GetValue(wl[0]);
    thePeriods[ periodSum ] = G4bool(GmGenUtils::GetValue(wl[1])); 
  }
}


//------------------------------------------------------------------
void SHActivationUA::BeginOfEventAction( const G4Event* anEvent)
{
  G4double time = anEvent->GetPrimaryVertex(0)->GetT0();
  //  theMinimumTime = std::min(theMinimumTime, time);
  theMaximumTime = std::max(theMaximumTime, time);
  //  G4cout << " TIME " << time << " MAX " << theMaximumTime << " MIN " << theMinimumTime << G4endl;
}


//-----------------------------------------------------------------
void SHActivationUA::PreUserTrackingAction(const G4Track* aTrack)
{
  G4ParticleDefinition* particle = aTrack->GetDefinition();

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " SHActivationUA::PreUserTrackingAction PARTICLE " << particle->GetParticleName() << " TYPE= " << particle->GetParticleType() << " SUBTYPE= " << particle->GetParticleSubType() << " LIFETIME " << particle->GetPDGLifeTime() << G4endl;
#endif

  //-  theParticlesInEvent[particle] = aTrack->GetWeight();

  //  if( particle->GetPDGStable() ) return;

  theRadDecayChainSet->StoreIon( aTrack );
}


//-----------------------------------------------------------------
void SHActivationUA::EndOfRunAction(const G4Run* )
{
  PrintActivities();
}

//-----------------------------------------------------------------
void SHActivationUA::PrintActivities()
{

  G4double nev = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEvent();
  theRadDecayChainSet->SetNEventsInJob( nev );
  //  theRadDecayChainSet->SetJobTime(theMaximumTime-theMinimumTime);
  theRadDecayChainSet->SetJobTime(theMaximumTime); // assume theMinimumTime = 0; else 5 events are counted as 4 time intervals
  //  theRadDecayChainSet->SetNEventsPerNanoSecond ( nev / (theMaximumTime-theMinimumTime) );
  theRadDecayChainSet->SetNEventsPerNanoSecond ( nev / (theMaximumTime) );
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) )  G4cout << "SHActivationUA::EndOfRunAction NEventsInjob " << nev << " JobTime " << theMaximumTime << " NEventsPerNanoSecond " << nev / (theMaximumTime) << G4endl;
#endif

  //??  radChainSet to radChain (set the name...)
  std::map<G4String, SHRadDecayChain* > chains = theRadDecayChainSet->BuildPossibleDecayChains();
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(testVerb) ) G4cout << "SHActiv  chains " << chains.size() << G4endl; //GDEB
#endif

  // Print activities
  theRadDecayChainSet->SetNAtomsInJob( theParticles );
  //  theRadDecayChainSet->CheckLifeTimes(); 
  theRadDecayChainSet->PrintData();

}


