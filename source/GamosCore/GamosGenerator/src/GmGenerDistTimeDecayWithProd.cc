#include "GmGenerDistTimeDecayWithProd.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "G4tgrUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandPoisson.h"
#include "G4UnitsTable.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "GmGenerator.hh"
#include "G4RunManager.hh"
#include "G4Ions.hh"

//----------------------------------------------------------------------
GmGenerDistTimeDecayWithProd::GmGenerDistTimeDecayWithProd() 
{
  theGenerator = 0;
  theLifeTime = 0.;
  theProduction = 0.;
  thePreviousTime = 0.;
}

//----------------------------------------------------------------------
G4double GmGenerDistTimeDecayWithProd::GenerateTime( const GmParticleSource* source )
{
  if( !theGenerator ) {
    const G4VUserPrimaryGeneratorAction* generact = G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();
    theGenerator = (GmGenerator*)(generact);
  }

  if( theProduction == 0. ) {
    G4Exception("GmGenerDistTimeDecayWithProd::GenerateTime","Error",FatalException,(G4String(" production per second 0 for source ") + source->GetName()).c_str() );
  }

  G4double evtTime = theGenerator->GetLastEventTime();
  G4double currentActivity;
  if( evtTime == 0. ) {
    if( thePreviousTime == 0. ) { // first event in run: make activity as with only one isotope
      G4double nIsot = 1.;
      currentActivity = nIsot/theLifeTime;
    } else {
      currentActivity = theProduction*(1.-exp(-(evtTime+thePreviousTime)/theLifeTime));
    }
  } else {
    currentActivity = theProduction*(1.-exp(-evtTime/theLifeTime));
  }
  //  G4cout << theName << " GmGenerDistTimeDecayWithProd NISOT " << theProduction*(1.-exp(-(evtTime+thePreviousTime)/theLifeTime))*theLifeTime << " ACTIVITY " << currentActivity << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout <<this<< " GmGenerDistTimeDecayWithProd::Generate  activity= " << currentActivity << " theProduction=" << theProduction << " lifetime "  << theLifeTime << " event_time " << evtTime << " +thePreviousTime "<< thePreviousTime << G4endl;
#endif
  if( currentActivity < 1.e-20 ) {
    G4String ErrMsg = "source= " + source->GetName();
    G4Exception("GmGenerDistTimeDecayWithProd::GenerateTime",
		"Too low activity, life time ended",
		JustWarning,
		ErrMsg);
  }

  double rnd = CLHEP::RandFlat::shoot();
  double time = -(1.0/ currentActivity ) * log(rnd);
  if( time < theProductionInv ) { // decay time cannot be before first isotope is created
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistTimeDecayWithProd::Generate time= " << time << " is smaller than first_isotope_time " << theProductionInv << G4endl;
#endif
    time = theProductionInv;
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistTimeDecayWithProd::Generate  activity " << currentActivity << " final_time "  << time << " rnd " << rnd << G4endl;
#endif

  return time;
}

//---------------------------------------------------------------------
void GmGenerDistTimeDecayWithProd::SetParams( const std::vector<G4String>& params )
{
  if( params.size() == 2 ) {
    theProduction = GmGenUtils::GetValue( params[0] ); // number of isotopes per second
    theProductionInv = 1./theProduction;
    theLifeTime = GmGenUtils::GetValue( params[1] );
  } else {
    G4Exception("GmGenerDistTimeDecayWithProd::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"Must have one or two paremeters: ACTIVITY LIFETIME(optional)");
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistTimeDecayWithProd::SetParams production " << theProduction << " lifetime " << theLifeTime << G4endl;
#endif

}


//---------------------------------------------------------------------
void GmGenerDistTimeDecayWithProd::SetProduction( const G4double act ) 
{
  theProduction = act;
  theProductionInv = 1./theProduction;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistTimeDecayWithProd::Generate  activity " << theProduction << G4endl;
#endif
}


//---------------------------------------------------------------------
void GmGenerDistTimeDecayWithProd::SetLifeTime( const G4double act ) 
{
  theLifeTime = act;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistTimeDecayWithProd::Generate  activity " << theLifeTime << G4endl;
#endif
}

//---------------------------------------------------------------------
void GmGenerDistTimeDecayWithProd::SetPreviousTime( const G4double act ) 
{
  thePreviousTime = act;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistTimeDecayWithProd::Generate  activity " << thePreviousTime << G4endl;
#endif
}
