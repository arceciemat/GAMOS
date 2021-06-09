#ifndef GmVRecHitsIOMgr_h
#define GmVRecHitsIOMgr_h
class GmRecHit;
class GmHitsEventMgr;

//----------------------------------------------- 
/*! Class to manage the reading and writing of rec hits  */ 

//------------------------------------------------------------------------
class GmVRecHitsIOMgr 
{ 
public:
  GmVRecHitsIOMgr();

  ~GmVRecHitsIOMgr(){};

  virtual void WriteAllHits();
  virtual void WriteHit(GmRecHit* rhit) = 0;

  GmHitsEventMgr* theHitsEventMgr;
};

#endif
