#ifndef DicomJobUtils_hh
#define DicomJobUtils_hh

class DicomMgr;
//class DicomReaderMgr;
class DicomParameterMgr;
class DicomVDrawer;

#include "globals.hh"
#include <vector>
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DicomJobUtils
{
public:
  DicomJobUtils();
  ~DicomJobUtils(){};
  
  void ProcessArguments(int argc,char** argv);

  void CheckArguments() {};

  void ReadFilesAndGetImages() {};

  void PrintHelp();
  
protected:
  void DicomException(G4String msg, G4ExceptionSeverity sever = FatalException);

protected:
  G4String theExeName;

  DicomParameterMgr* theParamMgr;
  DicomMgr* theDicomMgr;
  //  DicomReaderMgr* theReaderMgr;
};

#endif
