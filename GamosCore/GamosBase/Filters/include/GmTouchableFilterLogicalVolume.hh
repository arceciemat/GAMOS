#ifndef GmTouchableFilterLogicalVolume_h
#define GmTouchableFilterLogicalVolume_h 1

class G4LogicalVolume;
class G4VTouchable;
#include "GmVTouchableFilterVolume.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4LogicalVolume
// 
///////////////////////////////////////////////////////////////////////////////

class GmTouchableFilterLogicalVolume : public GmVTouchableFilterVolume 
{

public: // with description
  GmTouchableFilterLogicalVolume();
  
  virtual ~GmTouchableFilterLogicalVolume();
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* );

  virtual void show();

  virtual void SetParameters( std::vector<G4String>& params);

protected:
  std::set<G4LogicalVolume*> theLogicalVolumes;

};

#endif
