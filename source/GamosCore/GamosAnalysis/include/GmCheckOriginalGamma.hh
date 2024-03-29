#ifndef GmCheckOriginalGamma_hh
#define GmCheckOriginalGamma_hh

#include "globals.hh"
class G4Track;
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include <set>
#include <map>

class GmCheckOriginalGamma : public GmUserEventAction, public GmUserTrackingAction
{
  public:
  GmCheckOriginalGamma();
  ~GmCheckOriginalGamma(){};

  virtual void BeginOfEventAction( const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);

  static G4int CheckIfOriginalGamma(const G4Track* aTrack);
  static G4int GetOriginalGamma(const G4Track* aTrack);

#ifndef WIN32
  static void SetCheckParentID(G4int val);
#else
#if defined GmAnalysis_ALLOC_EXPORT
  G4DLLEXPORT void SetCheckParentID(G4int val);
#else
  G4DLLIMPORT void SetCheckParentID(G4int val);
#endif
#endif

private:
  static std::set<G4int> theOrigGammas;
  static std::map<G4int,G4int> theTrackAndParentIDs;
  static std::map<G4int,G4bool> isFromRadiactiveDecay;

  static G4int bCheckParentID;
};
#endif
