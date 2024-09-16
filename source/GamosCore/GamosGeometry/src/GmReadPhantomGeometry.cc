#include "GmReadPhantomGeometry.hh"

#include "GmGeomVerbosity.hh"
#include "GmDICOMIntersectVolume.hh"
#include "GmOpticalPropertiesMgr.hh"

#include "G4PhantomParameterisation.hh"
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
#include "GmGeomTextDetectorBuilder.hh"
#include "GmGeometryUtils.hh"
#include "GmRegionCutsMgr.hh"
#include "GmGeometryUtils.hh"
#include "GmParallelWorldCreator.hh"
#include "G4tgbParallelGeomMgr.hh"
#include "G4VUserParallelWorld.hh"

//---------------------------------------------------------------------------
GmReadPhantomGeometry::GmReadPhantomGeometry()
{
  new G4tgrMessenger;

  theFileName = "";
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  parameterisedPhysVolume = 0;

  std::vector<G4double> inidisp = paramMgr->GetVNumericValue("GmReadPhantomGeometry:InitialDisplacement", std::vector<G4double>());
  if( inidisp.size() == 0 ){
    theInitialDisp = G4ThreeVector(0.,0.,0.);
  } else if( inidisp.size() == 3 ){
    theInitialDisp = G4ThreeVector(inidisp[0],inidisp[1],inidisp[2]);
  } else {
    G4Exception("GmReadPhantomGeometry","Error in /gamos/setParam GmReadPhantomGeometry:InitialDisp",FatalErrorInArgument,G4String("it must have 3 arguments: POS_X POS_Y POS_Z, and it has "+GmGenUtils::itoa(inidisp.size())).c_str());
  }

  std::vector<G4double> inirot = paramMgr->GetVNumericValue("GmReadPhantomGeometry:InitialRotAngles", std::vector<G4double>());
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

  bRecalculateMaterialDensities = G4bool(paramMgr->GetNumericValue("GmReadPhantomGeometry:RecalculateMaterialDensities", 1));

  new GmDICOMIntersectVolume;
}

//---------------------------------------------------------------------------
GmReadPhantomGeometry::~GmReadPhantomGeometry()
{
  delete cont_logic;
  delete parameterisedPhysVolume; 

  //  delete [] theMatePhantom;
  delete [] theMateIDs;
  delete [] theMateDensities;
}


