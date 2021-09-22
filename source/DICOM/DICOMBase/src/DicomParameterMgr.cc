#include "DicomParameterMgr.hh"
#include "DicomMgr.hh"
#include "DicomOperResize.hh"
#include "DicomOperMultByNumber.hh"
#include "DicomOperLog.hh"
#include "DicomOperLog10.hh"
#include "DicomOperLogN.hh"
#include "DicomOperPrintMax.hh"
#include "DicomOperPrintValueAtPoint.hh"
#include "DicomOperRotateXY180.hh"

#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"

//----------------------------------------------------------------------
//DicomParameterMgr* DicomParameterMgr::theInstance = 0; //use the on from GmparameterMgr
//----------------------------------------------------------------------
/*DicomParameterMgr* DicomParameterMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new DicomParameterMgr;
  }

  return theInstance;
  
  }
*/
//----------------------------------------------------------------------
DicomParameterMgr::DicomParameterMgr()
{
}

//----------------------------------------------------------------------
G4int DicomParameterMgr::ReadParameter(char** argv, size_t iPar)
{
  G4int iAddPar = 0;

  G4bool bResize = false;
  
  G4String argvstr = argv[iPar];
  G4String argvstr1 = argv[iPar+1];
  G4String argvName = argvstr.substr(1,999);
  if( argvstr == "-verb" ) {
    G4String verbstr = G4String(argvstr1);
    if(verbstr == "test" ) {
      GmVVerbosity::SetVerbosityLevel( "DicomVerbosity", testVerb );
      GmVVerbosity::SetVerbosityLevel( "GmReadDICOMVerbosity", testVerb );
      DicomVerb.SetFilterLevel(testVerb);
    } else if(verbstr == "debug" ) {
      GmVVerbosity::SetVerbosityLevel( "DicomVerbosity", debugVerb );
      GmVVerbosity::SetVerbosityLevel( "GmReadDICOMVerbosity", debugVerb );
    } else if(verbstr == "info" ) {
      GmVVerbosity::SetVerbosityLevel( "DicomVerbosity", infoVerb );
      GmVVerbosity::SetVerbosityLevel( "GmReadDICOMVerbosity", infoVerb );
    } else if(verbstr == "warning" ) {
      GmVVerbosity::SetVerbosityLevel( "DicomVerbosity", warningVerb );
      GmVVerbosity::SetVerbosityLevel( "GmReadDICOMVerbosity", warningVerb );
    } else if(verbstr == "error" ) {
      GmVVerbosity::SetVerbosityLevel( "DicomVerbosity", errorVerb );
      GmVVerbosity::SetVerbosityLevel( "GmReadDICOMVerbosity", errorVerb );
    } else if(verbstr == "silent" ) {
      GmVVerbosity::SetVerbosityLevel( "DicomVerbosity", silentVerb );
      GmVVerbosity::SetVerbosityLevel( "GmReadDICOMVerbosity", silentVerb );
    } else {
      G4cerr << " VERBOSITY " << argvstr1  << G4endl;
      G4Exception("DicomParameterMgr",
		  "Error",
		  FatalErrorInArgument,
		  "verbosity wrong");
    }

  } else if( argvstr == "-minX" ||
	     argvstr == "-maxX" ||
	     argvstr == "-minY" ||
	     argvstr == "-maxY" ||
	     argvstr == "-minZ" ||
	     argvstr == "-maxZ" ||
	     argvstr == "-nVoxX" ||
	     argvstr == "-nVoxY" ||
	     argvstr == "-nVoxZ" ||
    	     argvstr == "-resizeFitOld" ) {
    //-	     argvstr == "-closestPolygon"
    AddParam( argvName + " " + argvstr1, PTdouble );
    bResize = true;
  } else if( argvstr == "-fG4dcmCT" ||
	     argvstr == "-fG4dcmNM" || 
	     argvstr == "-fG4dcmDose" ||
	     argvstr == "-fSqdose" ||
	     argvstr == "-fCT" ||
	     argvstr == "-fNM" ||
	     argvstr == "-fRTDose" ||
	     argvstr == "-fInterfile" ||
	     argvstr == "-fTextLines" ) {
    //    if( IsParameterInScript(argvName) { 
    std::vector<G4String> fileNames;
    fileNames = GetVStringValue(argvName,fileNames);
    G4String argList;
    for( size_t ii = 0; ii < fileNames.size(); ii++ ) {
      argList += fileNames[ii] + " ";
    }
    G4String addPstr = argvName + " " + argList + " " + argvstr1;
    //    AddParam( argvName+ " " + argList + " " + argvstr1, PTVstring );
    DicomParameterMgr::AddParam( addPstr,PTVstring );
    //    std::vector<G4String> files;
    //    GetVStringValue("fSqdose",files);
    //-    G4cout << " N SQdose " << files.size();
    //-    PrintParametersUsage(99);
  } else if( argvstr == "-fOut" ||
	     argvstr == "-fRTStruct" ||
	     argvstr == "-fDrawLines" ) {    
    AddParam( argvName+ " " + argvstr1, PTstring );
  } else if( argvstr == "-multByNumber" ) {
    AddParam( argvName+ " " + argvstr1, PTdouble );
    new DicomOperMultByNumber(GmGenUtils::GetValue(argvstr1),1,"DicomOperMultByNumber");
  } else if( argvstr == "-log" ) {
    AddParam( argvName+ " " + argvstr1, PTdouble );
    if( argvstr1 == "1" ) new DicomOperLog(1,"DicomOperLog");
  } else if( argvstr == "-log10" ) {
    AddParam( argvName+ " " + argvstr1, PTdouble );
    if( argvstr1 == "1" ) new DicomOperLog10(1,"DicomOperLog10");
  } else if( argvstr == "-logN" ) {
    AddParam( argvName+ " " + argvstr1, PTdouble );
    new DicomOperLogN(GmGenUtils::GetValue(argvstr1),1,"DicomOperLogN");
  } else if( argvstr == "-rotateXY" ) {
    AddParam( argvName+ " " + argvstr1, PTdouble );
    new DicomOperRotateXY180(GmGenUtils::GetValue(argvstr1),1,"DicomOperRotateXY180");
    DicomMgr::GetInstance()->SetRotateXY180(GmGenUtils::GetValue(argvstr1));
    //    G4cout << " SetRotateXY180 " << GmGenUtils::GetValue(argvstr1) << G4endl; //GDEB
  } else if( argvstr == "-printMax" ) {
    AddParam( argvName+ " " + argvstr1, PTdouble );
    if( argvstr1 == "1" ) {
      new DicomOperPrintMax(2,"DicomOperPrintMax");
    }
  } else if( argvstr == "-printValueAtPoint" ) {
    /*    if( argc < iPar+3 ){
      G4Exception("DicomParameterMgr",
		  "",
		  FatalErrorInArgument,
		  "You need to put three numbers after -printValueAtPoint");
		  } */
    AddParam( argvName+ " " + argvstr1 + " " + argv[iPar+2] + " " + argv[iPar+3], PTVdouble );    
    G4ThreeVector pt( GmGenUtils::GetValue(argv[iPar+1]), GmGenUtils::GetValue(argv[iPar+2]), GmGenUtils::GetValue(argv[iPar+3]));
    new DicomOperPrintValueAtPoint( pt, 3, "DicomOperPrintValueAtPoint");
    iAddPar = 3;
  } else if( argvstr == "-imageFormat" ) {
    AddParam( argvName+ " " + argvstr1, PTstring );
    
  } else if( argvstr == "-palette"
	     || argvstr == "-paletteMin"
	     || argvstr == "-paletteMax"
	     || argvstr == "-nPixelsX" 
	     || argvstr == "-nPixelsY" 
	     || argvstr == "-nPixelsZ" ) {
    AddParam( argvName+ " " + argvstr1, PTdouble );
  } else if( argvstr == "-nPixels" ) {
    AddParam( "nPixelsX " + argvstr1, PTdouble );
    AddParam( "nPixelsY " + argvstr1, PTdouble );
    AddParam( "nPixelsZ " + argvstr1, PTdouble ); 
  } else {
    iAddPar = -1;
  }
  if( iAddPar == 0 ) iAddPar = 1;

  if( bResize ) {
    if( DicomMgr::GetInstance()->FindOperator("DicomOperResize") == 0 ) new DicomOperResize(0,"DicomOperResize");
  }
 
  return iAddPar;
}

