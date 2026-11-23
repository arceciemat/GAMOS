#ifndef GmNumberOfEvent_h
#define GmNumberOfEvent_h 1

class G4Step;
#include "globals.hh"
#include <vector>

// class description:
//
//  This is the abstract base class of a filter to be associated with a
// sensitive detector. 

class GmNumberOfEvent
{
public:
  GmNumberOfEvent(G4String){};
  ~GmNumberOfEvent(){};

public:
  static void SetNumberOfEvent(G4double nev);
  static G4double GetNumberOfEvent();
  static void SetNumberOfEventFactor(G4double nevfac);
  static G4double GetNumberOfEventFactor();
  static void AddEvent();

  //--- Multithreading: add the events counted in this worker since last merge to the master count (called by GmMTMergeMgr)
  static void MergeToMaster();

private:
  static G4ThreadLocal G4double theNEvent;
  static G4ThreadLocal G4double theNEventFactor;
  static G4ThreadLocal G4double theNEventMerged; // worker: events (times factor) already merged
  static G4double theMasterNEvent; // master: sum of worker events (times factor); -1 if not set by any worker

};

#endif
