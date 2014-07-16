#ifndef GmCutsStudyMgr_h
#define GmCutsStudyMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmCutsStudyMgr    
Author:      P. Arce
Changes:     05/08: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Manager for cuts studies  */ 

#include "globals.hh"
class G4Region;
class GmCSTrackInfo;
class GmCSTrackStepInfo;
class G4Step;
#include "G4VFilter.hh"
#include "G4ThreeVector.hh"

#include <map>
#include <vector>

typedef std::multimap<G4Region*,GmCSTrackInfo*> mmrcs;
typedef G4VFilter<G4Step> GmVCSFilter;

//------------------------------------------------------------------------
class GmCutsStudyMgr 
{ 
private:
  GmCutsStudyMgr();
 public:
  ~GmCutsStudyMgr();
  static GmCutsStudyMgr* GetInstance();
  void AddTrackInfo( GmCSTrackInfo* );
  mmrcs GetTrackInfos() const { return theTrackInfos; }
  GmCSTrackInfo* GetTrackInfo( G4int id ); 
  GmCSTrackStepInfo* GetTrackStepInfo( G4int id, G4ThreeVector pos );
  std::vector<GmCSTrackStepInfo*> GetTrackStepInfosBefore( G4int id, G4double trklen );
  GmVCSFilter* GetFilter() const { 
    return theFilter; }
  void SetFilter( GmVCSFilter* filt ) {
    theFilter = filt; }

  void CleanTrackInfos();

private:
  static GmCutsStudyMgr* theInstance;
  std::multimap<G4Region*,GmCSTrackInfo*> theTrackInfos;
  GmVCSFilter* theFilter;
};

#endif
