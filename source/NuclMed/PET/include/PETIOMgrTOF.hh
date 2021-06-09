#ifndef PETIOMgrTOF_h
#define PETIOMgrTOF_h
class GmRecHit;
#include "NuclMed/Base/include/DetRecHitSet.hh"

/*---------------------------------------------------------------------------   
ClassName:   PETIOMgrTOF    
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
  float xVtx,yVtx,zVtx,EVtx,tVtx,x1,y1,z1,E1,t1,x2,y2,z2,E2,t2;
};


//------------------------------------------------------------------------
class PETIOMgrTOF : public GmIObinMgr
{ 
 private:
  PETIOMgrTOF();

 public:
  ~PETIOMgrTOF();

  //! Get the only instance 
  static PETIOMgrTOF* GetInstance();
  void WriteEvent( const G4ThreeVector& posVtx, G4double EVtx, G4double tVtx, const DetRecHitSet& rh1, const DetRecHitSet& rh2, const G4String& name = "PET " ); 
  void ReadFile();
  PETOutput ReadEvent( G4bool& bEof );
  void PrintEvent(const G4String& text, const PETOutput& po, G4bool bCart );

private:
  static PETIOMgrTOF* theInstance;
  G4bool bDumpCout;
  G4bool bCartesian;
};

#endif
