#include "GmPSWithDirection.hh"
#include "G4StepStatus.hh"
////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Surface Flux.
//  Current version assumes only for G4Box shape, and the surface
//  is fixed on -Z plane.
//
// Surface is defined at the -Z surface.
// Direction                  -Z   +Z
//   0  IN || OUT            ->|<-  |
//   1  IN                   ->|    |
//   2  OUT                    |<-  |
///////////////////////////////////////////////////////////////////////////////

GmPSWithDirection::GmPSWithDirection()
{
  fDirection = fFlux_InOut;
}

GmPSWithDirection::~GmPSWithDirection()
{;}

void GmPSWithDirection::SetParameters( const std::vector<G4String>& params)
{
 if( params.size() != 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSWithDirection::SetParameters",
"There should be one parameters: DIRECTION",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
 }

 if( params[0] == "InOut" ) {
   fDirection = fFlux_InOut;
 } else if( params[0] == "In" ) {
   fDirection = fFlux_In;
 } else if( params[0] == "Out" ) {
   fDirection = fFlux_Out;
 } else {
   G4Exception("GmPSWithDirection::SetParameters",
	       "The parameter should be InOut, In or Out",
	       FatalErrorInArgument,G4String("It is: "+params[0]).c_str());
 }
 G4cout << " GmPSWithDirection  direction " << fDirection << G4endl;

}

