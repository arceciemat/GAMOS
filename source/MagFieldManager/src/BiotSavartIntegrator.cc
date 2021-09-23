#include"BiotSavartIntegrator.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"special_functions.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;

BiotSavartIntegrator* BiotSavartIntegrator::instance = 0;

////////////////////////////////////////////////////////////////////////////////
//
BiotSavartIntegrator::BiotSavartIntegrator()
{ nx =20;
  ny =20;
  nphi=20;
  CurvedSegmentTypeOfIntegration=0;
  nphi1=640;
  nphi2=160;
	
} 
////////////////////////////////////////////////////////////////////////////////
//
BiotSavartIntegrator::~BiotSavartIntegrator()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
BiotSavartIntegrator* BiotSavartIntegrator::GetInstance()
{ 
  if (!instance) instance = new BiotSavartIntegrator();
  return instance;
}
////////////////////////////////////////////////////////////////////////////////
// 
G4ThreeVector BiotSavartIntegrator::BfieldFromLinearSegment(const G4double pos[3],double lxx,  double lyy, double lzz)
{ 
  
  G4double x,y,z;
  x=pos[0];
  y=pos[1];
  z=pos[2];
  
  lx= lxx;
  ly= lyy;
  lz= lzz;
  G4double bx,by,bx1,by1;
  bx=0.;
  by=0.;
  
 /* if (LinearSegmentType1) {
  
  	G4double x0,d;
  	d=2.*lx/nx;
  	x0=-lx +d/2.;
  	beginning_of_integration = true;
  	for (size_t i=0;i<nx;i++){
  		x=pos[0]-x0;
  		bx+=BxFromYZCurrentSheet(x,y,z);
		x0+=d;
	
  	}
 	bx=bx/ly/2./nx;
  
  	G4double y0;
  	d=2.*ly/ny;
  	y0=-ly +d/2.;
  	x=pos[0];
  	beginning_of_integration = true;
  	for (size_t i=0;i<ny;i++){
  		y=pos[1]-y0;
  		by+=ByFromXZCurrentSheet(x,y,z);
		y0+=d;
  	}
	by=by/lx/2./ny;
  }	
  else { */

 	x=pos[0];
  	beginning_of_integration = true;
	int n_use=ny; //look if inside the
	double dy_max= 2.*lyy;
	if (std::abs(z)<lzz+dy_max &&  std::abs(x) <lxx+dy_max &&  std::abs(y)<dy_max){
		 n_use=	500;
		 
	
	
	}
	
	
	
	double d=2.*lxx/n_use;
  	double y0=-ly +d/2.;
  	for (int i=0;i<n_use;i++){
	  //GAMOS  	for (size_t i=0;i<n_use;i++){
  		y=pos[1]-y0;
  		BfieldFromXZCurrentSheet(x,y,z,bx1,by1);
		bx+=bx1;
		by+=by1;
		y0+=d;
	}
	bx/=n_use;
	by/=n_use;
	
		
  //}
  
  //Still need the factor mu0.I/4pi 

  return G4ThreeVector(bx,by,0.);
  
  
}
////////////////////////////////////////////////////////////////////////////////
// 
/*G4ThreeVector BiotSavartIntegrator::BfieldFromLinearSegment(const G4double pos[3],double lxx,  double lyy, double lzz)
{ //The term mu0I/4pi is not contained here
  
  G4double x,y,z;
  x=pos[0];
  y=pos[1];
  z=pos[2];
  
  lx= lxx;
  ly= lyy;
  lz= lzz;
  G4double bx,by,bx1,by1;
  bx=0.;
  by=0.;
 
  G4double y0;
  G4double d=2.*ly/ny;
  y0=-ly +d/2.;
  x=pos[0];
  beginning_of_integration = true;
  for (size_t i=0;i<ny;i++){
  	y=pos[1]-y0;
  	BfieldFromXZCurrentSheet(x,y,z,bx1,by1);
	bx+=bx1;
	by+=by1;
	y0+=d;
  }
  //Still need the factor 
  return G4ThreeVector(bx,by,0.)/double(ny);
  
  
}*/

