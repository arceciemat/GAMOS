#include "GmGenerDistPositionDirectionInVolumeSurface.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4VSolid.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"

//------------------------------------------------------------------------
GmGenerDistPositionDirectionInVolumeSurface::GmGenerDistPositionDirectionInVolumeSurface()
{
  //  theName = "GmGenerDistPositionDirectionInVolumeSurface";
  theTranslation = G4ThreeVector();
  theRotation = 0;
}

//------------------------------------------------------------------------
void GmGenerDistPositionDirectionInVolumeSurface::SetParams( const std::vector<G4String>& params )
{
  if( params.size() == 0 ) {
    G4Exception("GmGenerDistPositionDirectionInVolumeSurface::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"No solid defined: you have to add at least one, and then the solid parameters in the command line setting this distribution ");
  }

  theSolidType = params[0];
  unsigned int iParams = 1;
  if( theSolidType == "BOX" ) {
    for( unsigned int ii = 1; ii < 4; ii++ ){
      theParams.push_back( GmGenUtils::GetValue( params[ii] ) );
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirectionInVolumeSurface param added " << theParams[ii-1] << G4endl;
#endif
      iParams++;
    }
  } else {
    G4Exception("GmGenerDistPositionDirectionInVolumeSurface::GeneratePosInSolid solid",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Type not supported: " + theSolidType ).c_str());
  }
  
  if( params.size() > iParams ) {
    if( params.size() - iParams == 3 ) {
      theTranslation = G4ThreeVector( GmGenUtils::GetValue(params[iParams]), GmGenUtils::GetValue(params[iParams+1]), GmGenUtils::GetValue(params[iParams+2]) );
    } else if( params.size() - iParams == 6 ) {
      theRotation = new G4RotationMatrix;
      theRotation->rotateX( GmGenUtils::GetValue(params[iParams+3]) );
      theRotation->rotateY( GmGenUtils::GetValue(params[iParams+4]) );
      theRotation->rotateZ( GmGenUtils::GetValue(params[iParams+5]) );
    } else {
      G4Exception("GmGenerDistPositionDirectionInVolumeSurface::SetParams",
		  "Wrong argument",
		  FatalErrorInArgument,
		  "Incorrect number of parameters, it should be SOLID_TYPE n*SOLID_PARAM POS_X POS_Y POS_Z ANG_X ANG_Y ANG_Z");
    }
  }
  
}
  
//------------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDirectionInVolumeSurface::GeneratePosition( const GmParticleSource* )
{
  thePosition = G4ThreeVector();

  G4double x = 0.;
  G4double y = 0.;
  G4double z = 0.;
  
  if( theSolidType == "BOX" ) {
  
    // Radius and axial size 
    G4int side = G4int(CLHEP::RandFlat::shoot()*6.);
    if( side == 0 ) {
      x = -theParams[0];
      y = -theParams[1]+2*theParams[1]*CLHEP::RandFlat::shoot();
      z = -theParams[2]+2*theParams[2]*CLHEP::RandFlat::shoot();
    } else if( side == 1 ) {
      x = theParams[0];
      y = -theParams[1]+2*theParams[1]*CLHEP::RandFlat::shoot();
      z = -theParams[2]+2*theParams[2]*CLHEP::RandFlat::shoot();
    } else if( side == 2 ) {
      y = -theParams[1];
      x = -theParams[0]+2*theParams[0]*CLHEP::RandFlat::shoot();
      z = -theParams[2]+2*theParams[2]*CLHEP::RandFlat::shoot();
    } else if( side == 3 ) {
      y = theParams[1];
      x = -theParams[0]+2*theParams[0]*CLHEP::RandFlat::shoot();
      z = -theParams[2]+2*theParams[2]*CLHEP::RandFlat::shoot();
    } else if( side == 4 ) {
      z = -theParams[2];
      y = -theParams[1]+2*theParams[1]*CLHEP::RandFlat::shoot();
      x = -theParams[0]+2*theParams[0]*CLHEP::RandFlat::shoot();
    } else if( side == 5 ) {
      z = theParams[2];
      y = -theParams[1]+2*theParams[1]*CLHEP::RandFlat::shoot();
      x = -theParams[0]+2*theParams[0]*CLHEP::RandFlat::shoot();
    }
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirectionInVolumeSurface side " << side << " pos " << G4ThreeVector(x,y,z) << " params " << theParams[0] << G4endl;
#endif

  }

  thePosition = G4ThreeVector(x,y,z);
  thePosition += theTranslation;
  if( theRotation ) {
    thePosition = (*theRotation) * thePosition;
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirectionInVolumeSurface pos " << G4ThreeVector(x,y,z) << " theTranslation " << theTranslation;
#endif
  if( theRotation ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " theRotation " << *theRotation << G4endl;
#endif
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << G4endl;
#endif
  }
  return thePosition;
}

//------------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDirectionInVolumeSurface::GenerateDirection( const GmParticleSource* source)
{
  G4ThreeVector dir = -source->GetPosition();
  if( theRotation ) {
    //  dir = (*(theRotation->Inverse()) ) * dir;
    dir = CLHEP::inverseOf(*theRotation) * dir;
  }
  dir -= theTranslation;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirectionInVolumeSurface dir " << dir << G4endl; 
#endif

  return dir;

}
