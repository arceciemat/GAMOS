#ifndef GmTouchableFilterPhysicalVolumeChildren_h
#define GmTouchableFilterPhysicalVolumeChildren_h 1

class G4VPhysicalVolume;
class G4VTouchable;
#include "GmVTouchableFilterVolume.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4VPhysicalVolume, using also any of the children of the selected volume
// 
///////////////////////////////////////////////////////////////////////////////

class GmTouchableFilterPhysicalVolumeChildren : public GmVTouchableFilterVolume 
{

public: // with description
  GmTouchableFilterPhysicalVolumeChildren();
  
  virtual ~GmTouchableFilterPhysicalVolumeChildren();
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* );

  void show();

  virtual void SetParameters( std::vector<G4String>& params);

protected:
  std::set<G4VPhysicalVolume*> theVPhysicalVolumes;

};

#endif
