#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistDirectionFan.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"

//---------------------------------------------------------------------
GmGenerDistDirectionFan::GmGenerDistDirectionFan()
{
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistDirectionFan::GenerateDirection( GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionFan::Generate:  initial dir " << theAxisDir << " starting angle (deg) " << theStartAngle/CLHEP::deg << " opening angle (deg) " << theOpeningAngle/CLHEP::deg << G4endl;
#endif
  G4ThreeVector referenceDirection(1., 0., 0.);
  if( fabs(theAxisDir * referenceDirection - 1) < 1e-8 ) {
    referenceDirection = G4ThreeVector(0.,1.,0.);
  }
  
  //---- Get phi angle around theAxisDir
  G4double phi = CLHEP::RandFlat::shoot()*theOpeningAngle + theStartAngle;

  G4ThreeVector newDir = thePerpDir; 
  newDir.rotate( phi , theAxisDir );

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionFan::Generate:  dir after phi rotation " << newDir << " phi " << phi/CLHEP::deg << " " << thePerpDir << G4endl;
#endif

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionFan::Generate:  final dir " << newDir << " phi " << phi/CLHEP::deg << G4endl;
#endif

  return newDir;

}

//---------------------------------------------------------------------
void GmGenerDistDirectionFan::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 4 ) {
    G4Exception(" GmGenerDistDirectionFan::SetParam",
		"Wrong argument",
		FatalErrorInArgument,
		"To set direction you have to add 4 parameters: DIR_X DIR_Y DIR_Z OPENING_ANGLE");  
  }

  theAxisDir = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );

  theOpeningAngle = GmGenUtils::GetValue(params[3]);

  //----- Get one perpendicular direction
  G4ThreeVector dir(1.,0.,0.);
  if( fabs(fabs(theAxisDir*dir) - 1.) < 1.E-9 ){
    dir = G4ThreeVector(0.,1.,0.);
  }
  thePerpDir = theAxisDir.cross(dir);

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionFan::SetParams theAxisDir " << theAxisDir << "thePerpDir " << thePerpDir << " opening angle " << theOpeningAngle << G4endl;
#endif


}

//---------------------------------------------------------------------
void GmGenerDistDirectionFan::SetDirection( G4ThreeVector dirIni )
{
  theAxisDir = dirIni;
  G4ThreeVector dirPP(1.,0.,0.);
  if( fabs(fabs(theAxisDir*dirPP) - 1.) < 1.E-9 ){
    dirPP = G4ThreeVector(0.,1.,0.);
  }
  thePerpDir = theAxisDir.cross(dirPP);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionFan::SetDirection theAxisDir " << theAxisDir << "thePerpDir " << thePerpDir << G4endl;
#endif
}
