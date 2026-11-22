#ifndef GmIOtextMgr_h
#define GmIOtextMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmIOtextMgr    
Author:      P. Arce
---------------------------------------------------------------------------*/ 
/*! Base class for class to manage reading and writing of text files */ 

#include "GmIOMgr.hh"
#include <fstream>
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//------------------------------------------------------------------------
class GmIOtextMgr : public GmIOMgr
{ 
 public:
  GmIOtextMgr();

  ~GmIOtextMgr();

  virtual void Init();

  virtual void CloseFileIn();
  virtual void CloseFileOut();

  virtual void OpenFileIn( const G4String& fname = "" );
  virtual void OpenFileOut( const G4String& fname = "" );

  GmFileIn * GetFileIn() const { return theFileIn; }
  std::ofstream * GetFileOut() const { return theFileOut; }

protected:
  GmFileIn *theFileIn;
  std::ofstream *theFileOut;
};

#endif
