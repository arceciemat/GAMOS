#ifndef GmStackDataCoutUA_hh
#define GmStackDataCoutUA_hh

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
#include "GmVDataUser.hh"

class GmStackDataCoutUA : public GmVDataUser,
			      public GmUserRunAction,
			      public GmUserStackingAction
{
public:
  GmStackDataCoutUA();
  ~GmStackDataCoutUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
