#include "DCMDmToDw.hh"

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderCT.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmCT.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmNM.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmDose.hh"
#include "DICOM/DICOMReaders/include/DicomReaderSqdose.hh"
#include "DICOM/DICOMReaders/include/DicomReaderRTStruct.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdoseHeader.hh"

#include <iostream>
#include <map>

//---------------------------------------------------------------------------
DCMDmToDw::DCMDmToDw()
{
  theExeName = "DmToDw";

  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
  theDicomMgr = DicomMgr::GetInstance();
}
 
//---------------------------------------------------------------------------
void DCMDmToDw::ProcessArguments(int argc,char** argv)
{
  if(argc%2 != 1) {
    G4Exception(theExeName.c_str(),
		"Error",
		FatalErrorInArgument,
		"WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
  }
  for( G4int ii = 1; ii < argc; ii++ ){
    G4String argvstr = argv[ii];
    G4String argvstr1 = argv[ii+1];
    G4String argvName = argvstr.substr(1,999);
    if( argvstr == "-bInterpolate" ) {
      theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
      ii++;
    } else if( argvstr == "-fSPR" ) {
      theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
      ii++;
    } else if( argvstr == "-fOut" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
	ii++;
    } else {
      G4int iAddPar = theParamMgr->ReadParameter( argv, ii );
      if( iAddPar == -1 ) {
	PrintHelp();
	G4Exception(theExeName.c_str(),
		    "Wrong argument",
		    FatalErrorInArgument,
		    (G4String("ARGUMENT: ") + argv[ii]).c_str());
      }
      ii += iAddPar;
    }
  }

}

//---------------------------------------------------------------------------
void DCMDmToDw::CheckArguments()
{
  std::vector<G4String> sqds = theParamMgr->GetVStringValue("fSqdose",std::vector<G4String>());
  G4cout << "SQDS " << sqds.size() << G4endl; //GDEB
  if( sqds.size() == 0 ) {
    PrintHelp();
    G4Exception(theExeName,
		"No SqDose input file",
		FatalException,
		"Use -fSqdose in command line");
  }
  theSqdoseFN  = sqds[0];

  std::vector<G4String> cts = theParamMgr->GetVStringValue("fG4dcmCT",std::vector<G4String>());
  if( cts.size() == 0 ) {
    PrintHelp();
    G4Exception(theExeName,
		"No G4dcmCT input file",
		FatalException,
		"Use -fG4dcmCT in command line");
  }
  theG4dcmCTFN = cts[0];

  theSPRFN = theParamMgr->GetStringValue("fSPR","");
  if( theSPRFN == "" ) {
    PrintHelp();
    G4Exception(theExeName,
		"No SPR input file",
		FatalException,
		"Use -fSPR in command line");
  }

  theOutFN = theParamMgr->GetStringValue("fOut","");
  if( theOutFN == "" ) {
    PrintHelp();
    G4Exception(theExeName,
		"No output file",
		FatalException,
		"Use -fOut in command line");
  }
  
}
  
//---------------------------------------------------------------------------
void DCMDmToDw::ReadFilesAndGetImages()
{
  //--- READ AND BUILD IMAGES
  theReaderMgr->CreateReaders();
  theReaderMgr->CreateImages();

  G4bool bInterpolate = G4bool(theParamMgr->GetNumericValue("bInterpolate",1));
  theParamMgr->AddParam("bInterpolate "+GmGenUtils::itoa(bInterpolate));

  //--- GET INPUT IMAGES AND WRITE NEW IMAGES
  std::vector<DicomVReaderImage*> imageReaders =  theReaderMgr->GetImageReaders(DRM_Sqdose,false);
  DicomReaderSqdose* readerSqdose = dynamic_cast<DicomReaderSqdose*>(imageReaders[0]);
  GmSqdose* sqdose = readerSqdose->GetSqdose();
  GmSqdoseHeader* sqdoseHeader = sqdose->GetHeader();
  DicomVImage* doseImage = readerSqdose->GetDoseImage();
  
  imageReaders =  theReaderMgr->GetImageReaders(DRM_G4dcmCT,false);
  //  G4cout << " N READERS CT " << imageReaders.size() << G4endl; //GDEB
  DicomReaderG4dcmCT* G4dcmCTReader = dynamic_cast<DicomReaderG4dcmCT*>(imageReaders[0]);
  DicomVImage* imageMateDens = G4dcmCTReader->GetMateDensImage();
  if ( (*imageMateDens == *doseImage) != true ) {
    G4cerr << " Sqdose IMAGE: " << *doseImage
	   << " CT IMAGE: " << *imageMateDens;
    G4Exception(theExeName,
		"",
		FatalException,
		"SqDose and G4dcmCT file have different dimensions");
  }
    
  DicomVImage* imageMateID = G4dcmCTReader->GetMateIDImage();
  std::map<G4int,G4String> materials = G4dcmCTReader->GetMaterialNames();
  //--- Read SPR file
  std::map<G4String,std::map<G4double,G4double> > theSPRs;
  GmFileIn finSPR = GmFileIn::GetInstance(theSPRFN);
  std::vector<G4String> wl;
  for(;;) {
     if( !finSPR.GetWordsInLine(wl) ) break;
     G4String mate = wl[1];
     if( theSPRs.find(mate) == theSPRs.end() ) {
       theSPRs[mate] = std::map<G4double,G4double>();
     }
     G4cout << " ADD SPR " << mate << " " << wl[2]<< " " << wl[4] << G4endl; //GDEB
     theSPRs[mate][GmGenUtils::GetValue(wl[2])] = GmGenUtils::GetValue(wl[4]);
  }
	
  G4int nVoxels = doseImage->GetNoVoxels();
  size_t idNVoxelX = doseImage->GetNoVoxelsX();
  size_t idNVoxelY = doseImage->GetNoVoxelsY();
  size_t idNVoxelXY = idNVoxelX*idNVoxelY;
  G4double idMinX = doseImage->GetMinX();
  G4double idMinY = doseImage->GetMinY();
  G4double idMinZ = doseImage->GetMinZ();
  G4double idVoxDimX = doseImage->GetVoxelDimX();
  G4double idVoxDimY = doseImage->GetVoxelDimY();
  G4double idVoxDimZ = doseImage->GetVoxelDimZ();
  std::vector<G4double>* doseData = doseImage->GetData();
  for( G4int ii = 0; ii < nVoxels; ii++ ) {
    G4int ix = ii%idNVoxelX;
    G4int iy = (ii/idNVoxelX)%idNVoxelY;
    G4int iz = ii/idNVoxelXY;
    // get spr
    G4int mateID = imageMateID->GetData(ii);
    G4String mateName = materials[mateID];
    G4double dens = imageMateDens->GetData(ii);
    if( theSPRs.find(mateName) == theSPRs.end() ) {
      for( auto ite = theSPRs.begin(); ite != theSPRs.end(); ite++ ) {
	G4cerr << " SPR MATERIAL: " << ite->first << G4endl;
      }
      G4Exception(theExeName,
		  "",
		  FatalException,
		  ("Material "+mateName+" not found in spr file "+theSPRFN).c_str());
    } 
    G4double spr = GmGenUtils::InterpolateLinLin(dens,theSPRs[mateName]);
    //G4cout << ii << " SPR= " << spr << " XYZ " << idMinX+(ix+0.5)*idVoxDimX << " " << idMinY+(iy+0.5)*idVoxDimY << " " << idMinZ+(iz+0.5)*idVoxDimZ << " mate " << mateName << " dens " << dens << G4endl; //GDEB 
    doseData->at(ii) *= spr;
  }
    
  //---- WRITE Sqdose OUTPUT FILE
  G4cout << " @@@ Dumping To Text File: " << theOutFN << G4endl;
  FILE* fout = fopen(theOutFN,"wb");
  float theNoEvent = sqdoseHeader->GetNumberOfEvents();
  if(fwrite(&theNoEvent,sizeof(float),1,fout)!=1)
    printf("\n Error writing nEvent. \n");
  
  size_t theNoVoxelsX = doseImage->GetNoVoxelsX();
  if(fwrite(&theNoVoxelsX, sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelX. \n");
  size_t theNoVoxelsY = doseImage->GetNoVoxelsY();
  if(fwrite(&theNoVoxelsY, sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelY. \n");
  size_t theNoVoxelsZ = doseImage->GetNoVoxelsZ();
  if(fwrite(&theNoVoxelsZ, sizeof(size_t),1,fout)!=1)
    printf("\n Error writing nVoxelZ. \n");
  
  //  G4cout << " HEADER NEvent " << theNoEvent << " Nvoxels " << theNoVoxelsX << " " << theNoVoxelsY<< " " << theNoVoxelsZ << G4endl;
  G4double theMinX = doseImage->GetMinX();
  G4double theVoxelDimX = doseImage->GetVoxelDimX();
  float* vlimx = new float[theNoVoxelsX+1];
  for( unsigned int ix = 0; ix <= theNoVoxelsX; ix++ ) {
    vlimx[ix] = theMinX + ix*theVoxelDimX;
    //      G4cout << " VLIMX " << ix << " = " << vlimx[ix] << " = " << theMinX << " + " << ix << "* " << theVoxelDimX << G4endl; //GDEB
  }
  if(fwrite(vlimx, sizeof(float),theNoVoxelsX+1,fout)!=theNoVoxelsX+1)
    printf("\n Error writing VoxelLimitsX. \n");
  G4double theMinY = doseImage->GetMinY();
  G4double theVoxelDimY = doseImage->GetVoxelDimY();
  float* vlimy = new float[theNoVoxelsY+1];
  for( unsigned int iy = 0; iy <= theNoVoxelsY; iy++ ) {
    vlimy[iy] = theMinY + iy*theVoxelDimY;
  }
  if(fwrite(vlimy, sizeof(float),theNoVoxelsY+1,fout)!=theNoVoxelsY+1)
    printf("\n Error writing VoxelLimitsY. \n");
  G4double theMinZ = doseImage->GetMinZ();
  G4double theVoxelDimZ = doseImage->GetVoxelDimZ();
  float* vlimz = new float[theNoVoxelsZ+1];
  for( unsigned int iz = 0; iz <= theNoVoxelsZ; iz++ ) {
    vlimz[iz] = theMinZ + iz*theVoxelDimZ;
  }
  if(fwrite(vlimz, sizeof(float),theNoVoxelsZ+1,fout)!=theNoVoxelsZ+1)
    printf("\n Error writing VoxelLimitsZ. \n");
  
  G4RotationMatrix theRotationMatrix;
  float rotxx = theRotationMatrix.xx();
  if(fwrite(&rotxx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxx. \n");
  float rotxy = theRotationMatrix.xy();
  if(fwrite(&rotxy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxy. \n");
  float rotxz = theRotationMatrix.xz();
  if(fwrite(&rotxz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotxz. \n");
  float rotyx = theRotationMatrix.yx();
  if(fwrite(&rotyx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyx. \n");
  float rotyy = theRotationMatrix.yy();
  if(fwrite(&rotyy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyy. \n");
  float rotyz = theRotationMatrix.yz();
  if(fwrite(&rotyz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotyz. \n");
  float rotzx = theRotationMatrix.zx();
  if(fwrite(&rotzx, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzx. \n");
  float rotzy = theRotationMatrix.zy();
  if(fwrite(&rotzy, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzy. \n");
  float rotzz = theRotationMatrix.zz();
  if(fwrite(&rotzz, sizeof(float),1,fout)!=1)
    printf("\n Error writing rotzz. \n");
  
  delete [] vlimx;
  delete [] vlimy;
  delete [] vlimz;
  
  //--- WRITE DOSE & DOSESQ
  std::vector<G4double>* theData = doseImage->GetData();
  DicomVImage* doseSqImage = readerSqdose->GetDoseSqImage();
  std::vector<G4double>* theDataSq = doseSqImage->GetData();
  
  SqdoseType theType = sqdose->GetType();
  if( theType != SqTALL ) {
    G4Exception(theExeName,
		"",
		FatalException,
		("Only sqdose type  SqTALL is supported, while file " + theOutFN + " is of type " + sqdose->GetTypeStr()).c_str());
  }
  if(fwrite(&theType, sizeof(size_t),1,fout)!=1) {
    G4Exception("GmSqdose::Print",
		"Error",
		FatalException,
		"Error writing type");
  }
  
  size_t siz = doseImage->GetNoVoxels();
  G4String thePatientPosition = readerSqdose->GetPatientPosition();
  size_t theNoVoxelsXY = theNoVoxelsX*theNoVoxelsY;
  //  G4cout << " N VOXELS WRITE " << siz << G4endl;  //GDEB
  
  //----- Loop to all voxels and print dose
  for( size_t iv = 0; iv < siz; iv++ ) {
    size_t copyNo = iv;
    if( thePatientPosition == "FFS" ) {
      size_t ix = iv%theNoVoxelsX;
      size_t iy = (iv/theNoVoxelsX)%theNoVoxelsY;
      size_t iz = iv/theNoVoxelsXY;
      copyNo = ix+iy*theNoVoxelsX+(theNoVoxelsZ-1-iz)*theNoVoxelsXY;
    }
    float dose = theData->at(copyNo);
    if(fwrite((float *) &dose, sizeof(float),1,fout)!=1) {
      G4Exception(theExeName,
		  "",
		  FatalException,
		  "Error writing dose");
    }
  }
  for( size_t iv = 0; iv < siz; iv++ ) {
    size_t copyNo = iv;
    if( thePatientPosition == "FFS" ) {
      size_t ix = iv%theNoVoxelsX;
      size_t iy = (iv/theNoVoxelsX)%theNoVoxelsY;
      size_t iz = iv/theNoVoxelsXY;
      copyNo = ix+iy*theNoVoxelsX+(theNoVoxelsZ-1-iz)*theNoVoxelsXY;
    }
    float doseSq = theDataSq->at(copyNo);
    if(fwrite((float *) &doseSq, sizeof(float),1,fout)!=1) {
      G4Exception(theExeName,
		  "",
		  FatalException,
		  "Error writing doseSq");
    }
  }
}

//---------------------------------------------------------------------------
void DCMDmToDw::PrintHelp()
{
  G4cout << " -fSPR        file name containing SPR list for each material and amaterial density " << G4endl
	 << " -fOut        file name of sqdose file with new doses, rescaled by SPR " << G4endl
    	 << " -verb        sets the verbosity: test/debug/info/warning/error/silent" << G4endl
	 << " -h           prints the set of arguments " << G4endl
	 << " -help        prints the set of arguments " << G4endl;
}
  
