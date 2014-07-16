#include "GmGenerDistTimeDecay.hh"
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

G4double GmGenerDistTimeDecay::theCurrentActivity = 0.;

//----------------------------------------------------------------------
GmGenerDistTimeDecay::GmGenerDistTimeDecay() 
{
  theGenerator = 0;
  theLifeTime = 0.;
}

//----------------------------------------------------------------------
G4double GmGenerDistTimeDecay::GenerateTime( const GmParticleSource* source )
{
  if( !theGenerator ) {
    const G4VUserPrimaryGeneratorAction* generact = G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();
    theGenerator = (GmGenerator*)(generact);
  }

  if( theActivity == 0. ) {
    G4Exception("GmGenerDistTimeDecay::GenerateTime","Error",FatalException,(G4String(" activity is 0 for source ") + source->GetName()).c_str() );
  }

  //  const G4Event* evt = G4EventManager::GetEventManager()->GetConstCurrentEvent();
  G4double evtTime = theGenerator->GetLastEventTime();
  theCurrentActivity = theActivity;
  if( theLifeTime > 0. ) theCurrentActivity *= exp(-evtTime/theLifeTime);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistTimeDecay::Generate  activity before lifetime " << theActivity << " after " << theCurrentActivity << " lifetime "  << theLifeTime << " event_time " << evtTime << G4endl;
#endif
  if( theCurrentActivity < 1.e-20 ) {
    G4String ErrMsg = "source= " + source->GetName();
    G4Exception("GmGenerDistTimeDecay::GenerateTime",
		"Too low activity, life time ended",
		RunMustBeAborted,
		ErrMsg);
  }

  double rnd_poiss = 0.0;
  rnd_poiss = -(1.0/ theCurrentActivity ) * log(CLHEP::RandFlat::shoot());
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistTimeDecay::Generate  activity " << theCurrentActivity << " time "  << rnd_poiss << G4endl;
#endif

  return rnd_poiss;

  //  G4double ndec = CLHEP::RandPoisson::shoot( theActivity * second);
  //  G4cout << " NDEC " << ndec <<" acti " << theActivity*second << G4endl;

  //  return ( 1. / ndec / ns);  

}

//---------------------------------------------------------------------
void GmGenerDistTimeDecay::SetParams( const std::vector<G4String>& params )
{
  if( params.size() == 1 ) {
    theActivity = GmGenUtils::GetValue( params[0] );
    theLifeTime = 0.;
    // If source particle is ion, get lifetime
    if( theParticleSource->GetParticleName().find("[") != std::string::npos &&
	theParticleSource->GetParticleName().find("]") != std::string::npos ) {
      G4Ions* ion = GmGetParticleMgr::GetInstance()->CreateIon( theParticleSource->GetParticleName() );
      if( ion ) {
	theLifeTime = ion->GetPDGLifeTime();
      }
    }

  }else if( params.size() == 2 ) {
    theActivity = GmGenUtils::GetValue( params[0] );
    theLifeTime = GmGenUtils::GetValue( params[1] );
  } else {
    G4Exception("GmGenerDistTimeDecay::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"Must have one or two paremeters: ACTIVITY LIFETIME(optional)");
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistTimeDecay::SetParams  activity " << theActivity << " lifetime " << theLifeTime << G4endl;
#endif

}


//---------------------------------------------------------------------
void GmGenerDistTimeDecay::SetActivity( const G4double act ) 
{
  theActivity = act;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistTimeDecay::Generate  activity " << theActivity << G4endl;
#endif
}
