#include "Dicom2G4FileMgr.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include <cfloat>
#include "float.h"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include <fstream>


void PrintHelp();
void CheckNColumns(std::vector<G4String> wl, size_t vsizeTh );

//-----------------------------------------------------------------------------
int main(int argc,char** argv) 
{
  DicomMgr::GetInstance();

  Dicom2G4FileMgr* theFileMgr = Dicom2G4FileMgr::GetInstance();
  DicomParameterMgr* theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  G4String theFileName = "";

  if( argc == 2 ) {
    if( G4String(argv[1]) == "-h" || G4String(argv[1]) == "-help" ) {
       PrintHelp();
     } else {
       theFileName = argv[1];
     }
  } else {
    if(argc%2 != 1) { 
      G4Exception("DICOM2G4",
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    for( G4int ii = 1; ii < argc; ii++ ){
      G4String argvstr = argv[ii];
      if( argvstr == "-f" ) {
	theFileName = argv[ii+1];
	ii++;
      } else if( argvstr == "-inPolygonNCheck" ) {
	DicomMgr::GetInstance()->SetInPolygonNCheck(GmGenUtils::GetInt(argv[ii+1]));
	ii++;
      } else if( argvstr == "-posVoxelCentre" ) {
	DicomMgr::GetInstance()->SetImagePositionPatientIsPosVoxelCentre(GmGenUtils::GetInt(argv[ii+1]));
	ii++;
      } else {
	G4int iAddPar = theParamMgr->ReadParameter( argv, ii );
	if( iAddPar == -1 ) {
	  PrintHelp();
	  G4Exception("DICOM2G4",
		      "Wrong argument",
		      FatalErrorInArgument,
		      (G4String("ARGUMENT: ") + argvstr).c_str());
	}
	ii+=iAddPar;
      }
    }
  }

  theFileMgr->Convert(theFileName);
  
  return 0;
}

//-----------------------------------------------------------------------------
void PrintHelp()
{
  G4cout << " -f          metadata  file name " << G4endl
         << " -inPolygonNCheck   number of directions tried to get if a voxel belongs to a structure " << G4endl
 	 << " -verb       verbosity: sets the verbosity" << G4endl
	 << " -help       prints the set of arguments " << G4endl;
}

//-----------------------------------------------------------------------------
void CheckNColumns(std::vector<G4String> wl, size_t vsizeTh )
{
  if( wl.size() != vsizeTh ) {
    G4cerr << " Reading line " << G4endl;
    for( size_t ii = 0; ii < wl.size(); ii++){
      G4cerr << wl[ii] << " ";
    }
    G4cerr << G4endl;
    G4Exception("DICOM2G4",
		"D2G0010",
		FatalErrorInArgument,
		("Wrong number of columns in line " + std::to_string(wl.size()) + " <> " + std::to_string(vsizeTh)).c_str());
  }

}
  
