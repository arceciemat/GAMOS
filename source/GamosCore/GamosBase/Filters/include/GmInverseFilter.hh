#ifndef GmInverseFilter_h
#define GmInverseFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter returns the opposite that the filter ir receives
// 
///////////////////////////////////////////////////////////////////////////////

class GmInverseFilter : public GmVFilter
{

public: // with description
  GmInverseFilter(G4String name);
  
  virtual ~GmInverseFilter();
  

public: // with description
  virtual G4bool AcceptTrack(const G4Track* );
  virtual G4bool AcceptStackedTrack(const G4Track* );
  virtual G4bool AcceptStep(const G4Step* );

  //
  void show();
  void SetParameters( std::vector<G4String>& params);
  
protected:
  GmVFilter* theFilters;

};

#endif