//---------------------------------------------------------------------------
G4VPhysicalVolume* GmReadPhantomGeometry::Construct()
{
  //------------------- construct g4 geometry
  theFileName = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:FileName", theFileName);
  if( theFileName == "" ){
    G4Exception("GmReadPhantomGeometry::Construct",
		"",
		FatalException,
		"No file name defined, please use  /gamos/setParam GmReadPhantomGeometry:FileName FILE_NAME ");
  }
  
  theFileName = GmGenUtils::FileInPath( theFileName );

  GmGeomTextDetectorBuilder* gtb = new GmGeomTextDetectorBuilder;
  G4tgbVolumeMgr* volmgr = G4tgbVolumeMgr::GetInstance();
  volmgr->SetDetectorBuilder( gtb );
  volmgr->AddTextFile(theFileName);


  std::vector<G4String> fnamepar;
  fnamepar = GmParameterMgr::GetInstance()->GetVStringValue("GmReadPhantomGeometry:FileNameParallel", fnamepar);
  if( fnamepar.size() != 0 ){
     if( fnamepar.size()%2 != 0 ){
      G4Exception("GmReadPhantomGeometry::Construct",
		  "Error in number of arguments of parameter 'GmReadPhantomGeometry:FileNameParallel'",
		  FatalException,
		  (G4String("There should be a multiple of two: FILE_NAME_1 PARALLEL_WORLD_NUMBER_1 FILE_NAME_2 PARALLEL_WORLD_NUMBER_2 ..., there are ")
		   +GmGenUtils::itoa(fnamepar.size())).c_str());
    }
    for( unsigned int ii = 0; ii < fnamepar.size(); ii+=2 ) {
      volmgr->AddTextFileParallel(fnamepar[ii],GmGenUtils::GetInteger(fnamepar[ii+1]));
    }
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
    if( GeomVerb(debugVerb) ) G4cout << "GmReadPhantomGeometry::Construct RegisterParallelWorld " << parallelWorlds[ii]->GetName() << G4endl;
#endif
    RegisterParallelWorld( parallelWorlds[ii] );
   
  }

  //------------------------------------------------ 
  G4String motherName = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:MotherName",theMotherName);
  G4VPhysicalVolume* physiWorld = gtb->ConstructDetectorGAMOS(tgrVoltop,-1,0, false);
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
  G4int nvox = thePhantomParam->GetNoVoxels();
  for( size_t ii = 0; ii < nvox; ii++ ){
    G4cout << ii << " PARAM ID " << (thePhantomParam->ComputeMaterial( ii, (G4VPhysicalVolume *) 0, (const G4VTouchable *)0))->GetName() << G4endl;
    }*/

  // Dummy declarations ...

  //  G4cout << "GmReadPhantomGeometry CALL  GmOpticalPropertiesMgr::GetInstance()->BuildG4() " << G4endl; //GDEB
  GmOpticalPropertiesMgr::GetInstance()->BuildG4();
    
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
  cont_logic = 
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
      if( GeomVerb(debugVerb) ) G4cout << " PhantomContainer posCentreVoxels " << posCentreVoxels << G4endl;
#endif

  G4RotationMatrix* rotm = new G4RotationMatrix;
  //  rotm->rotateX(45.*deg);
  rotm->rotateX( theInitialRotAngleX);
  //  G4cout << " ReadPhantomGeometry original " << *rotm << G4endl; //GDEB
  rotm->rotateY( theInitialRotAngleY);
  rotm->rotateZ( theInitialRotAngleZ);
  //  G4cout << " ReadPhantomGeometry original rotated " << *rotm << G4endl; //GDEB
  *rotm = CLHEP::inverseOf(*rotm);
  
  //  G4cout << "GmReadPhantomGeometry posCentre before rot " <<   posCentreVoxels << G4endl; //GDEB
  posCentreVoxels.rotateX( theInitialRotAngleX);
  posCentreVoxels.rotateY( theInitialRotAngleY);
  posCentreVoxels.rotateZ( theInitialRotAngleZ);
  //  G4cout << "GmReadPhantomGeometry posCentre after rot " << posCentreVoxels << " + " << theInitialDisp << G4endl; //GDEB

  posCentreVoxels += theInitialDisp;
  //  G4cout << "GmReadPhantomGeometry posCentre final " << posCentreVoxels << G4endl; //GDEB

  //  G4cout << "PLACING PHANTOM CONTAINER " << *rotm << " POS " << posCentreVoxels << " theInitialDisp " << theInitialDisp << G4endl; //GDEB
  G4VPhysicalVolume * cont_phys =  new G4PVPlacement(rotm,  // rotation
						     posCentreVoxels,
		      cont_logic,     // The logic volume
		      "phantomContainer",  // Name
		      parentLV,               // Mother
		      false,           // No op. bool.
		      1);              // Copy number

  //  G4cout << cont_phys << " CENTER PHANTOM CONTAINER " << cont_phys->GetTranslation() << G4endl; //GDEB
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
  thePhantomParam->SetNoVoxels( nVoxelX, nVoxelY, nVoxelZ );
  thePhantomParam->SetMaterialIndices( theMateIDs );

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
      if( GeomVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry  phantom_phys " << phantom_phys << " trans " << phantom_phys->GetTranslation() << G4endl;
#endif

  thePhantomParam->BuildContainerSolid(cont_phys);

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry::constructPhantom ended " << G4endl;
#endif

}


//---------------------------------------------------------------------------
G4Material* GmReadPhantomGeometry::BuildMaterialChangingDensity( G4Material* origMate, float density, G4String mateName )
{
  if( density == 0 ) {
    G4Exception("GmReadPhantomGeometry::BuildMaterialChangingDensity",
		"",
		JustWarning,
		("Material " + origMate->GetName() + " HAS 0 DENSITY, it is changed to 1e-25 g/cm3").c_str());
    density = 1.e-25;
  }    
  G4Material* mate = 0;
  if( bRecalculateMaterialDensities ) {
    G4int nelem = origMate->GetNumberOfElements();
    mate = new G4Material( mateName, density*CLHEP::g/CLHEP::cm3, nelem, kStateUndefined, CLHEP::STP_Temperature );

    for( G4int ii = 0; ii < nelem; ii++ ){
      G4double frac = origMate->GetFractionVector()[ii];
      G4Element* elem = const_cast<G4Element*>(origMate->GetElement(ii));
      mate->AddElement( elem, frac );
    }
    //    G4cout << " IONISATION POTENTIAL " << mate->GetName() << " = " << mate->GetIonisation()->GetMeanExcitationEnergy() << G4endl; //GDEB
    mate->GetIonisation()->SetMeanExcitationEnergy(origMate->GetIonisation()->GetMeanExcitationEnergy());
    
    G4double ioniPot = GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:IonisationPotential",-1);
    if( ioniPot != -1 ) mate->GetIonisation()->SetMeanExcitationEnergy(ioniPot);

#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmReadPhantomGeometry::BuildMaterialChangingDensity new material " << *mate << " density " << density <<  G4endl;
#endif

  } else {
    mate = origMate;
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmReadPhantomGeometry::BuildMaterialChangingDensity keep original material " << *mate << " density " << density <<  G4endl;
#endif
  }

  return mate;
}


void GmReadPhantomGeometry::ReadVoxelDensities( std::ifstream& fin )
{
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(warningVerb) ) G4cout << " ReadVoxelDensities " << G4endl;
#endif
  G4String stemp;
  std::map<G4int, std::pair<G4double,G4double> > densiMinMax;
  std::map<G4int, std::pair<G4double,G4double> >::iterator mpite;
  for( size_t ii = 0; ii < thePhantomMaterialsOriginal.size(); ii++ ){
    densiMinMax[ii] = std::pair<G4double,G4double>(DBL_MAX,-DBL_MAX);
  }

  G4double densityStep = GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:DensityStep", DBL_MAX);
  if( densityStep != DBL_MAX ) {
    G4Exception("GmReadPhantomGeometry",
		"",
		FatalErrorInArgument,
		"GmReadPhantomGeometry:Phantom:DensityStep IS DEPRECATED, PLEASE USE GmReadPhantomGeometry:Phantom:DensityNSplit INSTEAD");
  }
  G4double densityNSplit = GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:DensityNSplit", 100);

  std::map<G4int,G4double> densitySteps;
  //---- Read the material densities and calculate density min/max for each material (to calculate densitySteps)
  theMateDensities = new float[nVoxelX*nVoxelY*nVoxelZ];

  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	//	G4cout << " stemp " << stemp << G4endl;
	fin >> stemp; 
	//	if( ix == 0 && iy == 0 ) G4cout << ix << " " << iy << " " << iz << " density " << stemp << G4endl; //GDEB
	G4double dens = GmGenUtils::GetValue(stemp.c_str() );
	G4int copyNo = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	theMateDensities[copyNo] = dens;

#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(testVerb) ) {
	  if( copyNo%1000000 == 1 ) G4cout << "ReadVoxelDensities mate IDs: " << copyNo << G4endl; //GDEB
	}
