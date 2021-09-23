#ifndef GmHistoryOrAncestorsFilterForScorer_h
#define GmHistoryOrAncestorsFilterForScorer_h 1

#include "GmVANDFilter.hh"
#include <set>
class GmTrackInfo;

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a list of filters and checks if a track passes them in this step or any previous one of any of its ancestors (if it passed, it does not check again)
// It is invoked for all G4Step and G4Track to store information (Scorer only invokes if step in MFD)
// 
///////////////////////////////////////////////////////////////////////////////

#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"

class GmHistoryOrAncestorsFilterForScorer : public GmVANDFilter,
					    public GmUserEventAction, 
					    public GmUserSteppingAction,
					    public GmUserTrackingAction
{

public: // with description
  GmHistoryOrAncestorsFilterForScorer(G4String name);
  
  virtual ~GmHistoryOrAncestorsFilterForScorer();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );
  virtual G4bool AcceptStackedTrack(const G4Track* );

  virtual void BeginOfEventAction(const G4Event* );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void PreUserTrackingAction( const G4Track* aTrack );
  
private:
  std::set<G4int> bPassed; // list of tracks (ancestors) that passed the filters in any previous step, or PreTrack
};

#endif
