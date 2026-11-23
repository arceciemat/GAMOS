#ifndef GmPSLET_LETESpect_h
#define GmPSLET_LETESpect_h 1

#include "GmPSLET_ESpect.hh"
#include <map>
class GmNumericDistributionLogLog;

class GmPSLET_LETESpect : public GmPSLET_ESpect
{
 
public: // with description
  GmPSLET_LETESpect(G4String name);
  virtual ~GmPSLET_LETESpect();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    
};
#endif
