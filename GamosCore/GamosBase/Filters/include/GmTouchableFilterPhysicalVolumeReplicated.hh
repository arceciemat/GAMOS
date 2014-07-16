#ifndef GmTouchableFilterPhysicalVolumeReplicated_h
#define GmTouchableFilterPhysicalVolumeReplicated_h 1

class G4VPhysicalVolume;
class G4VTouchable;
#include "GmVTouchableFilterVolume.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4VPhysicalVolume name + copy number
// 
///////////////////////////////////////////////////////////////////////////////

class GmTouchableFilterPhysicalVolumeReplicated : public GmVTouchableFilterVolume 
{

public: // with description
  GmTouchableFilterPhysicalVolumeReplicated();
  
  virtual ~GmTouchableFilterPhysicalVolumeReplicated();
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* );

  void show();

  virtual void SetParameters( std::vector<G4String>& params);

protected:
  std::set<G4String> theVPhysicalVolumes;

};

#endif
