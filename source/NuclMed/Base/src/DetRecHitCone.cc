#include "DetRecHitCone.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------------
DetRecHitCone::DetRecHitCone(const GmRecHit* rhit1, GmRecHit* rhit2 )
{
  thePoint = rhit1->GetPosition();
  theDir = rhit2->GetPosition() - rhit1->GetPosition();
  theDir = theDir.unit();

  // theAngle = acos( (rhit2->GetEnergy() - rhit1->GetEnergy() ) / rhit2->GetEnergy() );

  // cos (theAngle) = 1 - mc2 * (Etracker) / ((Ecalor)*(Etracker+Ecalor))
  // mc2 = electron rest mass equivalent energy (=511 CLHEP::keV)
  // PE absorption in 2 is assumed, so that:
  // total Egamma (initial photon) = Etracker(Compton)+Ecalor(PE)

  //  G4double ener1 = rhit1->GetEnergy();
  G4double ener2 = rhit2->GetEnergy();
  // assume first energy is 511 CLHEP::keV
  G4double firstEner = GmParameterMgr::GetInstance()->GetNumericValue("DetRecHitCone:InitialHitEnergy",CLHEP::electron_mass_c2);
  //  theAngle = acos (1.- CLHEP::electron_mass_c2 * (firstEner-ener2) / ( ener2 * firstEner ) ); 
  theAngle = acos (1.- (firstEner-ener2) / ener2 ); 
  
#ifndef GAMOS_NO_VERBOSE
  if( DetVerb(infoVerb) ) G4cout << " Created DetRecHitCone pnt " << thePoint << " dir " << theDir << " angle " << theAngle/CLHEP::deg << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
  if( DetVerb(infoVerb) ) G4cout << " 1 cone " << *rhit1 << " 2 cone " << *rhit2 << G4endl;
#endif
  //-  G4cout << " angle " << ener1 / ( ener2 * (ener1 + ener2) ) << "  " << ener1 << " " << ( ener2 * (ener1 + ener2) ) << G4endl;

}
