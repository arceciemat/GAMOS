#ifndef GmDumpGeometryUA_hh
#define GmDumpGeometryUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmDumpGeometryUA : public GmUserRunAction
{
public:
  GmDumpGeometryUA();
  ~GmDumpGeometryUA(){};
  virtual void BeginOfRunAction(const G4Run* );

private:
  G4int theVerbosity;
};

#endif
