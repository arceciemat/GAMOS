#ifndef GmHitsIObinMgr_h
#define GmHitsIObinMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmHitsIObinMgr    
Author:      P. Arce
Changes:     10/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Class to manage the reading and writing of hits  */ 
#include <map>

#include "GamosCore/GamosBase/Base/include/GmIObinMgr.hh"
#include "GmHitList.hh"
#include "GmVHitsIOMgr.hh"
class GmHitsEventMgr;
class GmEventTimeExtractorFromHit;
class GmHit;

//------------------------------------------------------------------------
class GmHitsIObinMgr : public GmIObinMgr, public GmVHitsIOMgr
{ 
public:
  GmHitsIObinMgr();

  ~GmHitsIObinMgr();

  virtual void WriteHit(GmHit* hit);

};

#endif
