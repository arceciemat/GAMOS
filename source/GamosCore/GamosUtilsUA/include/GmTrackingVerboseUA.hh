#ifndef GmTrackingVerboseUA_h
#define GmTrackingVerboseUA_h 1

///////////////////////////////////////////////////////////////////////////////
// File : TrackingVerboseAction.h
// Author: P.Arce  12.09.01
// Description:
// Modifications:
// Class with the commands to switch on/off the verbosity of tracking and event, see TrackingVerboseAction for a detailed explanation
// for a given range of tracks each 'n' tracks
// the GEANT4 command '/tracking/verbose N' will be executed when the trackNo is
//     fTVTrackMin <= trackNo <= fTVTrackMax
// each fTVTrackStep tracks (starting at 1, not 0) and if the trackNo is
//     fTVTrackMin <= trackNo <= fTVTrackMax
// each fTVTrackStep tracks (starting at 1, not 0)
// 
///////////////////////////////////////////////////////////////////////////////

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "G4TrackingManager.hh"

class GmTrackingVerboseUA : public GmUserRunAction, public GmUserEventAction, public GmUserTrackingAction
{
public:
  GmTrackingVerboseUA();
  ~GmTrackingVerboseUA(){};
  
 private:
  void SetTrackingVerbose( int verblev );

 public:
  virtual void BeginOfRunAction(const G4Run* );
  virtual void BeginOfEventAction(const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);

 private:
  int fTVTrackMin;
  int fTVTrackMax;
  int fTVTrackStep;
  int fTVEventMin;
  int fTVEventMax;
  int fTVEventStep;

  int fVerboseLevel;

  bool fTrackingVerboseON;
  bool fTkVerbThisEventON;

  G4TrackingManager* theTrackingManager;
};

#endif
