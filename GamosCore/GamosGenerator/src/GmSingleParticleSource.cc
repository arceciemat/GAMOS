#include "GmSingleParticleSource.hh"
#include "G4PrimaryVertex.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmGenerDistEnergyConstant.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "GmGenerDistTimeConstant.hh"
#include "GmGenerDistDirectionRandom.hh"
#include "GmGenerDistPositionPoint.hh" 
#include "GmGenerDistEnergyConstant.hh"
#include "GmGeneratorMgr.hh"

#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Element.hh"

#include "CLHEP/Random/RandFlat.h"

//-----------------------------------------------------------------------
GmSingleParticleSource::GmSingleParticleSource( const G4String& name, const G4String& partName, const G4double ener ): GmParticleSource( name )
{
  theType = "SingleParticleSource";

  theTimeDistribution = new GmGenerDistTimeConstant;
  theActivity = 1.*CLHEP::becquerel; 

  theDirectionDistribution = new GmGenerDistDirectionRandom;

  thePositionDistribution = new GmGenerDistPositionPoint;  // source centered at (0,0,0)

  //  if( name == "F18" && energyDist == "BetaDecay" ) {
  theEnergy = ener;
  theEnergyDistribution = new GmGenerDistEnergyConstant;
  static_cast<GmGenerDistEnergyConstant*>(theEnergyDistribution)->SetEnergy(theEnergy);  // could be extracted from source info at Generate(), but it is more efficient here
  CheckDistributionsExist();

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable();
  theParticleDef = partTable->FindParticle( partName);
  if( !theParticleDef ) {
    theParticleDef = (G4ParticleDefinition*)(GmGetParticleMgr::GetInstance()->CreateIon(partName));

  }

  if( !theParticleDef ) {
    G4Exception("GmSingleParticleSource::GmSingleParticleSource",
		"Wrong argument",
		FatalErrorInArgument,
		G4String(GetName() + "  particle does not exist: " + partName).c_str());
  }

  bPolarization = false;
  std::vector<G4double> polariz;
  polariz = GmParameterMgr::GetInstance()->GetVNumericValue(theName+":Polarization",polariz);

  if( polariz.size() != 0 ) {
    bPolarization = true;
    if( polariz.size() != 3 ) {
      G4Exception("GmSingleParticleSource",
		  "Wrong number of arguments in setting polarization",
		  FatalErrorInArgument,
		  G4String("There must be 3, while there are "+GmGenUtils::itoa(polariz.size())).c_str());
    }
    thePolarizationX = polariz[0];
    thePolarizationY = polariz[1];
    thePolarizationZ = polariz[2];
  }
  
}


//-----------------------------------------------------------------------
G4PrimaryVertex* GmSingleParticleSource::GenerateVertex( G4double time )
{
	
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " G4PrimaryVertex* GmSingleParticleSource::GenerateVertex( G4double time ) " << G4endl;
#endif
 
 thePosition = thePositionDistribution->GeneratePosition( this );
  if( bBiasDistributions ) {
    BiasPosition();
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "GmSingleParticleSource::GenerateVertex pos " << thePosition << G4endl;
#endif
  
  G4PrimaryVertex* vtx = new G4PrimaryVertex( thePosition, time );
 
  theEnergy = theEnergyDistribution->GenerateEnergy( this );
  if( bBiasDistributions ) {
    BiasEnergy();
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "GmSingleParticleSource::GenerateVertex ener " << theEnergy << G4endl;
#endif

  theDirection = theDirectionDistribution->GenerateDirection( this );
  theDirection /= theDirection.mag();
  if( bBiasDistributions ) {
    BiasDirection();
  }

  G4ThreeVector mom = theDirection * std::sqrt(theEnergy*theEnergy + 2.*theParticleDef->GetPDGMass()*theEnergy);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmSingleParticleSource::GenerateVertex  mom " << mom << " theEnergy " << theEnergy << G4endl;
#endif
  G4PrimaryParticle* particle = new G4PrimaryParticle( theParticleDef, mom.x(), mom.y(), mom.z() );

  if( bPolarization ) particle->SetPolarization( thePolarizationX, thePolarizationY, thePolarizationZ );

  particle->SetWeight( theWeight );

  vtx->SetPrimary(particle);

  return vtx;

}

