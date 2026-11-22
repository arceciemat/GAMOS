#ifndef RTCutsStudyFilter_HH
#define RTCutsStudyFilter_HH

#include "globals.hh"
#include "G4String.hh"
#include <iostream>
#include "GamosCore/GamosPhysics/Cuts/include/GmCutsStudyMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

class RTCutsStudyFilter : public GmVFilter {


public: // With description

  // Construct with filter name
  RTCutsStudyFilter(const G4String& name);

  virtual ~RTCutsStudyFilter(){};
  
  // Filter method
   virtual G4bool AcceptStep(const G4Step*);

  // Print configuration
  virtual void PrintAll(std::ostream& ) const{};
  
  // Reset 
  virtual void Reset(){};

private:
  G4double thePlaneZ;
  G4double thePlaneXDim;
  G4double thePlaneYDim;
};

#endif
