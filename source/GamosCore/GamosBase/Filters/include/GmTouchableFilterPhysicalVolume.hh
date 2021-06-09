#ifndef GmTouchableFilterPhysicalVolume_h
#define GmTouchableFilterPhysicalVolume_h 1

class G4VPhysicalVolume;
class G4VTouchable;
#include "GmVTouchableFilterVolume.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4VPhysicalVolume
// 
///////////////////////////////////////////////////////////////////////////////

class GmTouchableFilterPhysicalVolume : public GmVTouchableFilterVolume 
{

public: // with description
  GmTouchableFilterPhysicalVolume();
  
  virtual ~GmTouchableFilterPhysicalVolume();
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* );

  void show();

  virtual void SetParameters( std::vector<G4String>& params);

protected:
  std::set<G4VPhysicalVolume*> theVPhysicalVolumes;

};

#endif
