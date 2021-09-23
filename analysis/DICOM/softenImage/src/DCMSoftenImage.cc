#include "DCMSoftenImage.hh"

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomOperSoften.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderCT.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmCT.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmNM.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmDose.hh"
#include "DICOM/DICOMReaders/include/DicomReaderSqdose.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdoseHeader.hh"

#include <iostream>
#include <map>

//---------------------------------------------------------------------------
DCMSoftenImage::DCMSoftenImage()
{
  theExeName = "softenImage";

  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
  theDicomMgr = DicomMgr::GetInstance();
}
 
//---------------------------------------------------------------------------
void DCMSoftenImage::ProcessArguments(int argc,char** argv)
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
    G4int iAddPar = theParamMgr->ReadParameter( argv, ii );
    G4String argvName = argvstr.substr(1,999);
    if( argvstr == "-factor"){
      theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
      ii++;
    } else if( argvstr == "-nAround"){
      theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
      ii++;
    } else if( argvstr == "-maxValue"){
      theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
      ii++;
    } else if( argvstr == "-minSpareValue"){
      theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
      ii++;
    } else if( argvstr == "-maxSpareValue"){
      theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
      ii++;
    } else {
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

  theFactor = theParamMgr->GetNumericValue("factor",2.);
  theNAround = G4int(theParamMgr->GetNumericValue("nAround",1));
  theMaxValue = theParamMgr->GetNumericValue("maxValue",DBL_MAX);
  theMinSpareValue = theParamMgr->GetNumericValue("minSpareValue",-DBL_MAX);
  theMaxSpareValue = theParamMgr->GetNumericValue("minSpareValue",-DBL_MAX);

}

//---------------------------------------------------------------------------
void DCMSoftenImage::CheckArguments()
{
}
  
