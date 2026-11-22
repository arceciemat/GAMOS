#ifndef GmEventDataBinFileUA_hh
#define GmEventDataBinFileUA_hh

#include "GmVDataUserBinFile.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmEventDataBinFileUA : public GmVDataUserBinFile,
			     public GmUserRunAction,
			     public GmUserEventAction,
			     public GmUserSteppingAction
{

public:
  GmEventDataBinFileUA();
  ~GmEventDataBinFileUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void BeginOfEventAction(const G4Event* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfEventAction(const G4Event* );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
