#include"InterpolatedField.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"globals.hh"
#include "fstream"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

////////////////////////////////////////////////////////////////////////////////
//
InterpolatedField::InterpolatedField(G4String file_name)//current turn from y to z
{ Read3DGrid(file_name);	
} 
////////////////////////////////////////////////////////////////////////////////
//
InterpolatedField::~InterpolatedField()
{;
}

////////////////////////////////////////////////////////////////////////////////
//
void InterpolatedField::Read3DGrid(G4String file_name)
{ 
  std::ifstream InputFile;
  InputFile.open(file_name, std::ios::in);
  G4String type;
  InputFile>>type;
  if (type == "Cartesian") IsCartesian=true;
  else  IsCartesian=false;
  
  G4int n1,n2,n3;
  InputFile>>n1>>n2>>n3;
  
  G4String st1,st2,st3,st4,st5,st6,st7,st8;
  InputFile>>st1>>st2>>st3>>st4>>st5>>st6>>st7>>st8;
  G4cout<<st1<<'\t'<<st2<<'\t'<<st3<<'\t'<<std::endl;
  
  B1_grid3D.clear();
  B2_grid3D.clear();
  B3_grid3D.clear();
  
  G4double b1,b2,b3,bb;
  G4double var1,var2,var3;
  G4bool first=true;
  for (int i=0;i<n1;i++){
 	B1_grid3D.push_back(std::vector< std::vector<float> >());
	B2_grid3D.push_back(std::vector< std::vector<float> >());
	B3_grid3D.push_back(std::vector< std::vector<float> >());
	for (int j=0;j<n2;j++){
		B1_grid3D[i].push_back(std::vector<float>());
	        B2_grid3D[i].push_back(std::vector<float>());
	        B3_grid3D[i].push_back(std::vector<float>());
		for (int k=0;k<n3;k++){
		        
			InputFile>>var1>>var2>>var3>>b1>>b2>>b3>>bb;
			if (first) {
				G4cout<<var1<<'\t'<<var2<<'\t'<<var3<<'\t'<<std::endl;
			}
			if (IsCartesian){
			  xmax=var1*CLHEP::m; 
				ymax=var2*CLHEP::m;
				zmax=var3*CLHEP::m;
				if (first){
					xmin=xmax;
					ymin=ymax;
					zmin=zmax;
					first=false;
				}
			}
			else {
				rho_max=var1*CLHEP::m;
				phi2=var2;
				zmax=var3*CLHEP::m;
				if (first){
					rho_min=rho_max;
					phi1=phi2;
					zmin=zmax;
					first=false;
				}
			}	

			
				
			B1_grid3D[i][j].push_back(b1*CLHEP::tesla);
			B2_grid3D[i][j].push_back(b2*CLHEP::tesla);
			B3_grid3D[i][j].push_back(b3*CLHEP::tesla);
					
		}
	}
  }
  dx=0.;
  dy=0.;
  dz=0.;
  drho=0.;
  dphi=0.;
  if (IsCartesian){
  	if (n1 > 1) dx=(xmax-xmin)/float(n1-1);
  	if (n2 > 1) dy=(ymax-ymin)/float(n2-1);
  	if (n3 > 1) dz=(zmax-zmin)/float(n3-1);
  }
  else {
  	if (n1 > 1) drho=(rho_max-rho_min)/float(n1-1);
  	if (n2 > 1) dphi=(phi2-phi1)/float(n2-1);
	if (n3 > 1) dz=(zmax-zmin)/float(n3-1);
	G4cout<<n1<<'\t'<<n2<<'\t'<<n3<<std::endl;
	G4cout<<rho_min<<'\t'<<phi1<<'\t'<<zmin<<std::endl;
	G4cout<<rho_max<<'\t'<<phi2<<'\t'<<zmax<<std::endl;
	
	G4cout<<"Cylindrical "<<'\t'<<drho/CLHEP::m<<'\t'<<dphi/CLHEP::degree<<'\t'<<dz/CLHEP::m<<std::endl;
  }	
}

