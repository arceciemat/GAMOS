#include "GmIsotope.hh"
#include "CLHEP/Random/RandFlat.h"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"

#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"
 
//-----------------------------------------------------------------------
GmIsotope::GmIsotope( const G4String& name ): theName( name )
{
}

//-----------------------------------------------------------------------
void GmIsotope::AddDecay( GmIsotopeDecay* dec )
{
  theDecays.push_back( dec );
}

//-----------------------------------------------------------------------
std::vector<GmIsotopeDecay*> GmIsotope::GenerateDecayProducts()
{
  std::vector<GmIsotopeDecay*> decs;

  if( theDecays.size() == 0 ) {
    G4Exception("GmIsotope::GenerateDecayProducts()",
		"",
		FatalException,
		("!!! Isotope "+theName+" has no decays").c_str());
  }
      
  while(decs.size() == 0 ){
    for( unsigned int ii=0; ii < theDecays.size(); ii++ ) {
      double rand = CLHEP::RandFlat::shoot();
      if( GenerVerb(debugVerb) ) G4cout << ii << " " << rand << " <? " << theDecays[ii]->GetProbability()  << " TEST " << theDecays[ii]->GetEnergy() << " GmIsotope::GenerateDecayProducts() trying decay " << theDecays[ii]->GetProductName() << " decs size " << decs.size() << G4endl;
      if( rand < theDecays[ii]->GetProbability() ){
	decs.push_back( theDecays[ii] );
	if( GenerVerb(debugVerb) ) G4cout << ii << " " << rand << " <? " << theDecays[ii]->GetProbability()  << " OK " << theDecays[ii]->GetEnergy() << " GmIsotope::GenerateDecayProducts() trying decay " << theDecays[ii]->GetProductName() << " decs size " << decs.size() << G4endl;

      }      
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << ii << " " << rand << " <? " << theDecays[ii]->GetProbability()  << "GmIsotope::GenerateDecayProducts() trying decay " << theDecays[ii]->GetProductName() << " decs size " << decs.size() << G4endl;
#endif
    }
  }

  return decs;
}
