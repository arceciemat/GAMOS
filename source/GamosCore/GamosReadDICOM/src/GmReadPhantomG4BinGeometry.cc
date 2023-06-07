#include "G4Material.hh"

#include "GmReadPhantomG4BinGeometry.hh"
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
GmReadPhantomG4BinGeometry::GmReadPhantomG4BinGeometry()
{
}

//---------------------------------------------------------------------------
GmReadPhantomG4BinGeometry::~GmReadPhantomG4BinGeometry()
{
  thePhantomFileName = "test.g4dcmb";
}


//---------------------------------------------------------------------------
void GmReadPhantomG4BinGeometry::ReadPhantomData()
{
   G4String filename = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:Phantom:FileName", thePhantomFileName);
  
  filename = GmGenUtils::FileInPath( filename );
  FILE* fin = std::fopen(filename,"rb");
  
  size_t nMaterials;
  if( fread(&nMaterials, sizeof(size_t),  1, fin) != 1) {
    G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		"Error",
		FatalException,
		"Problem reading number of materials");
  }
  //  G4cout << "GmReadPhantomG4BinGeometry::ReadPhantomDat  N materials " << nMaterials << " " << sizeof(size_t) << G4endl;
  char sc;
  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 
  for( size_t ii = 0; ii < nMaterials; ii++ ){
    G4String stemp;
    for( G4int jj = 0; jj < 40; jj++) {
    //    if( fread(stemp, sizeof(char), 2, fin) != 1) {
      if( fread(&sc, sizeof(char), 1, fin) != 1) {
	G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		    "Error",
		    FatalException,
		    "Problem reading material name");
      }
      stemp += G4String(1,sc);
      //      G4cout << jj << " " << sc << " nmate " << ii << " mate " << stemp << G4endl;
    }
    G4int jj;
    for( jj = 39; jj > 0; jj-- ){
      if( stemp.substr(jj,1) != G4String(" ") ) break;
    }
    stemp = stemp.substr(0,jj+1);

    G4Material* mate = matmgr->FindOrBuildG4Material(stemp);
    thePhantomMaterialsOriginal[ii] = mate;
  }

  for( G4int jj = 0; jj < 3; jj++) {
    if( fread(&sc, sizeof(char), 1, fin) != 1) {
      G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		  "Error",
		  FatalException,
		  "Problem reading patient position");
    }
    thePatientPosition += G4String(1,sc);
  }
  if( fread(&nVoxelX, sizeof(size_t),  1, fin) != 1) {
    G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		"Error",
		FatalException,
		"Problem reading number of voxels X");
  }
  if( fread(&nVoxelY, sizeof(size_t),  1, fin) != 1) {
    G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		"Error",
		FatalException,
		"Problem reading number of voxels Y");
  }
  if( fread(&nVoxelZ, sizeof(size_t),  1, fin) != 1) {
    G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		"Error",
		FatalException,
		"Problem reading number of voxels Z");
  }
  G4cout << "GmReadPhantomG4BinGeometry::ReadPhantomData nVoxel X/Y/Z " << nVoxelX << " " << nVoxelY << " " << nVoxelZ << G4endl;

  //--- Read minimum and maximum extensions
  if( fread(&offsetX, sizeof(G4float),  1, fin) != 1) {
    G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		"Error",
		FatalException,
		"Problem reading minimum X position");
  }
  if( fread(&dimX, sizeof(G4float),  1, fin) != 1) {
    G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		"Error",
		FatalException,
		"Problem reading maximum X position");
  }
  dimX = (dimX - offsetX)/nVoxelX;

  if( fread(&offsetY, sizeof(G4float),  1, fin) != 1) {
    G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		"Error",
		FatalException,
		"Problem reading minimum Y position");
  }
  if( fread(&dimY, sizeof(G4float),  1, fin) != 1) {
    G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		"Error",
		FatalException,
		"Problem reading maximum Y position");
  }
  dimY = (dimY - offsetY)/nVoxelY;

  if( fread(&offsetZ, sizeof(G4float),  1, fin) != 1) {
    G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		"Error",
		FatalException,
		"Problem reading minimum Z position");
  }
  if( fread(&dimZ, sizeof(G4float),  1, fin) != 1) {
    G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		"Error",
		FatalException,
		"Problem reading maximum Z position");
  }
  dimZ = (dimZ - offsetZ)/nVoxelZ;
  G4cout << "GmReadPhantomG4BinGeometry::ReadPhantomData voxelDimX " << dimX << " offsetX " << offsetX << G4endl;
  G4cout << "GmReadPhantomG4BinGeometry::ReadPhantomData voxelDimY " << dimY << " offsetY " << offsetY << G4endl;
  G4cout << "GmReadPhantomG4BinGeometry::ReadPhantomData voxelDimZ " << dimZ << " offsetZ " << offsetZ << G4endl;

  size_t mateID; 
  theMateIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	if( fread(&mateID, sizeof(size_t),  1, fin) != 1) {
	  G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		      "Error",
		      FatalException,
		      G4String("Problem reading material ID" + GmGenUtils::itoa(ix) + " " + GmGenUtils::itoa(iy) + " " + GmGenUtils::itoa(iz)).c_str());
	}
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << " nnew " << nnew << " = " << mateID<< G4endl;
	theMateIDs[nnew] = mateID;
      }
    }
  }
  
  ReadVoxelDensitiesBin( fin );

  //  ReadPV( fin );

  fclose(fin);
 
}


