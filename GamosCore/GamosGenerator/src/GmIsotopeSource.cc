#include "GmIsotopeSource.hh"
#include "CLHEP/Random/RandFlat.h"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmGenerDistTimeDecay.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"
#include "GmGenerDistTimeDecay.hh"
#include "GmGenerDistDirectionRandom.hh"
#include "GmGenerDistPositionPoint.hh" 
#include "GmGenerDistEnergyConstantIsotopeDecay.hh"
 
//-----------------------------------------------------------------------
GmIsotopeSource::GmIsotopeSource( const G4String& name,  GmIsotope* isotope, G4double activity ): GmParticleSource( name )
{
  theIsotope = isotope;
  theType = "IsotopeSource";
  theCurrentDecay = 0;

  theTimeDistribution = new GmGenerDistTimeDecay;
  theActivity = activity;
  static_cast<GmGenerDistTimeDecay*>(theTimeDistribution)->SetActivity( activity ); // could be extracted from source info at Generate(), but it is more efficient here

  theDirectionDistribution = new GmGenerDistDirectionRandom;

  thePositionDistribution = new GmGenerDistPositionPoint;  // source centered at (0,0,0)

  /*  if( theIsotope->GetName() == "F18" ) {
    theEnergyDistribution = GmGenerDistEnergyFactory::get()->create("GmGenerDistEnergyBetaDecay"); 
    } else { */
  theEnergyDistribution = new GmGenerDistEnergyConstantIsotopeDecay; 
//-  }

  CheckDistributionsExist();

  //-  theActivity = 1.E-1; // becquerel is 1/second = 1E-9
  theEnergy = 0.;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "Creating GmIsotopeSource: " << theName << " from isotope " << theIsotope->GetName() << G4endl;
#endif
}


//-----------------------------------------------------------------------
G4PrimaryVertex* GmIsotopeSource::GenerateVertex( G4double time )
{
  std::vector<GmIsotopeDecay*> decs = theIsotope->GenerateDecayProducts();
  thePosition = thePositionDistribution->GeneratePosition( this );
  if( bBiasDistributions ) {
    BiasPosition();
  }

  G4PrimaryVertex* vtx = new G4PrimaryVertex( thePosition, time );

  std::vector<GmIsotopeDecay*>::const_iterator ite;
  for( ite = decs.begin(); ite != decs.end(); ite++ ){
    theCurrentDecay = *ite;
    theEnergy = theEnergyDistribution->GenerateEnergy( this );
    if( bBiasDistributions ) {
      BiasEnergy();
    }

    theDirection = theDirectionDistribution->GenerateDirection( this );
    theDirection /= theDirection.mag();
    if( bBiasDistributions ) {
      BiasDirection();
    }
    
    G4ParticleDefinition* partDef = (*ite)->GetProduct();
    G4ThreeVector mom = theDirection * std::sqrt(theEnergy*theEnergy + 2.*partDef->GetPDGMass()*theEnergy);
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << " GmIsotopeSource::GenerateVertex for decay " << (*ite)->GetProductName() << "  mom " << mom << " theEnergy " << theEnergy << G4endl;
#endif
    G4PrimaryParticle* particle = new G4PrimaryParticle( partDef, mom.x(), mom.y(), mom.z() );
    vtx->SetPrimary(particle);
  } 

  return vtx;
}
