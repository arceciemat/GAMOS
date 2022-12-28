#include "GmGenerDistPositionPETImage.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GmReadPETDicom.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4GeometryTolerance.hh"

//------------------------------------------------------------------------
GmGenerDistPositionPETImage::GmGenerDistPositionPETImage()
{
  G4Exception("GmGenerDistPositionPETImage::GmGenerDistPositionPETImage",
	      "",
	      JustWarning,
	      "GmGenerDistPositionPETImage is deprecated and will soon be removed from the GAMOS release, please use GmGenerDistPositionNMImage instead");
}

//------------------------------------------------------------------------
void GmGenerDistPositionPETImage::Initialise()
{
  GmReadPETDicom* petData = new GmReadPETDicom();

  petData->ReadData( theFileName );

  size_t* activities = petData->GetActivities();
  G4double probSUM = 0.;
  G4int nvox = petData->GetNoVoxels();
  std::multiset<G4double> probT;
  for( G4int ii = 0; ii < nvox; ii++ ) {
    probSUM += activities[ii];
    probT.insert(probSUM);
    //    G4cout << ii << " PROBT " << probSUM << G4endl; //GDEB
  }

  std::multiset<G4double>::iterator ite ;
  for( ite = probT.begin(); ite != probT.end(); ite++ ) {
    theProb.insert((*ite)/probSUM);
    //    G4cout << std::distance(probT.begin(),ite) << " THEPROB " << (*ite)/probSUM << " " << *ite << G4endl; //GDEB
    
  }
  theNVoxelX = petData->GetNoVoxelsX();
  theNVoxelY = petData->GetNoVoxelsY();
  theNVoxelZ = petData->GetNoVoxelsZ();
  theMinX = petData->GetMinX();
  theMinY = petData->GetMinY();
  theMinZ = petData->GetMinZ();
  theVoxelDimX = petData->GetVoxelHalfX()*2;
  theVoxelDimY = petData->GetVoxelHalfY()*2;
  theVoxelDimZ = petData->GetVoxelHalfZ()*2;

  /*  for( G4int ii = 0; ii < nvox; ii++ ) {
    G4int copyNo = ii;
    G4int nvx = copyNo % theNVoxelX;
    G4int nvy = copyNo/theNVoxelX % theNVoxelY;
    G4int nvz = copyNo/theNVoxelX/theNVoxelY % theNVoxelZ;
       G4cout << " PIXEL " << copyNo << " " << nvx << " " << nvy << " " << nvz << " " << activities[ii] << " " << G4endl; //GDEB
       } 
  std::multiset<G4double>::iterator ite2;
  for( ite = theProb.begin(); ite != theProb.end(); ite++ ) {
    ite2 = ite; ite2--;
    //    if( ite != theProb.begin() )   G4cout << "  PROBSUM " << std::distance(theProb.begin(),ite) << " " << *ite << " > " << *ite-*ite2 << G4endl;//GDEB
    } */
} 


//------------------------------------------------------------------------
void GmGenerDistPositionPETImage::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 1 && params.size() != 4 && params.size() != 7 ) {
    G4Exception(" GmGenerDistPositionPETImage::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"To set point you have to add 1, 4 or 7 parameters: FILE_NAME (POS_X POS_Y POS_Z) (DIR_X DIR_Y DIR_Z");
  }
  theFileName = params[0];

  if( params.size() >= 4 ) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ), GmGenUtils::GetValue( params[3] ) );
  }
  if( params.size() >= 7 ) {
    //normalize direction cosines
    G4ThreeVector dir(GmGenUtils::GetValue( params[4] ), GmGenUtils::GetValue( params[5] ), GmGenUtils::GetValue( params[6] ) );
    if( fabs(dir.mag()-1.) > G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() ) {
      G4Exception("GmGenerDistPositionPETImage::SetParams",
		  "Warning",
		  JustWarning,
		  G4String("direction cosines are normalized to one, they were " + GmGenUtils::ftoa(dir.mag())).c_str());
      dir /= dir.mag();
    } 
    G4double angx = -asin(dir.y());
    // there are always two solutions angx, angy and PI-angx, PI+angy, choose first
    G4double angy;
    if( dir.y() == 1. ) {
      angy = 0.;
    } else if( dir.y() == 0. ) {
      angy = 0.;
    } else {
      angy = asin( dir.x()/sqrt(1-dir.y()*dir.y()) );
    }

    // choose between  angy and PI-angy
    if( dir.z() * cos(angx)*cos(angy) < 0 ) angy = CLHEP::pi - angy;

    theRotation.rotateX( angx );
    theRotation.rotateY( angy );
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << " GmGenerDistPositionPETImage::SetParams file " << theFileName << " translation " << theCentre << " rotation " << theRotation << G4endl;
#endif

  Initialise();
}


//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionPETImage::GeneratePosition( GmParticleSource* )
{
  
  //----- Get x,y,z coordinates in voxel
  G4ThreeVector pos( -theVoxelDimX/2. + CLHEP::RandFlat::shoot()*theVoxelDimX,
		     -theVoxelDimY/2. + CLHEP::RandFlat::shoot()*theVoxelDimY,
		     -theVoxelDimZ/2. + CLHEP::RandFlat::shoot()*theVoxelDimZ );
  //----- Select randomly a voxel
  G4double randN = CLHEP::RandFlat::shoot();
  std::multiset<G4double>::iterator ite = theProb.upper_bound( randN );
  G4int copyNo = std::distance( theProb.begin(), ite );

  G4int nvx = copyNo % theNVoxelX;
  G4int nvy = copyNo/theNVoxelX % theNVoxelY;
  G4int nvz = copyNo/theNVoxelX/theNVoxelY % theNVoxelZ;
  
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << "GmGenerDistPositionPETImage::GeneratePosition  pos before transformation " << pos << G4endl;
#endif

  pos += G4ThreeVector(theMinX + theVoxelDimX*(nvx+0.5), theMinY + theVoxelDimY*(nvy+0.5), theMinZ + theVoxelDimZ*(nvz+0.5) );

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(testVerb) )  {
    G4cout << randN << " theMinX " << theMinX << " theVoxelDimX " << theVoxelDimX << "nvx  " << nvx << " = " << theMinX + theVoxelDimX*nvx << " copyNo " << copyNo << G4endl; 
    G4cout << " theMinY " << theMinY << " theVoxelDimY " << theVoxelDimY << "nvy  " << nvy << " = " << theMinY + theVoxelDimY*nvy << " copyNo " << copyNo << G4endl;
    G4cout << " theMinZ " << theMinZ << " theVoxelDimZ " << theVoxelDimZ << "nvz  " << nvz << " = " << theMinZ + theVoxelDimZ*nvz << " copyNo " << copyNo << G4endl;
  }
#endif
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << "GmGenerDistPositionPETImage::GeneratePosition  pos after voxel transformation " << pos << G4endl;
#endif

    pos = theRotation * pos;
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << " GmGenerDistPositionPETImage::Generate pos before translation " << pos << G4endl;
#endif
  pos += theCentre;
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ) G4cout << " GmGenerDistPositionPETImage::Generate pos FINAL " << pos << G4endl;
#endif

  return pos;
}
