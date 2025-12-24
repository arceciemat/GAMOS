#include "DCMGetImageValue.hh"

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
DCMGetImageValue::DCMGetImageValue()
{
  Initialise();
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
  theDicomMgr = DicomMgr::GetInstance();
  theImageMinX = -DBL_MAX;
  theImageMaxX = DBL_MAX;
  theImageMinY = -DBL_MAX;
  theImageMaxY = DBL_MAX;
  theImageMinZ = -DBL_MAX;
  theImageMaxZ = DBL_MAX;
  theValue = DBL_MAX;
  thePrecision = -999;

}

//---------------------------------------------------------------------------
void DCMGetImageValue::Initialise()
{
  theExeName = "cleanOutsideStruct";
  theAnalyseImage = 0;
}
 
//---------------------------------------------------------------------------
void DCMGetImageValue::ProcessArguments(int argc,char** argv)
{
  if(argc%2 != 1) {
    G4Exception(theExeName.c_str(),
		"Error",
		FatalErrorInArgument,
		"WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
  }
  for( G4int ii = 1; ii < argc; ii++ ){
    G4String argvstr = argv[ii];
    if( argvstr == "-val" ) {
      theValue = GmGenUtils::GetValue(argv[ii+1]);
      ii++;
    } else if( argvstr == "-prec" ) {
      thePrecision = GmGenUtils::GetValue(argv[ii+1]);
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

  if( thePrecision == -999 ) {
    thePrecision = 0.001*theValue;
  }
  if( theValue == DBL_MAX ) {
    G4Exception(theExeName, 
		"",
		FatalException,
		"NO value argument, use '-val'");
  }

  
}

//---------------------------------------------------------------------------
void DCMGetImageValue::CheckArguments()
{
  size_t nfim = theParamMgr->GetNofImageFiles("ANY");
  //--- THERE MUST BE AN INPUT IMAGE FILE
  if( nfim != 1 ) {
    G4Exception(theExeName, 
		"",
		FatalException,
		"NO image files");
  }
}

//---------------------------------------------------------------------------
void DCMGetImageValue::ReadFilesAndGetImages()
{
  //--- READ AND BUILD IMAGES
  theReaderMgr = DicomReaderMgr::GetInstance();
  //  theReaderMgr->ProcessData();
  theReaderMgr->CreateReaders();
  theReaderMgr->SetCTOnlyHU(true);
  theReaderMgr->CreateImages();
  
  //--- GET IMAGE TO ANALYSE. IF OTHER IMAGE THAN G4dcmCT, TAKE IT AND USE G4dcmCT ONLY FOR STRUCT
  std::vector<DicomVImage*> images = theDicomMgr->GetImages(DIM_NM,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
  images = theDicomMgr->GetImages(DIM_RTDose,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
  images = theDicomMgr->GetImages(DIM_Sqdose,false);
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
  images = theDicomMgr->GetImages(DIM_Interfile,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
}

//---------------------------------------------------------------------------
void DCMGetImageValue::GetClosestValue()
{ 
  //--- LOOP TO VOXELS 
  G4int nVoxels = theAnalyseImage->GetNoVoxels();
  //  G4cout << " DCMGetImageValue::ChangeImagesByStruct " << nVoxels << G4endl; //GDEB
  // Create Struct image to hold structures for each voxel. Size = theAnalyseImage

  G4double minDiff = DBL_MAX;
  G4double theClosestValue = DBL_MAX;
  G4ThreeVector theClosestPos;
  std::multimap<G4double,G4int> theVoxelIDs;
  std::vector<G4double> theVoxelValues;
  std::vector<G4ThreeVector> theVoxelPos;
  for( G4int ii = 0; ii < nVoxels; ii++ ) {
    G4double val = theAnalyseImage->GetData(ii);
    if( fabs(theValue-val) < thePrecision ) {
      G4int id=theVoxelValues.size();
      theVoxelValues.push_back(val);
      theVoxelPos.push_back(theAnalyseImage->GetPosition(ii));
      theVoxelIDs.insert( std::multimap<G4double,G4int>::value_type(fabs(theValue-val),id));
    }
    //    G4cout << ii << " val " << val << " dif " << fabs(theValue-val) <<" " << minDiff  << G4endl; //GDEB
    if( fabs(theValue-val) < minDiff ) {
      G4cout << " CLOSEST " << val << " at " << theAnalyseImage->GetPosition(ii) << G4endl; //GDEB
      minDiff = fabs(theValue-val);
      theClosestValue = val;
      theClosestPos = theAnalyseImage->GetPosition(ii);
    }
  }
  
  for(std::multimap<G4double,G4int>::const_iterator ite = theVoxelIDs.begin(); ite != theVoxelIDs.end(); ite++ ) {
    G4int id = ite->second;
    G4cout << "@#@# Value found " << theVoxelValues[id] << " at " << theVoxelPos[id] << "   diff= " << ite->first;
  }

  if( theVoxelIDs.size() == 0 ) {
    G4cout << "!! NO voxel found with value " << theValue << " +- " << thePrecision << G4endl;
    G4cout << "@#@# Closest value " << theClosestValue << " at " << theClosestPos<< G4endl;
  }

}

//---------------------------------------------------------------------------
void DCMGetImageValue::PrintHelp()
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
  
