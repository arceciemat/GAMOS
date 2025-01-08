#include "GmPSPrinterSqdose.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4PhantomParameterisation.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdoseHeader.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosGeometry/include/GmReadPhantomGeometry.hh"

#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

//-----------------------------------------------------------------------
GmPSPrinterSqdose::GmPSPrinterSqdose(G4String name) : GmVPSPrinter( name )
{
  theOutputType = "sqdose";
}

//-----------------------------------------------------------------------
void GmPSPrinterSqdose::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  SetUnit(scorer);
  
  if( !scorer->ScoreErrors() ) {
    G4Exception("GmPSPrinterSqdose::DumpAll ",
		"Scorer is not scoring errors",FatalErrorInArgument,scorer->GetName());
  }

  WriteSqdose(RunMap,scorer);
}


//-----------------------------------------------------------------------
GmSqdoseHeader* GmPSPrinterSqdose::BuildSqdoseHeader()
{
  GmSqdoseHeader* header = new GmSqdoseHeader;

  float nev = GmNumberOfEvent::GetNumberOfEvent();
  header->SetNumberOfEvents( nev );

  thePhantomParams = GmGeometryUtils::GetInstance()->GetPhantomParams( TRUE );

  unsigned int nx;
  unsigned int ny;
  unsigned int nz;
  if( thePhantomParams.size() == 1 ) {
    nx = thePhantomParams[0]->GetNoVoxelsX();
    ny = thePhantomParams[0]->GetNoVoxelsY();
    nz = thePhantomParams[0]->GetNoVoxelsZ();
    // CrossPhantom
  } else {
    if( thePhantomParams.size() != 3 ) {
      G4Exception("GmPSPrinterSqdose::BuildSqdoseHeader",
		  "",
		  FatalException,
		  (" There are not 3 phantom parameterisations, but " + GmGenUtils::itoa( thePhantomParams.size() ) + ". You are not using GmCrossPhantomGeometry").c_str());
    }
    G4VSolid* solidX = thePhantomParams[0]->GetContainerSolid();
    if( solidX->GetName() != "phantomContainerX" ) {
      G4Exception("GmPSPrinterSqdose::BuildSqdoseHeader",
		  "",
		  FatalException,
		  ("First phantom parameterisation is not of type 'phantomContainerX', but " + solidX->GetName() + " There are more than one phantom parameterisations and it is not of type GmCrossPhantomGeometry").c_str());
    }

    G4VSolid* solidYN = thePhantomParams[1]->GetContainerSolid();
    if( solidYN->GetName() != "phantomContainerYN" ) {
      G4Exception("GmPSPrinterSqdose::BuildSqdoseHeader",
		  "",
		  FatalException,
		  ("First phantom parameterisation is not of type 'phantomContainerYN', but " + solidYN->GetName() + " There are more than one phantom parameterisations and it is not of type GmCrossPhantomGeometry").c_str());
    }
    
    G4VSolid* solidYP = thePhantomParams[2]->GetContainerSolid();
    if( solidYP->GetName() != "phantomContainerYP" ) {
      G4Exception("GmPSPrinterSqdose::BuildSqdoseHeader",
		  "",
		  FatalException,
		  ("First phantom parameterisation is not of type 'phantomContainerYP', but " + solidYP->GetName() + " There are more than one phantom parameterisations and it is not of type GmCrossPhantomGeometry").c_str());
    }

    nx = thePhantomParams[0]->GetNoVoxelsX();
    ny = thePhantomParams[1]->GetNoVoxelsY()+thePhantomParams[2]->GetNoVoxelsY()+1;
    nz = thePhantomParams[0]->GetNoVoxelsZ();
  }
  header->SetNoVoxelsX( nx );
  header->SetNoVoxelsY( ny );
  header->SetNoVoxelsZ( nz );

  //---- Get the positon of the phantom negative corner (-x,-y,-z)
  G4ThreeVector thePhantomMinusCorner ;
  /*  if( thePhantomParams.size() == 1 ) {
    thePhantomMinusCorner  = (thePhantomParams[0]->GetTranslation(0)
			      + thePhantomParams[0]->GetTranslation(thePhantomParams[0]->GetNoVoxels()-1) )/ 2;
  } else {
    G4ThreeVector translation0X = thePhantomParams[0]->GetTranslation(0);
    //    G4ThreeVector trans( (2*nx+1)*fVoxelHalfX - fContainerWallX,
    //                       (2*ny+1)*fVoxelHalfY - fContainerWallY,
    //                       (2*nz+1)*fVoxelHalfZ - fContainerWallZ);
    //      fContainerWallX = fNoVoxelsX * fVoxelHalfX;
    G4double fHalfVoxelYN = thePhantomParams[1]->GetVoxelHalfY();
    G4double fContainerWallYN = fHalfVoxelYN * thePhantomParams[1]->GetNoVoxelsY();
    fContainerWallYN = 2*fContainerWallYN+fHalfVoxelYN;
    G4int ny0 = 0;
    G4double pos0Y = (2*ny0+1)*fHalfVoxelYN - fContainerWallYN;
    G4ThreeVector translation0(translation0X.x(),pos0Y,translation0X.z());

    G4ThreeVector translation1X = thePhantomParams[0]->GetTranslation(thePhantomParams[0]->GetNoVoxels()-1);
    G4double fHalfVoxelYP = thePhantomParams[2]->GetVoxelHalfY();
    G4double fContainerWallYP = fHalfVoxelYP * thePhantomParams[2]->GetNoVoxelsY();
    fContainerWallYP = 2*fContainerWallYP+fHalfVoxelYP;
    G4int ny1 = thePhantomParams[2]->GetNoVoxelsY();
    ny1 = 2*ny1+1-1; // first ny=0
    G4double pos1Y = (2*ny1+1)*fHalfVoxelYP - fContainerWallYP;
    G4ThreeVector translation1(translation1X.x(),pos1Y,translation1X.z());
    thePhantomMinusCorner = (translation0+translation1)/2;
    //    G4cout << " phantom minus corner (temp) " << thePhantomMinusCorner << " translation0 " << translation0 << " 1 " << translation1 << " 0X " << translation0X << " 1X " << translation1X << G4endl; //GDEB
    //-    G4cout << " phantom minus corner (temp) " << thePhantomMinusCorner << " translation0 " << translation0 << " 1 " << translation1 << " 0X " << translation0X << " 0YN " << translation0YN << " 1X " << translation1X << " 1YP " << translation1YP << " 1YN " << thePhantomParams[1]->GetTranslation(thePhantomParams[1]->GetNoVoxels()-1) << " 0YP " << thePhantomParams[2]->GetTranslation(0) << G4endl; //GDEB
    }
  float voxelX = thePhantomParams[0]->GetVoxelHalfX()*2;
  float voxelY = thePhantomParams[0]->GetVoxelHalfY()*2;
  float voxelZ = thePhantomParams[0]->GetVoxelHalfZ()*2;
  //  G4cout << " phantom minus corner (temp) " << thePhantomMinusCorner << " translation 0 " << thePhantomParam->GetTranslation(0) << " translation 1 " << thePhantomParam->GetTranslation(thePhantomParam->GetNoVoxels()-1) << G4endl;
  // G4cout << " voxelX " << thePhantomParam->GetVoxelHalfX() << " = " << voxelX <<  "  " << -voxelX*nx/2. << " " << G4ThreeVector( -nx*voxelX/2.,-ny*voxelY/2.,-nz*voxelZ/2.) << G4endl;
  thePhantomMinusCorner -= G4ThreeVector(voxelX*nx/2.,voxelY*ny/2.,voxelZ*nz/2.);
  // G4cout << " phantom minu corner " << thePhantomMinusCorner << G4endl;
  //----- Add coordinates of phantom volume 
  G4LogicalVolume* thePhantomVolume = GmRegularParamUtils::GetInstance()->GetPhantomMotherVolume( TRUE );
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( thePhantomVolume->GetName() );
  GmTouchable* pcont = touchs[0];
 
  G4ThreeVector phCentre = pcont->GetGlobalPosition();
  //  G4RotationMatrix phRotMat = pcont->GetGlobalRotMat();
  //  phCentre *= phRotMat; // print in the coordinates of the phantom without rotation
  */
  float voxelX = thePhantomParams[0]->GetVoxelHalfX()*2;
  float voxelY = thePhantomParams[0]->GetVoxelHalfY()*2;
  float voxelZ = thePhantomParams[0]->GetVoxelHalfZ()*2;

  GmReadPhantomGeometry* phGeom = dynamic_cast<GmReadPhantomGeometry*>(const_cast<G4VUserDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction()));
  if( phGeom ) {
    //  phCentre -= phGeom->GetInitialDisp();
    //  phCentre *= phRotMat; // print in the coordinates of the phantom without rotation
    //  thePhantomMinusCorner += phCentre;
    thePhantomMinusCorner = G4ThreeVector( phGeom->GetMinX(), phGeom->GetMinY(), phGeom->GetMinZ());
  } else {
    thePhantomMinusCorner = thePhantomParams[0]->GetTranslation(0); // local translation of first voxel in voxelContainer
    thePhantomMinusCorner -= G4ThreeVector( voxelX/2., voxelY/2., voxelZ/2. );
    //     thePhantomMinusCorner += thePhantomParams[0]->GetTranslation(thePhantomParams[0]->GetNoVoxels()-1)/ 2;
    //     G4cout << thePhantomMinusCorner << " Nx " << nx << " voxelX " << voxelX << G4endl; //GDEB
    G4LogicalVolume* thePhantomVolume = GmRegularParamUtils::GetInstance()->GetPhantomMotherVolume( TRUE );
    std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( thePhantomVolume->GetName() );
    GmTouchable* pcont = touchs[0];
    G4ThreeVector phCentre = pcont->GetLocalPosition();
  }

  std::vector<float> vval;
  for( unsigned int ii = 0; ii <= nx; ii++ ) {
    vval.push_back( thePhantomMinusCorner.x() + ii*voxelX );
    //    G4cout << ii << " set voxellimitX " << thePhantomMinusCorner.x() + ii*voxelX << G4endl; //GDEB
  }

  header->SetVoxelLimitsX( vval );

  vval.clear();
  //  *theDoseFile << G4endl;
  for( unsigned int ii = 0; ii <= ny; ii++ ) {
    vval.push_back( thePhantomMinusCorner.y() + ii*voxelY );
    // *theDoseFile << thePhantomMinusCorner.y() + ii*voxelY << " ";
    //    G4cout << ii << " set voxellimitY " << thePhantomMinusCorner.y() + ii*voxelY << G4endl; //GDEB
  }
  //  *theDoseFile << G4endl;
  header->SetVoxelLimitsY( vval );

  vval.clear();
  for( unsigned int ii = 0; ii <= nz; ii++ ) {
    vval.push_back( thePhantomMinusCorner.z() + ii*voxelZ );
    //  *theDoseFile << thePhantomMinusCorner.z() + ii*voxelZ << " ";
    //    G4cout << ii << " set voxellimitZ " << thePhantomMinusCorner.z() + ii*voxelZ << G4endl; //GDEB
  }
  header->SetVoxelLimitsZ( vval );

  return header;
}

