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

private:
  static G4double theNEvent;
  static G4double theNEventFactor;

};

#endif
