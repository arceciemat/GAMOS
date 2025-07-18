#include "DCMCleanOutsideStruct.hh"

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmCT.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "CLHEP/Random/RandFlat.h"

#include <iostream>
#include <map>

//---------------------------------------------------------------------------
DCMCleanOutsideStruct::DCMCleanOutsideStruct()
{
  Initialise();
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
  theDicomMgr = DicomMgr::GetInstance();
  bImageLimits = false;
  theImageMinX = -DBL_MAX;
  theImageMaxX = DBL_MAX;
  theImageMinY = -DBL_MAX;
  theImageMaxY = DBL_MAX;
  theImageMinZ = -DBL_MAX;
  theImageMaxZ = DBL_MAX;
}

//---------------------------------------------------------------------------
void DCMCleanOutsideStruct::Initialise()
{
  theExeName = "cleanOutsideStruct";
  theAnalyseImage = 0;
  theMateDensImage = 0;
  theStructImage = 0;

  bChangeMaterials = false;
  bChangeDensities = false;
  theStructDataListFN = "";
  theStructureName = "";
  theMaterialName = "";
  theMaterialID = -1;
  theDensity = -1.;
  bStOnly = 0;
  bGivenStructData = false;
  bImageG4dcmCT = false;
  theOutFN = "";
}
 
