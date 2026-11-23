#include "GmDoubleBackToBackParticleSource.hh"
#include "CLHEP/Random/RandFlat.h"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmGenerDistEnergyConstant.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "GmGenerDistTimeConstant.hh"
#include "GmGenerDistDirectionRandom.hh"
#include "GmGenerDistPositionPoint.hh" 
#include "GmGenerDistEnergyConstant.hh"

//-----------------------------------------------------------------------
GmDoubleBackToBackParticleSource::GmDoubleBackToBackParticleSource( const G4String& name, const G4String& partName, const G4double ener ): GmParticleSource( name )
{
  theType = "DoubleBackToBackParticleSource";

  theTimeDistribution = new GmGenerDistTimeConstant;
  theActivity = 1.*CLHEP::becquerel; 

  theDirectionDistribution = new GmGenerDistDirectionRandom;

  thePositionDistribution = new GmGenerDistPositionPoint;  // source centered at (0,0,0)

  //  if( name == "F18" && energyDist == "BetaDecay" ) {
  theEnergy = ener;
  if( bBiasDistributions ) {
    BiasEnergy();
  }
  theEnergyDistribution = new GmGenerDistEnergyConstant;
  static_cast<GmGenerDistEnergyConstant*>(theEnergyDistribution)->SetEnergy(theEnergy);  // could be extracted from source info at Generate(), but it is more efficient here
  CheckDistributionsExist();

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  theParticleDef = partTable->FindParticle( partName);
  if( !theParticleDef ) G4Exception("GmDoubleBackToBackParticleSource::GmDoubleBackToBackParticleSource",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String(GetName() + "  particle does not exist: " + partName).c_str());
}


//-----------------------------------------------------------------------
G4PrimaryVertex* GmDoubleBackToBackParticleSource::GenerateVertex( G4double time )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " G4PrimaryVertex* GmDoubleBackToBackParticleSource::GenerateVertex( G4double time ) " << G4endl;
#endif
  thePosition = thePositionDistribution->GeneratePosition( this );
  if( bBiasDistributions ) {
    BiasPosition();
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "GmDoubleBackToBackParticleSource::GenerateVertex pos " << thePosition << G4endl;
#endif

  G4PrimaryVertex* vtx = new G4PrimaryVertex( thePosition, time );

  theEnergy = theEnergyDistribution->GenerateEnergy( this );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "GmDoubleBackToBackParticleSource::GenerateVertex ener " << theEnergy << G4endl;
#endif

  theDirection = theDirectionDistribution->GenerateDirection( this );
  theDirection /= theDirection.mag();
  if( bBiasDistributions ) {
    BiasDirection();
  }
  
  G4ThreeVector mom = theDirection * std::sqrt(theEnergy*theEnergy + 2.*theParticleDef->GetPDGMass()*theEnergy);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmDoubleBackToBackParticleSource::GenerateVertex  mom " << mom << " theEnergy " << theEnergy << G4endl;
#endif
  G4PrimaryParticle* particle = new G4PrimaryParticle( theParticleDef, mom.x(), mom.y(), mom.z() );
  vtx->SetPrimary(particle);

 G4PrimaryParticle* particle2 = new G4PrimaryParticle( theParticleDef, -mom.x(), -mom.y(), -mom.z() );
  vtx->SetPrimary(particle2);

  return vtx;

}
