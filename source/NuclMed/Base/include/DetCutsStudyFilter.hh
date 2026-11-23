#ifndef DetCutsStudyFilter_HH
#define DetCutsStudyFilter_HH
// Filter tracks looking if it is making a hit (= step in sensitive detector with deposited energy != 0 )

#include "GamosCore/GamosPhysics/Cuts/include/GmCutsStudyMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include "globals.hh"
#include "G4String.hh"
#include <iostream>

class DetCutsStudyFilter : public GmVFilter {

public: // With description

  // Construct with filter name
  DetCutsStudyFilter(const G4String& name);

  virtual ~DetCutsStudyFilter(){};
  
  // Filter method
  virtual G4bool AcceptStep(const G4Step*);

  // Print configuration
  virtual void PrintAll(std::ostream& ) const{};
  
  // Reset 
  virtual void Reset(){};

private:

};

#endif
