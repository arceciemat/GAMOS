#ifndef GmTouchableFilterLogicalVolumeChildren_h
#define GmTouchableFilterLogicalVolumeChildren_h 1

class G4LogicalVolume;
class G4VTouchable;
#include "GmVTouchableFilterVolume.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4LogicalVolume, using also any of the children of the selected volume
// 
///////////////////////////////////////////////////////////////////////////////

class GmTouchableFilterLogicalVolumeChildren : public GmVTouchableFilterVolume 
{

public: // with description
  GmTouchableFilterLogicalVolumeChildren();
  
  virtual ~GmTouchableFilterLogicalVolumeChildren();
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* );

  void show();

  virtual void SetParameters( std::vector<G4String>& params);

protected:
  std::set<G4LogicalVolume*> theLogicalVolumes;

};

#endif
