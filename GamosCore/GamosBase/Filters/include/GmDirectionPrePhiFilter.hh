#ifndef GmDirectionPrePhiFilter_h
#define GmDirectionPrePhiFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles if its phi direction is inside a defined user range. It uses PreStepPoint direction in AcceptStep
//
///////////////////////////////////////////////////////////////////////////////

class GmDirectionPrePhiFilter : public GmVFilter 
{
  
  //-------
public: // with description
  GmDirectionPrePhiFilter(G4String name);
  // Constructor
  
  virtual ~GmDirectionPrePhiFilter();
  
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
