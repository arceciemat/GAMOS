#ifndef PETIOMgr_h
#define PETIOMgr_h

/*---------------------------------------------------------------------------   
ClassName:   PETIOMgr    
Author:      P. Arce
Changes:     08/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <iostream>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"

struct PETOutput
{
  char name[8];
  float xVtx,yVtx,zVtx,x1,y1,z1,x2,y2,z2;
};


//------------------------------------------------------------------------
class PETIOMgr : public GmIObinMgr
{ 
 private:
  PETIOMgr();

 public:
  ~PETIOMgr();

  //! Get the only instance 
  static PETIOMgr* GetInstance();
  void WriteEvent( const G4ThreeVector& pos1, const G4ThreeVector& pos2, const G4ThreeVector& posVtx, const G4String& name = "PET " );  
  void ReadFile();
  PETOutput ReadEvent( G4bool& bEof );
  void PrintEvent(const G4String& text, const PETOutput& po, G4bool bCart );

private:
  static PETIOMgr* theInstance;
  G4bool bDumpCout;
  G4bool bCartesian;
};

#endif
