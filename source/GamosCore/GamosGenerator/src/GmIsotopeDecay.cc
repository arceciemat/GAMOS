#include "GmIsotopeDecay.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4ParticleTable.hh"
#include "G4tgrUtils.hh"

GmIsotopeDecay::GmIsotopeDecay( const G4String& energy, const G4String& prob, const G4String& product )
{
  if( !GmGenUtils::IsNumber( energy ) ){
    G4Exception("GmIsotopeDecay::GmIsotopeDecay",
		    "Wrong argument",
		    FatalErrorInArgument,
		G4String("Energy should be given as a number " + energy).c_str());
  }
  theEnergy = G4tgrUtils::GetDouble( energy, CLHEP::keV); 

  if( !GmGenUtils::IsNumber( prob ) ){
    G4Exception("GmIsotopeDecay::GmIsotopeDecay",
		    "Wrong argument",
		    FatalErrorInArgument,
		G4String("Probability should be given as a number " + prob).c_str());
  }
  theProbability = GmGenUtils::GetValue( prob );

  G4ParticleTable* partTable = G4ParticleTable::GetParticleTable(); 
  theProduct = partTable->FindParticle(product);
  if( !theProduct ) {
    G4Exception("GmIsotopeDecay::GmIsotopeDecay","Error in argument",FatalErrorInArgument,(" particle does not exist: " + product).c_str() );
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmIsotopeDecay::GmIsotopeDecay  product " << theProduct << " energy " << theEnergy << " prob " << theProbability << G4endl;
#endif
}


G4String GmIsotopeDecay::GetProductName() const
{
  return theProduct->GetParticleName();
}
