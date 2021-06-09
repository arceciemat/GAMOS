#ifndef GmTouchableFilterTouchable_h
#define GmTouchableFilterTouchable_h 1

class G4VTouchable;
#include "GmVTouchableFilterVolume.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include <vector>
#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters touchables by G4VTouchable
// 
///////////////////////////////////////////////////////////////////////////////

class GmTouchableFilterTouchable : public GmVTouchableFilterVolume 
{

public: // with description
  GmTouchableFilterTouchable();
  
  virtual ~GmTouchableFilterTouchable();
  
public: // with description
  virtual G4bool AcceptTouchable( const G4VTouchable* );

  void show();

  void SetParameters( std::vector<G4String>& params);

protected:
  std::vector<vpsi> theTouchableNames;
  GmGeometryUtils* geomUtils;
};

#endif
