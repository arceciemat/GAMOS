#include"special_functions.hh"
#include"globals.hh"
#include"G4ios.hh"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Carlson Elliptic integral of the first kind see Num. Recip. C++ 2nd Edit. p.268
//
double special_functions::rf(double x, double y, double z)
{  
   G4cout<<"XYZ "<<x<<'\t'<<y<<'\t'<<z<<std::endl;
   const double errtol=0.0025, tiny=1.5e-90, big=3.0e37, third=1.0/3.0;
   const double c1=1.0/24.0, c2=0.1, c3=3.0/44.0, c4=1.0/14.0;
   double alamb,ave,delx,dely,delz,e2,e3,sqrtx,sqrty,sqrtz,xt,yt,zt;
   
   double min1 = std::min( std::min(x,y),z);
   double min2 = std::min( std::min(x+y,x+z),y+z);
   double max1 = std::max( std::max(x,y),z); 
   if (min1 < 0.0 || min2 < tiny || max1 > big){
   	std::cout<<"Invalid arguments for the function special_functions::rf"<<std::endl;
   	return -99999.;
   }
			
   xt=x;
   yt=y;
   zt=z;
   do {
	sqrtx=std::sqrt(xt);
	sqrty=std::sqrt(yt);
	sqrtz=std::sqrt(zt);
	alamb=sqrtx*(sqrty+sqrtz)+sqrty*sqrtz;
	xt=0.25*(xt+alamb);
	yt=0.25*(yt+alamb);
	zt=0.25*(zt+alamb);
	ave=third*(xt+yt+zt);
	delx=(ave-xt)/ave;
	dely=(ave-yt)/ave;
	delz=(ave-zt)/ave;
   } while (std::max(std::max(std::abs(delx),std::abs(dely)),std::abs(delz)) > errtol);
   
   e2=delx*dely-delz*delz;
   e3=delx*dely*delz;
   return (1.0+(c1*e2-c2-c3*e3)*e2+c4*e3)/std::sqrt(ave);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Carlson Elliptic integral of the second kind see Num. Recip. C++ 2nd Edit. p.269
//
double special_functions::rd(double x, double y, double z)
{   const double errtol=0.0015, tiny=1.0e-90, big=4.5e21;
    const double c1=3.0/14.0, c2=1.0/6.0, c3=9.0/22.0;
    const double c4=3.0/26.0, c5=0.25*c3, c6=1.5*c4;
    double alamb,ave,delx,dely,delz,ea,eb,ec,ed,ee,fac,sqrtx,sqrty,
	   sqrtz,sum,xt,yt,zt;

    double min1=std::min(x,y);
    double min2=std::min(x+y,y);;
    double max1=std::max(std::max(x,y),z);
    if (min1 < 0.0 || min2 < tiny || max1 > big){
   	std::cout<<"Invalid arguments for the function special_functions::rd"<<std::endl;
   	return -99999.;
    }
   
    xt=x;
    yt=y;
    zt=z;
    sum=0.0;
    fac=1.0;
    do {
	sqrtx=std::sqrt(xt);
	sqrty=std::sqrt(yt);
	sqrtz=std::sqrt(zt);
	alamb=sqrtx*(sqrty+sqrtz)+sqrty*sqrtz;
	sum += fac/(sqrtz*(zt+alamb));
	fac=0.25*fac;
	xt=0.25*(xt+alamb);
	yt=0.25*(yt+alamb);
	zt=0.25*(zt+alamb);
	ave=0.2*(xt+yt+3.0*zt);
	delx=(ave-xt)/ave;
	dely=(ave-yt)/ave;
	delz=(ave-zt)/ave;
    } while (std::max(std::max(std::abs(delx),std::abs(dely)),std::abs(delz)) > errtol);
    
    ea=delx*dely;
    eb=delz*delz;
    ec=ea-eb;
    ed=ea-6.0*eb;
    ee=ed+ec+ec;
    return 3.0*sum+fac*(1.0+ed*(-c1+c5*ed-c6*delz*ee)
		+delz*(c2*ee+delz*(-c3*ec+delz*c4*ea)))/(ave*std::sqrt(ave));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Complete elliptic integral of the 1st kind  see Num. Recip. C++ 2nd Edit. p.272
//
double special_functions::CompleteEllipticIntegral1stKindK(double m)
{   return rf(0.,1.-m*m,1.);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Complete elliptic integral of the 2nd kind  see Num. Recip. C++ 2nd Edit. p.272
//
double special_functions::CompleteEllipticIntegral2ndKindE(double m)
{   double m2 =m*m;
    double y=1.-m*m;
    double res= special_functions::rf(0.,y,1.) - m2*special_functions::rd(0.,y,1.)/3.; 
    return res;	 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Complete elliptic integrals of 1st kind K and 2nd kind E
//
void special_functions::CompleteEllipticIntegrals(double m, double& K, double& E)
{   double m2 =m*m;
    double y=1.-m2;
    K =special_functions::rf(0.,y,1.);
    E= K- m2*special_functions::rd(0.,y,1.)/3.; 
  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Bfield at position from a current line parallel to z
//
// input:	 r[3] position  where b should be computed
//		 o[3] center of line	
//		 2l = length of line
//		
// output:	 bfield // the bfield obtained is divied by mu_0*I/4pi 
//		
void special_functions::bfield_from_IZ_line(double r[3], double o[3],double l, double  bf[3])
{
 
 double x= r[0]-o[0];
 double y= r[1]-o[1];
 double z= r[2]-o[2];
 double rho2= x*x + y*y ;
 double za=z-l;
 double za2=za*za;
 double zb=z+l;
 double zb2=zb*zb;
 
 double fac = za/std::sqrt(rho2+za2) - zb/std::sqrt(rho2+zb2);
 fac /=rho2;
 bf[0]=-y*fac;
 bf[1]=x*fac;
 bf[2]=0;
 
}
void special_functions::bfield_from_IY_line(double r[3], double o[3],double l, double  bf[3])
{//Method used we set  a new coordinate system x',y'z' where x' is z, y' is x and z' is y

 double y1= r[0]-o[0];
 double z1= r[1]-o[1];
 double x1= r[2]-o[2];
 double rho2= x1*x1 + y1*y1 ;
 double za=z1-l;
 double za2=za*za;
 double zb=z1+l;
 double zb2=zb*zb;
 
 double fac = za/std::sqrt(rho2+za2) - zb/std::sqrt(rho2+zb2);
 fac /=rho2;
 //Back to x,y,z coordinate system
 bf[0]=x1*fac;
 bf[1]=0;
 bf[2]=-y1*fac;
 
}

void special_functions::bfield_from_IX_line(double r[3], double o[3],double l, double  bf[3])
{//Method used we set  a new coordinate system x',y'z' where x' is y, y' is z and z' is x

 double z1= r[0]-o[0];
 double x1= r[1]-o[1];
 double y1= r[2]-o[2];
 double rho2= x1*x1 + y1*y1 ;
 double za=z1-l;
 double za2=za*za;
 double zb=z1+l;
 double zb2=zb*zb;
 
 double fac = za/std::sqrt(rho2+za2) - zb/std::sqrt(rho2+zb2);
 fac /=rho2;
 //Back to x,y,z coordinate system
 bf[0]=0;
 bf[1]=-y1*fac;
 bf[2]=x1*fac;
 
}
void special_functions::bx_from_IYZ_surface(double r[3],double o[3],double ly,double lz,  double& bx)
{
 double x= r[0]-o[0];
 double y= r[1]-o[1];
 double z= r[2]-o[2];
 double x2= x*x;
 
 double zm=z-lz;
 double zm2=zm*zm;
 double zp=z+lz;
 double zp2=zp*zp;
 
 double ym=y-ly;
 double ym2=ym*ym;
 double yp=y+ly;
 double yp2=yp*yp;
 
 double rhom2=x2+ym2;
 double rhop2=x2+yp2;
 
 double rmm=std::sqrt(rhom2+zm2);
 double rpm=std::sqrt(rhop2+zm2);
 double rmp=std::sqrt(rhom2+zp2);
 double rpp=std::sqrt(rhop2+zp2);
 
 
 
 
 
 double f1=(rmm+zm)*(rpm-zm)*(rmp-zp)*(rpp+zp);
 double f2=(rmm-zm)*(rpm+zm)*(rmp+zp)*(rpp-zp);
 bx=std::log(f1/f2)/2.;
 
 
 
}
void special_functions::by_from_IXZ_surface(double r[3],double o[3],double lx,double lz,  double& by)
{

 double x= r[0]-o[0];
 double y= r[1]-o[1];
 double z= r[2]-o[2];
 double y2= y*y;
 
 double zm=z-lz;
 double zm2=zm*zm;
 double zp=z+lz;
 double zp2=zp*zp;
 
 double xm=x-lx;
 double xm2=xm*xm;
 double xp=x+lx;
 double xp2=xp*xp;
 
 double rhom2=y2+xm2;
 double rhop2=y2+xp2;
 
 double rmm=std::sqrt(rhom2+zm2);
 double rpm=std::sqrt(rhop2+zm2);
 double rmp=std::sqrt(rhom2+zp2);
 double rpp=std::sqrt(rhop2+zp2);

 double f1=(rmm+zm)*(rpm-zm)*(rmp-zp)*(rpp+zp);
 double f2=(rmm-zm)*(rpm+zm)*(rmp+zp)*(rpp-zp);
 
 
 by=std::log(f2/f1)/2.;
 
 
 
}
