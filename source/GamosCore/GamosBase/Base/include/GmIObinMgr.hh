#ifndef GmIObinMgr_h
#define GmIObinMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmIObinMgr    
Author:      P. Arce
---------------------------------------------------------------------------*/ 
/*! Base class for class to manage reading and writing of binary files */ 

#include "GmIOMgr.hh"
#include <fstream>

//------------------------------------------------------------------------
class GmIObinMgr : public GmIOMgr
{ 
 public:
  GmIObinMgr();

  ~GmIObinMgr();

  virtual void Init();

  virtual void CloseFileIn();
  virtual void CloseFileOut();

  virtual void OpenFileIn( const G4String& fname = "" );
  virtual void OpenFileOut( const G4String& fname = "" );
  virtual void OpenFileOutBin( const G4String& fname = "" );

  FILE * GetFileIn() const { return theFileIn; }
  FILE * GetFileOut() const { return theFileOut; }

  void WriteHeaderCheck(std::ofstream* fOut);

protected:
  FILE *theFileIn;
  FILE *theFileOut;
  std::ofstream *theFileOutBin;
};

#endif
