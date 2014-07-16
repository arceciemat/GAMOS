#include "GmReadPhantomGeometry.hh"
#include "G4PhantomParameterisation.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GmDICOMIntersectVolume.hh"

#include "G4GeometryTolerance.hh"
#include "G4Material.hh"
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
#include "G4tgbVolumeMgr.hh"
#include "G4tgbMaterialMgr.hh"
#include "G4tgrMessenger.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4tgrMessenger.hh"
#include "GamosCore/GamosGeometry/include/GmGeomTextDetectorBuilder.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmRegionCutsMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmParallelWorldCreator.hh"
#include "G4tgbParallelGeomMgr.hh"
#include "G4VUserParallelWorld.hh"

//---------------------------------------------------------------------------
GmReadPhantomGeometry::GmReadPhantomGeometry()
{
  new G4tgrMessenger;

  parameterisedPhysVolume = 0;

  std::vector<G4double> inidisp = GmParameterMgr::GetInstance()->GetVNumericValue("GmReadPhantomGeometry:InitialDisplacement", std::vector<G4double>());
  if( inidisp.size() == 0 ){
    theInitialDisp = G4ThreeVector(0.,0.,0.);
  } else if( inidisp.size() == 3 ){
    theInitialDisp = G4ThreeVector(inidisp[0],inidisp[1],inidisp[2]);
  } else {
    G4Exception("GmReadPhantomGeometry","Error in /gamos/setParam GmReadPhantomGeometry:InitialDisp",FatalErrorInArgument,G4String("it must have 3 arguments: POS_X POS_Y POS_Z, and it has "+GmGenUtils::itoa(inidisp.size())).c_str());
  }

  std::vector<G4double> inirot = GmParameterMgr::GetInstance()->GetVNumericValue("GmReadPhantomGeometry:InitialRotAngles", std::vector<G4double>());
  if( inirot.size() == 0 ){
    theInitialRotAngleX = 0.;
    theInitialRotAngleY = 0.;
    theInitialRotAngleZ = 0.;
  } else if( inirot.size() == 3 ){
    theInitialRotAngleX = inirot[0];
    theInitialRotAngleY = inirot[1];
    theInitialRotAngleZ = inirot[2];
  } else {
    G4Exception("GmReadPhantomGeometry","Error in /gamos/setParam GmReadPhantomGeometry:InitialRotAngles",FatalErrorInArgument,G4String("it must have 3 arguments: ANG_X ANG_Y ANG_Z, and it has "+GmGenUtils::itoa(inirot.size())).c_str());
  }

  bRecalculateMaterialDensities = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:RecalculateMaterialDensities", 1));

  new GmDICOMIntersectVolume;
}

//---------------------------------------------------------------------------
GmReadPhantomGeometry::~GmReadPhantomGeometry()
{
}