////////////////////////////////////////////////////////////////////////////////
// 
/*G4ThreeVector BiotSavartIntegrator::BfieldFromCurvedSegmentInYZ(const G4double pos[3],double phi1,  double dphi,  //the segement is considered
								  size_t nphi,double Rout,double Rin,double dx)	//turning asround x from y to Z
{ G4double dp = dphi/nphi;
  G4double sdp = std::sin(dp/2.);
  G4double cdp = std::cos(dp/2.);
  G4double phi0 = phi1+dp/2.;
  G4double cp,sp;
  G4double x,y,z1,y1,y0,x0;
  G4double bx,by,bz,by1,bx2,by2;
  G4double dxx = 2.*dx/nx;
  x=pos[0];
  bx =0.;
  by =0.;
  bz =0.;
  G4double dr=(Rout-Rin)/ny;
  G4double r0,r;
  for (size_t i=0;i< nphi;i++) {
  	cp = std::cos(phi0);
	sp = std::sin(phi0);
	y1 = pos[1]*cp+pos[2]*sp; //position in the new coordinate system
	z1 = pos[2]*cp-pos[1]*sp;  // the new coordiante system is such that the section is along Z and the center is on y at r0*
	
	r0 = Rin + dr/2.;
	by1=0.;
  	for (size_t k=0; k< ny; k++) {
		y0 =  r0*cdp;
		lz =  r0*sdp;
		x0 = -dx + dxx/2.;
		y= y1-y0;
		beginning_of_integration = true;
		for (size_t l=0; l < nx; l++) {
			x= pos[0] - x0;
			BfFromIZLine(x, y, z1, bx2, by2);
			bx += bx2;
			by1 += by2;
			x0 += dxx;
			
		}
		r0 += dr;
		
	}
	by = by + by1*cp;
	bz = bz + by1*sp;
	phi0 = phi0 + dp;
  }
  return G4ThreeVector(bx,by,bz);
}
*/
////////////////////////////////////////////////////////////////////////////////
// 
G4ThreeVector BiotSavartIntegrator::BfieldFromCurvedSegmentInYZ_1(const G4double pos[3],double phi1,  double dphi,  //the segement is considered
								 double Rout,double Rin,double lxx)	//turning asround x from y to Z
{ G4double dp = dphi/nphi;
  G4double sdp = std::sin(dp/2.);
  G4double cdp = std::cos(dp/2.);
  G4double phi0 = phi1+dp/2.;
  G4double cp,sp;
  G4double x,y,z1,y1,y0,x0;
  G4double bx,by,bz,by1,bx2,by2;
  x=pos[0];
  bx =0.;
  by =0.;
  bz =0.;
  G4double dr=(Rout-Rin)/ny;
  G4double r0;
  //GAMOS  G4double r0,r;
  lx=lxx;
  G4double dx= 2.*lx/nx;
  for (int i=0;i< nphi;i++) {
    //GAMOS  for (size_t i=0;i< nphi;i++) {
  	cp = std::cos(phi0);
	sp = std::sin(phi0);
	y1 = pos[1]*cp+pos[2]*sp; //position in the new coordinate system
	z1 = pos[2]*cp-pos[1]*sp;  // the new coordiante system is such that the section is along Z and the center is on y at r0*
	
	r0 = Rin + dr/2.;
	by1=0.;
  	for (int k=0; k< ny; k++) {
	  //GAMOS  	for (size_t k=0; k< ny; k++) {
		y0 =  r0*cdp;
		lz =  r0*sdp;
		y= y1-y0;
		
		
		beginning_of_integration = true;
		if (CurvedSegmentTypeOfIntegration == 1) {
			BfieldFromXZCurrentSheet(x, y, z1, bx2, by2); //the x integration disapears
			bx += bx2;
			by1 += by2;
		}
		else {	
			x0 = -lx + dx/2.;
			for (int l=0; l < nx; l++) {
			  //GAMOS			for (size_t l=0; l < nx; l++) {
				x= pos[0] - x0;
				BfFromIZLine(x, y, z1, bx2, by2);
				bx += bx2;
				by1 += by2;
				x0 += dx;
			}
		}	
		
		r0 += dr;
		
	}
	by = by + by1*cp;
	bz = bz + by1*sp;
	phi0 = phi0 + dp;
	
  }
  G4double factor=1./double(ny);
  if (CurvedSegmentTypeOfIntegration >1) factor*=1./double(nx);
  return G4ThreeVector(bx,by,bz)*factor;
}

