#ifndef GmReadPhantomSVMgr_hh
#define GmReadPhantomSVMgr_hh

#include <map>
#include "globals.hh"

class GmReadPhantomSVMgr 
{
  
  GmReadPhantomSVMgr(){};
  
public:
  ~GmReadPhantomSVMgr(){};
  
  static GmReadPhantomSVMgr* GetInstance();
  
  void ReadPVG4( std::ifstream& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ );
  void ReadPVEGS( std::ifstream& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ );

  G4int GetPVID( size_t id );

  G4int GetPVIDFromPVName( G4String pvName );

private:
  static GmReadPhantomSVMgr* theInstance;

  size_t* thePVIDs;
  std::map<G4int,G4String> thePVs;

};

#endif
