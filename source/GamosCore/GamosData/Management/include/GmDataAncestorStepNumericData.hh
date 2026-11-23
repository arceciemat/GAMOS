#ifndef GmDataAncestorStepNumericData_hh
#define GmDataAncestorStepNumericData_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"
class GmStepMgr;

class GmDataAncestorStepNumericData : public GmVData
{
public:
  GmDataAncestorStepNumericData();
  ~GmDataAncestorStepNumericData();

  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

  void SetAncestorData(GmVData* data );

protected:
  GmStepMgr* theStepMgr;
  GmVData* theAncestorData;
};
#endif
