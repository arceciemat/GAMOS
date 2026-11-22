#ifndef GmPrintXSUA_hh
#define GmPrintXSUA_hh 1

#include <map>
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmPrintXSUA : public GmUserRunAction
{
public:   

  GmPrintXSUA();
  virtual ~GmPrintXSUA();
  
  virtual void BeginOfRunAction( const G4Run* aRun );

private:

};


#endif
