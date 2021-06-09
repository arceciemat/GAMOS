#include "globals.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "GmReadPhantomG4Geometry.hh"
#include "G4PhantomParameterisation.hh"
#include "G4GeometryTolerance.hh"

#include "G4tgbVolumeMgr.hh"
#include "G4tgbMaterialMgr.hh"
#include "G4tgrMessenger.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4tgrMessenger.hh"

//---------------------------------------------------------------------------
GmReadPhantomG4Geometry::GmReadPhantomG4Geometry()
{
}

//---------------------------------------------------------------------------
GmReadPhantomG4Geometry::~GmReadPhantomG4Geometry()
{
}

//---------------------------------------------------------------------------
void GmReadPhantomG4Geometry::ReadPhantomData()
{
  G4String filename = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:Phantom:FileName", "test.g4dcm");

  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  filename = GmGenUtils::FileInPath( path, filename );

  std::ifstream fin(filename.c_str());
  G4int nMaterials;
  fin >> nMaterials;
  G4String stemp;
  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 
  G4int nmate;
  for( G4int ii = 0; ii < nMaterials; ii++ ){
    fin >> nmate >> stemp;
    G4cout << "GmReadPhantomG4Geometry::ReadPhantomData reading nmate " << ii << " = " << nmate << " mate " << stemp << G4endl;
    if( ii != nmate ) G4Exception("GmReadPhantomG4Geometry::ReadPhantomData",
		"Wrong argument",
		FatalErrorInArgument,
				  "Material number should be in increasing order: wrong material number ");
    G4Material* mate = matmgr->FindOrBuildG4Material(stemp);
    thePhantomMaterialsOriginal[ii] = mate;
  }

  fin >> nVoxelX >> nVoxelY >> nVoxelZ;
  G4cout << "GmReadPhantomG4Geometry::ReadPhantomData nVoxel X/Y/Z " << nVoxelX << " " << nVoxelY << " " << nVoxelZ << G4endl;
  fin >> offsetX >> dimX;
  dimX = (dimX - offsetX)/nVoxelX;
  fin >> offsetY >> dimY;
  dimY = (dimY - offsetY)/nVoxelY;
  fin >> offsetZ >> dimZ;
  dimZ = (dimZ - offsetZ)/nVoxelZ;
  G4cout << "GmReadPhantomG4Geometry::ReadPhantomData voxelDimX " << dimX << " offsetX " << offsetX << G4endl;
  G4cout << "GmReadPhantomG4Geometry::ReadPhantomData voxelDimY " << dimY << " offsetY " << offsetY << G4endl;
  G4cout << "GmReadPhantomG4Geometry::ReadPhantomData voxelDimZ " << dimZ << " offsetZ " << offsetZ << G4endl;

  mateIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	fin >> stemp; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " <<  atoi(stemp.c_str())-1 << " " << stemp << G4endl;
	G4int mateID = atoi(stemp.c_str());
	if( mateID < 0 || mateID >= nMaterials ) {
	  G4Exception("GmReadPhantomG4Geometry::ReadPhantomData",
		      "Wrong index in phantom file",
		      FatalException,
		      G4String("It should be between 0 and "
			       + GmGenUtils::itoa(nMaterials-1) 
			       + ", while it is " 
			       + GmGenUtils::itoa(mateID)).c_str());
	}
	mateIDs[nnew] = mateID;
      }
    }
  }

  ReadVoxelDensities( fin );

  ReadPV( fin );

  fin.close();
}
