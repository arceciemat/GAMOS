#ifndef GmRecHitsIObinMgr_h
#define GmRecHitsIObinMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmRecHitsIObinMgr    
Author:      P. Arce
Changes:     10/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Class to manage the reading and writing of hits  */ 
#include <map>

#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"
#include "GmHitList.hh"
#include "GmVRecHitsIOMgr.hh"
class GmEventTimeExtractorFromHit;
class GmRecHit;

//------------------------------------------------------------------------
class GmRecHitsIObinMgr : public GmIObinMgr, public GmVRecHitsIOMgr
{ 
public:
  GmRecHitsIObinMgr();

  ~GmRecHitsIObinMgr();

  virtual void WriteHit(GmRecHit* rhit);
  void ReadHits();

private:

};

#endif
