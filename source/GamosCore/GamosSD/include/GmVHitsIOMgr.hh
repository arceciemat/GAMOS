#ifndef GmVHitsIOMgr_h
#define GmVHitsIOMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmVHitsIOMgr    
Author:      P. Arce
Changes:     10/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Class to manage the reading and writing of hits  */ 
#include <map>

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GmHitList.hh"
class GmHitsEventMgr;
class GmEventTimeExtractorFromHit;
class GmHit;

//------------------------------------------------------------------------
class GmVHitsIOMgr
{ 
public:
  GmVHitsIOMgr();

  ~GmVHitsIOMgr();

  void WriteAllHits();
  void WriteHitsInCurrentEvent();
  void WriteHitsInEvent( G4int evtID );
  G4int GetFirstEventWithHit( gamosSDHitMap hits );

  virtual void WriteHit(GmHit* hit) = 0;
  virtual void ReadHits(){};

protected:
  typedef std::multimap<G4int,unsigned long long> mmil;

  GmHitsEventMgr* theHitsEventMgr;
  GmEventTimeExtractorFromHit* theEventTimeExtractor;

  GmHit* theLastHit;
  G4bool bLastHitRead;
  G4bool bFirstHitRead;

  std::map<G4String,G4double> theEnergyResols;
  std::map<G4String,G4double> theTimeMinResols;
};

#endif
