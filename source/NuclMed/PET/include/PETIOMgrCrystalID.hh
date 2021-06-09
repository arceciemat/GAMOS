#ifndef PETIOMgrCrystalID_h
#define PETIOMgrCrystalID_h
class GmRecHit;
class GmHitsEventMgr;
#include "NuclMed/Base/include/DetRecHitSet.hh"

/*---------------------------------------------------------------------------   
ClassName:   PETIOMgrCrystalID    
Author:      P. Arce
Changes:     08/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <iostream>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"
class G4Navigator;
class G4TouchableHistory;

struct PETOutput
{
  char name[8];
  float xVtx,yVtx,zVtx,EVtx,tVtx,x1,y1,z1,E1,t1,x2,y2,z2,E2,t2;
};


//------------------------------------------------------------------------
class PETIOMgrCrystalID : public GmIObinMgr
{ 
 private:
  PETIOMgrCrystalID();

 public:
  ~PETIOMgrCrystalID();

  //! Get the only instance 
  static PETIOMgrCrystalID* GetInstance();
  void WriteEvent( const G4ThreeVector& posVtx, G4double EVtx, G4double tVtx, const DetRecHitSet& rh1, const DetRecHitSet& rh2, const G4String& name = "PET " ); 
  void ReadFile();
  PETOutput ReadEvent( G4bool& bEof );
  void PrintEvent(const G4String& text, const PETOutput& po, G4bool bCart );

private:
  char binary_char(unsigned int num, int iset );
  unsigned int GetTouchableID( G4TouchableHistory* touch);

  static PETIOMgrCrystalID* theInstance;
  G4bool bDumpCout;
  G4bool bCartesian;
  G4Navigator* theNavigator;
  G4TouchableHistory* theTouchable;
  std::vector<unsigned int> theShifts;

  GmHitsEventMgr* theHitsEventMgr;
};

#endif
