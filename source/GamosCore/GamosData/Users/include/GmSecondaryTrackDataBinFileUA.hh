#ifndef GmSecondaryTrackDataBinFileUA_hh
#define GmSecondaryTrackDataBinFileUA_hh

#include "GmVDataUserBinFile.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include <set>

class GmSecondaryTrackDataBinFileUA : public GmVDataUserBinFile,
				      public GmUserRunAction,
				      public GmUserSteppingAction
{
public:
  GmSecondaryTrackDataBinFileUA();
  ~GmSecondaryTrackDataBinFileUA(){};
  virtual void BeginOfRunAction( const G4Run* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