//---------------------------------------------------------------------------
void DCMCleanOutsideStruct::ProcessArguments(int argc,char** argv)
{
  if(argc%2 != 1) {
    G4Exception(theExeName.c_str(),
		"Error",
		FatalErrorInArgument,
		"WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
  }
  for( G4int ii = 1; ii < argc; ii++ ){
    G4String argvstr = argv[ii];
    if( argvstr == "-struct" ) {
      theStructureName = argv[ii+1];
      bGivenStructData = true; 
      ii++;
    } else if( argvstr == "-bStructOnly" ) {
      bStOnly = GmGenUtils::GetBool(argv[ii+1]);
      bGivenStructData = true;
      ii++;
    } else if( argvstr == "-minX" ) {
      theImageMinX = GmGenUtils::GetValue(argv[ii+1]);
      bImageLimits = true;
      ii++;
    } else if( argvstr == "-maxX" ) {
      theImageMaxX = GmGenUtils::GetValue(argv[ii+1]);
      bImageLimits = true;
      ii++;
    } else if( argvstr == "-minY" ) {
      theImageMinY = GmGenUtils::GetValue(argv[ii+1]);
      bImageLimits = true;
      ii++;
    } else if( argvstr == "-maxY" ) {
      theImageMaxY = GmGenUtils::GetValue(argv[ii+1]);
      bImageLimits = true;
      ii++;
    } else if( argvstr == "-minZ" ) {
      theImageMinZ = GmGenUtils::GetValue(argv[ii+1]);
      bImageLimits = true;
      ii++;
    } else if( argvstr == "-maxZ" ) {
      theImageMaxZ = GmGenUtils::GetValue(argv[ii+1]);
      bImageLimits = true;	  
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
void DCMCleanOutsideStruct::CheckArguments()
{
  //--- THERE MUST BE AN OUTPUT FILE  
  theOutFN = theParamMgr->GetStringValue("fOut",theOutFN);
  if( theOutFN == "" ) {
    G4Exception(theExeName,
		"",
		FatalException,
		"No output file, please use the parameter -fOut");
  }

  size_t nfim = theParamMgr->GetNofImageFiles("ANY");
  //--- THERE MUST BE AN INPUT IMAGE FILE
  if( nfim != 1 ) {
    G4Exception(theExeName, 
		"",
		FatalException,
		"NO image files");
  }
  //--- THERE MUST BE A RTSTRUCT FILE (TO DO: USE a fG4dcmCT
  size_t nfst = theParamMgr->GetNofImageFiles("fRTStruct");
  //--- THERE MUST BE AN INPUT RTSTRUCT FILE
  if( nfst != 1 ) {
    G4Exception(theExeName, 
		"",
		FatalException,
		"No RTStruct file");
  }

}

//---------------------------------------------------------------------------
void DCMCleanOutsideStruct::ReadFilesAndGetImages()
{
  //--- READ AND BUILD IMAGES
  theReaderMgr->ProcessData();
  
  //--- GET STRUCT IMAGE (PRIORITY TO DIM_RTStruct)
  std::vector<DicomVImage*> images = theDicomMgr->GetImages(DIM_RTStruct, false);
  if( images.size() != 0 ) {
    theStructImage = dynamic_cast<DicomVImageStr*>(images[0]);
    G4cout << "DCMCleanOutsideStruct::ReadFilesAndGetImages()  theStructImage  DIM_RTStruct " << *theStructImage << G4endl; //GDEB
      
  }
  /* NOT IMPLEMENTED YET   if( !theStructImage ) {
    images = theDicomMgr->GetImages(DIM_G4dcmCT_StructID);
    if( images.size() != 0 ) {
      theStructImage = dynamic_cast<DicomVImageStr*>(images[0]);
    }
    }*/
  
  //--- GET IMAGE TO ANALYSE. IF OTHER IMAGE THAN G4dcmCT, TAKE IT AND USE G4dcmCT ONLY FOR STRUCT
  bImageG4dcmCT = false;
  images = theDicomMgr->GetImages(DIM_NM,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
  images = theDicomMgr->GetImages(DIM_RTDose,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
  images = theDicomMgr->GetImages(DIM_G4dcmNM,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
  images = theDicomMgr->GetImages(DIM_G4dcmDose,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
  if( !theAnalyseImage ) {
    images = theDicomMgr->GetImages(DIM_G4dcmCT_MateID);
    if( images.size() != 0 ) {
      theAnalyseImage = images[0];
      bImageG4dcmCT = true;
    }
  }

  //  G4cout << "theAnalyseImage " << theAnalyseImage << " " <<bImageG4dcmCT << G4endl; //GDEB
}

//---------------------------------------------------------------------------
void DCMCleanOutsideStruct::ChangeImages()
{
  if( !bImageLimits ) {
    ChangeImagesByStruct();
  } else {
    ChangeImagesByLimits();
  }
}
//---------------------------------------------------------------------------
void DCMCleanOutsideStruct::ChangeImagesByStruct()
{
  //--- LOOP TO VOXELS 
  G4int nVoxels = theAnalyseImage->GetNoVoxels();
  //  G4cout << " DCMCleanOutsideStruct::ChangeImagesByStruct " << nVoxels << G4endl; //GDEB
  // Create Struct image to hold structures for each voxel. Size = theAnalyseImage
  DicomVImageStr* theStructImage = new DicomVImageStr( theAnalyseImage, "RTStruct", DIM_RTStruct );
  // Get polygone set from DicomReaderRTStruct
  std::vector<DicomVReader*> RTPlanReaders = theReaderMgr->GetReaders(DRM_RTPlan,false);
  std::vector<DicomVReader*> structReaders = GetReaders(DRM_RTStruct,false);
  DicomReaderRTStruct* structRead = (DicomReaderRTStruct*)(structReaders[0]);
  DicomPolygonSet* polygonSet = structRead->GetPolygonSet();
  DicomPolygonSet* polygonSetNew = new DicomPolygonSet(polygonSet, imageDicomStructures, polygonSet->GetOrientation() );
  structRead->FindVoxelsInXY( imageDicomStructures, polygonSetNew );
  delete polygonSetNew;
  //    => imageDicomStructures contains struct list for each voxel

  
  for( G4int ii = 0; ii < nVoxels; ii++ ) {
    
    size_t nSTs = stIDList.size();
    //    if( stID != 0 ) G4cout << " PVSID " << ii << " : " << stID << " nSTs= " << nSTs << " : " << log(stID) << G4endl; //GDEB
    if( DicomVerb(debugVerb) && ii %100000 == 1 ) G4cout << " Looping Voxel " << ii << " nSTs " << nSTs << G4endl;

    /*    G4ThreeVector pos = theStructImage->GetPosition(ii);
    G4double posZ = pos.z(); //GDEB
    G4double posY = pos.y(); //GDEB
    if( posZ < -867 && posZ > -871 && posY < -100 && posY > -140 )   G4cout << " StIDLIST " << theStructImage->GetDataStr(ii) << " " << pos << stData.stName << " " << stData.mateName << G4endl; //GDEB 
    */
    }
  }

}

//---------------------------------------------------------------------------
void DCMCleanOutsideStruct::ChangeImagesByLimits()
{
  G4int nVoxels = theAnalyseImage->GetNoVoxels();
  size_t imNVoxelX = theAnalyseImage->GetNoVoxelsX();
  size_t imNVoxelY = theAnalyseImage->GetNoVoxelsY();
  size_t imNVoxelXY = imNVoxelX*imNVoxelY;
  G4double imMinX = theAnalyseImage->GetMinX();
  G4double imMinY = theAnalyseImage->GetMinY();
  G4double imMinZ = theAnalyseImage->GetMinZ();
  G4double imVoxDimX = theAnalyseImage->GetVoxelDimX();
  G4double imVoxDimY = theAnalyseImage->GetVoxelDimY();
  G4double imVoxDimZ = theAnalyseImage->GetVoxelDimZ();
  //  G4cout << " DCMCleanOutsideStruct::ChangeImagesByLimits " << nVoxels << G4endl; //GDEB
  for( G4int ii = 0; ii < nVoxels; ii++ ) {
    G4int ix = ii%imNVoxelX;
    G4int iy = (ii/imNVoxelX)%imNVoxelY;
    G4int iz = ii/imNVoxelXY;
    double voxXmin = imMinX + (ix+1)*imVoxDimX; // if minX line crosses voxel do not take it
    double voxXmax = imMinX + (ix)*imVoxDimX; // if maxX line crosses voxel do not take it
    double voxYmin = imMinY + (iy+1)*imVoxDimY; // if minY line crosses voxel do not take it
    double voxYmax = imMinY + (iy)*imVoxDimY; // if maxY line crosses voxel do not take it
    double voxZmin = imMinZ + (iz+1)*imVoxDimZ; // if minZ line crosses voxel do not take it
    double voxZmax = imMinZ + (iz)*imVoxDimZ; // if maxZ line crosses voxel do not take it
    /*      G4cout << ii << " " << ix << " CUT X " << voxXmin << " > " << theImageMinX << " && " << voxXmax << " < " << theImageMaxX << G4endl; //GDEB
      G4cout << ii << " " << iy << " CUT Y " << voxYmin << " > " << theImageMinY << " && " << voxYmax << " < " << theImageMaxY << G4endl; //GDEB
      G4cout << ii << " " << iz << " CUT Z " << voxZmin << " > " << theImageMinZ << " && " << voxZmax << " < " << theImageMaxZ << G4endl; //GDEB*/
    if( voxXmin > theImageMinX && voxXmax < theImageMaxX 
	&& voxYmin > theImageMinY && voxYmax < theImageMaxY
	&& voxZmin > theImageMinZ && voxZmax < theImageMaxZ ) {
      if( bChangeMaterials ) theAnalyseImage->SetData(ii,theMaterialID);
      if( bChangeDensities ) theMateDensImage->SetData(ii,theDensity);
      if( DicomVerb(debugVerb ) ) {
	G4cout << ii << " " << ix << " CUT X " << voxXmin << " > " << theImageMinX << " && " << voxXmax << " < " << theImageMaxX << G4endl; 
	G4cout << ii << " " << iy << " CUT Y " << voxYmin << " > " << theImageMinY << " && " << voxYmax << " < " << theImageMaxY << G4endl; 
	G4cout << ii << " " << iz << " CUT Z " << voxZmin << " > " << theImageMinZ << " && " << voxZmax << " < " << theImageMaxZ << G4endl; 
      }
    }
    //    G4cout << ii << " MATE ID  " << mateIDs[ii] << " " << mateDens[ii] << G4endl; //GDEB
  }
}

//---------------------------------------------------------------------------
void DCMCleanOutsideStruct::WriteNewImage()
{
  //--- DUMP TO FILE
  G4cout << " @@@ Dumping To Text File: " << theOutFN << G4endl;
  std::ofstream fout(theOutFN.c_str());

  if( bImageG4dcmCT ) {
    theG4dcmCTReader->DumpHeaderToTextFile(fout);
    theAnalyseImage->DumpHeaderToTextFile(fout);
  } else {
    theAnalyseImage->DumpHeaderToTextFile(fout);
  }
  theAnalyseImage->DumpDataToTextFile(fout);

  if( bImageG4dcmCT ) {
    theMateDensImage->DumpDataToTextFile(fout);
  }
  
  if( bImageG4dcmCT && theStructImage ) {
    theStructImage->DumpDataToTextFile(fout);   
    theG4dcmCTReader->DumpStructureList(fout);
  }

}

//---------------------------------------------------------------------------
void DCMCleanOutsideStruct::PrintHelp()
{
  G4cout << " -fDCMIn      G4 DICOM input file " << G4endl
	 << " -st          DICOM structure name " << G4endl
	 << " -mate        new material name " << G4endl
	 << " -density     new material density " << G4endl
	 << " -stOnly      Only change voxels that belong exclusively to the structure or all (1/0). Default = 0" << G4endl
	 << " -stMateDens  DICOM structure name, new material name, new material density, stOnly " << G4endl
	 << " -fChangeList file with list of changes to be made (each line four words) " << G4endl
	 << " -fDCMOut     G4 DICOM ouput file " << G4endl
    	 << " -structureROIShift   Value to build different stID when a voxel belongs to several ROI: stID=Sum(pow(ROIShift,ii)*stID)(for ii=1,N_of_structures) (stID has to be smaller than pow(2,64)  Default is 100 " << G4endl
	 << " -minX        change all voxels with upper X boundary < minX" << G4endl
	 << " -maxX        change all voxels with lower X boundary > maxX" << G4endl
	 << " -minY        change all voxels with upper Y boundary < minY" << G4endl
	 << " -maxY        change all voxels with lower Y boundary > maxY" << G4endl
	 << " -minZ        change all voxels with upper Z boundary < minZ" << G4endl
	 << " -maxZ        change all voxels with lower Z boundary > maxZ" << G4endl
    	 << " -verb        sets the verbosity: test/debug/info/warning/error/silent" << G4endl
	 << " -h           prints the set of arguments " << G4endl
	 << " -help        prints the set of arguments " << G4endl;
}
  
