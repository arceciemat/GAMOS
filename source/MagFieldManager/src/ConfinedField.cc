#include"ConfinedField.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Geometry/BasicVector3D.h"

using namespace CLHEP;

////////////////////////////////////////////////////////////////////////////////
//
ConfinedField::ConfinedField(G4MagneticField* aField, G4double var1_min,G4double var1_max, 
    			G4double var2_min,G4double var2_max, 
			G4double var3_min,G4double var3_max,
			G4bool aBool)
{ theField = aField;
  IsCartesian = aBool;
  if (IsCartesian){
  	x_min=var1_min;
	x_max=var1_max;
	y_min=var2_min;
	y_max=var2_max;
	z_min=var3_min;
	z_max=var3_max;
  }
  else {
  	rho_min=var1_min;
	rho_max=var1_max;
	phi_min=var2_min;
	phi_max=var2_max;
	z_min=var3_min;
	z_max=var3_max;
  }
  

} 
////////////////////////////////////////////////////////////////////////////////
//
ConfinedField::~ConfinedField()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void ConfinedField::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ G4ThreeVector B = G4ThreeVector(0.,0.,0.);
  G4bool ComputeTheField =false;
  
  if (IsCartesian){
  	ComputeTheField = (yTrack[0]<=x_max && yTrack[0]>=x_min &&  
			   yTrack[1]<=y_max && yTrack[1]>=y_min &&
			   yTrack[2]<=z_max && yTrack[2]>=z_min );
  }
  else {
  	G4double rho =std::sqrt(yTrack[0]*yTrack[0] + yTrack[1]*yTrack[1]);
	G4double cphi=1.;
	G4double sphi=0.;
	if (rho >0.0001*m){
		cphi=yTrack[0]/rho;
		sphi=yTrack[1]/rho;
	}
	G4double phi=std::acos(cphi);
	if (sphi <0) phi=twopi-phi;
	ComputeTheField = (rho<=rho_max && rho>=rho_min &&
			   phi<=phi_max && phi>= phi_min &&
			   yTrack[2]<=z_max && yTrack[2]>=z_min );
				
  
  }
  if (ComputeTheField) {
  	theField->GetFieldValue(yTrack,MagField);
  } 
  else {
  	MagField[0]=0.;
  	MagField[1]=0.;
  	MagField[2]=0.; 
  	
  }	
  
  
    
}