//-------------------------------------------------------------
void DicomParameterMgr::CheckNofImageFiles(G4String fileType, G4String exeName, size_t nFiles)
{
  size_t nf = GetNofImageFiles(fileType);
  if( nf != nFiles ){
      PrintHelpInputFiles();
      G4Exception(exeName.c_str(),
		  "",
		  FatalException,
		  ("Number of input files of type " + fileType + " is " + GmGenUtils::itoa(nf) + ", while it should be " + GmGenUtils::itoa(nFiles)).c_str());    
  }
}
  
//--------------------------------------------------------------
size_t DicomParameterMgr::GetNofImageFiles(G4String fileType)
{
  size_t nFiles = 0;  
  std::vector<G4String> files;
  if( fileType == "ANY" ) {
    files = GetVStringValue("fG4dcmCT",files);
    nFiles += files.size();
    //    G4cout << " nFiles fG4dcmCT " << nFiles << G4endl; //GDEB
    files.clear();
    
    files = GetVStringValue("fG4dcmNM",files);
    nFiles += files.size();
    files.clear();
    
    files = GetVStringValue("fG4dcmDose",files);
    nFiles += files.size();
    files.clear();
    
    files = GetVStringValue("fSqdose",files);
    nFiles += files.size();
    //    G4cout << " nFiles fSqdose " << nFiles << G4endl; //GDEB
    files.clear();
    
    files = GetVStringValue("fRTDose",files);
    nFiles += files.size();
    files.clear();
    
    files = GetVStringValue("fNM",files);
    nFiles += files.size();
    files.clear();

    files = GetVStringValue("fCT",files);
    nFiles += files.size();
    files.clear();

    files = GetVStringValue("fInterfile",files);
    nFiles += files.size();
    files.clear();

  } else {
    files = GetVStringValue(fileType,files);
    nFiles += files.size();
    files.clear();
  }
  //  PrintParametersUsage(5);

  //  G4cout << " RETURN nFiles " << nFiles << G4endl; //GDEB
  return nFiles;
}