////////////////////////////////////////////////////////////////////////////////
//
G4ThreeVector BiotSavartIntegrator::BfieldFromCurvedSegmentInYZ(const G4double pos[3],double phi1,  double dphi,  //the segment is considered
								  double Rout,double Rin,double lxx)	//turning around x from y to Z
{ 
 return BiotSavartIntegrator::BfieldFromCurvedSegmentInYZ_test(pos, phi1, dphi, 
								  Rout, Rin, lxx);
  
  
  
  if (CurvedSegmentTypeOfIntegration !=0) return BiotSavartIntegrator::BfieldFromCurvedSegmentInYZ_1(pos, phi1, dphi, 
								  Rout, Rin, lxx);
 
 //First check if the point where to compute B is very close to the segment if yes then use the high precision nphi or the mid precision one
  double nphi_to_use=nphi ;
  G4double dmax=20.*std::max(Rout-Rin,lxx);
  if (std::abs(pos[0]) < dmax) { //need to check the other coordinates
  	
	double r=std::sqrt(pos[1]*pos[1] + pos[2]*pos[2]);
	double rline=(Rout+Rin)/2.;
	if (std::abs(r-rline) < dmax) { //check phi
		double dphi_max= dmax/rline;
		double phi_point=std::atan2(pos[2],pos[1]);
		if (phi_point <0.) phi_point=twopi+phi_point;
		double phi0=phi1;
		if (phi0 <0.) phi0=twopi+phi0;
		double delta_phi = phi_point-phi0;
		if (delta_phi <0) delta_phi = delta_phi+twopi;
		if (delta_phi<dphi+dphi_max) {
			nphi_to_use=nphi2 ;
		        dmax=dmax/4.;
			if (std::abs(r-rline) < dmax/4. && std::abs(r-rline) < dmax) {//high precision check
				nphi_to_use=nphi1 ;
			}
		}	
	}	
  
  }
  
  
 
 
 
 
 
  /*G4cout<<"Method correct"<<std::endl;
  G4cout<<nphi<<std::endl;*/
  G4double dp = dphi/nphi_to_use;
  G4double sindp = std::sin(dp);
  G4double cosdp = std::cos(dp);
  G4double phi0 = phi1 + dp/2.;
  G4double cosp,sinp,cosp1,sinp1;
  cosp = std::cos(phi0);
  sinp = std::sin(phi0);
 
  G4double x,y,z;
  x=pos[0];
  y=pos[1];
  z=pos[2];
  
  G4double bx,by,bz,dBxdPhi,dBrhoPrimdPhi;
  bx =0.;
  by =0.;
  bz =0.;
  dBxdPhi=0.;
  dBrhoPrimdPhi=0;
  rhoout= Rout;
  rhoin= Rin;
  lx=lxx;
  beginning_of_integration = true;
 // G4cout<<y<<'\t'<<z<<std::endl;
  
  for (size_t i=0;i< nphi_to_use;i++) {
  
  	//G4cout<<"I :"<<i<<std::endl;
        aa=y*cosp+z*sinp;
	bb=y*sinp-z*cosp;
	dBfieldFromCurvedSegmentInYZdPhiPrime(x, y,z, dBxdPhi,dBrhoPrimdPhi);
	//G4cout<<dBxdPhi<<std::endl;
	bx += dBxdPhi;
	//G4cout<<bx<<std::endl;
	by += cosp*dBrhoPrimdPhi;
	bz += sinp*dBrhoPrimdPhi;
	cosp1=cosp*cosdp-sinp*sindp; 
	sinp1=cosp*sindp+sinp*cosdp;
	cosp=cosp1;
	sinp=sinp1;
	phi0+=dp;
	/*cosp = std::cos(phi0);
        sinp = std::sin(phi0);*/
	
  }
  G4double factor=dphi/double(nphi_to_use)/lx/2./(Rout-Rin);
 
  return G4ThreeVector(bx,by,bz)*factor;
}
////////////////////////////////////////////////////////////////////////////////
//
G4ThreeVector BiotSavartIntegrator::BfieldFromCurvedSegmentInYZ(const G4double pos[3],double phi1,  double dphi,  //the segment is considered
								  double Rout,double Rin,double lxx, int nphi_to_use)	//turning around x from y to Z
{ 
 
 
 
  if (CurvedSegmentTypeOfIntegration !=0) return BiotSavartIntegrator::BfieldFromCurvedSegmentInYZ_1(pos, phi1, dphi, 
								  Rout, Rin, lxx);
 

  /*G4cout<<"Method correct"<<std::endl;
  G4cout<<nphi<<std::endl;*/
  G4double dp = dphi/nphi_to_use;
  G4double sindp = std::sin(dp);
  G4double cosdp = std::cos(dp);
  G4double phi0 = phi1 + dp/2.;
  G4double cosp,sinp,cosp1,sinp1;
  cosp = std::cos(phi0);
  sinp = std::sin(phi0);
 
  G4double x,y,z;
  x=pos[0];
  y=pos[1];
  z=pos[2];
  G4double rho= std::sqrt(y*y+z*z);
  
  G4double bx,by,bz,dBxdPhi,dBrhoPrimdPhi;
  bx =0.;
  by =0.;
  bz =0.;
  dBxdPhi=0.;
  dBrhoPrimdPhi=0;
  rhoout= Rout;
  rhoin= Rin;
  lx=lxx;
  beginning_of_integration = true;
 // G4cout<<y<<'\t'<<z<<std::endl;
  
  for (int i=0;i< nphi_to_use;i++) {
    //GAMOS  for (size_t i=0;i< nphi_to_use;i++) {
  
  	//G4cout<<"I :"<<i<<std::endl;
        aa=y*cosp+z*sinp;
	bb=y*sinp-z*cosp;
	if (std::abs(bb)>0.00001) {
		dBfieldFromCurvedSegmentInYZdPhiPrime(x, y,z, dBxdPhi,dBrhoPrimdPhi);
	}
	else { //to avoid possible discontinuity
		 aa=rho*std::cos(dp/2/10.);
		 bb=rho*std::sin(dp/2/10.);
		 G4double dBxdPhi1,dBrhoPrimdPhi1;
		 dBfieldFromCurvedSegmentInYZdPhiPrime(x, y,z, dBxdPhi1,dBrhoPrimdPhi1); 
		 dBxdPhi=dBxdPhi1;
  		 dBrhoPrimdPhi=dBrhoPrimdPhi1;
		 bb=-bb;
		 dBfieldFromCurvedSegmentInYZdPhiPrime(x, y,z, dBxdPhi1,dBrhoPrimdPhi1); 
		 dBxdPhi+=dBxdPhi1;
  		 dBrhoPrimdPhi+=dBrhoPrimdPhi1;
		 //GAMOS		 dBxdPhi/2.;	       
		 //GAMOS		 dBrhoPrimdPhi/2.;
		 dBxdPhi/=2.;
		 dBrhoPrimdPhi/=2.;
		
		
	}
	//G4cout<<dBxdPhi<<std::endl;
	bx += dBxdPhi;
	//G4cout<<bx<<std::endl;
	by += cosp*dBrhoPrimdPhi;
	bz += sinp*dBrhoPrimdPhi;
	cosp1=cosp*cosdp-sinp*sindp; 
	sinp1=cosp*sindp+sinp*cosdp;
	cosp=cosp1;
	sinp=sinp1;
	phi0+=dp;
	/*cosp = std::cos(phi0);
        sinp = std::sin(phi0);*/
	
  }
  G4double factor=dphi/double(nphi_to_use)/lx/2./(Rout-Rin);
 
  return G4ThreeVector(bx,by,bz)*factor;
}

