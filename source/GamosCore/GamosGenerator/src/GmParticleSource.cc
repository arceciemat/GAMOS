#include "GmParticleSource.hh"
#include "GmVGenerDistTime.hh"
#include "GmVGenerDistEnergy.hh"
#include "GmVGenerDistPosition.hh"
#include "GmVGenerDistDirection.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "GamosCore/GamosData/Distributions/include/GmVNumericDistribution.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4PrimaryVertex.hh"
#include "G4ParticleDefinition.hh"
#include "CLHEP/Random/RandFlat.h"
 
#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GmGeneratorDistributionFactories.hh"
#endif

//-----------------------------------------------------------------------
GmParticleSource::GmParticleSource( const G4String& name )
{
  theName = name;
  theLastDecayTimeAnySource = 0.;
  bUsedLastTime = true;

  bBiasDistributions = false;

  theMaxBiasIterations = GmParameterMgr::GetInstance()->GetNumericValue(theName+":MaxBiasIterations",10000);
}


//-----------------------------------------------------------------------
void GmParticleSource::CheckDistributionsExist()
{
  if( !theTimeDistribution || !theDirectionDistribution || !thePositionDistribution || !theEnergyDistribution ) {
    G4cerr << "!!One distribution does not exist for ParticleSource: " << theName << " time " << theTimeDistribution << " dir " << theDirectionDistribution << " pos " << thePositionDistribution << " ener " << theEnergyDistribution << G4endl;
    G4Exception("GmParticleSource::CheckDistributionsExit",
		"Wrong argument",
		FatalErrorInArgument,
		"");
  }
}


//-----------------------------------------------------------------------
void GmParticleSource::SetDistributionTime( const G4String& distName, const std::vector<G4String>& wl )
{
#ifdef ROOT5
  theTimeDistribution = Reflex::PluginService::Create<GmVGenerDistTime*>(distName);
#else
  theTimeDistribution = GmGenerDistTimeFactory::get()->create(distName);
#endif
  if( !theTimeDistribution ) {
    G4Exception("GmParticleSource::SetDistributionTime","Fatal error in argument",FatalErrorInArgument,G4String(" Distribution does not exist, check documentation").c_str());
  }
  theTimeDistribution->SetName(distName);
  
  theTimeDistribution->SetParticleSource( this );
  theTimeDistribution->CopyParams( wl );
  theTimeDistribution->SetParams( wl );

}


//-----------------------------------------------------------------------
void GmParticleSource::SetDistributionEnergy( const G4String& distName, const std::vector<G4String>& wl )
{
#ifdef ROOT5
  theEnergyDistribution = Reflex::PluginService::Create<GmVGenerDistEnergy*>(distName);
#else
  theEnergyDistribution = GmGenerDistEnergyFactory::get()->create(distName);
#endif
  if( !theEnergyDistribution ) {
    G4Exception("GmParticleSource::SetDistributionEnergy","Fatal error in argument",FatalErrorInArgument,G4String(" Distribution does not exist, check documentation ").c_str());
  }
  theEnergyDistribution->SetName(distName);
  theEnergyDistribution->SetParticleSource( this );
  theEnergyDistribution->CopyParams( wl ); 
  theEnergyDistribution->SetParams( wl ); 

}


//-----------------------------------------------------------------------
void GmParticleSource::SetDistributionPosition( const G4String& distName, const std::vector<G4String>& wl )
{
#ifdef ROOT5
  thePositionDistribution = Reflex::PluginService::Create<GmVGenerDistPosition*>(distName);
#else
  thePositionDistribution = GmGenerDistPositionFactory::get()->create(distName);
#endif
  if(  !thePositionDistribution ) {
    G4Exception("GmParticleSource::SetDistributionPosition","Fatal error in argument",FatalErrorInArgument,G4String(" Distribution does not exist, check documentation ").c_str());
  }
  thePositionDistribution->SetName(distName);
  
  thePositionDistribution->SetParticleSource( this );
  thePositionDistribution->CopyParams( wl );
  thePositionDistribution->SetParams( wl );

}