//-----------------------------------------------------------------------
void GmPSPrinterSqdose::WriteSqdose( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  G4String fileName = "sqdose.out";
  G4String scorerName = "";
  if( scorer ) scorerName = scorer->GetName();
  fileName = GmParameterMgr::GetInstance()->GetStringValue(theName+"_"+scorerName+":FileName",fileName);
  if( GmParameterMgr::GetInstance()->GetNumericValue("GmScoringUA:DoseEachNEvents",0) != 0 ) {
    int nev = int(GmNumberOfEvent::GetNumberOfEvent());
    fileName += "_"+GmGenUtils::itoa(nev);
  }

  G4String suffix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  if(suffix != "" ) fileName += suffix;

  G4String prefix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNamePrefix","");
  if(prefix != "" ) fileName = prefix + fileName;

  OpenFileOut(fileName);

  GmSqdoseHeader* header = BuildSqdoseHeader();


  G4int nvoxels = 0;
  bool bCrossPhantom = 0;
  if( thePhantomParams.size() == 3 ) {
    bCrossPhantom = 1;
  }
  G4int nVoxelsX = 0;
  G4int nVoxelsY = 0;
  G4int nVoxelsZ = 0;
  if( !bCrossPhantom ) {
    nVoxelsX = thePhantomParams[0]->GetNoVoxelsX();
    nVoxelsY = thePhantomParams[0]->GetNoVoxelsY();
    nVoxelsZ = thePhantomParams[0]->GetNoVoxelsZ();
  } else {
    nVoxelsX = thePhantomParams[0]->GetNoVoxelsX();
    nVoxelsY = thePhantomParams[1]->GetNoVoxelsY()*2+1;
    nVoxelsZ = thePhantomParams[0]->GetNoVoxelsZ();
  }
  nvoxels = nVoxelsX * nVoxelsY * nVoxelsZ;
  //  G4cout << " NVOXELS " << nvoxels << " X " << nVoxelsX << " Y " << nVoxelsY << " Z " << nVoxelsZ << G4endl; //GDEB

  //----- Loop to all voxels and print dose
  G4int nVoxelsXHalf = nVoxelsX/2;
  G4int nVoxelsYHalf = nVoxelsY/2;
  std::vector<float> doses;
  std::map<G4int,G4double*>* scorerMap = RunMap->GetMap();
  for( G4int ii = 0; ii < nvoxels; ii++ ){
    if( bCrossPhantom ) {
      G4int nx = ii % nVoxelsX;
      G4int ny = ii/nVoxelsX % nVoxelsY;
      G4int nz = ii/nVoxelsX/nVoxelsY % nVoxelsZ;
      //      G4cout << "CROSSPHANTOM " << ii << " nx=" << nx << " ny=" << ny << " nz=" << nz << G4endl;  //GDEB
      if( nx != nVoxelsXHalf && ny != nVoxelsYHalf ) {
	std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
	float dose;
	if( ite != scorerMap->end() ){
	  dose = (*(ite->second))*theUnitRatio;
	  for ( ite = scorerMap->begin(); ite != scorerMap->end(); ite++ ) {
	    G4cerr << " SCORE " << (*ite).first << " : " << (*ite).second << G4endl; //GDEB
	  }
	  //	  if( dose != 0. ) {
	  //	    G4cerr << " nx " << nx << " !=? " << nVoxelsXHalf << " && " << ny << " !=? " << nVoxelsYHalf << std::endl; //GDEB
	  G4Exception("GmPSPrinterSqdose::WriteSqdose",
		      "",
		      FatalException,
		      G4String("CrossPhantom: voxel outside cross, dose= "+GmGenUtils::ftoa(dose)+" at voxel "+GmGenUtils::itoa(ii)+" nx="+GmGenUtils::itoa(nx)+" ny="+GmGenUtils::itoa(ny)+" nz="+GmGenUtils::itoa(nz)).c_str());
	}
	continue; // ONLY VOXELS IN CROSS
	//      } else {
	//	G4cout << ii << " nx=" << nx << " ny=" << ny << " nz=" << nz << G4endl;  //GDEB
      }
    }
    std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
    float dose;
    if( ite != scorerMap->end() ){
      dose = (*(ite->second))*theUnitRatio;
    } else {
      dose = 0.;
    }
    //    G4cerr << " SCORE Sqdose " << (*ite).first << " : " <<  (*(ite->second))*theUnitRatio << G4endl; //GDEB
    
    doses.push_back( dose );
    /*    G4int nx = ii % nVoxelsX;
    G4int ny = ii/nVoxelsX % nVoxelsY;
    G4int nz = ii/nVoxelsX/nVoxelsY % nVoxelsZ;
    G4cout << ii << " NXYZ " << nx << " " << ny << " " << nz << " GmPSPrinterSqdose::WriteSqdose dose " << dose << " theUnitRatio " << theUnitRatio << G4endl; */ //GDEB
  }
  
  //----- Loop to all voxels and print dose error
  std::vector<float> doseSqs;
  for( G4int ii = 0; ii < nvoxels; ii++ ){
    if( bCrossPhantom ) {
      G4int nx = ii % nVoxelsX;
      G4int ny = ii/nVoxelsX % nVoxelsY;
      G4int nz = ii/nVoxelsX/nVoxelsY % nVoxelsZ;
      if( nx != nVoxelsXHalf && ny != nVoxelsYHalf ) {
	std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
	float dose;
	if( ite != scorerMap->end() ){
	  dose = (*(ite->second))*theUnitRatio;
	  G4Exception("GmPSPrinterSqdose::WriteSqdose",
		      "",
		      FatalException,
		      G4String("CrossPhantom: voxle outside cross, dose= "+GmGenUtils::ftoa(dose)+" at voxel "+GmGenUtils::itoa(ii)+" nx="+GmGenUtils::itoa(nx)+" ny="+GmGenUtils::itoa(ny)+" nz="+GmGenUtils::itoa(nz)).c_str());
	}
	continue; // ONLY VOXELS IN CROSS
      }
    }
    std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
    float dose2;
    if( ite != scorerMap->end() ){
      dose2 = scorer->GetSumV2(ite->first)*theUnitRatio2;
    } else {
      dose2 = 0.;
    }
    doseSqs.push_back( dose2 );
    //    G4cout << ii << " GmPSPrinterSqdose::WriteSqdose dose " << dose2 << G4endl;
  }


  GmSqdose sqdose;
  sqdose.SetHeader( header );
  sqdose.SetDoses( doses );
  sqdose.SetDoseSqs( doseSqs );

  sqdose.Print( theFileOut );

  CloseFileOut();
}
