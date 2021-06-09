#ifndef GmStepDataBinFileUA_hh
#define GmStepDataBinFileUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GmVDataUserBinFile.hh"

class GmStepDataBinFileUA : public GmVDataUserBinFile,
			    public GmUserRunAction,
			    public GmUserSteppingAction
{
public:
  GmStepDataBinFileUA();
  ~GmStepDataBinFileUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
