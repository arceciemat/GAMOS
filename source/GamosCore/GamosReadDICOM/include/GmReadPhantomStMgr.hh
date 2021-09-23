#ifndef GmReadPhantomStMgr_hh
#define GmReadPhantomStMgr_hh

#include <map>
#include <set>
#include <vector>
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class GmReadPhantomStMgr 
{
  
  GmReadPhantomStMgr();
  
public:
  ~GmReadPhantomStMgr(){};
  
  static GmReadPhantomStMgr* GetInstance();
  
  void ReadStructG4( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ );
  void ReadStructEGS( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ );

  G4String GetStID( size_t id );
  std::set<size_t> GetStIDList( size_t id ); // decompose id in Structure ID's 
  G4String* GetStIDs() const {
   return theStIDs;
  }
  std::map<G4int,G4String> GetStructs() const {
    return theStructs;
  }

  std::vector<G4int> GetStIDFromPhysVolName( G4String pvName );

private:
  static GmReadPhantomStMgr* theInstance;

  G4String* theStIDs;
  std::map<G4int,G4String> theStructs; // the structure voxels
};

#endif
