#ifndef GmTouchableFilterPhysicalVolumeReplicatedChildren_h
#define GmTouchableFilterPhysicalVolumeReplicatedChildren_h 1

class G4VPhysicalVolume;
class G4VTouchable;
#include "GmVTouchableFilterVolume.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4VPhysicalVolume by name and copy number, using also any of the children of the selected volume
// 
///////////////////////////////////////////////////////////////////////////////

class GmTouchableFilterPhysicalVolumeReplicatedChildren : public GmVTouchableFilterVolume 
{

public: // with description
  GmTouchableFilterPhysicalVolumeReplicatedChildren();
  
  virtual ~GmTouchableFilterPhysicalVolumeReplicatedChildren();
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* );

  void show();

  virtual void SetParameters( std::vector<G4String>& params);

protected:
  std::set<G4String> theVPhysicalVolumes;

};

#endif
