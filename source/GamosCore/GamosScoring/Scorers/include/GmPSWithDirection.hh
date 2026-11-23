#ifndef GmPSWithDirection_h
#define GmPSWithDirection_h 1

#include "globals.hh"
#include <vector>
#include "G4PSDirectionFlag.hh"
////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Surface Flux.
//  Current version assumes only for G4Box shape, and the surface
//  is defined at the -Z plane of the box.
//   The surface flux is given in the unit of area. 
//    e.g.  sum of 1/cos(T)/mm2,  where T is a incident angle of the
//                                track on the surface.
//
//
// Surface is defined at the -Z surface.
// Direction                  -Z   +Z
//   0  IN || OUT            ->|<-  |        fFlux_InOut
//   1  IN                   ->|    |        fFlux_In
//   2  OUT                    |<-  |        fFlux_Out
//
///////////////////////////////////////////////////////////////////////////////

class GmPSWithDirection
{
  public: // with description
      GmPSWithDirection();
      virtual ~GmPSWithDirection();

  virtual void SetParameters( const std::vector<G4String>& params );

  protected:
      G4int  fDirection;

};

#endif
