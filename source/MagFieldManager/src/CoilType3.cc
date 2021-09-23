#include"CoilType3.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"special_functions.hh"
#include"globals.hh"
#include"BiotSavartIntegrator.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;
////////////////////////////////////////////////////////////////////////////////
//
CoilType3::CoilType3(G4double RoutCorner,G4double D1, G4double D2,G4double Rin, G4double Rout,G4double Alpha0,G4double dAlpha,G4double current)
{ d1=D1;
  d2=D2;
  rout_corner=RoutCorner;
  rin_corner=rout_corner-d2;
  rin=Rin;
  rout=Rout;
  l=rout-rin-2.*rout_corner;
  alpha0=Alpha0;
  dalpha= dAlpha;
  G4double alpha1=alpha0+dalpha;
  cos_alpha0=std::cos(alpha0);
  sin_alpha0=std::sin(alpha0);
  cos_alpha1=std::cos(alpha1);
  sin_alpha1=std::sin(alpha1);
  
  y0_lin1= rout_corner-0.5*d2;
  z0_lin1 = rin+rout_corner+0.5*l;
  
  y1_lin2= -y0_lin1; 
  z1_lin2= z0_lin1;
  
  I=current;
  
  theBiotSavartIntegrator = BiotSavartIntegrator::GetInstance();
} 
////////////////////////////////////////////////////////////////////////////////
//
CoilType3::~CoilType3()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void CoilType3::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{  

   
   
   
   //coordinates in the system 0 and 1
   
   G4double x=yTrack[0];
   G4double y=yTrack[1];
   G4double z=yTrack[2];
   
   G4double y0=cos_alpha0*y+sin_alpha0*z; 
   G4double z0=cos_alpha0*z-sin_alpha0*y;
   
   G4double y1=cos_alpha1*y+sin_alpha1*z; 
   G4double z1=cos_alpha1*z-sin_alpha1*y;
   
   G4double half_d1=0.5*d1;
   G4double half_d2=0.5*d2;
   G4double half_l=0.5*l;
   
    
   
   

  //Field from the first linear section, the two big circular sections and the corner joining them  
  //-----------------------------------------------------------------------------------------------
  
  //The field is first computed in the coordinate system 0
 
  //first linear section;
  G4double pos[3];
  pos[0]=x;
  pos[1]=y0-y0_lin1;
  pos[2]=z0-z0_lin1;
  G4ThreeVector B1 = theBiotSavartIntegrator->BfieldFromLinearSegment(pos,half_d1, half_d2, half_l);
  
  //big outer circular section 
  pos[1]=y0;
  pos[2]=z0;
  G4double r=rout-d2;
  B1 += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, halfpi,  dalpha , rout, r, half_d1);
  
  //big inner circular section 
  pos[1]=y0;
  pos[2]=z0;
  r=rin+d2;
  B1 = B1 - theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, halfpi,  dalpha, r, rin, half_d1);
  
  //up corner
  pos[1]=y0;
  pos[2]=z0-(rout-rout_corner);
  B1 += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, 0.,  halfpi , rout_corner, rin_corner, half_d1);
  
  //down corner
  pos[1]=y0;
  pos[2]=z0-(rin+rout_corner);
  B1 += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, 1.5*pi,  halfpi , rout_corner, rin_corner, half_d1);
  
  
  //Here we go back to the normal system of coordinate

   G4ThreeVector B=  G4ThreeVector(B1.x(),B1.y()*cos_alpha0-B1.z()*sin_alpha0,B1.z()*cos_alpha0+B1.y()*sin_alpha0); 
  
  
  //Field from the  second linear section and the corner joining it to the big circular sections  
  //-----------------------------------------------------------------------------------------------
  
  //The field is first computed in the coordinate system 1
  
  //second linear section;
 
   pos[0]=x;
   pos[1]=y1-y1_lin2;
   pos[2]=z1-z1_lin2;
   B1 = -theBiotSavartIntegrator->BfieldFromLinearSegment(pos,half_d1, half_d2, half_l);
   
   //up corner
   pos[1]=y1;
   pos[2]=z1-(rout-rout_corner);
   B1 += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos,halfpi ,  halfpi , rout_corner, rin_corner, half_d1);
  
   //down corner
   pos[1]=y1;
   pos[2]=z1-(rin+rout_corner);
   B1 += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos,pi ,  halfpi , rout_corner, rin_corner, half_d1);
  
   //Here we go back to the normal system of coordinate and add the first part of the field

    B+=  G4ThreeVector(B1.x(),B1.y()*cos_alpha1-B1.z()*sin_alpha1,B1.z()*cos_alpha1+B1.y()*sin_alpha1); 
  
   
   
   
   
   //still need to multiply by the factor mu_0/4./pi
   G4double fac=mu0*I/4./pi;
   B=B*fac;
   
   
   
   MagField[0]=B.x();
   MagField[1]=B.y();
   MagField[2]=B.z(); 
   
 
}
