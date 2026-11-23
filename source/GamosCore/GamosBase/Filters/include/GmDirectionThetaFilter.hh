#ifndef GmDirectionThetaFilter_h
#define GmDirectionThetaFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles if its theta direction is inside a defined user range. It uses PostStepPoint direction in AcceptStep
//
///////////////////////////////////////////////////////////////////////////////

class GmDirectionThetaFilter : public GmVFilter 
{
  
  //-------
public: // with description
  GmDirectionThetaFilter(G4String name);
  // Constructor
  
  virtual ~GmDirectionThetaFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  virtual G4bool AcceptStep(const G4Step*);
  
  void show();
  void SetParameters( std::vector<G4String>& params);

private:
  G4double theLowLimit;
  G4double theHighLimit;

};

#endif
