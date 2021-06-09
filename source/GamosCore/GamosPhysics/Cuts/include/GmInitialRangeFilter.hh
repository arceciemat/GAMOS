#ifndef GmInitialRangeFilter_h
#define GmInitialRangeFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
class GmCutsEnergy2Range;

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles if range at creation is in defined limits
//
///////////////////////////////////////////////////////////////////////////////

class GmInitialRangeFilter : public GmVFilter 
{
  
  //-------
public: // with description
  GmInitialRangeFilter(G4String name);
  // Constructor
  
  virtual ~GmInitialRangeFilter();
  public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  virtual G4bool AcceptStep(const G4Step*);
  
  //
  void show();
  void SetParameters( std::vector<G4String>& params);

private:
  G4double fLowLimit;
  G4double fHighLimit;

  G4bool bAccept;

  GmCutsEnergy2Range* theCutsEnergy2Range;
};

#endif
