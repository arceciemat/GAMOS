#include "GmMultiIsotopesSource.hh"
#include "GmGenerDistTimeConstant.hh"
#include "GmGenerDistDirectionRandom.hh"
#include "GmGenerDistPositionPoint.hh" 
#include "GmGenerDistEnergyConstant.hh"
#include "GmGenerDistTimeDecayWithProd.hh"
#include "GmGenerDistTimeDecay.hh"
#include "GmGenerDistTimeConstantChangeRandom.hh"
#include "GmFromTextFileSource.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmGenerDistEnergyConstant.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Element.hh"
#include "G4RunManager.hh"
#include "G4PrimaryVertex.hh"
#include "CLHEP/Random/RandFlat.h"

//-----------------------------------------------------------------------
GmMultiIsotopesSource::GmMultiIsotopesSource( const G4String& name ): GmFromTextFileSource( name )
{
  Initialize();
  theMinimumActivity = GmParameterMgr::GetInstance()->GetNumericValue("GmMultiIsotopesSource:MinimumActivity",1e-9);
}

//-----------------------------------------------------------------------
void GmMultiIsotopesSource::Initialize()
{
  theType = "GmMultiIsotopesSource";

  theTimeDistribution = 0;

  theDirectionDistribution = 0;

  thePositionDistribution = 0; 

  theEnergyDistribution = 0;

  theProductionTime = 0.;
  //  bUsedLastTime = 1;
}

//-----------------------------------------------------------------------
GmMultiIsotopesSource::~GmMultiIsotopesSource()
{
  theFileIn.Close();
}


//-----------------------------------------------------------------------
void GmMultiIsotopesSource::SetIsotopeData(GmGenerIsotopeData isotd )
{
  theIsotopeData = isotd;
  theProductionTime = 1./theIsotopeData.prodPerTime;
  if( GenerVerb(debugVerb) ) G4cout << " GmMultiIsotopesSource::SetIsotopeData( " << theName << " LIFETIME= " << theIsotopeData.lifeTime << " PRODUC_PER_PARTICLE_BEAM " << theIsotopeData.prodPerTime << G4endl;
}

//-----------------------------------------------------------------------
void GmMultiIsotopesSource::BuildGenerTimeDecayProd(G4double prevDecayTime )
{
  GmGenerDistTimeDecayWithProd* tdp = new GmGenerDistTimeDecayWithProd();
  tdp->SetProduction(theIsotopeData.prodPerTime);
  tdp->SetLifeTime(theIsotopeData.lifeTime);
  thePreviousTime = prevDecayTime;
  tdp->SetPreviousTime(thePreviousTime);
  theTimeDistribution = tdp;
  if( GenerVerb(testVerb) ) G4cout << this << " tdp " << tdp << " GmMultiIsotopesSource::BuildGenerTimeDecayProd " << theName << " LIFETIME= " << theIsotopeData.lifeTime << " PRODUCTION " << theIsotopeData.prodPerTime << G4endl;
}

//-----------------------------------------------------------------------
void GmMultiIsotopesSource::BuildGenerTimeConstantChangeRandom(G4double )
{
  GmGenerDistTimeConstantChangeRandom* tdp = new GmGenerDistTimeConstantChangeRandom();
  tdp->SetInterval(theProductionTime);
  theTimeDistribution = tdp;
  if( GenerVerb(testVerb) ) G4cout << " GmMultiIsotopesSource::BuildGenerTimeConstantChangeRandom( " << theName << " LIFETIME= " << theIsotopeData.lifeTime << " PRODUCTION " << theIsotopeData.prodPerTime << G4endl; 
}

//-----------------------------------------------------------------------
G4bool GmMultiIsotopesSource::BuildGenerTimeDecay(G4double prodTime, G4double prevDecayTime )
{
  GmGenerDistTimeDecay* tdp = new GmGenerDistTimeDecay();

  G4double lambda = 1./theIsotopeData.lifeTime;
  G4double nIsotopes = theIsotopeData.prodPerTime/lambda*(1.-exp(-(prodTime)*lambda))*exp(-prevDecayTime*lambda);
  G4double currentActivity = nIsotopes*lambda;
  tdp->SetActivity(currentActivity);
  tdp->SetLifeTime(theIsotopeData.lifeTime);
  theTimeDistribution = tdp;
  thePreviousTime = prevDecayTime;
  if( GenerVerb(testVerb) ) G4cout << " GmMultiIsotopesSource::BuildGenerTimeDecay( " << theName << " NISOTOPES " << nIsotopes << " ACTIVITY_PER_ns= " << currentActivity << " PROD_TIME " << prodTime << " PROD_PER_TIME " << theIsotopeData.prodPerTime << "*(1.-exp(-(PRODTIME*LAMBDA " << prodTime << " * " << lambda << "*" << exp(-prevDecayTime*lambda) << G4endl;
  if( currentActivity < theMinimumActivity ) { 
    return false;
  } else {
    return true;
  }
}


//-----------------------------------------------------------------------
G4double GmMultiIsotopesSource::GenerateTime()
{
  if( GenerVerb(testVerb) ) G4cout << " GmMultiIsotopesSource::GenerateTime " << theName << " bUsedLastTime " << bUsedLastTime << G4endl;
  if( bUsedLastTime ) {
    //    G4cout << " GmMultiIsotopesSource::GenerateTime " << theName << " bUsedLastTime " << bUsedLastTime << G4endl; //GDEB
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
  if( GenerVerb(debugVerb) ) G4cout << "GmParticleSource::GenerateTime: " << theName << " bUsedLastTime="<< bUsedLastTime << " time=" << theTime << " thePreviousTime " << thePreviousTime << " theLastDecayTimeAnySource=" << theLastDecayTimeAnySource << G4endl;
  #endif
   
  return theTime;
}

