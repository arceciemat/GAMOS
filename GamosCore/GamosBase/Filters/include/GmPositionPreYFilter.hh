#ifndef GmPositionPreYFilter_h
#define GmPositionPreYFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles if its Y position is inside a defined user range. It uses PreStepPoint position in AcceptStep
//
///////////////////////////////////////////////////////////////////////////////

class GmPositionPreYFilter : public GmVFilter 
{
  
  //-------
public: // with description
  GmPositionPreYFilter(G4String name);
  // Constructor
  
  virtual ~GmPositionPreYFilter();
  
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
