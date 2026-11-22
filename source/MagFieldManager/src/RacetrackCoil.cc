#include"RacetrackCoil.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"special_functions.hh"
#include"globals.hh"
#include"BiotSavartIntegrator.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;
////////////////////////////////////////////////////////////////////////////////
//
RacetrackCoil::RacetrackCoil(G4double Rout,G4double D1, G4double D2,G4double L1,G4double L2,G4double current)//current turn from y to z
{ d1=D1;
  d2=D2;
  rout=Rout;
  rin=rout-d2*2.;
  l1=L1;
  l2=L2;
  I=current;
  y1=d2+l2+rin;
  y2=l2;
  z2=l1;
  z3=l1+rin+d2;
  y4=-y2;
  z4=z2;
  y5=-y1;
  y6=-y2;
  z6=-z2;
  z7=-z3;
  y8=y2;
  z8=-z2;
  
  
  theBiotSavartIntegrator = BiotSavartIntegrator::GetInstance();
} 
////////////////////////////////////////////////////////////////////////////////
//
RacetrackCoil::~RacetrackCoil()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void RacetrackCoil::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ 
  G4ThreeVector B = G4ThreeVector(0,0,0); 
 
  //first section;
  G4double pos[3];
  pos[0]=yTrack[0];
  pos[1]=yTrack[1]-y1;
  pos[2]=yTrack[2];
  B += theBiotSavartIntegrator->BfieldFromLinearSegment(pos,d1, d2, l1);
  
  
  //second section;
   pos[1]=yTrack[1]-y2;
   pos[2]=yTrack[2]-z2; 
   B += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, 0.,  halfpi , rout, rin, d1);
   
  //third section;  //we first work in the system where z' =-y and y'=z
   pos[1]=yTrack[2]-z3;
   pos[2]=-yTrack[1];
   G4ThreeVector B1 = theBiotSavartIntegrator->BfieldFromLinearSegment(pos,d1, d2, l2);
   B +=G4ThreeVector(B1.x(),0,B1.y()); //go back to system y z -->Bz=By'
  
  //4th section;
   pos[1]=yTrack[1]-y4;
   pos[2]=yTrack[2]-z4;

   B += theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, halfpi, halfpi, rout, rin, d1);
   
   //5th section
   pos[1]=yTrack[1]-y5;
   pos[2]=yTrack[2];
   B = B - theBiotSavartIntegrator->BfieldFromLinearSegment(pos,d1, d2, l1);
   
   //6th section
   pos[1]=yTrack[1]-y6;
   pos[2]=yTrack[2]-z6;
   B = B + theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, pi, halfpi, rout, rin, d1);
   
   //7th section  //we first work in the system where z' =y and y'=-z
   pos[1]=yTrack[2]-z7;
   pos[2]=-yTrack[1];
   B1 = theBiotSavartIntegrator->BfieldFromLinearSegment(pos,d1, d2, l2);
   B = B - G4ThreeVector(B1.x(),0,B1.y()); //go back to system y z -->Bz=-By'
   
   //8th section
   pos[1]=yTrack[1]-y8;
   pos[2]=yTrack[2]-z8;
   B = B + theBiotSavartIntegrator->BfieldFromCurvedSegmentInYZ(pos, pi*1.5, halfpi, rout, rin, d1);
   
   
   //still need to multiply by the factor mu_0/4./pi
   G4double fac=mu0*I/4./pi;
   B=B*fac;
   
   
   
   MagField[0]=B.x();
   MagField[1]=B.y();
   MagField[2]=B.z(); 
   
 
}
