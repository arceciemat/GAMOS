#include "globals.hh"

#include "GmReadPhantomPartialG4Geometry.hh"
#include "GmReadDICOMVerbosity.hh"

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

#include "G4PartialPhantomParameterisation.hh"
#include "G4GeometryTolerance.hh"

#include "G4tgbVolumeMgr.hh"
#include "G4tgbMaterialMgr.hh"
#include "G4tgrMessenger.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4tgrMessenger.hh"

//---------------------------------------------------------------------------
GmReadPhantomPartialG4Geometry::GmReadPhantomPartialG4Geometry()
{
}

//---------------------------------------------------------------------------
GmReadPhantomPartialG4Geometry::~GmReadPhantomPartialG4Geometry()
{
  thePhantomFileName = "test.g4dcm";
}

//---------------------------------------------------------------------------
void GmReadPhantomPartialG4Geometry::ReadPhantomData()
{
  G4String filename = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:Phantom:FileName", thePhantomFileName);

  filename = GmGenUtils::FileInPath( filename );

  GmFileIn fing = GmFileIn::GetInstance(filename);
  std::vector<G4String> wl;
  G4int nMaterials;
  if( !fing.GetWordsInLine(wl) ) return;
  nMaterials = GmGenUtils::GetInteger(wl[0]);
  G4String stemp;
  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 
  G4int nmate;
  for( G4int ii = 0; ii < nMaterials; ii++ ){
    if( !fing.GetWordsInLine(wl) ) return;
    nmate = GmGenUtils::GetInteger(wl[0]);
    stemp = wl[1];;
    G4cout << "GmReadPhantomPartialG4Geometry::ReadPhantomData reading nmate " << ii << " = " << nmate << " mate " << stemp << G4endl;
    if( ii != nmate ) G4Exception("GmReadPhantomPartialG4Geometry::ReadPhantomData",
		    "Wrong argument",
		    FatalErrorInArgument,
				  "Material number should be in increasing order: wrong material number ");
    G4Material* mate = matmgr->FindOrBuildG4Material(stemp);
    thePhantomMaterialsOriginal[ii] = mate;
  }

  std::ifstream* fin = fing.GetIfstream();
  *fin >> nVoxelX >> nVoxelY >> nVoxelZ;
  G4cout << "GmReadPhantomPartialG4Geometry::ReadPhantomData nVoxel X/Y/Z " << nVoxelX << " " << nVoxelY << " " << nVoxelZ << G4endl;
  *fin >> offsetX >> dimX;
  dimX = (dimX - offsetX)/nVoxelX;
  *fin >> offsetY >> dimY;
  dimY = (dimY - offsetY)/nVoxelY;
  *fin >> offsetZ >> dimZ;
  dimZ = (dimZ - offsetZ)/nVoxelZ;
  G4cout << "GmReadPhantomPartialG4Geometry::ReadPhantomData voxelDimX " << dimX << " offsetX " << offsetX << G4endl;
  G4cout << "GmReadPhantomPartialG4Geometry::ReadPhantomData voxelDimY " << dimY << " offsetY " << offsetY << G4endl;
  G4cout << "GmReadPhantomPartialG4Geometry::ReadPhantomData voxelDimZ " << dimZ << " offsetZ " << offsetZ << G4endl;

  //--- Read voxels that are filled
  theNVoxels = 0;
  //  G4bool* isFilled = new G4bool[nVoxelX*nVoxelY*nVoxelZ];
  //  theFilledIDs = new size_t[nVoxelZ*nVoxelY+1];
  //?  theFilledIDs.insert(0);
  int ifxmin1, ifxmax1;
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    std::map< G4int, G4int > ifmin, ifmax;
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      *fin >> ifxmin1 >> ifxmax1;      
      // check coherence ...

      ifmin[iy] = ifxmin1;
      ifmax[iy] = ifxmax1;
      G4int ifxdiff = ifxmax1-ifxmin1+1;
      if( ifxmax1 == -1 && ifxmin1 == -1 ) ifxdiff = 0;
      //      theFilledIDs.insert(std::multimap<G4int,size_t>::value_type(ifxdiff+theNVoxels, ifxmin1));
      theFilledIDs.insert(std::pair<size_t,size_t>(ifxdiff+theNVoxels-1, ifxmin1));
      //      G4cout << "GmReadPhantomPartialG4Geometry::ReadPhantomData insert FilledIDs " << ifxdiff+theNVoxels-1 << " min " << ifxmin1 << " N= " << theFilledIDs.size() << G4endl; //GDEB
      //filledIDs[iz*nVoxelY+iy+1] = ifxmax1-ifxmin1+1;
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	//	G4cout << "REGNAV GmReadPhantomPartialG4Geometry::ReadPhantomData checking to add voxel iz " << iz << " iy " << iy << " ix " << ix << "  min= " << ifxmin1 << " max= " << ifxmax1 << G4endl;
	//	G4int copyNo = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	if( ix >= G4int(ifxmin1) && ix <= G4int(ifxmax1) ) {
	  //	  isFilled[copyNo] = true;
	  theNVoxels++;
#ifndef GAMOS_NO_VERBOSE
	  if( ReadDICOMVerb(debugVerb) ) G4cout << "REGNAV GmReadPhantomPartialG4Geometry::ReadPhantomData add voxel " << theNVoxels << " ix " << ix << " iy " << iy << " iz " <<iz << G4endl;
#endif
	} else {
	  //	  isFilled[copyNo] = false;
	}
      }
    }
    theFilledMins[iz] = ifmin;
    theFilledMaxs[iz] = ifmax;
  }

  //--- Read material IDs
  G4int mateID1;
  theMateIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  G4int copyNo = 0;
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    std::map< G4int, G4int > ifmin = theFilledMins[iz];
    std::map< G4int, G4int > ifmax = theFilledMaxs[iz];
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      ifxmin1 = ifmin[iy];
      ifxmax1 = ifmax[iy];
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	if( ix >= G4int(ifxmin1) && ix <= G4int(ifxmax1) ) {
	  *fin >> mateID1;
#ifndef GAMOS_NO_VERBOSE
	  if( ReadDICOMVerb(debugVerb) )
	    G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << copyNo << " = " <<  mateID1 << G4endl;
#endif
	  if( mateID1 < 0 || mateID1 >= nMaterials ) {
	    G4Exception("GmReadPhantomPartialG4Geometry::ReadPhantomData",
			"Wrong index in phantom file",
			FatalException,
			G4String("It should be between 0 and "
			       + GmGenUtils::itoa(nMaterials-1) 
				 + ", while it is " 
				 + GmGenUtils::itoa(mateID1)).c_str());
	  }
	  theMateIDs[copyNo] = mateID1;
	  copyNo++;
	}
      }
    }
  }

  ReadVoxelDensitiesPartial( *fin );

  ReadPV( fing );

  fin->close();
}

