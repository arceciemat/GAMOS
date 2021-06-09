#ifndef GmStepDataTextFileUA_hh
#define GmStepDataTextFileUA_hh

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GmVDataUserTextFile.hh"

class GmStepDataTextFileUA : public GmVDataUserTextFile,
			     public GmUserRunAction,
			     public GmUserSteppingAction
{
public:
  GmStepDataTextFileUA();
  ~GmStepDataTextFileUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
