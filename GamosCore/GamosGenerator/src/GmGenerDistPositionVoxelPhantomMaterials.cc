#include "GmGenerDistPositionVoxelPhantomMaterials.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
//#define private public
#include "G4PhantomParameterisation.hh"
//#define private private
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4PVParameterised.hh"
#include "G4AffineTransform.hh"
#include "G4Material.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4ThreeVector.hh"

//------------------------------------------------------------------------
GmGenerDistPositionVoxelPhantomMaterials::GmGenerDistPositionVoxelPhantomMaterials()
{
  theParamReg = 0;
}

//------------------------------------------------------------------------
void GmGenerDistPositionVoxelPhantomMaterials::SetParams( const std::vector<G4String>& params )
{
  if( params.size() == 0 ) {
    G4Exception("GmGenerDistPositionVoxelPhantomMaterials::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"No volume defined: you have to add at least one, as the last argument in the command line setting this distribution ");
  }

  GmGeometryUtils* geomUtils = GmGeometryUtils::GetInstance();
  //---- Add the G4PhantomParameterisation
  std::vector<G4VPhysicalVolume*>::const_iterator ite;
  G4PhysicalVolumeStore* pvstore = G4PhysicalVolumeStore::GetInstance();
  for( ite = pvstore->begin(); ite != pvstore->end(); ite++ ) {
    if ( (*ite)->IsReplicated() )  {
      EAxis axis;
      G4int nReplicas;
      G4double width,offset;
      G4bool consuming;      
      (*ite)->GetReplicationData(axis,nReplicas,width,offset,consuming);
      if( !consuming && (*ite)->GetRegularStructureId() == 1 ) {  
	const G4PVParameterised* pvparam = static_cast<const G4PVParameterised*>(*ite);
	G4VPVParameterisation* param = pvparam->GetParameterisation();
	theParamReg = static_cast<G4PhantomParameterisation*>(param);
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionVoxelPhantomMaterials volume found " << (*ite)->GetName() << G4endl;
#endif
	
	//---- Get container transformation
	GmTouchable* containerTouch = geomUtils->GetTouchables( (*ite)->GetMotherLogical()->GetName() )[0];
	theTransformation = G4AffineTransform( containerTouch->GetGlobalRotation(), containerTouch->GetGlobalPosition() );
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionVoxelPhantomMaterials container touchable " << *containerTouch << G4endl;
#endif
	
	G4VPhysicalVolume* containerPV = geomUtils->GetPhysicalVolumes( (*ite)->GetMotherLogical()->GetName() )[0];
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionVoxelPhantomMaterials container physvol " << containerPV->GetTranslation() << G4endl;
#endif
	
	//---- Get voxel dimensions
	theVoxelDimX = theParamReg->GetVoxelHalfX();
	theVoxelDimY = theParamReg->GetVoxelHalfY();
	theVoxelDimZ = theParamReg->GetVoxelHalfZ();
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionVoxelPhantomMaterials voxel dimensions " << theVoxelDimX << " " << theVoxelDimY << " " << theVoxelDimZ << G4endl;
#endif
	theNoVoxelX = theParamReg->GetNoVoxelX();
	theNoVoxelY = theParamReg->GetNoVoxelY();
	theNoVoxelZ = theParamReg->GetNoVoxelZ();
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionVoxelPhantomMaterials number of voxels " << theNoVoxelX << " " << theNoVoxelY << " " << theNoVoxelZ << G4endl;
#endif
	
	//---- Add the allowed materials of the voxels
	std::vector<G4Material*> paramMates = theParamReg->GetMaterials();
	for( unsigned int ii = 0; ii < params.size(); ii++ ){
	  G4Material* mate = geomUtils->GetMaterial( params[ii], true );	    
	  unsigned int jj;
	  for( jj = 0; jj < paramMates.size(); jj++ ) {
	    if( paramMates[jj] == mate ) { 
	      theMaterialIndexes.insert( jj );
#ifndef GAMOS_NO_VERBOSE
	      if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionInG4Volumes material added " << params[ii] << " index " << jj << G4endl;
#endif
	      break;
	    }
	  }
	  if( jj == paramMates.size() ) {
	    G4cerr << "LIST OF MATERIALS in G4PhantomParameterisation " << G4endl;
	    for( unsigned int kk = 0; kk < paramMates.size(); kk++ ) {
	      G4cerr << "MATE: " << paramMates[kk]->GetName() << G4endl;
	    }
	    G4Exception("GmGenerDistPositionVoxelPhantomMaterials::SetParams",
			"Wrong argument",
			FatalErrorInArgument,
			G4String("material not found: "+params[ii]).c_str() );
	  }
	}
	break; // do not look for another volume
      }
    }
  }

  if( !theParamReg ) {
    G4Exception("GmGenerDistPositionVoxelPhantomMaterials::SetParams",
		"Wrong distirbution",
		FatalErrorInArgument,
		"No G4PhantomParameterisation exists");
  } 
}


//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionVoxelPhantomMaterials::GeneratePosition( const GmParticleSource* )
{
  //----- Select randomly a voxel
  G4bool materialOK = false;
  unsigned int nvx, nvy, nvz, nvoxel;
  do{
    nvx = (unsigned int)(CLHEP::RandFlat::shoot() * theNoVoxelX);
    nvy = (unsigned int)(CLHEP::RandFlat::shoot() * theNoVoxelY);
    nvz = (unsigned int)(CLHEP::RandFlat::shoot() * theNoVoxelZ);
    nvoxel = theParamReg->GetMaterialIndex(nvx,nvy,nvz);
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionVoxelPhantomMaterials::GeneratePosition voxel numbers " << nvx << " " << nvy << " " << nvz << G4endl;
#endif
    if( theMaterialIndexes.find(nvoxel) != theMaterialIndexes.end() ) materialOK = true;
  } while( !materialOK );

  //---- Get x,y,z coordinates in voxel
  
  G4ThreeVector pos( -theVoxelDimX + 2*CLHEP::RandFlat::shoot()*theVoxelDimX,
		     -theVoxelDimY + 2*CLHEP::RandFlat::shoot()*theVoxelDimY,
		     -theVoxelDimZ + 2*CLHEP::RandFlat::shoot()*theVoxelDimZ );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionVoxelPhantomMaterials::GeneratePosition  pos before transformation " << pos << G4endl;
#endif

  pos += theParamReg->GetTranslation( nvy + theParamReg->GetNoVoxelY()*nvx + + theParamReg->GetNoVoxelX()*theParamReg->GetNoVoxelY()*nvz );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionVoxelPhantomMaterials::GeneratePosition  pos after voxel transformation " << pos << G4endl;
#endif

  theTransformation.ApplyPointTransform(pos);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionVoxelPhantomMaterials::GeneratePosition  pos after container transformation " << pos << G4endl;
#endif

  return pos;
}
