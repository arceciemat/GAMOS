#ifndef DicomParameterMgr_hh
#define DicomParameterMgr_hh

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------------
class DicomParameterMgr : public GmParameterMgr
{
  /*public:
  static DicomParameterMgr* theInstance = 0; 
  static DicomParameterMgr* GetInstance();
  */
  friend class GmParameterMgr;
protected:
  DicomParameterMgr();

public:
  ~DicomParameterMgr(){};

  //! Get the only instance 
  //  static DicomParameterMgr* GetInstance();

  G4int ReadParameter(char** argv, size_t iPar);
  void CheckNofImageFiles(G4String fileType, G4String exeName, size_t nFiles);
  size_t GetNofImageFiles(G4String fileType = "ANY");

  void PrintHelpInputFiles();

};

#endif
