#include"IdealToroidWithRectangularCoil.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Geometry/BasicVector3D.h"

using namespace CLHEP;

////////////////////////////////////////////////////////////////////////////////
//
IdealToroidWithRectangularCoil::IdealToroidWithRectangularCoil(G4double RToroid,G4double Lhoriz,G4double Lvert,G4double RoutCorner,G4double dCoil, G4double I)
{ 
  rout_corner = RoutCorner;
  dcoil = dCoil;
  current = I;
  z_max1 = Lvert/2.;
  z_max2 = z_max1 + rout_corner;
  
  
  rho_min1 = RToroid - Lhoriz/2.;
  rho_min2 = rho_min1 - rout_corner;
  rho_max1 = RToroid + Lhoriz/2.;
  rho_max2 = rho_max1 + rout_corner;
  
  
  
    

} 
////////////////////////////////////////////////////////////////////////////////
//
IdealToroidWithRectangularCoil::~IdealToroidWithRectangularCoil()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void IdealToroidWithRectangularCoil::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ 
  G4ThreeVector B = G4ThreeVector(0.,0.,0.);
  
  MagField[0] = 0.;
  MagField[1] = 0.;
  MagField[2] = 0.;
  
  G4double rho =std::sqrt(yTrack[0]*yTrack[0] + yTrack[1]*yTrack[1]);
  G4double abs_z=std::abs(yTrack[2]);
  G4double enclosed_current=0.;
  if (abs_z >=z_max2 || rho<=rho_min2 || rho>=rho_max2) return ;
  if (abs_z <z_max1 &&  rho>=rho_min1 && rho<=rho_max1) {
  	enclosed_current = current;
  }
  else {
  	if (abs_z <z_max1) {
		if (rho<=rho_min2+ dcoil) { //left vert linear section
			enclosed_current=current*(rho-rho_min2)/dcoil;
		
		} 
		else  if (rho>=rho_max2-dcoil ) { //right vert linear section
			enclosed_current=current*(rho_max2-rho)/dcoil;
		
		} 
		else enclosed_current=current;
		
	}
	else if (rho >= rho_min1 && rho <= rho_max1){
	 	if (abs_z >=z_max2-dcoil) { //horizontal linear section
			enclosed_current=current*(z_max2-abs_z)/dcoil;
		}
		else enclosed_current=current;
	}
	else {
		G4double dz =abs_z-z_max1;
		G4double drho;
		if (rho <=rho_min1)  {//could be in left circular section
			drho= rho_min1-rho;
		}
		else {  //could be in right circular section
		 	drho= rho-rho_max1;
		}
		G4double dr =std::sqrt(dz*dz + drho*drho);
		if (dr < rout_corner-dcoil) enclosed_current=current;
		else if (dr<rout_corner)  enclosed_current=current* (rout_corner-dr)/dcoil;
		else  return ;
		
	
	}
  
  }
  
  
  G4double b_phi =mu0*enclosed_current/2./pi/rho;
  //the direction of I is kept as for the Coil type1
  MagField[0] = yTrack[1]*b_phi/rho;
  MagField[1] = -yTrack[0]*b_phi/rho;
  
  return;	
 
  
  
 
  
  
    
}
