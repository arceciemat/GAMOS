#ifndef GmStackDataBinFileUA_hh
#define GmStackDataBinFileUA_hh

#include "GmVDataUserBinFile.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"

class GmStackDataBinFileUA : public GmVDataUserBinFile,
			     public GmUserRunAction,
			     public GmUserStackingAction
{
public:
  GmStackDataBinFileUA();
  ~GmStackDataBinFileUA(){};

  virtual void BeginOfRunAction( const G4Run* );
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void EndOfRunAction( const G4Run* );

private:

};

#endif
