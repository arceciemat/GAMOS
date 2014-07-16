#ifndef __GmGamosVerboseByEventUA__
#define __GmGamosVerboseByEventUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

typedef  std::multimap<G4int, std::pair<G4String,G4int> > mmis;

class GmGamosVerboseByEventUA :  public GmUserEventAction,
				public G4UImessenger
{

 public:
  GmGamosVerboseByEventUA();
  ~GmGamosVerboseByEventUA();

 public:
  virtual void BeginOfEventAction(const G4Event* );

  virtual void SetNewValue(G4UIcommand * command,G4String newValues);

 private:
  GmUIcmdWithAString* verbCmd;

  mmis theVerbs;

};

#endif