//------------------------------------------------------------------------
void GmReadPhantomPartialG4Geometry::ReadVoxelDensitiesPartial( std::ifstream& fin )
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

  theMateDensities = new float[nVoxelX*nVoxelY*nVoxelZ];

  //--- Calculate the average material density for each material/density bin
  std::map< std::pair<G4Material*,G4int>, matInfo* > newMateDens;
  G4double dens1;
  size_t ifxmin1, ifxmax1;

  //---- Read the material densities
  G4int copyNo = 0;
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    std::map< G4int, G4int > ifmin = theFilledMins[iz];
    std::map< G4int, G4int > ifmax = theFilledMaxs[iz];
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      ifxmin1 = ifmin[iy];
      ifxmax1 = ifmax[iy];
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	if( ix >= G4int(ifxmin1) && ix <= G4int(ifxmax1) ) {
	  fin >> dens1;
	  //	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << copyNo << " = " <<  atoi(stemp.c_str())-1 << " " << stemp << G4endl;
	  if( !bRecalculateMaterialDensities ) {
	    copyNo = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	    theMateDensities[copyNo] = dens1;
	    continue;
	  }
	  
	//--- store the minimum and maximum density for each material (just for printing)
	  mpite = densiMinMax.find( theMateIDs[copyNo] );
	  if( dens1 < (*mpite).second.first ) (*mpite).second.first = dens1;
	  if( dens1 > (*mpite).second.second ) (*mpite).second.second = dens1;
	  
	  //--- Get material from original list of material in file
	  int mateID = theMateIDs[copyNo];
	  std::map<G4int,G4Material*>::const_iterator imite = thePhantomMaterialsOriginal.find(mateID);
	  //	G4cout << copyNo << " mateID " << mateID << G4endl;
	  //--- Check if density is equal to the original material density
	  if( fabs(dens1 - (*imite).second->GetDensity()/CLHEP::g*CLHEP::cm3 ) < 1.e-9 ) continue;
	  
	  //--- Build material name with thePhantomMaterialsOriginal name + density
	  //	float densityBin = densitySteps[mateID] * (G4int(dens1/densitySteps[mateID])+0.5);
	  G4int densityBin = (G4int(dens1/densitySteps[mateID]));
	  //	G4cout << " densityBin " << densityBin << " " << dens1 << " " <<densitySteps[mateID] << G4endl; 
	  
	  G4String mateName = (*imite).second->GetName()+GmGenUtils::ftoa(densityBin);
	  //--- Look if it is the first voxel with this material/densityBin
	  std::pair<G4Material*,G4int> matdens((*imite).second, densityBin );
 
	  std::map< std::pair<G4Material*,G4int>, matInfo* >::iterator mppite = newMateDens.find( matdens );
	  if( mppite != newMateDens.end() ){
	    matInfo* mi = (*mppite).second;
	    mi->sumdens += dens1;
	    mi->nvoxels++;
	    theMateIDs[copyNo] = thePhantomMaterialsOriginal.size()-1 + mi->id;
	    //	  G4cout << copyNo << " mat new again " << thePhantomMaterialsOriginal.size()-1 + mi->id << " " << mi->id << G4endl;
	  } else {
	    matInfo* mi = new matInfo;
	    mi->sumdens = dens1;
	    mi->nvoxels = 1;
	    mi->id = newMateDens.size()+1;
	    newMateDens[matdens] = mi;
	    theMateIDs[copyNo] = thePhantomMaterialsOriginal.size()-1 + mi->id;
	    //	  G4cout << copyNo << " mat new first " << thePhantomMaterialsOriginal.size()-1 + mi->id << G4endl;
	  }
	  theMateDensities[copyNo] = dens1;

	  copyNo++;
	//	G4cout << ix << " " << iy << " " << iz << " filling theMateIDs " << copyNo << " = " << atoi(cid)-1 << G4endl;
				      //	theMateIDs[copyNo] = atoi(cid)-1;
	}
      }
    }
  }

  if( bRecalculateMaterialDensities ) { 
    for( mpite = densiMinMax.begin(); mpite != densiMinMax.end(); mpite++ ){
      G4cout << "REGNAV GmReadPhantomPartialG4Geometry::ReadVoxelDensities ORIG MATERIALS DENSITY " << (*mpite).first << " MIN " << (*mpite).second.first << " MAX " << (*mpite).second.second << G4endl;
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
    G4cout << "GmReadPhantomGeometry::ReadVoxelDensities AVER DENS " << averdens << " -> " << saverdens << " -> " << G4int(1000*averdens) << " " << G4int(1000*averdens)/1000 << " " <<  G4int(1000*averdens)/1000. << G4endl;

    G4cout << "GmReadPhantomGeometry::ReadVoxelDensities AVER DENS " << averdens << " -> " << saverdens << " -> " << GmGenUtils::ftoa(saverdens) << " Nvoxels " <<(*mppite).second->nvoxels << G4endl;
    G4String mateName = ((*mppite).first).first->GetName() + "_" + GmGenUtils::ftoa(saverdens);
    thePhantomMaterials.push_back( BuildMaterialChangingDensity( (*mppite).first.first, averdens, mateName ) );
  }

}