#endif
	//--- store the minimum and maximum density for each material 
	mpite = densiMinMax.find( theMateIDs[copyNo] );
	if( dens < (*mpite).second.first ) (*mpite).second.first = dens;
	if( dens > (*mpite).second.second ) (*mpite).second.second = dens;
      }
    }
  }

  /*  for( size_t ii = 0; ii < thePhantomMaterialsOriginal.size(); ii++ ){
    densitySteps[ii] = densityStep; //currently all materials with same step
    }*/
  //  densitySteps[0] = 0.0001; //air
  for( size_t ii = 0; ii < thePhantomMaterialsOriginal.size(); ii++ ){
    mpite = densiMinMax.find( ii );
    densitySteps[ii] = ((*mpite).second.second-(*mpite).second.first)/densityNSplit; //each materials a different density step
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) {
	G4cout << ii << " " << thePhantomMaterialsOriginal[ii]->GetName() << " ReadVoxelDensities  densitySteps " << densitySteps[ii] << " min " << (*mpite).second.first << " max " << (*mpite).second.second << " densityNSplit " << densityNSplit << G4endl; 
      }
#endif
  }

  std::map< std::pair<G4Material*,G4int>, matInfo* > newMateDens;
  if( bRecalculateMaterialDensities ) {
  //--- Calculate the average material density for each material/density bin
    //---- Read the material densities
    G4int nVoxels = nVoxelX*nVoxelY*nVoxelZ;
    for( G4int copyNo = 0; copyNo < nVoxels; copyNo++ ) {
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(testVerb) ) {
	if( copyNo%1000000 == 1 ) G4cout << "ReadVoxelDensities mate Densities: " << copyNo << G4endl; //GDEB
      }
#endif
      //--- Get material from original list of materials in file
      int mateID = theMateIDs[copyNo];
      std::map<G4int,G4Material*>::const_iterator imite = thePhantomMaterialsOriginal.find(mateID);
      //	G4cout << copyNo << " mateID " << mateID << G4endl;
      //--- Check if density is equal to the original material density
      G4double dens = theMateDensities[copyNo];
      if( fabs(dens - (*imite).second->GetDensity()/CLHEP::g*CLHEP::cm3 ) < 1.e-9 ) continue;
      
      //--- Build material name with thePhantomMaterialsOriginal name + density
      // Get to which densityBin of the original material mateID it corresponds
      G4int densityBin = (G4int(dens/densitySteps[mateID]));
      //      G4String mateName = (*imite).second->GetName()+GmGenUtils::ftoa(densityBin);
      //      G4cout << " densityBin " << densityBin << " " << dens << " " <<densitySteps[mateID] << " mateName= " << mateName << G4endl; 
      //--- Look if it is the first voxel with this material/densityBin
      std::pair<G4Material*,G4int> matdens((*imite).second, densityBin );      
      std::map< std::pair<G4Material*,G4int>, matInfo* >::iterator mppite = newMateDens.find( matdens );
      if( mppite != newMateDens.end() ){
	matInfo* mi = (*mppite).second;
	mi->sumdens += dens;
	mi->nvoxels++;
	theMateIDs[copyNo] = thePhantomMaterialsOriginal.size() + mi->id;
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(testVerb) ) {
	  G4cout << copyNo << "ReadVoxelDensities mat new repeated " << thePhantomMaterialsOriginal.size()-1 + mi->id << " " << mi->id << " dens " << dens << G4endl; 
      	}
#endif
      } else {
	matInfo* mi = new matInfo;
	mi->sumdens = dens;
	mi->nvoxels = 1;
	mi->id = newMateDens.size();
	newMateDens[matdens] = mi;
	theMateIDs[copyNo] = thePhantomMaterialsOriginal.size() + mi->id;
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(debugVerb) ) {
	  G4cout << copyNo << " " << newMateDens.size() << " mat new first " << thePhantomMaterialsOriginal.size() + mi->id  << " dens " << dens << " miid " << mi->id << " mate " << (*imite).second->GetName() << " densityBin " << densityBin << G4endl; //
	}
