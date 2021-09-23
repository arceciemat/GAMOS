#include "globals.hh"

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

#include "GmCrossPhantomGeometry.hh"
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
GmCrossPhantomGeometry::GmCrossPhantomGeometry()
{
  GmGenUtils::itoa(12);
  new G4tgrMessenger;

  parameterisedPhysVolume = 0;
}

//---------------------------------------------------------------------------
GmCrossPhantomGeometry::~GmCrossPhantomGeometry()
{
}


//---------------------------------------------------------------------------
void GmCrossPhantomGeometry::ReadPhantomData()
{

  theMaterialNames = GmParameterMgr::GetInstance()->GetVStringValue("GmCrossPhantomGeometry:MaterialNames",theMaterialNames);
  std::vector<G4double> matez;
  matez = GmParameterMgr::GetInstance()->GetVNumericValue("GmCrossPhantomGeometry:MaterialZVoxels",matez);

  G4int izv = 0;
  for( size_t ii = 0; ii < matez.size(); ii++ ){
    izv += G4int(matez[ii]);
    theMaterialZVoxels[izv] = ii;
    //    G4cout << " theMaterialZVoxels[izv] " << theMaterialZVoxels[izv] << " izv " << izv << " ii " << ii << G4endl;
  }
 
  //  G4cout << " theMaterialZVoxels.size() != theMaterialNames.size() " << theMaterialZVoxels.size() << " " <<  theMaterialNames.size()  << G4endl;
  if( theMaterialZVoxels.size() != theMaterialNames.size() ) {
    G4Exception("GmCrossPhantomGeometry::ReadPhantomData","Wrong number of material Z voxels",FatalErrorInArgument,G4String(G4String("Number of material Z voxels = ") + GmGenUtils::itoa(theMaterialZVoxels.size()) + G4String(" different than number of materials = ") + GmGenUtils::itoa(theMaterialNames.size())).c_str());
  }
 
  theMaterialDensities = GmParameterMgr::GetInstance()->GetVNumericValue("GmCrossPhantomGeometry:MaterialDensities",theMaterialDensities);
  if( theMaterialDensities.size() != theMaterialNames.size() ) {
    G4Exception("GmCrossPhantomGeometry::ReadPhantomData","Wrong number of material densities",FatalErrorInArgument,G4String(G4String("Number of material densities = ") + GmGenUtils::itoa(theMaterialDensities.size()) + G4String(" different than number of materials = ") + GmGenUtils::itoa(theMaterialNames.size())).c_str());
  }
  
  //------- Materials
  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 

  for( size_t ii = 0; ii < theMaterialNames.size(); ii++ ){
    G4double density = theMaterialDensities[ii]; // BuildMaterialChangingDensity uses g/cm3 
    //    G4cout << " mate " << ii << " density " << density << G4endl;
    G4Material* mate = matmgr->FindOrBuildG4Material(theMaterialNames[ii]);
    G4String mateName = mate->GetName()+GmGenUtils::ftoa(density);
    if( fabs(density - mate->GetDensity() )/density > 0.001 ) {
      thePhantomMaterials.push_back(BuildMaterialChangingDensity(mate,density,mateName));
    } else {
      thePhantomMaterials.push_back(mate);
    }
    //    G4cout << " MATE " << *(thePhantomMaterials[ii]) << G4endl;
  }

  std::vector<G4double> vdou;
  vdou = GmParameterMgr::GetInstance()->GetVNumericValue("GmCrossPhantomGeometry:NVoxels",vdou);
  if( vdou.size() != 3 ){
    G4cerr << " GmCrossPhantomGeometry:NVoxels must have 3 parameters, it has " << vdou.size() << G4endl;
    G4Exception("GmCrossPhantomGeometry::ReadPhantomData",
		"Wrong argument",
		FatalErrorInArgument,
		"");
  }

  nVoxelX = G4int(vdou[0]);
  nVoxelY = G4int(vdou[1]);
  nVoxelZ = G4int(vdou[2]);
  //  G4cout << " nVoxel X/Y/Z " << nVoxelX << " " << nVoxelY << " " << nVoxelZ << G4endl;
  if( izv != nVoxelZ ) {
    G4Exception("GmCrossPhantomGeometry::ReadPhantomData",
		"Number of voxels in Z is different that total number of Z materials defined",
		FatalErrorInArgument,
		G4String("Number of voxels in Z = " 
			 + GmGenUtils::itoa(nVoxelZ)
			 + " total number of Z materials = " 
			 + GmGenUtils::itoa(izv)).c_str());
  }

  vdou.clear();
  vdou = GmParameterMgr::GetInstance()->GetVNumericValue("GmCrossPhantomGeometry:PhantomDims",vdou);
  if( vdou.size() != 6 ){
    G4cerr << " GmCrossPhantomGeometry:PhantomDims must have 6 parameters, it has " << vdou.size() << G4endl;
    G4Exception("GmCrossPhantomGeometry::ReadPhantomData",
		"Wrong argument",
		FatalErrorInArgument,
		"");
  }
  offsetX = vdou[0];
  offsetY = vdou[2];
  offsetZ = vdou[4];
  dimX = (vdou[1]-vdou[0])/nVoxelX;
  dimY = (vdou[3]-vdou[2])/nVoxelY;
  dimZ = (vdou[5]-vdou[4])/nVoxelZ;

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(infoVerb) ){
    G4cout << "GmCrossPhantomGeometry dimX " << dimX << " offsetX " << offsetX << G4endl;
    G4cout << "GmCrossPhantomGeometry dimY " << dimY << " offsetY " << offsetY << G4endl;
    G4cout << "GmCrossPhantomGeometry dimZ " << dimZ << " offsetZ " << offsetZ << G4endl;
  }
