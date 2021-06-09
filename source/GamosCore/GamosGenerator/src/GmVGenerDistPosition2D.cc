#include "GmVGenerDistPosition2D.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4GeometryTolerance.hh"
#include "CLHEP/Random/RandFlat.h"
#include "geomdefs.hh"


//---------------------------------------------------------------------
G4ThreeVector GmVGenerDistPosition2D::RotateAndTranslate( G4ThreeVector& posini )
{
  G4ThreeVector pos = posini;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmVGenerDistPosition2D::Generate pos before rotation " << pos << G4endl;
#endif
  pos = theRotation * pos;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmVGenerDistPosition2D::Generate pos before translation " << pos << G4endl;
#endif
  pos += theCentre;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmVGenerDistPosition2D::Generate pos " << pos << G4endl;
#endif

  return pos;
}
