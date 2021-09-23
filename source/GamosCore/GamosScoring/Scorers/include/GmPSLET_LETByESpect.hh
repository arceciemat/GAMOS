#ifndef GmPSLET_LETByESpect_h
#define GmPSLET_LETByESpect_h 1

#include "GmPSLETByESpect.hh"
#include <map>
class GmNumericDistributionLogLog;

class GmPSLET_LETByESpect : public GmPSLETByESpect
{
 
public: // with description
  GmPSLET_LETByESpect(G4String name);
  virtual ~GmPSLET_LETByESpect();
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    
};
#endif