#endif
      }
      theMateDensities[copyNo] = dens;
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(testVerb) ) {
	G4cout << "ReadVoxelDensities " << copyNo << " filling mateIDs " << theMateIDs[copyNo] << " from " << mateID << " mateDens " << theMateDensities[copyNo] << G4endl;
	//	G4cout << ix << "ReadVoxelDensities " << iy << " " << iz << " " << copyNo << " filling mateIDs " << theMateIDs[copyNo] << " from " << mateID << " mateDens " << theMateDensities[copyNo] 	       << " POS= " << offsetX+dimX*2*(ix+0.5) << "," << offsetY+dimY*2*(iy+0.5) << "," << offsetZ+dimZ*2*(iz+0.5) << G4endl;
      }
#endif
      //	mateIDs[copyNo] = atoi(cid)-1;
    }
  }   

  //  G4cout << " ReadVoxelDensities ENDED " << G4endl; //GDEB

  if( bRecalculateMaterialDensities ) { 
    for( mpite = densiMinMax.begin(); mpite != densiMinMax.end(); mpite++ ){
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << "GmReadPhantomGeometry::ReadVoxelDensities ORIG MATERIALS DENSITY " << (*mpite).first << " MIN " << (*mpite).second.first << " MAX " << (*mpite).second.second << G4endl;
#endif
    }
  }

  //----- Build the list of phantom materials that go to Parameterisation
  //--- Add original materials
  std::map<G4int,G4Material*>::const_iterator mimite;
  thePhantomMaterials.resize( thePhantomMaterialsOriginal.size() + newMateDens.size() );
  size_t iim = 0;
  for( mimite = thePhantomMaterialsOriginal.begin(); mimite != thePhantomMaterialsOriginal.end(); mimite++, iim++){
    //    thePhantomMaterials.push_back( (*mimite).second );
    thePhantomMaterials[iim] = (*mimite).second;
  }
  // 
  //---- Build and add new materials
  std::map< std::pair<G4Material*,G4int>, matInfo* >::iterator mppite;
  for( mppite= newMateDens.begin(); mppite != newMateDens.end(); mppite++ ){
    G4double averdens = (*mppite).second->sumdens/(*mppite).second->nvoxels;
    G4double saverdens = G4int(100000.001*averdens)/100000.;
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << "GmReadPhantomGeometry::ReadVoxelDensities AVER DENS " << averdens << " -> " << saverdens << " -> " << G4int(100000*averdens) << " " << G4int(100000.0001*averdens)/100000 << " " <<  G4int(100000*averdens)/100000. << G4endl;
#endif
    G4String mateName = ((*mppite).first).first->GetName() + "_" + GmGenUtils::ftoa(saverdens);
    //  thePhantomMaterials.push_back( BuildMaterialChangingDensity( (*mppite).first.first, averdens, mateName ) );
    thePhantomMaterials[(*mppite).second->id+iim] = BuildMaterialChangingDensity( (*mppite).first.first, averdens, mateName );
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(infoVerb) ) {
        G4cout << "GmReadPhantomGeometry::ReadVoxelDensities ADD MATERIAL " << (*mppite).second->id+iim	   << " ID " << thePhantomMaterials[(*mppite).second->id+iim]->GetName()
	       << " from " << (*mppite).first.first->GetName()
	       << " dens " << thePhantomMaterials[(*mppite).second->id+iim]->GetDensity()/CLHEP::g*CLHEP::cm3
	       << " averdens " << averdens << " saverdens " << saverdens << G4endl;
      }
#endif
  }

  // for( size_t ii = 0; ii < thePhantomMaterials.size(); ii++ ) {
  //   G4cout << " PHANTOM MATERIAL " << ii << " : " << thePhantomMaterials[ii]->GetName() << " " << thePhantomMaterials[ii]->GetDensity()/CLHEP::g*CLHEP::cm3  << G4endl;  //GDEB
  // }

  //  for( size_t ii = 0; ii < thePhantomMaterialsOriginal.size(); ii++ ){
  //   G4cout << " DENSITY STEP  " << densitySteps[ii] << G4endl; //GDEB
  //  }

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(testVerb) ) {
    G4int nVoxels = nVoxelX*nVoxelY*nVoxelZ;
    for( G4int copyNo = 0; copyNo < nVoxels; copyNo++ ) {
      G4cout << copyNo << " VOXEL_MATEID " << theMateIDs[copyNo] << " DENS= " << theMateDensities[copyNo] << " ~= " << thePhantomMaterials[theMateIDs[copyNo]]->GetDensity()/CLHEP::g*CLHEP::cm3 << G4endl;
    }
  }
#endif
}
