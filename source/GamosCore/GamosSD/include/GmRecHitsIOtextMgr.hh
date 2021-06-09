#ifndef GmRecHitsIOtextMgr_h
#define GmRecHitsIOtextMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmRecHitsIOtextMgr    
Author:      P. Arce
Changes:     10/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Class to manage the reading and writing of hits  */ 
#include <map>

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GmHitList.hh"
#include "GmVRecHitsIOMgr.hh"
class GmEventTimeExtractorFromHit;
class GmRecHit;

//------------------------------------------------------------------------
class GmRecHitsIOtextMgr : public GmIOtextMgr, public GmVRecHitsIOMgr
{ 
public:
  GmRecHitsIOtextMgr();

  ~GmRecHitsIOtextMgr();

  virtual void WriteHit(GmRecHit* rhit);
  void ReadHits();

private:
};

#endif
