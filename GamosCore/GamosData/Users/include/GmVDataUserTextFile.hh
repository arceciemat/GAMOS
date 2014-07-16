#ifndef GmVDataUserTextFile_hh
#define GmVDataUserTextFile_hh

#include "GmVDataUser.hh"
#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"


class GmVDataUserTextFile : public GmVDataUser, 
			    public GmIOtextMgr
{
public:
  GmVDataUserTextFile();
  ~GmVDataUserTextFile(){};

protected:
  void WriteHeaderText();
  
protected:
  G4int bWriteHeaderData;
};

#endif
