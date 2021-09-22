#include "DicomJobUtils.hh"

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"

#include "DICOM/DICOMBase/include/DicomDrawerROOT.hh"
//#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include "TStyle.h"

//---------------------------------------------------------------------------
DicomJobUtils::DicomJobUtils()
{
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  //  theReaderMgr = DicomReaderMgr::GetInstance();
  theDicomMgr = DicomMgr::GetInstance();
}

//---------------------------------------------------------------------------
void DicomJobUtils::ProcessArguments(int argc,char** argv)
{
  //----- PROCESS PARAMETERS
  if( argc == 1 ) {
    G4Exception(theExeName.c_str(),
		  "Error",
		FatalErrorInArgument,
		"NO ARGUMENT GIVEN ");
  } else if( argc == 2 ) {
    if( G4String(argv[1]) == "-h" || G4String(argv[1]) == "-help" ) {
       PrintHelp();
       exit(1);
    } else {
      DicomException(theExeName + ": ONLY ONE ARGUMENT: " + argv[1]);
    }
  }

}

//-------------------------------------------------------------------
void DicomJobUtils::PrintHelp()
{
  ((DicomParameterMgr*)(DicomParameterMgr::GetInstance()))->PrintHelpInputFiles();

}


//---------------------------------------------------------------------------
void DicomJobUtils::DicomException(G4String msg, G4ExceptionSeverity sever )
{
  if( sever == FatalException || sever == FatalErrorInArgument ) {
    PrintHelp();
  }
  G4Exception(theExeName.c_str(),
	      "",
	      sever,
	      msg.c_str());
}

