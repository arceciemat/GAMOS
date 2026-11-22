#include"CoilType2.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"special_functions.hh"
#include"globals.hh"
#include"BiotSavartIntegrator.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;
////////////////////////////////////////////////////////////////////////////////
//
CoilType2::CoilType2(G4double RoutCorner, G4double D1, G4double D2, G4double LVertLeft, 
    	      G4double LVertRight, G4double LHorizUp, G4double LHorizDown,G4double current)//current turn from y to z
{ 

 
 
  d1=D1;
  d2=D2;
  Rout_corner = RoutCorner;
  Rin_corner = RoutCorner-d2;
  Lvert_left = LVertLeft;
  Lvert_right = LVertRight;
  Lvert_mid = Lvert_left + d2 - Lvert_right -2.* Rout_corner;
  Lhoriz_up = LHorizUp;
  Lhoriz_down = LHorizDown;
  Lhoriz_mid = Lhoriz_up + d2 - Lhoriz_down -2. * Rout_corner;
  
  
  y_vert_left = -0.5*LHorizUp - Rout_corner +0.5*d2;
  y_vert_right = -y_vert_left;
  y_vert_mid  = y_vert_right - 2.*Rout_corner - Lhoriz_mid + d2;
  
  z_vert_left = 0.;
  z_vert_right =0.5*Lvert_left - 0.5*Lvert_right; 
  z_vert_mid = -0.5*Lvert_left + 0.5*Lvert_mid;

  y_horiz_up = 0;
  y_horiz_down = -0.5*Lhoriz_up + 0.5*Lhoriz_down;
  y_horiz_mid = 0.5*Lhoriz_up - 0.5*Lhoriz_mid;
 
  z_horiz_up = 0.5*LVertLeft + Rout_corner -0.5*d2;
  z_horiz_down = -z_horiz_up;
  z_horiz_mid =  z_horiz_up + d2 - 2.*Rout_corner -Lvert_right ;
  
  
  
  y_corner_left_up =  y_vert_left - 0.5*d2 + Rout_corner ;
  y_corner_right_up = -y_corner_left_up;
  y_corner_right_mid = y_corner_right_up;
  y_corner_mid_mid =  y_vert_mid - 0.5*d2 + Rout_corner;
  y_corner_mid_down  =  y_vert_mid + 0.5*d2 - Rout_corner;
  y_corner_left_down = y_corner_left_up;
  
  z_corner_left_up = z_horiz_up + 0.5*d2 - Rout_corner ;
  z_corner_right_up = z_corner_left_up;
  z_corner_right_mid = z_corner_right_up - Lvert_right;
  z_corner_mid_mid = z_horiz_mid +0.5*d2- Rout_corner ;
  z_corner_mid_down = -z_corner_left_up;
  z_corner_left_down = z_corner_mid_down;

  
  I=current;
  

  theBiotSavartIntegrator = BiotSavartIntegrator::GetInstance();
} 
////////////////////////////////////////////////////////////////////////////////
//
CoilType2::~CoilType2()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void CoilType2::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ 
  G4ThreeVector B = G4ThreeVector(0,0,0); 
 
  //right vertical linear section;
  G4double pos[3];
  pos[0]=yTrack[0];
  pos[1]=yTrack[1]-y_vert_right;
  pos[2]=yTrack[2]-z_vert_right;
 /* G4cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<std::endl;
  G4cout<<0.5*d1<<" "<<0.5*d2<<" "<<0.5*Lvert_right<<std::endl;*/
  B += theBiotSavartIntegrator->BfieldFromLinearSegment(pos,0.5*d1, 0.5*d2, 0.5*Lvert_right);
 // G4cout<<B<<" 1"<<std::endl;
  
  //Upper right corner;
   pos[1]=yTrack[1]-y_corner_right_up;
   pos[2]=yTrack[2]-z_corner_right_up; 
   B += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, 0.,  halfpi , Rout_corner, Rin_corner, 0.5*d1);
   //G4cout<<B<<" 2"<<std::endl;
   
  //up horizontal linear section; //we first work in the system where z' =-y and y'=z
  pos[2]=-(yTrack[1]-y_horiz_up);
  pos[1]=yTrack[2]-z_horiz_up;
  G4ThreeVector B1 = theBiotSavartIntegrator->BfieldFromLinearSegment(pos,0.5*d1, 0.5*d2, 0.5*Lhoriz_up);
  B += G4ThreeVector(B1.x(),0,B1.y());//go back to system y z -->Bz=By'
  // G4cout<<B<<" 3"<<std::endl;
  
  //Upper left corner;
   pos[1]=yTrack[1]-y_corner_left_up;
   pos[2]=yTrack[2]-z_corner_left_up; 
   B += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, halfpi,  halfpi , Rout_corner, Rin_corner, 0.5*d1);
   // G4cout<<B<<" 4"<<std::endl;
   
   //left vertical linear section;
 
   pos[1]=yTrack[1]-y_vert_left;
   pos[2]=yTrack[2]-z_vert_left;
   B = B- theBiotSavartIntegrator->BfieldFromLinearSegment(pos,0.5*d1, 0.5*d2, 0.5*Lvert_left);
   // G4cout<<B<<" 5"<<std::endl;
   
   //down left corner;
   pos[1]=yTrack[1]-y_corner_left_down;
   pos[2]=yTrack[2]-z_corner_left_down; 
   B += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, pi,  halfpi , Rout_corner, Rin_corner, 0.5*d1);
   // G4cout<<B<<" 6"<<std::endl;
   
   //down horizontal linear section; //we first work in the system where z' =-y and y'=z
   pos[2]=-(yTrack[1]-y_horiz_down);
   pos[1]=yTrack[2]-z_horiz_down;
   B1 = theBiotSavartIntegrator->BfieldFromLinearSegment(pos,0.5*d1, 0.5*d2, 0.5*Lhoriz_down);
   B = B - G4ThreeVector(B1.x(),0,B1.y());//go back to system y z -->Bz=By'
   // G4cout<<B<<" 7"<<std::endl;
   
   //down mid corner;
   pos[1]=yTrack[1]-y_corner_mid_down;
   pos[2]=yTrack[2]-z_corner_mid_down; 
   B += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, pi*1.5,  halfpi , Rout_corner, Rin_corner, 0.5*d1);
   // G4cout<<B<<" 8"<<std::endl;
   //mid vertical linear section;
   pos[1]=yTrack[1]-y_vert_mid;
   pos[2]=yTrack[2]-z_vert_mid;
   B += theBiotSavartIntegrator->BfieldFromLinearSegment(pos,0.5*d1, 0.5*d2, 0.5*Lvert_mid);
   // G4cout<<B<<" 9"<<std::endl;
   
   //mid mid corner;
   pos[1]=yTrack[1]-y_corner_mid_mid;
   pos[2]=yTrack[2]-z_corner_mid_mid; 
   B = B - theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, halfpi,  halfpi , Rout_corner, Rin_corner, 0.5*d1);
  // G4cout<<B<<" 10"<<std::endl;
   //mid horizontal linear section; //we first work in the system where z' =-y and y'=z
   pos[2]=-(yTrack[1]-y_horiz_mid);
   pos[1]=yTrack[2]-z_horiz_mid;
   B1 = theBiotSavartIntegrator->BfieldFromLinearSegment(pos,0.5*d1, 0.5*d2, 0.5*Lhoriz_mid);
   B = B - G4ThreeVector(B1.x(),0,B1.y());//go back to system y z -->Bz=By'
  // G4cout<<B<<" 11"<<std::endl;
   
   //mid right corner;
   pos[1]=yTrack[1]-y_corner_right_mid;
   pos[2]=yTrack[2]-z_corner_right_mid; 
   B += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, pi*1.5,  halfpi , Rout_corner, Rin_corner, 0.5*d1);
  // G4cout<<B<<" 12"<<std::endl;
   

   
   //still need to multiply by the factor mu_0/4./pi
   G4double fac=mu0*I/4./pi;
   B=B*fac;
   
   
   
   MagField[0]=B.x();
   MagField[1]=B.y();
   MagField[2]=B.z(); 
   
 
}
