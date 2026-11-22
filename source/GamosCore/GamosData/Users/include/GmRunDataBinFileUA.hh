#ifndef GmRunDataBinFileUA_hh
#define GmRunDataBinFileUA_hh

#include "GmVDataUserBinFile.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

class GmRunDataBinFileUA : public GmVDataUserBinFile,
			     public GmUserRunAction,
			     public GmUserSteppingAction
{

public:
  GmRunDataBinFileUA();
  ~GmRunDataBinFileUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
