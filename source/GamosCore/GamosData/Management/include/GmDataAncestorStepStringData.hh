#ifndef GmDataAncestorStepStringData_hh
#define GmDataAncestorStepStringData_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"
class GmStepMgr;

class GmDataAncestorStepStringData : public GmVDataString
{
public:
  GmDataAncestorStepStringData();
  ~GmDataAncestorStepStringData();

  virtual G4String GetStringValueFromStep( const G4Step* aStep);

  void SetAncestorData(GmVData* data );

protected:
  GmStepMgr* theStepMgr;
  GmVData* theAncestorData;
};
#endif
