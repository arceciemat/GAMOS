#ifndef GmEventTimeExtractorFromHit_HH
#define GmEventTimeExtractorFromHit_HH

#include "globals.hh"
class G4Event;
#include "GmVEventTimeExtractor.hh"

class GmEventTimeExtractorFromHit : public GmVEventTimeExtractor
{
public:
  GmEventTimeExtractorFromHit(){};
  ~GmEventTimeExtractorFromHit(){};
  
  virtual G4double GetEventTime( const G4Event* evt );
  void SetEventTime( G4double evtt ){
    theEventTime = evtt; }

private:
  G4double theEventTime;
};

#endif
