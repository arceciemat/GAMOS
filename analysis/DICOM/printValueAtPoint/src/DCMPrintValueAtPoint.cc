#include "DCMPrintValueAtPoint.hh"

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

//---------------------------------------------------------------------------
DCMPrintValueAtPoint::DCMPrintValueAtPoint()
{
  Initialise();
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
  theDicomMgr = DicomMgr::GetInstance();
  theExtension = G4ThreeVector();
  theExtensionN = 0.;
  theUsedExtensionMin = G4ThreeVector();
  theUsedExtensionMax = G4ThreeVector();
}

//---------------------------------------------------------------------------
void DCMPrintValueAtPoint::Initialise()
{
  theExeName = "printValueAtPoint";
}

//---------------------------------------------------------------------------
void DCMPrintValueAtPoint::ProcessArguments(int argc,char** argv)
{
  bPointGiven = false;
  //----- PROCESS PARAMETERS
  if( argc == 1 ) {
    G4Exception(theExeName.c_str(),
		  "Error",
		FatalErrorInArgument,
		"NO ARGUMENT GIVEN ");
  } else {
    if(argc%2 != 1) { 
      G4Exception(theExeName.c_str(),
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    
    for( G4int ii = 1; ii < argc; ii++ ){
      G4String argvstr = argv[ii];
      if( argvstr == "-point" ) {
	thePoint = G4ThreeVector(GmGenUtils::GetValue(argv[ii+1]),GmGenUtils::GetValue(argv[ii+2]),GmGenUtils::GetValue(argv[ii+3]));
	bPointGiven = true;
	ii+=3;
      } else if( argvstr == "-extensionN" ) {
	theExtensionN = GmGenUtils::GetValue(argv[ii+1]);
	ii++;
      } else if( argvstr == "-extension" ) {
	theExtension = G4ThreeVector(GmGenUtils::GetValue(argv[ii+1]),GmGenUtils::GetValue(argv[ii+2]),GmGenUtils::GetValue(argv[ii+3]));
	ii+=3;
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

}

//---------------------------------------------------------------------------
void DCMPrintValueAtPoint::CheckArguments()
{
  //----- CHECK CORRECT ARGUMENTS
  //--- THERE MUST BE AN OUTPUT FILE
  if( !bPointGiven ) {
    PrintHelp();
    G4Exception(theExeName.c_str(),
		"",
		FatalErrorInArgument,
		"NO VALUE GIVEN FOR point");
  }

  //--- THERE CAN BE ONE AND ONLY ONE IMAGE FILE
  G4int nFiles = theParamMgr->GetNofImageFiles();
  if( nFiles != 1 ) {
    G4Exception("DCMPrintValueAtPoint",
		"",
		FatalException,
		"NUMBER OF IMAGE FILES CAN ONLY BE 1");
  }
}

//---------------------------------------------------------------------------
void DCMPrintValueAtPoint::ReadFilesAndGetImages()
{
  //--- READ AND BUILD IMAGES
  theReaderMgr->ProcessData();
 
}
 
//--------------------------------------------------------------------------
void DCMPrintValueAtPoint::PrintInfoFromImage()
{
  std::vector<DicomVImage*> images = theReaderMgr->GetImages();
  //--- DicomReaderMgr::GetImages does not return Sqdose_err image
  /*  for( std::vector<DicomVImage*>::iterator itevi = images.begin(); itevi != images.end(); itevi++ ) {
    if( (*itevi)->GetModality() == DIM_Sqdose_sq
	|| (*itevi)->GetModality() == DIM_Sqdose_err
	|| (*itevi)->GetModality() == DIM_Sqdose_errRel ) {
	images.erase(itevi);
	break;
	}
	}*/
  
  
  DicomVImage* image;
  DicomVImage* imageErr = 0;
  for( size_t ii = 0; ii < images.size(); ii++ ) {
    image = images[ii];
    if( image->GetModality() == DIM_Sqdose ) {
      std::vector<DicomVImage*> imageErrs = theDicomMgr->GetImages(DIM_Sqdose_err);
      imageErr = imageErrs[0]; // checked that there can be only 1
    }      
    //--- GET GEOMETRY DATA
    size_t nVoxX = image->GetNoVoxelsX();
    size_t nVoxY = image->GetNoVoxelsY();
    //  size_t nVoxZ = image->GetNoVoxelsZ();
    size_t nVoxXY = nVoxX*nVoxY;
    float fWidthX = image->GetVoxelDimX();
    float fWidthY = image->GetVoxelDimY();
    float fWidthZ = image->GetVoxelDimZ();
    G4double fMinX = image->GetMinX();
    G4double fMinY = image->GetMinY();
    G4double fMinZ = image->GetMinZ();
    G4double fMaxX = image->GetMaxX();
    G4double fMaxY = image->GetMaxY();
    G4double fMaxZ = image->GetMaxZ();
    // check point inside limits
    theExtension += G4ThreeVector(fWidthX*theExtensionN,fWidthY*theExtensionN,fWidthZ*theExtensionN);
    if( thePoint.x()-theExtension.x() < fMinX || thePoint.y()-theExtension.y() < fMinY || thePoint.z()-theExtension.z() < fMinZ ||
	thePoint.x()+theExtension.x() > fMaxX || thePoint.y()+theExtension.y() > fMaxY || thePoint.z()+theExtension.z() > fMaxZ ) {
      DicomException("Point out of image limits");
    }
    G4int nCVoxXMin = int((thePoint.x()-theExtension.x()-fMinX)/fWidthX);
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(testVerb) ) G4cout << " nCVoxXMin " << nCVoxXMin << " " << (thePoint.x()-theExtension.x()-fMinX)/fWidthX << " " << thePoint.x() << " - " << theExtension.x() << " - " << fMinX << " )/ " << fWidthX << G4endl; //GDEB
#endif
    G4int nCVoxYMin = int((thePoint.y()-theExtension.y()-fMinY)/fWidthY);
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(testVerb) ) G4cout << " nCVoxYMin " << nCVoxYMin << " " << (thePoint.y()-theExtension.y()-fMinY)/fWidthY << " " << thePoint.y() << " - " << theExtension.y() << " - " << fMinY << " )/ " << fWidthY << G4endl; //GDEB
#endif
    G4int nCVoxZMin = int((thePoint.z()-theExtension.z()-fMinZ)/fWidthZ);
    G4int nCVoxXMax = int((thePoint.x()+theExtension.x()-fMinX)/fWidthX);
    G4int nCVoxYMax = int((thePoint.y()+theExtension.y()-fMinY)/fWidthY);
    G4int nCVoxZMax = int((thePoint.z()+theExtension.z()-fMinZ)/fWidthZ);
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(testVerb) ) G4cout << " nCVoxZMin " << nCVoxZMin << " " << (thePoint.z()-theExtension.z()-fMinZ)/fWidthZ << " " << thePoint.z() << " - " << theExtension.z() << " - " << fMinZ << " )/ " << fWidthZ << G4endl; //GDEB
#endif
    theUsedExtensionMin = G4ThreeVector(fMinX+nCVoxXMin*fWidthX,fMinY+nCVoxYMin*fWidthY,fMinZ+nCVoxZMin*fWidthZ);
    theUsedExtensionMax = G4ThreeVector(fMinX+nCVoxXMax*fWidthX,fMinY+nCVoxYMax*fWidthY,fMinZ+nCVoxZMax*fWidthZ);

    G4int nofCVox = (nCVoxXMax-nCVoxXMin+1)*(nCVoxYMax-nCVoxYMin+1)*(nCVoxZMax-nCVoxZMin+1);
    G4double sumData = 0;
    G4double sumDataErr2 = 0;
    for( G4int ix = nCVoxXMin; ix <= nCVoxXMax; ix++ ) {
      for( G4int iy = nCVoxYMin; iy <= nCVoxYMax; iy++ ) {
	for( G4int iz = nCVoxZMin; iz <= nCVoxZMax; iz++ ) {
	  G4int copyNo = ix + iy*nVoxX + iz*nVoxXY;
	  sumData += image->GetData(copyNo);
	  //	  G4cout <<iz*nVoxXY+iy*nVoxX+ix<< " printValueAtPoint copyNo " << copyNo << " = " << ix << ":" << iy << ":" << iz << " " << image->GetData(copyNo) << " : " << sumData << " / " << nofCVox << G4endl; //GDEB
	  if( imageErr ) {
	    sumDataErr2 += imageErr->GetData(copyNo)*imageErr->GetData(copyNo);
	  }
	}
      }
    }
    sumData /= nofCVox;
    if( imageErr ) sumDataErr2 = sqrt(sumDataErr2);
    sumDataErr2 /= nofCVox;
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(silentVerb) ) G4cout << " " << image->GetName() << " DCMPrintValueAtPoint POINT_VALUE " << sumData << " +- " << sumDataErr2 << " VOLUME_AVERAGED FROM " << theUsedExtensionMin << " TO " << theUsedExtensionMax << G4endl;
#endif
  }
}


//-------------------------------------------------------------------
void DCMPrintValueAtPoint::PrintHelp()
{
  ((DicomParameterMgr*)(DicomParameterMgr::GetInstance()))->PrintHelpInputFiles();
  G4cout << " -fOut        Output file " << G4endl
	 << " -verb        sets the verbosity: test/debug/info/warning/error/silent" << G4endl
	 << " -h           prints the set of arguments " << G4endl
	 << " -help        prints the set of arguments " << G4endl;
}


//---------------------------------------------------------------------------
void DCMPrintValueAtPoint::DicomException(G4String msg, G4ExceptionSeverity sever )
{
  if( sever == FatalException || sever == FatalErrorInArgument ) {
    PrintHelp();
  }
  G4Exception(theExeName.c_str(),
	      "",
	      sever,
	      msg.c_str());
}

  