//--------------------------------
void DicomParameterMgr::PrintHelpInputFiles()
{
  G4cout
    << " -fG4dcmCT FILE_NAME  CT image file in G4 DICOM format" << G4endl
    << " -fG4dcmNM FILE_NAME  PET/SPECT image file in Geant4 g4dcm format" << G4endl
    << " -fCT      FILE_NAME  CT image file in DICOM format" << G4endl
    << " -fNM      FILE_NAME  PET/SPECT image file in PT/ST DICOM format" << G4endl
    << " -fG4dcmDose FILE_NAME  dose image file in Geant4 g4dcm format" << G4endl
    << " -fSqdose FILE_NAME   dose image file in GAMOS Sqdose format " << G4endl
    << " -fRTDose FILE_NAME   dose image file in DICOM RTDOSE format" << G4endl
    << " -fInterfile FILE_NAME   image file in Interfile 3.3 format " << G4endl
    << " -fRTStruct FILE_NAME Structures image file in DICOM RTSTRUCT format" << G4endl
    << " -minY VAL     minimum image Y value" << G4endl
    << " -maxY VAL     maximum image Y value" << G4endl
    << " -nVoxY VAL    number of image voxels in Y" << G4endl
    << " -log 1        figure values are converted by logarithm " << G4endl
    << " -log10 1      figure values are converted by logarithm base 10" << G4endl
    << " -logN NUNBER  figure values are converted by logarithm base N" << G4endl
    << " -multByNumber VALUE  figure values are multiplied by a number " << G4endl
    << " -verb VERB_NAME  sets the verbosity: test/debug/info/warning/error/silent" << G4endl
    << " -h           prints the set of arguments " << G4endl
    << " -help        prints the set of arguments " << G4endl;
  
}