void GmReadPhantomG4BinGeometry::ReadVoxelDensitiesBin( FILE* fin )
{
  std::map<G4int, std::pair<G4double,G4double> > densiMinMax;
  std::map<G4int, std::pair<G4double,G4double> >::iterator mpite;
  for( size_t ii = 0; ii < thePhantomMaterialsOriginal.size(); ii++ ){
    densiMinMax[ii] = std::pair<G4double,G4double>(DBL_MAX,-DBL_MAX);
  }

  G4double densityStep = GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:DensityStep", 10.);
  
  std::map<G4int,G4double> densitySteps;
  for( size_t ii = 0; ii < thePhantomMaterialsOriginal.size(); ii++ ){
    densitySteps[ii] = densityStep; //currently all materials with same step
  }
  //  densitySteps[0] = 0.0001; //air
  
  //--- Calculate the average material density for each material/density bin
  std::map< std::pair<G4Material*,G4int>, matInfo* > newMateDens;
  
  theMateDensities = new float[nVoxelX*nVoxelY*nVoxelZ];

  G4float dens;
  //---- Read the material densities
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	//	G4cout << " stemp " << stemp << G4endl;
	if( fread(&dens, sizeof(G4float),  1, fin) != 1) {
	  G4Exception(" GmReadPhantomG4BinGeometry::ReadPhantomData",
		      "Error",
		      FatalException,
		      G4String("Problem reading material density" + GmGenUtils::itoa(ix) + " " + GmGenUtils::itoa(iy) + " " + GmGenUtils::itoa(iz)).c_str());
	}
	//	G4cout << ix << " " << iy << " " << iz << " density " << dens << G4endl;
	if( !bRecalculateMaterialDensities ) {
	  G4int copyNo = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	  theMateDensities[copyNo] = dens;
	  continue;
	}
	
	G4int copyNo = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//--- store the minimum and maximum density for each material (just for printing)
	mpite = densiMinMax.find( theMateIDs[copyNo] );
	if( dens < (*mpite).second.first ) (*mpite).second.first = dens;
	if( dens > (*mpite).second.second ) (*mpite).second.second = dens;
	
	//--- Get material from original list of material in file
	int mateID = theMateIDs[copyNo];
	std::map<G4int,G4Material*>::const_iterator imite = thePhantomMaterialsOriginal.find(mateID);
	//	G4cout << copyNo << " mateID " << mateID << G4endl;
	//--- Check if density is equal to the original material density
	if( fabs(dens - (*imite).second->GetDensity()/CLHEP::g*CLHEP::cm3 ) < 1.e-9 ) continue;
	
	//--- Build material name with thePhantomMaterialsOriginal name + density
	//	float densityBin = densitySteps[mateID] * (G4int(dens/densitySteps[mateID])+0.5);
	G4int densityBin = (G4int(dens/densitySteps[mateID]));
	//	G4cout << " densityBin " << densityBin << " " << dens << " " <<densitySteps[mateID] << G4endl; 
	
	G4String mateName = (*imite).second->GetName()+GmGenUtils::ftoa(densityBin);
	//--- Look if it is the first voxel with this material/densityBin
	std::pair<G4Material*,G4int> matdens((*imite).second, densityBin );
	
	std::map< std::pair<G4Material*,G4int>, matInfo* >::iterator mppite = newMateDens.find( matdens );
	if( mppite != newMateDens.end() ){
	  matInfo* mi = (*mppite).second;
	  mi->sumdens += dens;
	  mi->nvoxels++;
	  theMateIDs[copyNo] = thePhantomMaterialsOriginal.size()-1 + mi->id;
	  //	  G4cout << copyNo << " mat new again " << thePhantomMaterialsOriginal.size()-1 + mi->id << " " << mi->id << G4endl;
	} else {
	  matInfo* mi = new matInfo;
	  mi->sumdens = dens;
	  mi->nvoxels = 1;
	  mi->id = newMateDens.size()+1;
	  newMateDens[matdens] = mi;
	  theMateIDs[copyNo] = thePhantomMaterialsOriginal.size()-1 + mi->id;
	  //	  G4cout << copyNo << " mat new first " << thePhantomMaterialsOriginal.size()-1 + mi->id << G4endl;
	}
	theMateDensities[copyNo] = dens;
	//	G4cout << ix << " " << iy << " " << iz << " filling theMateIDs " << copyNo << " = " << atoi(cid)-1 << G4endl;
	//	theMateIDs[copyNo] = atoi(cid)-1;
      }
    }
  }
  
  if( bRecalculateMaterialDensities ) { 
    for( mpite = densiMinMax.begin(); mpite != densiMinMax.end(); mpite++ ){
      G4cout << "GmReadPhantomG4BinGeometry::ReadPhantomVoxelDensitiesBin ORIG MATERIALS DENSITY " << (*mpite).first << " MIN " << (*mpite).second.first << " MAX " << (*mpite).second.second << G4endl;
    }
  }
  
  //----- Build the list of phantom materials that go to Parameterisation
  //--- Add original materials
  std::map<G4int,G4Material*>::const_iterator mimite;
  for( mimite = thePhantomMaterialsOriginal.begin(); mimite != thePhantomMaterialsOriginal.end(); mimite++ ){
    thePhantomMaterials.push_back( (*mimite).second );
  }
  // 
  //---- Build and add new materials
  std::map< std::pair<G4Material*,G4int>, matInfo* >::iterator mppite;
  for( mppite= newMateDens.begin(); mppite != newMateDens.end(); mppite++ ){
    G4double averdens = (*mppite).second->sumdens/(*mppite).second->nvoxels;
    G4double saverdens = G4int(1000.001*averdens)/1000.;
    G4cout << "GmReadPhantomG4BinGeometry::ReadVoxelDensitiesBin AVER DENS " << averdens << " -> " << saverdens << " -> " << G4int(1000*averdens) << " " << G4int(1000*averdens)/1000 << " " <<  G4int(1000*averdens)/1000. << G4endl;
    
    G4cout << "GmReadPhantomG4BinGeometry::ReadVoxelDensitiesBin AVER DENS " << averdens << " -> " << saverdens << " -> " << GmGenUtils::ftoa(saverdens) << " Nvoxels " <<(*mppite).second->nvoxels << G4endl;
    G4String mateName = ((*mppite).first).first->GetName() + "_" + GmGenUtils::ftoa(saverdens);
    thePhantomMaterials.push_back( BuildMaterialChangingDensity( (*mppite).first.first, averdens, mateName ) );
  }
  
}

