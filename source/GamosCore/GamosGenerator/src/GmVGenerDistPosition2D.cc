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
  if( GenerVerb(debugVerb) ) G4cout << " GmVGenerDistPosition2D::Generate pos before translation " << pos << " + " << theCentre << G4endl;
#endif
  pos += theCentre;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmVGenerDistPosition2D::Generate pos " << pos << G4endl;
#endif

  return pos;
}

//---------------------------------------------------------------------
void GmVGenerDistPosition2D::SetRotation( G4RotationMatrix rot )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmVGenerDistPosition2D::SetRotation ROT " << rot << G4endl;
#endif
  theRotation = rot;
}
    
//---------------------------------------------------------------------
void GmVGenerDistPosition2D::SetRotation( G4ThreeVector dir )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmVGenerDistPosition2D::SetRotation DIR " << dir << G4endl;
#endif
  theRotation = G4RotationMatrix();
  if( fabs(dir.mag()-1.) > G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() ) {
    G4Exception("GmVGenerDistPosition2D::SetParams",
		"Warning",
		JustWarning,
		G4String("direction cosines are normalized to one, they were " + GmGenUtils::ftoa(dir.mag())).c_str());
    dir /= dir.mag();
  } 
  G4double angx;
  G4double angy;
  if( fabs(dir*G4ThreeVector(0.,0.,1.)-1.) < 1.E-6 ) {
    angx = 0.;
    angy = 0.;
  } else if( fabs(dir*G4ThreeVector(0.,0.,-1.)-1.) < 1.E-6 ) {
    angx = 180.*CLHEP::deg; 
    angy = 0.;
  } else if( fabs(dir*G4ThreeVector(0.,1.,0.)-1.) < 1.E-6 ) {
    angx = -90.*CLHEP::deg;
    angy = 0.;
  } else if( fabs(dir*G4ThreeVector(0.,-1.,0.)-1.) < 1.E-6 ) {
    angx = 90.*CLHEP::deg;
    angy = 0.;
  } else if( fabs(dir*G4ThreeVector(1.,0.,0.)-1.) < 1.E-6 ) {
    angx = 0.;
    angy = 90.*CLHEP::deg;
  } else if( fabs(dir*G4ThreeVector(-1.,0.,0.)-1.) < 1.E-6 ) {
    angx = 0.;
    angy = -90.*CLHEP::deg;
  } else {
    angx = -asin(dir.y());
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmVGenerDistPosition2D angx " << angx/CLHEP::deg << " " << dir.y() << G4endl; //GDEB
#endif
    // there are always two solutions angx, angy and PI-angx, PI+angy, choose first                             
    angy = asin( dir.x()/sqrt(1-dir.y()*dir.y()) );
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmVGenerDistPosition2D angy " << angy/CLHEP::deg << G4endl; 
#endif    
    // choose between  angy and PI-angy 
    if( dir.z() * cos(angx)*cos(angy) < 0 ) angy =CLHEP::pi - angy;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) )     G4cout << " GmVGenerDistPosition2D angy final " << angy/CLHEP::deg << G4endl;
#endif
   }
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmVGenerDistPosition2D::SetRotation init " << theRotation << G4endl;
#endif
  theRotation.rotateX( angx );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmVGenerDistPosition2D::SetRotation after rotationX " << angx << " " << theRotation << G4endl;
#endif
  theRotation.rotateY( angy );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmVGenerDistPosition2D::SetRotation after rotationY " << angy << " " << theRotation << G4endl;
#endif
}
