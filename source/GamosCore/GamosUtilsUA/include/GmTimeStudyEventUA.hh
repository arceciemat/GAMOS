#ifndef GmTimeStudyEventUA_hh
#define GmTimeStudyEventUA_hh 1

#include "GmTimeStudyUA.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"

class GmTimeStudyEventUA : public GmTimeStudyUA,
		      public GmUserEventAction
{
  typedef  std::map<unsigned int, G4SliceTimer*> mut;

public:   

  GmTimeStudyEventUA();
  virtual ~GmTimeStudyEventUA();
  
  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfEventAction( const G4Event* );


};


#endif
