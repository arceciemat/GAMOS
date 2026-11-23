#ifndef GmCutsEnergy2RangeUA__HH
#define GmCutsEnergy2RangeUA__HH

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

#include "G4UImessenger.hh"

class GmUIcmdWithAString;
class GmCutsEnergy2Range;

class GmCutsEnergy2RangeUA : public GmUserRunAction, public G4UImessenger
{
public:
  GmCutsEnergy2RangeUA();
  ~GmCutsEnergy2RangeUA();
  
  void SetNewValue(G4UIcommand * command,G4String newValues);

private:
  GmUIcmdWithAString*   cutsCmd;

  GmCutsEnergy2Range* theAction;
};

#endif
