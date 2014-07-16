#ifndef GmIOMgr_h
#define GmIOMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmIOMgr    
Author:      P. Arce
Changes:     09/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Base class for class to manage reading and writing  */ 

#include <iostream>
#include "globals.hh"
#include "G4ThreeVector.hh"

//------------------------------------------------------------------------
class GmIOMgr 
{ 
 public:
  GmIOMgr();
  ~GmIOMgr();

  virtual void Init() = 0;
  void InitDebugging();

  virtual void CloseFileIn() = 0;
  virtual void CloseFileOut() = 0;

  virtual void OpenFileIn( const G4String& fname = "" ) = 0;
  virtual void OpenFileOut( const G4String& fname = "" ) = 0;

  void SetFileNameIn( const G4String& fname );
  void SetFileNameOut( const G4String& fname );

protected:
  void PrintCart(const G4String& text, const G4ThreeVector& pos );
  void PrintCyl(const G4String& text, const G4ThreeVector& pos );
  void PrintSphe(const G4String& text, const G4ThreeVector& pos );

protected:
  G4String theOutputType;

  G4String theFileNameIn;
  G4String theFileNameOut;

  G4bool bDebugIn;
  G4bool bDebugOut;
};

#endif
