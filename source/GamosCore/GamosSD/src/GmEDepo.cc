#include "GmEDepo.hh"

#include "G4ParticleDefinition.hh"

//----------------------------------------------------------------------
GmEDepo::GmEDepo(G4double edepo, G4ThreeVector pos, G4ParticleDefinition* part, G4double tim ): theEnergy(edepo), thePosition(pos), theParticle(part), theTime( tim )
{
}



//----------------------------------------------------------------------
void GmEDepo::Print(std::ostream& os, G4bool bVerbose )
{
  if( bVerbose ) os << "Edepo ";
  if( bVerbose ) os << G4endl << "energy= ";
  os << theEnergy << " ";
  //     << " pos= " << thePosition.mag() << " " << thePosition.theta()/deg << " " if( bVerbose ) os<< thePosition.phi()/deg
  if( bVerbose ) os << G4endl << " posXYZ= ";
  os << thePosition.x() << " " << thePosition.y() << " " << thePosition.z() << " ";
  if( bVerbose ) os << G4endl;
}