////////////////////////////////////////////////////////////////////////////////
//
void InterpolatedField::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ G4int i,j,k;
  G4double x,y,z,rho,phi,cphi = 0,sphi = 0;
  //GAMOS  G4int n1 = B1_grid3D.size()-1;
  //GAMOS  G4int n2= B1_grid3D[0].size()-1;
  //GAMOS  G4int n3= B1_grid3D[0][0].size()-1;
  x=yTrack[0];
  y=yTrack[1];
  z=yTrack[2]; 
  
  MagField[0]=0.;
  MagField[1]=0.;
  MagField[2]=0.;
  
  float fi,fj,fk;
  
  if (IsCartesian) {
        G4bool ComputeField = (x<xmax && x>=xmin &&  
			       y<ymax && y>=ymin &&
			       z<zmax && z>=zmin );
	if (!ComputeField) return;		       
  	i=int( (x-xmin)/dx);
  	fi = (x- (xmin+i*dx))/dx;
	
	j=int( (y-ymin)/dy);
  	fj = (y- (ymin+j*dy))/dy;
  }
  else {
  
  	rho=std::sqrt(x*x + y*y);
  	cphi=1.;
  	sphi=0;
  	if (rho >0.000000001){
  		cphi=x/rho;
		sphi=y/rho;
  	}
  	
	phi = std::acos(cphi);
  	if (sphi <0) phi= CLHEP::twopi-phi;
  	phi=phi-phi1;
	if (phi < 0.) phi += CLHEP::twopi;	
  	phi=std::fmod(phi,phi2-phi1);
	
	G4bool ComputeField = (rho<rho_max && rho>=rho_min &&
			   z<zmax && z>=zmin );
	if (!ComputeField) return;		   
  	
	i=int( (rho-rho_min)/drho);
  	fi = (rho- (rho_min+i*drho))/drho;
	
  	
	j=int( phi/dphi);
  	fj = (phi- (j*dphi))/dphi;
  	
  
  }	
 // std::cout<<i<<'\t'<<j<<'\t'<<k<<std::endl;

  k=int( (z-zmin)/dz);
  fk = (z- (zmin+k*dz)) /dz;
  
  
  
  float fi1=1.-fi;
  float fj1=1.-fj;
  float fk1=1.-fk;
 
  float fij=fi*fj;
  float fijk=fij*fk;
  float fijk1=fij*fk1;
  
  float fi1j=fi1*fj;
  float fi1jk=fi1j*fk;
  float fi1jk1=fi1j*fk1;
  
  float fij1=fi*fj1;
  float fij1k = fij1*fk;
  float fij1k1 = fij1*fk1;
  
  float fi1j1=fi1*fj1;
  float fi1j1k=fi1j1*fk;
  float fi1j1k1=fi1j1*fk1;
  
  float b1,b2;
  
  
  b1=     B1_grid3D[i][j][k]* fi1j1k1+
 	  B1_grid3D[i][j][k+1]*fi1j1k+
	  B1_grid3D[i][j+1][k]*fi1jk1 +
 	  B1_grid3D[i][j+1][k+1]*fi1jk+
	  B1_grid3D[i+1][j][k]*fij1k1+
 	  B1_grid3D[i+1][j][k+1]*fij1k+
	  B1_grid3D[i+1][j+1][k]*fijk1+
 	  B1_grid3D[i+1][j+1][k+1]*fijk;
  
  b2=     B2_grid3D[i][j][k]* fi1j1k1+
 	  B2_grid3D[i][j][k+1]*fi1j1k+
	  B2_grid3D[i][j+1][k]*fi1jk1 +
 	  B2_grid3D[i][j+1][k+1]*fi1jk+
	  B2_grid3D[i+1][j][k]*fij1k1+
 	  B2_grid3D[i+1][j][k+1]*fij1k+
	  B2_grid3D[i+1][j+1][k]*fijk1+
 	  B2_grid3D[i+1][j+1][k+1]*fijk;
	  
  if (IsCartesian) {
  	
	MagField[0] = b1;	
  	MagField[1] = b2;	
  }   
  else {
  	MagField[0]=	cphi*b1 -sphi*b2;   
  	MagField[1]=	sphi*b1 +cphi*b2;
  }
	  	  
	  
  MagField[0]=	cphi*b1 -sphi*b2;   
  MagField[1]=	sphi*b1 +cphi*b2; 
  
  MagField[2]= B3_grid3D[i][j][k]* fi1j1k1+
 	  B3_grid3D[i][j][k+1]*fi1j1k+
	  B3_grid3D[i][j+1][k]*fi1jk1 +
 	  B3_grid3D[i][j+1][k+1]*fi1jk+
	  B3_grid3D[i+1][j][k]*fij1k1+
 	  B3_grid3D[i+1][j][k+1]*fij1k+
	  B3_grid3D[i+1][j+1][k]*fijk1+
 	  B3_grid3D[i+1][j+1][k+1]*fijk;
  
  
 
   
 
}
