#ifndef GmRegionCutsMgr_h
#define GmRegionCutsMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmRegionCutsMgr    
Author:      P. Arce
Changes:      
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <vector>
#include <map>
#include "globals.hh"
class GmRegionData;
class G4Region;

//------------------------------------------------------------------------
class GmRegionCutsMgr 
{ 
 private:
  GmRegionCutsMgr();

 public:
  ~GmRegionCutsMgr();

  //! Get the only instance 
  static GmRegionCutsMgr* GetInstance();

  void AddRegionData( const std::vector<G4String>& rd );

  void AddRegionCuts( const std::vector<G4String>& rc );
  
  std::vector<GmRegionData*> FindRegionData( const G4String& name);

  void BuildRegions();
  void BuildProductionCuts();

  std::map<G4Region*,G4int> GetRegionIndices() const {
    return theRegionIndices;
  }

  void BuildRegionIndices();
  G4int GetRegionIndex( G4Region* reg );
  G4Region* GetIndexRegion( G4int index );

private:
  static GmRegionCutsMgr* theInstance;

  std::vector<GmRegionData*> theRegionDatae;

  std::map<G4Region*,G4int> theRegionIndices;

};

#endif
