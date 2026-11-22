#ifndef __GmGeomTextDumperUA__
#define __GmGeomTextDumperUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmGeomTextDumperUA : public GmUserRunAction
{
public:
  GmGeomTextDumperUA();
  ~GmGeomTextDumperUA(){};
  virtual void BeginOfRunAction( const G4Run* aRun );

private:
  G4String theFileName;
};

#endif
