#ifndef __GmProdCutsForAllProcessesUA__
#define __GmProdCutsForAllProcessesUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"
#include <map>

class G4Run;
class G4Track;
class G4LogicalVolume;

class GmProdCutsForAllProcessesUA : public GmUserRunAction, public GmUserStackingAction
{
public:
  GmProdCutsForAllProcessesUA();
  ~GmProdCutsForAllProcessesUA(){};
  virtual void BeginOfRunAction(const G4Run* );

  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);

private:
  std::map<G4LogicalVolume*,G4double> theGammaCuts;
  std::map<G4LogicalVolume*,G4double> theElectronCuts;
  std::map<G4LogicalVolume*,G4double> thePositronCuts;


};

#endif
