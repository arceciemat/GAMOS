#include "GmPSPrinterInterfile.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4PhantomParameterisation.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmInterfileHeader.hh"
#include "GamosCore/GamosReadDICOM/include/GmInterfile.hh"
#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosGeometry/include/GmReadPhantomGeometry.hh"

#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

GmPSPrinterInterfile::GmPSPrinterInterfile(G4String name) : GmVPSPrinter( name )
{
  theOutputType = "interfile";
}

void GmPSPrinterInterfile::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  SetUnit(scorer);
  
  if( !scorer->ScoreErrors() ) {
    G4Exception("GmPSPrinterInterfile::DumpAll ",
		"Scorer is not scoring errors",FatalErrorInArgument,scorer->GetName());
  }

  WriteInterfile(RunMap,scorer);
}


//-----------------------------------------------------------------------
GmInterfileHeader* GmPSPrinterInterfile::BuildInterfileHeader()
{
  GmInterfileHeader* header = new GmInterfileHeader;

  //  float nev = GmNumberOfEvent::GetNumberOfEvent();
  //  header->SetNumberOfEvents( nev );

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
      G4Exception("GmPSPrinterInterfile::BuildInterfileHeader",
		  "",
		  FatalException,
		  (" There are not 3 phantom parameterisations, but " + GmGenUtils::itoa( thePhantomParams.size() ) + ". You are not using GmCrossPhantomGeometry").c_str());
    }
    G4VSolid* solidX = thePhantomParams[0]->GetContainerSolid();
    if( solidX->GetName() != "phantomContainerX" ) {
      G4Exception("GmPSPrinterInterfile::BuildInterfileHeader",
		  "",
		  FatalException,
		  ("First phantom parameterisation is not of type 'phantomContainerX', but " + solidX->GetName() + " There are more than one phantom parameterisations and it is not of type GmCrossPhantomGeometry").c_str());
    }

    G4VSolid* solidYN = thePhantomParams[1]->GetContainerSolid();
    if( solidYN->GetName() != "phantomContainerYN" ) {
      G4Exception("GmPSPrinterInterfile::BuildInterfileHeader",
		  "",
		  FatalException,
		  ("First phantom parameterisation is not of type 'phantomContainerYN', but " + solidYN->GetName() + " There are more than one phantom parameterisations and it is not of type GmCrossPhantomGeometry").c_str());
    }
    
    G4VSolid* solidYP = thePhantomParams[2]->GetContainerSolid();
    if( solidYP->GetName() != "phantomContainerYP" ) {
      G4Exception("GmPSPrinterInterfile::BuildInterfileHeader",
		  "",
		  FatalException,
		  ("First phantom parameterisation is not of type 'phantomContainerYP', but " + solidYP->GetName() + " There are more than one phantom parameterisations and it is not of type GmCrossPhantomGeometry").c_str());
    }

    nx = thePhantomParams[0]->GetNoVoxelsX();
    ny = thePhantomParams[1]->GetNoVoxelsY()+thePhantomParams[2]->GetNoVoxelsY()+1;
    nz = thePhantomParams[0]->GetNoVoxelsZ();
  }
  header->SetNoVoxelsX(nx);
  header->SetNoVoxelsY(ny);
  header->SetNoVoxelsZ(nz);

  //---- Get the positon of the phantom negative corner (-x,-y,-z)
  G4ThreeVector thePhantomMinusCorner ;
  float voxelDimX = thePhantomParams[0]->GetVoxelHalfX()*2;
  float voxelDimY = thePhantomParams[0]->GetVoxelHalfY()*2;
  float voxelDimZ = thePhantomParams[0]->GetVoxelHalfZ()*2;
  header->SetVoxelDimX(voxelDimX);
  header->SetVoxelDimY(voxelDimY);
  header->SetVoxelDimZ(voxelDimZ);
  GmReadPhantomGeometry* phGeom = dynamic_cast<GmReadPhantomGeometry*>(const_cast<G4VUserDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction()));
  G4ThreeVector minV;
  if( phGeom ) {
    //  phCentre -= phGeom->GetInitialDisp();
    //  phCentre *= phRotMat; // print in the coordinates of the phantom without rotation
    //  thePhantomMinusCorner += phCentre;
    minV = G4ThreeVector( phGeom->GetMinX(), phGeom->GetMinY(), phGeom->GetMinZ());
    G4cout << " phGeom minV " << minV << G4endl; //GDEB
  } else {
    minV = thePhantomParams[0]->GetTranslation(0); // local translation of first voxel in voxelContainer
    G4cout << " NO phGeom minV " << minV << G4endl; //GDEB
  }
  header->SetMinX(minV.x());
  header->SetMinY(minV.y());
  G4cout << header << " SET MinZ " << minV.z() << G4endl; //GDEB
  header->SetMinZ(minV.z());

  return header;
}

//-----------------------------------------------------------------------
void GmPSPrinterInterfile::WriteInterfile( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  G4String fileName = "interfile.out";
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

  OpenFileOut(fileName+".v");
  GmInterfileHeader* header = BuildInterfileHeader();
  header->SetDataFileName(fileName+".v");
  header->Print(fileName+".hv");
  G4int nVoxelsX = header->GetNoVoxelsX();
  G4int nVoxelsY = header->GetNoVoxelsY();
  G4int nVoxelsZ = header->GetNoVoxelsZ();
  G4int nvoxels = nVoxelsX * nVoxelsY * nVoxelsZ;
  //  G4cout << " NVOXELS " << nvoxels << " X " << nVoxelsX << " Y " << nVoxelsY << " Z " << nVoxelsZ << G4endl; //GDEB

  //----- Loop to all voxels and print data
  std::vector<float> dataV;
  std::map<G4int,G4double*>* scorerMap = RunMap->GetMap();
  for( G4int ii = 0; ii < nvoxels; ii++ ){
    std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
    float data;
    if( ite != scorerMap->end() ){
      data = (*(ite->second))*theUnitRatio;
    } else {
      data = 0.;
    }
    G4cerr << " SCORE Interfife " << (*ite).first << " : " << data << " = " << (*(ite->second))*theUnitRatio << G4endl; //GDEB
   
    dataV.push_back( data );
    /*    G4int nx = ii % nVoxelsX;
    G4int ny = ii/nVoxelsX % nVoxelsY;
    G4int nz = ii/nVoxelsX/nVoxelsY % nVoxelsZ;
    G4cout << ii << " NXYZ " << nx << " " << ny << " " << nz << " GmPSPrinterInterfile::WriteInterfile data " << data << " theUnitRatio " << theUnitRatio << G4endl; */ //GDEB
  }
  
  GmInterfile interfile;
  interfile.SetHeader( header );

  G4cout << " interfile.SetData " << G4endl; //GDEB
  for( G4int ii = 0; ii < 10; ii++ ) {
    G4cout << " interfile.SetData " << ii << " : " << dataV[ii] << G4endl; //GDEB
  }
  interfile.SetData( dataV );

  interfile.Print( theFileOut );

  //  CloseFileOut();
}
