#ifndef GmEventTimeExtractorFromFirstTrack_HH
#define GmEventTimeExtractorFromFirstTrack_HH

#include "globals.hh"
class G4Event;
#include "GmVEventTimeExtractor.hh"

class GmEventTimeExtractorFromFirstTrack : public GmVEventTimeExtractor
{
public:
  GmEventTimeExtractorFromFirstTrack(){};
  ~GmEventTimeExtractorFromFirstTrack(){};
  
  virtual G4double GetEventTime( const G4Event* evt );

};

#endif
