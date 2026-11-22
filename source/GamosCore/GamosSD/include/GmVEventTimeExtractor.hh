#ifndef GmVEventTimeExtractor_HH
#define GmVEventTimeExtractor_HH

#include "globals.hh"
class G4Event;

class GmVEventTimeExtractor {
public:
  GmVEventTimeExtractor(){};
  ~GmVEventTimeExtractor(){};
  
  virtual G4double GetEventTime( const G4Event* evt ) = 0;

};

#endif
