#ifndef GmHitsIOtextMgr_h
#define GmHitsIOtextMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmHitsIOtextMgr    
Author:      P. Arce
Changes:     10/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Class to manage the reading and writing of hits  */ 
#include <map>

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GmHitList.hh"
#include "GmVHitsIOMgr.hh"
class GmHitsEventMgr;
class GmEventTimeExtractorFromHit;
class GmHit;

//------------------------------------------------------------------------
class GmHitsIOtextMgr : public GmIOtextMgr, public GmVHitsIOMgr
{ 
public:
  GmHitsIOtextMgr();

  ~GmHitsIOtextMgr();

  virtual void WriteHit(GmHit* hit);
  virtual void ReadHits();

};

#endif