//---------------------------------------------------------------------------
G4VPhysicalVolume* GmReadPhantomGeometry::Construct()
{
  //------------------- construct g4 geometry
  G4String filename = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:FileName", ".geom");

  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  filename = GmGenUtils::FileInPath( path, filename );

  GmGeomTextDetectorBuilder* gtb = new GmGeomTextDetectorBuilder;
  G4tgbVolumeMgr* volmgr = G4tgbVolumeMgr::GetInstance();
  volmgr->SetDetectorBuilder( gtb );
  volmgr->AddTextFile(filename);


  std::vector<G4String> fnamepar;
  fnamepar = GmParameterMgr::GetInstance()->GetVStringValue("GmGeometryFromText:FileNameParallel", fnamepar);
  if( fnamepar.size() != 0 ){
    if( fnamepar.size() != 2 ){
      G4Exception("GmGeometryFromText::Construct",
		  "Error in number of arguments of parameter 'GmGeometryFromText:FileNameParallel'",
		  FatalException,
		  (G4String("There should be two: FILE_NAME PARALLEL_WORLD_NUMBER, there are ")
		   +GmGenUtils::itoa(fnamepar.size())).c_str());
    }
    volmgr->AddTextFileParallel(fnamepar[0],GmGenUtils::GetInteger(fnamepar[1]));
  }

  //  G4VPhysicalVolume* physiWorld = volmgr->ReadAndConstructDetector();
  const G4tgrVolume* tgrVoltop = gtb->ReadDetector();

  //------------------------------------------------ 
  // Construct the Geant4 parallel worlds
  //------------------------------------------------ 
  G4tgbParallelGeomMgr* parallelMgr = G4tgbParallelGeomMgr::GetInstance();
  parallelMgr->SetWorldCreator( new GmParallelWorldCreator );
  std::vector<G4VUserParallelWorld*> parallelWorlds = parallelMgr->CreateParalleWorlds();
  for( size_t ii = 0; ii < parallelWorlds.size(); ii++ ) {
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) G4cout << "GmReadPhantomGeometry::Construct RegisterParallelWorld " << parallelWorlds[ii]->GetName() << G4endl;
#endif
    RegisterParallelWorld( parallelWorlds[ii] );
   
  }

  //------------------------------------------------ 
  G4String motherName = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:MotherName","");
  G4VPhysicalVolume* physiWorld = gtb->ConstructDetectorGAMOS(tgrVoltop,-1,0);
  G4VPhysicalVolume* physiMother;
  if( motherName == "" ) {
    physiMother = physiWorld;
  } else {
    std::vector<G4VPhysicalVolume*> physiMothers = GmGeometryUtils::GetInstance()->GetPhysicalVolumes(motherName);
    if( physiMothers.size() > 1 ) {
      G4Exception("GmReadPhantomGeometry::Construct","Mother volume name corresponds to more than G4VPhysicalVolume, only the first one will be used",JustWarning,motherName);
    }
    physiMother = physiMothers[0];
  }

  ReadPhantomData();
  ConstructPhantom(physiMother->GetLogicalVolume());

  //--- Create regions
  GmRegionCutsMgr::GetInstance()->BuildRegions();
  //--- Set cuts to regions
  GmRegionCutsMgr::GetInstance()->BuildProductionCuts();

  //  GmGeometryUtils::GetInstance()->DumpMaterialList();

  /*  std::vector<G4Material*> mates = thePhantomParam->GetMaterials();
  G4int nMate = thePhantomParam->GetMaterials().size();
  for( size_t ii = 0; ii < nMate; ii++ ){
    G4cout << ii << " PARAM MATE " << mates[ii]->GetName() << G4endl;
  }
  G4int nvox = thePhantomParam->GetNoVoxel();
  for( size_t ii = 0; ii < nvox; ii++ ){
    G4cout << ii << " PARAM ID " << (thePhantomParam->ComputeMaterial( ii, (G4VPhysicalVolume *) 0, (const G4VTouchable *)0))->GetName() << G4endl;
    }*/

  // Dummy declarations ...

  return physiWorld;
}