//-----------------------------------------------------------------------
void GmParticleSource::SetDistributionDirection( const G4String& distName, const std::vector<G4String>& wl )
{
#ifdef ROOT5
  theDirectionDistribution = Reflex::PluginService::Create<GmVGenerDistDirection*>(distName);
#else
  theDirectionDistribution = GmGenerDistDirectionFactory::get()->create(distName);
#endif
  if(  !theDirectionDistribution ) {
    G4Exception("GmParticleSource::SetDistributionDirection","Fatal error in argument",FatalErrorInArgument,G4String(" Distribution does not exist, check documentaiton ").c_str());
  }
  theDirectionDistribution->SetName(distName);

  theDirectionDistribution->SetParticleSource( this );
  theDirectionDistribution->CopyParams( wl );
  theDirectionDistribution->SetParams( wl );

}
//-----------------------------------------------------------------------
G4double GmParticleSource::GenerateTime()
{
  if( bUsedLastTime ) {
    theTime = theTimeDistribution->GenerateTime( this );
  } else {
    theTime -= theLastDecayTimeAnySource;
    //    if( theTime < 0 ) theTime = 0.;
    if( theTime < 0 ) {
      G4Exception("GmParticleSource::GenerateTime",
		  "time - theLastDecayTimeAnySource is negative",
		  FatalErrorInArgument,
		  G4String("Time= "+GmGenUtils::ftoa(theTime+theLastDecayTimeAnySource)+" LastDecayTimeAnySource= "+GmGenUtils::ftoa(theLastDecayTimeAnySource)).c_str());
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "GmParticleSource::GenerateTime: " << theName << " iso activity " << theActivity << " time= " << theTime << " theLastDecayTimeAnySource " << theLastDecayTimeAnySource << G4endl;
#endif

  return theTime;
}



//-----------------------------------------------------------------------
void GmParticleSource::LastTimeActive( G4double )
{
  theLastDecayTimeAnySource = 0.;
  bUsedLastTime = true;
}


//-----------------------------------------------------------------------
void GmParticleSource::LastTimeNotActive( G4double tim )
{
  //  theLastDecayTimeAnySource += tim;
  theLastDecayTimeAnySource = tim;
  bUsedLastTime = false;
  //  G4cout << theName << "  GmParticleSource::LastTimeNotActive theLastDecayTimeAnySource " <<  theLastDecayTimeAnySource << G4endl; //GDEB
}


//-----------------------------------------------------------------------
void GmParticleSource::ReInitialiseDistributions()
{

  theTimeDistribution->ResetParams();
  theEnergyDistribution->ResetParams();
  thePositionDistribution->ResetParams();
  theDirectionDistribution->ResetParams();

}

//---------------------------------------------------------------------
void GmParticleSource::AddBiasDistribution( const G4String& varName, const G4String& distName )
{
  std::vector<G4String> distNames;
  distNames.push_back(distName);
  distNames.push_back(distName);
  GmVDistribution* distrib = GmDistributionMgr::GetInstance()->FindOrBuildDistribution( distNames, true );
  distrib->Normalize();

  GmVNumericDistribution* distribN = (GmVNumericDistribution*)distrib;
  theBiasDistributions[varName] = distribN;

  bBiasDistributions = true;
}


//-----------------------------------------------------------------------
void GmParticleSource::BiasPosition()
{
  std::map<G4String,GmVNumericDistribution*>::const_iterator ite = theBiasDistributions.find("PosX");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.x() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }
  

  ite = theBiasDistributions.find("PosY");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.y() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }

  ite = theBiasDistributions.find("PosZ");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.z() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }


  ite = theBiasDistributions.find("PosR2");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.perp() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }


  ite = theBiasDistributions.find("PosR");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.mag() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }


  ite = theBiasDistributions.find("PosTheta");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.theta() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }


  ite = theBiasDistributions.find("PosPhi");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( thePosition.phi() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      thePosition = thePositionDistribution->GeneratePosition( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasPosition",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }
 
}


//-----------------------------------------------------------------------
void GmParticleSource::BiasDirection()
{
  std::map<G4String,GmVNumericDistribution*>::const_iterator ite = theBiasDistributions.find("DirTheta");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( theDirection.theta() );
      G4double rnd = CLHEP::RandFlat::shoot();
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) 
	G4cout << ii << " GmParticleSource::BiasDirection DirTheta= " << theDirection.theta() << " val= " << val << " rndm " << rnd << G4endl;
#endif
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      theDirection = theDirectionDistribution->GenerateDirection( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasDirection",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }


  ite = theBiasDistributions.find("DirPhi");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( theDirection.phi() );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      theDirection = theDirectionDistribution->GenerateDirection( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasDirection",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }
}
  

//-----------------------------------------------------------------------
void GmParticleSource::BiasEnergy()
{
  std::map<G4String,GmVNumericDistribution*>::const_iterator ite = theBiasDistributions.find("Energy");
  if( ite != theBiasDistributions.end() ) {
    G4int ii = 0;
    for( ;; ) {
      G4double val = (*ite).second->GetNumericValueFromIndex( theEnergy );
      G4double rnd = CLHEP::RandFlat::shoot();
      if( rnd < val ) {
	theWeight /= val;	
	break;
      }
      theEnergy = theEnergyDistribution->GenerateEnergy( this );
      ii++;
      if( ii == theMaxBiasIterations ) {
	G4Exception("GmParticleSource::BiasEnergy",
		    "Too many iterations to calculate bias",
		    JustWarning,
		    G4String("N iterations = " + GmGenUtils::itoa(ii)).c_str());
      }
    }
    
  }

}
 
//-----------------------------------------------------------------------
std::vector<G4PrimaryVertex*> GmParticleSource::GenerateVertices( G4double time )
{
  std::vector<G4PrimaryVertex*> vtxs;
  vtxs.push_back(GenerateVertex( time ));

  return vtxs;
}
