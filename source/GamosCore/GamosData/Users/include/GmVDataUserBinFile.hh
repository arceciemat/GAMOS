#ifndef GmVDataUserBinFile_hh
#define GmVDataUserBinFile_hh

#include "GmVDataUser.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"

class GmVDataUserBinFile : public GmVDataUser,
			   public GmIObinMgr
{
public:
  GmVDataUserBinFile();
  ~GmVDataUserBinFile(){};

protected:
  void WriteHeaderBin();

  void WriteHeaderInfo();

  void WriteHeaderNEvents( G4bool bIsFirst = 0 );

  void WriteHeaderNCalls( G4bool bIsFirst = 0 );

  void WriteHeaderData();

  virtual void EndOfRun();
  //  virtual void EndOfEvent();
  
protected:
  G4int theNCallsWritten;
  G4double theNEvents;

  G4int bWriteHeaderData;
  G4int bWriteHeaderCheck;
  G4int bWriteHeaderNEvents;
  G4int bWriteHeaderNCalls;
};

#endif
