#include"ReplicateField.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include "CLHEP/Geometry/BasicVector3D.h"

////////////////////////////////////////////////////////////////////////////////
//
ReplicateField::ReplicateField(G4double Phi0, G4double dPhi, size_t Nreplicate,G4double R, G4MagneticField* aField)
{ phi0 = Phi0; 
  dphi = dPhi;
  n_replicate = Nreplicate;
  r = R;
  FieldToReplicate = aField;
} 
////////////////////////////////////////////////////////////////////////////////
//
ReplicateField::~ReplicateField()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void ReplicateField::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ G4double B[3];
  G4double pos[4];
  
  pos[2] = yTrack[2]; 
  pos[3] = yTrack[3];
  
  MagField[0]=0.;
  MagField[1]=0.;
  MagField[2]=0.;
  
  B[0] =0.;
  B[1] =0.;
  B[2] =0.;
  
  G4double cphi,sphi,cdphi,sdphi,x_center,y_center;
  
  
  x_center=r*std::cos(phi0);
  y_center=r*std::sin(phi0);
  
  cphi=1.;
  sphi=0.;
  
  cdphi=std::cos(dphi);
  sdphi=std::sin(dphi);
  
  G4double sphi1,cphi1;
  
  for (size_t i=0;i<n_replicate;i++){
        /*G4cout<<i<<'\t'<<cphi<<'\t'<<sphi<<std::endl;
	G4cout<<std::acos(cphi)*180/pi<<'\t'<<std::asin(sphi)*180/pi<<std::endl;*/
	
	//compute the coordinate of the position in the internal reference fram of thefield
	G4double x,y;
	x=yTrack[0]-x_center;
	y=yTrack[1]-y_center;
	pos[0]=cphi*x+sphi*y; 
	pos[1]=cphi*y-sphi*x;
	pos[2] = yTrack[2]; 
        pos[3] = yTrack[3];
	//compute the field in the interanl reference frame and then go back to the world volume
	FieldToReplicate->GetFieldValue(pos,B);
	MagField[0] = MagField[0] + cphi*B[0] - sphi*B[1];
	MagField[1] = MagField[1] + sphi*B[0] + cphi*B[1];
	MagField[2] = MagField[2] + B[2];
	cphi1=cphi*cdphi-sphi*sdphi;
	sphi1=cphi*sdphi+sphi*cdphi;
	cphi=cphi1;
	sphi=sphi1;
	
	cphi1=x_center*cdphi-y_center*sdphi;
	sphi1=x_center*sdphi+y_center*cdphi;
	x_center=cphi1;
	y_center=sphi1;
	
  }
  
    
}
