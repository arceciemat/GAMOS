#define _USE_MATH_DEFINES
#include <cmath>

#include "GmGenerDistPositionInVoxelsFromFile.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4GeometryTolerance.hh"
#include "CLHEP/Random/RandFlat.h"
#include "geomdefs.hh"

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionInVoxelsFromFile::GeneratePosition( const GmParticleSource* )
{

  G4double rnd = CLHEP::RandFlat::shoot();

  mdi::const_iterator ite = theProbAccum.upper_bound(rnd);
  
  G4int nVox = (*ite).second;

  G4int nx = size_t(nVox%theNVoxelX);
  G4int ny = size_t( (nVox/theNVoxelX)%theNVoxelY );
  G4int nz = size_t(nVox/theNVoxelXY);

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionInVoxelsFromFile::Generate rnd " << rnd 
				    << " nVox " << nVox 
				    << " nx " << nx 
				    << " ny " << ny
				    << " nz " << nz 
				    << G4endl;
#endif

  G4ThreeVector pos( theOffsetX + theDimX*(nx + CLHEP::RandFlat::shoot()-0.5),
		     theOffsetY + theDimY*(ny + CLHEP::RandFlat::shoot()-0.5),
		     theOffsetZ + theDimZ*(nz + CLHEP::RandFlat::shoot()-0.5));

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionInVoxelsFromFile::Generate pos before rotation " << pos << G4endl;
#endif
  pos = theRotation * pos;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionInVoxelsFromFile::Generate pos before translation " << pos << G4endl;
#endif
  pos += theCentre;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionInVoxelsFromFile::Generate pos " << pos << G4endl;
#endif

  return pos;

}


//---------------------------------------------------------------------
void GmGenerDistPositionInVoxelsFromFile::SetParams( const std::vector<G4String>& params )
{

  if( params.size() != 1 && params.size() != 4 && params.size() != 7 ) {
    G4Exception(" GmGenerDistPositionInVoxelsFromFile::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"You need to defined a parameter: FILE_NAME (POS_X POS_Y POS_Z) (ROT_X ROT_Y ROT_Z");
  }
 
  G4String fileName = params[0];
 
  // open file 
  std::ifstream fin(fileName);
  
  fin >> theNVoxelX >> theNVoxelY >> theNVoxelZ;
  if( fin.eof() ) ErrorTooShortFile();
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ){
    G4cout << "GmGenerDistPositionInVoxelsFromFile::SetParams nVoxel X/Y/Z " << theNVoxelX << " " << theNVoxelY << " " << theNVoxelZ << G4endl;
  }
#endif
  
  fin >> theOffsetX >> theDimX;
  if( fin.eof() ) ErrorTooShortFile();
  theDimX = (theDimX - theOffsetX)/theNVoxelX;
  fin >> theOffsetY >> theDimY;
  if( fin.eof() ) ErrorTooShortFile();
  theDimY = (theDimY - theOffsetY)/theNVoxelY;
  fin >> theOffsetZ >> theDimZ;
  if( fin.eof() ) ErrorTooShortFile();
  theDimZ = (theDimZ - theOffsetZ)/theNVoxelZ;
  theNVoxelXY = theNVoxelX * theNVoxelY;
  // displace the offset by half voxel for faster calculatiosn
  theOffsetX += theDimX/2.;
  theOffsetY += theDimY/2.;
  theOffsetZ += theDimZ/2.;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ){
    G4cout << "GmGenerDistPositionInVoxelsFromFile::SetParams voxelDimX " << theDimX << " OffsetX " << theOffsetX << G4endl;
    G4cout << "GmGenerDistPositionInVoxelsFromFile::SetParams voxelDimY " << theDimY << " OffsetY " << theOffsetY << G4endl;
    G4cout << "GmGenerDistPositionInVoxelsFromFile::SetParams voxelDimZ " << theDimZ << " OffsetZ " << theOffsetZ << G4endl;
  }
#endif
  
  theProbSum = 0.;
  G4double prob1;
  G4int nVoxels = theNVoxelX * theNVoxelY * theNVoxelZ;
  std::map<G4double,G4int> probAccum;
  for( G4int ii = 0; ii < nVoxels; ii++ ){
    fin >> prob1;
    if( fin.eof() && ii != nVoxels-1 ) ErrorTooShortFile();
    if( prob1 > 0. ) {
      theProbSum += prob1;
      probAccum[theProbSum] = ii;
    }
  }

  //-- Normalize probabilities
  std::map<G4double,G4int>::iterator ite;
  for( ite = probAccum.begin(); ite != probAccum.end(); ite++  ){
    theProbAccum[ (*ite).first / theProbSum ] = (*ite).second;
  }


  //--- Get translation 
  if( params.size() >= 4 ) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ), GmGenUtils::GetValue( params[3] ) );
  }

  //--- Get rotation
  if( params.size() >= 7 ) {
    theRotation.rotateX( GmGenUtils::GetValue( params[4] ) );
    theRotation.rotateY( GmGenUtils::GetValue( params[5] ) );
    theRotation.rotateZ( GmGenUtils::GetValue( params[6] ) );
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionInVoxelsFromFile::SetParams  translation " << theCentre << " rotation " << theRotation << G4endl;
#endif


}

//---------------------------------------------------------------------
void GmGenerDistPositionInVoxelsFromFile::ErrorTooShortFile()
{

  G4Exception("GmGenerDistPositionInVoxelsFromFile::SetParams",
	      "Wrong argument",
	      FatalErrorInArgument,
	      "Too short file, please check it");
}
