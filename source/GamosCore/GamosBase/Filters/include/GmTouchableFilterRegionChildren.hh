#ifndef GmTouchableFilterRegionChildren_h
#define GmTouchableFilterRegionChildren_h 1

class G4Region;
class G4VTouchable;
#include "GmVTouchableFilterVolume.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4Region, using also any of the children of the selected volume
// 
///////////////////////////////////////////////////////////////////////////////

class GmTouchableFilterRegionChildren : public GmVTouchableFilterVolume 
{
public: 
  GmTouchableFilterRegionChildren();
  virtual ~GmTouchableFilterRegionChildren();
  
  virtual G4bool AcceptTouchable( const G4VTouchable* );
  
  void show();

  void SetParameters( std::vector<G4String>& params);
  
protected:
  std::set<G4Region*> theRegions;

};

#endif