#endif

  theMateIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
  G4int imate;
  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    std::map<G4int,G4int>::iterator ite = theMaterialZVoxels.upper_bound( iz );
    if( ite == theMaterialZVoxels.end() ) {
      imate = theMaterialZVoxels.size()-1;
    } else {
      imate = (*ite).second;
    }
    //    G4cout << " setting mate Z " << iz << " = " << imate << G4endl;
    for( G4int ix = 0; ix < nVoxelX; ix++ ) {
      //      G4cout << " stemp " << stemp << G4endl;
      for( G4int iy = 0; iy < nVoxelY; iy++ ) {
	G4int nnew = iy + (ix)*nVoxelY + (iz)*nVoxelX*nVoxelY;
	
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	theMateIDs[nnew] = imate;
      }
    }
  }

}

//---------------------------------------------------------------------------
void GmCrossPhantomGeometry::ConstructPhantom(G4LogicalVolume* parentLV)
{
  G4String OptimAxis = GmParameterMgr::GetInstance()->GetStringValue("GmReadPhantomGeometry:Phantom:OptimAxis", "kXAxis");
  G4bool bSkipEqualMaterials = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:SkipEqualMaterials", 0));
  G4int regStructureID = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:RegularStructureID", 1));

  G4tgbMaterialMgr* matmgr = G4tgbMaterialMgr::GetInstance(); 
  theMatePhantom = matmgr->FindOrBuildG4Material("G4_AIR");
  //----- Build 3 phantom containers to form the Cross
  G4RotationMatrix* rotm = new G4RotationMatrix;
  rotm->rotateX( theInitialRotAngleX);
  rotm->rotateY( theInitialRotAngleY);
  rotm->rotateZ( theInitialRotAngleZ);
  G4ThreeVector posCentreVoxelsX(offsetX+nVoxelX*dimX/2.,offsetY+nVoxelY*dimY/2.,offsetZ+nVoxelZ*dimZ/2.+00.);
  posCentreVoxelsX.rotateX( theInitialRotAngleX);
  posCentreVoxelsX.rotateY( theInitialRotAngleY);
  posCentreVoxelsX.rotateZ( theInitialRotAngleZ);
  posCentreVoxelsX += theInitialDisp;

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
  phantom_logic->SetSmartless( theSmartless );

  //---- Build voxels along X
  G4Box* cont_solidX = new G4Box("phantomContainerX",nVoxelX*dimX/2.,dimY/2.,nVoxelZ*dimZ/2.+0.*G4GeometryTolerance::GetInstance()->GetSurfaceTolerance());
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry::constructPhantomX phantomContainer solid " << *cont_solidX << G4endl;
#endif
  G4LogicalVolume* cont_logicX = 
    new G4LogicalVolume( cont_solidX, 
			 theMatePhantom,
			 "phantomContainerX", 
			 0, 0, 0 );
  G4bool pcVisX = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:PhantomContainer:VisOff",1));
  if( !pcVisX ) {
    G4VisAttributes* visAtt = new G4VisAttributes( FALSE );
    cont_logicX->SetVisAttributes( visAtt );
  }
  cont_logicX->SetSmartless( theSmartless );

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << " PhantomContainer posCentreVoxelsX " << posCentreVoxelsX << G4endl;
#endif

  //  G4cout << " PLACING PHANTOM CONTAINER " << *rotm << " POS " << posCentreVoxels << " theInitialDisp " << theInitialDisp << G4endl; //GDEB
  G4VPhysicalVolume * cont_physX =  new G4PVPlacement(rotm,  // rotation
						     posCentreVoxelsX,
						      cont_logicX,     // The logic volume
						      "phantomContainerX",  // Name
						      parentLV,               // Mother
						      false,           // No op. bool.
						      1);              // Copy number

  //----- Build phantom
  G4PhantomParameterisation* phantomParamX = new G4PhantomParameterisation();
  phantomParamX->SetMaterials( thePhantomMaterials );
  phantomParamX->SetVoxelDimensions( dimX/2., dimY/2., dimZ/2. );
  phantomParamX->SetNoVoxels( nVoxelX, 1, nVoxelZ );
  phantomParamX->SetMaterialIndices( theMateIDs );
  G4cout << phantomParamX << " phantomParamX " << G4endl; //GDEB
  //  G4cout << " Number of Materials " << thePhantomMaterials.size() << G4endl;
  //  G4cout << " SetMaterialIndices(0) " << mateIDs[0] << G4endl;

  G4PVParameterised * phantom_physX;
  if( OptimAxis == "kUndefined" ) {
    phantom_physX = new G4PVParameterised(voxelName,phantom_logic,cont_logicX,
					 kUndefined, nVoxelX*1*nVoxelZ, phantomParamX);
  } else   if( OptimAxis == "kXAxis" ) {
    //    G4cout << " optim kX " << G4endl;
    phantom_physX = new G4PVParameterised(voxelName,phantom_logic,cont_logicX,
					 kXAxis, nVoxelX*1*nVoxelZ, phantomParamX);
    phantom_physX->SetRegularStructureId(regStructureID);
    phantomParamX->SetSkipEqualMaterials(bSkipEqualMaterials);
  } else {
    G4Exception("GmReadPhantomGeometry::ConstructPhantom","Wrong argument to parameter GmReadPhantomGeometry:Phantom:OptimAxis",FatalErrorInArgument,(G4String("Only allowed 'kUndefined' or 'kXAxis', it is: "+OptimAxis).c_str()));
  }
  
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry  phantom_physX " << phantom_physX << " trans " << phantom_physX->GetTranslation() << G4endl;
#endif

  phantomParamX->BuildContainerSolid(cont_physX);

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry::constructPhantomX ended " << G4endl;
#endif
 
  //---- Build voxels along Y negative
  if( nVoxelY%2 != 1 ) {
    G4Exception("GmCrossPhantomGeometry::ConstructPhantom",
		"",
		FatalException,
		(G4String("Number of voxels in Y dimension has to be odd, it is ") + GmGenUtils::itoa(nVoxelY)).c_str());
  }

  G4ThreeVector posCentreVoxelsYN(offsetX+nVoxelX*dimX/2.,offsetY+(nVoxelY-1)/2.*dimY/2.,offsetZ+nVoxelZ*dimZ/2.+0.);
  posCentreVoxelsYN.rotateX( theInitialRotAngleX);
  posCentreVoxelsYN.rotateY( theInitialRotAngleY);
  posCentreVoxelsYN.rotateZ( theInitialRotAngleZ);
  posCentreVoxelsYN += theInitialDisp;
  
  G4Box* cont_solidYN = new G4Box("phantomContainerYN",dimX/2.,(nVoxelY-1)/2*dimY/2.,nVoxelZ*dimZ/2.+0.*G4GeometryTolerance::GetInstance()->GetSurfaceTolerance());
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry::constructPhantomYN phantomContainer solid " << *cont_solidYN << G4endl;
#endif
  G4LogicalVolume* cont_logicYN = 
    new G4LogicalVolume( cont_solidYN, 
			 theMatePhantom,
			 "phantomContainerYN", 
			 0, 0, 0 );
  G4bool pcVisYN = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:PhantomContainer:VisOff",1));
  if( !pcVisYN ) {
    G4VisAttributes* visAtt = new G4VisAttributes( FALSE );
    cont_logicYN->SetVisAttributes( visAtt );
  }
  cont_logicYN->SetSmartless( theSmartless );

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << " PhantomContainer posCentreVoxelsYN " << posCentreVoxelsYN << G4endl;
#endif

  //  G4cout << " PLACING PHANTOM CONTAINER " << *rotm << " POS " << posCentreVoxels << " theInitialDisp " << theInitialDisp << G4endl; //GDEB
  G4VPhysicalVolume * cont_physYN =  new G4PVPlacement(rotm,  // rotation
						     posCentreVoxelsYN,
						      cont_logicYN,     // The logic volume
						      "phantomContainerYN",  // Name
						      parentLV,               // Mother
						      false,           // No op. bool.
						      2);              // Copy number

  //----- Build phantom
  G4PhantomParameterisation* phantomParamYN = new G4PhantomParameterisation();
  G4cout << phantomParamYN << " phantomParamYN " << G4endl; //GDEB
  phantomParamYN->SetMaterials( thePhantomMaterials );
  phantomParamYN->SetVoxelDimensions( dimX/2., dimY/2., dimZ/2. );
  phantomParamYN->SetNoVoxels( 1, (nVoxelY-1)/2, nVoxelZ );
  phantomParamYN->SetMaterialIndices( theMateIDs );
  //  G4cout << " Number of Materials " << thePhantomMaterials.size() << G4endl;
  //  G4cout << " SetMaterialIndices(0) " << mateIDs[0] << G4endl;

  G4PVParameterised * phantom_physYN;
  if( OptimAxis == "kUndefined" ) {
    phantom_physYN = new G4PVParameterised(voxelName,phantom_logic,cont_logicYN,
					   kUndefined, 1*(nVoxelY-1)/2*nVoxelZ, phantomParamYN);
  } else   if( OptimAxis == "kXAxis" ) {
    //    G4cout << " optim kX " << G4endl;
    phantom_physYN = new G4PVParameterised(voxelName,phantom_logic,cont_logicYN,
					  kXAxis, 1*(nVoxelY-1)/2*nVoxelZ, phantomParamYN);
    phantom_physYN->SetRegularStructureId(regStructureID);
    phantomParamYN->SetSkipEqualMaterials(bSkipEqualMaterials);
  } else {
    G4Exception("GmReadPhantomGeometry::ConstructPhantom","Wrong argument to parameter GmReadPhantomGeometry:Phantom:OptimAxis",FatalErrorInArgument,(G4String("Only allowed 'kUndefined' or 'kXAxis', it is: "+OptimAxis).c_str()));
  }
  

#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry  phantom_physYN " << phantom_physYN << " trans " << phantom_physYN->GetTranslation() << G4endl;
#endif

  phantomParamYN->BuildContainerSolid(cont_physYN);

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry::constructPhantomYN ended " << G4endl;
#endif
  
  
  //---- Build voxels along Y positive
  if( nVoxelY%2 != 1 ) {
    G4Exception("GmCrossPhantomGeometry::ConstructPhantom",
		"",
		FatalException,
		(G4String("Number of voxels in Y dimension has to be odd, it is ") + GmGenUtils::itoa(nVoxelY)).c_str());
  }

  G4ThreeVector posCentreVoxelsYP(offsetX+nVoxelX*dimX/2.,-(offsetY+(nVoxelY-1)/2.*dimY/2.),offsetZ+nVoxelZ*dimZ/2.+0.);
  posCentreVoxelsYP.rotateX( theInitialRotAngleX);
  posCentreVoxelsYP.rotateY( theInitialRotAngleY);
  posCentreVoxelsYP.rotateZ( theInitialRotAngleZ);
  posCentreVoxelsYP += theInitialDisp;

  G4Box* cont_solidYP = new G4Box("phantomContainerYP",dimX/2.,(nVoxelY-1)/2*dimY/2.,nVoxelZ*dimZ/2.+0.*G4GeometryTolerance::GetInstance()->GetSurfaceTolerance());
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry::constructPhantomYP phantomContainer solid " << *cont_solidYP << G4endl;
#endif
  G4LogicalVolume* cont_logicYP = 
    new G4LogicalVolume( cont_solidYP, 
			 theMatePhantom,
			 "phantomContainerYP", 
			 0, 0, 0 );
  G4bool pcVisYP = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:PhantomContainer:VisOff",1));
  if( !pcVisYP ) {
    G4VisAttributes* visAtt = new G4VisAttributes( FALSE );
    cont_logicYP->SetVisAttributes( visAtt );
  }
  cont_logicYP->SetSmartless( theSmartless );

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) G4cout << " PhantomContainer posCentreVoxelsYP " << posCentreVoxelsYP << G4endl;
#endif

  //  G4cout << " PLACING PHANTOM CONTAINER " << *rotm << " POS " << posCentreVoxels << " theInitialDisp " << theInitialDisp << G4endl; //GDEB
  G4VPhysicalVolume * cont_physYP =  new G4PVPlacement(rotm,  // rotation
						     posCentreVoxelsYP,
						      cont_logicYP,     // The logic volume
						      "phantomContainerYP",  // Name
						      parentLV,               // Mother
						      false,           // No op. bool.
						      3);              // Copy number

  //----- Build phantom
  G4PhantomParameterisation* phantomParamYP = new G4PhantomParameterisation();
  G4cout << phantomParamYP << " phantomParamYP " << G4endl; //GDEB
  phantomParamYP->SetMaterials( thePhantomMaterials );
  phantomParamYP->SetVoxelDimensions( dimX/2., dimY/2., dimZ/2. );
  phantomParamYP->SetNoVoxels( 1, (nVoxelY-1)/2, nVoxelZ );
  phantomParamYP->SetMaterialIndices( theMateIDs );
  //  G4cout << " Number of Materials " << thePhantomMaterials.size() << G4endl;
  //  G4cout << " SetMaterialIndices(0) " << mateIDs[0] << G4endl;

  G4PVParameterised * phantom_physYP;
  if( OptimAxis == "kUndefined" ) {
    phantom_physYP = new G4PVParameterised(voxelName,phantom_logic,cont_logicYP,
					   kUndefined, 1*(nVoxelY-1)/2*nVoxelZ, phantomParamYP);
  } else   if( OptimAxis == "kXAxis" ) {
    //    G4cout << " optim kX " << G4endl;
    phantom_physYP = new G4PVParameterised(voxelName,phantom_logic,cont_logicYP,
					  kXAxis, 1*(nVoxelY-1)/2*nVoxelZ, phantomParamYP);
    phantom_physYP->SetRegularStructureId(regStructureID);
    phantomParamYP->SetSkipEqualMaterials(bSkipEqualMaterials);
  } else {
    G4Exception("GmReadPhantomGeometry::ConstructPhantom","Wrong argument to parameter GmReadPhantomGeometry:Phantom:OptimAxis",FatalErrorInArgument,(G4String("Only allowed 'kUndefined' or 'kXAxis', it is: "+OptimAxis).c_str()));
  }
  

#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry  phantom_physYP " << phantom_physYP << " trans " << phantom_physYP->GetTranslation() << G4endl;
#endif

  phantomParamYP->BuildContainerSolid(cont_physYP);

#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) )  G4cout << " GmReadPhantomGeometry::constructPhantomYP ended " << G4endl;
#endif
  
}

