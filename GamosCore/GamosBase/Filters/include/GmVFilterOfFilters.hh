#ifndef GmVFilterOfFilters_h
#define GmVFilterOfFilters_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <vector>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class makes a lsit of filters to filter on all of them
// 
///////////////////////////////////////////////////////////////////////////////

class GmVFilterOfFilters : public GmVFilter 
{

public: // with description
  GmVFilterOfFilters(G4String name);
  
  virtual ~GmVFilterOfFilters();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track* ){
    return TRUE; 
  };
  virtual G4bool AcceptStep(const G4Step* ){
    return TRUE; 
  };
 
  //
  void show();
  void SetParameters( std::vector<G4String>& params);
  
protected:
  std::vector<GmVFilter*> theFilters;

};

#endif
