#ifndef GmVisModelUA_hh
#define GmVisModelUA_hh 1

#include <map>
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmVisModelUA : public GmUserRunAction
{
public:   

  GmVisModelUA();
  virtual ~GmVisModelUA();
  
  virtual void BeginOfRunAction( const G4Run* aRun );

private:

};


#endif
