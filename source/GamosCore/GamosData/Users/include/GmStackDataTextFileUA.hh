#ifndef GmStackDataTextFileUA_hh
#define GmStackDataTextFileUA_hh

#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
#include "GmVDataUserTextFile.hh"

class GmStackDataTextFileUA : public GmVDataUserTextFile,
			      public GmUserRunAction,
			      public GmUserStackingAction
{
public:
  GmStackDataTextFileUA();
  ~GmStackDataTextFileUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