//---------------------------------------------------------------------------
void GmReadPhantomGeometry::ConstructPhantom(G4LogicalVolume* parentLV)
{
  G4String OptimAxis = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:Phantom:OptimAxis", "kXAxis");
  G4bool bSkipEqualMaterials = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:SkipEqualMaterials", 0));
  G4int regStructureID = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:RegularStructureID", 1));

  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 
  //----- Build phantom container
  theMatePhantom = matmgr->FindOrBuildG4Material("G4_AIR");
  G4Box* cont_solid = new G4Box("phantomContainer",nVoxelX*dimX/2.,nVoxelY*dimY/2.,nVoxelZ*dimZ/2.+0.*G4GeometryTolerance::GetInstance()->GetSurfaceTolerance());
  G4LogicalVolume* cont_logic = 
    new G4LogicalVolume( cont_solid, 
			 theMatePhantom,
			 "phantomContainer", 
			 0, 0, 0 );
  G4bool pcVis = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:PhantomContainer:VisOff",1));
  if( !pcVis ) {
    G4VisAttributes* visAtt = new G4VisAttributes( FALSE );
    cont_logic->SetVisAttributes( visAtt );
  }

  G4ThreeVector posCentreVoxels(offsetX+nVoxelX*dimX/2.,offsetY+nVoxelY*dimY/2.,offsetZ+nVoxelZ*dimZ/2.+00.);
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) G4cout << " PhantomContainer posCentreVoxels " << posCentreVoxels << G4endl;
#endif

  G4RotationMatrix* rotm = new G4RotationMatrix;
  //  rotm->rotateX(45.*deg);
  rotm->rotateX( theInitialRotAngleX);
  rotm->rotateY( theInitialRotAngleY);
  rotm->rotateZ( theInitialRotAngleZ);

  posCentreVoxels.rotateX( theInitialRotAngleX);
  posCentreVoxels.rotateY( theInitialRotAngleY);
  posCentreVoxels.rotateZ( theInitialRotAngleZ);

  posCentreVoxels += theInitialDisp;

  //  G4cout << " PLACING PHANTOM CONTAINER " << *rotm << " POS " << posCentreVoxels << G4endl;
  G4VPhysicalVolume * cont_phys =  new G4PVPlacement(rotm,  // rotation
						     posCentreVoxels,
		      cont_logic,     // The logic volume
		      "phantomContainer",  // Name
		      parentLV,               // Mother
		      false,           // No op. bool.
		      1);              // Copy number

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
  cont_logic->SetSmartless( theSmartless );
  phantom_logic->SetSmartless( theSmartless );

  thePhantomParam = new G4PhantomParameterisation();

  /*
  //--------------//
  //--- All same material
  thePhantomMaterials.clear();
  G4Material* mate = matmgr->FindOrBuildG4Material("Water");
  thePhantomMaterials.push_back(mate);
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
  for( G4int iy = 0; iy < nVoxelY; iy++ ) {
  for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	mateIDs[nnew] = 0;
      }
    }
  }
  //--------------//
  */

  thePhantomParam->SetMaterials( thePhantomMaterials );
  thePhantomParam->SetVoxelDimensions( dimX/2., dimY/2., dimZ/2. );
  thePhantomParam->SetNoVoxel( nVoxelX, nVoxelY, nVoxelZ );
  thePhantomParam->SetMaterialIndices( mateIDs );

  //  G4cout << " Number of Materials " << thePhantomMaterials.size() << G4endl;
  //  G4cout << " SetMaterialIndices(0) " << mateIDs[0] << G4endl;

  G4PVParameterised * phantom_phys;
  if( OptimAxis == "kUndefined" ) {
    phantom_phys = new G4PVParameterised(voxelName,phantom_logic,cont_logic,
					 kUndefined, nVoxelX*nVoxelY*nVoxelZ, thePhantomParam);
  } else   if( OptimAxis == "kXAxis" ) {
    //    G4cout << " optim kX " << G4endl;
    phantom_phys = new G4PVParameterised(voxelName,phantom_logic,cont_logic,
					 //					  kXAxis, nVoxelX*nVoxelY*nVoxelZ, thePhantomParam);
					  kXAxis, nVoxelX*nVoxelY*nVoxelZ, thePhantomParam);
    phantom_phys->SetRegularStructureId(regStructureID);
    thePhantomParam->SetSkipEqualMaterials(bSkipEqualMaterials);
  } else {
    G4Exception("GmReadPhantomGeometry::ConstructPhantom","Wrong argument to parameter GmReadPhantomGeometry:Phantom:OptimAxis",FatalErrorInArgument,(G4String("Only allowed 'kUndefined' or 'kXAxis', it is: "+OptimAxis).c_str()));
  }
  

#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry  phantom_phys " << phantom_phys << " trans " << phantom_phys->GetTranslation() << G4endl;
#endif

  thePhantomParam->BuildContainerSolid(cont_phys);

#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry::constructPhantom ended " << G4endl;
#endif

}