//---------------------------------------------------------------------------
void GmReadPhantomPartialG4Geometry::ConstructPhantom(G4LogicalVolume* )
{
  G4String OptimAxis = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:Phantom:OptimAxis", "kXAxis");
  G4bool bSkipEqualMaterials = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:SkipEqualMaterials", 0));
  G4int regStructureID = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:RegularStructureID", 2));

  //----- Get phantom container
  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 
  theMatePhantom = matmgr->FindOrBuildG4Material("G4_AIR");
 
  G4String motherName = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:ContainerName","phantomContainer");
  cont_logic = GmGeometryUtils::GetInstance()->GetLogicalVolumes(motherName,1,1)[0];

  G4ThreeVector posCentreVoxels(offsetX+nVoxelX*dimX/2.,offsetY+nVoxelY*dimY/2.,offsetZ+nVoxelZ*dimZ/2.+00.);
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << " PhantomContainer posCentreVoxels " << posCentreVoxels << G4endl;
#endif

  G4RotationMatrix* rotm = new G4RotationMatrix;
  rotm->rotateX( theInitialRotAngleX);
  rotm->rotateY( theInitialRotAngleY);
  rotm->rotateZ( theInitialRotAngleZ);

  posCentreVoxels.rotateX( theInitialRotAngleX);
  posCentreVoxels.rotateY( theInitialRotAngleY);
  posCentreVoxels.rotateZ( theInitialRotAngleZ);

  posCentreVoxels += theInitialDisp;

  //----- Build phantom
  G4String voxelName = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:VoxelName", "phantom");
  G4Box* phantom_solid = new G4Box(voxelName, dimX/2., dimY/2., dimZ/2. );
  //  G4cout << " phantomMate " << *theMatePhantom << G4endl;
  G4LogicalVolume* phantom_logic = 
    new G4LogicalVolume( phantom_solid, 
			 theMatePhantom,
			 voxelName, 
			 0, 0, 0 );
  G4bool pVis = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:VisOff",1));
  if( !pVis ) {
    G4VisAttributes* visAtt = new G4VisAttributes( FALSE );
    phantom_logic->SetVisAttributes( visAtt );
  }

  G4double theSmartless = 0.5;
  //  cont_logic->SetSmartless( theSmartless );
  phantom_logic->SetSmartless( theSmartless );

  thePartialPhantomParam = new G4PartialPhantomParameterisation();

  thePartialPhantomParam->SetMaterials( thePhantomMaterials );
  thePartialPhantomParam->SetVoxelDimensions( dimX/2., dimY/2., dimZ/2. );
  thePartialPhantomParam->SetNoVoxels( nVoxelX, nVoxelY, nVoxelZ );
  thePartialPhantomParam->SetMaterialIndices( theMateIDs );

  thePartialPhantomParam->SetFilledIDs(theFilledIDs);

  thePartialPhantomParam->SetFilledMins(theFilledMins);

  thePartialPhantomParam->BuildContainerWalls();

  //  G4cout << " Number of Materials " << thePhantomMaterials.size() << G4endl;
  //  G4cout << " SetMaterialIndices(0) " << theMateIDs[0] << G4endl;

  G4PVParameterised * phantom_phys;
  if( OptimAxis == "kUndefined" ) {
    phantom_phys = new G4PVParameterised(voxelName,phantom_logic,cont_logic,
					 kUndefined, theNVoxels, thePartialPhantomParam);
    phantom_phys->SetRegularStructureId(regStructureID);
  } else   if( OptimAxis == "kXAxis" ) {
    //    G4cout << " optim kX " << G4endl;
    phantom_phys = new G4PVParameterised(voxelName,phantom_logic,cont_logic,
					 //					  kXAxis, nVoxelX*nVoxelY*nVoxelZ, thePartialPhantomParam);
					  kXAxis, theNVoxels, thePartialPhantomParam);
    phantom_phys->SetRegularStructureId(regStructureID);
    thePartialPhantomParam->SetSkipEqualMaterials(bSkipEqualMaterials);
  } else {
    G4Exception("GmReadPhantomGeometry::ConstructPhantom","Wrong argument to parameter GmReadPhantomGeometry:Phantom:OptimAxis",FatalErrorInArgument,(G4String("Only allowed 'kUndefined' or 'kXAxis', it is: "+OptimAxis).c_str()));
  }
  

  //  G4cout << " GmReadPhantomGeometry  phantom_phys " << phantom_phys << " trans " << phantom_phys->GetTranslation() << G4endl;
  //-  thePartialPhantomParam->BuildContainerSolid(cont_phys);

  //  G4cout << " GmReadPhantomGeometry::constructPhantom ended " << G4endl;

}