////////////////////////////////////////////////////////////////////////////////
//
G4ThreeVector BiotSavartIntegrator::BfieldFromCurvedSegmentInYZ_test(const G4double pos[3],double phi1,  double dphi,  //the segment is considered
								  double Rout,double Rin,double lxx)	//turning around x from y to Z
{
 
 
 
 G4double p_min=phi1;
 if (p_min <0) p_min +=twopi;
 G4double p_max=p_min+dphi;
 
 

 double phi_point = std::atan2(pos[2],pos[1]);
 if (phi_point <0.) phi_point=twopi+phi_point;
 double rho_point = std::sqrt(pos[1]*pos[1] + pos[2]*pos[2]);
 double rline=(Rout+Rin)/2.;
 double dist_min= std::sqrt((rline-rho_point)*(rline-rho_point) + pos[0]*pos[0]);
 double p_high_min=0.;
 double dp_high=0.;
 double p_mid_min=0.;
 double dp_mid=0.;
  
 
 //Check if in [p_min,p_max] a  section is considered very close to  the point
 //-------------------------------------------------------------------------
 
 G4double dmax=15.*std::max(Rout-Rin,lxx);
 if (dist_min< dmax) {//then potential candidates for mid precision
 	if ( (rline-rho_point)/rline >0.99) {//integration with mid or high precision on all the section 
		p_mid_min=p_min;
		dp_mid=dphi;
	}
	else {
		double cos_delta=(dmax*dmax-dist_min*dist_min-2.*rho_point*rline)/2./rho_point/rline;
		if (cos_delta> 1. || cos_delta<-1){//integration with mid or high precision on all the section 
			p_mid_min=p_min;
			dp_mid=dphi;
		}
		else {
			double delta=std::acos(cos_delta);
			p_mid_min=std::max(phi_point-delta, p_min);
			dp_mid =std::min(p_max-p_mid_min,phi_point+delta-p_mid_min);
		}	 
	}
	dmax=5.*std::max(Rout-Rin,lxx);
	if (dist_min< dmax && dp_mid >0 ) {//possibility of high precision 
		double cos_delta=(dmax*dmax-dist_min*dist_min-2.*rho_point*rline)/rho_point/rline;
		if (cos_delta> 1. || cos_delta<-1){ 
			p_high_min=p_mid_min;
			dp_high=dp_mid;
		}
		else {
			double delta=std::acos(cos_delta);
			p_high_min=std::max(phi_point-delta, p_mid_min);
			double p_mid_max=p_mid_min+dp_mid;
			dp_high =std::min(p_mid_max-p_high_min,phi_point+delta-p_high_min);
		}
		
		
		
	}
	 	
 
 } 
 G4ThreeVector Bvec=G4ThreeVector(0.,0.,0.);

 int nphi_low_prec=nphi;
 Bvec =BfieldFromCurvedSegmentInYZ(pos,p_min, dphi, Rout,Rin,lxx, nphi_low_prec);
 return Bvec;
 if (dp_mid <=0){ // no high precision section
 	Bvec =BfieldFromCurvedSegmentInYZ(pos,p_min, dphi, Rout,Rin,lxx, nphi_low_prec);
 }
 else {
 	int nphi_high_prec=int(640.*dp_high/halfpi)+1;
	int nphi_mid_prec=int(160.*dp_mid/halfpi)+1;
	if (dp_mid  == dphi)  {
		if (dp_high  == dp_mid) Bvec= BfieldFromCurvedSegmentInYZ(pos,p_min, dphi, Rout,Rin,lxx, nphi_high_prec);
		else if (dp_high <=0) Bvec= BfieldFromCurvedSegmentInYZ(pos,p_min, dphi, Rout,Rin,lxx, nphi_mid_prec);
		else {
			if (p_mid_min<p_high_min)
				Bvec += BfieldFromCurvedSegmentInYZ(pos,p_mid_min,p_high_min-p_mid_min , Rout,Rin,lxx, nphi_mid_prec);
			
			Bvec += BfieldFromCurvedSegmentInYZ(pos,p_high_min,dp_high, Rout,Rin,lxx, nphi_high_prec);
			double p_high_max=p_high_min+dp_high;
			double p_mid_max=p_mid_min+dp_mid;
			
			if (p_mid_max>p_high_max)
				Bvec += BfieldFromCurvedSegmentInYZ(pos,p_high_max,p_mid_max-p_high_max, Rout,Rin,lxx, nphi_mid_prec);
			
		
		}
		
	}
	else {
		if (p_min<p_mid_min)
			Bvec += BfieldFromCurvedSegmentInYZ(pos,p_min,p_mid_min-p_min , Rout,Rin,lxx, nphi_low_prec);
		double p_mid_max=p_mid_min+dp_mid;
		
		if (dp_high  == dp_mid) Bvec+= BfieldFromCurvedSegmentInYZ(pos,p_high_min, dp_high, Rout,Rin,lxx, nphi_high_prec);
		else if (dp_high <=0) Bvec+= BfieldFromCurvedSegmentInYZ(pos,p_mid_min, dp_mid, Rout,Rin,lxx, nphi_mid_prec);
		else {
			if (p_mid_min<p_high_min)
				Bvec += BfieldFromCurvedSegmentInYZ(pos,p_mid_min,p_high_min-p_mid_min, Rout,Rin,lxx, nphi_mid_prec);
			
			Bvec += BfieldFromCurvedSegmentInYZ(pos,p_high_min,dp_high, Rout,Rin,lxx, nphi_high_prec);
			double p_high_max=p_high_min+dp_high;
			
			
			if (p_mid_max>p_high_max)
				Bvec += BfieldFromCurvedSegmentInYZ(pos,p_high_max,p_mid_max-p_high_max, Rout,Rin,lxx, nphi_mid_prec);
			
		
		}	
		
		
		if (p_max>p_mid_max){
			Bvec += BfieldFromCurvedSegmentInYZ(pos,p_mid_max,p_max-p_mid_max, Rout,Rin,lxx, nphi_low_prec);
		}
	
	
	}
 }	
 return Bvec;
 
 
 
  
 
}
//////////////////////////////////////////////////////////////////////////////////
//
void BiotSavartIntegrator::BfFromIZLine(double& x, double& y,double& z, double& bx, double& by)
{ 
  if (beginning_of_integration){ //variabels kept constant during x integration
  
  	zm=z-lz;
  	zm2=zm*zm;
  	zp=z+lz;
  	zp2=zp*zp;
	y2=y*y;
  }
  G4double rho2= x*x + y2 ;
  if (rho2<mm2) rho2 = mm2;  
  beginning_of_integration =false;	
  by = zm/std::sqrt(rho2+zm2) - zp/std::sqrt(rho2+zp2);
  by /= rho2;
  bx = y*by;
  by =-x*by;
  
}
//////////////////////////////////////////////////////////////////////////////////
//
G4double BiotSavartIntegrator::BfieldFromXZCurrentSheet(double& x, double& y,double& z, double& bx, double& by)
{G4double y2t= y*y;
 if (beginning_of_integration){
	 zm=z-lz;
 	 zm2=zm*zm;
 	 zp=z+lz;
 	 zp2=zp*zp;
 	 xm=x-lx;
 	 xm2=xm*xm;
 	 xp=x+lx;
 	 xp2=xp*xp;
	 rhomm2 = xm2 +zm2;
	 if (rhomm2<mm2) rhomm2 = mm2;
	 rhomp2 = xm2 +zp2;
	 if (rhomp2<mm2) rhomp2 = mm2;
	 rhopm2 = xp2 +zm2;
	 if (rhopm2<mm2) rhopm2 = mm2;
	 rhopp2 = xp2 +zp2;
	 if (rhopp2<mm2) rhopp2 = mm2;
	 xpzm =xp*zm;
	 xmzp =xm*zp;
	 xmzm =xm*zm;
	 xpzp =xp*zp;
 }
 beginning_of_integration = false;

 
 G4double rmm=std::sqrt(rhomm2+y2t);
 G4double rpm=std::sqrt(rhopm2+y2t);
 G4double rmp=std::sqrt(rhomp2+y2t);
 G4double rpp=std::sqrt(rhopp2+y2t);
 
 //Bx
 
 bx = std::atan(-xmzm/rmm/y)- std::atan(-xpzm/rpm/y)- std::atan(-xmzp/rmp/y) + std::atan(-xpzp/rpp/y);
 bx=bx/lx/2.;

 G4double f1=(rmm-zm)*(rpm+zm)*(rmp+zp)*(rpp-zp);
 G4double f2=(rmm+zm)*(rpm-zm)*(rmp-zp)*(rpp+zp);
 by = std::log(f1/f2)/4./lx;
 //by = 0.;
 return 0.; // GAMOS
}
//////////////////////////////////////////////////////////////////////////////////
//
G4double BiotSavartIntegrator::BxFromYZCurrentSheet(double& x,double& y,double& z)
{ 
 G4double x2t= x*x;
 if (beginning_of_integration){
	 zm=z-lz;
 	 zm2=zm*zm;
 	 zp=z+lz;
 	 zp2=zp*zp;
 	 ym=y-ly;
 	 ym2=ym*ym;
 	 yp=y+ly;
 	 yp2=yp*yp;
 }	 
 beginning_of_integration = false;
 G4double rhom2=x2t+ym2;
 if (rhom2<mm2) rhom2 = mm2; 
 G4double rhop2=x2t+yp2;
 if (rhop2<mm2) rhop2 = mm2;
 G4double rmm=std::sqrt(rhom2+zm2);
 G4double rpm=std::sqrt(rhop2+zm2);
 G4double rmp=std::sqrt(rhom2+zp2);
 G4double rpp=std::sqrt(rhop2+zp2);
 G4double f1=(rmm+zm)*(rpm-zm)*(rmp-zp)*(rpp+zp);
 G4double f2=(rmm-zm)*(rpm+zm)*(rmp+zp)*(rpp-zp);
 return std::log(f1/f2)/2.;
}
//////////////////////////////////////////////////////////////////////////////////
//
G4double BiotSavartIntegrator::ByFromXZCurrentSheet(double& x,double& y,double& z)
{ G4double xx,yy;
  xx=-y;
  yy=x;
  return -BxFromYZCurrentSheet(xx,yy,z);
}
//////////////////////////////////////////////////////////////////////////////////
//
G4double BiotSavartIntegrator::dBfieldFromCurvedSegmentInYZdPhiPrime(double& x, double& ,double& , double& dBxdPhi, double& dBrhoPrimdPhi)
{if (beginning_of_integration){
	 xu=lx-x;
 	 xu2=xu*xu;
 	 xd=-x-lx;
 	 xd2=xd*xd;
 }

 
 beginning_of_integration = false;
 xu=lx-x;
 xu2=xu*xu;
 xd=-x-lx;
 xd2=xd*xd;
 
 //G4cout<<a<<std::endl;
 G4double rhooutma=rhoout-aa;
 G4double rhooutma2=rhooutma*rhooutma;
 G4double rhoinma=rhoin-aa;
 G4double rhoinma2=rhoinma*rhoinma;
 G4double b2=bb*bb;
 
 G4double routu=std::sqrt(rhooutma2+xu2+b2);
 if (routu<mm) routu=mm;
 G4double routd=std::sqrt(rhooutma2+xd2+b2);
 if (routd<mm) routd=mm;
 G4double rinu=std::sqrt(rhoinma2+xu2+b2);
 if (rinu<mm) rinu=mm;
 G4double rind=std::sqrt(rhoinma2+xd2+b2);
 if (rind<mm) rind=mm;
 
 G4double t1outu=rhooutma+routu;
 //if (t1outu ==0 ) t1outu = 0.0000000001;
 G4double t1outd=rhooutma+routd;
 //if (t1outd ==0 ) t1outd = 0.0000000001;
 G4double t1inu=rhoinma+rinu;
 //if (t1inu ==0 ) t1inu =   0.0000000001;
 G4double t1ind=rhoinma+rind;
 //if (t1ind ==0 ) t1inu =   0.0000000001;
 //G4cout<<"t1 "<<t1outu<<'\t'<<t1outd<<'\t'<<t1inu<<'\t'<<t1ind<<std::endl;
 
 G4double t1u=t1outu/t1inu;
 G4double t1d=t1outd/t1ind;
 G4double t1=t1u/t1d;
 
 
 
 
 
 G4double t2outu = routu+xu;
 G4double t2outd = routd+xd;
 G4double t2inu  = rinu+xu;
 G4double t2ind = rind+xd;
 
 //if (t2outu ==0 ) t2outu = 0.0000000001;
 //if (t2outd ==0 ) t2outd = 0.0000000001;
 //if (t2inu ==0 ) t2inu =   0.0000000001;
 //if (t2ind ==0 ) t2inu =   0.0000000001;
 
 G4double t2=t2outu*t2ind/t2outd/t2inu;
 
  
 dBxdPhi=xu*std::log(t1u)- xd*std::log(t1d)-aa*std::log(t2);
 //dBxdPhi+=-aa*std::log(t2);
 dBxdPhi+=-bb*(std::atan(rhooutma*xu/routu/bb)- std::atan(rhooutma*xd/routd/bb)- std::atan(rhoinma*xu/rinu/bb) + std::atan(rhoinma*xd/rind/bb));
 dBrhoPrimdPhi=routu+rind-routd-rinu+aa*std::log(t1);
 //aa*std::log(t1);
 //G4cout<<"dBxdPhi "<<dBxdPhi<<std::endl;
 
 
 return 0.; //GAMOS
} 