//---------------------------------------------------------------------------
G4Material* GmReadPhantomGeometry::BuildMaterialChangingDensity( const G4Material* origMate, float density, G4String mateName )
{
  G4int nelem = origMate->GetNumberOfElements();
  G4Material* mate = new G4Material( mateName, density*CLHEP::g/CLHEP::cm3, nelem, kStateUndefined, CLHEP::STP_Temperature );

  for( G4int ii = 0; ii < nelem; ii++ ){
    G4double frac = origMate->GetFractionVector()[ii];
    G4Element* elem = const_cast<G4Element*>(origMate->GetElement(ii));
    mate->AddElement( elem, frac );
  }

#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) G4cout << " GmReadPhantomGeometry::BuildMaterialChangingDensity new material " << *mate << " density " << density <<  G4endl;
#endif

  return mate;
}


void GmReadPhantomGeometry::ReadVoxelDensities( std::ifstream& fin )
{
  G4String stemp;
  std::map<G4int, std::pair<G4double,G4double> > densiMinMax;
  std::map<G4int, std::pair<G4double,G4double> >::iterator mpite;
  for( size_t ii = 0; ii < thePhantomMaterialsOriginal.size(); ii++ ){
    densiMinMax[ii] = std::pair<G4double,G4double>(DBL_MAX,-DBL_MAX);
  }

  G4double densityStep = GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:DensityStep", 0.1);

  std::map<G4int,G4double> densitySteps;
  for( size_t ii = 0; ii < thePhantomMaterialsOriginal.size(); ii++ ){
    densitySteps[ii] = densityStep; //currently all materials with same step
  }
  //  densitySteps[0] = 0.0001; //air

  //--- Calculate the average material density for each material/density bin
  std::map< std::pair<G4Material*,G4int>, matInfo* > newMateDens;


  //---- Read the material densities
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	//	G4cout << " stemp " << stemp << G4endl;
	fin >> stemp; 
	//	G4cout << ix << " " << iy << " " << iz << " density " << stemp << G4endl;
	if( !bRecalculateMaterialDensities ) continue; 

	G4int copyNo = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//--- store the minimum and maximum density for each material (just for printing)
	mpite = densiMinMax.find( mateIDs[copyNo] );
	G4double dens = GmGenUtils::GetValue(stemp.c_str() );
	if( dens < (*mpite).second.first ) (*mpite).second.first = dens;
	if( dens > (*mpite).second.second ) (*mpite).second.second = dens;

	//--- Get material from original list of material in file
	int mateID = mateIDs[copyNo];
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
	  mateIDs[copyNo] = thePhantomMaterialsOriginal.size()-1 + mi->id;
	  //	  G4cout << copyNo << " mat new again " << thePhantomMaterialsOriginal.size()-1 + mi->id << " " << mi->id << G4endl;
	} else {
	  matInfo* mi = new matInfo;
	  mi->sumdens = dens;
	  mi->nvoxels = 1;
	  mi->id = newMateDens.size()+1;
	  newMateDens[matdens] = mi;
	  mateIDs[copyNo] = thePhantomMaterialsOriginal.size()-1 + mi->id;
	  //	  G4cout << copyNo << " mat new first " << thePhantomMaterialsOriginal.size()-1 + mi->id << G4endl;
	}
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << copyNo << " = " << atoi(cid)-1 << G4endl;
				      //	mateIDs[copyNo] = atoi(cid)-1;
      }
    }
  }

  if( bRecalculateMaterialDensities ) { 
    for( mpite = densiMinMax.begin(); mpite != densiMinMax.end(); mpite++ ){
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) G4cout << "GmReadPhantomGeometry::ReadVoxelDensities ORIG MATERIALS DENSITY " << (*mpite).first << " MIN " << (*mpite).second.first << " MAX " << (*mpite).second.second << G4endl;
#endif
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
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) G4cout << "GmReadPhantomGeometry::ReadVoxelDensities AVER DENS " << averdens << " -> " << saverdens << " -> " << G4int(1000*averdens) << " " << G4int(1000*averdens)/1000 << " " <<  G4int(1000*averdens)/1000. << G4endl;
#endif

    G4String mateName = ((*mppite).first).first->GetName() + "_" + GmGenUtils::ftoa(saverdens);
    thePhantomMaterials.push_back( BuildMaterialChangingDensity( (*mppite).first.first, averdens, mateName ) );
  }

}
