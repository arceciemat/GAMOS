#ifndef GmReadPhantomPSMgr_hh
#define GmReadPhantomPSMgr_hh

#include <map>
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class GmReadPhantomPSMgr 
{
  
  GmReadPhantomPSMgr(){};
  
public:
  ~GmReadPhantomPSMgr(){};
  
  static GmReadPhantomPSMgr* GetInstance();
  
  void ReadPSG4( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ );
  void ReadPSEGS( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ );

  G4int GetPVSID( size_t id );
  size_t* GetPVSIDs() const {
   return thePVSIDs;
  }
  std::map<G4int,G4String> GetPVSs() const {
    return thePVSs;
  }
  
  G4int GetPVSIDFromPVSName( G4String pvName );

private:
  static GmReadPhantomPSMgr* theInstance;

  size_t* thePVSIDs;
  std::map<G4int,G4String> thePVSs;

};

#endif