//---------------------------------------------------------------------------
void DCMSoftenImage::ReadFilesAndGetImages()
{
  //--- READ AND BUILD IMAGES
  theReaderMgr->CreateReaders();
  theReaderMgr->SetCTOnlyHU(true);
  theReaderMgr->CreateImages();
  DicomOperSoften* theSoftenOperator = new DicomOperSoften(1,"OperResize");  
  
  //--- GET INPUT IMAGES AND WRITE NEW IMAGES
  std::vector<DicomVReaderImage*> imageReaders;
  imageReaders =  theReaderMgr->GetImageReaders(DRM_NM,false);
  /*  if( imageReaders.size() != 0 ) {
    G4Exception(theExeName, 
		"",
		FatalException,
		"DICOM NM image not implemented yet. Please ask the GAMOS team");
		} */

  imageReaders = theReaderMgr->GetImageReaders(DRM_RTDose,false);
  /*  if( imageReaders.size() != 0 ) {
    G4Exception(theExeName, 
		"",
		FatalException,
		"DICOM RTDose image not implemented yet. Please ask the GAMOS team");
		} */

  if( theReaderMgr->GetNofImageReaders(DRM_OTHER) == 0 ) {
    G4Exception(theExeName, 
		"",
		FatalException,
		"No image file selected");
  }

  G4String fileName = theParamMgr->GetStringValue("fOut","");
  if( fileName != "" && theReaderMgr->GetNofImageReaders(DRM_OTHER) != 1 ) {
    G4Exception(theExeName, 
		"",
		FatalException,
		"-fOut parameter used but there is more than one image file");
  }

  imageReaders =  theReaderMgr->GetImageReaders(DRM_G4dcmCT,false);

  G4cout << " N READERS " << imageReaders.size() << G4endl; //GDEB
  for( size_t ii = 0; ii < imageReaders.size(); ii++ ) {    
    DicomReaderG4dcmCT* readerCT = dynamic_cast<DicomReaderG4dcmCT*>(imageReaders[ii]);
    if( fileName == "" ) fileName = readerCT->GetFileName()+".new";
    G4cout << " @@@ Dumping To Text File: " << fileName << G4endl;
    std::ofstream fout(fileName.c_str());

    static std::map<G4int,G4String> materialNames = readerCT->GetMaterialNames();
    fout << materialNames.size() << std::endl;
    std::map<G4int,G4String>::const_iterator ite;
    for(ite = materialNames.begin(); ite != materialNames.end(); ite++){
      fout << (*ite).first << " \"" << (*ite).second << "\"" << std::endl;
    }
        
    DicomVImage* image = readerCT->GetMateIDImage();
    image->DumpHeaderToTextFile(fout);
    image->DumpDataToTextFile(fout);
    image = readerCT->GetMateDensImage();
    image->DumpDataToTextFile(fout);
    image = readerCT->GetStructIDImage();
    if( image ) image->DumpDataToTextFile(fout);
  }
  
  imageReaders =  theReaderMgr->GetImageReaders(DRM_G4dcmNM,false);
  for( size_t ii = 0; ii < imageReaders.size(); ii++ ) {    
    DicomReaderG4dcmNM* readerNM = dynamic_cast<DicomReaderG4dcmNM*>(imageReaders[ii]);
    if( fileName == "" ) fileName = readerNM->GetFileName()+".new";
    G4cout << " @@@ Dumping To Text File: " << fileName << G4endl;
    std::ofstream fout(fileName.c_str());
    DicomVImage* image = readerNM->GetImage();
    image->DumpHeaderToTextFile(fout);
    image->DumpDataToTextFile(fout);
  }

  imageReaders = theReaderMgr->GetImageReaders(DRM_G4dcmDose,false);
  for( size_t ii = 0; ii < imageReaders.size(); ii++ ) {    
    DicomReaderG4dcmDose* readerDose = dynamic_cast<DicomReaderG4dcmDose*>(imageReaders[ii]);
    if( fileName == "" ) fileName = readerDose->GetFileName()+".new";
    G4cout << " @@@ Dumping To Text File: " << fileName << G4endl;
    std::ofstream fout(fileName.c_str());
    DicomVImage* image = readerDose->GetImage();
    image->DumpHeaderToTextFile(fout);
    image->DumpDataToTextFile(fout);
  }
  
  imageReaders =  theReaderMgr->GetImageReaders(DRM_RTDose,false);
  if( imageReaders.size() != 0 ) {
    G4Exception(theExeName, 
		"",
		FatalException,
		"DICOM RTDose image not implemented yet. Please ask the GAMOS team");
  }

  imageReaders =  theReaderMgr->GetImageReaders(DRM_Sqdose,false);
  for( size_t ii = 0; ii < imageReaders.size(); ii++ ) {    
    DicomReaderSqdose* readerSqdose = dynamic_cast<DicomReaderSqdose*>(imageReaders[0]);
    if( fileName == "" ) fileName = readerSqdose->GetFileName()+".new";
    G4cout << " @@@ Dumping To Sqdose File: " << fileName << G4endl;
    FILE* fout = fopen(fileName,"wb");
    GmSqdose* sqdose = readerSqdose->GetSqdose();
    GmSqdoseHeader* sqdoseHeader = sqdose->GetHeader();
    DicomVImage* doseImage = readerSqdose->GetDoseImage();
    theSoftenOperator->Operate(doseImage, theFactor, theNAround, theMaxValue, theMinSpareValue, theMaxSpareValue);
    G4cout << " DOSEIMAGE SOFTENED " << doseImage->GetData(1476) << G4endl; //GDEB
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
		  ("Only sqdose type  SqTALL is supported, while file " + fileName + " is of type " + sqdose->GetTypeStr()).c_str());
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
    G4cout << " DOSEIMAGE SOFTENED WRITE " << theData->at(1476) << G4endl; //GDEB

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

}

//---------------------------------------------------------------------------
void DCMSoftenImage::PrintHelp()
{
  G4cout << " -minX        change all voxels with upper X boundary < minX" << G4endl
	 << " -maxX        change all voxels with lower X boundary > maxX" << G4endl
	 << " -minY        change all voxels with upper Y boundary < minY" << G4endl
	 << " -maxY        change all voxels with lower Y boundary > maxY" << G4endl
	 << " -minZ        change all voxels with upper Z boundary < minZ" << G4endl
	 << " -maxZ        change all voxels with lower Z boundary > maxZ" << G4endl
    	 << " -verb        sets the verbosity: test/debug/info/warning/error/silent" << G4endl
	 << " -h           prints the set of arguments " << G4endl
	 << " -help        prints the set of arguments " << G4endl;
}
  
