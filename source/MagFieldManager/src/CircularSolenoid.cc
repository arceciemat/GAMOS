#include"CircularSolenoid.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"special_functions.hh"
#include"globals.hh"
#include "CLHEP/Units/PhysicalConstants.h"

////////////////////////////////////////////////////////////////////////////////
//
CircularSolenoid::CircularSolenoid(G4double R,G4double dr, G4double dz,G4double current)
{ Radius = R;
  DR = dr;
  DZ = dz;
  I = current;
} 
////////////////////////////////////////////////////////////////////////////////
//
CircularSolenoid::~CircularSolenoid()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void CircularSolenoid::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ 
  G4ThreeVector pos = G4ThreeVector(yTrack[0],yTrack[1],yTrack[2]);
  G4double z= pos.z();
  G4double rho =pos.rho();
  
  G4double z1=z;
  if (z1<0) z1=-z1;
  
  
  //look for nZ and nR could be probably improved
  G4int nZ;
  G4int nR;
  
  for (int i=2; i<= 10; i++){
  	G4double z0 =  0.5*DZ - 0.5*DZ/i;
	G4double z00 = 0.5*DZ - 1.5*DZ/i;
	G4double a= Radius+  0.5*DR;
	G4double k0=std::sqrt(4.*rho*a/( (rho+a)*(rho+a) + (z1-z0)*(z1-z0)));
	G4double k00=std::sqrt(4.*rho*a/( (rho+a)*(rho+a) + (z1-z00)*(z1-z00)));
	nZ=i; 
	G4cout<<k0<<'\t'<<k00<<std::endl;
	G4double diff =(k0-k00)/k0;
	if (diff <0) diff=-diff; 
	if ( diff <0.005) {
		if ( diff <0.001 && nZ==2) nZ=1;
		break;
	}	
	
  	
  }
  for (int i=2; i<= 10; i++){
  	G4double a0 =   Radius+ 0.5*DR/i;
	G4double a00 =  Radius+ 1.5*DR/i;
	G4double k0=std::sqrt(4.*rho*a0/( (rho+a0)*(rho+a00) + z1*z1));
	G4double k00=std::sqrt(4.*rho*a00/( (rho+a00)*(rho+a00) + z1*z1));
	G4cout<<k0<<'\t'<<k00<<std::endl;
	
	nR=i;
	G4double diff =(k0-k00)/k0;
	if (diff <0) diff=-diff; 
	if ( diff <0.005) {
		if ( diff <0.001 && nR==2) nR=1;
		break;
	}	
	
	
  	
  }
  G4cout<<nZ<<'\t'<<nR<<std::endl;
  
  
  //Compute the field as a sum of loops
  G4double Brho;
  G4double Bz;
  SumOfLoops(nZ, nR, rho, z, Brho, Bz);
  G4double r=pos.r();
  
  G4double Bx =0.;
  G4double By= 0.;
  if (rho>0.000000001) {
  	Bx=pos.x()*Brho/r;
	By=pos.y()*Brho/r;
  } 
  
  MagField[0] = Bx;
  MagField[1] = By;
  MagField[2] = Bz;
  G4cout<<Bx<<'\t'<<By<<'\t'<<Bz<<std::endl;
  
  
  
 
 
  
  
 
}
////////////////////////////////////////////////////////////////////////////////
// taken from http://www.netdenizen.com/emagnettest/offaxis/?offaxisloop
// a is the radius of the current loop
// i is the current of the curent loop
void CircularSolenoid::BCurrentloop(G4double a, G4double i,G4double rho, G4double z, G4double& Brho, G4double& Bz) const 
{ 
  G4cout<<"BCurrentloop "<<a<<'\t'<<i<<'\t'<<rho<<'\t'<<z<<std::endl;
  
  if (rho == 0.) { // on axis formula
  	G4double a2=a*a;
	Brho=i*a2/2./std::pow(a2+z*z,1.5);
	Bz=0.;
	return;
  }
  
  G4double alpha = rho/a;
  G4double alpha2 = alpha*alpha;
  G4double beta  = z/a;
  G4double beta2 = beta*beta;
  G4double beta2_plus_alpha2=alpha2+beta2;
  G4double gamma = z/rho;
  
  G4double Q= (1.+ alpha)* (1.+alpha) +  beta2;
  G4double k=std::sqrt(4.*alpha/Q);
  
  //elliptic integrals 
  G4double K,E;
  special_functions::CompleteEllipticIntegrals(k,K,E);
  
  //still need to put the  correct permeability
  
  G4double B0=i*CLHEP::mu0/2./a;
  G4double fac=B0/3.1415926/std::sqrt(Q);
  G4double Q_min_4alpha=Q-4.*alpha;
  Bz=fac*(E*(1.-beta2_plus_alpha2)/Q_min_4alpha + K);
  Brho=fac*gamma*(E*(1.+beta2_plus_alpha2)/Q_min_4alpha - K);
  G4cout<<"BCurrentloop1 "<<Brho<<'\t'<<Bz<<std::endl;
  
  G4cout<<"Elliptic "<<K<<'\t'<<E<<std::endl;
  k=1.-1.e-16;
  special_functions::CompleteEllipticIntegrals(k,K,E);
  G4cout<<"Elliptic "<<K<<'\t'<<E<<std::endl;
  
  

}
/////////////////////////////////////////////////////////////////////////////////////
//
void CircularSolenoid::SumOfLoops(G4int nr, G4int nz,G4double rho, G4double z, G4double& Brho, G4double& Bz) const
{ Brho=0.;
  Bz=0.;
  G4double icur;
  icur = I/(nr*nz);
  
  for (size_t i=0; i<size_t(nr);i++){
  	G4double a = Radius+ (i+0.5)*DR/nr;
	if ((rho-a)/a <0.001) a=a*(1.001);
	for (size_t j=0; j<size_t(nz);j++){
		G4double z0= - 0.5*DZ + (j+0.5)*DZ/nz;
		G4double Brho1,Bz1; 
		BCurrentloop(a, icur, rho,  z-z0, Brho1, Bz1);
		G4cout<<"SumOfLoops "<<Brho1<<'\t'<<Bz1<<std::endl;
		Brho+=Brho1;
		Bz+=Bz1;
	}
  }
  
  G4cout<<"SumOfLoops1 "<<Brho<<'\t'<<Bz<<std::endl;
}
