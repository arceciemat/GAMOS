#ifndef CCIOMgr_h
#define CCIOMgr_h

/*---------------------------------------------------------------------------   
ClassName:   CCIOMgr    
Author:      P. Arce
Changes:     08/07: creation 
	     02/11: Edited by L. Harkness for Compton output
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <iostream>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmIOMgr.hh"

struct CCOutput
{
char name[9];
float x1,y1,z1,e1,x2,y2,z2,e2;
};

//------------------------------------------------------------------------
class CCIOMgr: public GmIObinMgr
{ 
 private:
  CCIOMgr();

public:
  ~CCIOMgr();
 static CCIOMgr* GetInstance();
void WriteEvent( const G4ThreeVector& pos1, const G4double& energy1, const G4ThreeVector& pos2, const G4double& energy2, const G4String& name = "COMPCAM " );  
  void ReadFile();
  CCOutput ReadEvent( G4bool& bEof );
  void PrintEvent(const G4String& text, const CCOutput& po, G4bool bCart );

 
private:

GmIOMgr* theCCIOMgr;
static CCIOMgr* theInstance;
G4bool bBinFile; 
G4bool bDumpCout;
G4bool bCartesian;

};

#endif
