#define _USE_MATH_DEFINES
#include <cmath>
#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistPositionDirectionTowardsBox.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmPlane.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"

#include "CLHEP/Random/RandFlat.h"

//---------------------------------------------------------------------
GmGenerDistPositionDirectionTowardsBox::GmGenerDistPositionDirectionTowardsBox()
{
  theTotalStereoAngle = 0.;
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDirectionTowardsBox::GeneratePosition( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirectionTowardsBox::GeneratePosition " << thePoint << G4endl;
#endif
 return thePoint;
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDirectionTowardsBox::GenerateDirection( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirectionTowardsBox::Generate " << G4endl;
#endif
  
  //----- Select touchable
  G4double rvol = CLHEP::RandFlat::shoot()* theTotalStereoAngle;
  G4int itou = 0;
  G4double tempSA = 0.;
  for( unsigned int ii = 0; ii < theTouchInfos.size(); ii++ ){
    tempSA += theTouchInfos[ii]->stereoAngle;
    if( tempSA >= rvol ) {
      itou = ii; 
      break;
    }
  }

  //----- Select cosine x and y
  TouchInfo* tinfo = theTouchInfos[itou];
  G4double dcx = tinfo->minX + CLHEP::RandFlat::shoot()* (tinfo->maxX-tinfo->minX );
  G4double dcy = tinfo->minY + CLHEP::RandFlat::shoot()* (tinfo->maxY-tinfo->minY );

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "dcx " << dcx << " " << tinfo->minX << " " << tinfo->maxX << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "dcy " << dcy << " " << tinfo->minY << " " << tinfo->maxY << G4endl;
#endif
  G4ThreeVector dir( dcx, dcy, sqrt(1.-dcx*dcx+dcy*dcy) );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirectionTowardsBox::GenerateDirection before rot " << dir << "   rotmPlane " <<  tinfo->rotmPlane << G4endl;
#endif

  //G4ThreeVector dir( dcx, dcy, 0. );
  dir = tinfo->rotmPlane * dir;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirectionTowardsBox::GenerateDirection " << dir << "   touch dir " << tinfo->dir << G4endl;
#endif

  return dir;

}

//---------------------------------------------------------------------
void GmGenerDistPositionDirectionTowardsBox::SetParams( const std::vector<G4String>& params )
{
  if( params.size() < 4 ) {
    G4Exception(" GmGenerDistPositionDirectionTowardsBox::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"To set direction you have to add as parameters the position and one or more volumes");
  }

  thePoint = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );

  for( unsigned int ii = 3; ii < params.size(); ii++ ){
    std::vector<GmTouchable*> touchables = GmGeometryUtils::GetInstance()->GetTouchables( params[ii] );
    std::vector<GmTouchable*>::const_iterator ite;
    for( ite = touchables.begin(); ite != touchables.end(); ite++) {
      SetDirCosineLimits( *ite );
      delete *ite;
    }
  }

}
//---------------------------------------------------------------------
void GmGenerDistPositionDirectionTowardsBox::SetDirCosineLimits( const GmTouchable* touch )
{
  TouchInfo* tinfo = new TouchInfo;
  theTouchInfos.push_back( tinfo );
  tinfo->name = touch->GetLongName();

  //----- Get Plane perpendicular to line towards touchable centre
  G4ThreeVector centre =  touch->GetGlobalPosition();
  G4ThreeVector direction = thePoint - centre;
  G4double distCP = direction.mag();
  direction /= distCP;
  tinfo->dir = -direction;
  GmPlane plane(direction, centre);
  direction *= -1.;
  //--- Define rotation matrix for plane
  G4ThreeVector tempv(1.,0.,0.);
  if( fabs(1.-fabs(direction * tempv)) < 1.E-6 ) tempv = G4ThreeVector(0.,1.,0.);
  G4ThreeVector planeVec1 = direction.cross(tempv);
  G4ThreeVector planeVec2 = direction.cross(planeVec1);
  G4RotationMatrix rotmPlane( planeVec1, planeVec2, direction );
  G4RotationMatrix rotmPlaneInv = rotmPlane.inverse();
  G4cout << " rotmPlane " << rotmPlane << G4endl;
  G4cout << " rotmPlaneInv " << rotmPlane.inverse() << G4endl;
  G4cout << " direction " << direction << " planeVec1 " << planeVec1 << " planeVec2 " << planeVec2 << G4endl;
  tinfo->rotmPlane = rotmPlane;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirectionTowardsBox name " << tinfo->name << " centre " << centre << " dir " << direction << " distCP " << distCP << G4endl;
#endif
  
  //----- Get eight corners and project the line passing from thePoint onto the plane
  if( touch->GetSolid()->GetEntityType() != "G4Box" ) {
    G4Exception("GmGenerDistPositionDirectionTowardsBox::SetDirCosineLimits",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Solid " + touch->GetLongName()).c_str());
  }
  G4Box* box = (G4Box*)(touch->GetSolid());
  G4double dimx = box->GetXHalfLength();
  G4double dimy = box->GetYHalfLength();
  G4double dimz = box->GetZHalfLength();
  G4ThreeVector dirx(1.,0.,0.);
  G4ThreeVector diry(0.,1.,0.);
  G4ThreeVector dirz(0.,0.,1.);
  G4RotationMatrix rotm = touch->GetGlobalRotation();
  G4RotationMatrix rotmInv = rotm.inverse();
  dirx = rotm*dirx * dimx;
  diry = rotm*diry * dimy;
  dirz = rotm*dirz * dimz;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) {
    G4cout << " rotm " << rotm << G4endl;
    G4cout << " rotmInv " << rotmInv << G4endl;
    G4cout << " dimx " << dimx << G4endl;
    G4cout << " dimy " << dimy << G4endl;
    G4cout << " dimz " << dimz << G4endl;
    G4cout << " dirx " << dirx << G4endl;
    G4cout << " diry " << diry << G4endl;
    G4cout << " dirz " << dirz << G4endl;
  }
#endif
  G4double minX = DBL_MAX;
  G4double maxX = -DBL_MAX;
  G4double minY = DBL_MAX;
  G4double maxY = -DBL_MAX;
  
  for( int ix = -1; ix <= 1; ix+=2 ){
    for( int iy = -1; iy <= 1; iy+=2 ){
      for( int iz = -1; iz <= 1; iz+=2 ){
	//project line 
	G4ThreeVector corner = centre + dirx*ix + diry*iy + dirz*iz;
	G4ThreeVector dir = corner-thePoint;
	dir = dir.unit(); 
	G4ThreeVector inters = plane.Intersect( thePoint, dir );
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) {
	  G4cout << " GmGenerDistPositionDirectionTowardsBox " << ix << " " << iy << " " << iz << " corner " << corner << " inters global " << inters << G4endl;
	  G4cout << " GmGenerDistPositionDirectionTowardsBox inters " << inters << " point " << thePoint << " dir " << dir << G4endl;
	}
#endif
	//-- Transform to local coordinates
	inters -= centre;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) )
	  G4cout << " GmGenerDistPositionDirectionTowardsBox inters global-centre " << inters << " " <<  rotmPlane * inters <<" " << rotmPlaneInv * inters << G4endl;
#endif
	inters = rotmPlaneInv * inters;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) )
	  G4cout << " GmGenerDistPositionDirectionTowardsBox inters local " << inters << G4endl;
#endif
	if( inters.x() < minX ) minX = inters.x();
	if( inters.x() > maxX ) maxX = inters.x();
	if( inters.y() < minY ) minY = inters.y();
	if( inters.y() > maxY ) maxY = inters.y();
      }
    }
  }
  
  //----- Set cosine limits
  minX = minX / sqrt(minX*minX+distCP*distCP);
  maxX = maxX / sqrt(maxX*maxX+distCP*distCP);
  minY = minY / sqrt(minY*minY+distCP*distCP);
  maxY = maxY / sqrt(maxY*maxY+distCP*distCP);

  tinfo->minX = minX;
  tinfo->maxX = maxX;
  tinfo->minY = minY;
  tinfo->maxY = maxY;

  //----- Get stereo angle included in these cosine limits
  unsigned int nPhiBins = 1000;
  unsigned int nThetaBins = 1000;
  G4double phiBin = 2*M_PI/nPhiBins;
  G4double thetaBin = M_PI/nThetaBins;
  G4double phi, theta;
  G4double dcx, dcy;
  G4double stang = 0.;
  for(unsigned int ip = 0; ip < nPhiBins; ip++ ){
    for( unsigned int it = 0; it < nThetaBins; it++ ){
      phi = (ip+0.5) *phiBin;
      theta = (it+0.5) *thetaBin;
      dcx = sin(theta)*sin(phi);
      dcy = sin(theta)*cos(phi);
      if( dcx > minX && dcx <= maxX  &&
	  dcy > minY && dcy <= maxY ) {
	stang += sin(theta)*phiBin*thetaBin;
	//	G4cout << " add stang " << stang << G4endl;
      }
      //      G4cout << " integral " << ip << " " << it << " phi " << phi << " theta " << theta << " dcx " << dcx << " dcy " << dcy << " stang " << stang << G4endl;
      // G4cout << " minX " << minX << " maxX " << maxX << " minY " << minY << " maxY " << maxY << G4endl;
    }
  }
  tinfo->stereoAngle = stang;
  theTotalStereoAngle += stang;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " TINFO: " << tinfo->name << " dir " << tinfo->dir << " stereoAngle " << tinfo->stereoAngle << " minX " << tinfo->minX << " maxX " << tinfo->maxX << " minY " << tinfo->minY << " maxY " << tinfo->maxY << G4endl;
#endif
}
