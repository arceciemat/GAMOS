#ifndef GmFutureFilter_h
#define GmFutureFilter_h 1

#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

class GmFutureFilter : public GmVFilter
{
  
public: // with description
  GmFutureFilter(G4String name);
  
public:
  virtual ~GmFutureFilter();
  
public: // with description
  virtual void SetParameters( std::vector<G4String>& );

  virtual G4bool AcceptTrack(const G4Track*){ 
    return TRUE; 
  }
  virtual G4bool AcceptFutureStep(const G4Step* aStep); // accept current step
  virtual G4bool AcceptPastStep(const G4Step* aStep);
  virtual G4bool AcceptStep(const G4Step* aStep){ 
    return AcceptFutureStep( aStep );
  }
  
public:
  G4bool IsFilterWithChildren() const {
    return bFilterWithChildren; 
  }

protected:
  GmVFilter* thePastFilter;
  GmVFilter* theFutureFilter;
  G4bool bFilterWithChildren;
};

#endif
