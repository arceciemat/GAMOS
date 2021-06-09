#ifndef SPECTIOMgr_h
#define SPECTIOMgr_h

/*---------------------------------------------------------------------------   
ClassName:   SPECTIOMgr    
Author:      P. Arce
Changes:     08/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <iostream>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"

struct SPECTOutput
{
  char name[8];
  float xVtx,yVtx,zVtx,x1,y1,z1,x2,y2,z2;
  float energy;
  int eventClass;
};


//------------------------------------------------------------------------
class SPECTIOMgr : public GmIObinMgr
{ 
 private:
  SPECTIOMgr();

 public:
  ~SPECTIOMgr();

  //! Get the only instance 
  static SPECTIOMgr* GetInstance();
  void WriteEvent( const G4ThreeVector& pos1, const G4ThreeVector& pos2, const G4ThreeVector& posVtx, const G4double energy, G4int evtClass, const G4String& name = "SPECT " );  
  void ReadFile();
  SPECTOutput ReadEvent( G4bool& bEof );
  void PrintEvent(const G4String& text, const SPECTOutput& po, G4bool bCart );

private:
  static SPECTIOMgr* theInstance;
  G4bool bDumpCout;
  G4bool bCartesian;
};

#endif
