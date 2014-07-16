#ifndef GmTouchableFilterTouchableChildren_h
#define GmTouchableFilterTouchableChildren_h 1

class G4VTouchable;
#include "GmVTouchableFilterVolume.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4VTouchable, using also any of the children of the selected volume
// 
///////////////////////////////////////////////////////////////////////////////

class GmTouchableFilterTouchableChildren : public GmVTouchableFilterVolume 
{

public: // with description
  GmTouchableFilterTouchableChildren();
  
  virtual ~GmTouchableFilterTouchableChildren();
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* );

  void show();

  void SetParameters( std::vector<G4String>& params);

protected:
  std::vector<vpsi> theTouchableNames;
  GmGeometryUtils* geomUtils;
};

#endif
